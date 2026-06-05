#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "matrix.h"
#include "matrix_storage.h"
#include "errors_handling.h"
#include "menu.h"

// Helper to clear input buffer
void clear_input() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Helper to read integer safely
int read_int(const char* prompt) {
    int val;
    printf("%s", prompt);
    while (scanf("%d", &val) != 1) {
        printf("Invalid input. %s", prompt);
        clear_input();
    }
    clear_input();
    return val;
}

// Helper to read double safely
double read_double(const char* prompt) {
    double val;
    printf("%s", prompt);
    while (scanf("%lf", &val) != 1) {
        printf("Invalid input. %s", prompt);
        clear_input();
    }
    clear_input();
    return val;
}

void create_matrix_menu(matrix_storage_t* storage) {
    printf("\n--- Create New Matrix ---\n");
    
    char name[64];
    printf("Enter matrix name: ");
    if (scanf("%63s", name) != 1) {
        clear_input();
        printf("Error reading name.\n");
        return;
    }
    clear_input();

    int type_choice = read_int("Type: 1 for Int, 2 for Double: ");
    field_info_t* field_info = NULL;
    if (type_choice == 1) {
        field_info = get_int_field_info();
    } else if (type_choice == 2) {
        field_info = get_double_field_info();
    } else {
        printf("Invalid type.\n");
        return;
    }

    int size = read_int("Enter size N (NxN): ");
    if (size <= 0) {
        printf("Invalid size.\n");
        return;
    }

    matrix_t* mat = matrix_create(field_info, size);
    if (!mat) {
        printf("Failed to create matrix.\n");
        return;
    }

    printf("Enter elements row by row:\n");
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (type_choice == 1) {
                int val = read_int("");
                matrix_set_element(mat, i, j, &val);
            } else {
                double val = read_double("");
                matrix_set_element(mat, i, j, &val);
            }
        }
    }

    error_code_t err = matrix_storage_add(storage, name, mat);
    if (err != ERR_OK) {
        printf("Error adding matrix to storage: %d\n", err);
        matrix_destroy(mat);
    } else {
        printf("Matrix '%s' added successfully.\n", name);
    }
}

void print_matrix_menu(matrix_storage_t* storage) {
    printf("\n--- Print Matrix ---\n");
    if (storage->amount == 0) {
        printf("Storage is empty.\n");
        return;
    }

    for (size_t i = 0; i < storage->amount; ++i) {
        printf("[%zu] %s (ID: %d)\n", i, storage->data[i].name, storage->data[i].id);
    }

    int idx = read_int("Select index to print: ");
    if (idx < 0 || (size_t)idx >= storage->amount) {
        printf("Invalid index.\n");
        return;
    }

    matrix_t* mat = storage->data[idx].matrix;
    matrix_print(mat);
}

void operate_matrices_menu(matrix_storage_t* storage) {
    printf("\n--- Operations ---\n");
    if (storage->amount < 2) {
        printf("Need at least 2 matrices for binary operations.\n");
        return;
    }

    for (size_t i = 0; i < storage->amount; ++i) {
        printf("[%zu] %s\n", i, storage->data[i].name);
    }

    int idx1 = read_int("Select first matrix index: ");
    int idx2 = read_int("Select second matrix index: ");

    if (idx1 < 0 || idx2 < 0 || 
        (size_t)idx1 >= storage->amount || (size_t)idx2 >= storage->amount) {
        printf("Invalid index.\n");
        return;
    }

    matrix_t* m1 = storage->data[idx1].matrix;
    matrix_t* m2 = storage->data[idx2].matrix;

    if (m1->field_info != m2->field_info) {
        printf("Error: Matrices have different types.\n");
        return;
    }

    int op = read_int("Operation: 1 for Add, 2 for Multiply: ");
    matrix_t* res = NULL;

    if (op == 1) {
        res = matrix_add(m1, m2);
    } else if (op == 2) {
        res = matrix_multiply(m1, m2);
    } else {
        printf("Invalid operation.\n");
        return;
    }

    if (res) {
        printf("Result:\n");
        matrix_print(res);
        
        char res_name[64];
        snprintf(res_name, sizeof(res_name), "Res_%s_%s", storage->data[idx1].name, storage->data[idx2].name);
        matrix_storage_add(storage, res_name, res);
        printf("Result saved as '%s'\n", res_name);
    } else {
        printf("Operation failed.\n");
    }
}

void scalar_mul_menu(matrix_storage_t* storage) {
    printf("\n--- Scalar Multiplication ---\n");
    if (storage->amount == 0) {
        printf("Storage is empty.\n");
        return;
    }

    for (size_t i = 0; i < storage->amount; ++i) {
        printf("[%zu] %s\n", i, storage->data[i].name);
    }

    int idx = read_int("Select matrix index: ");
    if (idx < 0 || (size_t)idx >= storage->amount) {
        printf("Invalid index.\n");
        return;
    }

    matrix_t* m = storage->data[idx].matrix;
    void* scalar = malloc(m->field_info->elem_size);
    if (!scalar) return;

    if (m->field_info == get_int_field_info()) {
        int val = read_int("Enter scalar value: ");
        *(int*)scalar = val;
    } else {
        double val = read_double("Enter scalar value: ");
        *(double*)scalar = val;
    }

    matrix_t* res = matrix_scalar_mul(m, scalar);
    free(scalar);

    if (res) {
        printf("Result:\n");
        matrix_print(res);
        char res_name[64];
        snprintf(res_name, sizeof(res_name), "Scalar_%s", storage->data[idx].name);
        matrix_storage_add(storage, res_name, res);
    }
}

int main() {
    matrix_storage_t storage;
    matrix_storage_init(&storage);

    int running = 1;
    while (running) {
        printf("\n=== Lab 1: Polymorphic Matrix Collection ===\n");
        printf("1. Create Matrix\n");
        printf("2. Print Matrix\n");
        printf("3. Add/Multiply Matrices\n");
        printf("4. Scalar Multiplication\n");
        printf("5. Exit\n");
        
        int choice = read_int("Choice: ");

        switch (choice) {
            case 1: create_matrix_menu(&storage); break;
            case 2: print_matrix_menu(&storage); break;
            case 3: operate_matrices_menu(&storage); break;
            case 4: scalar_mul_menu(&storage); break;
            case 5: running = 0; break;
            default: printf("Invalid choice.\n");
        }
    }

    matrix_storage_destroy(&storage);
    return 0;
}