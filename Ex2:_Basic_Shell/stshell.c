#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "stdio.h"
#include "errno.h"
#include "stdlib.h"
#include "unistd.h"
#include <string.h>
#include <signal.h>

#define MAX_COMMANDS 5
#define COMMAND_LENGTH 100

void handle_sigint(){
	printf("\nYou typed Control-C!");
	fflush(stdout);
}

int main() 
{
	int i, file_descriptor, num_of_commands, num_of_pipes, redirect, append_redirect;
	char *argv[10], *command_array[5];
	char command[1024];
	char *token, *outfile;

	// Register the signal handler:
	signal(SIGINT, handle_sigint);

	while (1) 
	{
	    printf("hello: ");
	    fgets(command, 1024, stdin);
	    command[strlen(command) - 1] = '\0'; // replace \n with \0

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

	    

		
		/* Adding the piping command*/
		if(strchr(command, '|') != NULL)
		{

			num_of_commands = 0;
			for(i=0; i < MAX_COMMANDS; i++)
			{
				command_array[i] = NULL;
				command_array[i] = (char *)malloc(COMMAND_LENGTH * sizeof(char));
			}

			token = strtok(command, "|");

			// Loop through the command input and parse the commands to the command array:
			while(token != NULL && num_of_commands < MAX_COMMANDS) 
			{
				while(*token == ' ') // gets rif of the extra space before each command
				{
					token++;
				}
				strcpy(command_array[num_of_commands], token);
				token = strtok(NULL, "|");
				num_of_commands++;
			}

			num_of_pipes = num_of_commands - 1;
		}
		else // command is a normal command
		{
			command_array[0] = (char *)malloc(COMMAND_LENGTH * sizeof(char));
			strcpy(command_array[0], command);
			num_of_commands = 1;
			num_of_pipes = 0;
		}

		int pipe_file_descriptor[2 * num_of_pipes];
		pid_t pipe_id;

		for(i=0; i < num_of_pipes; i++)
		{
			if(pipe(pipe_file_descriptor + (i * 2)) < 0)
			{
				perror("\n--ERROR. COULD NOT PIPE--");
				exit(EXIT_FAILURE);
			}
		}

		int pipe_index = 0;

		// Create pipes and fork processes:
		for(int k = 0; k < num_of_commands; k++)
		{
			char *cmd = command_array[k];
			i=0;
			token = strtok(cmd, " ");
			for(i = 0; i < COMMAND_LENGTH; i++)
			{
				argv[i] = malloc(COMMAND_LENGTH * sizeof(char));
			}

			// Copying the words of the command as arguments in argv[]:
			i=0;
			while(token != NULL)
			{
				if(i < 5)
				{
					strcpy(argv[i], token);
					i++;
				}
				else
				{
					printf("--ERROR. TO MANY ARUMENTS IN COMMAND (MAX 5)--");
					break;
				}
				token = strtok(NULL, " ");
			}

			argv[i] = NULL;
			redirect = 0;
			append_redirect = 0;
			outfile = NULL;

			/* Is command empty */ 
			if (argv[0] == NULL)
			{
				continue;
			}

			/* Does command line contain ">" or ">>"*/
			if(i > 1 && (!strcmp(argv[i - 2], ">") || (!strcmp(argv[i - 2], ">>"))))
			{
				redirect = 1;
				if(!strcmp(argv[i - 2], ">>"))
				{
					append_redirect = 1;
				}
				outfile = argv[i - 1];
				argv[i - 2] = NULL;
				argv[i - 1] = NULL;
			}
			else
			{
				redirect = append_redirect = 0;
			}


			/* for commands not part of the shell command language */ 
			pipe_id = fork();
			if (pipe_id == 0)
			{
				if((k + 1 != num_of_commands) && (dup2(pipe_file_descriptor[pipe_index + 1], 1) < 0))
				{
					perror("dup2");
					exit(EXIT_FAILURE);
				}
			 

				// if not first command and pip_index != 2 * num_of_pipes:
				if((pipe_index != 0) && (dup2(pipe_file_descriptor[pipe_index - 2], 0) < 0))
				{
					perror("dup2");
					exit(EXIT_FAILURE);
				}
				
				// Entering child process:
				/* Redirection of IO */
				if(redirect)
				{
					if(append_redirect)
					{
						file_descriptor = open(outfile, O_WRONLY | O_CREAT | O_APPEND | O_RDONLY, 0664);
					}
					else
					{
						file_descriptor = open(outfile, O_CREAT | O_TRUNC | O_WRONLY | O_RDONLY, 0664);
					}

					dup2(file_descriptor, STDERR_FILENO);
					close(file_descriptor);
				}

				for(int z = 0; z < 2 * num_of_pipes; z++)
				{
					close(pipe_file_descriptor[z]);
				}

				if(execvp(argv[0], argv) == -1)
				{
					perror("--ERROR EXECUTING COMMAND--");
					exit(EXIT_FAILURE);
				}
			
			}

			else if(pipe_id < 0)
			{
				perror("--ERROR. PIPE ID < 0--");
			}

			pipe_index += 2;
		}

		for(i = 0; i < 2 + num_of_pipes; i++)
		{
			close(pipe_file_descriptor[i]);
		}	  
	}
	free(command);
	free(argv);
	free(command_array);
	free(outfile);
	free(token);
}
