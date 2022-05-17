#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "four_matricies_to_diffusion_asymptotic.h"
#include "trapezoidal_rule.h"

#define n 2
#define n_squared (n*n)
#define x_end 20
#define percision 0.01
#define x_n (lround(ceil(x_end/percision)))
#define n_of_states (n*x_n)
const double lambda = 1;
const double sigma = percision;
const double mu = 2.1;
const double r = 0.5;
/*
double difference_between_one_and_sum(
        const double* probabilities)
{
    double sum = 0;
    for (size_t i = 0; i < n_of_states; i++) {
        sum += probabilities[i];
    }
    return fabs(1 - sum);
}
*/

void print_array_with_indices(
        const double* a,
        size_t a_n)
{
    for (size_t i = 0; i < a_n; i++) {
        printf("%d:%E ", i, a[i]);
    }
}

void print_array(
        const double* a,
        size_t a_n)
{
    for (size_t i = 0; i < a_n; i++) {
        printf("%E ", a[i]);
    }
}

double sum_of_all_propabilities(
        const double* probabilities)
{
    double sum = 0;
    for (size_t i = 0; i < x_n; i++) {
        sum += probabilities[i];
    }
    return sum;
}

void
normalize_distribution(
        double* probabilities)
{
    double S = sum_of_all_propabilities(probabilities);
    for (size_t i = 0; i < x_n; i++) {
        probabilities[i] /= S;
    } 
}

double kolmogorov_distance(
        const double* result_a,
        const double* result_b,
        size_t kolmogorov_n) 
{
    double max_difference = 0;
    size_t max_i = 0; 
    double temp;
    printf("first diff: %f\n", fabs(result_a[0] - result_b[0]));
    for (size_t i = 0; i < kolmogorov_n; i++) {
        temp = fabs(result_a[i] - result_b[i]);
        if (temp > max_difference) {
            max_i = i;
            max_difference = temp; 
        }
    }
    printf("max_i for kolmogorov: %d\n", max_i);
    return max_difference;
}

void
charge_percise_r(
        double* result)
{
    double cur_x = 0;
    for (size_t i = 0; i < x_n; i++) {
        result[2*i] = mu/(lambda + mu + cur_x);
        result[2*i + 1] = 1- result[2*i];
        cur_x += percision;
    }
}

void 
charge_percise_a_from_r(
        double* result,
        double* r_array)
{
    double cur_x = 0;
    for (size_t i = 0; i < x_n; i++) { 
        result[i] = (r*mu + lambda)*r_array[2*i + 1] 
            - cur_x*r_array[2*i];
        cur_x += percision;
    }
}

void 
charge_percise_orbit_distribution(
        double* solution)
{
    const double alpha = mu*(1-r)/lambda;
    const double beta = (lambda + mu*r)/sigma;
    const double start_element = pow(1 - (1/alpha), beta + 1);
    double  P0[x_n];
    double  P1[x_n];
    size_t i = 0;
    size_t k = 1;
    P0[0] = start_element;
    P1[0] = start_element / alpha;
    solution[0] = P0[0] + P1[0];
    while (k < x_n) {
        P0[k] = P0[i]*(beta+i)/(k*alpha);
        P1[k] = P1[i]*(beta+k)/(k*alpha);
        solution[k] = P0[k] + P1[k];
        i = k++;
    }
}

void 
charge_percise_solution(
        double* solution)
{
    const double alpha = mu*(1-r)/lambda;
    const double beta = (lambda + mu*r)/sigma;
    const double start_element = pow(1 - (1/alpha), beta + 1);
    double*  P0 = solution;
    double*  P1 = &(solution[x_n]);
    size_t i = 0;
    size_t k = 1;
    P0[0] = start_element;
    P1[0] = start_element / alpha;
    while (k < x_n) {
        P0[k] = P0[i]*(beta+i)/(k*alpha);
        P1[k] = P1[i]*(beta+k)/(k*alpha);
        i = k++;
    }
}

/*
void 
charge_percise_solution(
        double* solution)
{
    const double alpha = mu*(1-r)/lambda;
    const double beta = (lambda + mu*r)/sigma;
    const double start_element = pow(1 - (1/alpha), beta + 1);
    size_t i = 0;
    size_t k = 1;
    solution[0] = start_element;
    solution[1] = start_element / alpha;
    while (i < n) {
        solution[2*k] = solution[2*i]*(beta+i)/(k*alpha);
        solution[2*k + 1] = solution[2*i + 1]*(beta+k)/(k*alpha);
        i = k++;
    }
}
*/

void
charge_A(
        double* A)
{
    A[0] = -lambda;
    A[1] = lambda;
    A[2] = mu*(1-r);
    A[3] = -(mu+lambda);
}

void
charge_B(
        double* B)
{
    B[0] = 0;
    B[1] = 0;
    B[2] = mu*r;
    B[3] = lambda;
}

void 
charge_K(
        double* K)
{
    K[0] = 0;
    K[1] = 1;
    K[2] = 0;
    K[3] = 0; 
}

void
charge_I(
        double* I)
{
    I[0] = 1;
    I[1] = 0;
    I[2] = 0;
    I[3] = 0;
}
/*
void
r_a_b_to_approximate_result(
        double* result,
        const double* r,
        const double* a,
        const double* b,
        const double* b_divided_by_a_integrated)
{
    double not_normalized[n_of_states];
    double temp_pi;
    double S = 0;
    for (size_t i = 0; i < x_n; i++) {
        temp_pi = exp(2*b_divided_by_a_integrated[i]/sigma)/b[i];
        not_normalized[i] = r[2*i]*temp_pi;
        not_normalized[2*i] = r[2*i+1]*temp_pi;
        S += not_normalized[i];
    }
    for (size_t i = 0; i < n_of_states; i++) {
        result[i] = not_normalized[i] / S;
    }
}
*/

void
to_approximate_orbit_distribution(
        double* result,
        const double* b,
        const double* b_divided_by_a_integrated)
{
    for (size_t i = 0; i < x_n; i++) {
        result[i] = 
            exp(2*b_divided_by_a_integrated[i]/sigma)/b[i];
    }
}

/*
void
charge_approximate_solution(
        double* result)
{
    double percise_r_array[n_of_states];
    double percise_a[x_n];

    double A[n_squared];
    double B[n_squared];
    double K[n_squared];
    double I[n_squared];
    double a[x_n];
    double b[x_n];
    double a_b_integral[x_n];
    double r_array[n_of_states];

    charge_percise_r(percise_r_array);
    charge_percise_a_from_r(percise_a, percise_r_array);

    charge_A(A);
    charge_B(B);
    charge_K(K);
    charge_I(I);
    charge_a_b_integral(a_b_integral, a, b, r_array, 
        A, B, K, I, n, x_n, percision);
    r_a_b_to_approximate_result(result, r_array, a, b,
        a_b_integral);
    normalize_distribution(result);
    printf("difference for r: %f\n", 
        kolmogorov_distance(r_array, percise_r_array, n_of_states));
    printf("difference for a: %f\n", 
        kolmogorov_distance(a, percise_a, x_n));
}
*/
void
charge_approximate_orbit_distribution(
        double* result)
{
    double percise_r_array[n_of_states];
    double percise_a[x_n];

    double A[n_squared];
    double B[n_squared];
    double K[n_squared];
    double I[n_squared];
    double a[x_n];
    double b[x_n];
    double a_b_integral[x_n];
    double r_array[n_of_states];

    charge_percise_r(percise_r_array);
    charge_percise_a_from_r(percise_a, percise_r_array);

    charge_A(A);
    charge_B(B);
    charge_K(K);
    charge_I(I);
    charge_a_b_integral(a_b_integral, a, b, r_array, 
        A, B, K, I, n, x_n, percision);
    to_approximate_orbit_distribution(result, b, a_b_integral);
    normalize_distribution(result);
    printf("difference for r: %f\n", 
        kolmogorov_distance(r_array, percise_r_array, n_of_states));
    printf("difference for a: %f\n", 
        kolmogorov_distance(a, percise_a, x_n));
    //printf("a:\n");
    //print_array_with_indices(a, x_n);
    //printf("\n");
    //printf("b:\n");
    //print_array_with_indices(b, x_n);
    //printf("\n");
    //printf("a_b_integral:\n");
    //print_array(a_b_integral, x_n);
    //printf("\n");
}

double identity(double x, void* c)
{
    return x;
}

double one(double x, void* c)
{
    return 1;
}

int main() 
{
    double approximate_orbit_distribution[x_n];        
    double percise_orbit_distribution[x_n];
    
    charge_approximate_orbit_distribution(approximate_orbit_distribution);
    charge_percise_orbit_distribution(percise_orbit_distribution);
    //printf("Approx:\n");
    //print_array_with_indices(approximate_orbit_distribution, x_n);
    //printf("\n");
    //printf("Percise:\n");
    //print_array(percise_orbit_distribution, x_n);
    //printf("\n");
    printf("difference for distribution: %f\n", 
        kolmogorov_distance(
            approximate_orbit_distribution, 
            percise_orbit_distribution, x_n));
    printf("sum of approximate distribution: %f\n", 
        sum_of_all_propabilities(approximate_orbit_distribution));
/*
    double test[x_n];
    execute_at_every_point_trapezoidal_rule(
        test, one, 0, percision, x_n , NULL);
    printf("test_integration: %f\n", test[x_n-1]);
*/

    return 0;
}
