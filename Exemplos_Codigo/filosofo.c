#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


#define NUM_THREADS 16


void *threadBody (void *id)
{
	long tid =(long) id;
	printf("t%2ld: Olá!\n", tid);
	sleep(3);
	printf("t%2ld: Tchau!!\n", tid);
	
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	pthread_t thread[NUM_THREADS];
	long i, status ;
	
	for (i=0; i<N; i++)
	{
		printf("Main: criando thread %02ld\n", i);
		
		status= pthread_create (&thread[i], NULL, threadBody, (void *) i);
		
		if (status)
		{
			perror("phtread_create");
			exit(1);
		}
	}
	pthread_exit (NULL);
}
