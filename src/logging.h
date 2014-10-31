#ifndef LOGGING_H
#define LOGGING_H

#include "ocean.h"
#include "logging.c"

/*
 * Write a line to the log.
 */
void write_line(char *line);

/*
 * Write information about a grid square.
 */
void write_square(grid_square square);

/*
 * Initializes a log file
 */
void initLogFile(void);

/*
 * Closes a log file
 */
void closeLogFile(void);

#endif
