#!/bin/bash

# Remove existing CSV file to start fresh
rm -f PS_1.csv

sizes=(100 500 1000 2000)

procs=(1 2 4 5 10)

mpicc PS_1.c -o PS_1

for N in "${sizes[@]}"; do
    for p in "${procs[@]}"; do
        # Check if matrix size is divisible by number of processes
        if [ $((N % p)) -eq 0 ]; then
            mpirun --oversubscribe -np $p ./PS_1 $N
        fi
    done
done

echo "All experiments completed. Results saved in PS_1.csv"

echo "---------------------------------------------"
printf "| %-13s | %-12s | %-10s |\n" "Matrix Size" "Processes" "Time(s)"
echo "---------------------------------------------"

tail -n +2 PS_1.csv | while IFS=, read -r n proc time; do
    printf "| %-13s | %-12s | %-10s |\n" "$n" "$proc" "$time"
done