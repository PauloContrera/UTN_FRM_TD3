/////////// final_56 /////////////////////
/*
Tres procesos NO relacionados (A,B,C) se comunican de la siguiente forma:
A y B se comunican por medio de una cola de mensaje
A y C se comunican por medio de un socket datagrama 
1) El proceso A crea un socket datagrama servidor
2) El proceso A crea y abre la cola de mensajes con permisos de solo escritura 
3) El proceso B abre la cola de mensajes con permisos de solo lectura 
4) El proceso C crea un socket datagrama cliente 
5) El proceso C esta en una espera activa y cuando recibe la señal SIGUSR1 le envía al servidor el mensaje  "ABCDEFGHIJ"
6) El proceso A (servidor) cuando recibe algo en por el socket lo lee lo muestra en pantalla y lo escribe en la cola de mensajes
7) El Proceso B está en una espera activa y al recibir la señal SIGUSR1 leer la cola de mensajesy muestra lo leído

El proceso A debe ejecutarse primero
El proceso B debe ejecutarse segundo
El proceso C debe ejecutarse tercero
Las señales se envían desde otra consola

       ---              ---                  ---  
   S->| C |            | A |             S->| B |
       ---              ---                  ---
       |     ------     | |     --------     | |     --------  
       |--->|socket|--->| |--->|COLA MSJ|--->| |--->|pantalla|  
             ------             --------             --------

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


//------------------------------------------------------------//
void manejador_SIGUSR1(int a){





}
//------------------------------------------------------------//
int main() {






	
   
  exit(0);
}

//------------------------------------------------------------//

