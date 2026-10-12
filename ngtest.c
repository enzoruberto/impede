#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ngspice/sharedspice.h>
#include <math.h>


vector_info time_vector;
vector_info vin_vector;
vector_info vl_vector;
float timescale = (1000000); 


/*
 * Receives ngspice console messages.
 */
static int receive_char(char *message, int id, void *user_data)
{
    (void)id;
    (void)user_data;

    fprintf(stderr, "%s\n", message);
    return 0;
}

/*
 * Ngspice requires a controlled-exit callback.
 */
static int controlled_exit(
    int status,
    NG_BOOL immediate,
    NG_BOOL quit_exit,
    int id,
    void *user_data
) {
    (void)immediate;
    (void)quit_exit;
    (void)id;
    (void)user_data;

    fprintf(stderr, "ngspice exit status: %d\n", status);
    return 0;
}


double get_VL( double time ){
    if(!vl_vector.v_realdata){
        return -1; 
    }
    return vl_vector.v_realdata[llround((time * timescale))]; 
}


double get_vin( double time ){
    if(!vin_vector.v_realdata){
        return -1; 
    }
    return vin_vector.v_realdata[llround((time * timescale))]; 
}

double get_current(double time){
    if(!vin_vector.v_realdata || !vl_vector.v_realdata){
        return -1; 
    }
    return (get_vin(time) - get_VL(time)) / 100.0; 
}


int config(void)
{    
    /*
     * Each array entry is one line of the SPICE netlist.
     * The final entry must be NULL.
     */
    char *circuit[] = {
        "* Series R-L circuit",
        "Vsource vin 0 PULSE(0 5 0 1n 1n 1m 2m)",
        "R1 vin vl 100",
        "L1 vl 0 10m",
        ".save v(vin) v(vl)",
        ".end",
        NULL
    };

    // char *circuit[] = {
    //     "V1 N001 0 PULSE(0 5 0 1n 1n 1ms 2ms)",
    //     "R1 N002 N001 10k",
    //     "L1 N002 0 10m",
    //     ".tran 1u 5s",
    //     ".control",
    //     "run",
    //     "",
    //     "wrdata results.txt v(N002) i(N002)",
    //     ".endc",
    //     ".end",
    //     NULL
    // };

    // char *circuit[] = {
    //     "* Series R-L circuit",
    //     "Vsource vin 0 SINE(0 5 100k)",
    //     "R1 vin vl 100",
    //     "L1 vl 0 10m",
    //     ".save v(vin) v(vl)",
    //     ".end",
    //     NULL
    // };

    /*
     * Initialize the shared ngspice library.
     */
    ngSpice_Init(
        receive_char,       /* Console output callback */
        NULL,               /* Simulation status callback */
        controlled_exit,    /* Controlled-exit callback */
        NULL,               /* Per-time-point data callback */
        NULL,               /* Vector initialization callback */
        NULL,               /* Background-thread callback */
        NULL                /* User data */
    );

    /*
     * Load the circuit.
     */
    if (ngSpice_Circ(circuit) != 0) {
        fprintf(stderr, "Failed to load circuit\n");
        return EXIT_FAILURE;
    }

    /*
     * Run synchronously. This call returns when the simulation finishes.
     */
    if (ngSpice_Command("tran 1u 4m") != 0) {
        fprintf(stderr, "Transient simulation failed\n");
        return EXIT_FAILURE;
    }
    if (ngSpice_Command("linearize v(vin) v(vl)") != 0) {
        fprintf(stderr, "Could not resample transient results\n");
        return EXIT_FAILURE;
    }
    /*
     * Query the completed simulation vectors.
     */
    // pvector_info time_vector = ngGet_Vec_Info("time");
    // pvector_info vin_vector  = ngGet_Vec_Info("v(vin)");
    // pvector_info vl_vector   = ngGet_Vec_Info("v(vl)");

    vector_info time_info, vin_info, vl_info;
    pvector_info result;

    result = ngGet_Vec_Info("time");
    if (!result) return EXIT_FAILURE;
    time_vector = *result;

    result = ngGet_Vec_Info("v(vin)");
    if (!result) return EXIT_FAILURE;
    vin_vector = *result;

    result = ngGet_Vec_Info("v(vl)");
    if (!result) return EXIT_FAILURE;
    vl_vector = *result;

    if (time_vector.v_realdata == NULL ||
        vin_vector.v_realdata == NULL ||
        vl_vector.v_realdata == NULL) {
        fprintf(stderr, "Could not find one or more result vectors\n");
        return EXIT_FAILURE;
    }

    int points = time_vector.v_length;

    if (vin_vector.v_length < points)
        points = vin_vector.v_length;

    if (vl_vector.v_length < points)
        points = vl_vector.v_length;

    return points;
}

int print(){
    FILE *file_ptr;
    file_ptr = fopen("example.txt", "w");
    fprintf(file_ptr, "time,vin,v_inductor,current\n");
     if (file_ptr == NULL) {
        printf("Error: Could not open file.\n");
        return 1;
    }
     int points = time_vector.v_length;

    if (vin_vector.v_length < points)
        points = vin_vector.v_length;

    if (vl_vector.v_length < points)
        points = vl_vector.v_length;


    for (int i = 0; i < points; i++) {
        double time       = time_vector.v_realdata[i];
        double vin        = vin_vector.v_realdata[i];
        double v_inductor = vl_vector.v_realdata[i];

        double current = (vin - v_inductor) / 100.0;
        // fprintf(file_ptr, "%.9e\n", vin); 
        fprintf(file_ptr,
            "%.9e,%.9e,%.9e,%.9e\n",
            time,
            vin,
            v_inductor,
            current
        );
    }
    fclose(file_ptr);
    return 0; 
}