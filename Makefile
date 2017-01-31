CC=gcc
CFLAGS=-O3 -march=native
OBJ=ctrlalt.o

all: ctrlalt

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

ctrlalt: ctrlalt.o
	$(CC) $(OBJ) -o ctrlalt

clean:
	rm ctrlalt $(OBJ)
