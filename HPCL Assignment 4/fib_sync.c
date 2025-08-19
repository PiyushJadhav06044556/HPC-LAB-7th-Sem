
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>


long long fib_task(int n) {
    if (n < 2) return n;

    long long x = 0, y = 0;

    // Create tasks for the two recursive calls.
    // Only create tasks when n is large enough to justify the overhead.
    // Otherwise do sequential recursion.
    const int THRESHOLD = 20; // tune as needed

    if (n > THRESHOLD) {
        #pragma omp task shared(x) firstprivate(n)
        x = fib_task(n - 1);

        #pragma omp task shared(y) firstprivate(n)
        y = fib_task(n - 2);

        #pragma omp taskwait
    } else {
        // sequential recursion for small n
        x = fib_task(n - 1);
        y = fib_task(n - 2);
    }
    return x + y;
}

int main(int argc, char *argv[]) {
    int n = 40; 
    if (argc >= 2) n = atoi(argv[1]);

    double t0 = omp_get_wtime();
    long long result;

    // Start a parallel region; one thread triggers the single region that
    // creates the initial tasks. Other threads help execute tasks.
    #pragma omp parallel
    {
        #pragma omp single
        {
            // The single thread initiates the task-based recursion
            result = fib_task(n);
        }
    } 

    double t1 = omp_get_wtime();
    printf("Fibonacci(%d) = %lld\n", n, result);
    printf("Time (s): %f\n", t1 - t0);
    return 0;
}

