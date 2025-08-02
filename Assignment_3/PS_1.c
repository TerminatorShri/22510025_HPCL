#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int compare_asc(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int compare_desc(const void *a, const void *b) {
    return (*(int *)b - *(int *)a);
}

void generate_random_vector(int *vec, int n) {
    for (int i = 0; i < n; i++) {
        vec[i] = rand() % 100;
    }
}

long long min_scalar_product(int *A, int *B, int n, int num_threads) {
    qsort(A, n, sizeof(int), compare_asc);
    qsort(B, n, sizeof(int), compare_desc);

    long long result = 0;

    #pragma omp parallel for reduction(+:result) num_threads(num_threads)
    for (int i = 0; i < n; i++) {
        result += (long long)A[i] * B[i];
    }

    return result;
}

int main() {
    int sizes[] = {1000000, 10000000, 50000000};
    int threads[] = {1, 2, 4, 8};
    FILE *fp = fopen("PS_1.csv", "w");

    if (!fp) {
        perror("Error opening file");
        return 1;
    }

    srand(time(NULL));

    fprintf(fp, "Vector_Size,Threads,Time(s),Min_Scalar_Product\n");
    printf("%-12s %-8s %-10s %-20s\n", "Size", "Threads", "Time(s)", "Min Scalar Product");

    for (int s = 0; s < sizeof(sizes)/sizeof(sizes[0]); s++) {
        int n = sizes[s];

        for (int t = 0; t < sizeof(threads)/sizeof(threads[0]); t++) {
            int thread_count = threads[t];

            int *A = (int *)malloc(n * sizeof(int));
            int *B = (int *)malloc(n * sizeof(int));

            generate_random_vector(A, n);
            generate_random_vector(B, n);

            double start = omp_get_wtime();
            long long result = min_scalar_product(A, B, n, thread_count);
            double end = omp_get_wtime();
            double elapsed = end - start;

            printf("%-12d %-8d %-10.6f %-20lld\n", n, thread_count, elapsed, result);
            fprintf(fp, "%d,%d,%.6f,%lld\n", n, thread_count, elapsed, result);

            free(A);
            free(B);
        }
    }

    fclose(fp);
    return 0;
}
