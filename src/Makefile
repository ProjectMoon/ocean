# generated binary name
BINARY=ocean

# compiler info: which compiler to use and flags
CC=mpicc
CCFLAGS=-std=c99 -Wall -Werror

# build rules: the actual build rules for the program. Do not call locally.
all: ocean.o
	mkdir -p ../bin/ && $(CC) $(CCFLAGS) ocean.o -o ../bin/$(BINARY)

ocean.o: ocean.c
	$(CC) $(CCFLAGS) -c ocean.c
