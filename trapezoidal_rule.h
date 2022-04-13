#ifndef TRAPEZOIDAL_RULE_H
#define TRAPEZOIDAL_RULE_H

#include <stdint.h>
#include <stddef.h>

void
execute_at_every_point_trapezoidal_rule(double* result,
    double (*f)(double,void*),
    double a, double h, size_t n, 
    void* additional_information_to_integration_function);

#endif /* TRAPEZOIDAL_RULE_H */
