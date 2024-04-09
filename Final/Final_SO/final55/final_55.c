/////////// final_55 /////////////////////
/*
Tres procesos A, B y C se comunican de la siguiente forma:
A y B se comunican por medio de una cola de mensajes
A y C se comunican por medio de un FIFO

1) El proceso C crea el FIFO y lo abre con permisos de solo lectura
2) El proceso A crea la cola de mensajes 
3) El proceso A al recibir la señal SIGUSR1 escribe en la cola de mensajes el mensaje "0123456789"
4) El proceso A al recibir la señal SIGUSR2 abre el FIFO con permisos de escritura y escribe en el FIFO "ABCDEFGHIJ" 
5) El proceso C lee el FIFO, muestra lo leído en pantalla y termina
6) El proceso B esta esperando leer un mensaje, al recibir un mensaje en la cola de mensajes lo muestra en pantalla

El proceso C debe ser el primero en comenzar en una consola
El proceso A debe ser el segundo en comenzar en otra consola
Las señales se envían desde otra consola

 ---      --------------      ---      --------  
|   |--->|cola mensajes |--->| B |--->|Pantalla|  
| A |     --------------      ---      --------
|   |     --------------      ---      --------  
|   |--->|     FIFO     |--->| C |--->|Pantalla|  
 ---      --------------      ---      --------


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

//------------------------------------------------------------//

