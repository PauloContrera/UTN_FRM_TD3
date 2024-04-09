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
mqd_t mqd; 
struct mq_attr attr, attr_rcv;


int main() {
   

   printf ("Soy el proceso C, mi pid es %d\n", getpid());
   
    attr_rcv.mq_msgsize = sizeof(buff);
    attr_rcv.mq_maxmsg = 5;


   mqd = mq_open(MQ_PATH, O_RDONLY , 0666, &attr_rcv);
   if (mqd < 0) {
      printf ("error en mq_open()\n");      
      exit(-1) ;}

   printf("Cola de mensajes abierta como solo lectura\n");
   
   //mq_getattr(mqd, &attr_rcv); // No se ejecuta mq_getattr, ya que los atributos de attr_rcv se definieron al principio.
   
   
   while(1) {
      
      leido = mq_receive(mqd, buff, attr_rcv.mq_msgsize, 0);  
      if(leido == -1){
         printf ("error en mq_receive()\n");
         exit(-1);}
 
      printf("\nMensaje recibido: %s",buff);
     
     // sleep(1);
 
   }

   exit(0);
}
