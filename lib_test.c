#include "get_rab_from_constans_and_x.h"

const size_t n = 2;
const size_t n_squared = n*n;
const double lambda;
const double sigma = 0.0000001;
const double mu;
const double r;
const double x_end = 10;
const size_t x_n = x_end/sigma;

double*
get_A()
{
    double* result = malloc(n_squared);
    result[0] = -lambda;
    result[1] = lambda;
    result[2] = mu*(1-r);
    result[3] = -(mu+lambda);
    return result;
}

double*
get_B()
{
    double* result = malloc(n_squared);
    result[0] = 0;
    result[1] = 0;
    result[2] = mu*r;
    result[3] = lambda;
    return result;
}

double* 
get_K()
{
    double* result = malloc(n_squared);
    K[0] = 0;
    K[1] = 1;
    K[2] = 0;
    K[3] = 0; 
    return result;
}

double* 
get_I()
{
    double* result = malloc(n_squared);
    I[0] = 1;
    I[1] = 0;
    I[2] = 0;
    I[3] = 0;
    return result;
}

double*
a_b_to_pi(
        double* a,
        double* b,
        double* b_divided_by_a_integrated)
{
    
    return result;
}

int main() 
{
    double* r = malloc(n_squared*x_n);
    double* a = malloc(x_n);
    double* b = malloc(x_n);
    double* a_divided_by_b = malloc(x_n);
    charge_a_b_integral(a_divided_by_b, a, b, r,
        A, B, K, I, n, x_n, sigma);
    double* all_differences =
    calculate_percisly(x);

}
