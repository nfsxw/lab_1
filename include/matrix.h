#ifndef MATRIX_H
#define MATRIX_H

#include <stddef.h>
#include "field_info.h"
#include "errors_handling.h"

typedef struct matrix_t
{
    field_info_t *field_info;
    void *data;
    size_t size;
} matrix_t;

matrix_t *matrix_create(field_info_t *field_info, size_t size);
void matrix_free(matrix_t *matrix);
void *matrix_get(const matrix_t *matrix, size_t row, size_t col);
error_code_t matrix_set(matrix_t *matrix, size_t row, size_t col, const void *value);
error_code_t matrix_print(const matrix_t *matrix);
matrix_t *matrix_add(const matrix_t *a, const matrix_t *b);
matrix_t *matrix_multiply(const matrix_t *a, const matrix_t *b);
matrix_t *matrix_scalar_mul(const matrix_t *matrix, const void *scalar);
matrix_t *matrix_add_linear_combination(const matrix_t *source,
                                        const size_t *source_rows,
                                        const void *coefficients,
                                        size_t target_row,
                                        size_t count);

#endif