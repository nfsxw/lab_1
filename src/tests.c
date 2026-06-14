#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "matrix.h"
#include "int_field_info.h"
#include "double_field_info.h"
#include "tests.h"

static void print_test_header(const char *name)
{
    printf("\n===== %s =====\n", name);
}

static void print_subtest(const char *desc, int passed)
{
    printf("%-60s [%s]\n", desc, passed ? "PASS" : "FAIL");
}

static void test_int_matrix_creation()
{
    print_test_header("INT: Matrix creation");
    field_info_t *info = get_int_field_info();
    matrix_t *m = matrix_create(info, 3);
    print_subtest("Create 3x3 int matrix", m != NULL);
    if (m) {
        print_subtest("Size = 3", m->size == 3);
        matrix_free(m);
    }
}

static void test_int_set_get()
{
    print_test_header("INT: Set/Get element");
    field_info_t *info = get_int_field_info();
    matrix_t *m = matrix_create(info, 2);
    int val = 42;
    matrix_set(m, 0, 0, &val);
    int *got = (int*)matrix_get(m, 0, 0);
    print_subtest("Set (0,0)=42, get returns 42", got && *got == 42);
    matrix_free(m);
}

static void test_int_add()
{
    print_test_header("INT: Matrix addition");
    field_info_t *info = get_int_field_info();
    matrix_t *a = matrix_create(info, 2);
    matrix_t *b = matrix_create(info, 2);
    int data_a[2][2] = {{1,2},{3,4}};
    int data_b[2][2] = {{5,6},{7,8}};
    for (int i=0; i<2; i++)
        for (int j=0; j<2; j++) {
            matrix_set(a, i, j, &data_a[i][j]);
            matrix_set(b, i, j, &data_b[i][j]);
        }
    matrix_t *c = matrix_add(a, b);
    int expected[2][2] = {{6,8},{10,12}};
    int ok = 1;
    for (int i=0; i<2 && ok; i++)
        for (int j=0; j<2; j++) {
            int *val = (int*)matrix_get(c, i, j);
            if (*val != expected[i][j]) ok = 0;
        }
    print_subtest("Add: [[1,2],[3,4]] + [[5,6],[7,8]] = [[6,8],[10,12]]", ok);
    matrix_free(a); matrix_free(b); matrix_free(c);
}

static void test_int_multiply()
{
    print_test_header("INT: Matrix multiplication");
    field_info_t *info = get_int_field_info();
    matrix_t *a = matrix_create(info, 2);
    matrix_t *b = matrix_create(info, 2);
    int data_a[2][2] = {{1,2},{3,4}};
    int data_b[2][2] = {{5,6},{7,8}};
    for (int i=0; i<2; i++)
        for (int j=0; j<2; j++) {
            matrix_set(a, i, j, &data_a[i][j]);
            matrix_set(b, i, j, &data_b[i][j]);
        }
    matrix_t *c = matrix_multiply(a, b);
    int expected[2][2] = {{19,22},{43,50}};
    int ok = 1;
    for (int i=0; i<2 && ok; i++)
        for (int j=0; j<2; j++) {
            int *val = (int*)matrix_get(c, i, j);
            if (*val != expected[i][j]) ok = 0;
        }
    print_subtest("Multiply: [[1,2],[3,4]] * [[5,6],[7,8]] = [[19,22],[43,50]]", ok);
    matrix_free(a); matrix_free(b); matrix_free(c);
}

static void test_int_scalar_mul()
{
    print_test_header("INT: Scalar multiplication");
    field_info_t *info = get_int_field_info();
    matrix_t *m = matrix_create(info, 2);
    int data[2][2] = {{1,2},{3,4}};
    for (int i=0; i<2; i++)
        for (int j=0; j<2; j++)
            matrix_set(m, i, j, &data[i][j]);
    int scalar = 3;
    matrix_t *res = matrix_scalar_mul(m, &scalar);
    int expected[2][2] = {{3,6},{9,12}};  // ИСПРАВЛЕНО: 1000 → 3
    int ok = 1;
    for (int i=0; i<2 && ok; i++)
        for (int j=0; j<2; j++) {
            int *val = (int*)matrix_get(res, i, j);
            if (*val != expected[i][j]) ok = 0;
        }
    print_subtest("Scalar mul by 3: [[1,2],[3,4]] -> [[3,6],[9,12]]", ok);
    matrix_free(m); matrix_free(res);
}

static void test_int_linear_combination()
{
    print_test_header("INT: Add linear combination of rows");
    field_info_t *info = get_int_field_info();
    matrix_t *m = matrix_create(info, 3);
    int data[3][3] = {{1,2,3},{4,5,6},{7,8,9}};
    for (int i=0; i<3; i++)
        for (int j=0; j<3; j++)
            matrix_set(m, i, j, &data[i][j]);
    size_t src_rows[] = {0, 2};
    int coeffs[] = {2, 1};
    matrix_t *res = matrix_add_linear_combination(m, src_rows, coeffs, 1, 2);
    // new row1 = old row1 + 2*row0 + 1*row2 = (4+2*1+7, 5+2*2+8, 6+2*3+9) = (13, 17, 21)
    int expected[3][3] = {{1,2,3},{13,17,21},{7,8,9}};
    int ok = 1;
    for (int i=0; i<3 && ok; i++)
        for (int j=0; j<3; j++) {
            int *val = (int*)matrix_get(res, i, j);
            if (*val != expected[i][j]) ok = 0;
        }
    print_subtest("Linear combination: row1 += 2*row0 + 1*row2", ok);
    matrix_free(m); matrix_free(res);
}

static void test_double_scalar_mul()
{
    print_test_header("DOUBLE: Scalar multiplication");
    field_info_t *info = get_double_field_info();
    matrix_t *m = matrix_create(info, 2);
    double data[2][2] = {{1.5, 2.5}, {3.5, 4.5}};
    for (int i=0; i<2; i++)
        for (int j=0; j<2; j++)
            matrix_set(m, i, j, &data[i][j]);
    double scalar = 2.0;
    matrix_t *res = matrix_scalar_mul(m, &scalar);
    double expected[2][2] = {{3.0, 5.0}, {7.0, 9.0}};
    int ok = 1;
    for (int i=0; i<2 && ok; i++)
        for (int j=0; j<2; j++) {
            double *val = (double*)matrix_get(res, i, j);
            if (*val != expected[i][j]) ok = 0;
        }
    print_subtest("Double scalar mul: [[1.5,2.5],[3.5,4.5]] * 2 = [[3,5],[7,9]]", ok);
    matrix_free(m); matrix_free(res);
}

static void test_edge_cases()
{
    print_test_header("Edge cases");
    field_info_t *info = get_int_field_info();
    matrix_t *m = matrix_create(info, 1);
    int val = 123;
    matrix_set(m, 0, 0, &val);
    int *got = (int*)matrix_get(m, 0, 0);
    print_subtest("1x1 matrix works", got && *got == 123);
    matrix_free(m);
    
    // Попытка создания с нулевым размером
    matrix_t *null_size = matrix_create(info, 0);
    print_subtest("Create with size 0 returns NULL", null_size == NULL);
    
    // Попытка сложения матриц разных размеров
    matrix_t *a = matrix_create(info, 2);
    matrix_t *b = matrix_create(info, 3);
    matrix_t *bad_sum = matrix_add(a, b);
    print_subtest("Add matrices of different sizes returns NULL", bad_sum == NULL);
    matrix_free(a); matrix_free(b);
}

void run_all_tests()
{
    printf("\n========== RUNNING ALL TESTS ==========\n");
    test_int_matrix_creation();
    test_int_set_get();
    test_int_add();
    test_int_multiply();
    test_int_scalar_mul();
    test_int_linear_combination();
    test_double_scalar_mul();
    test_edge_cases();
    printf("\n========== TESTS FINISHED ==========\n");
}