#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int header_printed = 0;

void apply_convolution(int *input_chunk, int *output_chunk, int *kernel, 
                      int chunk_rows, int cols, int kernel_size, 
                      int start_row, int total_rows) {
    int kernel_center = kernel_size / 2;
    
    for (int i = 0; i < chunk_rows; i++) {
        for (int j = 0; j < cols; j++) {
            int sum = 0;
            
            for (int ki = 0; ki < kernel_size; ki++) {
                for (int kj = 0; kj < kernel_size; kj++) {
                    int row_idx = (start_row + i) + ki - kernel_center;
                    int col_idx = j + kj - kernel_center;
                    
                    if (row_idx >= 0 && row_idx < total_rows && 
                        col_idx >= 0 && col_idx < cols) {
                        int target_process = row_idx / chunk_rows;
                        int local_row = row_idx % chunk_rows;
                        
                        if (target_process * chunk_rows + local_row < total_rows) {
                            sum += input_chunk[(local_row * cols) + col_idx] * 
                                   kernel[ki * kernel_size + kj];
                        }
                    }
                }
            }
            output_chunk[i * cols + j] = sum;
        }
    }
}

int main(int argc, char *argv[]) {
    int rank, size, N, kernel_size;
    int *input_matrix = NULL, *output_matrix = NULL;
    int *local_input, *local_output, *kernel;
    int rows_per_proc;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc < 2) {
        if (rank == 0) printf("Usage: %s <matrix_size> [kernel_size]\n", argv[0]);
        MPI_Finalize();
        return 0;
    }

    N = atoi(argv[1]);
    kernel_size = (argc > 2) ? atoi(argv[2]) : 3; 
    
    if (N % size != 0) {
        if (rank == 0) 
            printf("Error: N (%d) must be divisible by number of processes (%d)\n", N, size);
        MPI_Finalize();
        return 0;
    }

    rows_per_proc = N / size;

    // Allocate memory for local data
    local_input = (int *)malloc(rows_per_proc * N * sizeof(int));
    local_output = (int *)malloc(rows_per_proc * N * sizeof(int));
    kernel = (int *)malloc(kernel_size * kernel_size * sizeof(int));

    if (rank == 0) {
        input_matrix = (int *)malloc(N * N * sizeof(int));
        output_matrix = (int *)malloc(N * N * sizeof(int));
        
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                input_matrix[i * N + j] = (i + j) % 10 + 1;
            }
        }
        
        if (kernel_size == 3) {
            int edge_kernel[9] = {
                -1, -1, -1,
                -1,  8, -1,
                -1, -1, -1
            };
            memcpy(kernel, edge_kernel, 9 * sizeof(int));
        } else {
            for (int i = 0; i < kernel_size * kernel_size; i++) {
                kernel[i] = 1;
            }
        }
        
        FILE *fp = fopen("PS_1.csv", "r");
        if (fp == NULL) {
            fp = fopen("PS_1.csv", "w");
            if (fp != NULL) {
                fprintf(fp, "MatrixSize,NumProcesses,TimeTaken\n");
                fclose(fp);
            }
        } else {
            fclose(fp);
        }
    }

    MPI_Bcast(kernel, kernel_size * kernel_size, MPI_INT, 0, MPI_COMM_WORLD);

    double start_time = MPI_Wtime();

    MPI_Scatter(input_matrix, rows_per_proc * N, MPI_INT,
                local_input, rows_per_proc * N, MPI_INT,
                0, MPI_COMM_WORLD);

    int *extended_input = (int *)malloc((rows_per_proc + kernel_size - 1) * N * sizeof(int));

    int *all_data = NULL;
    if (rank == 0) {
        all_data = (int *)malloc(N * N * sizeof(int));
    }
    
    MPI_Gather(local_input, rows_per_proc * N, MPI_INT,
               all_data, rows_per_proc * N, MPI_INT,
               0, MPI_COMM_WORLD);

    if (rank == 0) {
        int kernel_center = kernel_size / 2;
        
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                int sum = 0;
                
                for (int ki = 0; ki < kernel_size; ki++) {
                    for (int kj = 0; kj < kernel_size; kj++) {
                        int row_idx = i + ki - kernel_center;
                        int col_idx = j + kj - kernel_center;
                        
                        if (row_idx >= 0 && row_idx < N && 
                            col_idx >= 0 && col_idx < N) {
                            sum += all_data[row_idx * N + col_idx] * 
                                   kernel[ki * kernel_size + kj];
                        }
                    }
                }
                output_matrix[i * N + j] = sum;
            }
        }
    }

    MPI_Scatter(output_matrix, rows_per_proc * N, MPI_INT,
                local_output, rows_per_proc * N, MPI_INT,
                0, MPI_COMM_WORLD);

    double end_time = MPI_Wtime();
    double exec_time = end_time - start_time;

    if (rank == 0) {
        FILE *fp = fopen("PS_1.csv", "a");
        if (fp == NULL) {
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        fprintf(fp, "%d,%d,%f\n", N, size, exec_time);
        fclose(fp);
    }
    
    free(local_input);
    free(local_output);
    free(kernel);
    free(extended_input);
    
    if (rank == 0) {
        free(input_matrix);
        free(output_matrix);
        if (all_data) free(all_data);
    }

    MPI_Finalize();
    return 0;
}