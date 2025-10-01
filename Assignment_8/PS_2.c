#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    int rank, size, N;
    double *vector_a = NULL, *vector_b = NULL;
    double *local_a, *local_b;
    double local_dot_product, global_dot_product;
    int elements_per_proc;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc < 2) {
        if (rank == 0) printf("Usage: %s <vector_size>\n", argv[0]);
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

    elements_per_proc = N / size;

    local_a = (double *)malloc(elements_per_proc * sizeof(double));
    local_b = (double *)malloc(elements_per_proc * sizeof(double));

    if (rank == 0) {
        vector_a = (double *)malloc(N * sizeof(double));
        vector_b = (double *)malloc(N * sizeof(double));
        
        srand(time(NULL));
        for (int i = 0; i < N; i++) {
            vector_a[i] = (double)(rand() % 100) / 10.0; 
            vector_b[i] = (double)(rand() % 100) / 10.0; 
        }
        
        FILE *fp = fopen("PS_2.csv", "r");
        if (fp == NULL) {
            fp = fopen("PS_2.csv", "w");
            if (fp != NULL) {
                fprintf(fp, "VectorSize,NumProcesses,TimeTaken\n");
                fclose(fp);
            }
        } else {
            fclose(fp);
        }
    }

    double start_time = MPI_Wtime();

    MPI_Scatter(vector_a, elements_per_proc, MPI_DOUBLE,
                local_a, elements_per_proc, MPI_DOUBLE,
                0, MPI_COMM_WORLD);
                
    MPI_Scatter(vector_b, elements_per_proc, MPI_DOUBLE,
                local_b, elements_per_proc, MPI_DOUBLE,
                0, MPI_COMM_WORLD);

    local_dot_product = 0.0;
    for (int i = 0; i < elements_per_proc; i++) {
        local_dot_product += local_a[i] * local_b[i];
    }

    MPI_Reduce(&local_dot_product, &global_dot_product, 1, MPI_DOUBLE,
               MPI_SUM, 0, MPI_COMM_WORLD);

    double end_time = MPI_Wtime();
    double exec_time = end_time - start_time;

    if (rank == 0) {
        FILE *fp = fopen("PS_2.csv", "a");
        if (fp == NULL) {
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        fprintf(fp, "%d,%d,%f\n", N, size, exec_time);
        fclose(fp);
    }

    free(local_a);
    free(local_b);
    
    if (rank == 0) {
        free(vector_a);
        free(vector_b);
    }

    MPI_Finalize();
    return 0;
}