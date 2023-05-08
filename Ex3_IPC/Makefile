CC = gcc
CFLAGS = -Wall -Wextra
OBJ = stnc.o

default: all

all: stnc

stnc: $(OBJ)
	$(CC) $(CFLAGS) -o stnc $(OBJ)

stnc.o: stnc.c stnc.h
	$(CC) $(CFLAGS) -c stnc.c

stnc2: stnc2.o
	$(CC) $(CFLAGS) -o stnc2 stnc.o

stnc2.o: stnc2.c 
	$(CC) $(CFLAGS) -c stnc2.c	

clean:
	rm -f stnc $(OBJ)
