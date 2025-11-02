#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define TAM_BUFFER 6
#define NUM_PRODUTORES 5
#define NUM_CONSUMIDORES 3

int buffer[TAM_BUFFER];
int in = 0, out = 0;
int num_itens = 0; // contador de itens no buffer

// Controle de sincronização
pthread_mutex_t mbuf = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_vaga = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_item = PTHREAD_COND_INITIALIZER;

void* produtor(void* arg) {
    int id = *(int*)arg;

    while (1) {
        int produto = rand() % 100;

        pthread_mutex_lock(&mbuf);

        // Espera enquanto o buffer estiver cheio
        while (num_itens == TAM_BUFFER)
            pthread_cond_wait(&cond_vaga, &mbuf);

        // Insere no buffer
        buffer[in] = produto;
        printf("[Produtor %d] Produziu %d na posição %d\n", id, produto, in);
        in = (in + 1) % TAM_BUFFER;
        num_itens++;

        pthread_cond_signal(&cond_item); // sinaliza que há item disponível
        pthread_mutex_unlock(&mbuf);

        sleep(rand() % 2 + 1);
    }

    return NULL;
}

void* consumidor(void* arg) {
    int id = *(int*)arg;

    while (1) {
        pthread_mutex_lock(&mbuf);

        // Espera enquanto o buffer estiver vazio
        while (num_itens == 0)
            pthread_cond_wait(&cond_item, &mbuf);

        // Remove do buffer
        int produto = buffer[out];
        printf("   [Consumidor %d] Consumiu %d da posição %d\n", id, produto, out);
        out = (out + 1) % TAM_BUFFER;
        num_itens--;

        pthread_cond_signal(&cond_vaga); // sinaliza que há vaga disponível
        pthread_mutex_unlock(&mbuf);

        sleep(rand() % 3 + 1);
    }

    return NULL;
}

int main() {
    pthread_t prod[NUM_PRODUTORES];
    pthread_t cons[NUM_CONSUMIDORES];
    int ids_prod[NUM_PRODUTORES];
    int ids_cons[NUM_CONSUMIDORES];

    srand(time(NULL));

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

    return 0;
}

