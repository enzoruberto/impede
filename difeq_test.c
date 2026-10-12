#include <stdio.h> 
#include <math.h> 


//equation is i(t) = i(0)e^(-Rt/L) + ((e^-Rt)/L)int(0->t)(e^Ru/l)*v(t)du

double iterate_difeq(double current_voltage, double timestep, double R, double L, int reset){
    static double current = 0.0;  
    current = reset ? 0 : current; 
    if (R == 0.0) {
        current += current_voltage * timestep / L;
    } else {
        double weight = 1.0 - exp(-R * timestep / L);
        current += weight * (current_voltage / R - current);
    }
    return current;
}