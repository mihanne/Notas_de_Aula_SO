#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "semaphore.h"

#define TAM_BUFFER 6
#define NUM_PRODUTORES 5
#define NUM_CONSUMIDORES 3

int buffer[TAM_BUFFER];
int in = 0, out = 0;

sem_t mutex;  // exclusão mútua no buffer
sem_t vaga;   // controla número de vagas disponíveis
sem_t item;   // controla número de itens disponíveis

void* produtor(void* arg) {
    int id = *(int*)arg;

    while (1) {
        int produto = rand() % 100;

        sem_down(&vaga);
        sem_down(&mutex);

        // inserir no buffer
        buffer[in] = produto;
        printf("[Produtor %d] Produziu %d na posição %d\n", id, produto, in);
        in = (in + 1) % TAM_BUFFER;

        sem_up(&mutex);
        sem_up(&item);

        sleep(rand() % 2 + 1);  // simula tempo de produção
    }
    return NULL;
}

void* consumidor(void* arg) {
    int id = *(int*)arg;

    while (1) {
        sem_down(&item);
        sem_down(&mutex);

        // consumir do buffer
        int produto = buffer[out];
        printf("   [Consumidor %d] Consumiu %d da posição %d\n", id, produto, out);
        out = (out + 1) % TAM_BUFFER;

        sem_up(&mutex);
        sem_up(&vaga);

        sleep(rand() % 3 + 1);  // simula tempo de consumo
    }
    return NULL;
}

int main() {
    pthread_t prod[NUM_PRODUTORES];
    pthread_t cons[NUM_CONSUMIDORES];
    int ids_prod[NUM_PRODUTORES];
    int ids_cons[NUM_CONSUMIDORES];

    srand(time(NULL));

    sem_init(&mutex, 1);
    sem_init(&vaga, TAM_BUFFER);
    sem_init(&item, 0);

    for (int i = 0; i < NUM_PRODUTORES; i++) {
        ids_prod[i] = i + 1;
        pthread_create(&prod[i], NULL, produtor, &ids_prod[i]);
    }

    for (int i = 0; i < NUM_CONSUMIDORES; i++) {
        ids_cons[i] = i + 1;
        pthread_create(&cons[i], NULL, consumidor, &ids_cons[i]);
    }

    for (int i = 0; i < NUM_PRODUTORES; i++) {
        pthread_join(prod[i], NULL);
    }

    for (int i = 0; i < NUM_CONSUMIDORES; i++) {
        pthread_join(cons[i], NULL);
    }

    sem_destroy(&mutex);
    sem_destroy(&vaga);
    sem_destroy(&item);

    return 0;
}

