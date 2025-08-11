#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

long fib_iter(int n) {
    if (n < 2) return n;
    long a = 0, b = 1, c;
    for (int i = 2; i <= n; i++) {
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}

int main() {
    int nums[] = {10, 20, 25, 30};
    int size = 4;
    long *results = malloc(size * sizeof(long));
    if (results == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    double start = omp_get_wtime();

    #pragma omp parallel for schedule(dynamic) shared(results)
    for (int i = 0; i < size; i++) {
        results[i] = fib_iter(nums[i]);
        #pragma omp critical
        printf("Thread %d computed fib(%d) = %ld\n",
               omp_get_thread_num(), nums[i], results[i]);
    }

    double end = omp_get_wtime();
    printf("\nTotal time: %f seconds\n", end - start);
    free(results);
    return 0;
}