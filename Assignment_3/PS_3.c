#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define VECTOR_SIZE 1000000
#define SCALAR 10

void generate_vector(int *vec, int size) {
    for (int i = 0; i < size; i++)
        vec[i] = rand() % 100;
}

void vector_scalar_add_static(int *vec, int *result, int size, int chunk, int threads) {
    #pragma omp parallel for schedule(static, chunk) num_threads(threads)
    for (int i = 0; i < size; i++) {
        result[i] = vec[i] + SCALAR;
    }
}

void vector_scalar_add_dynamic(int *vec, int *result, int size, int chunk, int threads) {
    #pragma omp parallel for schedule(dynamic, chunk) num_threads(threads)
    for (int i = 0; i < size; i++) {
        result[i] = vec[i] + SCALAR;
    }
}

void nowait_demo(int *vec, int *res1, int *res2, int size, int threads) {
    #pragma omp parallel num_threads(threads)
    {
        #pragma omp for nowait
        for (int i = 0; i < size / 2; i++)
            res1[i] = vec[i] + 1;

        #pragma omp for nowait
        for (int i = size / 2; i < size; i++)
            res2[i] = vec[i] + 2;
    }
}

int main() {
    int chunk_sizes[] = {1, 5, 10, 20, 50};
    int thread_counts[] = {1, 2, 4, 8};

    int *vec = (int *)malloc(VECTOR_SIZE * sizeof(int));
    int *res_static = (int *)malloc(VECTOR_SIZE * sizeof(int));
    int *res_dynamic = (int *)malloc(VECTOR_SIZE * sizeof(int));
    int *res1 = (int *)calloc(VECTOR_SIZE, sizeof(int));
    int *res2 = (int *)calloc(VECTOR_SIZE, sizeof(int));

    srand(time(NULL));
    generate_vector(vec, VECTOR_SIZE);

    FILE *fs = fopen("PS_3_Static.csv", "w");
    FILE *fd = fopen("PS_3_Dynamic.csv", "w");
    fprintf(fs, "Chunk_Size,Threads,Time(s)\n");
    fprintf(fd, "Chunk_Size,Threads,Time(s)\n");

    printf("STATIC SCHEDULING:\n");
    printf("%-12s %-8s %-10s\n", "Chunk", "Threads", "Time(s)");
    for (int c = 0; c < sizeof(chunk_sizes)/sizeof(chunk_sizes[0]); c++) {
        for (int t = 0; t < sizeof(thread_counts)/sizeof(thread_counts[0]); t++) {
            double start = omp_get_wtime();
            vector_scalar_add_static(vec, res_static, VECTOR_SIZE, chunk_sizes[c], thread_counts[t]);
            double end = omp_get_wtime();
            double elapsed = end - start;

            printf("%-12d %-8d %-10.6f\n", chunk_sizes[c], thread_counts[t], elapsed);
            fprintf(fs, "%d,%d,%.6f\n", chunk_sizes[c], thread_counts[t], elapsed);
        }
    }

    printf("\nDYNAMIC SCHEDULING:\n");
    printf("%-12s %-8s %-10s\n", "Chunk", "Threads", "Time(s)");
    for (int c = 0; c < sizeof(chunk_sizes)/sizeof(chunk_sizes[0]); c++) {
        for (int t = 0; t < sizeof(thread_counts)/sizeof(thread_counts[0]); t++) {
            double start = omp_get_wtime();
            vector_scalar_add_dynamic(vec, res_dynamic, VECTOR_SIZE, chunk_sizes[c], thread_counts[t]);
            double end = omp_get_wtime();
            double elapsed = end - start;

            printf("%-12d %-8d %-10.6f\n", chunk_sizes[c], thread_counts[t], elapsed);
            fprintf(fd, "%d,%d,%.6f\n", chunk_sizes[c], thread_counts[t], elapsed);
        }
    }

    fclose(fs);
    fclose(fd);

    printf("\nNOWAIT CLAUSE DEMO (partial result shown):\n");
    nowait_demo(vec, res1, res2, VECTOR_SIZE, 4);
    for (int i = 0; i < 10; i++)
        printf("res1[%d] = %d, res2[%d] = %d\n", i, res1[i], i, res2[i]);

    free(vec);
    free(res_static);
    free(res_dynamic);
    free(res1);
    free(res2);

    return 0;
}
