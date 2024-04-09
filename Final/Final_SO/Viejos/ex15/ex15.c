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

//compilar con: gcc -o ex15 ex15.c -lrt

#define MENSAJE1 "0123456789"
#define MENSAJE2 "ABCDEFGHIJ"
#define MQ_PATH "/MQ_TD3" 
int err, leido;
char buff[1024];   
mqd_t mqd; 		//descriptor
struct mq_attr attr, attr_rcv;
int a,estado;

int main (){
	
   // Definicion de parametros de la cola de mensajes
   attr.mq_msgsize = sizeof(buff); //tamaño maximo de mensaje (en bytes)
   attr.mq_maxmsg = 5; //numero maximo de mensajes en la cola
   printf ("Proceso C, mi PID es %d\n", getpid());

   mqd = mq_open(MQ_PATH, O_RDONLY | O_CREAT, 0666, &attr); //creacion de la cola en modo solo lectura
   if (mqd < 0) {
   	printf ("Proceso C: error en mq_open()");      
   	exit(-1) ;}

   printf("Proceso C: Cola de mensajes creada\n");
   pid_t pid;
   pid = fork();
   switch(pid) {
   
	case(-1):
		printf ("Proceso C: Error, no se creo el proceso B");
		return(-1);
	break;
	
	case(0):
		//comunicamos el proceso B con el C
		pid_t pid;
   		pid = fork();
   		switch(pid) {
   		
   			case(-1):
				printf ("Proceso C: Error, no se creo el proceso A");
				return(-1);
			break;
			
			case(0):
				printf ("Proceso A, mi PID es %d\n", getpid());
				//abrimos la cola de mensajes como solo escritura
				printf("\nProceso A: Cola de mensajes abierta\n");
   				mqd = mq_open(MQ_PATH, O_WRONLY | O_CREAT, 0666, &attr); //creacion de la cola en modo solo escritura
			        if (mqd < 0) {
			      		printf ("Proceso A: error en mq_open()");      
			      		exit(-1) ;}
			   	err = mq_send(mqd, MENSAJE1, strlen(MENSAJE1)+1, 1);  //strlen nos da la longitud de una cadena
			   	if(err == -1){
				 	printf ("Proceso A: error en mq_send()");
				 	exit(-1);}
			 
			   	printf("\nProceso A: Mensaje enviado\n");
			   	exit(0);
			break;
			
			default:
				printf ("Proceso B, mi PID es %d\n", getpid());
				wait(NULL); //espera a que el proceso A termine
				// Se lee de cola de mensajes para ver el mensaje de A
	   			leido = mq_receive(mqd, buff, attr.mq_msgsize, 0);  
				if(leido == -1){
			 	printf ("error en mq_receive()\n");
			 	exit(-1);
				}
	   			printf("\nProceso B: Mensaje leido del Proceso A: %s\n",buff);
	   			//abrimos la cola de mensajes como solo escritura
	   			printf("\nProceso B: Cola de mensajes abierta\n");
				mqd = mq_open(MQ_PATH, O_WRONLY | O_CREAT, 0666, &attr); //creacion de la cola en modo solo escritura
				if (mqd < 0) {
				      	printf ("Proceso B: error en mq_open()");      
				      	exit(-1) ;}
				err = mq_send(mqd, MENSAJE2, strlen(MENSAJE2)+1, 1);  //strlen nos da la longitud de una cadena
				if(err == -1){
					 printf ("Proceso B: error en mq_send()");
					 exit(-1);}
				 
				printf("\nProceso B: Mensaje enviado\n");
				exit(0);
			break;
		} //fin del primer switch
	break;
	
	default:
		wait(NULL); //Proceso C espera a que el proceso B termine
		// Se lee de cola de mensajes para ver el mensaje de B
		leido = mq_receive(mqd, buff, attr.mq_msgsize, 0);  
		if(leido == -1){
	 	printf ("error en mq_receive()\n");
	 	exit(-1);
		}
		printf("\nProceso C: Mensaje leido del Proceso B: %s\n",buff);
		printf("Proceso C: borrando cola de mensajes\n");
		mq_unlink(MQ_PATH); // Se borra la cola
		exit(0);
	break;
	
   }	
   exit(0);
   
}

