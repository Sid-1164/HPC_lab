#include <stdio.h>
#include <omp.h>
#include <unistd.h> // For usleep

#define BUFFER_SIZE 10

int buffer[BUFFER_SIZE];
int count = 0;
int in = 0;
int out = 0;

void produce(int item) {
    while (1) {
        if (count < BUFFER_SIZE) {
            buffer[in] = item;
            in = (in + 1) % BUFFER_SIZE;
            count++;
            printf("Produced: %d\n", item);
            break; // Exit the loop when done
        }
        usleep(100); // Sleep to allow other thread to run
    }
}

int consume() {
    int item;
    while (1) {
        if (count > 0) {
            item = buffer[out];
            out = (out + 1) % BUFFER_SIZE;
            count--;
            printf("Consumed: %d\n", item);
            break; // Exit the loop when done
        }
        usleep(100); // Sleep to allow other thread to run
    }
    return item;
}

int main() {
    #pragma omp parallel sections       // Section will be executed by different threads in parallel
    {
        #pragma omp section             // Section 1
        {
            for (int i = 0; i < 20; i++) {
                produce(i);
                usleep(100); // Simulate work
            }
        }

        #pragma omp section             // Section 2
        {
            for (int i = 0; i < 20; i++) {
                consume();
                usleep(150); // Simulate work
            }
        }
    }

    return 0;
}
