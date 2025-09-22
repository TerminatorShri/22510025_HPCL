Here’s a complete README incorporating your MPI setup, compilation, execution, automated bash script, and Python analysis steps:

---

# MPI Matrix Computation Guide

This guide explains how to install MPI (OpenMPI), compile and run MPI programs, automate experiments using a bash script, and analyze results using Python.

---

## 1. Install MPI (OpenMPI)

Update your package list and install the OpenMPI binaries and development libraries:

```bash
sudo apt update
sudo apt install openmpi-bin libopenmpi-dev -y
```

---

## 2. Verify Installation

After installation, verify that MPI is installed correctly:

```bash
mpiexec --version
```

Expected output example:

```
mpiexec (OpenRTE) 4.1.2
```

---

## 3. Compile Your MPI Program

Use `mpicc` to compile your MPI C programs:

```bash
mpicc PS_X.c -o PS_X
```

- This generates an executable named `PS_X`.

---

## 4. Run Your MPI Program Manually

You can run the compiled program using `mpiexec` with a specified number of processes and matrix size:

```bash
mpiexec -n <num_processors> ./PS_X <matrix_size>
```

**Example:**

```bash
mpiexec -n 4 ./PS_X 1000
```

- This runs `PS_X` using 4 processes with a matrix size of 1000.

---

## 5. Run Automated Experiments Using Bash Script

To test multiple matrix sizes and process counts, you can create a bash script (`run_experiments.sh`) to automate the runs and store results in a CSV file.

### Example Bash Script

```bash
#!/bin/bash

# CSV header
echo "MatrixSize,NumProcesses,TimeTaken" > results.csv

# Define matrix sizes and process counts
sizes=(100 500 1000 2000)
procs=(1 2 4 5 10)

# Compile MPI program
mpicc PS_X.c -o PS_X

# Run experiments
for N in "${sizes[@]}"; do
    for p in "${procs[@]}"; do
        mpirun --oversubscribe -np $p ./PS_X $N
    done
done

echo "All experiments completed. Results saved in results.csv"
```

### Run the Bash Script

```bash
chmod +x run_experiments.sh
./run_experiments.sh
```

- The script will execute all combinations of matrix sizes and process counts.
- Execution times are automatically saved in `results.csv`.

---

## 6. Run Python Scripts for Analysis

You can visualize the results using Python.

### 1. Install Python

Download and install Python from [https://www.python.org/downloads](https://www.python.org/downloads).
Make sure to check **“Add Python to PATH”** during installation.

### 2. Install Required Libraries

Open a terminal or Command Prompt:

```bash
pip install pandas matplotlib
```

### 3. Run the Python Script

```bash
python PS_X.py
```

- Replace `analysis_script.py` with your script filename.
- This will generate execution time and speedup plots based on your experiments.

---

## 7. Notes

1. Ensure `N` (matrix size) is divisible by the number of processes when running MPI programs.
2. Use `--oversubscribe` with `mpirun` if you want to run more processes than available CPU cores.
3. You can modify the Python script to generate more plots or export data for reports.

---

