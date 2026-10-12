#include <stdio.h>
#include "ngquery.h"
#include "difeq.h"
#include <stdlib.h>
#include <math.h>

int main(){
    int steps = config(); 
    double current = 0; 
    double timestep = 0.000001; 
    double time = 0; 
    int incorrect_vals = 0; 
    printf("steps %d\n", steps); 
    for(int i = 0; i < steps; i++){
        current = iterate_difeq(get_vin(time), timestep, 100, 0.010, 0); 
        time += timestep; 
        double expected_current = get_current(time-timestep); 
        if(i < 10){
            printf("step %d: expected == %f, real == %f\n", i,expected_current, current); 
        }
    
        if((fabs(expected_current - current) > (expected_current * 0.01)) && current != 0 && (incorrect_vals > 6)){
            printf("incorrect value at time %f, expected: %f, real %f\n", time, expected_current, current); 
            return 0; 
        }
        else{
            incorrect_vals++; 
        }
        
    }
    printf("all tests passed\n"); 

}