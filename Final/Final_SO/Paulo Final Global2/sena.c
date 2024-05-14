#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void signal_handler(int signum) {

        printf("Señal SIGUSR1 recibida\n");
    
}

int main() {
    // Obtener y mostrar el PID del proceso actual
    pid_t pid = getpid();
    printf("Mi PID es: %d\n", pid);

    // Configurar el manejador de señales para SIGUSR1
    signal(SIGUSR1, signal_handler);

    printf("Esperando la señal SIGUSR1 para imprimir un mensaje...\n");

    // Mantener el programa en ejecución
    while (1) {
        sleep(1); // Dormir para evitar el uso excesivo de la CPU
    }

    return 0;
}
