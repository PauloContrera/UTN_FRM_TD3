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

#define MENSAJE_B "ABCDEFGHIJ"
#define MQ_PATH "/MQ_TD3"

char buff_tcl[256], buff_sck[56];
struct sockaddr_in direccion = {};
int sockfd, lee_tcl, lee_sck, conn;

//------------------------------------------------------------//
void mnjd1(int a)
{
  printf("Se recibio señal\n");
  int mensaje = atoi(MENSAJE_B);
  write(sockfd, MENSAJE_B, 13); // escribe en socket
}
//------------------------------------------------------------//

//------------------------------------------------------------//
int main(int argc, const char *argv[])
{

  //close(sockfd);

  signal(SIGTSTP, mnjd1);
  printf("Proceso C PID = %d\n", getpid());

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

  //****************** 3 *******************//

  while (1)
  {
    /*
    lee_sck = read(sockfd, buff_sck, sizeof(buff_tcl)); // lee de socket
    write(STDOUT_FILENO, "Servidor:--> ", 13);          // escribe leyenda en pantalla
    write(STDOUT_FILENO, buff_sck, lee_sck);            // escribe lo leido del socket

    if ((lee_tcl = read(STDIN_FILENO, buff_tcl, sizeof(buff_tcl))) > 0)
    {                                   // lee de teclado
      write(sockfd, buff_tcl, lee_tcl); // escribe en socket
    }
    */
  }
  //****************** 4 *******************//
  //------------cierrar la conexion --------//

  close(sockfd);

  pthread_exit(0);
}
//------------------------------------------------------------//

//------------------------------------------------------------//
