#include <stdio.h>
#include <omp.h>

#define SIZE 1000000

int main() {
    int array[SIZE];
    long long total_sum = 0;
    double start_time, end_time;

    // Initialize the array
    for (int i = 0; i < SIZE; i++) {
        array[i] = 1;  // Simple initialization
    }

    // Measure time for unsynchronized version
    start_time = omp_get_wtime();
    #pragma omp parallel
    {
        long long local_sum = 0;

        #pragma omp for
        for (int i = 0; i < SIZE; i++) {
            local_sum += array[i];
        }

        // Update the global total_sum
        total_sum += local_sum;
    }
    end_time = omp_get_wtime();
    printf("Total sum (unsynchronized): %lld\n", total_sum);
    printf("Time taken (unsynchronized): %f seconds\n", end_time - start_time);

    total_sum = 0;  // Reset total_sum

    // Measure time for synchronized version using critical
    start_time = omp_get_wtime();
    #pragma omp parallel
    {
        long long local_sum = 0;

        #pragma omp for
        for (int i = 0; i < SIZE; i++) {
            local_sum += array[i];
        }

        // Update the global total_sum using critical section
        #pragma omp critical
        {
            total_sum += local_sum;
        }
    }
    end_time = omp_get_wtime();
    double time_critical = end_time - start_time;
    printf("Total sum (using critical): %lld\n", total_sum);
    printf("Time taken (using critical): %f seconds\n", time_critical);

    total_sum = 0;  // Reset total_sum

    // Measure time for synchronized version using atomic
    start_time = omp_get_wtime();
    #pragma omp parallel
    {
        long long local_sum = 0;

        #pragma omp for
        for (int i = 0; i < SIZE; i++) {
            local_sum += array[i];
        }

        // Update the global total_sum using atomic directive
        #pragma omp atomic
        total_sum += local_sum;
    }
    end_time = omp_get_wtime();
    printf("Total sum (using atomic): %lld\n", total_sum);
    printf("Time taken (using atomic): %f seconds\n", end_time - start_time);

    // Speedup 
    printf("Speedup (using critical): %f\n", time_critical / (end_time - start_time));

    

    return 0;
}
