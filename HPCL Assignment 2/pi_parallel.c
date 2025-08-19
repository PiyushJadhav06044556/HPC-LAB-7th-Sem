#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    long long num_steps;
    int threads;

    printf("Enter number of steps (data size): ");
    scanf("%lld", &num_steps);

    printf("Enter number of threads: ");
    scanf("%d", &threads);
    omp_set_num_threads(threads);

    double step = 1.0 / (double) num_steps;
    double sum = 0.0;
    double start_time, end_time;

    start_time = omp_get_wtime();

    #pragma omp parallel
    {
        double x;
        double local_sum = 0.0;

        #pragma omp for
        for (long long i = 0; i < num_steps; i++) {
            x = (i + 0.5) * step;
            local_sum += 4.0 / (1.0 + x * x);
        }

        #pragma omp atomic
        sum += local_sum;
    }

    double pi = step * sum;

    end_time = omp_get_wtime();

    printf("Calculated Pi = %.15f\n", pi);
    printf("Parallel Time = %.9f seconds\n", end_time - start_time);

    return 0;
}
