#ifndef GET_RAB_FROM_CONSTANS_AND_X_H
#define GET_RAB_FROM_CONSTANS_AND_X_H

#include <stddef.h>
#include <stdint.h>

void* get_constants_for_rab_from_ABKI(
    double* A, double* B, double* K, double* I, size_t n);

void set_rab_from_constants_for_rab_and_x(double* r, double* a, double* b,
    const void* c, double x);

void free_constants_for_rab(void*);

#endif /* GET_RAB_FROM_CONSTANS_AND_X_H */

