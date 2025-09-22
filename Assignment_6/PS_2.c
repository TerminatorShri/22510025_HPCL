#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int rank, size;
    MPI_Group world_group;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Comm_group(MPI_COMM_WORLD, &world_group);

    printf("Process %d of %d is in communicator MPI_COMM_WORLD\n", rank, size);

    MPI_Group_free(&world_group);
    MPI_Finalize();

    return 0;
}
