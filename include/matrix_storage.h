#ifndef MATRIX_STORAGE_H
#define MATRIX_STORAGE_H

#include <string.h>
#include "matrix.h"

#define MAX_NAME_LEN 64
#define INITIAL_CAPACITY 5

typedef struct matrix_entry_t
{
	char name[MAX_NAME_LEN];
	double id;
	matrix_t *matrix;
} matrix_entry_t;

typedef struct matrix_storage_t
{
	matrix_entry_t *entries;
	size_t amount;
	size_t capacity;
} matrix_storage_t;

void storage_init(matrix_storage_t *storage);
void add_to_storage(matrix_storage_t *storage, const char *name, matrix_t *matrix);
void remove_from_storage(matrix_storage_t *storage, const char *name);
void clear_storage(matrix_storage_t *storage);

#endif