#include <stdio.h>
#include "ocean.h"
#include "logging.h"

/* Declarations */
MPI_Info info;
/* MPI_Status status; */
int size, rank;
MPI_File fh;
MPI_Status status;

void initLogFile(int rank,int size) {
    char *file_name = "logFile.txt";
    char buf[80];
    MPI_Info_create(&info);

    /*printf("Rank %d of %d has initialized their log file.\n", rank, size);*/

    int rc = MPI_File_open( MPI_COMM_WORLD, file_name, MPI_MODE_CREATE | MPI_MODE_RDWR, info, &fh);
    sprintf(buf,"I'm %d of %d and I'm writing to the log\n",rank,size);
    MPI_File_write_ordered(fh, buf, strlen(buf), MPI_CHAR, &status); 
}

void closeLogFile() {
    MPI_File_close(&fh);
	// printf("%d/%d closed their chunk of the log file.\n", rank, size);
}

void writeLine(char *line) {
    MPI_File_write_ordered(fh, line, strlen(line), MPI_CHAR, &status);
}

void write_square(grid_square square) {
    printf("I am square with coords (%d, %d)\n", square.x, square.y);
}