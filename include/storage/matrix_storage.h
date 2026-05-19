#ifndef MATRIX_STORAGE_H
#define MATRIX_STORAGE_H

#include "matrix_storage_elem.h"
#include <stddef.h>

typedef struct matrix_storage_t
{
	matrix_storage_elem_t *data;
	size_t elem_size;
	size_t amount;
	size_t capacity;
} matrix_storage_t;

void matrix_storage_add(const matrix_storage_elem_t *elem);
void matrix_storage_get(const double *id);
void matrix_storage_remove(const double *id);
matrix_storage_t *matrix_storage_init(void);
matrix_storage_t *matrix_storage_free(matrix_storage_t *storage);

#endif