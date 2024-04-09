/////////// final_51 /////////////////////
/*
Tres procesos (A,B,C) se comunican de la siguiente forma:
C y A se comunican por medio de un pipe
C y B se comunican por medio de una cola de mensajes

1) El proceso C crea el pipe 
2) El proceso C crea al proceso A.
3) El proceso C crea la cola de mensajes
4) El proceso C cuando recibe la se�al SIGUSR1 escribe en el pipe "0123456789" 
5) El proceso C cuando recibe la se�al SIGUSR2 escribe en la cola de mensajes "ABCDEFGHIJ" 
6) El proceso A lee el pipe y muestra en pantalla lo le�do
7) El proceso B abre la cola de mensajes
8) El proceso B lee el mensaje de la cola de mensajes y muestra en pantalla lo le�do

El proceso C es el encargado de crear el pipe y la cola de mensajes, es el primer proceso que se ejecuta.
Las se�ales se env�an desde otra consola


 ---      -----------------      ---      --------  
|   |--->|     pipe        |--->| A |--->|Pantalla|  
| C |     -----------------      ---      --------
|   |     -----------------      ---      --------  
|   |--->|Cola de mensajes |--->| B |--->|Pantalla|  
 ---      -----------------      ---      --------
 
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
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <wait.h>
#include <arpa/inet.h> 


#define MENSAJE_A "0123456789"
#define MENSAJE_B "ABCDEFGHIJ"

#define MQ_PATH "/MQ_TD3" 



//------------------------------------------------------------//
void mnjs1(int a){


}
//------------------------------------------------------------//
void mnjs2(int b){


}
//------------------------------------------------------------//


//------------------------------------------------------------//
int main() {

int escrito;
int fd[2];
int err, leido;
            char buff[1024];

 mqd_t mqd; 
   struct mq_attr attr, attr_rcv;
   attr.mq_msgsize = sizeof(buff);
   attr.mq_maxmsg = 5;
    
   mqd = mq_open(MQ_PATH, O_RDONLY | O_CREAT, 0666, &attr); 
   /*mqd = mq_open(
      MQ_PATH, 
      O_RDWR | O_CREAT, --> Intenta abrir como lectura y escritura pero (| significa o) sino la crea
      0777, 
      &attr); */
   if(mqd < 0) {
      write (STDOUT_FILENO, "Error al crear Cola\n", sizeof("Error al crear Cola\n"));
      // Puede que ya exista si hay error
   }else {
      write (STDOUT_FILENO, "Cola creada correctamente\n", sizeof("Cola creada correctamente\n"));
   } 



	while (1) { // Bucle infinito
        // Obtener los atributos de la cola para verificar si hay mensajes pendientes
        mq_getattr(mqd, &attr_rcv);

        // Leer desde la cola de mensajes, bloqueando si está vacía
        leido = mq_receive(mqd, buff, attr_rcv.mq_msgsize, 0);
        if(leido < 0) {
            write(STDOUT_FILENO, "Error al leer mensaje\n", sizeof("Error al leer mensaje\n"));
        } else {
            write(STDOUT_FILENO, "\nMensaje leido:\n", sizeof("\nMensaje leido:\n"));
			sleep(0.1);
            write(STDOUT_FILENO, buff, leido);
        }
    }
 





}
//------------------------------------------------------------//


