#include <iostream>
#include <omp.h>

int main()
{
    int num_threads;

    // Ask the user for the number of threads
    std::cout << "Enter the number of threads: ";
    std::cin >> num_threads;

    // Set the number of threads
    omp_set_num_threads(num_threads);

    // Print "Hello, World" sequentially
    std::cout << "Sequential output:" << std::endl;
    for (int i = 0; i < num_threads; ++i) {
        std::cout << "Hello, World" << std::endl;
    }

    // Print "Hello, World" in parallel
    std::cout << "Parallel output:" << std::endl;
    #pragma omp parallel
    {
        #pragma omp critical
        {
            std::cout << "Hello, World from thread " << omp_get_thread_num() << std::endl;
        }
    }

    return 0;
}
