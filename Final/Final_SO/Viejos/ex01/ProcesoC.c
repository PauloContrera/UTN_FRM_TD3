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
6) El proceso c lee la cola de mensaje y lo recibido lo muestra en pantalla

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
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>
#include <signal.h>

#define MENSAJE "DATA PARA PROCESO"
#define MQ_PATH "/MQ_TD3" 

int err, leido;
char buff[1024];   
mqd_t mqd; 		//descriptor
struct mq_attr attr, attr_rcv;

int main() {

   printf ("Mi pid es %d\n", getpid());

   mqd = mq_open(MQ_PATH, O_RDWR | O_CREAT, 0777, &attr); 
   if (mqd < 0) {
      printf ("error en mq_open()");   
      exit(-1); }
   
         // Se leen parametros de la cola de mensajes por unica vez
   if (mq_getattr(mqd, &attr_rcv) == -1){
      printf ("error en mq_getattr()");
      exit(-1); }
      
   printf("Nro max. de mensajes en cola de mensajes: %ld\n",attr_rcv.mq_maxmsg);
   printf("Longitud max. de mensaje: %ld\n",attr_rcv.mq_msgsize);
   printf("Nros de mensajes pendientes en cola de mensajes: %ld\n",attr_rcv.mq_curmsgs);
   
   while(1) {

   // Se lee de cola de mensajes
   leido = mq_receive(mqd, buff, attr_rcv.mq_msgsize, 0);
   if (( leido < 0 )){
      printf ("error en mq_receive()");
      exit(-1); }
   printf("Mensaje leido: %s\n", buff);
   //printf("\nMensaje leido: ");
   //write(STDOUT_FILENO, buff, leido-1);
   //printf("\n");
   sleep(1);

   }


   exit(0);
}

