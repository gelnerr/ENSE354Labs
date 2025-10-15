#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/ipc.h>

int main(){
        //step 1
        key_t key = ftok("shared_mem.txt", 1);
        int shm_id = shmget(key, 1024, 0666 | IPC_CREAT);
        if (shm_id==-1){
                perror("Error: shmget failed.");
                exit(1);
        }

        //step 2
        pid_t pid1 = fork();
        if (pid1>0){                                    //Parent logic
                int *account_pointer = (int *) shmat(shm_id, NULL, 0);
                *account_pointer = 1000;                //setting value at account pointer
                printf("[Parent] Parent deposited $1000.\n");
                wait(NULL);
                printf("[Parent] Final value = %d\n", *account_pointer);

                //step 3: cleanup after all processes
                shmdt(account_pointer);                 //detaching
                shmctl(shm_id, IPC_RMID, NULL);         //deleting
        }
        else if (pid1==0){                              //Child logic
                int *account_pointer = (int *) shmat(shm_id, NULL, 0);
                *account_pointer += 100;
                while (*account_pointer < 4000){
                        sleep(1);
                        *account_pointer += 100;
                        printf("[Child] The current balance is %d\n", *account_pointer);
                }
                shmdt(account_pointer);
                exit(0);
        }
        else{
                printf("Forking error.");
                exit(1);
        }
        return 0;
}