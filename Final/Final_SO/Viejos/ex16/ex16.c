/////////////////////////EJERICIO 16//////////////////////////////////////////////////////////////
/*
Tres procesos (A, B y C) se comunican por medio de 2 Tuberías.
1) El proceso A escribe en una Tubería "0123456789" y termina
2) El proceso B escribe en otra Tubería "ABCDEFGHIJ" y termina.
3) El proceso C lee de las Tuberías después que A y B terminaron y muestra lo leído.

Considerar los procesos con la siguiente jerarquía: Proceso C (padre), Proceso B (hijo 1 de C), Proceso A (hijo 2 de C)
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>

// Compilar usando gcc -o ex16 ex16.c

//PIPE
#define BUFF_SIZE 80
#define MENSAJE1 "0123456789"
#define MENSAJE2 "ABCDEFGHIJ"
int ipc[2],ipc2[2], proc;
int leido, leido2, success;
char buff[BUFF_SIZE] = {0}, buff2[BUFF_SIZE] = {0};

int main(int argc , char const * argv[])
{  
   printf("Soy el Proceso C: %d\n ",getpid());
   
   //Procesos
   pipe(ipc); //abrimos la tuberia que va a usar el proceso A
   switch (fork()){ 
      
      case 0:
      	close(ipc[0]);  //cierra lectura Proceso A
      	printf("Soy el Proceso A: mi PID es %d y voy a escribir en la tuberia 1\n ",getpid());
      	write(ipc[1], MENSAJE1, sizeof(MENSAJE1));	//escribe en tuberia 1
      	printf("Proceso A: mensaje enviado y ahora termino\n ");
      	exit(0);
      break;
      
      default:
        
        pipe(ipc2); //abrimos la tuberia que va a usar el proceso B
        switch (fork()){
           case 0:
      		close(ipc[0]);  //cierra lectura Proceso B
      		printf("Soy el Proceso B: mi PID es %d y voy a escribir en la tuberia 2\n ",getpid());
      		write(ipc2[1], MENSAJE2, sizeof(MENSAJE2));	//escribe en tuberia 2
      		printf("Proceso B: mensaje enviado y ahora termino\n ");
      		exit(0);
      	   break;
      	   
      	   default:
      	   
	      	wait(NULL); //Espera a que termine el proceso A
	        wait(NULL); //Espera a que termine el proceso B
	  	printf("Proceso C: Leyendo de la tuberia 1 el mensaje del Proceso A:\n ");
	      	leido = read(ipc[0], buff, sizeof(buff)); //se lee de la tuberia 1
		if(leido < 1){
		      write (STDOUT_FILENO, "\nError al leer tuberia", sizeof("\nError al leer tuberia"));
		}else {
		      write (STDOUT_FILENO, buff, leido-1);
		      write (STDOUT_FILENO, "\n", sizeof("\n"));
		 }
	      	printf("Proceso C: Leyendo de la tuberia 2 el mensaje del Proceso B:\n ");
	      	leido = read(ipc2[0], buff, sizeof(buff)); //se lee de la tuberia 2
		if(leido < 1){
		      write (STDOUT_FILENO, "\nError al leer tuberia", sizeof("\nError al leer tuberia"));
		}else {
		   write (STDOUT_FILENO, buff, leido-1);
		   write (STDOUT_FILENO, "\n", sizeof("\n"));
                 }
           break;
      	}
     	
      break;
  }
 }
