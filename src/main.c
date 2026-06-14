#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "matrix.h"
#include "int_field_info.h"
#include "double_field_info.h"
#include "tests.h"

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

#define MAX_MATRICES 10

static matrix_t *matrices[MAX_MATRICES] = {NULL};

static void clear_screen()
{
    system(CLEAR);
}

static void wait_for_enter()
{
    printf("\nPress Enter to continue...");
    fflush(stdout);
    getchar();
}

static void show_available_matrices()
{
    printf("Available matrix IDs: ");
    int first = 1;
    for (int i = 0; i < MAX_MATRICES; i++) {
        if (matrices[i]) {
            if (!first) printf(", ");
            printf("%d", i);
            first = 0;
        }
    }
    if (first) printf("none");
    printf("\n");
}

static int read_int_range(const char *prompt, int min, int max, int *value)
{
    char buffer[32];
    while (1) {
        printf("%s", prompt);
        if (!fgets(buffer, sizeof(buffer), stdin))
            return 0;
        char *endptr;
        errno = 0;
        long val = strtol(buffer, &endptr, 10);
        if (endptr == buffer || errno != 0 || val < min || val > max) {
            printf("Error: please enter an integer between %d and %d.\n", min, max);
            continue;
        }
        while (*endptr == ' ' || *endptr == '\t' || *endptr == '\n')
            endptr++;
        if (*endptr != '\0') {
            printf("Error: please enter an integer without extra characters.\n");
            continue;
        }
        *value = (int)val;
        return 1;
    }
}

static int read_size_range(const char *prompt, size_t *value, size_t min, size_t max)
{
    int int_val;
    if (!read_int_range(prompt, (int)min, (int)max, &int_val))
        return 0;
    *value = (size_t)int_val;
    return 1;
}

static int read_double_range(const char *prompt, double *value)
{
    char buffer[64];
    while (1) {
        printf("%s", prompt);
        if (!fgets(buffer, sizeof(buffer), stdin))
            return 0;
        char *endptr;
        errno = 0;
        double val = strtod(buffer, &endptr);
        if (endptr == buffer || errno != 0) {
            printf("Error: please enter a valid number.\n");
            continue;
        }
        while (*endptr == ' ' || *endptr == '\t' || *endptr == '\n')
            endptr++;
        if (*endptr != '\0') {
            printf("Error: please enter a number without extra characters.\n");
            continue;
        }
        *value = val;
        return 1;
    }
}

static int any_matrix_exists()
{
    for (int i = 0; i < MAX_MATRICES; i++)
        if (matrices[i]) return 1;
    return 0;
}

static int read_matrix_elements(matrix_t *mat, int type)
{
    size_t total = mat->size * mat->size;
    while (1) {
        printf("Enter %zu numbers separated by spaces or newlines:\n", total);
        size_t count = 0;
        int ok = 1;
        while (count < total) {
            char buffer[4096];
            if (!fgets(buffer, sizeof(buffer), stdin)) {
                printf("Input error.\n");
                return 0;
            }
            char *ptr = buffer;
            while (count < total && *ptr) {
                while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n') ptr++;
                if (*ptr == '\0') break;
                
                char *endptr;
                if (type == 1) {
                    long val = strtol(ptr, &endptr, 10);
                    if (endptr == ptr) {
                        ok = 0;
                        break;
                    }
                    if (*endptr != '\0' && *endptr != ' ' && *endptr != '\t' && *endptr != '\n') {
                        ok = 0;
                        break;
                    }
                    int iv = (int)val;
                    size_t row = count / mat->size;
                    size_t col = count % mat->size;
                    matrix_set(mat, row, col, &iv);
                    ptr = endptr;
                    count++;
                } else {
                    double val = strtod(ptr, &endptr);
                    if (endptr == ptr) {
                        ok = 0;
                        break;
                    }
                    if (*endptr != '\0' && *endptr != ' ' && *endptr != '\t' && *endptr != '\n') {
                        ok = 0;
                        break;
                    }
                    size_t row = count / mat->size;
                    size_t col = count % mat->size;
                    matrix_set(mat, row, col, &val);
                    ptr = endptr;
                    count++;
                }
            }
            if (!ok) break;
        }
        if (ok && count == total) return 1;
        printf("Invalid input. Please enter all %zu numbers again.\n", total);
    }
}

static matrix_t* create_matrix_interactive()
{
    int type_choice;
    if (!read_int_range("Type of elements: 1 - int, 2 - double: ", 1, 2, &type_choice))
        return NULL;

    size_t size;
    if (!read_size_range("Matrix size (N x N, 1..10): ", &size, 1, 10))
        return NULL;

    field_info_t *info = (type_choice == 1) ? get_int_field_info() : get_double_field_info();
    matrix_t *mat = matrix_create(info, size);
    if (!mat) {
        printf("Failed to create matrix.\n");
        return NULL;
    }

    if (!read_matrix_elements(mat, type_choice)) {
        matrix_free(mat);
        return NULL;
    }
    return mat;
}

static void print_menu()
{
    printf("\n=== Matrix Operations ===\n");
    printf("1. Create matrix\n");
    printf("2. Print matrix\n");
    printf("3. Add two matrices\n");
    printf("4. Multiply two matrices\n");
    printf("5. Scalar multiplication\n");
    printf("6. Add linear combination to row\n");
    printf("7. Show all matrices\n");
    printf("8. Free matrix\n");
    printf("9. Run all tests (auto)\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

static int read_menu_choice()
{
    char buf[32];
    while (1) {
        print_menu();
        if (!fgets(buf, sizeof(buf), stdin))
            return -1;
        buf[strcspn(buf, "\n")] = '\0';
        char *endptr;
        long val = strtol(buf, &endptr, 10);
        if (endptr == buf || *endptr != '\0') {
            printf("Invalid input. Please enter a number between 0 and 9.\n");
            wait_for_enter();
            clear_screen();
            continue;
        }
        if (val < 0 || val > 9) {
            printf("Choice must be between 0 and 9.\n");
            wait_for_enter();
            clear_screen();
            continue;
        }
        return (int)val;
    }
}

static int add_matrix(matrix_t *m)
{
    for (int i = 0; i < MAX_MATRICES; i++) {
        if (!matrices[i]) {
            matrices[i] = m;
            return i;
        }
    }
    return -1;
}

static matrix_t* get_matrix_by_id(const char *prompt)
{
    while (1) {
        show_available_matrices();
        int id;
        if (!read_int_range(prompt, 0, MAX_MATRICES-1, &id))
            continue;
        if (matrices[id])
            return matrices[id];
        printf("Matrix ID %d does not exist. Please try again.\n", id);
    }
}

int main()
{
    int choice;
    while (1) {
        clear_screen();
        choice = read_menu_choice();
        if (choice == -1 || choice == 0)
            break;

        clear_screen();

        switch (choice) {
            case 1: {
                matrix_t *m = create_matrix_interactive();
                if (m) {
                    int id = add_matrix(m);
                    if (id >= 0) printf("Matrix created with ID %d\n", id);
                    else printf("Storage full.\n");
                } else printf("Creation failed.\n");
                wait_for_enter();
                break;
            }
            case 2: {
                if (!any_matrix_exists()) {
                    printf("No matrices exist.\n");
                    wait_for_enter();
                    break;
                }
                matrix_t *m = get_matrix_by_id("Enter matrix ID to print: ");
                printf("Matrix:\n");
                matrix_print(m);
                wait_for_enter();
                break;
            }
            case 3: {
                if (!any_matrix_exists()) {
                    printf("No matrices exist.\n");
                    wait_for_enter();
                    break;
                }
                matrix_t *a = get_matrix_by_id("Matrix A ID: ");
                matrix_t *b = get_matrix_by_id("Matrix B ID: ");
                matrix_t *res = matrix_add(a, b);
                if (res) {
                    int id = add_matrix(res);
                    printf("Result matrix ID: %d\n", id);
                    matrix_print(res);
                } else {
                    printf("Addition failed.\n");
                }
                wait_for_enter();
                break;
            }
            case 4: {
                if (!any_matrix_exists()) {
                    printf("No matrices exist.\n");
                    wait_for_enter();
                    break;
                }
                matrix_t *a = get_matrix_by_id("Matrix A ID: ");
                matrix_t *b = get_matrix_by_id("Matrix B ID: ");
                matrix_t *res = matrix_multiply(a, b);
                if (res) {
                    int id = add_matrix(res);
                    printf("Result matrix ID: %d\n", id);
                    matrix_print(res);
                } else {
                    printf("Multiplication failed.\n");
                }
                wait_for_enter();
                break;
            }
            case 5: {
                if (!any_matrix_exists()) {
                    printf("No matrices exist.\n");
                    wait_for_enter();
                    break;
                }
                matrix_t *m = get_matrix_by_id("Matrix ID: ");
                if (m->field_info->elem_size == sizeof(int)) {
                    int scalar;
                    if (!read_int_range("Enter integer scalar: ", -1000000, 1000000, &scalar))
                        break;
                    matrix_t *res = matrix_scalar_mul(m, &scalar);
                    if (res) {
                        int id = add_matrix(res);
                        printf("Scaled matrix ID: %d\n", id);
                        matrix_print(res);
                    } else printf("Scalar multiplication failed.\n");
                } else {
                    double scalar;
                    if (!read_double_range("Enter double scalar: ", &scalar))
                        break;
                    matrix_t *res = matrix_scalar_mul(m, &scalar);
                    if (res) {
                        int id = add_matrix(res);
                        printf("Scaled matrix ID: %d\n", id);
                        matrix_print(res);
                    } else printf("Scalar multiplication failed.\n");
                }
                wait_for_enter();
                break;
            }
            case 6: {
                if (!any_matrix_exists()) {
                    printf("No matrices exist.\n");
                    wait_for_enter();
                    break;
                }
                matrix_t *src = get_matrix_by_id("Source matrix ID: ");
                printf("\nCurrent matrix:\n");
                matrix_print(src);
                printf("\n");
                size_t target_row;
                if (!read_size_range("Target row index (0..n-1): ", &target_row, 0, src->size-1))
                    break;
                size_t count;
                if (!read_size_range("Number of source rows: ", &count, 1, src->size))
                    break;
                size_t *rows = malloc(count * sizeof(size_t));
                if (!rows) {
                    printf("Memory error.\n");
                    wait_for_enter();
                    break;
                }
                printf("Enter %zu source row indices (0..%zu):\n", count, src->size-1);
                int ok = 1;
                for (size_t i = 0; i < count; i++) {
                    if (!read_size_range("", &rows[i], 0, src->size-1)) {
                        ok = 0;
                        break;
                    }
                }
                if (!ok) {
                    free(rows);
                    wait_for_enter();
                    break;
                }
                void *coeffs = malloc(count * src->field_info->elem_size);
                if (!coeffs) {
                    free(rows);
                    printf("Memory error.\n");
                    wait_for_enter();
                    break;
                }
                printf("Enter %zu coefficients:\n", count);
                if (src->field_info->elem_size == sizeof(int)) {
                    for (size_t i = 0; i < count; i++) {
                        int c;
                        if (!read_int_range("", -1000000, 1000000, &c)) {
                            ok = 0;
                            break;
                        }
                        memcpy((char*)coeffs + i*sizeof(int), &c, sizeof(int));
                    }
                } else {
                    for (size_t i = 0; i < count; i++) {
                        double c;
                        if (!read_double_range("", &c)) {
                            ok = 0;
                            break;
                        }
                        memcpy((char*)coeffs + i*sizeof(double), &c, sizeof(double));
                    }
                }
                if (!ok) {
                    free(rows);
                    free(coeffs);
                    wait_for_enter();
                    break;
                }
                matrix_t *res = matrix_add_linear_combination(src, rows, coeffs, target_row, count);
                if (res) {
                    int id = add_matrix(res);
                    printf("Result matrix ID: %d\n", id);
                    matrix_print(res);
                } else {
                    printf("Linear combination failed.\n");
                }
                free(rows);
                free(coeffs);
                wait_for_enter();
                break;
            }
            case 7: {
                if (!any_matrix_exists()) {
                    printf("No matrices exist.\n");
                    wait_for_enter();
                    break;
                }
                printf("Stored matrices:\n");
                for (int i = 0; i < MAX_MATRICES; i++) {
                    if (matrices[i]) {
                        printf("ID %d:\n", i);
                        matrix_print(matrices[i]);
                        printf("\n");
                    }
                }
                wait_for_enter();
                break;
            }
            case 8: {
                if (!any_matrix_exists()) {
                    printf("No matrices exist.\n");
                    wait_for_enter();
                    break;
                }
                while (1) {
                    show_available_matrices();
                    int id;
                    if (!read_int_range("Matrix ID to free (or -1 to cancel): ", -1, MAX_MATRICES-1, &id))
                        continue;
                    if (id == -1) break;
                    if (matrices[id]) {
                        matrix_free(matrices[id]);
                        matrices[id] = NULL;
                        printf("Freed.\n");
                        break;
                    } else {
                        printf("Matrix ID %d does not exist. Try again.\n", id);
                    }
                }
                wait_for_enter();
                break;
            }
            case 9: {
                run_all_tests();
                wait_for_enter();
                break;
            }
            default:
                printf("Invalid choice.\n");
                wait_for_enter();
        }
    }
    for (int i = 0; i < MAX_MATRICES; i++)
        if (matrices[i]) matrix_free(matrices[i]);
    return 0;
}