
/////////// final_46 /////////////////////
/*
Cuatro procesos (A,B,C,D) relacionados de la siguiente forma:
A padre de B y padre de C

Los procesos se comunican de la siguiente forma:
A y B se comunican por medio de un pipe
A y C se comunican por medio de un pipe
A y D se comunican por medio de un socket datagrama

1) El proceso D crea el socket datagrama servidor
2) El proceso A crea el socket datagrama cliente, crea al proceso B y al proceso C 
3) El proceso B cuando recibe la señal SIGUSR1 escribe en el pipe1 el mensaje "0123456789" y termina.
4) El proceso C cuando recibe la señal SIGUSR2 escribe en el pipe2 el mensaje "ABCDEFGHIJ" y termina.
5) El proceso A espera a que termine el proceso B, lee el pipe1 y lo leído lo envía al socket datagrama servidor
6) El proceso A espera a que termine el proceso C, lee el pipe2 y lo leído lo envía al socket datagrama servidor
7) El proceso D lee el socket datagrama y muestra lo leído en pantalla

El proceso D debe ser el primero en comenzar en una consola
El proceso A comienza luego de D otra consola
Las señales se envían desde otra consola


       --- 
      | B |
       --- 
       |     -------      ---      --------      ---      --------  
       |--->| PIPE1 |--->|   |    | Socket |    |   |    |        |  
       |     -------     | A |--->|Datagama|--->| D |--->|pantalla|
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
#include <netinet/in.h>
#include <unistd.h>
#include <wait.h>
#include <arpa/inet.h> 

#define MENSAJE_1 "0123456789"
#define MENSAJE_2 "ABCDEFGHIJ"

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


//VARIABLES IPC PIPE
int ipcPipe[2];
//int recievedChars = 0;
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
	
	int i = 0;
	
	if (argc != 2){
		argv[1]="2000";
	} 
	
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
	//********************************* Socket **********************************//	 
	//***************************************************************************//	
	
	
	
	//************************************ 1 ***********************************//
	
	//-- socket(): Crear el socket DATAGRAMA----------//
	socketCliente = socket(AF_INET, SOCK_DGRAM, 0);
	
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
		
		sleep(1);
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
	sleep(2);
	
	//Cierra escritura lado padre
	close(ipcPipe[1]);
	
	int recievedChars = 0;
	char recieveBufferPipe[50] = {0};  
	
	for(i = 0; i < 2; i++)
	{
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
			
			//Envío por datagrama
			sendto(socketCliente , recieveBufferPipe, sizeof(recieveBufferPipe), 0, (struct sockaddr *) &direccion, sizeof(direccion)); 
			
			printf("Mensaje enviado por Socket DATAGRAMA a D: %s\n", recieveBufferPipe); 
		}
	}
	
	
	//Cierro conexion de datagrama
	close(socketCliente);
	
	exit(0);
}
		
		
		
		
		
		
		
		
		
		
		

