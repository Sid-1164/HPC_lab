#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define N 1000 // Matrix size (N x N)

void initialize_matrix(double **matrix) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = rand() % 10;
        }
    }
}

void multiply_matrices_serial(double **A, double **B, double **C) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0.0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void multiply_matrices_parallel(double **A, double **B, double **C) {
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0.0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    double **A, **B, **C;
    double start_time, end_time, serial_time, parallel_time;

    // Allocate memory for matrices
    A = (double **)malloc(N * sizeof(double *));
    B = (double **)malloc(N * sizeof(double *));
    C = (double **)malloc(N * sizeof(double *));
    for (int i = 0; i < N; i++) {
        A[i] = (double *)malloc(N * sizeof(double));
        B[i] = (double *)malloc(N * sizeof(double));
        C[i] = (double *)malloc(N * sizeof(double));
    }

    // Initialize matrices
    srand(time(NULL));
    initialize_matrix(A);
    initialize_matrix(B);

    // Serial multiplication
    start_time = omp_get_wtime();
    multiply_matrices_serial(A, B, C);
    end_time = omp_get_wtime();
    serial_time = end_time - start_time;
    printf("Serial execution time: %f seconds\n", serial_time);

    // Parallel multiplication
    start_time = omp_get_wtime();
    multiply_matrices_parallel(A, B, C);
    end_time = omp_get_wtime();
    parallel_time = end_time - start_time;
    printf("Parallel execution time: %f seconds\n", parallel_time);

    // Calculate speedup
    double speedup = serial_time / parallel_time;
    printf("Speedup: %f\n", speedup);

    // Free allocated memory
    for (int i = 0; i < N; i++) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);

    return 0;
}