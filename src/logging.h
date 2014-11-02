#ifndef LOGGING_H
#define LOGGING_H

#include "ocean.h"

/*
 * Write a line to the log.
 */
void write_line2(char *line);

/*
 * Write information about a grid square.
 */
void write_square2(grid_square square);

/*
 * Initializes a log file
 */
void initLogFile2();

/*
 * Closes a log file
 */
void closeLogFile2(void);

#endif
