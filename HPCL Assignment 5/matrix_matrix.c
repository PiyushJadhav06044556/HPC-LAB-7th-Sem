#include <stdio.h>
#include <omp.h>

#define N 3   // size of square matrices (change if needed)

int main() {
    int A[N][N], B[N][N], C[N][N];

    printf("Enter Matrix A (%dx%d):\n", N, N);
    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++)
            scanf("%d", &A[i][j]);

    printf("Enter Matrix B (%dx%d):\n", N, N);
    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++)
            scanf("%d", &B[i][j]);

    // Initialize result
    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++)
            C[i][j] = 0;

    #pragma omp parallel for collapse(2)
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            for(int k=0;k<N;k++){
                C[i][j] += A[i][k]*B[k][j];
            }
        }
    }

    printf("Result Matrix (A*B):\n");
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }

    return 0;
}
