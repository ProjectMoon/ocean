#include <stdio.h>
#include "ocean.h"
#include "logging.h"

void write_line(char *line) {
    printf("%s\n", line);
}

void write_square(grid_square square) {
    printf("I am square with coords (%d, %d)\n", square.x, square.y);
}
