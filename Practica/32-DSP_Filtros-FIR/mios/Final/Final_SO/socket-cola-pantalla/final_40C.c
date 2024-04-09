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

#define MENSAJE_B "ABCDEFGHIJ"


#define MQ_PATH "/MQ_TD3" 

//VARIABLES SOCKET

//Creo el descriptor de socket
int socketCliente;
//Creo estructura direccion
struct sockaddr_in direccion = {};
//Variable auxiliares
int connectedSocket;
int recievedChars = 0;
char bufferSocket[50];
char IP[] = {"127.0.0.1"};

//------------------------------------------------------------//
void mnjd1(int a){

	//Escribo en pantalla lo enviado
	printf ("\nPaso 4 Socket: Senial recibida, envio:\n");
	printf ("Cliente C --> Servidor A: %s \n", MENSAJE_B);

	//Escribo en el socket
	write ( socketCliente , MENSAJE_B, sizeof(MENSAJE_B));
}
//------------------------------------------------------------//

//------------------------------------------------------------//
int main(int argc, const char *argv[]) {

	printf ("-------------->Programa C<--------------\n");	
	
//***************************************************************************//		
//******************************* SEÑALES ***********************************//
//***************************************************************************//		
	
	
	//Si recibo la señal Ctrl + z voy al manejador
	signal(SIGTSTP, mnjd1);  //Ctrl + z
	
	
	
	 if (argc != 2){
		   argv[1]="2000";
	   } 
   
//***************************************************************************//		 
//********************************* Socket **********************************//	 
//***************************************************************************//	
	 
	 
	 
 //************************************ 1 ***********************************//
 
	 //-- socket(): Crear el socket STREAM----------//
	 socketCliente = socket(AF_INET, SOCK_STREAM, 0);
	 
	 if (socketCliente == -1) { 
		 printf ("ERROR en funcion socket()\n");
		 exit(-1);     } 
	 
	 printf ("Paso 1 Socket: Se creo socket cliente\n");
	 
 //************************************ 2 ***********************************//
 
	 //-- preparar el address:port del host servidor STREAM ------//
	 //Tipo de red
	 direccion.sin_family = AF_INET;
	 
	 //Transformo el puerto
	 direccion.sin_port = htons(atoi(argv[1])); 
	 
	 //inet_aton("localhost", &direccion.sin_addr); //convierte la direccion de Internet a una cadena de caracteres en la notacion estandar, Localhost = 127.0.0.1
	 inet_aton(IP, &direccion.sin_addr); //convierte la direccion IP a una cadena de caracteres en la notacion estandar como el servidor esta en el mismo host usamos Localhost = 127.0.0.1
	 
	 
	 printf ("Cliente va a conectarse con IP:%s Puerto: %s\n", IP, argv[1]);
	 
	 //-- conectar el socket activo al socket de escucha --//
	 connectedSocket = connect(socketCliente, (struct sockaddr *)&direccion, sizeof (direccion));
	 
	 if (connectedSocket == -1) {   
		 printf ("ERROR en funcion connect()\n");
		 exit(-1);     }  
	 
	 printf ("Paso 2 Socket: Connect(), cliente conectado\n");
	 

	 
//************************************ 3 ***********************************//
	 
	 printf ("Paso 3 Socket: Esperando Senial\n");
	 
	 while(1){ 
		 
		 //Imprimo lo recibido por el servidor
		 recievedChars = read ( socketCliente , bufferSocket, sizeof (bufferSocket));    //lee de socket 
		 if(recievedChars != 0)
		 {
			 //escribe leyenda en pantalla lo leido del socket
			 printf ("Servidor A: %s", bufferSocket);
			 recievedChars = 0;
		 }
		 
	 }
//************************************ 4 ***********************************//
	 
	 //------------cierrar la conexion --------// 
	 close(socketCliente);
	 
	 //pthread_exit(0);
	 return(0);
}
//------------------------------------------------------------//

//------------------------------------------------------------//























