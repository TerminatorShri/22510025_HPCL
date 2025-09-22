#!/bin/bash

echo "MatrixSize,NumProcesses,TimeTaken" > PS_2.csv

sizes=(100 200 500 1000)
procs=(1 2 4 5 10)

mpicc PS_2.c -o PS_2

for N in "${sizes[@]}"; do
    for p in "${procs[@]}"; do
        mpirun --oversubscribe -np $p ./PS_2 $N
    done
done

echo "All experiments completed. Results saved in results_matrix_matrix.csv"

echo "---------------------------------------------"
printf "| %-13s | %-12s | %-10s |\n" "Matrix Size" "Processes" "Time(s)"
echo "---------------------------------------------"

tail -n +2 PS_2.csv | while IFS=, read -r n proc time; do
    printf "| %-13s | %-12s | %-10s |\n" "$n" "$proc" "$time"
done

