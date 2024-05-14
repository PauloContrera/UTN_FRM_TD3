#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFF_SIZE 80

int main() {
    int ipc[2]; // Descriptores de archivo para la tubería
    int leido;
    char buff[BUFF_SIZE] = {0};

    // Crear la tubería
    if (pipe(ipc) == -1) {
        perror("Error al crear la tuberia");
        exit(EXIT_FAILURE);
    }

    // Crear un proceso hijo
    pid_t pid = fork();

    if (pid == -1) {
        perror("Error al crear proceso hijo");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Proceso hijo (lector)
        close(ipc[1]); // Cerramos el descriptor de escritura ya que el hijo solo va a leer

        // Leer desde la tubería
        leido = read(ipc[0], buff, sizeof(buff));
        if (leido == -1) {
            perror("Error al leer desde la tuberia en el proceso hijo");
            exit(EXIT_FAILURE);
        }

        // Imprimir el mensaje leído
        printf("Mensaje leido por el proceso hijo: %s\n", buff);

        // Cerrar descriptor de lectura
        close(ipc[0]);
    } else { // Proceso padre (escritor)
        close(ipc[0]); // Cerramos el descriptor de lectura ya que el padre solo va a escribir

        // Pedir mensaje por consola
        printf("Ingrese un mensaje: ");
        fflush(stdout); // Vaciar el buffer de salida para asegurar que el mensaje se muestra antes de leer
        if (fgets(buff, sizeof(buff), stdin) == NULL) {
            perror("Error al leer desde la consola en el proceso padre");
            exit(EXIT_FAILURE);
        }

        // Escribir en la tubería
        if (write(ipc[1], buff, strlen(buff)) == -1) {
            perror("Error al escribir en la tuberia en el proceso padre");
            exit(EXIT_FAILURE);
        }

        // Cerrar descriptor de escritura
        close(ipc[1]);
    }

    return 0;
}
