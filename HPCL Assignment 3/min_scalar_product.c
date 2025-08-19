#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Function to compare ascending
int cmp_asc(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

// Function to compare descending
int cmp_desc(const void *a, const void *b) {
    return (*(int*)b - *(int*)a);
}

int main() {
    int n;
    printf("Enter size of vectors: ");
    scanf("%d", &n);

    int *A = (int*)malloc(n * sizeof(int));
    int *B = (int*)malloc(n * sizeof(int));

    printf("Enter elements of vector A:\n");
    for(int i=0; i<n; i++) scanf("%d", &A[i]);

    printf("Enter elements of vector B:\n");
    for(int i=0; i<n; i++) scanf("%d", &B[i]);

    // Sort A in ascending, B in descending
    qsort(A, n, sizeof(int), cmp_asc);
    qsort(B, n, sizeof(int), cmp_desc);

    long long min_scalar_product = 0;

    // Parallel reduction with OpenMP
    #pragma omp parallel for reduction(+:min_scalar_product) schedule(static)
    for(int i=0; i<n; i++) {
        min_scalar_product += (long long)A[i] * B[i];
    }

    printf("\nMinimum Scalar Product = %lld\n", min_scalar_product);

    free(A);
    free(B);
    return 0;
}
