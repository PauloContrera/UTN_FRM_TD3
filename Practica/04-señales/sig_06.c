#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#include <stdbool.h>  // Agregamos la librería correspondiente

#define TRUE true     // Definimos la constante TRUE

/*
Escriba un programa sig_04.c que cree tres procesos hijos que ejecuten un bucle
infinito. Luego de una espera activa, el proceso padre debe finalizar cada uno de los
procesos enviando a cada uno de ellos la señal SIGKILL. Para ello utilice la función
kill()
*/

#define NUM_HIJOS 3

int main() {
    int i;
    pid_t pid;
    pid_t hijos[NUM_HIJOS];

    // Crear los procesos hijos
    for (i = 0; i < NUM_HIJOS; i++) {
        pid = fork();
    printf("\nCreando Pibe %d",getpid());

        if (pid == 0) {  // Proceso hijo
            while (1);  // Bucle infinito
        } else if (pid > 0) {  // Proceso padre
            hijos[i] = pid;
        } else {  // Error
            perror("Error al crear proceso hijo");
            exit(EXIT_FAILURE);
        }
    }

    // Esperar un poco
    sleep(1);

    // Finalizar los procesos hijos
    for (i = 0; i < NUM_HIJOS; i++) {
        if (kill(hijos[i], SIGKILL) < 0) {
            perror("Error al finalizar proceso hijo");
            exit(EXIT_FAILURE);
        }
    }

    // Esperar a que los procesos hijos terminen
    for (i = 0; i < NUM_HIJOS; i++) {
        wait(NULL);
    }

    printf("Procesos hijos finalizados\n");
    return 0;
}
