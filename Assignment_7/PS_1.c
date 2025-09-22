#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

static int header_printed = 0;

int main(int argc, char *argv[]) {
    int rank, size, N;
    int *A = NULL, *x = NULL, *local_rows, *local_result, *final_result;
    int rows_per_proc;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc < 2) {
        if (rank == 0) printf("Usage: %s <matrix_size>\n", argv[0]);
        MPI_Finalize();
        return 0;
    }

    N = atoi(argv[1]);
    if (N % size != 0) {
        if (rank == 0) 
            printf("Error: N (%d) must be divisible by number of processes (%d)\n", N, size);
        MPI_Finalize();
        return 0;
    }

    rows_per_proc = N / size;

    x = (int *)malloc(N * sizeof(int));
    local_rows = (int *)malloc(rows_per_proc * N * sizeof(int));
    local_result = (int *)malloc(rows_per_proc * sizeof(int));

    if (rank == 0) {
        A = (int *)malloc(N * N * sizeof(int));
        final_result = (int *)malloc(N * sizeof(int));

        for (int i = 0; i < N; i++) {
            x[i] = i + 1;
            for (int j = 0; j < N; j++) {
                A[i * N + j] = (i + j + 1) % 10;
            }
        }
    }

    MPI_Bcast(x, N, MPI_INT, 0, MPI_COMM_WORLD);

    double start_time = MPI_Wtime();

    MPI_Scatter(A, rows_per_proc * N, MPI_INT,
                local_rows, rows_per_proc * N, MPI_INT,
                0, MPI_COMM_WORLD);

    for (int i = 0; i < rows_per_proc; i++) {
        local_result[i] = 0;
        for (int j = 0; j < N; j++) {
            local_result[i] += local_rows[i * N + j] * x[j];
        }
    }

    MPI_Gather(local_result, rows_per_proc, MPI_INT,
               final_result, rows_per_proc, MPI_INT,
               0, MPI_COMM_WORLD);

    double end_time = MPI_Wtime();
    double exec_time = end_time - start_time;

    if (rank == 0) {
        FILE *fp = fopen("PS_1.csv", "a");
        if (fp == NULL) {
            printf("Error opening file!\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        fprintf(fp, "%d,%d,%f\n", N, size, exec_time);
        fclose(fp);
    }

    free(x);
    free(local_rows);
    free(local_result);
    if (rank == 0) {
        free(A);
        free(final_result);
    }

    MPI_Finalize();
    return 0;
}
