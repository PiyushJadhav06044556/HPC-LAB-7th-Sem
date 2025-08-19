#include <stdio.h>
#include <omp.h>

#define N 8   // change size if needed

int main() {
    int A[N], P[N];

    printf("Enter %d elements of array:\n", N);
    for(int i=0;i<N;i++)
        scanf("%d", &A[i]);

    P[0] = A[0];
    for(int i=1;i<N;i++){
        P[i] = P[i-1] + A[i];
    }

    printf("Prefix Sum:\n");
    for(int i=0;i<N;i++){
        printf("%d ", P[i]);
    }
    printf("\n");

    return 0;
}
