#include <stdio.h>
#include <omp.h>

#define VECTOR_SIZE 200

void vector_scalar_add_static(int *a, int scalar, int chunk_size) {
    #pragma omp parallel for schedule(static, chunk_size)
    for (int i = 0; i < VECTOR_SIZE; i++) {
        a[i] += scalar;
    }
}

void vector_scalar_add_dynamic(int *a, int scalar, int chunk_size) {
    #pragma omp parallel for schedule(dynamic, chunk_size)
    for (int i = 0; i < VECTOR_SIZE; i++) {
        a[i] += scalar;
    }
}

void vector_scalar_add_nowait(int *a, int scalar) {
    #pragma omp parallel
    {
        #pragma omp for nowait
        for (int i = 0; i < VECTOR_SIZE; i++) {
            a[i] += scalar;
        }

        // Additional work that can be done without waiting for the loop to complete
        // This simulates potential workload that benefits from using nowait
        #pragma omp single
        {
            printf("Thread %d did some work without waiting for the loop.\n", omp_get_thread_num());
        }
    }
}

int main() {
    int a[VECTOR_SIZE];
    int scalar = 5;

    // Initialize the vector
    for (int i = 0; i < VECTOR_SIZE; i++) {
        a[i] = i;
    }

    double start_time, end_time;

    // Test STATIC scheduling with different chunk sizes
    printf("STATIC Scheduling:\n");
    for (int chunk_size = 1; chunk_size <= VECTOR_SIZE; chunk_size *= 10) {
        start_time = omp_get_wtime();
        vector_scalar_add_static(a, scalar, chunk_size);
        end_time = omp_get_wtime();
        printf("Chunk Size: %d, Execution Time: %f seconds\n", chunk_size, end_time - start_time);
    }

    // Test DYNAMIC scheduling with different chunk sizes
    printf("\nDYNAMIC Scheduling:\n");
    for (int chunk_size = 1; chunk_size <= VECTOR_SIZE; chunk_size *= 10) {
        start_time = omp_get_wtime();
        vector_scalar_add_dynamic(a, scalar, chunk_size);
        end_time = omp_get_wtime();
        printf("Chunk Size: %d, Execution Time: %f seconds\n", chunk_size, end_time - start_time);
    }

    // Demonstrate the use of nowait clause
    printf("\nDemonstrating nowait clause:\n");
    vector_scalar_add_nowait(a, scalar);

    return 0;
}
