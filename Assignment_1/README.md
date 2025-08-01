## Requirements

- **Operating System**: Windows (or Linux/macOS with minor changes)
- **Compiler**: GCC with OpenMP support (e.g., [MinGW-w64](https://www.mingw-w64.org/) on Windows)
- **Language**: C with OpenMP (`#include <omp.h>`)

---

## Installation (Windows)

1. **Install MinGW-w64**

   - Download from [https://www.mingw-w64.org/downloads](https://www.mingw-w64.org/downloads)
   - Choose architecture (x86_64) and set `bin/` folder in your system's PATH

2. **Verify Installation**
   Open Command Prompt and run:

   ```bash
   gcc --version
   ```

3. **Check OpenMP Support**
   Try compiling a simple OpenMP code with `-fopenmp`. If it compiles, you're good to go.

---

## How to Compile and Run

### Step 1: Navigate to the Project Directory

```bash
cd Assignment_1/
```

---

### Step 2: Compile the Program

```bash
gcc -fopenmp File_Name.c -o File_Name
```

- `-fopenmp`: Enables OpenMP support
- `-o File_Name`: Sets the output executable name

---

### Step 3: Run the Executable

```bash
./File_Name.exe
```

> On Windows, you can also run it via double-click or from Command Prompt.

---
