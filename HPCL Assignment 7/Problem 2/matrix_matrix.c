#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int rank, size, n;
    double *A = NULL, *B = NULL, *C = NULL;
    double *local_A, *local_C;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        printf("Enter matrix size n (matrices are n x n): ");
        fflush(stdout);
        scanf("%d", &n);

        // allocate full matrices
        A = (double*)malloc(n * n * sizeof(double));
        B = (double*)malloc(n * n * sizeof(double));
        C = (double*)malloc(n * n * sizeof(double));

        int choice;
        printf("Enter 1 to auto-fill matrices, 0 for manual input: ");
        fflush(stdout);
        scanf("%d", &choice);

        if (choice == 1) {
            // auto-fill with simple values
            for (int i = 0; i < n * n; i++) {
                A[i] = i % 10 + 1;   // values 1–10
                B[i] = (i % n) + 1;  // repeating 1..n
            }
            printf("Auto-filled matrices A and B.\n");
        } else {
            printf("Enter %d elements for matrix A (row-major):\n", n * n);
            for (int i = 0; i < n * n; i++) scanf("%lf", &A[i]);

            printf("Enter %d elements for matrix B (row-major):\n", n * n);
            for (int i = 0; i < n * n; i++) scanf("%lf", &B[i]);
        }
    }

    // Broadcast size n
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int rows_per_proc = n / size;
    int extra_rows = n % size;

    // calculate send counts and displacements for scatterv/gatherv
    int *sendcounts = (int*)malloc(size * sizeof(int));
    int *displs = (int*)malloc(size * sizeof(int));
    int *recvcounts = (int*)malloc(size * sizeof(int));
    int *recvdispls = (int*)malloc(size * sizeof(int));

    int offset = 0;
    for (int i = 0; i < size; i++) {
        int rows = rows_per_proc + (i < extra_rows ? 1 : 0);
        sendcounts[i] = rows * n;
        displs[i] = offset;
        recvcounts[i] = rows * n;
        recvdispls[i] = offset;
        offset += rows * n;
    }

    // allocate local buffers
    local_A = (double*)malloc(sendcounts[rank] * sizeof(double));
    local_C = (double*)malloc(sendcounts[rank] * sizeof(double)); // same #rows as A
    if (rank != 0) B = (double*)malloc(n * n * sizeof(double));

    // broadcast B to all
    MPI_Bcast(B, n * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double t_start = MPI_Wtime();

    // scatter rows of A
    MPI_Scatterv(A, sendcounts, displs, MPI_DOUBLE,
                 local_A, sendcounts[rank], MPI_DOUBLE,
                 0, MPI_COMM_WORLD);

    // local matrix multiplication
    int local_rows = sendcounts[rank] / n;
    for (int i = 0; i < local_rows; i++) {
        for (int j = 0; j < n; j++) {
            double sum = 0.0;
            for (int k = 0; k < n; k++) {
                sum += local_A[i * n + k] * B[k * n + j];
            }
            local_C[i * n + j] = sum;
        }
    }

    // gather results into C
    MPI_Gatherv(local_C, sendcounts[rank], MPI_DOUBLE,
                C, recvcounts, recvdispls, MPI_DOUBLE,
                0, MPI_COMM_WORLD);

    double t_end = MPI_Wtime();

    if (rank == 0) {
        printf("\n=== Matrix-Matrix Multiplication Result (n=%d, procs=%d) ===\n", n, size);
        if (n <= 8) { // only print small matrices
            printf("Result matrix C:\n");
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    printf("%8.2f ", C[i * n + j]);
                }
                printf("\n");
            }
        } else {
            printf("Result not printed (n > 8)\n");
        }
        printf("\nTotal Time: %lf seconds\n", t_end - t_start);
    }

    // cleanup
    free(local_A);
    free(local_C);
    free(sendcounts);
    free(displs);
    free(recvcounts);
    free(recvdispls);
    if (rank != 0) free(B);
    if (rank == 0) {
        free(A);
        free(B);
        free(C);
    }

    MPI_Finalize();
    return 0;
}
