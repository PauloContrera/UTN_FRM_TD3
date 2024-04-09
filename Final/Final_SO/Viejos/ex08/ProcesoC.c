/////////////////////////EJERICIO 8//////////////////////////////////////////////////////////////
/*
Tres procesos (A,B,C) No relacionados se comunican de la siguiente forma:
A y B se comunican por medio de una cola de mensajes
A y C se comunican por medio de un socket stream
1) El proceso A crea la cola de mensajes
2) El proceso A crea un socket stream servidor
3) El proceso C (cliente) crea un socket stream cliente 
4) El proceso C (cliente) cuando recibe la se al SIGTSTP (Ctrl+z) le env a al servidor el mensaje  "ABCDEFGHIJ"
5) El proceso B abre la cola de mensajes
6) El proceso A cuando recibe la señal SIGINT (Ctrl+c) cierra el socket y borra la cola de mensajes 
7) El proceso A (servidor) cuando recibe algo en por el socket lo lee lo muestra en pantalla y se lo env a al proceso B por medio de la cola de mensajes
8) Proceso B la cola de mensajes un mensaje y muestra lo leido
El proceso A debe ser el primero en ejecutarse. 

       ---              ---                ---  
      | C |            | A |              | B |
       ---              ---                ---
       |     ------     | |     ------     | |     --------  
       |--->|socket|--->| |--->|ColaMe|--->| |--->|pantalla|  
             ------             ------             --------

   c --> /_Socket_/--> A --> /_ColaMe_/ --> C --> /_Pantalla_/ 

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
