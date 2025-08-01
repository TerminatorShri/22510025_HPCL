## Requirements

* **Operating System**: Windows (or Linux/macOS with minor changes)
* **Compiler**: GCC with OpenMP support (e.g., [MinGW-w64](https://www.mingw-w64.org/) on Windows)
* **Language**: C with OpenMP (`#include <omp.h>`)
* **Python (for analysis scripts)**: Python 3.x with `pandas` and `matplotlib` installed

---

## Installation (Windows)

### 1. Install MinGW-w64

* Download from: [https://www.mingw-w64.org/downloads](https://www.mingw-w64.org/downloads)
* Select architecture: `x86_64`
* During installation, add the `bin/` folder (e.g., `C:\Program Files\mingw-w64\...\bin`) to your system’s `PATH` environment variable.

### 2. Verify Installation

Open Command Prompt and run:

```bash
gcc --version
```

You should see version information if installed correctly.

### 3. Check OpenMP Support

Try compiling a simple OpenMP program with:

```bash
gcc -fopenmp test.c -o test.exe
```

If it compiles and runs, OpenMP is supported.

---

## How to Compile and Run the C Program

### Step 1: Navigate to the Project Directory

```bash
cd Assignment_2/
```

### Step 2: Compile the Program

```bash
gcc -fopenmp File_Name.c -o File_Name.exe
```

* `-fopenmp`: Enables OpenMP support
* `-o File_Name.exe`: Specifies the output executable name

### Step 3: Run the Executable

```bash
./File_Name.exe
```

Alternatively, you can double-click the `.exe` file or run it from the Command Prompt.

---

## Step 4: Run the Python Scripts Used for Analysis

### 1. Install Python

Download and install Python from [https://www.python.org/downloads](https://www.python.org/downloads).
Make sure to check **“Add Python to PATH”** during installation.

### 2. Install Required Libraries

Open Command Prompt and run:

```bash
pip install pandas matplotlib
```

### 3. Run the Python Script

Navigate to the folder containing your script:

```bash
cd Assignment_2/
```

Then run the script:

```bash
python analysis_script.py
```

Replace `analysis_script.py` with the actual filename you are using.

This will generate execution time and speedup graphs based on your output data.

---

