/////////////////////////EJERICIO 12//////////////////////////////////////////////////////////////
/*
Dos procesos (A y B) se comunican por medio de una Cola de Mensaje, del siguiente modo:
1) El proceso A crea la cola de mensaje y crea al proceso B.
2) Proceso B al recibir la señal SIGINT escribe en la Cola de Mensaje "0123456789" y termina.
3) Proceso A espera que B termine y lee de la Cola de Mensaje lo escrito por el proceso B y muestra lo leído y elimina la cola de mensajes.

Considerar los procesos con la siguiente jerarquía: Proceso A (padre), Proceso B (hijo de A).
*/
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

//compilar con: gcc -o ex12 ex12.c -lrt

#define MENSAJE "0123456789"
#define MQ_PATH "/MQ_TD3" 
int err, leido;
char buff[1024];   
mqd_t mqd; 		//descriptor
struct mq_attr attr, attr_rcv;
int a,estado;

void sig_user1(int a){
   
   err = mq_send(mqd, MENSAJE, strlen(MENSAJE)+1, 1);  //strlen nos da la longitud de una cadena
      if(err == -1){
         printf ("Proceso B: error en mq_send()");
         exit(-1);}
 
      printf("Proceso B: Mensaje enviado\n");
      exit(0);

}

int main (){
	
	pid_t pid;
	pid = fork();
	switch(pid) {
		case(0):
		printf ("Proceso B, mi PID es %d\n", getpid());
		signal(SIGUSR1, sig_user1);
		mqd = mq_open(MQ_PATH, O_WRONLY | O_CREAT, 0666, &attr); //creacion de la cola en modo solo escritura
   		if (mqd < 0) {
      		printf ("Proceso B: error en mq_open()");      
      		exit(-1) ;}

   		printf("Proceso B: Cola de mensajes creada\n");
   		while(1);
		break;
		
		case(-1):
		printf ("Proceso A: Error, no se creo el proceso B");
		return(-1);
		break;
		
		default:
		printf ("Proceso A, mi PID es %d\n", getpid());
		attr.mq_msgsize = sizeof(buff); //tamaño maximo de mensaje (en bytes)
   		attr.mq_maxmsg = 5; //numero maximo de mensajes en la cola

   		mqd = mq_open(MQ_PATH, O_RDONLY | O_CREAT, 0666, &attr); //creacion de la cola en modo solo lectura
   		if (mqd < 0) {
      		printf ("Proceso A: error en mq_open()");      
      		exit(-1) ;}

   		printf("Proceso A: Cola de mensajes creada\n");
   		
   		wait(NULL);
   		
   		// Se lee de cola de mensajes
   		leido = mq_receive(mqd, buff, attr.mq_msgsize, 0);  
		if(leido == -1){
		 printf ("error en mq_receive()\n");
		 exit(-1);
		}
   		printf("\nProceso A: Mensaje leido: %s",buff);
   		mq_unlink(MQ_PATH); // Se borra la cola
   		printf("\n");
   		
   		exit(0);
		break;
	}

}

