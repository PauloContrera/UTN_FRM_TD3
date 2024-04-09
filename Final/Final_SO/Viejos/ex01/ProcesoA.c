///////////////////////EJERCICIO 1 ///////////////////////////////////////////////////////////////
/*
Tres procesos (A,B,C) NO relacionados se comunican de la siguiente forma:
A y B se comunican por medio de un socket datagrama
A y C se comunican por medio de una cola de mensajes

1) El proceso B crea el socket datagrama servidor
2) El proceso A crea el socket datagrama cliente y la cola de mensajes
3) El proceso A cuando recibe la señal SIGUSR1 escribe en el socket "0123456789" 
4) El proceso A cuando recibe la señal SIGUSR2 escribe en la cola de mensaje el mensaje "ABCDEFGHIJ"
5) El proceso B lee el socket y lo recibido lo muestra en pantalla
6) El proceso B lee la cola de mensaje y lo recibido lo muestra en pantalla

El proceso B debe ser el primero en comenzar en una consola
El proceso A debe ser el segundo en comenzar en otra consola
El proceso C debe ser el tercero en comenzar en otra consola
Las señales se envían desde otra consola


 ---      ---------------       ---      --------  
|   |--->|Socket datagrma |--->| B |--->|Pantalla|  
| A |     ----------------      ---      --------
|   |     ----------------      ---      --------  
|   |--->|Cola de mensajes|--->| C |--->|Pantalla|  
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

#define MENSAJE1 "0123456789"
#define MENSAJE2 "ABCDEFGHIJ"
#define MQ_PATH "/MQ_TD3"
//cola de mensajes
int err, leido;
char buff[1024];   
mqd_t mqd; 		//descriptor
struct mq_attr attr, attr_rcv;
//socket
char buff_tcl[256], buff_sck[256];
struct sockaddr_in direccion={};
int sockfd, lee_tcl, lee_sck, conn;

void escribe_mensaje1(int a){ 

   write(STDOUT_FILENO, "\nenviar mensaje socket\n", sizeof("\nenviar mensaje socket\n"));

   //-- Escribe en socket --------------------------
  write ( sockfd , MENSAJE1, strlen(MENSAJE1)+1);

}

void escribe_mensaje2(int a){ 

   write(STDOUT_FILENO, "\nenviar mensaje cola\n", sizeof("\nenviar mensaje cola\n"));

   //-- Escribe en cola de mensajes --------------------------
   err = mq_send(mqd, MENSAJE2, strlen(MENSAJE2)+1, 1);  //strlen nos da la longitud de una cadena
   if(err == -1){
        write(STDOUT_FILENO, "\nerror en mq_send()\n", sizeof("\nerror en mq_send()\n"));  }
   else {
        write(STDOUT_FILENO, "\nMensaje2 enviado\n", sizeof("\nMensaje2 enviado\n"));   }

}


void finaliza_proceso(int a){ 

   err = mq_close(mqd);
   if (( err < 0 )){
      write(STDOUT_FILENO, "\nerror en mq_close()\n", sizeof("\nerror en mq_close()\n"));
      exit(0);   }

   write(STDOUT_FILENO, "\nCola de mensajes cerrada\n", sizeof("\nCola de mensajes cerrada\n"));

   err = mq_unlink(MQ_PATH);
   if(err == -1){
      write(STDOUT_FILENO, "\nerror en mq_unlink()\n", sizeof("\nerror en mq_unlink()\n"));
      exit(0);  }

   write(STDOUT_FILENO, "\nCola de mensajes eliminada\n", sizeof("\nCola de mensajes eliminada\n"));

   write(STDOUT_FILENO, "\nTerminando proceso...\n", sizeof("\nTerminando proceso...\n"));

   exit(0);
}

int main(int argc , char const * argv[])
{  

   signal(SIGUSR1, escribe_mensaje1);  //mensaje1
   signal(SIGUSR2, escribe_mensaje2);  //mensaje2
   signal(SIGINT, finaliza_proceso);   //termina

   if (argc != 2){
       argv[1]="2000";
   } 

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
   //cola de mensajes
   printf("Soy el proceso: %d \n ",getpid());

   mq_unlink(MQ_PATH);

   attr.mq_msgsize = sizeof(buff);
   attr.mq_maxmsg = 5;
   
   // abrir y crear cola de mensajes
    mqd = mq_open(MQ_PATH, O_RDWR | O_CREAT , 0666, &attr);
   if (mqd < 0) {
      printf ("error en mq_open()");   
      exit(-1);  }

   printf("Cola de mensajes creada\n");

 
  if (mq_getattr(mqd, &attr_rcv) == -1) {
      printf ("error en mq_getattr()");
      exit(-1); }
   printf("Nro max. de mensajes en cola de mensajes: %ld\n", attr_rcv.mq_maxmsg);
   printf("Longitud max. de mensaje: %ld\n", attr_rcv.mq_msgsize);
   printf("Nros de mensajes pendientes en cola de mensajes: %ld\n", attr_rcv.mq_curmsgs);
  //fin cola de mensajes
  
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

   return 0;   
}
