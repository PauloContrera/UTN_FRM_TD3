
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

#define MENSAJE_1 "0123456789"
#define MENSAJE_2 "ABCDEFGHIJ"

#define MQ_PATH "/MQ_TD3" 


//VARIABLES COLA DE MENSAJES

//Creo el descriptor de cola de mensajes
mqd_t messageQueue; 
int queueError;
struct mq_attr queueAttr;
char recieveBuffer[1024];  

//VARIABLES IPC PIPE
int ipcPipe[2];
int recievedChars = 0;
char recieveBufferPipe[50] = {0};  


//Hijo B
void mnjs1(int a){
	
	//Escribe en el PIPE
	write(ipcPipe[1], MENSAJE_1, sizeof(MENSAJE_1));
	printf ("Mensaje de Hijo B a Padre A: %s\n", MENSAJE_1);
	
	//Termina con 0
	exit(0);
}

//Hijo C
void mnjs2(int b){

	//Escribe en el PIPE
	write(ipcPipe[1], MENSAJE_2, sizeof(MENSAJE_2));
	printf ("Mensaje de Hijo C a Padre A: %s\n", MENSAJE_2);
	
	//Termina con 0
	exit(0);
   
}

	  
int main(int argc, const char *argv[]) {

	printf ("-------------->Programa A<--------------\n");
	
//***************************************************************************//		
//*********************************** IPC ***********************************//	 
//***************************************************************************//		
	
	if (pipe(ipcPipe) == -1)
	{
		printf ("Error. No se crea Pipe");
		exit(-1);
	}
	
//***************************************************************************//		
//**************************** Cola de Mensajes *****************************//	 
//***************************************************************************//		

// Se crea cola de mensajes
messageQueue = mq_open(MQ_PATH, O_RDWR | O_CREAT | O_NONBLOCK, 0777, NULL); 
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
printf("Nros de mensajes pendientes en cola de mensajes: %ld\n",queueAttr.mq_curmsgs);
	
//***************************************************************************//		
//********************************** FORK ***********************************//	 
//***************************************************************************//		
	
	switch (fork())
	{
		case -1:   // No pudo crear el proceso
			printf ("Error. No se crea proceso hijo B");
		break;
			
		case 0:   //proceso hijo
			
			printf ("Soy el proceso hijo B, pid %d\n", getpid());
			printf ("Espero señal SIGUSR1\n");
			
			//Si recibo la señal SIGUSR1 voy al manejador mnjs1
			signal(SIGUSR1, mnjs1);
			
			//Cierra lectura lado hijo
			close(ipcPipe[0]);
			printf ("Cierro lectura PIPE hijo B\n");
			
			while(1);

		break;
			
		default:  //proceso padre
		break;
	}
	
	switch (fork())
	{
		case -1:   // No pudo crear el proceso
			printf ("Error. No se crea proceso hijo C");
			break;
			
		case 0:   //proceso hijo
			
			printf ("Soy el proceso hijo C, pid %d\n", getpid());
			printf ("Espero señal SIGUSR2\n");
			
			//Si recibo la señal SIGUSR2 voy al manejador mnjs2
			signal(SIGUSR2, mnjs2);
			
			//Cierra lectura lado hijo
			close(ipcPipe[0]);
			printf ("Cierro lectura PIPE hijo C\n");
			
			while(1);

		break;
			
		default:  //proceso padre
		break;
	}
	
	//Cierra escritura lado padre
	close(ipcPipe[1]);
	
	int recievedChars = 0;
	char recieveBufferPipe[50] = {0};  
	
	
	printf("Leyendo PIPE... \n");
	wait(NULL);					//Espera que termine el hijo
	
	//Espera lectura de la tuberia
	recievedChars = read(ipcPipe[0], recieveBufferPipe, sizeof(recieveBufferPipe));
	
	if(recievedChars < 1)
	{	//Si falla
		printf("\nError al leer tuberia");
		
	}
	else
	{//Si esta OK imprime el contenido de la tuberia y la manda a D por Cola de Mensajes
		
		printf("Padre lee de la tuberia: %s\n",recieveBufferPipe);
		
		//Envio mensaje por cola de mensajes a D
		// Escribe en cola de mensajes
		queueError = mq_send(messageQueue, recieveBufferPipe, recievedChars, 1); 
		if(queueError == -1){
			printf ("error en mq_send()");
			exit(-1);    }
		
		printf("Mensaje enviado por Cola de Mensaje a D: %s (%d)\n", recieveBufferPipe, queueError); 
	}

	printf("Leyendo PIPE... \n");
	wait(NULL);					//Espera que termine el hijo
	
	//Espera lectura de la tuberia
	recievedChars = read(ipcPipe[0], recieveBufferPipe, sizeof(recieveBufferPipe));
	
	if(recievedChars < 1)
	{	//Si falla
		printf("\nError al leer tuberia");
		
	}
	else
	{//Si esta OK imprime el contenido de la tuberia y la manda a D por Cola de Mensajes
		
		printf("Padre lee de la tuberia: %s\n",recieveBufferPipe);
		
		//Envio mensaje por cola de mensajes a D
		// Escribe en cola de mensajes
		queueError = mq_send(messageQueue, recieveBufferPipe, recievedChars, 1); 
		if(queueError == -1){
			printf ("error en mq_send()");
			exit(-1);    }
		
		printf("Mensaje enviado por Cola de Mensaje a D: %s (%d)\n", recieveBufferPipe, queueError); 
	}
	
  exit(0);
}










