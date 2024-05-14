////////////////////////////////////////////////////////////////////////
/*
Tres procesos A, B y C se comunican de la siguiente forma:

A y B se comunican por medio de una cola de mensajes
A y C se comunican por medio de un socket datagram
El proceso A debe resolver la sincronizaci�n de dos hilos.

1) El proceso A debe crear 2 hilos, asignarles las funciones hilo1() e 
hilo2() y resolver su sincronizacion.
2) Cuando la variable variable_global tenga cierto valor, la funcion 
comparar_var_global() entregar� un puntero a un arreglo de caracteres, 
a lo que llamamos mensaje. Este mensaje debe ser transmitido a los 
procesos B y C.
3) Se provee la funcion comparar_var_global() como un objeto binario.
4) El proceso A crea la cola de mensajes. 
5) El proceso A al recibir la se�al SIGUSR1 escribe en la cola de 
mensajes el mensaje.
6) El proceso C al recibir un datagrama, muestra lo le�do en pantalla 
y termina.
7) El proceso B esta esperando leer un mensaje, al recibir un mensaje 
en la cola de mensajes lo muestra en pantalla.

El proceso C debe ser el primero en comenzar en una consola
El proceso A debe ser el segundo en comenzar en otra consola
Las se�ales se env�an desde otra consola.

 ---
|   |  <---- SIGUSR1
|   |      --------------      ---      --------  
|   |--->|cola mensajes |--->| B |--->|Pantalla|  
| A |     --------------      ---      --------
|   |     --------------       ---      --------  
|   |--->|socket datagram|--->| C |--->|Pantalla|  
 ---      --------------       ---      --------
cliente                     servidor   

Compile el programa: gcc -c proc_a.c && gcc comparar_var_global.o proc_a.o -o proc_a -lpthread -lrt

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
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

const char * comparar_var_global(int var)
{
  	//~ if (var > 50 & var < 51) 
  	if (var == 51)   	
	{
		//~ printf("h3 %d\n", var);
		char *name = "TD3_MENSAJE_ABCD" ;
		//~ usleep(1000);
		return  name;
	}
	else{
		//char *name = "TD3_MENSAJE_ABCD" ;
		return 0 ;
	}
}	


int main()
{
	printf("");
   for(int t=0; t<100; t++){
	printf("%d", t);
   }
}
//------------------------------------------------------------//

