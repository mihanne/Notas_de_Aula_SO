#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "semaphore.h"

#define NUM_FILOSOFOS 5

sem_t palitos[NUM_FILOSOFOS];

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

        sem_down(&palitos[dir]);
        printf("Filósofo %d pegou palito direito (%d).\n", id, dir);

        sem_down(&palitos[esq]);
        printf("Filósofo %d pegou palito esquerdo (%d).\n", id, esq);

        comer(id);

        sem_up(&palitos[dir]);
        printf("Filósofo %d devolveu palito direito (%d).\n", id, dir);

        sem_up(&palitos[esq]);
        printf("Filósofo %d devolveu palito esquerdo (%d).\n", id, esq);
    }

    return NULL;
}

int main() {
    pthread_t threads[NUM_FILOSOFOS];
    int ids[NUM_FILOSOFOS];

    srand(time(NULL));

    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        sem_init(&palitos[i], 1);
    }

    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, filosofo, &ids[i]);
    }

    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        sem_destroy(&palitos[i]);
    }

    return 0;
}

