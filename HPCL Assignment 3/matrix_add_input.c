#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Function to allocate matrix
int** allocate_matrix(int n) {
    int **mat = (int**)malloc(n * sizeof(int*));
    for(int i = 0; i < n; i++)
        mat[i] = (int*)malloc(n * sizeof(int));
    return mat;
}

// Function to fill matrix with random numbers
void fill_matrix(int **mat, int n) {
    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++)
            mat[i][j] = rand() % 100;
}

int main() {
    int n, threads;

    printf("Enter matrix size (n x n): ");
    scanf("%d", &n);

    printf("Enter number of threads: ");
    scanf("%d", &threads);

    // Allocate matrices
    int **A = allocate_matrix(n);
    int **B = allocate_matrix(n);
    int **C = allocate_matrix(n);

    fill_matrix(A, n);
    fill_matrix(B, n);

    double start = omp_get_wtime();

    // Parallel matrix addition
    #pragma omp parallel for num_threads(threads) collapse(2)
    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }

    double end = omp_get_wtime();
    printf("Matrix Size: %d x %d, Threads: %d, Time Taken: %f seconds\n",
           n, n, threads, end - start);

    // Free memory
    for(int i=0; i<n; i++) {
        free(A[i]); free(B[i]); free(C[i]);
    }
    free(A); free(B); free(C);

    return 0;
}
