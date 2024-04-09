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

//cola de mensajes
#define MQ_PATH "/MQ_TD3"
int err, leido;
char buff[1024];
mqd_t mqd; 		//descriptor
struct mq_attr attr, attr_rcv;

//socket
int SockEscucha, sck_server, rx_socket, largo, cont;
char buffer_rx[256];
struct sockaddr_in struct_direccion={};

void termina(int a){
   write (STDOUT_FILENO, "\nProceso A: terminando\n", sizeof("\nProceso A: terminando\n"));
   mq_close(mqd);  //cierra cola de mensajes
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
 //Cola de mensajes
 //CREACION COLA DE MENSAJES
   
   mq_unlink(MQ_PATH); //si existe la cola de mensajes, la borra

   attr.mq_msgsize = sizeof(buff);
   attr.mq_maxmsg = 5;
   
   // abrir y crear cola de mensajes
    mqd = mq_open(MQ_PATH, O_RDWR | O_CREAT , 0666, &attr); //O_WRONLY: solo escritura | O_CREAT: si no existe la crea la cola de mensajes
   if (mqd < 0) {
      printf ("error en mq_open()");   
      exit(-1);  }

   printf("\nCola de mensajes creada\n");

 
  /*if (mq_getattr(mqd, &attr_rcv) == -1) {		por si pide los atributos
      printf ("error en mq_getattr()");
      exit(-1); }
   printf("Nro max. de mensajes en cola de mensajes: %ld\n", attr_rcv.mq_maxmsg);
   printf("Longitud max. de mensaje: %ld\n", attr_rcv.mq_msgsize);
   printf("Nros de mensajes pendientes en cola de mensajes: %ld\n", attr_rcv.mq_curmsgs); */ 
   
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
     write(STDOUT_FILENO, "\nProceso A: escribiendo en cola de mensaje\n", sizeof("\nProceso A: escribiendo en cola de mensaje\n"));
     err = mq_send(mqd, buffer_rx, strlen(buffer_rx)+1, 1);  //strlen nos da la longitud de una cadena
   if(err == -1){
        write(STDOUT_FILENO, "\nerror en mq_send()\n", sizeof("\nerror en mq_send()\n"));  }
   else {
        write(STDOUT_FILENO, "\nProceso A: Mensaje enviado\n", sizeof("\nProceso A: Mensaje enviado\n"));   }
 }
 
  return 0;
}  
