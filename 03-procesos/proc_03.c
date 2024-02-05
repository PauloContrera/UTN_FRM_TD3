/*
 * TP Procesos, Ejercicio 3 
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>    // Define pid_t
#include <unistd.h>       // Define fork, getpid y getppid

int main (){
	// *Padre
	fork();
	// !Padre y el H1

	fork();
	fork();
	// !Padre, h1 + h2 h3(h1)
	printf ("Mi pid es %d y el pid de papa es %d\n", getpid(),getppid());
	sleep(300); // Probar con el comando pstree -p
	exit(0);

}
