#ifndef ERRORS_HANDLING_H
#define ERRORS_HANDLING_H

#include <stdio.h>
#include <stdlib.h>

typedef enum
{
    ERR_OK = 0,
    ERR_FAILED = -1,
    ERR_NULL_PTR = -2,
    ERR_NO_MEM = -3,
    ERR_INVALID_ARG = -4,
    ERR_NAME_EXISTS = -5,
    ERR_INDEX_OUT_OF_BOUNDS = -6,
    ERR_DATA_TYPES = -7,
    ERR_SIZES = -8
} error_code_t;

#define LOG_ERROR(fmt, ...) \
    fprintf(stderr, "[ERROR] %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#define LOG_FATAL(fmt, ...) \
    do { \
        fprintf(stderr, "[FATAL] %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
        exit(EXIT_FAILURE); \
    } while(0)

const char* error_str(error_code_t code);

#endif