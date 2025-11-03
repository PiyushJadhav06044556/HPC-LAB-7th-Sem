#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int send_data = rank;          // Each process sends its own rank
    int recv_data = -1;

    int next = (rank + 1) % size;  // Next neighbor in ring
    int prev = (rank - 1 + size) % size; // Previous neighbor in ring

    // Blocking send & receive
    MPI_Send(&send_data, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
    MPI_Recv(&recv_data, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    printf("Process %d received %d from process %d\n", rank, recv_data, prev);

    MPI_Finalize();
    return 0;
}
