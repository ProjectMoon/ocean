#include <mpi.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "ocean.h"

int main (int argc, char** argv) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    if (rank == 0) {
        master(rank);
    }
    else {
        child(rank);
    }

    MPI_Finalize();
    return 0;
}

void master(int rank) {
    printf("I am the master\n");
}

void child(int rank) {
    print("I am the child\n");
    //TODO randomly determine these values
    grid_square square = {
        .has_net = false,
        .has_boat = false,
        .fish = 5
    };
    
    while (true) {
        simulation_step(rank, square);
        sleep(1);
    }
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
