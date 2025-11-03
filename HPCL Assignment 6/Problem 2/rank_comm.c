#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    // Get total number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get rank of this process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Get communicator group
    MPI_Group group;
    MPI_Comm_group(MPI_COMM_WORLD, &group);

    printf("Process %d out of %d processes in communicator group\n", world_rank, world_size);

    MPI_Finalize();
    return 0;
}
