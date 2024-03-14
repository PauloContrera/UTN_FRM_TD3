
/////////// final_47 /////////////////////
/*
Tres procesos (A,B,C) NO relacionados se comunican de la siguiente forma:
A y B se comunican por medio de un socket datagrama
A y C se comunican por medio de una cola de mensajes

1) El proceso B crea el socket datagrama servidor
2) El proceso A crea el socket datagrama cliente y la cola de mensajes
3) El proceso A cuando recibe la se�al SIGUSR1 escribe en el socket "0123456789" 
4) El proceso A cuando recibe la se�al SIGUSR2 escribe en la cola de mensaje el mensaje "ABCDEFGHIJ"
5) El proceso B lee el socket y lo recibido lo muestra en pantalla
6) El proceso C lee la cola de mensaje y lo recibido lo muestra en pantalla

El proceso B debe ser el primero en comenzar en una consola
El proceso A debe ser el segundo en comenzar en otra consola
El proceso C debe ser el tercero en comenzar en otra consola
Las se�ales se env�an desde otra consola


 ---      ---------------       ---      --------  
|   |--->|Socket datagrma |--->| B |--->|Pantalla|  
| A |     ----------------      ---      --------
|   |     ----------------      ---      --------  
|   |--->|Cola de mensajes|--->| C |--->|Pantalla|  
 ---      ----------------      ---      --------
 

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

#define MENSAJE_A "0123456789"
#define MENSAJE_B "ABCDEFGHIJ"

#define MQ_PATH "/MQ_TD3" 


int main(int argc , char const * argv[]){
    
}
