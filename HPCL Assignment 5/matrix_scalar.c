#include <stdio.h>
#include <omp.h>

#define N 3

int main() {
    int A[N][N], B[N][N], scalar;

    printf("Enter Matrix A (%dx%d):\n", N, N);
    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++)
            scanf("%d", &A[i][j]);

    printf("Enter scalar value: ");
    scanf("%d", &scalar);

    #pragma omp parallel for collapse(2)
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            B[i][j] = scalar * A[i][j];
        }
    }

    printf("Result Matrix (scalar*A):\n");
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            printf("%d ", B[i][j]);
        }
        printf("\n");
    }

    return 0;
}
