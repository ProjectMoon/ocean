#include <mpi.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include "ocean.h"
#include "logging.h"

//Defines: event types for various oceanic commuication
#define EVENT_FISH 0
#define EVENT_BOAT 1

int main (int argc, char** argv) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm grid;

    create_communicator(MPI_COMM_WORLD, &grid, 3, 3);
    MPI_Comm_rank(grid, &rank);

    work(rank, grid);
    
    MPI_Finalize();
    return 0;
}

void create_communicator(MPI_Comm input, MPI_Comm *comm, int x, int y) {
    //number of processors required is x * y
    int num_dims = 2;
    int dims[2] = { x, y };
    int periods[2] = { 0, 0 };
    int reorder = 0;
    
    MPI_Cart_create(input, num_dims, dims, periods, reorder, comm);
}

void work(int rank, MPI_Comm grid) {
    write_line("I am the child\n");
    int left, right, up, down;

    //Where am I?
    int coords[2];
    MPI_Cart_coords(grid, rank, 2, coords);

    //Get neighbors. dim 0 = columns, dim 1 = rows
    MPI_Cart_shift(grid, 0, 1, &up, &down);
    MPI_Cart_shift(grid, 1, 1, &left, &right);
    
    //TODO randomly determine these values
    grid_square square = {
        .has_net = false,
        .has_boat = false,
        .fish = 5,
        .rank = rank,
        .x = coords[0],
        .y = coords[1],
        .left = left,
        .right = right,
        .up = up,
        .down = down
    };
    
    while (true) {
        simulation_step(square);
        sleep(1);
    }
}

void simulation_step(grid_square square) {
    char str[80];
    sprintf(str, "Coords are: %d, %d\n", square.x, square.y);
    write_line(str);
    sprintf(str, "Neighboys(u,d,l,r): %d, %d, %d, %d\n", square.up, square.down, square.left, square.right);
    write_line(str);
    
    //TODO receive info from neighbors

    if (square.has_net) {
        square.fish_caught += square.fish;
        square.fish = 0;
    }

    //TODO handle fish swimming (or no event if no fish)
    

    //TODO handle boats chatting
}
