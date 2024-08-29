#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

// Serial sum function
long long serialSum(int *array, size_t size) {
    long long sum = 0;
    for (size_t i = 0; i < size; i++) {
        sum += array[i];
    }
    return sum;
}

// Parallel sum function using OpenMP
long long parallelSum(int *array, size_t size) {
    long long sum = 0;

    #pragma omp parallel for reduction(+:sum)
    for (size_t i = 0; i < size; i++) {
        sum += array[i];
    }

    return sum;
}

int main() {
    size_t size = 1000000000; // 1000 million elements
    int *array = (int *)malloc(size * sizeof(int));
    if (array == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    // Initialize the array with 1s
    for (size_t i = 0; i < size; i++) {
        array[i] = 1;
    }

    // Measure time for serial sum
    clock_t start = clock();
    long long serialResult = serialSum(array, size);
    clock_t end = clock();
    double serialTime = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Serial Sum: %lld\n", serialResult);
    printf("Serial Time: %f seconds\n", serialTime);

    // Measure time for parallel sum
    start = clock();
    long long parallelResult = parallelSum(array, size);
    end = clock();
    double parallelTime = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Parallel Sum: %lld\n", parallelResult);
    printf("Parallel Time: %f seconds\n", parallelTime);

    // Calculate and display the speedup
    double speedup = serialTime / parallelTime;
    printf("Speedup: %f\n", speedup);

    // Free allocated memory
    free(array);

    return 0;
}
