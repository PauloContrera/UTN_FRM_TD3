/*
Tres procesos A, B y C se comunican de la siguiente forma:
A y B se comunican por medio de una cola de mensajes
A y C se comunican por medio de un Socket Stream

1) El proceso C crea un servidor socket stream
2) El proceso A crea la cola de mensajes
3) El proceso A al recibir la se�al SIGUSR1 escribe en la cola de mensajes el mensaje "0123456789"
4) El proceso A al recibir la se�al SIGUSR2 crea un cliente socket stream y escribe en el FIFO "ABCDEFGHIJ"
5) El proceso C lee el el socket, muestra lo le�do en pantalla y termina
6) El proceso B esta esperando leer un mensaje, al recibir un mensaje en la cola de mensajes lo muestra en pantalla

El proceso C debe ser el primero en comenzar en una consola
El proceso A debe ser el segundo en comenzar en otra consola
Las se�ales se env�an desde otra consola

 ---      --------------      ---      --------
|   |--->|cola mensajes |--->| B |--->|Pantalla|
| A |     --------------      ---      --------
|   |     --------------      ---      --------
|   |--->| SOCKET STREAM|--->| C |--->|Pantalla|
 ---      --------------      ---      --------


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


char buff_tcl[256], buff_sck[56];
struct sockaddr_in direccion = {};
int sockfd, lee_tcl, lee_sck, conn;

int des_socket, sck_server, rx_socket, largo, cont;
char buffer_rx[256];
struct sockaddr_in struct_direccion = {};
int SockEscucha, SockConexion;

int err, leido;
char buff[1024];
mqd_t mqd;
struct mq_attr attr, attr_rcv;

//------------------------------------------------------------//
int main(int argc, const char *argv[])
{

  printf("Proceso A PID = %d\n", getpid());

  if (argc != 2)
  {
    argv[1] = "2000";
  }

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

  pthread_exit(0);
}
//------------------------------------------------------------//

//------------------------------------------------------------//
