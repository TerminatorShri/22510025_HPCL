#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2) {
        if (rank == 0) printf("This program requires exactly 2 processes.\n");
        MPI_Finalize();
        return 0;
    }

    int n = 10;  
    int A[10] = {1,2,3,4,5,6,7,8,9,10};
    int local_sum = 0;

    if (rank == 0) {
        for (int i = 0; i < n/2; i++)
            local_sum += A[i];
        MPI_Send(&local_sum, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    } else if (rank == 1) {
        for (int i = n/2; i < n; i++)
            local_sum += A[i];
        int sum0;
        MPI_Recv(&sum0, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        int total_sum = local_sum + sum0;
        printf("Sum of array elements = %d\n", total_sum);
    }

    MPI_Finalize();
    return 0;
}
