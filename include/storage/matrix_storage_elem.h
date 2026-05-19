#ifndef MATRIX_STORAGE_ELEM_H
#define MATRIX_STORAGE_ELEM_H

#define MAX_NAME_LEN 64
#include "matrix.h"

typedef struct matrix_storage_elem_t
{
	char name[MAX_NAME_LEN];
	double id;
	matrix_t *matrix;

} matrix_storage_elem_t;

void set_name(const char *name);
const char (*get_name)(const void *elem);
void set_id(const double id);
const double (*get_id)(const void *elem);
void matrix_storage_elem_free(matrix_storage_elem_t *elem);

#endif




