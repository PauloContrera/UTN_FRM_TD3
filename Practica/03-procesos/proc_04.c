/*
 * TP Procesos, Ejercicio 2 
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>    // Define pid_t
#include <unistd.h>       // Define fork, getpid y getppid

int main (){

	pid_t pid;
	int i;

	pid = fork();

 // ~ Identifica al padre dado que es el primer proceso, pero el pid de fork se le asigna al hijo
/*
if(pid-1 == getpid()){
	printf("\nPadre:");
	printf("Yo soy tu padre");
}else{
	printf("\nHijo:");	
	printf("Noooooooooo");
}
*/



	// printf ("Mi pid es %d y el pid de papa es %d. fork() devolvio %d\n", getpid(), getppid(), pid);

	// Ejecute pstree en otra consola	

 // ~ Ejecuta 5 hijos con 5 hijos dentro, el siguiente con 4 y asi sucesivamente hasta que llegar a padre - hijo - hijo 
	/*
	 for(int i=0; i<5; i++){
	 	pid = fork();
	 printf ("Mi pid es %d y el pid de papa es %d. fork() devolvio %d\n", getpid(), getppid(), pid);
	 }
	*/


	// sleep(30); 

	exit(0);

}
