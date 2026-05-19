#define INITIAL_AMOUNT 0
#define INITIAL_CAPACITY 5

#include "matrix_storage.h"
#include <string.h>

storage_t *matrix_storage_init(void)
{
	matrix_storage_t storage = malloc(sizeof(storage_t));
	if(!storage)
		LOG_FATAL("%s - storage not created.", error_str(ERR_NO_MEM));
	storage->amount = INITIAL_AMOUNT;
	storage->capacity = INITIAL_CAPACITY;
	storage->elem_size = sizeof(matrix_storage_elem_t);
	storage->data = malloc(sizeof(capacity * elem_size));

	if(!data)
		LOG_FATAL("%s - storage data were not allocated.",
						error_str(ERR_NO_MEM));
	return storage;
}

error_code_t matrix_storage_add(const matrix_storage_t *storage,
				const matrix_storage_elem_t *elem)
{
	if(!storage) {
		LOG_ERROR("%s from storage.", error_str(ERR_NULL_PTR));
		return ERR_FAILED;
	}

	if(!elem) {
		LOG_ERROR("%s from element.", error_str(ERR_NULL_PTR));
		return ERR_FAILED;
	}

	char *dest = (char *)storage->data + storage->amount *
					     storage->elem_size;
	memcpy(dest, elem, storage->elem_size);

	return ERR_OK;
}

matrix_storage_elem_t *matrix_storage_get_elem(const double *id)
{
	if(!id) {
		LOG_ERROR("%s from id.", error_str(ERR_NULL_PTR));
		return NULL;
	}

	char *elem = (char *)storage->data + id * storage->elem_size;
	return (matrix_storage_elem_t *)elem;		     
}

void matrix_storage_remove_elem(const double *id)
{
	if(!id) {
		LOG_ERROR("%s from id.", error_str(ERR_NULL_PTR));
		return NULL;
	}

	char *elem = (char *)storage->data + id * storage->elem_size;
	
}
matrix_storage_t *matrix_storage_init(void);
matrix_storage_t *matrix_storage_free(matrix_storage_t *storage);