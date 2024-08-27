#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Function to compare two elements in ascending order (used for sorting)
int compareAsc(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

// Function to compare two elements in descending order (used for sorting)
int compareDesc(const void *a, const void *b) {
    return (*(int*)b - *(int*)a);
}

// Parallel function to calculate the minimum scalar product
int minScalarProductParallel(int *a, int *b, int n) {
    // Parallel sorting is more complex; let's parallelize sorting and the scalar product computation

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            // Sort first vector in ascending order
            qsort(a, n, sizeof(int), compareAsc);
        }

        #pragma omp section
        {
            // Sort second vector in descending order
            qsort(b, n, sizeof(int), compareDesc);
        }
    }

    // Calculate the scalar product in parallel
    int product = 0;

    // calculated the product in parallel 
    #pragma omp parallel for reduction(+:product)   

    // each thread will calculate the product of the corresponding element of the array
    // reduction is used to calculate the sum of the product of the elements of the array
    // reduction(+:product) is used to calculate the sum of the product of the elements of the array

    for (int i = 0; i < n; i++) {
        product += a[i] * b[i];
    }

    return product;
}

int main() {
    int n = 900000;
    int *a = malloc(n * sizeof(int));
    int *b = malloc(n * sizeof(int));

    // Parallel initialization of arrays
    unsigned int seed;                      // Seed for random number generator it is used to generate random number for each thread 
    #pragma omp parallel private(seed)      // Seed must be private
    {
        seed = omp_get_thread_num();           // Get the thread number
        #pragma omp for                        // Distribute the work among threads
        for (int i = 0; i < n; i++) {
            a[i] = rand_r(&seed) % 100;
            b[i] = rand_r(&seed) % 100;
        }
    }

    double start_time = omp_get_wtime(); // Start timing

    int minProduct = minScalarProductParallel(a, b, n);

    double end_time = omp_get_wtime(); // End timing

    double execution_time = end_time - start_time;

    printf("For size of n = %d\n", n);
    printf("Minimum Scalar Product (Parallel): %d\n", minProduct);
    printf("Execution Time: %f seconds\n", execution_time);

    free(a);
    free(b);

    return 0;
}
