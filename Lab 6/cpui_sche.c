#include <stdio.h>
#include <stdlib.h>
#define NUM_PROCESS 5

float fcfs(int *cpuBurstTime);
float priority(int *cpuBurstTime, int *processPriority);
float sjf(int *cpuBurstTime);

int main()
{
	int cpuBurstTime[NUM_PROCESS]={10,29,3,7,12};
	int processPriority[NUM_PROCESS]={3,1,5,4,2};
	float aveWaitingTimeFcFs;
	float aveWaitingTimePriority;
	float aveWaitingTimeSjf;

	aveWaitingTimeFcFs = fcfs(cpuBurstTime);
	printf("First come first serve algorithm:\n");
	printf("average waiting time:%5.3f\n", aveWaitingTimeFcFs);
	aveWaitingTimePriority = priority(cpuBurstTime,processPriority);
	printf("Priority Scheduling algorithm:\n");
	printf("average waiting time:%5.3f\n", aveWaitingTimePriority);
	aveWaitingTimeSjf = sjf(cpuBurstTime);
	printf("Shortest Job First algorithm:\n");
        printf("average waiting time:%5.3f\n", aveWaitingTimeSjf);
}

float fcfs(int *cpuBurstTime)
{
	float avWaitingTime, waitingTime=0;
	int i=0;
	float sum=0;
	while(i!=(NUM_PROCESS-1))
	{
		waitingTime+=*(cpuBurstTime+i);
		sum+=waitingTime; i++;
	}
	avWaitingTime=sum/NUM_PROCESS;
	return avWaitingTime;
}

float priority(int *cpuBurstTime, int *processPriority)
{
	int sortedProcess[NUM_PROCESS];
	int i;
	float avWaitingTime;
	for(i=0;i < NUM_PROCESS;i++)
	{
		sortedProcess[*(processPriority+i)-1]=*(cpuBurstTime+i);
	}
	avWaitingTime=fcfs(sortedProcess);
	return avWaitingTime;
}


float sjf(int *cpuBurstTime){
	int i, j, temp;
	int sorted[NUM_PROCESS];
	float waitingTime = 0;
	float sum = 0;

	//Copy of original array into sorted[]
	for (i=0; i<NUM_PROCESS; i++){
		sorted[i] = cpuBurstTime[i];
	}

	//selection sort algo
	for (i=0; i<NUM_PROCESS - 1; i++){
		int minIndex = i;
		for (j = i+1; j<NUM_PROCESS;j++){
			if (sorted[j] < sorted[minIndex]){
				minIndex = j;
			}
		}

		if (minIndex != i){
			temp = sorted[i];
			sorted[i] = sorted[minIndex];
			sorted[minIndex] = temp;
		}
	}

	for (i=0; i<NUM_PROCESS - 1; i++){
		waitingTime += sorted[i];
		sum+= waitingTime;
	}

	float avWaitingTime = sum / NUM_PROCESS;
	return avWaitingTime;
}

