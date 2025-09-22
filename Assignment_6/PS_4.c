#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int send_data, recv_data;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        if (rank == 0) printf("Run with at least 2 processes.\n");
        MPI_Finalize();
        return 0;
    }

    send_data = rank;                     
    int next = (rank + 1) % size;         
    int prev = (rank + size - 1) % size;  

    if (rank % 2 == 0) {
        MPI_Send(&send_data, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
        MPI_Recv(&recv_data, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else {
        MPI_Recv(&recv_data, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(&send_data, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
    }

    printf("Process %d sent %d to process %d and received %d from process %d\n",
           rank, send_data, next, recv_data, prev);

    MPI_Finalize();
    return 0;
}
