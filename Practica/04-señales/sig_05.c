#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#include <stdbool.h>  // Agregamos la librería correspondiente

#define TRUE true     // Definimos la constante TRUE

void manejador_senial(int a) {
    printf("La señal SIGUSR1 fue recibida. El proceso no puede ser detenido.\n");
}

int main() {
  /*
  ~No es posible matar el proceso porque en el manejador de señal para SIGTERM se establece que la señal debe ser ignorada. 
  ~Por lo tanto, cuando se recibe la señal SIGTERM desde otra consola, el manejador simplemente ignora la señal y no se produce ninguna acción adicional.
  */

    signal(SIGUSR1, manejador_senial);
    printf("Proceso PID = %d\n", getpid());
    while (TRUE);
    exit(0);
}
