#ifndef OCEAN_H
#define OCEAN_H

#include <stdbool.h>
#include <mpi.h>

/*
 * fish: number of fish in the square. for each fish, swim away!
 * net: yes or no. if there is a net, all fish that enter are caught.
 * fish_caught: number of fish in the net. should be 0 if net = false.
 * boat: yes or no. whether or not there is a boat.
 */
typedef struct {
    int fish;
    bool has_net;
    int fish_caught;
    bool has_boat;

    int rank;
    int x;
    int y;

    int left;
    int right;
    int up;
    int down;
} grid_square;

//ndims = 2
//periods = 0
//reorder = 0
void create_communicator(MPI_Comm input, MPI_Comm *comm, int x, int y);

/*
 * Execute a step of the simulation for a processor. Each processor
 * represents one square in the grid. The square must receive MPI
 * messages from its neighbors, and then send them out to its
 * neighbors.
 */
void simulation_step(grid_square square);

/*
 * Do the thing.
 */
void work(int rank, MPI_Comm grid);

#endif
