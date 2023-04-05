#define _GNU_SOURCE

#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

#include "utility.h"

#define BITS_PER_BATCH 8

unsigned char* arr;
unsigned char* turn;

int main(int argc, char *argv[])
{
	key_t key = ftok("README",1);
	int shmid = shmget(key,2*4096,0666|IPC_CREAT);
	arr = (unsigned char*) shmat(shmid,(void*)0,0);
	arr[0] = 1;
	
	key_t key3 = ftok("README",3);
	int shmid3 = shmget(key3,8*sizeof(int),0666|IPC_CREAT);
	turn = (unsigned char*) shmat(shmid3,(void*)0,0);
	
	turn[0] = 0;
	
	for (int j = 0; j < BITS_PER_BATCH; j++)
	{
		clflush(&arr[j*64]);
	}
		
	while(1)
	{
		// uint64_t timing = measure_one_block_access_time((uint64_t)&arr[512+(j*64)]);
		
		while (turn[0] != 1)
		{
			
		}
		
		unsigned long long temp = 0;
		
		for (int j = BITS_PER_BATCH-1; j >= 0; j--)
		{
			// _mm_prefetch((char*)(arr[10000+(j*64)]),_MM_HINT_T0);

			uint64_t timing = measure_one_block_access_time((uint64_t)&arr[j*64]);
			printf("%ld ", timing);
			
			temp <<= 1;
			
			if (timing < 200)
			{
				temp |= 1;
			}
		}
		
		for (int j = 0; j < BITS_PER_BATCH; j++)
		{
			clflush(&arr[j*64]);
		}
		
		printf("msg: %lld\n", temp);
		
		// arr[0] = 81;
		turn[0] = 0;
	}
	
	shmdt(arr);
    shmctl(shmid,IPC_RMID,NULL);
	
	return 0;
}