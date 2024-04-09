/*
 * TP Procesos, Ejercicio 5 
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>    // Define pid_t
#include <unistd.h>       // Define fork, getpid y getppid

int main ()
{
	
	switch (fork())
	{
		case -1:   // No pudo crear el proceso
			printf ("Error. No se crea proceso hijo");
			break;

		case 0:   //proceso hijo
			printf ("Soy el proceso hijo, pid: %d", getpid());
			break;

		default:  //proceso padre
			printf ("Soy el proceso padre, pid: %d", getpid());
	}
    exit(0);

}
