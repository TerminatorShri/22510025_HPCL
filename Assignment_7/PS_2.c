#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int rank, size, N;
    int *A = NULL, *B = NULL, *local_A, *local_C, *C = NULL;
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

    local_A = (int *)malloc(rows_per_proc * N * sizeof(int));
    local_C = (int *)malloc(rows_per_proc * N * sizeof(int));
    B = (int *)malloc(N * N * sizeof(int));  

    if (rank == 0) {
        A = (int *)malloc(N * N * sizeof(int));
        C = (int *)malloc(N * N * sizeof(int));

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                A[i * N + j] = (i + j + 1) % 10;
                B[i * N + j] = (i + j + 2) % 10;
            }
        }
    }

    MPI_Bcast(B, N * N, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Scatter(A, rows_per_proc * N, MPI_INT,
                local_A, rows_per_proc * N, MPI_INT,
                0, MPI_COMM_WORLD);

    double start_time = MPI_Wtime();

    for (int i = 0; i < rows_per_proc; i++) {
        for (int j = 0; j < N; j++) {
            local_C[i * N + j] = 0;
            for (int k = 0; k < N; k++) {
                local_C[i * N + j] += local_A[i * N + k] * B[k * N + j];
            }
        }
    }

    MPI_Gather(local_C, rows_per_proc * N, MPI_INT,
               C, rows_per_proc * N, MPI_INT,
               0, MPI_COMM_WORLD);

    double end_time = MPI_Wtime();
    double exec_time = end_time - start_time;

    if (rank == 0) {
        FILE *fp = fopen("PS_2.csv", "a");
        if (!fp) {
            printf("Error opening file!\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        fprintf(fp, "%d,%d,%f\n", N, size, exec_time);
        fclose(fp);
    }

    free(local_A);
    free(local_C);
    free(B);
    if (rank == 0) {
        free(A);
        free(C);
    }

    MPI_Finalize();
    return 0;
}
