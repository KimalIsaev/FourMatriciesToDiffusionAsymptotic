#include "get_rab_from_constans_and_x.h"
#include "error_handle_library.h"
#include "Meschach/matrix.h"
#include "Meschach/matrix2.h"

struct constants_for_calculating_rab {
    MAT* B;
    MAT* K;
    MAT* I;
    MAT* A_plus_B;
    MAT* K_minus_I;
    VEC* Be;
    VEC* Ie;
    VEC* u;
    VEC* e;
    size_t n;
};

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

VEC*
concatenate_number_to_vector_end(
        const VEC* v,
        double number)
{
    VEC* result = v_get(v->dim + 1);
    for(size_t i = 0; i < v->dim; i++) {
            v_set_val(result, i, v_get_val(v, i));
    }
    v_set_val(result, v->dim, number);
    return result;
}

MAT*
concatenate_vector_to_matrix_right(
        const MAT* m,
        const VEC* v)
{
    size_t matrix_rows = m->n;
    size_t matrix_cols = m->m;
    size_t vector_size = v->dim;
    CONDITION_RETURN(vector_size != matrix_rows, NULL);

    MAT* result = m_get(matrix_rows, matrix_cols + 1);
    for(size_t i = 0; i < matrix_rows; i++) {
        for(size_t j = 0; j < matrix_cols; j++) {
            m_set_val(result, i, j, m_get_val(m, i, j));
        }
    }

    for(size_t i = 0; i < matrix_rows; i++) {
            m_set_val(result, i, matrix_cols, v_get_val(v, i));
    }

    return result;
}

MAT*
get_right_g_inverse(
        const MAT* matrix)
{
    MAT* matrix_transposed = m_transp(matrix, MNULL);
    MAT* matrix_to_matrix_transposed = 
        m_mlt(matrix, matrix_transposed, MNULL);
    MAT* inverse_of_matrix_to_matrix_transposed = 
        m_inverse(matrix_to_matrix_transposed, MNULL);
    MAT* result = m_mlt(matrix_transposed, 
        inverse_of_matrix_to_matrix_transposed, MNULL);

    M_FREE(matrix_transposed);
    M_FREE(matrix_to_matrix_transposed);
    M_FREE(inverse_of_matrix_to_matrix_transposed);
    return result;
}

MAT*
get_S(
        const MAT* A_plus_B,
        const MAT* K_minus_I,
        const VEC* e,
        double x)
{
    MAT* x_to_K_minus_I = sm_mlt(x, K_minus_I, MNULL);
    MAT* S_without_vector_of_ones_on_the_right =
         m_add(A_plus_B, x_to_K_minus_I, MNULL);
    MAT* result = concatenate_vector_to_matrix_right(
        S_without_vector_of_ones_on_the_right, e);

    M_FREE(x_to_K_minus_I);
    M_FREE(S_without_vector_of_ones_on_the_right);
    return result;
}

VEC*
get_u(size_t n)
{
    size_t i;
    VEC* result = v_get(n+1);
    for(i = 0; i < n; i++) {
        v_set_val(result, i, 0);
    }
    v_set_val(result, i, 1);
    return result;
}

MAT*
get_inverse_of_S(
        const MAT* A_plus_B,
        const MAT* K_minus_I,
        const VEC* e,
        double x)
{
    MAT* S = get_S(A_plus_B, K_minus_I, e, x);
    MAT* result = get_right_g_inverse(S);

    M_FREE(S);
    return result;    
}

VEC*
get_r(
        const MAT* g_inverse_of_S,
        const VEC* u)
{
    return vm_mlt(g_inverse_of_S, u, VNULL);
}

double
get_a(
        const VEC* Be,
        const VEC* Ie,
        const VEC* r,
        double x)
{
    return in_prod(r, Be) - x*in_prod(r, Ie);
    //return sv_mlt(x, Ie, VNULL);
}

VEC*
get_g(
        const MAT* g_inverse_of_S,
        const MAT* B,
        const MAT* K,
        const VEC* r,
        double a,
        double x)
{
    MAT* xK = sm_mlt(x, K, MNULL);
    MAT* xK_minus_B = m_sub(xK, B, MNULL);
    VEC* xrK_minus_rB = vm_mlt(xK_minus_B, r, VNULL);
    VEC* ar = sv_mlt(a, r, VNULL);
    VEC* ar_plus_xrK_minus_rB = 
        v_add(ar, xrK_minus_rB, VNULL);
    VEC* ar_plus_xrK_minus_rB_with_concatenated_zero = 
        concatenate_number_to_vector_end(ar_plus_xrK_minus_rB, 0);
    VEC* result = vm_mlt(g_inverse_of_S, 
        ar_plus_xrK_minus_rB_with_concatenated_zero, VNULL);

    M_FREE(xK);
    M_FREE(xK_minus_B);
    V_FREE(xrK_minus_rB);
    V_FREE(ar);
    V_FREE(ar_plus_xrK_minus_rB);
    V_FREE(ar_plus_xrK_minus_rB_with_concatenated_zero);
    return result;
}

double
get_b(
        const MAT* g_inverse_of_S,
        const MAT* B,
        const MAT* K,
        const MAT* I,
        const VEC* e,
        const VEC* r,
        double a,
        double x)
{
    VEC* g = get_g(g_inverse_of_S, B, K, r, a, x);
    MAT* xI = sm_mlt(x, I, MNULL);
    MAT* B_minus_xI = m_sub(B, xI, MNULL);
    VEC* g_to_B_minus_xI = vm_mlt(B_minus_xI, g, VNULL);
    VEC* xrI = vm_mlt(xI, r, VNULL);
    VEC* g_to_B_minus_xI_all_plus_xrI = v_add(
        g_to_B_minus_xI, xrI, VNULL);
    double result = 
        2*in_prod(g_to_B_minus_xI_all_plus_xrI, e) + a;
    
    V_FREE(g);
    M_FREE(xI);
    M_FREE(B_minus_xI);
    V_FREE(g_to_B_minus_xI);
    V_FREE(xrI);
    V_FREE(g_to_B_minus_xI_all_plus_xrI);
    return result;
}

void 
set_rab_from_constants_for_rab_and_x(
        double* r,
        double* a,
        double* b,
        const void* constants,
        double x)
{
    struct constants_for_calculating_rab* c =
        (struct constants_for_calculating_rab*) constants;    
    MAT* g_inverse_of_S = get_inverse_of_S(
        c->A_plus_B, c->K_minus_I, c->e, x);
    VEC* r_temp = get_r(g_inverse_of_S, c->u);
    double a_temp = get_a(c->Be, c->Ie, r_temp, x);
    double b_temp = get_b(g_inverse_of_S, c->B, c->K, c->I,
        c->e, r_temp, a_temp, x);
    
    for (size_t i = 0; i < c->n; i++) {
        r[i] = v_get_val(r_temp, i);
    }
    *a = a_temp;
    *b = b_temp;
    M_FREE(g_inverse_of_S);
    V_FREE(r_temp);
}

MAT*
get_matrix_from_array(
        const double* a,
        size_t n)
{
    MAT* result = m_get(n,n);
    for(size_t i = 0; i < n; i++) {
        for(size_t j = 0; j < n; j++) {
            m_set_val(result, i, j, a[n*i + j]);
        }
    }
    return result;
}

void* get_constants_for_rab_from_ABKI(
    double* A_given,
    double* B_given,
    double* K_given,
    double* I_given,
    size_t n)
{
    struct constants_for_calculating_rab* c = 
        malloc(sizeof(struct constants_for_calculating_rab)); 
    MAT* A = get_matrix_from_array(A_given, n);
    MAT* B = get_matrix_from_array(B_given, n);
    MAT* K = get_matrix_from_array(K_given, n);
    MAT* I = get_matrix_from_array(I_given, n);
    VEC* e = get_vector_full_of_ones(n);
    VEC* u = get_u(n);
    c->B = B;
    c->K = K;
    c->I = I;
    c->A_plus_B = m_add(A, B, MNULL);
    c->K_minus_I = m_sub(K, I, MNULL);
    c->Be = mv_mlt(B, e, VNULL);
    c->Ie = mv_mlt(I, e, VNULL);
    c->u = u;
    c->e = e;
    c->n = n;
    
    M_FREE(A);
    return (void*)c;
}

void free_constants_for_rab(
        void* constants)
{
    struct constants_for_calculating_rab* c =
        (struct constants_for_calculating_rab*) constants;
    M_FREE(c->B);
    M_FREE(c->K);
    M_FREE(c->I);
    M_FREE(c->A_plus_B);
    M_FREE(c->K_minus_I);
    V_FREE(c->Be);
    V_FREE(c->Ie);
    V_FREE(c->u);
    V_FREE(c->e);
    free(c);
}

