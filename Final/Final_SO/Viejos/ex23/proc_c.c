////////////////////////////////////////////////////////////////////////
/*
Tres procesos A, B y C se comunican de la siguiente forma:

A y B se comunican por medio de una FIFO
B y C se comunican por medio de un socket STREAM

1) El proceso A crea la FIFO. 
2) El proceso C envia la señal SIGUSR1
3) El proceso A al recibir la señal SIGUSR1 escribe en la FIFO un mensaje.
4) El proceso B esta esperando leer un mensaje por la FIFO. 
5) El proceso B al recibir un mensaje lo reenvia al proceso C via socket STREAM.
6) El proceso C al recibir un mensaje, muestra lo leído en pantalla. 

 ---
|   |<------------------------------------------- SIGUSR1	
|   | 												 |
|   |     ---------     ---      --------------     ---      -------- 
|   |--->|  FIFO  |--->| B |--->|socket STREAM|--->| C |--->|Pantalla|  
| A |     ---------     ---      --------------     ---      --------
|   |   			  cliente					  servidor
|   |
 ---    
                        
Compile el programa: gcc proc_a.o -o proc_a -lpthread -lrt

*/

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

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define MENSAJE "ENVIADO INFORMACION IMPORTANTE"

//----------- Definición de Variables Globales---------------------
//Variables Socket
   int des_socket, sck_server, rx_socket, largo, cont;
   char buffer_rx[256]; //Buffer para los datos que se leen del socket.
   struct sockaddr_in struct_direccion={}; //Se define una estructura del tipo "sockaddr_in" con el nombre "struct_direccion" para contener la Direccion a la cual se va a enlazar el socket.
   int SockEscucha,SockConexion; //Se definen las variables "SockEscucha" y "SockCOnexion" de tipo "int" para guardar los descriptores del socket "servidor" y del socket "cliente" respectivamente.



int main(int argc, const char *argv[]) //La variable "argc" guarda el numero de argumentos que recibe el "main(). Es decir, cuando se ejecuta el programa "servidor.c" mediante ./servidor podemos incluir un argumento para el Numero de Puerto cuyo valor se guardará en la posición 1 del arreglo "argv". Por ejemplo: "./servidor 2000"       
{

   printf("\n\t\t----------- PROCESO C -----------\n");
//----------------- SOCKET ----------------------------------
   if (argc != 2) //Si el numero de argumentos recibidos por consola es distino de 2, significa que no especificamos ningun Puerto, por lo tanto se asigna el Puerto "2001".
	{
       	argv[1]="2001"; 
	} 

 //****************** PASO 1 *******************//
 //-- socket(): Crear el socket -----------//
   SockEscucha=socket(AF_INET, SOCK_STREAM, 0); //La llamada a socket() devuelve en "SockEscucha" el descriptor del socket creado en caso de éxito ó -1 en caso de error. El argumento AF_INET (Via IPv4) especifica el "Dominio ó Familia" de la comunicación, el argumento SOCK_STREAM (TCP_con conexión) especifica el "Tipo de Socket" y el arguemnto 0 (por defecto) especifica el "Protocolo".
   
   if(( SockEscucha)<0) //Si socket() devuelve "-1", hubo un ERROR al crear el socket.
	{
       	printf ("ERROR en funcion socket()\n");
   	exit(-1);      
	} 
   
   printf ("Paso 1: Servidor creo socket\n");

 //****************** PASO 2 *******************//

//-- preparar el address:port -------------//Configuramos los campos de la estructura "struct_direccion" que define la Direccion que se va a enlazar a socket servidor.
   struct_direccion.sin_family = AF_INET; //Especifica el "Dominio ó Familia de la Dirección". AF_INET (via IPv4)
   struct_direccion.sin_addr.s_addr = htonl(INADDR_ANY); //Especifica la "Dirección IP de la maquina (host)" a la que se asocia el socket. Para permitir conexiones de cualquier dirección se utiliza el valor "INADDR_ANY".
   struct_direccion.sin_port = htons(atoi (argv[1])); //Especifica el "Numero de Puerto de Protocolo" al que se asocia el socket.

 //-- bind(): asociamos el socket a la direccion------//

   if (bind (SockEscucha, (struct sockaddr *)&struct_direccion, sizeof(struct sockaddr_in))<0) //Se asocia el socket cuyo descriptor es "SockEscucha" con la Dirección especificada por la estructura "struct_direccion". El argumento "SockEscucha" contiene el Descriptor del socket servidor, el "&struct_direccion" es un Puntero a la estructura que especifica la direccion y el "sizeof(struct sockaddr_in)" especifia el Tamaño de la estructura tipo "sockaddr_in". La función "bind()" devuelve 0 en caso de éxito ó -1 si hay error.
	{
       	printf ("ERROR en funcion bind()\n"); //La función bind() devolvió "-1".
   	exit(-1);     
	} 

   printf ("Paso 2: Asociar bind() \n");

 //****************** PASO 3 *******************//
 //-- listen(): Permitir hasta 1 conexion pendiente --//

   if ((listen(SockEscucha, 1))<0) //Se especifica que el socket servidor cuyo descriptor es "SockEscucha" va a actuar como PASIVO, es decir que desea aceptar conexiones. El argumento "SockEscucha" especifica el Descriptor del socket pasivo y el argumento "1" indica el Backlog que define el numero máximo de conexiones pendientes, en este caso solo una. La funcion "listen()" devuelve 0 en caso de éxito y -1 en caso de error.
	{
       	printf ("ERROR en funcion listen()\n"); //La función "listen() devolvió -1.
   	exit(-1);     
	} 

   printf ("Paso 3: Permitir conexiones listen()\n");

  while(1) //Se define un lazo while para aceptar las peticiones de conexión recibidas.
	{

 //****************** PASO 4 *******************//
 //-- accept(): se bloquea hasta que entre una conexion --//

   	printf ("Paso 4: Bloqueo hasta que entre conexion accept()\n");
   	cont=0;     
   	SockConexion=accept(SockEscucha, NULL, 0); //Se aceptan conexiones entrantes al socket pasivo cuyo descriptor es "SockEscucha". El argumento "SockEscucha" especifica el descriptor del socket pasivo, el argumento "NULL" especifica un Puntero a la estructura de datos que contiene la Direccion asociada al socket pasivo y el argumento "0" especifica la Longitud de la estructura de Direccion. Como ya se enlazó la estructura de Direccion al socket "SockEscucha" no es necesario hacerlo nuevamente, por ello se coloca NULL y 0. La función "accept()" devuelve en "SockConexion" un entero No Negativo que es el Descriptor del socket aceptado (cliente) ó -1 en caso de error.

   	if (SockConexion >=0) //Si "accept()" tuvo éxito, se estableció la conexión con el cliente.
		{  
      		if (cont==0) 
			{
        		printf ("Desbloqueo de accept, entro conexion: %d\n",SockConexion); //Se imprime en pantalla el contenido de "SockConexion" que es el Descriptor del socket cliente. 
        		send (SockConexion ,"Bienvenido al servidor\n", 23,0 ); //Se envia un "mensaje" de bienvenida por única vez al socket cliente cuyo descriptor es "SockConexion".
      			cont=1; //Se modifica el valor de "cont" para que NO se vuelva a enviar el mensaje de bienvenida.  
			}   

       		while (( rx_socket = read(SockConexion, buffer_rx, sizeof (buffer_rx))) > 0) //Se leen los datos del socket cliente cuyo Descriptor es "SockConexion" y se guardan en el string "buffer_rx[]". La función "read()" devuelve en la variable entera "rx_socket" el numero de bytes leidos si tuvo éxito la lectura ó -1 en caso de error.
			{    
                  	write (STDOUT_FILENO , "cliente:--> ", 12);         //Escribe leyenda en pantalla (STDOUT_FILENO).
                  	write (STDOUT_FILENO , buffer_rx, rx_socket);      //Escribe lo leido del socket cliente cuyo descriptor es "SockConexion".

			
          		}

	    	} 
	else //No se estableció la conexión con el cliente. "accept()" devolvió -1.
		{ 
           	printf ("Error en la conexion\n");  
           	}
 	}

 
}  
