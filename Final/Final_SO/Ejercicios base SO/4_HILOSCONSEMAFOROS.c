#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

sem_t sem;

// Función del primer hilo
void *hilo1(void *arg) {
    // Esperar a que el semáforo esté disponible
    if (sem_wait(&sem) != 0) {
        perror("Error en sem_wait en hilo1");
        exit(EXIT_FAILURE);
    }

    // Tarea del hilo 1
    printf("Mensaje desde el hilo 1\n");

    // Incrementar el semáforo
    if (sem_post(&sem) != 0) {
        perror("Error en sem_post en hilo1");
        exit(EXIT_FAILURE);
    }

    return NULL;
}

// Función del segundo hilo
void *hilo2(void *arg) {
    // Esperar a que el semáforo esté disponible
    if (sem_wait(&sem) != 0) {
        perror("Error en sem_wait en hilo2");
        exit(EXIT_FAILURE);
    }

    // Tarea del hilo 2
    printf("Mensaje desde el hilo 2\n");

    // Incrementar el semáforo
    if (sem_post(&sem) != 0) {
        perror("Error en sem_post en hilo2");
        exit(EXIT_FAILURE);
    }

    return NULL;
}

int main() {
    pthread_t hilo_id1, hilo_id2;
    int s;

    // Inicializar el semáforo
    if (sem_init(&sem, 0, 1) != 0) {
        perror("Error en sem_init");
        exit(EXIT_FAILURE);
    }

    // Crear los hilos
    s = pthread_create(&hilo_id1, NULL, hilo1, NULL);
    if (s != 0) {
        printf("Error en pthread_create para hilo1: %d\n", s);
        exit(EXIT_FAILURE);
    }

    s = pthread_create(&hilo_id2, NULL, hilo2, NULL);
    if (s != 0) {
        printf("Error en pthread_create para hilo2: %d\n", s);
        exit(EXIT_FAILURE);
    }

    // Esperar a que los hilos terminen
    s = pthread_join(hilo_id1, NULL);
    if (s != 0) {
        printf("Error en pthread_join para hilo1: %d\n", s);
        exit(EXIT_FAILURE);
    }

    s = pthread_join(hilo_id2, NULL);
    if (s != 0) {
        printf("Error en pthread_join para hilo2: %d\n", s);
        exit(EXIT_FAILURE);
    }

    // Destruir el semáforo
    if (sem_destroy(&sem) != 0) {
        perror("Error en sem_destroy");
        exit(EXIT_FAILURE);
    }

    return 0;
}
