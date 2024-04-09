/////////// final_40 /////////////////////
/*
Tres procesos (A,B,C) No relacionados se comunican de la siguiente forma:
A y B se comunican por medio de una cola de mensajes
A y C se comunican por medio de un socket stream
1) El proceso A crea la cola de mensajes
2) El proceso A crea un socket stream servidor
3) El proceso C (cliente) crea un socket stream cliente
4) El proceso C (cliente) cuando recibe la se�al SIGTSTP (Ctrl+z) le env�a al servidor el mensaje  "ABCDEFGHIJ"
5) El proceso B abre la cola de mensajes
6) El proceso A cuando recibe la se�al SIGINT (Ctrl+c) cierra el socket y borra la cola de mensajes
7) El proceso A (servidor) cuando recibe algo en por el socket lo lee lo muestra en pantalla y se lo env�a al proceso B por medio de la cola de mensajes
8) Proceso B la cola de mensajes un mensaje y muestra lo le�do
El proceso A debe ser el primero en ejecutarse.

       ---              ---                ---
      | C |            | A |              | B |
       ---              ---                ---
       |     ------     | |     ------     | |     --------
       |--->|socket|--->| |--->|ColaMe|--->| |--->|pantalla|
             ------             ------             --------

   c --> /_Socket_/--> A --> /_ColaMe_/ --> C --> /_Pantalla_/

*/

// Compile el programa: gcc -o name name.c -lpthread -lrt
// Ejecute el programa ./name

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

int err, leido;
char buff[1024];
mqd_t mqd;
struct mq_attr attr, attr_rcv;

//------------------------------------------------------------//
int main()
{

  mqd = mq_open(MQ_PATH, O_RDONLY | O_CREAT, 0666, &attr);
  if (mqd < 0)
  {
    printf("error en mq_open()\n");
    exit(-1);
  }

  printf("Cola de mensajes creada\n");

 
  while (1)
  {
    // Se leen parametros de la cola de mensajes
    if (mq_getattr(mqd, &attr_rcv) == -1)
    {
      printf("error en mq_getattr()\n");
      exit(-1);
    }

    // Se lee de cola de mensajes
    leido = mq_receive(mqd, buff, attr_rcv.mq_msgsize, 0);
    if ((leido < 0))
    {
      printf("error en mq_receive()\n");
      exit(-1);
    }

    printf("Mensaje leido: %s\n", buff);

    sleep(1);
  }

  exit(0);
}

//------------------------------------------------------------//
