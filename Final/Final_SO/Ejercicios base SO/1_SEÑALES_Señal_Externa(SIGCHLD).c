#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

// Definición del manejador de señales
void sigchld_handler(int signum) {
    pid_t pid;
    int status;
    pid = wait(&status); // Llamada a wait() para obtener el estado de finalización del hijo
    printf("Proceso hijo con PID %d finalizado. Estado de salida: %d\n", pid, status);
}

int main() {
    // Establecer un manejador de señales para SIGCHLD
    signal(SIGCHLD, sigchld_handler);

    // Aquí iría tu código principal
    
    return 0;
}
