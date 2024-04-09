/////////// final_48 /////////////////////
/*
Tres procesos (A,B,C) NO relacionados se comunican de la siguiente forma:
C y A se comunican por medio de una memoria compartida, sincronizada con un sem�foro
C y B se comunican por medio de una cola de mensajes


1) El proceso C crea el sem�foro lo inicializa en el valor adecuado
2) El proceso C crea el objeto de memoria compartida,lo dimensiona y mapea el objeto de memoria compartida en el espacio virtual de direcciones del proceso.
3) El proceso C crea la cola de mensajes
4) El proceso C cuando recibe la se�al SIGUSR1 escribe en la Memoria Compartida "0123456789" y le avisa al proceso A por medio del sem�foro
5) El proceso C cuando recibe la se�al SIGUSR2 escribe en la cola de mensajes "ABCDEFGHIJ"
6) El proceso A abre el objeto de memoria compartida y mapea el objeto de memoria compartida en el espacio virtual de direcciones del proceso.
7) El proceso A esta en una espera a que C escriba la memoria compartida y le avise que est� lista para ser le�da y cuando lee la memoria muestra lo le�do y termina
8) El proceso B abre la cola de mensajes
9) El proceso B lee el mensaje de la cola de mensajes y termina

El proceso C es el encargado de crear la cola de mensajes, memoria compartida y el sem�foro, es el primer proceso que se ejecuta.

Las se�ales se env�an desde otra consola

 ---      -----------------      ---      --------
|   |--->|Memoria comartida|--->| A |--->|Pantalla|
| C |     -----------------      ---      --------
|   |     -----------------      ---      --------
|   |--->|Cola de mensajes |--->| B |--->|Pantalla|
 ---      -----------------      ---      --------

*/

// Compile el programa: gcc -o name name.c -lpthread -lrt
// Ejecute el programa ./name

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

#define MQ_PATH "/MQ_TD3"

int err, leido;
char buff[1024];
mqd_t mqd;
struct mq_attr attr, attr_rcv;

//------------------------------------------------------------//
int main()
{

    mqd = mq_open(MQ_PATH, O_RDONLY, 0666, &attr);
    if (mqd < 0)
    {
        printf("error en mq_open()\n");
        exit(-1);
    }

    printf("Cola de mensajes abierta\n");


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

    printf("Mensaje leido por B: %s\n", buff);

    sleep(1);

    pthread_exit(0);
}

//------------------------------------------------------------//
