#ifndef LOGGING_H
#define LOGGING_H

#include "ocean.h"
#include "logging.c"

/*
 * Write a line to the log.
 */
void writeLine(char *line);

/*
 * Write information about a grid square.
 */
void write_square(grid_square square);

/*
 * Initializes a log file
 */
void initLogFile(int rank,int size);

/*
 * Closes a log file
 */
void closeLogFile();

#endif
