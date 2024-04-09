/////////////////////////EJERICIO 7//////////////////////////////////////////////////////////////
/*

Tres procesos (A,B,C) se comunican de la siguiente forma:
A y B se comunican por medio de PIPE
A y C se comunican por medio de un socket datagrama

1) El proceso A crea al proceso B
2) El proceso A crea un socket datagrama servidor
3) El proceso C (cliente) crea un socket datagrama cliente
4) El proceso C (cliente) cuando recibe la señal SIGTSTP (Ctrl+z) le envia al servidor el mensaje "ABCDEFGHIJ"
5) El proceso A (servidor) cuando recibe algo por el socket lo lee lo muestra por pantalla y se lo escribe en el PIPE
6) El proceso B al recibir  la señal SIGTSTP (Ctrl+z) lee el PIPE y muestra lo leido
7) El proceso A cuando recibe la señal SIGINT (Ctrl+c) cierra el pipe, cierra el socket y termina
8) El proceso C cuando recibe la señal SIGINT (Ctrl+c) cierra el socket y termina

       --- 		  ---		     ---
      | C |		 | A |		    | B |
       --- 		  ---		     --- 
	|     ------     | |     ------     | |     --------
	|--->|socket|--->| |--->| PIPE |--->| |--->|pantalla|
	      ------		  ------             --------

C ---> /_socket_/--> A --> /_PIPE_/ --> B --> /_pantalla_/

programa SOCKET CLIENTE datagramas (UDP): el cliente envia al socket lo que recibe por teclado
y muestra en pantalla lo recibido en el socket: Servidor:--> datos rx socket
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>    // Define pid_t
#include <unistd.h>       // Define fork, getpid y getppid
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h> 

#define MENSAJE "ABCDEFGHIJ"
char buff_tcl[256], buff_sck[56];
struct sockaddr_in direccion={};
int sockfd, lee_tcl, lee_sck, conn;

void escribe_mensaje(int a){ 

   write(STDOUT_FILENO, "\nProceso C escribe en socket\n", sizeof("\nProceso C escribe en socket\n"));
   sendto(sockfd, MENSAJE, strlen(MENSAJE)+1, 0, (struct sockaddr *) &direccion, sizeof(direccion));
   write(STDOUT_FILENO, "\nProceso C escrito en socket\n", sizeof("\nProceso C escrito en socket\n"));

}

void termina(int a){ 

   write(STDOUT_FILENO, "\nProceso C termino\n", sizeof("\nProceso C termino\n"));
   close(sockfd);
   exit(0);

}

int main(int argc , char const * argv[])
{


   if (argc != 2){
       argv[1]="2000";
   } 
   
   signal(SIGTSTP, escribe_mensaje);
   signal(SIGINT, termina);
   
   printf("Soy el Proceso D: %d \n ",getpid());

//****************** 1 *******************//
 //-- socket(): Crear el socket -----------//

   sockfd=socket(AF_INET, SOCK_DGRAM, 0);

   if (sockfd==-1) { 
       printf ("ERROR en funcion socket()\n");
   exit(-1);     } 

   printf ("Se creo socket cliente\n");

   
//****************** 2 *******************//


//-- preparar el address:port del host servidor------//
   direccion.sin_family= AF_INET;
   direccion.sin_port=htons(atoi(argv[1])); 
//   inet_aton("localhost", &direccion.sin_addr); //convierte la direccion de Internet a una cadena de caracteres en la notacion estandar, Localhost = 127.0.0.1
   inet_aton("127.0.0.1", &direccion.sin_addr); //convierte la direccion IP a una cadena de caracteres en la notacion estandar
                                                //como el servidor esta en el mismo host usamos Localhost = 127.0.0.1

   printf ("Cliente va a enviar datos a IP: 127.0.0.1 Puerto: %s\n",  argv[1]);

   while(1){}; 

   return 0;   
}
