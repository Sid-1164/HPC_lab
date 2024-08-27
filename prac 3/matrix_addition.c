#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

// Function to perform matrix addition
void matrix_addition(int **A, int **B, int **C, int size) {
    #pragma omp parallel for collapse(2)         // collapse(2) is used to parallelize nested loops
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
}

// Function to initialize a matrix with random values
void initialize_matrix(int **matrix, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = rand() % 100;
        }
    }
}

// Function to allocate memory for a matrix
int** allocate_matrix(int size) {
    int **matrix = (int **)malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++) {
        matrix[i] = (int *)malloc(size * sizeof(int));
    }
    return matrix;
}

// Function to free allocated memory for a matrix
void free_matrix(int **matrix, int size) {
    for (int i = 0; i < size; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

int main() {
    int sizes[] = {250, 500, 750, 1000, 2000};
    int num_threads[] = {2, 4, 8, 12};

    for (int s = 0; s < 5; s++) {
        int size = sizes[s];
        printf("Matrix Size: %dx%d\n", size, size);

        // Allocate matrices
        int **A = allocate_matrix(size);
        int **B = allocate_matrix(size);
        int **C = allocate_matrix(size);

        // Initialize matrices
        initialize_matrix(A, size);
        initialize_matrix(B, size);

        for (int t = 0; t < 4; t++) {
            int threads = num_threads[t];
            omp_set_num_threads(threads);

            // Measure start time
            double start_time = omp_get_wtime();

            // Perform matrix addition
            matrix_addition(A, B, C, size);

            // Measure end time
            double end_time = omp_get_wtime();

            double execution_time = end_time - start_time;
            printf("Threads: %d, Execution Time: %f seconds\n", threads, execution_time);
        }

        // Free allocated memory
        free_matrix(A, size);
        free_matrix(B, size);
        free_matrix(C, size);

        printf("\n");
    }

    return 0;
}
