#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(){
        int my_pipe[2];
        char buffer[100];

        if (pipe(my_pipe)==-1){
                perror("Pipe creation failed\n");
                exit(1);}
        printf("Pipe created.\n");

        pid_t pid1 = fork();
        if (pid1>0){                                    //parent logic
                close(my_pipe[0]);                      //close reader, parent is writing
                printf("Enter oil price/gallon: ");
                scanf("%s", buffer);
                printf("\n");

                write(my_pipe[1], buffer, sizeof(buffer));      //sending string through pipe
                printf("[Parent] Sending price '%s' to child..\n", buffer);
                close(my_pipe[1]);                              //closed once done
                wait(NULL);                                     //waiting for child to finish
                printf("[Parent] Child finished. Parent is exiting.\n");
                exit(0);
        }

        else if (pid1==0){                              //child logic
                close(my_pipe[1]);                      //close writer, child doesnt need to write

                read(my_pipe[0], buffer, sizeof(buffer));       //getting message from pipe
                printf("[Child] Received data from parent: '%s'\n", buffer);
                char *endptr;                                   //ref pointer for where parsing stops
                float price_per_gallon = strtof(buffer, &endptr);
                float price_per_litre = price_per_gallon / 3.785f; // conversion

                printf("Final Price/Litre: %.2f\n", price_per_litre);

                close(my_pipe[0]);
                exit(0);
        }
        else{
                perror("Error creating child.\n");
                exit(1);
        }
        return 0;
}

