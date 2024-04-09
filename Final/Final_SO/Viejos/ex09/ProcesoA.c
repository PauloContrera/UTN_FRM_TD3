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

programa SOCKET SERVIDOR de datagramas (UDP) el servidor envia al socket
lo que recibe del socket y muestra en pantalla lo recibido en 
el socket: Cliente:--> datos rx socket
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h> 
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

//fifo
#define FIFO_PATH "/tmp/MI_FIFO1"
int err, fifo_d;
int leido;  

//socket
int SockEscucha, sck_server, rx_socket, largo, cont;
char buffer_rx[256];
struct sockaddr_in struct_direccion={};

void termina(int a){
   write (STDOUT_FILENO, "\nProceso A: terminando\n", sizeof("\nProceso A: terminando\n"));
   close(fifo_d);  //cierra fifo
   close(SockEscucha);
   exit(0);
}

int main(int argc, const char *argv[])      {

   if (argc != 2){
       argv[1]="2000";
   }
   
   signal(SIGINT, termina);
   
   printf("Soy el Proceso A: %d \n ",getpid());
 //****************** 0 *******************//
 //FIFO
  
  //eliminamos la fifo por si ya existe
  if (unlink(FIFO_PATH) < 0) {
      printf("\nNo se puede borrar FIFO.\n");
   }
   // FIFO puede ser leida, escrita y ejecutada por: 
   err = mkfifo(FIFO_PATH, 0777); // el resto de los usuarios del sistema.
   write(STDOUT_FILENO, "\nFIFO lista\n", sizeof("\nFIFO lista\n"));

   fifo_d = open(FIFO_PATH, O_WRONLY, 0); //fifo solo de escritura 
   
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

//****************** 3 *******************//
//-- servidor espera a recibir algo ---
   while(1) {  //lee del socket    

     int len=sizeof(struct_direccion);
     rx_socket = recvfrom(SockEscucha, buffer_rx, sizeof (buffer_rx), 0,(struct sockaddr*)&struct_direccion,&len);            //recibe del socket 

     if (rx_socket>0) {
        write ( STDOUT_FILENO , "Proceso C dijo:--> ", 19);      //escribe leyenda en pantalla
        write ( STDOUT_FILENO , buffer_rx, rx_socket);      //escribe lo leido del socket
     }
     write(STDOUT_FILENO, "\nProceso A: escribiendo en fifo\n", sizeof("\nProceso A: escribiendo en fifo\n"));
     err = write(fifo_d, buffer_rx, sizeof(buffer_rx)); //se escribe el contenido del socket en FIFO
   if(err == -1){
        write(STDOUT_FILENO, "\nerror al escribir en fifo()\n", sizeof("\nerror al escribir en fifo()\n"));  }
   else {
        write(STDOUT_FILENO, "\nProceso A: Mensaje enviado\n", sizeof("\nProceso A: Mensaje enviado\n"));   }
 }
 
  return 0;
}  
