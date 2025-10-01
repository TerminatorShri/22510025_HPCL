#!/bin/bash

# Remove existing CSV file to start fresh
rm -f PS_2.csv

sizes=(10000 50000 100000 500000 1000000)

procs=(1 2 4 5 10)

mpicc PS_2.c -o PS_2

for N in "${sizes[@]}"; do
    for p in "${procs[@]}"; do
        # Check if vector size is divisible by number of processes
        if [ $((N % p)) -eq 0 ]; then
            mpirun --oversubscribe -np $p ./PS_2 $N
        fi
    done
done

echo "All experiments completed. Results saved in PS_2.csv"

echo "---------------------------------------------"
printf "| %-13s | %-12s | %-10s |\n" "Vector Size" "Processes" "Time(s)"
echo "---------------------------------------------"

tail -n +2 PS_2.csv | while IFS=, read -r n proc time; do
    printf "| %-13s | %-12s | %-10s |\n" "$n" "$proc" "$time"
done