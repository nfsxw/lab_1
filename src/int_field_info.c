#include "int_field_info.h"
#include "errors_handling.h"
#include <stdlib.h>
#include <stdio.h>

static int compare_ints(const void *a, const void *b)
{
        const int *a_val = (const int *)a;
        const int *b_val = (const int *)b;
        return *a_val - *b_val;
}

static void print_int(const void *elem)
{
        printf("%d", *(const int *)elem);
}

static void add_ints(const void *a, const void *b, void *result)
{
        const int *a_val = (const int *)a;
        const int *b_val = (const int *)b;
        int *res_val = (int *)result;
        *res_val = *a_val + *b_val;
}

static void multiply_ints(const void *a, const void *b, void *result)
{
        const int *a_val = (const int *)a;
        const int *b_val = (const int *)b;
        int *res_val = (int *)result;
        *res_val = *a_val * *b_val;
}

static void scalar_mul_int(const void *elem, const void *scalar, void *result)
{
        const int *elem_val = (const int *)elem;
        const int *scalar_val = (const int *)scalar;
        int *res_val = (int *)result;

        *res_val = *elem_val * *scalar_val;
}

static field_info_t *int_field_info = NULL;

field_info_t *get_int_field_info(void)
{
        if (!int_field_info) {
                int_field_info = malloc(sizeof(field_info_t));
                if (!int_field_info)
                	LOG_FATAL("%s", error_str(ERR_NO_MEM));
                int_field_info->elem_size = sizeof(int);
                int_field_info->print = print_int;
                int_field_info->compare = compare_ints;
                int_field_info->add = add_ints;
                int_field_info->multiply = multiply_ints;
                int_field_info->scalar_mul = scalar_mul_int;
        }
        return int_field_info;
}