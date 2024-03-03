/////////////////////////EJERICIO 12//////////////////////////////////////////////////////////////
/*
Dos procesos (A y B) se comunican por medio de una Cola de Mensaje, del siguiente modo:
1) El proceso A crea la cola de mensaje y crea al proceso B.
2) Proceso B al recibir la señal SIGINT escribe en la Cola de Mensaje "0123456789" y termina.
3) Proceso A espera que B termine y lee de la Cola de Mensaje lo escrito por el proceso B y muestra lo leído y elimina la cola de mensajes.

Considerar los procesos con la siguiente jerarquía: Proceso A (padre), Proceso B (hijo de A).
*/

// Comando: gcc -o procesoA procesoA.c -lrt

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>    // Define pid_t
#include <unistd.h>       // Define fork, getpid y getppid
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h> 



// COLA DE MENSAJES
#define MENSAJE "0123456789"
#define MQ_PATH "/MQ_TD3" 

int err, leido;
char buff[1024];   
mqd_t mqd; 
struct mq_attr attr, attr_rcv;
//-----------------------------

void signal1(){
         
         err = mq_send(mqd, MENSAJE, strlen(MENSAJE)+1, 0);  //strlen nos da la longitud de una cadena
         if(err == -1){
            printf ("Proceso B: error en mq_send()");
            exit(-1);}
   
         printf("Proceso B: Mensaje enviado \n");

         exit(0);
}

int main (){

   

	pid_t pid;
	
	pid = fork();		
	
	switch(pid)
	{
	  case -1: // Cuando fork devuelve -1, hubo error
	  printf("Error al crear el proceso hijo\n");
	  return -1;
	  break;
	  case 0: 
	   printf ("Soy el proceso B mi pid es %d\n", getpid());
      // SIGNAL
      signal(SIGINT, signal1);
      //-----------------------------

      // COLA DE MENSAJES
      mqd = mq_open(MQ_PATH, O_WRONLY , 0666, &attr_rcv);
      if (mqd < 0) {
         printf ("Proceso B: error en mq_open()\n");      
         exit(-1) ;}

      printf("Proceso B: Cola de mensajes abierta como solo escritura\n");
      
      //-----------------------------

	   while(1);

	  break;
	  default: 

	   printf("Soy el proceso A\n");

      attr.mq_msgsize = sizeof(buff);
      attr.mq_maxmsg = 5;

      mqd = mq_open(MQ_PATH, O_RDONLY | O_CREAT, 0666, &attr); // fue abierto como lectura en vez de escritura
      if (mqd < 0) {
         printf ("Proceso B: error en mq_open()");      
         exit(-1) ;}

      printf("Proceso A: Cola de mensajes creada\n");
      
      wait(NULL);
      
      leido = mq_receive(mqd, buff, attr.mq_msgsize, 0);  
      if(leido == -1){
         printf ("error en mq_receive()\n");
         exit(-1);
      }

      printf("Proceso A: mensaje recibido: %s\n",buff);
      
      mq_unlink(MQ_PATH); // Se borra la cola

      exit(0);

	  break;
	}
	exit(0);
}