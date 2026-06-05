#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "matrix.h"
#include "matrix_storage.h"

#define EPSILON 1e-9

void test_int_matrix_creation() {
    field_info_t* fi = get_int_field_info();
    matrix_t* m = matrix_create(fi, 2);
    assert(m != NULL);
    assert(m->size == 2);
    assert(m->field_info == fi);
    
    int val = 42;
    matrix_set_element(m, 0, 0, &val);
    
    int res;
    matrix_get_element(m, 0, 0, &res);
    assert(res == 42);
    
    matrix_destroy(m);
    printf("[PASS] test_int_matrix_creation\n");
}

void test_double_matrix_ops() {
    field_info_t* fi = get_double_field_info();
    matrix_t* m1 = matrix_create(fi, 2);
    matrix_t* m2 = matrix_create(fi, 2);

    double v1[] = {1.0, 2.0, 3.0, 4.0};
    double v2[] = {5.0, 6.0, 7.0, 8.0};

    for(int i=0; i<2; ++i)
        for(int j=0; j<2; ++j) {
            matrix_set_element(m1, i, j, &v1[i*2+j]);
            matrix_set_element(m2, i, j, &v2[i*2+j]);
        }

    // Test Add
    matrix_t* sum = matrix_add(m1, m2);
    assert(sum != NULL);
    double s_val;
    matrix_get_element(sum, 0, 0, &s_val);
    assert(fabs(s_val - 6.0) < EPSILON);
    matrix_destroy(sum);

    // Test Multiply
    matrix_t* prod = matrix_multiply(m1, m2);
    assert(prod != NULL);
    double p_val;
    matrix_get_element(prod, 0, 0, &p_val);
    // 1*5 + 2*7 = 19
    assert(fabs(p_val - 19.0) < EPSILON);
    matrix_destroy(prod);

    // Test Scalar Mul
    double scalar = 2.0;
    matrix_t* sc = matrix_scalar_mul(m1, &scalar);
    assert(sc != NULL);
    matrix_get_element(sc, 1, 1, &p_val);
    assert(fabs(p_val - 8.0) < EPSILON);
    matrix_destroy(sc);

    matrix_destroy(m1);
    matrix_destroy(m2);
    printf("[PASS] test_double_matrix_ops\n");
}

void test_storage() {
    matrix_storage_t store;
    matrix_storage_init(&store);
    
    field_info_t* fi = get_int_field_info();
    matrix_t* m = matrix_create(fi, 1);
    int v = 10;
    matrix_set_element(m, 0, 0, &v);

    error_code_t err = matrix_storage_add(&store, "TestMat", m);
    assert(err == ERR_OK);
    assert(store.amount == 1);
    
    // Storage takes ownership, so we don't destroy m here manually if implementation is correct
    // But based on typical C patterns, let's assume storage copies pointer or owns it.
    // If storage_destroy frees matrices, this is fine.
    
    matrix_storage_destroy(&store);
    printf("[PASS] test_storage\n");
}

int main() {
    test_int_matrix_creation();
    test_double_matrix_ops();
    test_storage();
    
    printf("\nAll tests passed!\n");
    return 0;
}