#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int bigdata[1000000]; 

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 3) {
        if (rank == 0) printf("Run with 3 processes.\n");
        MPI_Finalize();
        return 0;
    }

    int next = (rank + 1) % 3;
    int prev = (rank + 2) % 3;

    printf("Process %d sending to process %d\n", rank, next);
    MPI_Send(bigdata, 1000000, MPI_INT, next, 0, MPI_COMM_WORLD); 

    printf("Process %d receiving from process %d\n", rank, prev);
    MPI_Recv(bigdata, 1000000, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    printf("Process %d finished\n", rank);

    MPI_Finalize();
    return 0;
}
