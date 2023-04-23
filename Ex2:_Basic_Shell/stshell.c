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


#define MAX_CMDS 10
#define MAX_LENGTH 100


void handle_sigint();

int main() {

    char *command = malloc(1024);
    char cmd_cpy[1024];
    char *cmds[MAX_CMDS];
    char *token;
    char *outfile;
    int i, fd, redirect, status, append_redirect, numPipes, num_cmds;
    char *argv[MAX_CMDS];
    char prompt[1024] = "hello";


    // Register the signal handler
    signal(SIGINT, handle_sigint);

    while (1){
        
        
        printf("%s: ", prompt);
        fflush(stdout);
        if (fgets(command, 1024, stdin) == NULL) 
        {
            if(command[strlen(command)-1] != '\n')
            {
                printf("Command buffer is overflowed\n");
                exit(1);
            }
            // Handle EOF or error
            break;
        }
        
        
        command[strlen(command) - 1] = '\0';
        strcpy(cmd_cpy, command);

        /* Check if user wants to quit */
        if (!strcmp(command, "exit")) 
            break;


        // Parsing command line

        // User input IS a pipe command
        if(strchr(command, '|') != NULL) 
        {
            
            num_cmds = 0;
            for(i=0; i<MAX_CMDS; i++)
            {
                cmds[i] = NULL;
                cmds[i] = (char *)malloc(MAX_LENGTH * sizeof(char));
            }

            // tokenizing the command line input
            token = strtok(command, "|");
            while(token != NULL && num_cmds < MAX_CMDS)
            {
                while(*token == ' ') 
                    token++;
                
                strcpy(cmds[num_cmds], token);
                token = strtok(NULL, "|");
                num_cmds++;
            }

            numPipes = num_cmds -1;  
        }

        // User input is NOT a pipe command
        else
        {
            cmds[0] = (char *)malloc(MAX_LENGTH * sizeof(char));
            strcpy(cmds[0], command);
            num_cmds = 1;
            numPipes = 0;
        }

        int pipe_fd[2*numPipes];

        for(i = 0; i < (numPipes); i++)
        {
            if(pipe(pipe_fd + i*2) < 0) 
            {
                perror("--ERROR PIPING--\n");
                exit(EXIT_FAILURE);
            }
        }

        int pipe_index=0;
        // Create pipes and fork processes
        for(int k=0; k<num_cmds; k++)
        {
            char *cmd = cmds[k];
            i = 0;
            token = strtok (cmd," ");

            for(i=0; i<MAX_CMDS; i++)
            {
                argv[i] = malloc(MAX_LENGTH * sizeof(char));
            }

            // Copying the words of the command as arguments in argv[]:
            i = 0;
            while (token != NULL)
            {
                if(i<10)
                {
                    strcpy(argv[i], token);
                    i++;
                } 
                else 
                {
                    printf("--To many arguments in argv.--\n");
                    break;
                }
                
                token = strtok (NULL, " ");
            }

            argv[i] = NULL;
            redirect = 0;
            append_redirect = 0;
            outfile = NULL;
        
        

            /* Is command empty */
            if (argv[0] == NULL)
                continue;

            /* Does command line end with & */ 
            if (! strcmp(argv[i - 1], "&")) 
                argv[i - 1] = NULL;
                

            /* Does command line contain ">" or ">>" */ 
            if(i > 1)
            {
                if (! strcmp(argv[i - 2], ">") || (! strcmp(argv[i - 2], ">>")))
                {
                    redirect = 1;
                    if((! strcmp(argv[i - 2], ">>")))
                    {
                        append_redirect =1 ;
                    }
                    outfile = argv[i - 1];
                    argv[i - 2] = NULL;
                    argv[i-1] = NULL;
                }
                /* Does command line contain "2>" */ 
                else if(!strcmp(argv[i-2], "2>"))
                {
                    redirect = 3;
                    argv[i-2] = NULL;
                    outfile = argv[i-1];
                    fd = creat(outfile, 0660);
                    close(STDERR_FILENO);
                    dup(fd);
                    close(fd);
                } else 
                {
                    redirect = append_redirect = 0; 
                }
            }


            /* Command to change directory */
            if (!strcmp(argv[0], "cd")) 
            {
                if (i < 2) 
                {
                    fprintf(stderr, "cd: missing operand\n");
                }
                else if (chdir(argv[1]) != 0) 
                {
                    fprintf(stderr, "cd: %s: No such file or directory\n", argv[1]);
                }
                continue;
            }


            /* for commands not part of the shell command language */ 
            pid_t pid = fork();
            if (pid == 0) 
            {
                //if not last command
                if(k + 1 != num_cmds)
                {
                    if(dup2(pipe_fd[pipe_index + 1], 1) < 0)
                    {
                        perror("dup2");
                        exit(EXIT_FAILURE);
                    }
                }

                //if not first command&& pipe_index!= 2*numPipes
                if(pipe_index != 0 )
                {
                    if(dup2(pipe_fd[pipe_index-2], 0) < 0)
                    {
                        perror(" dup2");
                        exit(EXIT_FAILURE);

                    }
                }
                // Entering child proccess: 
                /* redirection of IO ? */
                if(redirect)
                {
                    if(append_redirect)
                    {
                        fd = open(outfile, O_WRONLY | O_CREAT | O_APPEND | O_RDONLY, 0644);
                    } else
                    {
                        fd = open(outfile, O_CREAT | O_TRUNC | O_WRONLY | O_RDONLY, 0644);
                    }
                    
                    // close(STDOUT_FILENO);
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                }

                for(int z = 0; z < 2*numPipes; z++)
                {
                    close(pipe_fd[z]);
                }

                if (execvp(argv[0], argv) == -1) 
                {
                    perror("Error executing command");
                    exit(EXIT_FAILURE);
                }
                
                
            }

            else if (pid < 0) 
            {
                // Error
                perror("fork");
            } 

            pipe_index+=2;
        }

        for(i = 0; i < 2 * numPipes; i++)
        {
            close(pipe_fd[i]);
        }

        for(i = 0; i < numPipes + 1; i++)
        {
            wait(&status);
        }
         
    }
    free(command); 
}


void handle_sigint()
{
    printf("\nYou typed Control-C!\n");
    fflush(stdout);
    printf("hello: ");
    fflush(stdout);
}