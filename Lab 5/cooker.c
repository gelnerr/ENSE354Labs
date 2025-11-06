#include <sys/sem.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
int main() {
        key_t key=200499313;
        int shmid;
        int *pizza=0;
        int clock=10;
        int semid;
        if((shmid=shmget(key,50,0600|IPC_CREAT))==-1) {
                perror("shmget");
                exit(1);
        }
        if ((pizza=(int *)shmat(shmid,0,0))==(int *)-1) {
                perror("shmat");
                exit(2);
        }

        //Getting access to a set of 2 semaphores using our key
        if ((semid=semget(key, 2, 0600| IPC_CREAT))==-1){
                perror("semget");
                exit(3);
        }

        #define PIZZA_AVAILABLE 0
        #define SHELF_SPACE 1

        //define union for semctl()
         union semun {
            int val;
            struct semid_ds *buf;
            unsigned short *array;
        } arg;

        //initializing our semaphores (semctl)
        arg.val = 0;    //pizza_availabe = 0
        if (semctl(semid, PIZZA_AVAILABLE, SETVAL, arg) == -1){
                perror("semctl PIZZA_AVAILABLE");
                exit(4);
        }

        arg.val = 1;    //shelf space 1
        if (semctl(semid, SHELF_SPACE, SETVAL, arg) == -1){ perror("semctl SHELF_SPACE");
                exit(5);
        }

        printf("Cooker: I have started cooking pizza.\n");
        while(clock) {
                //wait for empty spot on shelf
                struct sembuf wait_shelf = { SHELF_SPACE, -1, 0 };
                if (semop(semid, &wait_shelf, 1) == -1){
                        perror("semop wait shelf");
                        exit(6);
                }

                //critical section
                sleep(1);
                (*pizza)++;
                printf("Cooker: Made a pizza. Total on Shelf: %d.\n",*pizza);

                struct sembuf signal_pizza = { PIZZA_AVAILABLE, +1, 0};
                if (semop(semid, &signal_pizza, 1) == 1){
                        perror("semop signal pizza");
                        exit(7);
                }

                clock--;
        }//end of while
        printf("Cooker: Time is up. I cooked 10 pizzas. %d are left.\n"
        ,*pizza);
        shmctl(shmid,IPC_RMID,(struct shmid_ds *) 0);
        semctl(semid, 0, IPC_RMID);     //removing semaphores.
        exit(0);
}//end of main