#include <stdio.h>
#include <omp.h>

long fib(int n) {
    if (n < 2) return n;

    long x, y;

    #pragma omp task shared(x) 
    x = fib(n - 1);

    #pragma omp task shared(y) 
    y = fib(n - 2);

    #pragma omp taskwait 
    return x + y;
}

int main() {
    int n = 10;
    long result;

    double start = omp_get_wtime();

    #pragma omp parallel 
    {
        #pragma omp single 
        result = fib(n);
    }

    double end = omp_get_wtime();

    printf("Fibonacci(%d) = %ld\n", n, result);
    printf("Time: %f seconds\n", end - start);

    return 0;
}
