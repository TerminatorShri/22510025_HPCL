#include <stdio.h>
#include <omp.h>
#include <windows.h>

#define BUFFER_SIZE 5
#define NUM_ITEMS 20

int buffer[BUFFER_SIZE];
int count = 0; 
int in = 0;     
int out = 0;    

void produce_item(int item) {
    Sleep(100);
    printf("Produced: %d\n", item);
}

void consume_item(int item) {
    Sleep(150);
    printf("Consumed: %d\n", item);
}

int main() {
    int i;

    omp_set_num_threads(2); 

    #pragma omp parallel shared(buffer, count, in, out)
    {
        int thread_id = omp_get_thread_num();

        if (thread_id == 0) { 
            for (i = 1; i <= NUM_ITEMS; i++) {
                int produced = 0;
                while (!produced) {
                    #pragma omp critical
                    {
                        if (count < BUFFER_SIZE) {
                            buffer[in] = i;
                            in = (in + 1) % BUFFER_SIZE;
                            count++;
                            produce_item(i);
                            produced = 1;
                        }
                    }
                }
            }
        }
        else if (thread_id == 1) {
            for (i = 1; i <= NUM_ITEMS; i++) {
                int consumed = 0;
                while (!consumed) {
                    #pragma omp critical
                    {
                        if (count > 0) {
                            int item = buffer[out];
                            out = (out + 1) % BUFFER_SIZE;
                            count--;
                            consume_item(item);
                            consumed = 1;
                        }
                    }
                }
            }
        }
    }

    return 0;
}
