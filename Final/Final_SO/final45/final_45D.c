
/////////// final_45 /////////////////////
/*
Cuatro procesos (A,B,C,D) relacionados de la siguiente forma:
A padre de B y padre de C

Los procesos se comunican de la siguiente forma:
A y B se comunican por medio de un pipe
A y C se comunican por medio de un pipe
A y D se comunican por medio de una cola de mensajes

1) El proceso A crea la cola de mensajes, crea al proceso B y al proceso C 
2) El proceso B cuando recibe la se�al SIGUSR1 escribe en el pipe1 el mensaje "0123456789" y termina.
3) El proceso C cuando recibe la se�al SIGUSR2 escribe en el pipe2 el mensaje "ABCDEFGHIJ" y termina.
4) El proceso A espera a que termine el proceso B, lee el pipe1 y lo le�do lo env�a a la cola de mensajes 
5) El proceso A espera a que termine el proceso C, lee el pipe2 y lo le�do lo env�a a la cola de mensajes 
6) El proceso D lee los mensajes de la cola mensajes (2 mensajes) y los muestra en pantalla

El proceso A debe ser el primero en comenzar en una consola
El proceso D comienza luego de A otra consola
Las se�ales se env�an desde otra consola


       --- 
      | B |
       --- 
       |     -------      ---      --------      ---      --------  
       |--->| PIPE1 |--->|   |    | Cola   |    |   |    |        |  
       |     -------     | A |--->|Mensajes|--->| D |--->|pantalla|
       |     -------     |   |--->|        |--->|   |--->|        | 
       |--->| PIPE2 |--->|   |     --------      ---      --------  
       |     -------      ---
       ---              
      | C |
       --- 
*/

//Compile el programa: gcc -o name name.c -lpthread -lrt
//Compile el programa: gcc -o file/procA final_45A.c -lpthread -lrt
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
#include <unistd.h>
#include <wait.h>
#include <arpa/inet.h> 


#define MQ_PATH "/MQ_TD3" 


//------------------------------------------------------------//

int escrito;
int fb[2];
int fc[2];
int err, err2, leido, mensajes;
            char buff[1024];
            char buff2[1024];
              mqd_t mqd;
            struct mq_attr attr, attr_rcv;


//------------------------------------------------------------//
int main() {
       attr.mq_msgsize = sizeof(buff);
                  attr.mq_maxmsg = 5;
                  //* Se crea cola de mensajes  
                  // Se fijan algunos parametros de la cola de mensajes antes de crearla
                  mqd = mq_open(MQ_PATH, O_RDWR | O_CREAT, 0777, & attr);
 
                  if (mqd < 0) {
                      write(STDOUT_FILENO, "Error al crear Cola\n", sizeof("Error al crear Cola\n"));
                      // Puede que ya exista si hay error
                  } else {
                      write(STDOUT_FILENO, "Cola creada correctamente\n", sizeof("Cola creada correctamente\n"));
                  }

//*Lectura

            //Funcion para saber que hay en la cola 
            if (mq_getattr(mqd, &attr_rcv) == -1){
               printf ("error en mq_getattr()\n");
            }
            else{
               printf("Nro max. de mensajes en cola de mensajes: %ld\n", attr_rcv.mq_maxmsg);
               printf("Longitud max. de mensaje: %ld\n", attr_rcv.mq_msgsize);
               printf("Nros de mensajes pendientes en cola de mensajes: %ld\n", attr_rcv.mq_curmsgs);
               mensajes=attr_rcv.mq_curmsgs;
               printf("Nros de mensajes pendientes en cola de mensajes: %d\n", mensajes);
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

