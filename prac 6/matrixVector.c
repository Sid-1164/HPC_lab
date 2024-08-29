#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define N 30000 // Matrix size (N x N) and vector size (N)

void initialize_matrix(double **matrix) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = rand() % 10;
        }
    }
}

void initialize_vector(double *vector) {
    for (int i = 0; i < N; i++) {
        vector[i] = rand() % 10;
    }
}

void matrix_vector_multiply_serial(double **A, double *x, double *y) {
    for (int i = 0; i < N; i++) {
        y[i] = 0.0;
        for (int j = 0; j < N; j++) {
            y[i] += A[i][j] * x[j];
        }
    }
}

void matrix_vector_multiply_parallel(double **A, double *x, double *y) {
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        y[i] = 0.0;
        for (int j = 0; j < N; j++) {
            y[i] += A[i][j] * x[j];
        }
    }
}

int main() {
    double **A, *x, *y;
    double start_time, end_time, serial_time, parallel_time;

    // Allocate memory
    A = (double **)malloc(N * sizeof(double *));
    for (int i = 0; i < N; i++) {
        A[i] = (double *)malloc(N * sizeof(double));
    }
    x = (double *)malloc(N * sizeof(double));
    y = (double *)malloc(N * sizeof(double));

    // Initialize matrix and vector
    srand(time(NULL));
    initialize_matrix(A);
    initialize_vector(x);

    // Serial multiplication
    start_time = omp_get_wtime();
    matrix_vector_multiply_serial(A, x, y);
    end_time = omp_get_wtime();
    serial_time = end_time - start_time;
    printf("Serial execution time: %f seconds\n", serial_time);

    // Parallel multiplication
    start_time = omp_get_wtime();
    matrix_vector_multiply_parallel(A, x, y);
    end_time = omp_get_wtime();
    parallel_time = end_time - start_time;
    printf("Parallel execution time: %f seconds\n", parallel_time);

    // Calculate speedup
    double speedup = serial_time / parallel_time;
    printf("Speedup: %f\n", speedup);

    // Calculate efficiency
    int num_threads = omp_get_max_threads();
    double efficiency = speedup / num_threads;
    printf("Efficiency: %f\n", efficiency);

    // Free allocated memory
    for (int i = 0; i < N; i++) {
        free(A[i]);
    }
    free(A);
    free(x);
    free(y);

    return 0;
}