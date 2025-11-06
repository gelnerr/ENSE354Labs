#define _XOPEN_SOURCE 500
#include <sys/sem.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <signal.h>
#include <time.h>

static int *pizza_global = NULL;
void signal_catcher();

int main() {
        key_t key=200499313;
        int shmid;
        int *pizza;
        int semid;
        sigset(SIGALRM,signal_catcher);
        alarm(30); //set a timer (30 seconds). End elegantly
        if((shmid=shmget(key,50,IPC_CREAT|0600))==-1) {
                perror("shmget");
                exit(1);
        }
        if((pizza=(int *)shmat(shmid,0,0))==(int *)-1) {
                perror("shmat");
                exit(2);
        }

        pizza_global = pizza;
        if ((semid = semget(key,2,0600|IPC_CREAT)) == -1){
                perror("semget");
                exit(3);
        }

        srand((unsigned int)(getpid() ^ time(NULL))); //randomizes time taken to>

        printf("Waiter: Ready to serve pizzas.\n");
        while(1){
                struct sembuf wait_pizza = { 0, -1, 0 };
                if (semop(semid, &wait_pizza, 1) == -1) {
                        perror("semop wait pizza");
                        exit(4);
                }

                sleep(rand()%2+1);
                (*pizza)--;
                printf("Waiter: Picked up a Pizza. Pizzas on shelf now = %d\n", *pizza);

                struct sembuf signal_shelf = {1, +1, 0};
                if (semop(semid, &signal_shelf, 1) == -1){
                        perror("semop signal shelf");
                        exit(5);
                }

                sleep(rand()%2+1);      //sleep random 1-2 secs
        }
}//End of main

void signal_catcher() {
        if (pizza_global && pizza_global != (int *)-1){
                shmdt(pizza_global);    //detachment
                printf("Waiter: Time is up. Detaching and exiting.");
        }
        exit(0);
}