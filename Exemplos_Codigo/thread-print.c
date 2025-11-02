/*
Criação de threads POSIX em UNIX, com impressão de variáveis.
 
Compilar com gcc -Wall thread-print.c -o thread-print -lpthread
 
Carlos Maziero, DINF/UFPR 2020
*/
 
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
 
#define NUM_THREADS 16
 
int x = 0 ;  // variável global
 
void *threadBody (void *id)
{
  long tid = (long) id ;
 
  x++ ;
  printf ("t%02ld: Olá!   (x=%02d)\n", tid, x) ;
  sleep (3) ;   
  x++ ;
  printf ("t%02ld: Tchau! (x=%02d)\n", tid, x) ;
  pthread_exit (NULL) ;
}
 
int main (int argc, char *argv[])
{
  pthread_t thread [NUM_THREADS] ;
  long i, status ;
 
  for (i=0; i<NUM_THREADS; i++)
  {
    printf ("Main: criando thread %02ld\n", i) ;
    status = pthread_create (&thread[i], NULL, threadBody, (void *) i) ;    
    if (status)
    {
      perror ("pthread_create") ;
      exit (1) ;
    }
  }
  printf ("Main: fim\n") ;
  pthread_exit (NULL) ;
}
