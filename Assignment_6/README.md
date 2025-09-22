# MPI Setup and Program Execution Guide

This guide explains how to install the MPI (OpenMPI) library, verify the installation, compile an MPI program, and run it using multiple processors.

---

## 1. Install MPI (OpenMPI)

Update your package list and install the OpenMPI binaries and development libraries:

```bash
sudo apt update
sudo apt install openmpi-bin libopenmpi-dev -y
```

---

## 2. Verify Installation

After installation, check that MPI is correctly installed by running:

```bash
mpiexec --version
```

You should see output similar to:

```
mpiexec (OpenRTE) <version_number>
```

Example:

```
mpiexec (OpenRTE) 4.1.2
```

---

## 3. Compile Your MPI Program

Use the `mpicc` compiler wrapper provided by OpenMPI to compile your C program.

Example:

```bash
mpicc PS_X.c -o PS_X
```

This will generate an executable named `PS_X`.

---

## 4. Run Your MPI Program

Run the compiled program with the desired number of processors using `mpiexec`:

```bash
mpiexec -n <num_processors> ./PS_X
```

- Replace `<num_processors>` with the number of processes you want to run (e.g., `4`).
- Ensure that your system has at least as many CPU cores available as the number of processes you specify.

---

## 5. Example Run

```bash
mpicc PS_X.c -o PS_X
mpiexec -n 4 ./PS_X
```

This runs the program `PS_X` using **4 processors**.

---
