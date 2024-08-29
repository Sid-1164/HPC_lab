#include <stdio.h>
#include <omp.h>

int main() {
    int Aryabhatta = 10;

    #pragma omp parallel private(Aryabhatta)
    {
        int thread_id = omp_get_thread_num();
        Aryabhatta = 10;  // Ensure each thread has its own copy of Aryabhatta with the value 10
        int result = thread_id * Aryabhatta;

        printf("Thread ID: %d, Result: %d\n", thread_id, result);
    }

    return 0;
}
