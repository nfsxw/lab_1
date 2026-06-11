#include "double_field_info.h"
#include "errors_handling.h"
#include <stdlib.h>
#include <stdio.h>

static int compare_double(const void *a, const void *b)
{
    const double *a_val = (const double *)a;
    const double *b_val = (const double *)b;
    if (*a_val < *b_val) return -1;
    if (*a_val > *b_val) return 1;
    return 0;
}

static void print_double(const void *elem)
{
    printf("%f", *(const double *)elem);
}

static void add_double(const void *a, const void *b, void *result)
{
    const double *a_val = (const double *)a;
    const double *b_val = (const double *)b;
    double *res_val = (double *)result;
    *res_val = *a_val + *b_val;
}

static void multiply_double(const void *a, const void *b, void *result)
{
    const double *a_val = (const double *)a;
    const double *b_val = (const double *)b;
    double *res_val = (double *)result;
    *res_val = *a_val * *b_val;
}

static field_info_t *double_field_info = NULL;

field_info_t *get_double_field_info(void)
{
    if (!double_field_info) {
        double_field_info = malloc(sizeof(field_info_t));
        if (!double_field_info)
            LOG_FATAL("%s.", error_str(ERR_NO_MEM));
        double_field_info->elem_size = sizeof(double);
        double_field_info->print = print_double;
        double_field_info->compare = compare_double;
        double_field_info->add = add_double;
        double_field_info->multiply = multiply_double;
    }
    return double_field_info;
}