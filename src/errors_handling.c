#include "errors_handling.h"

const char *error_str(error_code_t code)
{
	switch(code) {
		case ERR_OK:		return "Success";
		case ERR_FAILED:	return "Failed";
		case ERR_NULL_PTR:	return "Null pointer received";
		case ERR_NO_MEM:	return "Memory allocation failed";
		case ERR_INVALID_ARG:	return "Invalid argument";
		case ERR_NAME_EXISTS:	return "Matrix name already exists";
		case ERR_INDEX_OUT_OF_BOUNDS:	return "Index out of bounds";
		case ERR_DATA_TYPES:	return "Data types do not match";
		case ERR_SIZES:		return "Matrix sizes do not match";
		default:		return "Unknown error";
	}
}