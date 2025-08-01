#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

double calculate_pi(int num_steps, int num_threads) {
    double step = 1.0 / (double)num_steps;
    double sum = 0.0;

    #pragma omp parallel for num_threads(num_threads) reduction(+:sum)
    for (int i = 0; i < num_steps; i++) {
        double x = (i + 0.5) * step;
        sum += 1.0 / (1.0 + x * x);
    }

    return 4.0 * step * sum;
}

int main() {
    int steps[] = {1000000, 10000000, 50000000};     
    int threads[] = {1, 2, 4, 8};                    

    FILE *fp = fopen("PS_2.csv", "w");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }

    fprintf(fp, "Steps,Threads,Time(s),ApproxPi\n");

    printf("%-12s %-8s %-10s %-20s\n", "Steps", "Threads", "Time(s)", "Approx. Pi");

    for (int s = 0; s < sizeof(steps)/sizeof(steps[0]); s++) {
        int num_steps = steps[s];

        for (int t = 0; t < sizeof(threads)/sizeof(threads[0]); t++) {
            int num_threads = threads[t];
            double start = omp_get_wtime();

            double pi = calculate_pi(num_steps, num_threads);

            double end = omp_get_wtime();
            printf("%-12d %-8d %-10.6f %-20.15f\n", num_steps, num_threads, end - start, pi);

            fprintf(fp, "%d,%d,%.6f,%.15f\n", num_steps, num_threads, end - start, pi);
        }
    }

    fclose(fp);
    printf("Results saved to PS_2.csv\n");

    return 0;
}
