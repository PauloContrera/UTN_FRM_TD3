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

  //signal(SIGINT, mnjd1);
  printf("Proceso A PID = %d\n", getpid());

  if (argc != 2)
  {
    argv[1] = "2000";
  }

  //close(SockConexion);
  //close(SockEscucha);


  //****************** 1 *******************//
  //-- socket(): Crear el socket -----------//
  SockEscucha = socket(AF_INET, SOCK_STREAM, 0);

  if ((SockEscucha) < 0)
  {
    printf("ERROR en funcion socket()\n");
    exit(-1);
  }

  printf("Paso 1: Servidor creo socket\n");

  //****************** 2 *******************//

  //-- preparar el address:port -------------//
  struct_direccion.sin_family = AF_INET;
  struct_direccion.sin_addr.s_addr = htonl(INADDR_ANY); // asigna una IP de la maquina
  struct_direccion.sin_port = htons(atoi(argv[1]));     // puerto

  //-- bind(): asociamos el socket a la direccion------//

  if (bind(SockEscucha, (struct sockaddr *)&struct_direccion, sizeof(struct sockaddr_in)) < 0)
  {
    printf("ERROR en funcion bind()\n");
    printf("Puerto \n");
    exit(-1);
  }

  printf("Paso 2: Asociar bind() \n");

  //****************** 3 *******************//
  //-- listen(): Permitir hasta 1 conexion pendiente --//

  if ((listen(SockEscucha, 1)) < 0)
  {
    printf("ERROR en funcion listen()\n");
    exit(-1);
  }

  printf("Paso 3: Permitir conexiones listen()\n");

  while (1)
  {

    //****************** 4 *******************//
    //-- accept(): se bloquea hasta que entre una conexion --//

    printf("Paso 4: Bloqueo hasta que entre conexion accept()\n");
    cont = 0;
    SockConexion = accept(SockEscucha, NULL, 0);

    if (SockConexion >= 0)
    {
      if (cont == 0)
      {
        printf("Desbloqueo de accept, entro conexion: %d\n", SockConexion);
        send(SockConexion, "Bienvenido al servidor\n", 23, 0);
        cont = 1;
      }

      while ((rx_socket = read(SockConexion, buffer_rx, sizeof(buffer_rx))) > 0)
      { // lee del socket
        /*write(SockConexion, buffer_rx, rx_socket);  // escribe en socket
        write(STDOUT_FILENO, "cliente:--> ", 12);   // escribe leyenda en pantalla
        write(STDOUT_FILENO, buffer_rx, rx_socket); // escribe lo leido del socket*/
        
        printf("Leido %s \n",buffer_rx);
        // Se escribe por la cola de mensajes:
        /*err = mq_send(mqd, buffer_rx, strlen(buffer_rx) + 1, 1); // strlen nos da la longitud de una cadena
        if (err == -1)
        {
          printf("error en mq_send()\n");
          exit(-1);
        }

        printf("Mensaje enviado (%d)\n", err);*/

      }
    }
    else
    {
      printf("Error en la conexion\n");
    }
  }

  pthread_exit(0);
}
//------------------------------------------------------------//

//------------------------------------------------------------//