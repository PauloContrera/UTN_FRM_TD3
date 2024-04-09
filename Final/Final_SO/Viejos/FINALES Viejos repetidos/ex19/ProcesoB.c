/////////////////////////EJERICIO 19//////////////////////////////////////////////////////////////

/*
Tres procesos NO relacionados (A,B,C) se comunican de la siguiente forma:
A y B se comunican por medio de una cola de mensaje
A y C se comunican por medio de un socket datagrama 
1) El proceso A crea un socket datagrama servidor
2) El proceso A crea y abre la cola de mensajes con permisos de solo escritura 
3) El proceso B abre la cola de mensajes con permisos de solo lectura 
4) El proceso C crea un socket datagrama cliente 
5) El proceso C esta en una espera activa y cuando recibe la se�al SIGUSR1 le env�a al servidor el mensaje  "ABCDEFGHIJ"
6) El proceso A (servidor) cuando recibe algo en por el socket lo lee lo muestra en pantalla y lo escribe en la cola de mensajes
7) El Proceso B est� en una espera activa y al recibir la se�al SIGUSR1 leer la cola de mensajesy muestra lo le�do

El proceso A debe ejecutarse primero
El proceso B debe ejecutarse segundo
El proceso C debe ejecutarse tercero
Las se�ales se env�an desde otra consola

       ---              ---                  ---  
   S->| C |            | A |             S->| B |
       ---              ---                  ---
       |     ------     | |     --------     | |     --------  
       |--->|socket|--->| |--->|COLA MSJ|--->| |--->|pantalla|  
             ------             --------             --------

*/

//Compile el programa: gcc -o ProcesoB ProcesoB.c -lpthread -lrt

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>
#include <signal.h>

#define MQ_PATH "/MQ_TD3" 

int err, leido;
char buff[1024];   
mqd_t mqd; 		//descriptor
struct mq_attr attr, attr_rcv;

int main() {

   printf ("Proceso B: mi PID es %d\n", getpid());

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
   write(STDOUT_FILENO, "\nMensaje leido: ", sizeof("\nMensaje leido: "));
   write(STDOUT_FILENO, buff, leido-1);
   write(STDOUT_FILENO, "\n", sizeof("\n"));
   sleep(1);

   }


   exit(0);
}

