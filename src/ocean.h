#ifndef OCEAN_H
#define OCEAN_H

#include <stdbool.h>

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
} grid_square;

/*
 * Execute a step of the simulation for a processor. Each processor
 * represents one square in the grid. The square must receive MPI
 * messages from its neighbors, and then send them out to its
 * neighbors.
 */
void simulation_step(int rank, grid_square square);

/*
 * Execute the master thread
 */
void master();

/*
 * Execute the child thread.
 */
void child();

#endif
