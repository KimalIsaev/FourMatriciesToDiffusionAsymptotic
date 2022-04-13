#include "get_rab_from_constans_and_x.h"

struct additional_information_for_b_divided_by_a{
    void* consts_for_rab;
    double* bs;
    double* as;
    double* rs;
    size_t r_size;
    size_t i;
};

double
execute_b_divided_by_a(
        double x,
        void* not_converted_inf)
{
    struct additional_information_for_b_divided_by_a* inf =
        (struct additional_information_for_b_divided_by_a*)not_converted_inf;
    double a;
    double b;
    set_rab_from_constants_for_rab_and_x(&(inf->rs[i*r_size]),
        &a, &b, inf->consts_for_rab, x); 
    inf->as[i] = a;
    inf->bs[i] = b;
    i+=1;
    return b / a;
}

struct additional_information_for_b_divided_by_a
get_additional_information(
        double* as,
        double* bs,
        double* rs,
        double* A,
        double* B,
        double* K,
        double* I,
        size_t n)
{
    struct additional_information_for_b_divided_by_a result =
    result.consts_for_rab =
        get_constants_for_rab_from_ABKI(A, B, K, I, n);
    result.bs = bs;
    result.as = as;
    result.rs = rs;
    result.r_size = n;
    result.i = 0;
    return result;
{

void
charge_a_b_integral(
        double* a_b_integral,
        double* a, 
        double* b,
        double* r,
        double* A, 
        double* B, 
        double* K, 
        double* I,
        size_t n, 
        size_t x_n, 
        double x_delta)
{
    struct additional_information_for_b_divided_by_a inf =
        get_additional_information(a, b, r, A, B, K, I, n);
    execute_at_every_point_trapezoidal_rule(a_b_integral,
        execute_b_divided_by_a, 0, x_delta, x_n, &inf);
    free_consnats_for_rab(inf.consts_for_rab);
}

