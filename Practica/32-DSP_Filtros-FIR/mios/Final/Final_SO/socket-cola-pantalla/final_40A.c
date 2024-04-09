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

//VARIABLES SOCKET

//Creo el descriptor de socket
int SocketServidor;
//Creo estructura direccion
struct sockaddr_in direccion = {};
//Variable auxiliares
int connectedSocket;
int connections;
int recievedChars = 0;
char bufferSocket[50];

//VARIABLES COLA DE MENSAJES

//Creo el descriptor de cola de mensajes
mqd_t messageQueue; 
int queueError;
struct mq_attr queueAttr;


void mnjd1(int a){

//**************************** Cola de Mensajes *****************************//
	// Se cierra cola de mensajes
	queueError = mq_close(messageQueue);
	if (( queueError < 0 )){
		printf ("error en mq_close()");
		exit(-1);   }
	
	printf("Cola de mensajes cerrada (%d)\n", queueError);
	
	// Se elimina cola de mensajes
	queueError = mq_unlink(MQ_PATH);
	
	if(queueError == -1){
		printf ("error en mq_unlink()");
		exit(-1);   }
	
	printf("Cola de mensajes eliminada (%d)\n", queueError);
	
//********************************* Socket **********************************//
	
	//Cierra la conexión
	close(SocketServidor); 
	printf("Cierro conexion de Socket\n");   		

}
//------------------------------------------------------------//

//------------------------------------------------------------//
int main(int argc, const char *argv[]) {

	 if (argc != 2){
		   argv[1]="2000";
	   } 
	 
	 printf ("-------------->Programa A<--------------\n");

//***************************************************************************//		
//******************************* SEÑALES ***********************************//
//***************************************************************************//		
	 
	 
	 //Si recibo la señal Ctrl + c voy al manejador
	 signal(SIGINT, mnjd1);  //Ctrl + c
	 
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
//********************************* Socket **********************************//	 
//***************************************************************************//	
	 
//************************************ 1 ***********************************//
	 
	 //-- socket(): Crear el socket STREAM -----------//
	 SocketServidor = socket(AF_INET, SOCK_STREAM, 0);
	 
	 if(( SocketServidor)<0) {
		 printf ("ERROR en funcion socket()\n");
		 exit(-1);     } 
	 
	 printf ("Paso 1 Socket: Servidor creo socket\n");
   
//************************************ 2 ***********************************//
 
	 //-- preparar el address:port -------------//
	 //Tipo de red
	 direccion.sin_family = AF_INET;
	 
	 //Asigna una IP de la maquina
	 direccion.sin_addr.s_addr = htonl(INADDR_ANY);  // asigna una IP de la maquina
	 
	 //Transformo el puerto
	 direccion.sin_port = htons(atoi (argv[1]));     // puerto
	 
	 //-- bind(): asociamos el socket a la direccion------//
	 
	 if (bind (SocketServidor, (struct sockaddr *)&direccion,sizeof(struct sockaddr_in))<0) {
		 printf ("ERROR en funcion bind()\n");
		 exit(-1);     } 
	 
	 printf ("Paso 2 Socket: Asociar bind() \n");
 
//************************************ 3 ***********************************//
 
	 //-- listen(): Permitir hasta 1 conexion pendiente --//
	 if ((listen(SocketServidor, 1))<0) {
		 printf ("ERROR en funcion listen()\n");
		 exit(-1);     } 
	 
	 printf ("Paso 3 Socket: Permitir conexiones listen()\n");
 
//************************************ 4 ***********************************//
	 
	 //-- accept(): se bloquea hasta que entre una conexion --//
	 printf ("Paso 4 Socket: Bloqueo hasta que entre conexion accept()\n");
	 connections = 0;     
	 
	 //Acepto una conexion
	 connectedSocket = accept(SocketServidor, NULL, 0) ;
	 
	 //Si no habia conexiones
	 if (connectedSocket >= 0) 
	 {  
		 if (connections == 0) 
		 {
			 printf ("Desbloqueo de accept, entro conexion: %d\n",connectedSocket);
			 printf ("Envio mensaje de bienvenida al Cliente C: Bienvenido al servidor\n");
			 send (connectedSocket ,"Bienvenido al servidor\n", 23,0 ); 
			 connections = 1;  
		 }   
		 
		 //Lee el socket
		 while (( recievedChars = read(connectedSocket, bufferSocket, sizeof (bufferSocket))) > 0) 
		 {

			 
			 //Imprimo por pantalla lo recibido del socket
			 printf ("Recibido de Cliente C: %s\n", bufferSocket);
			 
			 //Envio mensaje por cola de mensajes a B
			 // Escribe en cola de mensajes
			 queueError = mq_send(messageQueue, bufferSocket, recievedChars, 1);
			 if(queueError == -1){
				 printf ("error en mq_send()");
				 exit(-1);    }
			 
			 printf("Mensaje enviado por Cola de Mensaje: %s (%d)\n", bufferSocket, queueError); 
		 }
	 } 
	 else
	 {
		 printf ("Error en la conexion\n");  
	 }
	//pthread_exit(0);
	 return(0);
}
























