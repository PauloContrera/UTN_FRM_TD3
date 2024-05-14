#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

// Función del primer hilo
void *hilo1(void *arg) {
    // Bloquear el mutex
    int s = pthread_mutex_lock(&mtx);
    if (s != 0) {
        printf("Error en pthread_mutex_lock en hilo1: %d\n", s);
        exit(EXIT_FAILURE);
    }

    // Tarea del hilo 1
    printf("Mensaje desde el hilo 1\n");

    // Desbloquear el mutex
    s = pthread_mutex_unlock(&mtx);
    if (s != 0) {
        printf("Error en pthread_mutex_unlock en hilo1: %d\n", s);
        exit(EXIT_FAILURE);
    }

    return NULL;
}

// Función del segundo hilo
void *hilo2(void *arg) {
    // Bloquear el mutex
    int s = pthread_mutex_lock(&mtx);
    if (s != 0) {
        printf("Error en pthread_mutex_lock en hilo2: %d\n", s);
        exit(EXIT_FAILURE);
    }

    // Tarea del hilo 2
    printf("Mensaje desde el hilo 2\n");

    // Desbloquear el mutex
    s = pthread_mutex_unlock(&mtx);
    if (s != 0) {
        printf("Error en pthread_mutex_unlock en hilo2: %d\n", s);
        exit(EXIT_FAILURE);
    }

    return NULL;
}

int main() {
    pthread_t hilo_id1, hilo_id2;
    int s;

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

    // Destruir el mutex
    s = pthread_mutex_destroy(&mtx);
    if (s != 0) {
        printf("Error en pthread_mutex_destroy: %d\n", s);
        exit(EXIT_FAILURE);
    }

    return 0;
}
