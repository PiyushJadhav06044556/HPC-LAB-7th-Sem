#include <stdio.h>
#include <omp.h>

#define N 3

int main() {
    int A[N][N], x[N], y[N];

    printf("Enter Matrix A (%dx%d):\n", N, N);
    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++)
            scanf("%d", &A[i][j]);

    printf("Enter Vector x (%d elements):\n", N);
    for(int i=0;i<N;i++)
        scanf("%d", &x[i]);

    // Initialize result vector
    for(int i=0;i<N;i++) y[i] = 0;

    #pragma omp parallel for
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            y[i] += A[i][j]*x[j];
        }
    }

    printf("Result Vector (A*x):\n");
    for(int i=0;i<N;i++){
        printf("%d ", y[i]);
    }
    printf("\n");

    return 0;
}
