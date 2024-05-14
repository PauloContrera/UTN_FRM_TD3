#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Función que se ejecutará dentro del hilo
void *hola(void *nro) {
    sleep(2); // El hilo espera 2 segundos
    printf("Hola, soy el hilo %d\n", *(int *)nro); // Se imprime un mensaje que indica el número del hilo
    pthread_exit(NULL); // Se finaliza el hilo
}

int main() {
    pthread_t hilo[1]; // Declaración de un array de hilos
    int rc, t; // Declaración de variables para almacenar el código de retorno de pthread_create y el número de hilo
    t = 0; // Inicialización del número de hilo

    // Creación del hilo
    rc = pthread_create(&hilo[0], NULL, hola, (void *)&t);
    if (rc) { // Verificación de errores
        printf("Error al crear el hilo; código de retorno: %d\n", rc);
        exit(-1);
    }

    // Espera de finalización del hilo
    pthread_join(hilo[0], NULL);

    // Se imprime un mensaje indicando que el hilo ha finalizado
    printf("El hilo ha finalizado\n");

    // Se finaliza el hilo principal
    pthread_exit(NULL);

    return 0;
}
