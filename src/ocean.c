#include <mpi.h>
#include <stdio.h>
#include <stdbool.h>
#include "ocean.h"

int main (int argc, char** argv) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    if (rank == 0) {
        master();
    }
    else {
        child();
    }

    MPI_Finalize();
    return 0;
}

void master() {
    printf("I am the master\n");
}

void child() {
    print("I am the child\n");
}

void simulation_step(int rank, grid_square square) {
    //TODO receive info from neighbors

    if (square.has_net) {
        square.fish_caught += square.fish;
        square.fish = 0;
    }

    //TODO handle fish swimming (or no event if no fish)

    //TODO handle boats chatting
}
