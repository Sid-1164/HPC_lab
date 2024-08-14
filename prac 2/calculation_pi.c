#include <stdio.h>
#include <omp.h>

double calculate_pi(long long num_steps, int num_threads) {
    double step = 1.0 / (double) num_steps;             // width of each step
    double pi = 0.0;                                    // to store final result

    omp_set_num_threads(num_threads);                   // set number of threads

    #pragma omp parallel                                // parallel region
    {
        double x, sum = 0.0;                            // local variables for each thread
        #pragma omp for                                         // parallel for loop
        for (long long i = 0; i < num_steps; i++) {
            x = (i + 0.5) * step;                           // midpoint of each step               
            sum += 4.0 / (1.0 + x * x);                     // sum of all midpoints
        }
        #pragma omp critical                                // critical section -> only one thread can execute at a time
        pi += sum;                                        // add sum of each thread to pi                               
    }

    return pi * step;                                       // multiply by step to get final result
}

int main() {
    int thread_counts[] = {2, 4, 6, 8, 10, 12};                             // different number of threads
    long long data_sizes[] = {1000000, 10000000, 100000000, 1000000000};    // different data sizes

    for (int i = 0; i < sizeof(data_sizes) / sizeof(data_sizes[0]); i++) {
        printf("Data size: %lld\n", data_sizes[i]);
        
        // for all combinations
        for (int j = 0; j < sizeof(thread_counts) / sizeof(thread_counts[0]); j++) {
            double start_time = omp_get_wtime();
            double pi = calculate_pi(data_sizes[i], thread_counts[j]);
            double end_time = omp_get_wtime();

            printf("Threads: %d, Pi: %.10f, Time: %.6f seconds\n", 
                   thread_counts[j], pi, end_time - start_time);
            
            printf("----------------------------------------------------------------------------------");
        }
        printf("___________________________________________________________________________________\n");
        printf("\n");
    }

    return 0;
}