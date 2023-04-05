#include <openssl/md5.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "utility.h"

#define BITS_PER_BATCH 8

unsigned char* arr;
unsigned char* turn;

unsigned long long secret = 100;

void encrypt_secret(unsigned long long key)
{
	secret += key;
}
void decrypt_secret(unsigned long long key)
{
	secret -= key;
}

void send_secret()
{
	// printf("turn: %d\n", turn[0]);
	
	while(turn[0] != 0)
	{
		
	}
	
	for (int j = 0; j < BITS_PER_BATCH; j++)
	{
		if((secret >> j) & 1)
		{
			asm volatile("mfence");
			one_block_access((uint64_t)&arr[j*64]);
		}
		// usleep(1);
	}
	
	turn[0] = 1;
}

int fib(int n)
{
    if (n <= 1)
        return n;
    return fib(n - 1) + fib(n - 2);
}

unsigned int factorial(unsigned int n)
{
	unsigned long long key = 1;
	
	encrypt_secret(key);
	send_secret();
	
	printf("in: %llu\n", secret);
	// printf("n is: %d\n", n);
	
	// printf("in: %lld\n", secret);
	// int temp = fib(2);
	
	if (n == 0)
	{
		printf("ou: %llu\n", secret);
		send_secret();
		decrypt_secret(key);
		
        return 1;
	}
	
	unsigned int ret = n * factorial(n - 1);
	
	printf("ou: %llu\n", secret);
	send_secret();
	decrypt_secret(key);
    
	return ret;
}
  
int main()
{
	unsigned char c[MD5_DIGEST_LENGTH];
	FILE *inFile = fopen ("program.c", "r");
	MD5_CTX mdContext;
	int bytes;
    unsigned char data[1024];

    MD5_Init (&mdContext);
    while ((bytes = fread (data, 1, 1024, inFile)) != 0)
	{
        MD5_Update (&mdContext, data, bytes);
	}
    MD5_Final (c,&mdContext);
	
    for(int i = 0; i < MD5_DIGEST_LENGTH; i++)
	{
		printf("%02x", c[i]);
	}
	printf("\n");
	
	
	key_t key = ftok("README",1);
	// int shmid = shmget(key,4096,0666|IPC_CREAT);
	int shmid = shmget(key,BITS_PER_BATCH*512,0666|IPC_CREAT);
	arr = (unsigned char*) shmat(shmid,(void*)0,0);
	
	key_t key3 = ftok("README",3);
	int shmid3 = shmget(key3,8*sizeof(int),0666|IPC_CREAT);
	turn = (unsigned char*) shmat(shmid3,(void*)0,0);
	

	
    int num = 8;
	clock_t t = clock();
	unsigned int val = factorial(num);
	
	t = clock() - t;
    printf("Factorial of %u is %u\n", num, val);
	
	double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
 
    printf("exec time: %f\n", time_taken);
	shmdt(arr);
	shmdt(turn);
	
    return 0;
}