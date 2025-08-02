#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

void generate_random_matrix(int **matrix, int size) {
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            matrix[i][j] = rand() % 100;
}

void matrix_addition(int **A, int **B, int **C, int size, int threads) {
    #pragma omp parallel for collapse(2) num_threads(threads)
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            C[i][j] = A[i][j] + B[i][j];
}

int **allocate_matrix(int size) {
    int **matrix = (int **)malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++)
        matrix[i] = (int *)malloc(size * sizeof(int));
    return matrix;
}

void free_matrix(int **matrix, int size) {
    for (int i = 0; i < size; i++)
        free(matrix[i]);
    free(matrix);
}

int main() {
    int sizes[] = {250, 500, 750, 1000, 2000};
    int thread_counts[] = {1, 2, 4, 8};

    FILE *fp = fopen("PS_2.csv", "w");
    if (!fp) {
        perror("Error opening file");
        return 1;
    }

    srand(time(NULL));
    fprintf(fp, "Matrix_Size,Threads,Time(s)\n");
    printf("%-12s %-8s %-10s\n", "Size", "Threads", "Time(s)");

    for (int s = 0; s < sizeof(sizes) / sizeof(sizes[0]); s++) {
        int size = sizes[s];

        for (int t = 0; t < sizeof(thread_counts) / sizeof(thread_counts[0]); t++) {
            int threads = thread_counts[t];

            int **A = allocate_matrix(size);
            int **B = allocate_matrix(size);
            int **C = allocate_matrix(size);

            generate_random_matrix(A, size);
            generate_random_matrix(B, size);

            double start = omp_get_wtime();
            matrix_addition(A, B, C, size, threads);
            double end = omp_get_wtime();
            double elapsed = end - start;

            printf("%-12d %-8d %-10.6f\n", size, threads, elapsed);
            fprintf(fp, "%d,%d,%.6f\n", size, threads, elapsed);

            free_matrix(A, size);
            free_matrix(B, size);
            free_matrix(C, size);
        }
    }

    fclose(fp);
    return 0;
}
