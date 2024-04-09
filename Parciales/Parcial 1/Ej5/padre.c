/*
 * PARCIAL 1: programa para ser ejecutado en el proceso padre.
 * 
 * Comando para compilar:
 * 
 * 	gcc -c padre.c && gcc hijo.o padre.o -o padre
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

#define MENSAJE "HOLA HIJA MIA"

int hijo (char * myfifo);

int main(void) 
{
	int fd_fifo;
	int pid;
	char tx_buffer[20] = MENSAJE;
    char * myfifo = "/tmp/myfifo";
    
    unlink(myfifo); 
    
    if (mkfifo(myfifo, 0666) == -1)
    {
        perror("fifo");
        return -1;
    }
    
    if ((pid = fork()) == -1)
    {
        perror("fork");
        return -1;
    }
    
// codigo que ejecuta el hijo
	if (pid==0) 
	{ 
        hijo(myfifo);

        return 0;
    }
// codigo que ejecuta el padre   
	else
	{
		printf("Proceso Padre en ejecucion... \n"); 
		
		sleep(1);
				
		// **** RESOLUCION DEL EJERCICIO *******************************
        fd_fifo = open(myfifo, O_WRONLY, 0); //!Abrimos la fifo
        write(fd_fifo, tx_buffer, sizeof(tx_buffer));
        close(fd_fifo); //!Cerramos la fifo
		// *************************************************************
		
		wait(NULL);
		return 0;
	}
}
