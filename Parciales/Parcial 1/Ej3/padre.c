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

int hijo (void);

int main(void) 
{
	int pid;
    
    if ((pid = fork()) == -1)
    {
        perror("fork");
        return -1;
    }
    
// codigo que ejecuta el hijo
	if (pid==0) 
	{ 
        hijo();

        return 0;
    }
// codigo que ejecuta el padre   
	else
	{
		printf("Proceso Padre en ejecucion... \n"); 
		
		sleep(1);
				
		// **** RESOLUCION DEL EJERCICIO *******************************
		3
    	// signal(SIGUSR1, 0);

		// *************************************************************
		
		wait(NULL);	
		return 0;
	}
}
