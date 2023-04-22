#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "stdio.h"
#include "errno.h"
#include "stdlib.h"
#include "unistd.h"
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

void handle_sigint();

int main() {
	int i;
	char *argv[10];
	char *command = (char *)malloc(1024);
	char *token;
    // int file_descriptor[2], status;
    // pid_t pid;

    // Register the signal handler
    signal(SIGINT, handle_sigint);

	while (1) 
    {
	    printf("hello: ");
        fflush(stdout);
	    fgets(command, 1024, stdin);
        command = (char *)realloc(command, strlen(command));
	    command[strlen(command) - 1] = '\0'; // replace \n with \0
        // printf("size of command: %ld\n ", strlen(command));
        // printf("%s", command);
        if(!strcmp(command, "exit"))
        {
            break;
        }

	    /* parse command line */
	    i = 0;
	    token = strtok (command," ");
	    while (token != NULL)
        {
            argv[i] = token;
            token = strtok (NULL, " ");
            i++;
        }
	    argv[i] = NULL;

	    /* Is command empty */ 
	    if (argv[0] == NULL)
		continue;

	    /* for commands not part of the shell command language */ 
	    if (fork() == 0) 
        { 
            execvp(argv[0], argv);
            wait(NULL);
	    }  
        else
        {
            wait(NULL);
            printf("\n");
            fflush(stdout);
        }  
	}
}




void handle_sigint()
{
    printf("\nYou typed Control-C!\n");
    fflush(stdout);
    printf("hello: ");
    fflush(stdout);
}