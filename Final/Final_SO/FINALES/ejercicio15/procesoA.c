/////////////////////////EJERICIO 15//////////////////////////////////////////////////////////////
/*
Tres procesos (A, B y c) se comunican por medio de una Cola de Mensaje, del siguiente modo:
1) El Proceso C crea la cola de mensaje y crea al proceso B.
2) El Proceso B crea al proceso A.
3) Proceso A escribe en la Cola de Mensaje "0123456789" y termina.
4) Proceso B espera que A termine y lee de la Cola de Mensaje lo escrito por el proceso A y lo muestra 
5) Luego el Proceso B escribe en la Cola de Mensaje "ABCDEFGHIJ" y termina.
6) El Proceso C espera que B termine, lee la Cola de Mensaje,  muestra lo leido y elimina la cola de mensajes.

Considerar los procesos con la siguiente jerarquia: Proceso C (padre), Proceso B (hijo de C), Proceso A (hijo de B),

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
#define MENSAJE_A "0123456789"
#define MENSAJE_B "ABCDEFGHIJ"
#define MQ_PATH "/MQ_TD3" 

int err, leido;
char buff[1024];   
mqd_t mqd; 
struct mq_attr attr, attr_rcv;
//-----------------------------


int main (){

   // Definicion de parametros de la cola de mensajes
   attr.mq_msgsize = sizeof(buff);
   attr.mq_maxmsg = 5;
   //-----------------------------

   // Creacion de la cola de mensajes para solo lectura
   printf("Soy el proceso C\n");

   mqd = mq_open(MQ_PATH, O_RDONLY | O_CREAT, 0666, &attr); // fue abierto como lectura en vez de escritura
   if (mqd < 0) {
      printf ("Proceso C: error en mq_open()");      
      exit(-1) ;}

   printf("Proceso C: Cola de mensajes creada\n");
   //-----------------------------

   
	pid_t pid;
	pid = fork();		
	
	switch(pid)
	{
	  case -1: 
            // Cuando fork devuelve -1, hubo error
            printf("Error al crear el proceso hijo\n");
            return -1;
	  break;

	  case 0:
            // Comunicacion entre procesos B y C
            pid = fork();		
            switch(pid)
            {
            case -1: 
                  // Cuando fork devuelve -1, hubo error
                  printf("Error al crear el proceso hijo\n");
                  return -1;
            break;

            case 0:
                  printf ("Soy el proceso A\n");

                  // COLA DE MENSAJES
                  mqd = mq_open(MQ_PATH, O_WRONLY , 0666, &attr);
                  if (mqd < 0) {
                     printf ("Proceso A: error en mq_open()\n");      
                     exit(-1) ;}
                  printf("Proceso A: Cola de mensajes abierta como solo escritura\n");
                  
                  err = mq_send(mqd, MENSAJE_A, strlen(MENSAJE_A)+1, 0);  //strlen nos da la longitud de una cadena
                  if(err == -1){
                     printf ("Proceso A: error en mq_send()");
                     exit(-1);}

                  printf("Proceso A: Mensaje enviado \n");

                  exit(0);
            break;

            default: 
                  
                  wait(NULL); // Espera al proceso A
                  
                  printf("Soy el proceso B\n");

                  leido = mq_receive(mqd, buff, attr.mq_msgsize, 0);  
                  if(leido == -1){
                     printf ("error en mq_receive()\n");
                     exit(-1);
                  }
                  printf("Proceso B: mensaje recibido: %s\n",buff);
                  
                  // COLA DE MENSAJES
                  mqd = mq_open(MQ_PATH, O_WRONLY , 0666, &attr);
                  if (mqd < 0) {
                     printf ("Proceso B: error en mq_open()\n");      
                     exit(-1) ;}
                  printf("Proceso B: Cola de mensajes abierta como solo escritura\n");
                  
                  err = mq_send(mqd, MENSAJE_B, strlen(MENSAJE_B)+1, 0);  //strlen nos da la longitud de una cadena
                  if(err == -1){
                     printf ("Proceso B: error en mq_send()");
                     exit(-1);}
            
                  printf("Proceso B: Mensaje enviado \n");

                  exit(0);
            break;
            }
	  break;

	  default: 
            
            wait(NULL); // Espera al proceso B
            
            leido = mq_receive(mqd, buff, attr.mq_msgsize, 0);  
            if(leido == -1){
               printf ("error en mq_receive()\n");
               exit(-1);
            }
            printf("Proceso C: mensaje recibido del proceso B: %s\n",buff);
            
            printf("Proceso C: borrando cola de mensajes\n");
            mq_unlink(MQ_PATH); // Se borra la cola
            
            exit(0);
	  break;
	}
	exit(0);
}