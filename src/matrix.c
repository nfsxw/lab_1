#include "matrix.h"
#include "errors_handling.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

matrix_t *matrix_create(field_info_t *field_info, size_t size)
{
	if (!field_info) {
		LOG_ERROR("%s", error_str(ERR_NULL_PTR));
		return NULL;
	}

	if (!size) {
		LOG_ERROR("%s - size must be positive.",
			error_str(ERR_INVALID_ARG));
		return NULL;
	}
		
	matrix_t *matrix = malloc(sizeof(matrix_t));
	if (!matrix)
		LOG_FATAL("%s, matrix is not created.", error_str(ERR_NO_MEM));

	matrix->field_info = field_info;
	matrix->size = size;
	matrix->data = calloc(size * size, field_info->elem_size);

	if (!matrix->data) {
		free(matrix);
		LOG_FATAL("%s, matrix memory is not allocated.",
			error_str(ERR_NO_MEM));
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
	if(!matrix) {
		LOG_ERROR("%s.", error_str(ERR_NULL_PTR));
		return NULL;
	}

	if(row >= matrix->size || col >= matrix->size) {
		LOG_ERROR("%s.", error_str(ERR_INDEX_OUT_OF_BOUNDS));
		return NULL;
	}

	size_t index = row * matrix->size + col;
	char *base = (char *)matrix->data;
	return base + (index * matrix->field_info->elem_size);
}

error_code_t matrix_set(matrix_t *matrix, size_t row, size_t col, const void *value)
{
	if(!matrix) {
		LOG_ERROR("%s from matrix.", error_str(ERR_NULL_PTR));
		return ERR_FAILED;
	}

	if(!value) {
		LOG_ERROR("%s from value.", error_str(ERR_NULL_PTR));
		return ERR_FAILED;
	}

	if (row >= matrix->size || col >= matrix->size) {
		LOG_ERROR("%s.", error_str(ERR_INDEX_OUT_OF_BOUNDS));
		return ERR_FAILED;
	}
    	
	void *dest = matrix_get(matrix, row, col);
	if (!dest) {
		LOG_ERROR("%s from dest.", error_str(ERR_NULL_PTR));
		return ERR_FAILED;
	}

	memcpy(dest, value, matrix->field_info->elem_size);
	return ERR_OK;
}

error_code_t matrix_print(const matrix_t *matrix)
{
	if(!matrix) {
		LOG_ERROR("%s.", error_str(ERR_NULL_PTR));
		return ERR_FAILED;
	}

	for (size_t i = 0; i < matrix->size; i++) {
		for (size_t j = 0; j < matrix->size; j++) {
        		void *elem = matrix_get(matrix, i, j);
        		matrix->field_info->print(elem);
        		printf(" ");
        	}
		printf("\n");
	}
	return ERR_OK;
}

matrix_t *matrix_add(const matrix_t *a, const matrix_t *b)
{
	if (!a) {
		LOG_ERROR("%s from matrix a.", error_str(ERR_NULL_PTR));
		return NULL;
	}

	if (!b) {
		LOG_ERROR("%s from matrix b.", error_str(ERR_NULL_PTR));
		return NULL;
	}

	if (a->size != b->size) {
		LOG_ERROR("%s.", error_str(ERR_SIZES));
		return NULL; 
	}

	matrix_t *result = matrix_create(a->field_info, a->size);
	if (!result) {
		LOG_ERROR("%s from result.", error_str(ERR_NULL_PTR));
		return NULL;
	}

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
	if (!a) {
		LOG_ERROR("%s from matrix a.", error_str(ERR_NULL_PTR));
		return NULL;
	}

	if (!b) {
		LOG_ERROR("%s from matrix b.", error_str(ERR_NULL_PTR));
		return NULL;
	}

	if (a->size != b->size) {
		LOG_ERROR("%s.", error_str(ERR_SIZES));
		return NULL; 
	}


	matrix_t *result = matrix_create(a->field_info, a->size);
	if (!result) {
		LOG_ERROR("%s from result.", error_str(ERR_NULL_PTR));
		return NULL;
	}
	
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
	if (!matrix) {
		LOG_ERROR("%s from matrix.", error_str(ERR_NULL_PTR));
		return NULL;
	}

	if (!scalar) {
		LOG_ERROR("%s from scalar.", error_str(ERR_NULL_PTR));
		return NULL;
	}

	matrix_t *result = matrix_create(matrix->field_info, matrix->size);
	if (!result) {
		LOG_ERROR("%s from result.", error_str(ERR_NULL_PTR));
		return NULL;
	}

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
					const size_t *source_rows,
					const void *coefficients,
					size_t target_row,
					size_t count)
{
	if (!source) {
		LOG_ERROR("%s from source.", error_str(ERR_NULL_PTR));
		return NULL;
	}
	
	if (!source_rows) {
		LOG_ERROR("%s from source_rows.", error_str(ERR_NULL_PTR));
		return NULL;
	}
	
	if (!coefficients) {
		LOG_ERROR("%s from coefficients.", error_str(ERR_NULL_PTR));
		return NULL;
	}

	if (target_row >= source->size) {
		LOG_ERROR("%s.", error_str(ERR_INDEX_OUT_OF_BOUNDS));
		return NULL;
	}


	matrix_t *result = matrix_create(source->field_info, source->size);
	if (!result) {
		LOG_ERROR("%s from result.", error_str(ERR_NULL_PTR));
		return NULL;
	}

	memcpy(result->data, source->data, source->size * source->size * 
					   source->field_info->elem_size);
	for (size_t i = 0; i < count; i++) {
        	size_t src_row = source_rows[i];

		if (src_row >= source->size) {
			matrix_free(result);
			LOG_ERROR("%s.", error_str(ERR_INDEX_OUT_OF_BOUNDS));
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


