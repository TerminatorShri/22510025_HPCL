#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define MAX_THREADS 16
#define MAX_SIZES 3

int is_safe(int *board, int row, int col) {
    for (int i = 0; i < row; i++) {
        if (board[i] == col || abs(board[i] - col) == abs(i - row)) {
            return 0;
        }
    }
    return 1;
}

void solve_nqueens(int *board, int row, int N, long *solutions) {
    if (row == N) {
        #pragma omp atomic
        (*solutions)++;
        return;
    }

    for (int col = 0; col < N; col++) {
        if (is_safe(board, row, col)) {
            board[row] = col;
            solve_nqueens(board, row + 1, N, solutions);
        }
    }
}

long parallel_nqueens(int N, int threads) {
    long solutions = 0;

    #pragma omp parallel num_threads(threads)
    {
        int *board = (int *)malloc(N * sizeof(int));
        int tid = omp_get_thread_num();

        #pragma omp for schedule(dynamic)
        for (int col = 0; col < N; col++) {
            board[0] = col;
            solve_nqueens(board, 1, N, &solutions);
        }
        free(board);
    }
    return solutions;
}

int main() {
    int sizes[MAX_SIZES] = {8, 10, 12};  
    FILE *fp = fopen("PS_1_A.csv", "a");

    if (fp == NULL) {
        printf("Error opening PS_1_A.csv!\n");
        return 1;
    }

    fseek(fp, 0, SEEK_END);
    if (ftell(fp) == 0) {
        fprintf(fp, "BoardSize,Threads,ExecutionTime,Solutions\n");
    }

    printf("%-10s %-7s %-12s %-12s\n", "BoardSize", "Threads", "Time(s)", "Solutions");
    printf("------------------------------------------------------\n");

    for (int s = 0; s < MAX_SIZES; s++) {
        int N = sizes[s];

        for (int t = 1; t <= MAX_THREADS; t *= 2) {
            double start_time = omp_get_wtime();
            long solutions = parallel_nqueens(N, t);
            double end_time = omp_get_wtime();

            double exec_time = end_time - start_time;

            printf("%-10d %-7d %-12.6f %-12ld\n", N, t, exec_time, solutions);
            fprintf(fp, "%d,%d,%f,%ld\n", N, t, exec_time, solutions);
        }
    }

    fclose(fp);
    return 0;
}
