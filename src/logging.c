#include <stdio.h>
#include "ocean.h"
#include "logging.h"

/* Declarations */
MPI_Info info;
/* MPI_Status status; */
int size, rank;
MPI_File fh;

void initLogFile2() {
  char *logName = "./logFile.txt";
  
  MPI_Info_create(&info);
	
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  printf("Rank %d of %d has initialized their log file.\n", rank, size);

  int rc = MPI_File_open(MPI_COMM_WORLD, logName, MPI_MODE_CREATE | MPI_MODE_RDWR, info, &fh);

  printf("Rank %d of %d achieved the file_open result %d for their log file.\n", rank, size, rc);
}

void closeLogFile2(void) {
  MPI_File_close(&fh);
  printf("%d/%d closed their chunk of the log file.\n", rank, size);

}

void write_line2(char *line) {
  printf("%s\n", line);
}

void write_square2(grid_square square) {
  printf("I am square with coords (%d, %d)\n", square.x, square.y);
}
