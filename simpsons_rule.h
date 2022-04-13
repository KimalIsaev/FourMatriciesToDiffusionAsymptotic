#ifndef SIMSONS_RULE_H
#define SIMSONS_RULE_H

#define N_GIVEN_POINTS_MINUS_N_INTEGRATED_POINTS_IN_SIMPSONS_RULE 

double*
execute_at_every_point_simpsons_rule(double (*f)(double,void*),
    double a, double h, size_t n, 
    void* additional_information_to_integration_function);

#endif /* SIMSONS_RULE_H */
