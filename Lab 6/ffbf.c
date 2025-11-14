#include <stdio.h>
#include <stdlib.h>
#define NUM_PARTITIONS 8
#define NUM_PROCESSES 6

void firstFit(int *partitions, int *processes, int *allocation);
void bestFit(int *partitions, int *processes, int *allocation);

int main()
{
    int partitions[NUM_PARTITIONS] = {100, 300, 200, 100, 400, 800, 500, 700};
    int processes[NUM_PROCESSES]   = {233, 455, 632, 312, 120, 505};

    int allocation[NUM_PROCESSES];

    // FIRST FIT
    firstFit(partitions, processes, allocation);

    printf("First Fit Allocation:\n");
    for (int i = 0; i < NUM_PROCESSES; i++) {
        if (allocation[i] == -1)
            printf("Process %d (%dk) -> Not allocated\n", i, processes[i]);
        else
            printf("Process %d (%dk) -> Partition %d (%dk)\n",
                   i, processes[i], allocation[i], partitions[allocation[i]]);
    }

    printf("\n");

    // BEST FIT
    bestFit(partitions, processes, allocation);

    printf("Best Fit Allocation:\n");
    for (int i = 0; i < NUM_PROCESSES; i++) {
        if (allocation[i] == -1)
            printf("Process %d (%dk) -> Not allocated\n", i, processes[i]);
        else
            printf("Process %d (%dk) -> Partition %d (%dk)\n",
                   i, processes[i], allocation[i], partitions[allocation[i]]);
    }

    return 0;
}

void firstFit(int *partitions, int *processes, int *allocation)
{
    int occupied[NUM_PARTITIONS] = {0};	// 0 = free, 1 = used
    for (int i = 0; i < NUM_PROCESSES; i++) {
        allocation[i] = -1;
        for (int j = 0; j<NUM_PARTITIONS; j++){
		if (occupied[j] == 0){	//partition free
			if (partitions[j] >= processes[i]){	//partition big enough
				allocation[i] = j;	//record which process i ended up in
				occupied[j] = 1;
				break; 		//break after first fit
			}
		}
	}
    }
}

void bestFit(int *partitions, int *processes, int *allocation)
{
    int occupied[NUM_PARTITIONS] = {0}; // track which partitions are used
    for (int i = 0; i < NUM_PROCESSES; i++) {
        allocation[i] = -1;
        int bestIndex = -1;
        for (int j = 0; j<NUM_PARTITIONS; j++){
		if (occupied[j] == 0 && partitions[j] >= processes[i]){
			if (bestIndex == -1 || partitions[j] < partitions[bestIndex]){
				bestIndex = j;
			}
		}
	}
	if (bestIndex!=-1){
		allocation[i] = bestIndex;
		occupied[bestIndex] = 1;
	}
    }
}
