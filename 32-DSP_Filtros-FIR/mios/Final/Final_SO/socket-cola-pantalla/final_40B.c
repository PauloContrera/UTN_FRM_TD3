/////////// final_40 /////////////////////
/*
Tres procesos (A,B,C) No relacionados se comunican de la siguiente forma:
A y B se comunican por medio de una cola de mensajes
A y C se comunican por medio de un socket stream
1) El proceso A crea la cola de mensajes
2) El proceso A crea un socket stream servidor
3) El proceso C (cliente) crea un socket stream cliente 
4) El proceso C (cliente) cuando recibe la señal SIGTSTP (Ctrl+z) le envía al servidor el mensaje  "ABCDEFGHIJ"
5) El proceso B abre la cola de mensajes
6) El proceso A cuando recibe la señal SIGINT (Ctrl+c) cierra el socket y borra la cola de mensajes 
7) El proceso A (servidor) cuando recibe algo en por el socket lo lee lo muestra en pantalla y se lo envía al proceso B por medio de la cola de mensajes
8) Proceso B la cola de mensajes un mensaje y muestra lo leído
El proceso A debe ser el primero en ejecutarse. 

       ---              ---                ---  
      | C |            | A |              | B |
       ---              ---                ---
       |     ------     | |     ------     | |     --------  
       |--->|socket|--->| |--->|ColaMe|--->| |--->|pantalla|  
             ------             ------             --------

   c --> /_Socket_/--> A --> /_ColaMe_/ --> C --> /_Pantalla_/ 
 
*/



//Compile el programa: gcc -o name name.c -lpthread -lrt
//Ejecute el programa ./name

#define _GNU_SOURCE

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

	
	printf ("-------------->Programa B<--------------\n");
	
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
		
  return(0);
}


