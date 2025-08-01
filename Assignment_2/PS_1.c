#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void vector_scalar_addition(float *A, float *C, float scalar, int N, int num_threads) {
    #pragma omp parallel for num_threads(num_threads)
    for (int i = 0; i < N; i++) {
        C[i] = A[i] + scalar;
    }
}

int main() {
    int N_values[] = {1000000, 10000000, 50000000};  
    int thread_counts[] = {1, 2, 4, 8};              
    float scalar = 3.5;

    FILE *fp = fopen("PS_1.csv", "w");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }

    fprintf(fp, "DataSize,Threads,Time\n");

    printf("%-12s %-7s %s\n", "DataSize", "Threads", "Time(s)");
    printf("--------------------------------------------------\n");

    for (int ni = 0; ni < sizeof(N_values)/sizeof(N_values[0]); ni++) {
        int N = N_values[ni];

        float *A = (float *)malloc(N * sizeof(float));
        float *C = (float *)malloc(N * sizeof(float));

        for (int i = 0; i < N; i++) {
            A[i] = i * 0.5;
        }

        for (int t = 0; t < sizeof(thread_counts)/sizeof(thread_counts[0]); t++) {
            int num_threads = thread_counts[t];
            double start = omp_get_wtime();

            vector_scalar_addition(A, C, scalar, N, num_threads);

            double end = omp_get_wtime();
            double elapsed = end - start;

            printf("%-12d %-7d %.6f\n", N, num_threads, elapsed);

            fprintf(fp, "%d,%d,%.6f\n", N, num_threads, elapsed);
        }

        free(A);
        free(C);
    }

    fclose(fp);
    return 0;
}
