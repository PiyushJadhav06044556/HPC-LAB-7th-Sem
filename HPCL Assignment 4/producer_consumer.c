// producer_consumer_sync.c
// Compile: gcc -fopenmp producer_consumer_sync.c -o producer_consumer_sync.exe
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>   // For Sleep()
#include <omp.h>

#define MAX_BUF 8
#define ITEMS 20
#define STOP_FLAG -999  // sentinel to signal consumers to stop

int queue[MAX_BUF];
int head = 0, tail = 0, curr_count = 0;

omp_lock_t q_lock;

void enqueue(int val) {
    int done = 0;
    while (!done) {
        omp_set_lock(&q_lock);
        if (curr_count < MAX_BUF) {
            queue[tail] = val;
            tail = (tail + 1) % MAX_BUF;
            curr_count++;
            done = 1;
        }
        omp_unset_lock(&q_lock);
        if (!done) Sleep(1);  // wait if buffer is full
    }
}

int dequeue() {
    int taken = 0, value = 0;
    while (!taken) {
        omp_set_lock(&q_lock);
        if (curr_count > 0) {
            value = queue[head];
            head = (head + 1) % MAX_BUF;
            curr_count--;
            taken = 1;
        }
        omp_unset_lock(&q_lock);
        if (!taken) Sleep(1); // wait if buffer is empty
    }
    return value;
}

int main(int argc, char *argv[]) {
    int producers = 2, consumers = 3;

    if (argc >= 3) {
        producers = atoi(argv[1]);
        consumers = atoi(argv[2]);
    }

    omp_init_lock(&q_lock);

    #pragma omp parallel num_threads(producers + consumers)
    {
        int tid = omp_get_thread_num();

        if (tid < producers) {
            // Producer role
            int pid = tid;
            for (int i = 0; i < ITEMS; i++) {
                int product = pid * 1000 + i;
                enqueue(product);
                #pragma omp critical
                printf("[Producer %d] -> Produced item %d\n", pid, product);
                Sleep(1);
            }

            // Only producer 0 sends stop signals
            if (pid == 0) {
                for (int j = 0; j < consumers; j++) {
                    enqueue(STOP_FLAG);
                }
            }

            #pragma omp critical
            printf("[Producer %d] Finished.\n", pid);

        } else {
            // Consumer role
            int cid = tid - producers;
            while (1) {
                int val = dequeue();
                if (val == STOP_FLAG) {
                    enqueue(STOP_FLAG); // keep stop token for others
                    break;
                }
                #pragma omp critical
                printf("    [Consumer %d] <- Consumed item %d\n", cid, val);
                Sleep(2);
            }
            #pragma omp critical
            printf("    [Consumer %d] Finished.\n", cid);
        }
    }

    omp_destroy_lock(&q_lock);
    printf("All threads have completed. Remaining items in buffer = %d\n", curr_count);
    return 0;
}
