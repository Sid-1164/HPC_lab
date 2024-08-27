#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to compare two elements (used for sorting)
int compare(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

// Function to calculate the minimum scalar product
int minScalarProduct(int *a, int *b, int n) {
    // Sort first vector in ascending order
    qsort(a, n, sizeof(int), compare);

    // Sort second vector in descending order
    // sort in acending order first
    qsort(b, n, sizeof(int), compare);
    // then Reverse the second vector
    for (int i = 0; i < n / 2; i++) {
        int temp = b[i];
        b[i] = b[n - i - 1];
        b[n - i - 1] = temp;
    }

    // Calculate the scalar product
    int product = 0;
    for (int i = 0; i < n; i++) {
        product += a[i] * b[i];
    }

    return product;
}

int main() {

    int n = 900000;
    int a[900000];
    int b[900000];


    // Generate random values for arrays a and b
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        a[i] = rand() % 100 + 1;
        b[i] = rand() % 100 + 1;
    }

    clock_t start = clock(); // Start the timer

    int minProduct = minScalarProduct(a, b, n);

    clock_t end = clock(); // Stop the timer

    double executionTime = (double)(end - start) / CLOCKS_PER_SEC;

    printf("For size of n = %d\n", n);
    printf("Minimum Scalar Product: %d\n", minProduct);
    printf("Execution Time: %.6f seconds\n", executionTime);

    return 0;
}
