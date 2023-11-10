CC = gcc
CFLAGS = -Wall -g

# Define the target and dependencies
all: P_shared_mem Q_shared_mem

# Compile P_shared_mem.c
P_shared_mem: P_shared_mem.c shared_mem.h
	$(CC) $(CFLAGS) -o P_shared_mem P_shared_mem.c

# Compile Q_shared_mem.c
Q_shared_mem: Q_shared_mem.c shared_mem.h
	$(CC) $(CFLAGS) -o Q_shared_mem Q_shared_mem.c

# Clean up generated files
clean:
	rm -f P_shared_mem Q_shared_mem
