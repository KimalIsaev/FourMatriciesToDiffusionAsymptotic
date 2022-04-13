#include "trapezoidal_rule.h"
#include <stdlib.h>

void
execute_at_every_point_trapezoidal_rule(
        double* result,
        double (*f)(double,void*),
        double a,
        double h,
        size_t n,
        void* additional_information_to_integration_function)
{
    double prev_x = a;
    double f_prev_x = f(prev_x,
        additional_information_to_integration_function);
    double S = 0;
    double next_x = a + h;
    double f_next_x;
    for(int i = 0; i < n; i++)
    {
        f_next_x = 
            f(next_x, additional_information_to_integration_function);
        S += (f_prev_x + f_next_x)/2;
        result[i] = S;
        f_prev_x = f_next_x;
        prev_x = next_x;
        next_x += h;
    }
    return result;
}


