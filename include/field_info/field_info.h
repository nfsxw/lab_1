#ifndef FIELD_INFO_H
#define FIELD_INFO_H

#include <stddef.h>

typedef struct field_info_t 
{
	size_t elem_size;
	int (*compare)(const void *a, const void *b);
	void (*print)(const void *elem);
	void (*add)(const void *a, const void *b, void *result);
	void (*multiply)(const void *a, const void *b, void *result);
} field_info_t;

#endif