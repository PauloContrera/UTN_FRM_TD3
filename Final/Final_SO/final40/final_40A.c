// Chequeado con Teoria: 
// Socket Stream --> TCP (CON Conexion)
// Socket Datagram --> UDP (SIN Conexion)


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
  //****************** 5 *******************//
  //------------cierrar la conexion --------//
  close(SockConexion);

  // Se cierra cola de mensajes
  err = mq_close(mqd);
  if ((err < 0))
  {
    printf("error en mq_close()\n");
    exit(-1);
  }
  printf("Cola de mensajes cerrada (%d)\n", err);

  // Se elimina cola de mensajes
  err = mq_unlink(MQ_PATH);
  if (err == -1)
  {
    printf("error en mq_unlink()\n");
    exit(-1);
  }
  printf("Cola de mensajes eliminada (%d)\n", err);
}
//------------------------------------------------------------//

//------------------------------------------------------------//
int main(int argc, const char *argv[])
{

  signal(SIGINT, mnjd1);
  printf("Proceso A PID = %d\n", getpid());

  if (argc != 2)
  {
    argv[1] = "2000";
  }

  //close(SockConexion);
  //close(SockEscucha);

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
        err = mq_send(mqd, buffer_rx, strlen(buffer_rx) + 1, 1); // strlen nos da la longitud de una cadena
        if (err == -1)
        {
          printf("error en mq_send()\n");
          exit(-1);
        }

        printf("Mensaje enviado (%d)\n", err);
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
