CC=gcc
CFLAGS=-Wall -Wextra -Werror -pedantic

all: shell

shell: stshell.c 
	$(CC) $(CFLAGS) -o shell stshell.c 

clean:
	rm -f shell