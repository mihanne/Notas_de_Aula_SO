/*
O problema do Jantar dos filósofos, sem controle de impasses.

Compilar com gcc -Wall filosofos.c -o filosofos -lpthread

Carlos Maziero, DINF/UFPR 2020
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define NUMFILO 5

pthread_t filosofo [NUMFILO] ;	// threads filosofos
sem_t     hashi    [NUMFILO] ;	// um semaforo para cada palito (iniciam em 1)

// espaços para separar as colunas de impressão
char *space[] = {"", "\t", "\t\t", "\t\t\t", "\t\t\t\t" } ;

// espera um tempo aleatório entre 0 e n segundos (float)
void espera (int n)
{
  sleep (random() % n) ;		// pausa entre 0 e n segundos (inteiro)
  usleep (random() % 1000000) ;	// pausa entre 0 e 1 segundo (float)
}

// filósofo comendo
void come (int f)
{
  printf ("%sF%d COMENDO\n", space[f], f) ;
  espera (1) ;
}

// filósofo meditando
void medita (int f)
{
  printf ("%sF%d meditando\n", space[f], f) ;
  espera (1) ;
}

// pega o hashi
void pega_hashi (int f, int h)
{
  printf ("%sF%d quer  h%d\n", space[f], f, h) ;
  sem_wait (&hashi [h]) ;
  printf ("%sF%d pegou h%d\n", space[f], f, h) ;
}

// larga o hashi
void larga_hashi (int f, int h)
{
  printf ("%sF%d larga h%d\n", space[f], f, h) ;
  sem_post (&hashi [h]) ;
}

// corpo da thread filosofo
void *threadFilosofo (void *arg)
{
  int i = (long int) arg ;
  while (1)
  {
    medita (i) ;
    pega_hashi (i, i) ;
    pega_hashi (i, (i+1) % NUMFILO) ;
    come (i) ;
    larga_hashi (i, i) ;
    larga_hashi (i, (i+1) % NUMFILO) ;
  }
  pthread_exit (NULL) ;
}

// programa principal
int main (int argc, char *argv[])
{
  long i, status ;

  // para o printf não se confundir com a threads
  setvbuf (stdout, 0, _IONBF, 0) ;

  // inicia os hashis
  for(i=0; i<NUMFILO; i++)
    sem_init (&hashi[i], 0, 1) ;

  // inicia os filosofos
  for(i=0; i<NUMFILO; i++)
  {
    status = pthread_create (&filosofo[i], NULL, threadFilosofo, (void *) i) ;
    if (status)
    {
      perror ("pthread_create") ;
      exit (1) ;
    }
  }

  // a main encerra aqui
  pthread_exit (NULL) ;
}

