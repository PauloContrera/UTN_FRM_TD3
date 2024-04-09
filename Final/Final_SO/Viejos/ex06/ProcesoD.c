/////////////////////////EJERICIO 5 //////////////////////////////////////////////////////////////
/*
Cuatro procesos (A,B,C,D) relacionados de la siguiente forma:
A padre de B y padre de C

Los procesos se comunican de la siguiente forma:
A y B se comunican por medio de un pipe
A y C se comunican por medio de un pipe
A y D se comunican por medio de una cola de mensajes

1) El proceso A crea la cola de mensajes, crea al proceso B y al proceso C 
2) El proceso B cuando recibe la señal SIGUSR1 escribe en el pipe1 el mensaje "0123456789" y termina.
3) El proceso C cuando recibe la señal SIGUSR2 escribe en el pipe2 el mensaje "ABCDEFGHIJ" y termina.
4) El proceso A espera a que termine el proceso B, lee el pipe1 y lo leído lo envía a la cola de mensajes 
5) El proceso A espera a que termine el proceso C, lee el pipe2 y lo leído lo envía a la cola de mensajes 
6) El proceso D lee los mensajes de la cola mensajes (2 mensajes) y los muestra en pantalla

El proceso A debe ser el primero en comenzar en una consola
El proceso D comienza luego de A otra consola
Las señales se envían desde otra consola


       --- 
      | B |
       --- 
       |     -------      ---      --------      ---      --------  
       |--->| PIPE1 |--->|   |    | Cola   |    |   |    |        |  
       |     -------     | A |--->|Mensajes|--->| D |--->|pantalla|
       |     -------     |   |--->|        |--->|   |--->|        | 
       |--->| PIPE2 |--->|   |     --------      ---      --------  
       |     -------      ---
       ---              
      | C |
       --- 
*/

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

   printf ("Proceso D: mi PID es %d\n", getpid());

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

