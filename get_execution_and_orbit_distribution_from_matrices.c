#include "meschach/matrix.h"

VEC*
get_vector_full_of_ones(
        size_t n)
{
    VEC* result = v_get(n);    
    for(size_t i = 0; i < n; i++) {
            v_set_val(result, i, 1);
    }

    return result;
}

MAT*
get_matrix_from_array(
        const double* a, 
        size_t n)
{
    MAT* result = m_get(n,n);
    for(size_t i = 0; i < n; i++) {
        for(size_t j = 0; j < n; j++) {
            m_set_val(result, i, j, a[i + n*j]);
        }
    }
    return result;
}
/*
(
        MAT* A,
        MAT* B,
        MAT* K,
        MAT* I,
        double* x,
        size_t x_n)
{
    MAT** S_matrices = calloc(x_n, sizeof(MAT*));
    MAT* A_plus_B = m_add(A, B, MNULL);
    MAT* K_minus_I = m_sub(K, I, MNULL);
    MAT* x_to_K_minus_I;
    MAT* S_without_vector_of_ones_on_the_right;
    for(size_t x_i = 0; x_i < x_n; x_i++) {
    }


    M_FREE(A_plus_B);
    M_FREE(K_minus_I);
    M_FREE(temp_S);    

    return S_matrices;
}
*/
     

VEC* 
get_u(size_t n)
{
    VEC* result = v_set(n+1);
    for(size_t i = 0; i < n; i++) {
        v_set(result, i, 0);
    }
    v_set(result, i, 1);
}


double*
get_all_a(
        MAT* B, 
        MAT* I, 
        VEC** r,
        double* x,
        size_t x_n)
{
    double* result = calloc(x_n, sizeof(double*));
    VEC* e = get_vector_full_of_ones(B->n);
    VEC* Be = mv_mlt(B, e, VNULL);
    VEC* Ie = mv_mlt(B, e, VNULL);
    for(size_t x_i = 0; x_i < x_n; x_i++){
        result[x_i] = in_prod(r[x_i], Be) - x[x_i]*in_prod(r[x_i], Ie);
    }
    V_FREE(e);
    V_FREE(Be);
    V_FREE(Ie);
    return result;
}


double*
get_all_b(
        MAT* B,
        MAT* K
        MAT* I, 
        MAT** S,
        VEC** r,
        double* x,
        size_t x_n)
{
    VEC* g;
 
    = get_g
}

double*
get_p(
        MAT* B,
        MAT* K
        MAT* I, 
        MAT** S,
        VEC** r,
        double* x,
        size_t x_n)
{
    double* a = get_all_a(B, I, r, x, x_n);
    
}

void
set_execution_and_orbit_from_ABKI_and_x(
        VEC*** execution_distribution,
        double** orbit_distribution,
        MAT* A,
        MAT* B,
        MAT* K,
        MAT* I,
        double* x,
        size_t x_n)
{
    MAT** g_inverses_of_S;
    VEC** r;
    double* p;

    g_inverses_of_S = get_g_inverses_of_S_from_ABKI_x(A, B, K, I, x, x_n);
    r = get_r_vectors_from_g_iverses_of_S(g_inverses_of_S, x_n);
    p = get_p(B, I, g_inverses_of_S, r, x, x_n));
    
    *execution_distribution = r;
    *orbit_distribution = p;
    return error_code;
}

void
set_matrices_ABKI_from_given_arrays(
        MAT* A,
        MAT* B,
        MAT* K,
        MAT* I,
        const double* A_given,
        const double* B_given,
        const double* K_given,
        const double* I_given,
        size_t n)
{
    A = get_matrix_from_array(A_given, n);
    B = get_matrix_from_array(B_given, n);
    K = get_matrix_from_array(K_given, n);
    I = get_matrix_from_array(I_given, n);
}

void
set_x_from_sup_and_delta(
        double** x, 
        size_t* x_n,
        double sup_x,
        double delta_x)
{
    size_t temp_n = ceil(sup_x / delta_x);
    double* temp_x = calloc(x_n, sizeof(double));
    for(size_t i = 0; i < temp_n; i++) {
        temp_x[i] =  delta_x*i;
    }

    return SUCCESS;
}

double*
get_execution_distribution_to_give_out(
        VEC** execution_distribution,
        size_t x_n)
{
    size_t v_n = (*execution_distribution)->dim;
    double* result = calloc(v_n*x_n, sizeof(double));
    for(size_t x_i = 0; x_i < x_n; x_i++) {
        for(size_t v_i = 0; v_i < v_n; v_n++) {
            result[x_i*v_n + v_i] = execution_distribution[x_i]->ve[v_i];
        }
    }
    return result;
}

int 
set_execution_and_orbit_distribution_from_matrices(
        double* execution_distribution_to_give,
        double* orbit_distribution_to_give,
        const double* A_given,
        const double* B_given,
        const double* K_given, 
        const double* I_given,
        long long n,
        double sup_x,
        double delta_x)
{
    error_code_t error_code = SUCCESS;
    
    MAT* A;
    MAT* B;
    MAT* K;
    MAT* I;

    size_t x_n;
    double* x;
    
    VEC** execution_distribution;
    double* orbit_distribution;

    set_matrices_ABKI_from_given_arrays(A, B, K, I,
        A_given, B_given, K_given, I_given, n);
    set_x_from_sup_and_delta(&x, &x_n, sup_x, delta_x)); 

    set_execution_and_orbit_from_ABKI_and_x(
        &execution_distribution, 
        &orbit_distribution,
        A, B, K, I, x, x_n);

    execution_distribution_to_give = 
        get_execution_distribution_to_give_out(execution_distribution, x_n);

finalization:
    M_FREE(A);
    M_FREE(B);
    M_FREE(K);
    M_FREE(I);
    for(size_t i = 0; i < x_n; i++) {
        V_FREE(execution_distribution[i]);
    }
    free(x);
    free(execution_distribution);

    return error_code;
}


