#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 1000000 // Size of the vector

// Parallely add a scalar to each element of the vector
void vectorScalarAdd(double *a, double scalar, int size) {
    #pragma omp parallel for
    for (int i = 0; i < size; i++) {
        a[i] += scalar;
    }
}

int main() {
    double *vector = (double*)malloc(N * sizeof(double));   // creating a vector of size N
    double scalar = 5.0;                                    // scalar value to be added             

    // Initialize the vector
    for (int i = 0; i < N; i++) {
        vector[i] = i * 1.0;
    }

    // First 5 elements before addition
    printf("First 5 elements before addition:\n");
    for (int i = 0; i < 5; i++) {
        printf("%.2f ", vector[i]);
    }

    // Last 5 elements before addition
    printf("\nLast 5 elements before addition:\n");
    for (int i = N - 5; i < N; i++) {
        printf("%.2f ", vector[i]);
    }

    // Perform vector scalar addition
    double start_time = omp_get_wtime();        // Get the start time
    vectorScalarAdd(vector, scalar, N);         // Perform vector scalar addition
    double end_time = omp_get_wtime();          // Get the end time

    // Print some results for verification
    printf("First 5 elements after addition:\n");
    for (int i = 0; i < 5; i++) {
        printf("%.2f ", vector[i]);
    }
    printf("\n");

    printf("Last 5 elements after addition:\n");
    for (int i = N - 5; i < N; i++) {
        printf("%.2f ", vector[i]);
    }
    printf("\n");

    printf("Time taken: %f seconds\n", end_time - start_time);

    free(vector);       
    return 0;
}