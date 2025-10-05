#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define MAX_THREADS 2
#define NUM_INPUTS 10      
#define REPETITIONS 100000 

double sequential_eval(double x) {
    double res = 0.0;
    for(int rep = 0; rep < REPETITIONS; rep++) {
        if (x > 0)
            res = sqrt(x);
        else
            res = log(fabs(x));
    }
    return res;
}

double speculative_eval(double x) {
    double result1 = 0.0, result2 = 0.0, final_result = 0.0;

    for(int rep = 0; rep < REPETITIONS; rep++) {
        #pragma omp parallel sections num_threads(2)
        {
            #pragma omp section
            { result1 = sqrt(fabs(x)); } 
            #pragma omp section
            { result2 = log(fabs(x)); }
        }

        if (x > 0)
            final_result = result1;
        else
            final_result = result2;
    }

    return final_result;
}

int main() {
    FILE *fp = fopen("PS_1_B.csv", "w");
    if (!fp) {
        printf("Error opening file!\n");
        return 1;
    }

    fprintf(fp, "InputX,Mode,Threads,ExecutionTime,Result,WastedWorkPercent\n");

    double start_val = -50.0;
    double end_val = 50.0;
    double step = (end_val - start_val) / NUM_INPUTS;

    printf("%-10s %-12s %-7s %-12s %-12s %-10s\n", 
           "InputX", "Mode", "Threads", "Time(s)", "Result", "Wasted(%)");
    printf("---------------------------------------------------------------\n");

    for (int i = 0; i < NUM_INPUTS; i++) {
        double x = start_val + i*step;

        double start = omp_get_wtime();
        double res_seq = sequential_eval(x);
        double end = omp_get_wtime();
        double time_seq = end - start;

        printf("%-10.2f %-12s %-7d %-12.6f %-12.6f %-10d\n",
               x, "Sequential", 1, time_seq, res_seq, 0);
        fprintf(fp, "%f,Sequential,1,%f,%f,0\n", x, time_seq, res_seq);

        start = omp_get_wtime();
        double res_spec = speculative_eval(x);
        end = omp_get_wtime();
        double time_spec = end - start;

        double wasted = 50.0; 
        printf("%-10.2f %-12s %-7d %-12.6f %-12.6f %-10.2f\n",
               x, "Speculative", 2, time_spec, res_spec, wasted);
        fprintf(fp, "%f,Speculative,2,%f,%f,%f\n", x, time_spec, res_spec, wasted);
    }

    fclose(fp);
    return 0;
}
