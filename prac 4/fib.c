#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

// Recursive function to compute Fibonacci using OpenMP tasks
long long int fib_para(int n) {
    long long int x, y;

    if (n < 2) {
        return n;
    } else {
        #pragma omp task shared(x) if(n > 10) // Creating a task for the first recursive call
        x = fib_para(n - 1);

        #pragma omp task shared(y) if(n > 10) // Creating a task for the second recursive call
        y = fib_para(n - 2);

        #pragma omp taskwait // Wait for both tasks to complete
        return x + y;
    }
}

long long int fib(int n) {
    if (n < 2) {
        return n;
    } else {
        return fib(n - 1) + fib(n - 2);
    }
}

int main() {
    int n = 45; // Fibonacci number to compute
    long long int result; // Change from int to long long int to handle large Fibonacci numbers

    // Using Simple Sequential Code
    double start_time = omp_get_wtime(); // Start timing    
    result = fib(n);    
    double end_time = omp_get_wtime(); // End timing
    double execution_time = end_time - start_time;
    
    printf("Fibonacci number F(%d) = %lld, using Sequential Code\n", n, result);
    printf("Execution Time: %f seconds\n", execution_time);

    // Using Parallel Code
    start_time = omp_get_wtime(); // Start timing

    #pragma omp parallel
    {
        #pragma omp single // Ensure that the Fibonacci computation starts in a single thread
        {
            result = fib_para(n);
        }
    }

    end_time = omp_get_wtime(); // End timing
    execution_time = end_time - start_time;

    printf("Fibonacci number F(%d) = %lld, using Parallel Code\n", n, result);
    printf("Execution Time: %f seconds\n", execution_time);

    return 0;
}
