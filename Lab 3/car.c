#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

//Signal Handlers
void carIsComing(int sig){
    printf("The president's car is coming. Take security measures.\n");
}

void carIsGone(int sig){
    printf("The president's car is gone. You may relax\n");
}

int main(){
    pid_t pid;

    signal(SIGUSR1, carIsComing);
    signal(SIGUSR2, carIsGone);

    pid = fork(); //creating a child
    if (pid<0){
        perror("Forking error.\n");
        exit(1);
    }
    else if (pid == 0){
        pid_t ppid = getppid(); //parent process ID
        printf("[Child] My PID is %d, my parent's PID is %d\n", getpid(), ppid);
        printf("[Child] Sending SIGUSR1 to parent...\n");
        kill(ppid, SIGUSR1);    //SIGUSR1 to parent

        sleep(3);

        printf("[Child] Sending SIGUSR2 to parent...\n");
        kill(ppid, SIGUSR2);
        exit(0);
    }
    else {
        printf("[Parent] My PID is %d. Waiting for signals from child %d...\n", getpid(), pid);
        // Pause until first signal is received
        pause();
        // Pause until second signal is received
        pause();

        wait(NULL); // Wait for child to terminate
        printf("[Parent] Terminating.\n");
    }
    return 0;
}
