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


/*
 * The directions in which we can go from a square. This type, the
 * constants, and convenience method define a simple way to retrieve
 * the value of a node adjacent to the square based on some inputted
 * number, e.g. created from a random value. It will return
 * MPI_PROC_NULL if an invalid direction is specified.
 */
typedef enum {
    DIR_LEFT,
    DIR_RIGHT,
    DIR_UP,
    DIR_DOWN
} direction_t;

/*
 * Gets the adjacent in the direction, e.g. DIR_LEFT returns the node
 * on the "left" in the cartesian grid.
 */
int get_adjacent(grid_square square, direction_t dir);

/*
 * Gets the adjacent opposite the specified direction. e.g. if left is
 * specified, then return the node on the right.
 */
int get_opposite(grid_square square, direction_t dir);

//ndims = 2
//periods = 0
//reorder = 0
/*
 * Create the 2D communicator in the given dimensions. The
 * communicator is non-periodic.
 */
void create_communicator(MPI_Comm input, MPI_Comm *comm, int x, int y);

/*
 * Execute a step of the simulation for a processor. Each processor
 * represents one square in the grid. The square must receive MPI
 * messages from its neighbors, and then send them out to its
 * neighbors.
 */
void simulation_step(MPI_Comm grid, grid_square square, int size);

/*
 * Do the thing.
 */
void work(int rank, int size, MPI_Comm grid);

#endif
