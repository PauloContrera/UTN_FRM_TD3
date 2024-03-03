/*
* Ejercicio 4 del TP de Cola de mensajes
* 
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
mqd_t mqd; 
struct mq_attr attr, attr_rcv;


int main() {

   
   attr.mq_msgsize = sizeof(buff);
   attr.mq_maxmsg = 5;

   mqd = mq_open(MQ_PATH, O_RDONLY, 0666, &attr);

   if (mqd < 0) {
      printf ("\nerror en mq_open()");      
      exit(-1) ;}

   printf("Cola de mensajes creada\n");
   
   mq_getattr(mqd, &attr_rcv);
      
      leido = mq_receive(mqd, buff, attr_rcv.mq_msgsize, 0);  
      if(leido == -1){
         printf ("\nerror en mq_receive()");
         exit(-1);}
 
      printf("\nMensaje recibido: %s",buff);
      


   exit(0);
}
