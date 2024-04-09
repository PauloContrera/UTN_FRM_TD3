/////////////////////// EJERCICIO 2 ///////////////////////////////////////////////////////////////
/*
Tres procesos (A,B,C) NO relacionados se comunican de la siguiente forma:
A y B se comunican por medio de un socket datagrama
A y C se comunican por medio de una FIFO

1) El proceso B crea el socket datagrama servidor
2) El proceso A crea el socket datagrama cliente y la FIFO
3) El proceso A cuando recibe la señal SIGUSR1 escribe en el socket "0123456789" 
4) El proceso A cuando recibe la señal SIGUSR2 escribe en la FIFO "ABCDEFGHIJ"
5) El proceso B lee el socket y lo recibido lo muestra en pantalla
6) El proceso C lee la FIFO y lo recibido lo muestra en pantalla

El proceso B debe ser el primero en comenzar en una consola
El proceso A debe ser el segundo en comenzar en otra consola
El proceso C debe ser el tercero en comenzar en otra consola
Las señales se envían desde otra consola


 ---      ---------------       ---      --------  
|   |--->|Socket datagrma |--->| B |--->|Pantalla|  
| A |     ----------------      ---      --------
|   |     ----------------      ---      --------  
|   |--->|     FIFO	    |--->| C |--->|Pantalla|  
 ---      ----------------      ---      --------
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

#define MENSAJE1 "0123456789"
#define MENSAJE2 "ABCDEFGHIJ"
#define FIFO_PATH "/tmp/MI_FIFO1"
//fifo
int err, fifo_d;
char buff[80];
int leido;  
//socket
char buff_tcl[256], buff_sck[256];
struct sockaddr_in direccion={};
int sockfd, lee_tcl, lee_sck, conn;

void escribe_mensaje1(int a){ 

   write(STDOUT_FILENO, "\nenviar mensaje socket\n", sizeof("\nenviar mensaje socket\n"));

   //-- Escribe en socket--------------------------
  write ( sockfd , MENSAJE1, strlen(MENSAJE1)+1);
}

void escribe_mensaje2(int a){ 

   write(STDOUT_FILENO, "\nenviar mensaje FIFO\n", sizeof("\nenviar mensaje FIFO\n"));

      // Se escribe en el FIFO
   err = write(fifo_d, MENSAJE2, sizeof(MENSAJE2));
   write(STDOUT_FILENO, "\nProceso A: escrito en FIFO\n", sizeof("\nProceso A: escrito en FIFO\n"));
   write(STDOUT_FILENO, MENSAJE2, sizeof(MENSAJE2));

}


int main(int argc , char const * argv[])
{  

   signal(SIGUSR1, escribe_mensaje1);  //mensaje1
   signal(SIGUSR2, escribe_mensaje2);  //mensaje2

   if (argc != 2){
       argv[1]="2000";
   }
   
   printf("Soy el proceso: %d \n ",getpid()); 

//****************** 1 *******************//
 //-- socket(): Crear el socket -----------//

   sockfd=socket(AF_INET, SOCK_STREAM, 0);

   if (sockfd==-1) { 
       printf ("ERROR en funcion socket()\n");
   exit(-1);     } 

   printf ("Paso 1: Se creo socket cliente\n");

//****************** 2 *******************//

//-- preparar el address:port del host servidor------//
   direccion.sin_family= AF_INET;
   direccion.sin_port=htons(atoi(argv[1])); 
//   inet_aton("localhost", &direccion.sin_addr); //convierte la direccion de Internet a una cadena de caracteres en la notacion estandar, Localhost = 127.0.0.1
   inet_aton("127.0.0.1", &direccion.sin_addr); //convierte la direccion IP a una cadena de caracteres en la notacion estandar
                                                //como el servidor esta en el mismo host usamos Localhost = 127.0.0.1
                                                                                                
   printf ("Cliente va a conectarse con IP:127.0.0.1 Puerto: %s\n",  argv[1]);

//-- conectar el socket activo al socket de escucha --//
   conn=connect(sockfd, (struct sockaddr *)&direccion, sizeof (direccion));

   if (conn == -1) {   
        printf ("ERROR en funcion connect()\n");
   exit(-1);     }  

   printf ("Paso 2: Connect(), cliente conectado\n");

//****************** 3 *******************//
   //FIFO
   // FIFO puede ser leida, escrita y ejecutada por: 
   err = mkfifo(FIFO_PATH, 0777); // el resto de los usuarios del sistema.
   write(STDOUT_FILENO, "\nFIFO lista", sizeof("\nFIFO lista"));

   fifo_d = open(FIFO_PATH, O_WRONLY, 0);
  //fin FIFO
  
  while(1){ 
   
     lee_sck = read ( sockfd , buff_sck, sizeof (buff_tcl));    //lee de socket 
     write (STDOUT_FILENO, "Servidor:--> ", 13);               //escribe leyenda en pantalla
     write (STDOUT_FILENO, buff_sck, lee_sck);                  //escribe lo leido del socket

     if (( lee_tcl = read(STDIN_FILENO, buff_tcl, sizeof (buff_tcl))) > 0) {     // lee de teclado
        write ( sockfd , buff_tcl, lee_tcl);                   //escribe en socket
   }
 }
//****************** 4 *******************//
//------------cierrar la conexion --------// 

   close(sockfd);
   close(fifo_d);

   return 0;   
}
