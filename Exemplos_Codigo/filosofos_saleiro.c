#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "semaphore.h"

#define NUM_FILOSOFOS 5

sem_t palito[NUM_FILOSOFOS];  // Semáforos dos palitos
sem_t saleiro;                // Semáforo do saleiro

void meditar(int id) {
    printf("Filósofo %d está meditando.\n", id);
    sleep(rand() % 3 + 1);
}

void comer(int id) {
    printf("Filósofo %d está comendo.\n", id);
    sleep(rand() % 2 + 1);
}

void* filosofo(void* arg) {
    int id = *(int*)arg;
    int dir = id;
    int esq = (id + 1) % NUM_FILOSOFOS;

    while (1) {
        meditar(id);

        sem_down(&saleiro);  // Pega o saleiro
        sem_down(&palito[dir]);
        sem_down(&palito[esq]);
        sem_up(&saleiro);    // Devolve o saleiro

        comer(id);

        sem_up(&palito[dir]);
        sem_up(&palito[esq]);
    }

    return NULL;
}

int main() {
    pthread_t threads[NUM_FILOSOFOS];
    int ids[NUM_FILOSOFOS];

    srand(time(NULL));

    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        sem_init(&palito[i], 1);
    }
    sem_init(&saleiro, 1);  // Saleiro inicia com 1

    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, filosofo, &ids[i]);
    }

    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

