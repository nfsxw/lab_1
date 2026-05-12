#include "matrix.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

matrix_t *matrix_create(field_info_t *field_info, size_t size)
{
	if (!field_info || !size)
		return NULL;

	matrix_t *matrix = malloc(sizeof(matrix_t));
	if (!matrix)
		return NULL;

	matrix->field_info = field_info;
	matrix->size = size;
	matrix->data = calloc(size * size, field_info->elem_size);

	if (!matrix->data) {
		free(matrix);
		return NULL;
	}
	return matrix;
}

void matrix_free(matrix_t *matrix)
{
	if (!matrix)
		return;
	free(matrix->data);
	free(matrix);
}

void *matrix_get(const matrix_t *matrix, size_t row, size_t col)
{
	if (!matrix || row >= matrix->size || col >= matrix->size) return NULL;
	size_t index = row * matrix->size + col;
	char *base = (char *)matrix->data;
	return base + (index * matrix->field_info->elem_size);
}

int matrix_set(matrix_t *matrix, size_t row, size_t col, const void *value)
{
    if (!matrix || !value)
    	return -1;
    if (row >= matrix->size || col >= matrix->size)
    	return -1;
    void *dest = matrix_get(matrix, row, col);
    if (!dest)
    	return -1;

    memcpy(dest, value, matrix->field_info->elem_size);
    return 0;
}

void matrix_print(const matrix_t *matrix)
{
	if (!matrix)
    		return;
	for (size_t i = 0; i < matrix->size; i++) {
		for (size_t j = 0; j < matrix->size; j++) {
        		void *elem = matrix_get(matrix, i, j);
        		matrix->field_info->print(elem);
        		printf(" ");
        	}
		printf("\n");
	}
}

matrix_t *matrix_add(const matrix_t *a, const matrix_t *b)
{
	if (!a || !b)
		return NULL;
	if (a->field_info != b->field_info)
		return NULL;
	if (a->size != b->size)
		return NULL;

	matrix_t *result = matrix_create(a->field_info, a->size);
	if (result == NULL)
		return NULL;

	void *ptr_a, *ptr_b, *ptr_res;

	for (size_t i = 0; i < a->size; i++) {
        	for (size_t j = 0; j < a->size; j++) {
            		ptr_a = matrix_get(a, i, j);
            		ptr_b = matrix_get(b, i, j);
			ptr_res = matrix_get(result, i, j);
			a->field_info->add(ptr_a, ptr_b, ptr_res);
		}
	}
	return result;
}

matrix_t *matrix_multiply(const matrix_t *a, const matrix_t *b)
{
	if (!a || !b)
		return NULL;
	if (a->field_info != b->field_info)
		return NULL;
	if (a->size != b->size)
		return NULL; 

	matrix_t *result = matrix_create(a->field_info, a->size);
	if (!result) return NULL;
	
	char temp[a->field_info->elem_size];
	void *ptr_a, *ptr_b, *ptr_res;

	for (size_t i = 0; i < a->size; i++) {
		for (size_t j = 0; j < a->size; j++) {
			for (size_t k = 0; k < a->size; k++) {
				ptr_a = matrix_get(a, i, k);
				ptr_b = matrix_get(b, k, j);
				ptr_res = matrix_get(result, i , j);

				a->field_info->multiply(ptr_a, ptr_b, temp);
				a->field_info->add(ptr_res, temp, ptr_res);
			}
		}
	}
	return result;
}

matrix_t *matrix_scalar_mul(const matrix_t *matrix, const void *scalar)
{
	if (!matrix || !scalar) return NULL;
	matrix_t *result = matrix_create(matrix->field_info, matrix->size);
	if (!result) return NULL;

	void *elem, *res_elem;
	for (size_t i = 0; i < matrix->size; i++) {
		for (size_t j = 0; j < matrix->size; j++) {
			elem = matrix_get(matrix, i, j);
			res_elem = matrix_get(result, i, j);
			matrix->field_info->scalar_mul(elem, scalar, res_elem);
		}
	}
	return result;
}

matrix_t *matrix_add_linear_combination(const matrix_t *source,
					size_t target_row,
					const size_t *source_rows,
					const void *coefficients,
					size_t count)
{
	if (!source)
		return NULL;
	if (!source_rows)
		return NULL;
	if (!coefficients)
		return NULL;
	if (target_row >= source->size)
		return NULL;

	matrix_t *result = matrix_create(source->field_info, source->size);
	if (!result)
		return NULL;
	
	memcpy(result->data, source->data, source->size * source->size * 
					   source->field_info->elem_size);
	for (size_t i = 0; i < count; i++) {
        	size_t src_row = source_rows[i];
		if (src_row >= source->size) {
			matrix_free(result);
			return NULL;
		}
		const void* coeff = (const char*)coefficients + i *
						 source->field_info->elem_size;
		void *src_elem, *res_elem;
		char temp[source->field_info->elem_size];

		for (size_t j = 0; j < source->size; j++) {
			src_elem = matrix_get(source, src_row, j);
			res_elem = matrix_get(result, target_row, j);

			source->field_info->scalar_mul(src_elem, coeff, temp);
			source->field_info->add(res_elem, temp, res_elem);
		}
	}
	return result;
}


