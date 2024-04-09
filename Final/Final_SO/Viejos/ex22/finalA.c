/*
Tres procesos A, B y C se comunican de la siguiente forma:
A y B se comunican por medio de una cola de mensajes
A y C se comunican por medio de un Socket Stream

1) El proceso C crea un servidor socket stream 
2) El proceso A crea la cola de mensajes 
3) El proceso A al recibir la se�al SIGUSR1 escribe en la cola de mensajes el mensaje "0123456789"
4) El proceso A al recibir la se�al SIGUSR2 crea un cliente socket stream y escribe en el Socket "ABCDEFGHIJ" 
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


#define MENSAJE_B "ABCDEFGHIJ"
#define MENSAJE_A "0123456789"
#define MQ_PATH "/MQ_TD3"

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
void mnjd1(int a)
{
    // Se escribe por la cola de mensajes:
    //err = mq_send(mqd, buffer_rx, strlen(buffer_rx) + 1, 1); // strlen nos da la longitud de una cadena
    err = mq_send(mqd, MENSAJE_A, strlen(MENSAJE_A) + 1, 1); // strlen nos da la longitud de una cadena
    if (err == -1)
    {
        printf("error en mq_send()\n");
        exit(-1);
    }

    printf("Mensaje enviado (%d)\n", err);
}
//------------------------------------------------------------//


//------------------------------------------------------------//
void mnjd2(int a)
{
    
    /*if (argc != 2)
    {
        argv[1] = "2000";
    }*/
    
   

    //****************** 3 *******************//

    //-- Escribe en socket --------------------------
    write ( sockfd , MENSAJE_B, strlen(MENSAJE_B)+1);
    //write(sockfd, buff_tcl, lee_tcl); // escribe en socket

}
//------------------------------------------------------------//



//------------------------------------------------------------//
int main(int argc, const char *argv[])
{

  signal(SIGUSR1, mnjd1);
  signal(SIGUSR2, mnjd2);
  printf("Proceso A PID = %d\n", getpid());

  if (argc != 2)
  {
    argv[1] = "2000";
  }

   //****************** 1 *******************//
    //-- socket(): Crear el socket -----------//

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1)
    {
        printf("ERROR en funcion socket()\n");
        exit(-1);
    }

    printf("Paso 1: Se creo socket cliente\n");

    //****************** 2 *******************//

    //-- preparar el address:port del host servidor------//
    direccion.sin_family = AF_INET;
    direccion.sin_port = htons(atoi(argv[1]));
    //   inet_aton("localhost", &direccion.sin_addr); //convierte la direccion de Internet a una cadena de caracteres en la notacion estandar, Localhost = 127.0.0.1
    inet_aton("127.0.0.1", &direccion.sin_addr); // convierte la direccion IP a una cadena de caracteres en la notacion estandar
                                                // como el servidor esta en el mismo host usamos Localhost = 127.0.0.1

    printf("Cliente va a conectarse con IP:127.0.0.1 Puerto: %s\n", argv[1]);

    //-- conectar el socket activo al socket de escucha --//
    conn = connect(sockfd, (struct sockaddr *)&direccion, sizeof(direccion));

    if (conn == -1)
    {
        printf("ERROR en funcion connect()\n");
        exit(-1);
    }

    printf("Paso 2: Connect(), cliente conectado\n");



  // Se fijan algunos parametros de la cola de mensajes antes de crearla
  attr.mq_msgsize = sizeof(buff);
  attr.mq_maxmsg = 5;

  // Se crea cola de mensajes
  mqd = mq_open(MQ_PATH, O_WRONLY | O_CREAT, 0777, &attr);
  if (mqd < 0)
  {
    printf("error en mq_open()\n");
    exit(-1);
  }

  printf("Cola de mensajes creada\n");

  
 while(1){ }




  pthread_exit(0);
}
//------------------------------------------------------------//

//------------------------------------------------------------//
