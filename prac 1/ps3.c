#include <stdio.h>
#include <unistd.h>

int main(void) {
    int num_cores;
    FILE *fp;
    char buffer[128];
    double clock_speed = 0.0;

    // Retrieve the number of CPU cores
    num_cores = sysconf(_SC_NPROCESSORS_ONLN);
    if (num_cores == -1) {
        perror("sysconf");
        return 1;
    }
    printf("Number of CPU cores: %d\n", num_cores);

    // Retrieve the CPU clock speed
    fp = fopen("/proc/cpuinfo", "r");
    if (fp == NULL) {
        perror("Failed to open /proc/cpuinfo");
        return 1;
    }

    // Scan /proc/cpuinfo for "cpu MHz" line
    while (fgets(buffer, sizeof(buffer), fp)) {
        if (sscanf(buffer, "cpu MHz : %lf", &clock_speed) == 1) {
            printf("CPU Clock Speed: %.2f MHz\n", clock_speed);
            break; // Assuming all CPUs have the same clock speed
        }
    }

    fclose(fp);
    
    // Note: Determining FLOP per cycle is architecture-specific and is generally 
    // obtained from the CPU documentation or performance analysis tools.
    // For illustrative purposes, you can refer to the CPU documentation for this information.

    return 0;
}
