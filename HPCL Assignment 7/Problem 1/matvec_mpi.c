// matvec_mpi.c
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void fill_matrix(double *A, int rows, int cols) {
    for (int i = 0; i < rows*cols; ++i) A[i] = (double)((i % 100) + 1); // deterministic values
}
void fill_vector(double *x, int n) {
    for (int i = 0; i < n; ++i) x[i] = 1.0; // simple vector of ones
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // default matrix size -- you can change via command line: ./exe N
    int N = 2000;
    if (argc >= 2) N = atoi(argv[1]);

    double *A = NULL;      // only root keeps full matrix
    double *x = (double*)malloc(sizeof(double) * N);
    double *y = NULL;      // final result (only root receives full y)

    // Root creates matrix and vector
    if (rank == 0) {
        A = (double*)malloc(sizeof(double) * N * N);
        fill_matrix(A, N, N);
        fill_vector(x, N);
        y = (double*)malloc(sizeof(double) * N);
    }

    // Broadcast vector x to all processes
    MPI_Bcast(x, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // compute rows distribution (handle uneven N)
    int base = N / size;
    int rem = N % size;
    int local_rows = (rank < rem) ? base + 1 : base;

    // displacements and counts for scatterv/gatherv
    int *sendcounts = NULL, *displs = NULL;
    if (rank == 0) {
        sendcounts = (int*)malloc(sizeof(int) * size);
        displs = (int*)malloc(sizeof(int) * size);
        int offset = 0;
        for (int i = 0; i < size; ++i) {
            int r = (i < rem) ? base + 1 : base;
            sendcounts[i] = r * N;    // number of doubles
            displs[i] = offset;
            offset += r * N;
        }
    }

    // allocate local A chunk (local_rows x N)
    double *local_A = (double*)malloc(sizeof(double) * local_rows * N);
    // Scatter rows of A to all processes
    MPI_Scatterv(A, sendcounts, displs, MPI_DOUBLE,
                 local_A, local_rows * N, MPI_DOUBLE,
                 0, MPI_COMM_WORLD);

    // local output
    double *local_y = (double*)malloc(sizeof(double) * local_rows);

    // Synchronize and start timer
    MPI_Barrier(MPI_COMM_WORLD);
    double t0 = MPI_Wtime();

    // local multiplication: local_y = local_A * x
    for (int i = 0; i < local_rows; ++i) {
        double sum = 0.0;
        double *row = local_A + i * N;
        for (int j = 0; j < N; ++j) sum += row[j] * x[j];
        local_y[i] = sum;
    }

    // End timer after local compute (include gather if you want)
    MPI_Barrier(MPI_COMM_WORLD);
    double t1 = MPI_Wtime();
    double local_elapsed = t1 - t0;

    // Gather local_y sizes & displacements to root
    int *recvcounts = NULL, *rdispls = NULL;
    if (rank == 0) {
        recvcounts = (int*)malloc(sizeof(int) * size);
        rdispls = (int*)malloc(sizeof(int) * size);
        int offset = 0;
        for (int i = 0; i < size; ++i) {
            int r = (i < rem) ? base + 1 : base;
            recvcounts[i] = r;
            rdispls[i] = offset;
            offset += r;
        }
    }

    // Gather results to root
    MPI_Gatherv(local_y, local_rows, MPI_DOUBLE,
                y, recvcounts, rdispls, MPI_DOUBLE,
                0, MPI_COMM_WORLD);

    // Gather max elapsed time to root (we use MAX to represent parallel time)
    double max_elapsed;
    MPI_Reduce(&local_elapsed, &max_elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        // print timing and a checksum to verify
        double checksum = 0.0;
        for (int i = 0; i < N; ++i) checksum += y[i];
        printf("N=%d, processes=%d, time = %lf seconds, checksum=%lf\n", N, size, max_elapsed, checksum);
    }

    // free
    if (A) free(A);
    if (y) free(y);
    free(x);
    free(local_A);
    free(local_y);
    if (sendcounts) free(sendcounts);
    if (displs) free(displs);
    if (recvcounts) free(recvcounts);
    if (rdispls) free(rdispls);

    MPI_Finalize();
    return 0;
}
