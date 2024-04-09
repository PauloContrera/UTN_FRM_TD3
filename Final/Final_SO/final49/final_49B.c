/////////// final_49 /////////////////////
/*
Tres procesos (A,B,C) NO relacionados se comunican de la siguiente forma:
C y A se comunican por medio de una memoria compartida, sincronizada con un sem�foro
A y B se comunican por medio de una cola de mensajes

1) El proceso A crea el sem�foro lo inicializa en el valor adecuado
2) El proceso A crea el objeto de memoria compartida,lo dimensiona y mapea el objeto de memoria compartida en el espacio virtual de direcciones del proceso.
3) El proceso A crea la cola de mensajes
4) El proceso C abre el objeto de memoria compartida y mapea el objeto de memoria compartida en el espacio virtual de direcciones del proceso.
5) El proceso C cuando recibe la se�al SIGUSR1 escribe en la Memoria Compartida "0123456789" y le avisa al proceso A por medio del sem�foro
6) El proceso A esta en una espera a que C escriba la memoria compartida y le avise que est� lista para ser le�da y cuando lee la memoria muestra lo le�do y escribe el mensaje en la cola de mensajes
7) El proceso B abre la cola de mensajes
8) El proceso B lee el mensaje de la cola de mensajes y termina

El proceso A es el encargado de crear la cola de mensajes, memoria compartida y el sem�foro, es el primer proceso que se ejecuta.

Las se�ales se env�an desde otra consola

 ---      -----------------      ---      -----------------      ---      --------  
| C |--->|Memoria comartida|--->| A |--->|Cola de mensajes |--->| B |--->|Pantalla| 
 ---      -----------------      ---      -----------------      ---      --------
 
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
int main() {








  exit(0);
}

//------------------------------------------------------------//

