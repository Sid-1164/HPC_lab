#include <iostream>
#include <omp.h>

int main()
{
    #pragma omp parallel         //  This will create multiple threads
    {
        std::cout << "Hello, world." << omp_get_thread_num() << std::endl;  //  This will be executed by multiple threads
        std::cout << "-------------------------------------------------" << std::endl;
    }
    return 0;
}
