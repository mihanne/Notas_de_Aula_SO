#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#define NUM_THREADS 16

void *print_hello (void *threadid)
// cada thread executa esta função
{
    printf ("%ld: Hello World!\n", (long) threadid);
    sleep (5) ;
    printf ("%ld: Bye bye World!\n", (long) threadid);
    pthread_exit (NULL);
// encerra esta thread
}

int main (int argc, char *argv[])
{
    pthread_t thread[NUM_THREADS];
    long status, i;
    // thread "main"
    for(i = 0; i < NUM_THREADS; i++) // cria as demais threads
    {
        printf ("Creating thread %ld\n", i);
        status = pthread_create (&thread[i], NULL, print_hello, (void *) i);
        if (status) 
        { // ocorreu um erro
            perror ("pthread_create");
            exit (-1);
        }
    }
    printf("Main: encerrando \n");
    pthread_exit (NULL);
}
