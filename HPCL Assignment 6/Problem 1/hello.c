#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    // Initialize MPI environment
    MPI_Init(&argc, &argv);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank (ID) of this process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    printf("Hello World from process %d out of %d processes\n", world_rank, world_size);

    MPI_Finalize();
    return 0;
}
