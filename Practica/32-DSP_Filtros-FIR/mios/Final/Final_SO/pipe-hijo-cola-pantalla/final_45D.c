
/////////// final_45 /////////////////////
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

//Compile el programa: gcc -o name name.c -lpthread -lrt
//Ejecute el programa ./name


#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <wait.h>
#include <arpa/inet.h> 


#define MQ_PATH "/MQ_TD3" 


//VARIABLES COLA DE MENSAJES

//Creo el descriptor de cola de mensajes
mqd_t messageQueue; 
int queueError;
struct mq_attr queueAttr;
char recieveBuffer[1024];  



int main() {
      
	
	
	printf ("-------------->Programa D<--------------\n");
	
	//***************************************************************************//		
	//**************************** Cola de Mensajes *****************************//	 
	//***************************************************************************//		
	
	// Se crea cola de mensajes
	messageQueue = mq_open(MQ_PATH, O_RDWR, 0777, NULL); 
	if (messageQueue < 0) {
		printf ("error en mq_open()");   
		exit(-1); }
	
	printf("Cola de mensajes creada o abierta\n"); 
	
	// Se leen parametros de la cola de mensajes
	if (mq_getattr(messageQueue, &queueAttr) == -1){
		printf ("error en mq_getattr()");
		exit(-1); }
	
	printf("Nro max. de mensajes en cola de mensajes: %ld\n",queueAttr.mq_maxmsg);
	printf("Longitud max. de mensaje: %ld\n",queueAttr.mq_msgsize);
	//printf("Nros de mensajes pendientes en cola de mensajes: %ld\n",queueAttr.mq_curmsgs);
	
	while(1)
	{
		// Se leen parametros de la cola de mensajes
		if (mq_getattr(messageQueue, &queueAttr) == -1){
			printf ("error en mq_getattr()");
			exit(-1); }
		
		printf("Nros de mensajes pendientes en cola de mensajes: %ld\n",queueAttr.mq_curmsgs);
		
		//Leo la cola de Mensajes
		queueError = mq_receive(messageQueue, recieveBuffer, queueAttr.mq_msgsize, 0);
		
		if (queueError < 0)
		{
			printf ("error en mq_receive()");
			exit(-1); 
		}
		
		//Imprimo por pantalla lo recibido
		printf("Mensaje leido de A: %s\n", recieveBuffer);
		
	}
	
  exit(0);
}

//------------------------------------------------------------//

