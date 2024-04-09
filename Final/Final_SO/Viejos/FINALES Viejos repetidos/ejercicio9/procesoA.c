/////////////////////////EJERICIO 9//////////////////////////////////////////////////////////////
/*
Tres procesos NO relacionados (A,B,C) se comunican de la siguiente forma:
A y B se comunican por medio de un FIFO
A y C se comunican por medio de un socket datagrama 
1) El proceso A crea un socket datagrama servidor
2) El proceso A crea y abre la FIFO con permisos de solo escritura 
3) El proceso B abre la FIFO con permisos de solo lectura 
4) El proceso C crea un socket datagrama cliente 
5) El proceso C esta en una espera activa y cuando recibe la señal SIGTSTP(Ctrl + z) le envía al servidor el mensaje "ABCDEFGHIJ"
6) El proceso A (servidor) cuando recibe algo en por el socket lo lee lo muestra en pantalla y lo escribe en la FIFO 
7) El Proceso B está en una espera activa y al recibir la señal SIGTSTP(Ctrl + z) leer la FIFO y muestra lo leído

El proceso A debe ser el primero en ejecutarse
El proceso B debe ejecutarse segundo
El proceso C debe ejecutarse tercero

       ---              ---                ---  
      | C |            | A |              | B |
       ---              ---                ---
       |     ------     | |     ------     | |     --------  
       |--->|socket|--->| |--->|FIFO  |--->| |--->|pantalla|  
             ------             ------              --------

   C --> /_socket_/--> A --> /_FIFO_/ --> C --> /_Pantalla_/ 
*/

// COMANDO PARA COMPILAR: gcc -o procesoA procesoA.c

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



// SOCKET
int SockEscucha, sck_server, rx_socket, largo, cont;
char buffer_rx[256];
struct sockaddr_in struct_direccion={};
// ---------------------------------

// FIFO
#define FIFO_PATH "/tmp/MI_FIFO"
int err, fifo_d;
// ---------------------------------



int main(int argc , char const * argv[])
{

   if (argc != 2){
       argv[1]="2000";
   }


// CREACION Y APERTURA DE LA FIFO

   //Eliminación FIFO
   if (unlink(FIFO_PATH) < 0) {
      printf("\nNo se puede borrar FIFO.\n");
   }     
   // FIFO puede ser leida, escrita y ejecutada por: 
   err = mkfifo(FIFO_PATH, 0777); // el resto de los usuarios del sistema.
   if(err == -1) {
      write(STDOUT_FILENO, "\nError al crear FIFO, la FIFO ya existe", sizeof("\nError al crear FIFO, la FIFO ya existe"));
   }else {
      write(STDOUT_FILENO, "\nFIFO creado correctamente\n", sizeof("\nFIFO creado correctamente\n"));
   }

   fifo_d = open(FIFO_PATH, O_WRONLY, 0); // abierto para solo escritura
   if(fifo_d == -1){
      write(STDOUT_FILENO, "\nError al abrir FIFO ", sizeof("\nError al abrir FIFO "));
      return -1;
   }else {
      write(STDOUT_FILENO, "\nProceso A: FIFO abierto correctamente", sizeof("\nProceso A: FIFO abierto correctamente"));
   }  

// ----------------------------------

// CREACION DEL SOCKET

 //****************** 1 *******************//
 //-- socket(): Crear el socket -----------//
   SockEscucha=socket(AF_INET, SOCK_DGRAM, 0);
   
   if(( SockEscucha)<0) {
       printf ("ERROR en funcion socket()\n");
   exit(-1);     } 
   
   printf ("Paso 1: creo socket servidor\n");

//----- preparar el address:port -------------//
   struct_direccion.sin_family = AF_INET;
   struct_direccion.sin_addr.s_addr = htonl(INADDR_ANY);  // asigna una IP de la maquina
   struct_direccion.sin_port = htons(atoi (argv[1]));     // port pasado por argumento

 //****************** 2 *******************//
 //-- bind(): asociamos el socket a la direccion ------//

   if (bind (SockEscucha, (struct sockaddr *)&struct_direccion,sizeof(struct sockaddr_in))<0) {
       printf ("ERROR en funcion bind()\n");
   exit(-1);     } 

   printf ("Paso 2: Asociar bind() \n");

// ----------------------------------

//-- servidor espera a recibir algo  y escribirlo por pantalla y en la FIFO --//
   while(1) {   

     int len = sizeof(struct_direccion);
     rx_socket = recvfrom(SockEscucha, buffer_rx, sizeof (buffer_rx), 0,(struct sockaddr*)&struct_direccion,&len); //recibe del socket 

     if (rx_socket>0) {
        write ( STDOUT_FILENO , "\nProceso A lee del cliente: ", sizeof("\nProceso A lee del cliente: "));   
        write ( STDOUT_FILENO , buffer_rx, rx_socket);      //escribe lo leido del socket por pantalla
        // Se escribe en el FIFO lo leido del socket
        err = write(fifo_d, buffer_rx, rx_socket);
        if(err == -1) {
            write(STDOUT_FILENO, "\nProceso A: Error al escribir en FIFO", sizeof("\nProceso A: Error al escribir en FIFO"));
         } else {
            write(STDOUT_FILENO, "\nProceso A: Escrito mensaje en FIFO", sizeof("\nProceso A: Escrito mensaje en FIFO"));
         }
     }

   } 

   return 0;   
}