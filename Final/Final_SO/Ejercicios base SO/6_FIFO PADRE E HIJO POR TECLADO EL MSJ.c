#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

#define FIFO_PATH "/tmp/MI_FIFO"
#define MAX_MESSAGE_LENGTH 80

int main() {
    int err, fifo_d;
    char buff[MAX_MESSAGE_LENGTH];
    int leido;

    // Si la FIFO existe, la borramos
    unlink(FIFO_PATH);

    // Establecemos los permisos para el FIFO
    err = mkfifo(FIFO_PATH, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if (err == -1) {
        perror("Error al crear FIFO");
        exit(EXIT_FAILURE);
    }

    // Creamos un proceso hijo
    pid_t pid = fork();

    if (pid == -1) {
        perror("Error al crear proceso hijo");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Proceso hijo
        // Solicitamos el mensaje al usuario
        printf("Introduce el mensaje para escribir en la FIFO: ");
        fflush(stdout); // Limpiar el buffer de salida para asegurarse de que la solicitud de entrada se imprima inmediatamente
        if (fgets(buff, sizeof(buff), stdin) == NULL) {
            perror("Error al leer el mensaje desde el teclado");
            exit(EXIT_FAILURE);
        }

        // Abrimos FIFO para escritura
        fifo_d = open(FIFO_PATH, O_WRONLY);
        if (fifo_d == -1) {
            perror("Error al abrir FIFO para escritura en el hijo");
            exit(EXIT_FAILURE);
        }

        // Escribimos en FIFO
        err = write(fifo_d, buff, strlen(buff));
        if (err == -1) {
            perror("Error al escribir en FIFO desde el hijo");
            exit(EXIT_FAILURE);
        }

        // Cerramos FIFO
        close(fifo_d);
    } else { // Proceso padre
        // Abrimos FIFO para lectura
        fifo_d = open(FIFO_PATH, O_RDONLY);
        if (fifo_d == -1) {
            perror("Error al abrir FIFO para lectura en el padre");
            exit(EXIT_FAILURE);
        }

        // Leemos de FIFO
        leido = read(fifo_d, buff, sizeof(buff));
        if (leido < 1) {
            perror("Error al leer de FIFO en el padre");
            exit(EXIT_FAILURE);
        }

        // Mostramos el mensaje leído
        printf("Mensaje leído por el padre: %s\n", buff);

        // Cerramos FIFO
        close(fifo_d);
    }

    return 0;
}
