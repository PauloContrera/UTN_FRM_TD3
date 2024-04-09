/////////////////////////EJERICIO 17//////////////////////////////////////////////////////////////
/*
Tres procesos (A, B y C) se comunican por medio de 2 Tuberias.
1) El proceso C envia una señal (SIGUSR1) al proceso A. El proceso A al recibir esta señal escribe en una Tuberia "0123456789" y termina
2) El proceso C lee de la Tuberia despues que A termina, muestra lo leido y desturye la tuberia.
3) El proceso C envia una señal (SIGUSR2) al proceso B. El proceso B al recibir esta señal escribe en una Tuberia "ABCDEFGHIJ" y termina
4) El proceso C lee de la Tuberia despues que B termina, muestra lo leido y destruye la tuberia.

Considerar los procesos con la siguiente jerarquia: Proceso C (padre), Proceso B (hijo 1 de C), Proceso A (hijo 2 de C)
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>    // Define pid_t
#include <unistd.h>       // Define fork, getpid y getppid
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h> 

//compilar con: gcc -o ex17 ex17.c

#define MENSAJE1 "0123456789"
#define MENSAJE2 "ABCDEFGHIJ"
#define BUFF_SIZE 80 
int ipc[2],ipc2[2], proc;
int leido, leido2, success;
char buff[BUFF_SIZE] = {0}, buff2[BUFF_SIZE] = {0};

void escribe_A(int a){

	printf("Proceso A escribiendo en la tuberia 1\n");
	write(ipc[1], MENSAJE1, sizeof(MENSAJE1));	//escribe en tuberia 1
	printf("\n");
	//cerramos los extremos de la tuberia 1 para destruirla
      	close(ipc[0]);
      	close(ipc[1]);
      	exit(0);

}

void escribe_B(int a){

	printf("Proceso B escribiendo en la tuberia 2\n");

	write(ipc2[1], MENSAJE2, sizeof(MENSAJE1));	//escribe en tuberia 2
	printf("\n");
	//cerramos los extremos de la tuberia 2 para destruirla
      	close(ipc2[0]);
      	close(ipc2[1]);
      	exit(0);   

}

int main (){
	
	//creamos las tuberias
	pipe(ipc);
	pipe(ipc2);
	
	//señales
	signal(SIGUSR1, escribe_A);
   	signal(SIGUSR2, escribe_B);
   	
	pid_t pid;
	pid = fork();
	switch(pid) {
		case(0):
			while(1);
		break;
		
		case(-1):
			printf ("Proceso A: Error, no se creo el proceso B");
			return(-1);
		break;
		
		default:
   		
   			kill(pid, SIGUSR1); // Envio de señal al proceso A
   		
   			wait(NULL); //esperamos a que termine el proceso A
   			
   			//Leemos lo escrito por A en la tuberia 1
   			
   			printf("Proceso C: Leyendo de la tuberia 1 el mensaje del Proceso A:\n ");
		      	leido = read(ipc[0], buff, sizeof(buff)); //se lee de la tuberia 1
			if(leido < 1){
			      write (STDOUT_FILENO, "\nError al leer tuberia", sizeof("\nError al leer tuberia"));
			}else {
			      write (STDOUT_FILENO, buff, leido-1);
			      write (STDOUT_FILENO, "\n", sizeof("\n"));
			 }
			//destruimos la tuberia 1, para esto se cierran los extremos
			close(ipc[0]);
			close(ipc[1]);
   			//-----------------------------------------------//
   		
   			//ahora tenemos que comunicar el proceso B y C, primero lo tenemos que crear
   			pid_t pid;
   			pid = fork();
   			switch(pid) {
   				case(-1):
					printf ("Proceso C: Error, no se creo el proceso B");
					return(-1);
				break;
				
				case(0):
					while(1);
				break;
				
				default:
		   			
		   			kill(pid, SIGUSR2); // Envio de señal al proceso B
		   			
		   			wait(NULL); //esperamos a que termine el proceso B
		   		
		   			//Leemos lo escrito por B en la tuberia 2
   			
		   			printf("Proceso C: Leyendo de la tuberia 2 el mensaje del Proceso B:\n ");
				      	leido2 = read(ipc2[0], buff, sizeof(buff)); //se lee de la tuberia 2
					if(leido < 1){
					      write (STDOUT_FILENO, "\nError al leer tuberia", sizeof("\nError al leer tuberia"));
					}else {
					      write (STDOUT_FILENO, buff, leido2-1);
					      write (STDOUT_FILENO, "\n", sizeof("\n"));
					 }
					//destruimos la tuberia 2, para esto se cierran los extremos
					close(ipc2[0]);
					close(ipc2[1]);
					printf("\nProceso C: termino\n");
					
					exit(0);
                  		break;
   			}
   			
		break;
	}
}

