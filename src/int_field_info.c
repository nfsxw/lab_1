#include "int_field_info.h"
#include "errors_handling.h"
#include <stdlib.h>
#include <stdio.h>

static int compare_int(const void *a, const void *b)
{
    const int *a_val = (const int *)a;
    const int *b_val = (const int *)b;
    if (*a_val < *b_val) return -1;
    if (*a_val > *b_val) return 1;
    return 0;
}

static void print_int(const void *elem)
{
    printf("%d", *(const int *)elem);
}

static void add_int(const void *a, const void *b, void *result)
{
    const int *a_val = (const int *)a;
    const int *b_val = (const int *)b;
    int *res_val = (int *)result;
    *res_val = *a_val + *b_val;
}

static void multiply_int(const void *a, const void *b, void *result)
{
    const int *a_val = (const int *)a;
    const int *b_val = (const int *)b;
    int *res_val = (int *)result;
    *res_val = *a_val * *b_val;
}

static field_info_t *int_field_info = NULL;

field_info_t *get_int_field_info(void)
{
    if (!int_field_info) {
        int_field_info = malloc(sizeof(field_info_t));
        if (!int_field_info)
            LOG_FATAL("%s.", error_str(ERR_NO_MEM));
        int_field_info->elem_size = sizeof(int);
        int_field_info->print = print_int;
        int_field_info->compare = compare_int;
        int_field_info->add = add_int;
        int_field_info->multiply = multiply_int;
    }
    return int_field_info;
}