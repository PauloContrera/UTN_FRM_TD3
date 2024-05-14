#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void sigusr1_handler(int sig) {
    write(STDOUT_FILENO, "Terminé\n", sizeof("Terminé\n"));
    exit(0);
}

int main() {
    // Imprime el PID del proceso
    printf("PID del proceso: %d\n", getpid());

    // Establece el manejador de señal para SIGUSR1
    signal(SIGUSR1, sigusr1_handler);

    pid_t pid = fork();

    if (pid < 0) {
        perror("Error al crear el proceso hijo");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {  // Proceso hijo
        // Envía la señal SIGUSR1 al proceso padre
        kill(getppid(), SIGUSR1);
    }
    while(1){
        sleep(1);
    }

    exit(0);
}
