#include <stdio.h>
#include <omp.h>

int main() {
    int n_threads;

    printf("Enter number of threads: ");
    scanf("%d", &n_threads);

    printf("\nSequential Hello World:\n");
    for (int i = 0; i < n_threads; i++) {
        printf("Hello from thread %d (Sequential)\n", i);
    }

    printf("\nParallel Hello World using OpenMP:\n");
    omp_set_num_threads(n_threads);
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        printf("Hello from thread %d (Parallel)\n", tid);
    }

    return 0;
}
