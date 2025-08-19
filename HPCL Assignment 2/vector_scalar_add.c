#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int n, scalar, threads;
    printf("Enter size of vector: ");
    scanf("%d", &n);

    printf("Enter scalar value to add: ");
    scanf("%d", &scalar);

    printf("Enter number of threads: ");
    scanf("%d", &threads);
    omp_set_num_threads(threads);

    int *vector = (int *)malloc(n * sizeof(int));
    int *result = (int *)malloc(n * sizeof(int));

    // Initialize vector
    for (int i = 0; i < n; i++) {
        vector[i] = i;
    }

    double start_time, end_time;

    // Parallel execution
    start_time = omp_get_wtime();
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        result[i] = vector[i] + scalar;
    }
    end_time = omp_get_wtime();

    printf("Parallel Time: %f seconds\n", end_time - start_time);

    free(vector);
    free(result);
    return 0;
}
    