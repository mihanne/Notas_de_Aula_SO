#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_LEITORES 5
#define NUM_ESCRITORES 2

pthread_mutex_t marea = PTHREAD_MUTEX_INITIALIZER;

void* leitor(void* arg) {
    int id = *(int*)arg;

    while (1) {
        pthread_mutex_lock(&marea);
        printf("[Leitor %d] lendo dados...\n", id);
        sleep(rand() % 2 + 1);  // simula tempo de leitura
        printf("[Leitor %d] terminou leitura.\n", id);
        pthread_mutex_unlock(&marea);

        sleep(rand() % 3 + 1);  // intervalo entre leituras
    }

    return NULL;
}

void* escritor(void* arg) {
    int id = *(int*)arg;

    while (1) {
        pthread_mutex_lock(&marea);
        printf(">>> [Escritor %d] escrevendo dados...\n", id);
        sleep(rand() % 2 + 1);  // simula tempo de escrita
        printf(">>> [Escritor %d] terminou escrita.\n", id);
        pthread_mutex_unlock(&marea);

        sleep(rand() % 4 + 2);  // intervalo entre escritas
    }

    return NULL;
}

int main() {
    pthread_t leitores[NUM_LEITORES];
    pthread_t escritores[NUM_ESCRITORES];
    int ids_leitores[NUM_LEITORES];
    int ids_escritores[NUM_ESCRITORES];

    srand(time(NULL));

    for (int i = 0; i < NUM_LEITORES; i++) {
        ids_leitores[i] = i + 1;
        pthread_create(&leitores[i], NULL, leitor, &ids_leitores[i]);
    }

    for (int i = 0; i < NUM_ESCRITORES; i++) {
        ids_escritores[i] = i + 1;
        pthread_create(&escritores[i], NULL, escritor, &ids_escritores[i]);
    }

    for (int i = 0; i < NUM_LEITORES; i++) {
        pthread_join(leitores[i], NULL);
    }

    for (int i = 0; i < NUM_ESCRITORES; i++) {
        pthread_join(escritores[i], NULL);
    }

    return 0;
}

