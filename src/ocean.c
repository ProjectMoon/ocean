#include <mpi.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include "ocean.h"
#include "logging.h"

//Defines: event types for various oceanic commuication
#define EVENT_FISH 0
#define EVENT_BOAT 1

//Other helpful macros
#define ARR_LEN(arr) ( sizeof(arr) / sizeof(arr[0]) )

int main (int argc, char** argv) {
    int rank, size;
	int x_size = 3;
	int y_size = 3;
	
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm grid;
	
    create_communicator(MPI_COMM_WORLD, &grid, x_size, y_size);
    MPI_Comm_rank(grid, &rank);

    work(rank, size, grid);
    
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

int get_adjacent(grid_square square, direction_t dir) {
    switch (dir) {
    case DIR_LEFT:
        return square.left;
    case DIR_RIGHT:
        return square.right;
    case DIR_UP:
        return square.up;
    case DIR_DOWN:
        return square.down;
    default:
        return MPI_PROC_NULL;
    }
}

int get_opposite(grid_square square, direction_t dir) {
    switch (dir) {
    case DIR_LEFT:
        return square.right;
    case DIR_RIGHT:
        return square.left;
    case DIR_UP:
        return square.down;
    case DIR_DOWN:
        return square.up;
    default:
        return MPI_PROC_NULL;
    }
}

void work(int rank, int size, MPI_Comm grid) {
	
    //Where am I?
    int coords[2];
    MPI_Cart_coords(grid, rank, 2, coords);

    //Get neighbors. dim 0 = columns, dim 1 = rows
    int left, right, up, down;
    MPI_Cart_shift(grid, 0, 1, &up, &down);
    MPI_Cart_shift(grid, 1, 1, &left, &right);

    //Randomly generate a number of fish (0 to 20). Modulus slightly
    //reduces randomness, but whatever.
    srand(time(NULL) + rank);
    int fish = rand() % 21;
    
    //TODO randomly determine these values
    grid_square square = {
        .has_net = (rank % 3 == 0), //every 3rd square has a net
        .has_boat = (rank % 3 == 0),
        .fish = fish,
        .rank = rank,
        .x = coords[0],
        .y = coords[1],
        .left = left,
        .right = right,
        .up = up,
        .down = down
    };

    printf("(%d) Starting with %d fish\n", rank, square.fish);
    
    while (true) {
        simulation_step(grid, square, size);
        sleep(1);
    }
}

void simulation_step(MPI_Comm grid, grid_square square, int size) {
    //this really only covers fish that start in the square in the
    //beginning. -Should be okay, since moving fish will then be caught in the beginning
	//of the next timestep
    if (square.has_net) {
        square.fish_caught += square.fish;
        square.fish = 0;
    }

    //send fish swimming and receive incoming fish from neighbours
    direction_t dir = rand() % 5;
    int fish_dest = get_adjacent(square, dir);

    MPI_Request reqs[8]; //4 send, 4 receive.
    MPI_Status statuses[8];

    int fish_arrived[4] = { 0 }; //for each incoming direction.
            
    for (int c = 0; c < 4; c++) {
        //if we are currently sending to the chosen node of the fish,
        //then we send them off. otherwise no fish go there.
        int fish_swimming, destination;
        destination = get_adjacent(square, c);
        
        if (destination == fish_dest && fish_dest != MPI_PROC_NULL) {
            fish_swimming = square.fish;
        }
        else {
            fish_swimming = 0;
        }
        
        MPI_Isend(&fish_swimming, 1, MPI_INT, destination, EVENT_FISH, grid, &reqs[c]);

        //get fish arrivals from the opposite direction.
        int opposite;
        opposite = get_opposite(square, c);
        MPI_Irecv(&fish_arrived[c], 1, MPI_INT, opposite, EVENT_FISH, grid, &reqs[c + 4]);
    }

    MPI_Waitall(8, reqs, statuses);

    if (fish_arrived > 0) {
        int total_fish_arrived = 0;
        
        for (int c = 0; c < ARR_LEN(fish_arrived); c++) {
            total_fish_arrived += fish_arrived[c];
        }

        square.fish += total_fish_arrived;
        
        printf("(%d, %d) received %d fish\n", square.x, square.y, total_fish_arrived);
    }
    
    if (square.has_net && square.fish > 0) {
        printf("(%d, %d) caught %d fish\n", square.x, square.y, square.fish);
        square.fish_caught += square.fish;
        square.fish = 0;
    }
	//TODO check if net is full, if so, remove net and boat
	int msgLength = 16;
	char *messages[6] = {"|LOUD NOISES!!!|", "|I'm on a boat!|", "|Dance with me!|", "|Mambo number 5|", "|I like flowers|", "|Sail with me..|"};
	char *my_message = "|              |";
	int randmsg;
	if(square.has_boat) {
		randmsg = rand() % 5;
		my_message = messages[randmsg];
	}
	char *r_msgs = (char *)malloc(sizeof(char) * size*msgLength);
	assert(r_msgs != NULL);
	MPI_Allgather(my_message, msgLength, MPI_CHAR, r_msgs, msgLength, MPI_CHAR, MPI_COMM_WORLD);
	if(square.rank==0) {
	printf("%s \n",  r_msgs);
	}
	free(r_msgs);
}
