#include <stdio.h>
#include <omp.h>

#define VECTOR_SIZE 200

// Difference between Static and Dynamic scheduling
// Static scheduling divides the loop iterations into chunks of size chunk_size and assigns each chunk to a thread in a round-robin fashion.
// Dynamic scheduling assigns a chunk of size chunk_size to a thread, and when the thread finishes processing the chunk, it is assigned another chunk.

/*
Simple example : 
Let's say we have a loop with 100 iterations and 4 threads.

With Static scheduling and chunk size of 1, the iterations are divided as follows:
Thread 0: 0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, 68, 72, 76, 80, 84, 88, 92, 96, 
Thread 1: 1, 5, 9, 13, 17, 21, 25, 29, 33, 37, 41, 45, 49, 53, 57, 61, 65, 69, 73, 77, 81, 85, 89, 93, 97,
Thread 2: 2, 6, 10, 14, 18, 22, 26, 30, 34, 38, 42, 46, 50, 54, 58, 62, 66, 70, 74, 78, 82, 86, 90, 94, 98,
Thread 3: 3, 7, 11, 15, 19, 23, 27, 31, 35, 39, 43, 47, 51, 55, 59, 63, 67, 71, 75, 79, 83, 87, 91, 95, 99

doesn't matter who findish first, the next chunk will be assigned in round-robin fashion.

With Dynamic scheduling and chunk size of 1, the iterations are divided as follows:

Thread 0: might get more than Thread 1 
Thread 1: might get more than Thread 2

Example:
Thread 0: 0, 1, 2
Thread 1: 3, 4
Thread 0: 5, 6
Thread 2: 7, 8
Thread 3: 9, 10
Thread 0: 11, 12, 13, 14
Thread 1: 15, 16, 17
Thread 2: 18, 19
Thread 3: 20, 21

Whoever finishes first will get the next chunk.

*/

void vector_scalar_add_static(int *a, int scalar, int chunk_size) {
    #pragma omp parallel for schedule(static, chunk_size)   // Static scheduling with chunk size of 1
    for (int i = 0; i < VECTOR_SIZE; i++) {
        a[i] += scalar;
    }
}

void vector_scalar_add_dynamic(int *a, int scalar, int chunk_size) {
    #pragma omp parallel for schedule(dynamic, chunk_size)  // Dynamic scheduling with chunk size of 1
    for (int i = 0; i < VECTOR_SIZE; i++) {
        a[i] += scalar;
    }
}

void vector_scalar_add_nowait(int *a, int scalar) {
    #pragma omp parallel        // Parallel region
    {
        #pragma omp for nowait  // No wait for the loop to complete before continuing to the next block of code
        for (int i = 0; i < VECTOR_SIZE; i++) {
            a[i] += scalar;
        }

        // Total no. of threads 
        #pragma omp single
        printf("Total number of threads: %d\n", omp_get_num_threads());

        // Additional work that can be done without waiting for the loop to complete
        #pragma omp single  // Only one thread executes this block of code (thread which reaches this block first)
        {
            printf("Thread %d did some work without waiting for the loop to complete because it reached this block of code first.\n", omp_get_thread_num());
        }
    }
}

int main() {
    int a[VECTOR_SIZE];     // Intialize the vector
    int scalar = 5;         // Scalar value to add to the vector

    // Initialize the vector
    for (int i = 0; i < VECTOR_SIZE; i++) {
        a[i] = i;           // 0 to 199
    }

    double start_time, end_time;

    // Test STATIC scheduling with different chunk sizes
    printf("STATIC Scheduling:\n");
    for (int chunk_size = 1; chunk_size <= VECTOR_SIZE*10; chunk_size *= 10) { // Chunk size of 1, 10, 100, 1000
        start_time = omp_get_wtime();
        vector_scalar_add_static(a, scalar, chunk_size);
        end_time = omp_get_wtime();
        printf("Chunk Size: %d, Execution Time: %f seconds\n", chunk_size, end_time - start_time);
    }
    printf("----------------------------------------------------\n");

    // Test DYNAMIC scheduling with different chunk sizes
    printf("\nDYNAMIC Scheduling:\n");
    for (int chunk_size = 1; chunk_size <= VECTOR_SIZE*10; chunk_size *= 10) {     // Chunk size of 1, 10, 100, 1000
        start_time = omp_get_wtime();
        vector_scalar_add_dynamic(a, scalar, chunk_size);
        end_time = omp_get_wtime();
        printf("Chunk Size: %d, Execution Time: %f seconds\n", chunk_size, end_time - start_time);
    }
    printf("----------------------------------------------------\n");

    // Demonstrate the use of nowait clause -> Additional work that can be done without waiting for the loop to complete
    printf("\nDemonstrating nowait clause:\n");
    vector_scalar_add_nowait(a, scalar);                    // No chunk size 
    printf("----------------------------------------------------\n");

    return 0;
}
