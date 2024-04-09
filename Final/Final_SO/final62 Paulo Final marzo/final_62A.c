/*
/////////// final_62 ///////////////////// 

Tres procesos A, B y C se comunican de la siguiente forma:
B y A se comunican por medio de un socket1 stream
C y A se comunican por medio de un socket2 datagrama

1) El proceso A crea el socket1 stream servidor y el socket2 datagrama servidor
2) El proceso A debe ser capaz de atender los dos sockets en forma simultanea(utilizar hilos / hijos)
3) El proceso B crea un un socket stream cliente
4) El proceso C crea un un datagrama stream cliente
5) El proceso B al recibir la se�al SIGUSR1 escribe en el socket1 stream "0123456789"
6) El proceso C al recibir la se�al SIGUSR2 escribe en el socket2 stream "ABCDEFGHIJ" 
7) El proceso A al recibir algo por el socket1 stream lo muestra en pantalla y escribe en el socket lo recibido
8) El proceso A al recibir algo por el socket2 datagrama lo muestra en pantalla y escribe en el socket lo recibido

El proceso A debe ser el primero en comenzar en una consola
El proceso B debe ser el segundo en comenzar en otra consola
El proceso c debe ser el tercero en comenzar en otra consola
Las se�ales se env�an desde otra consola

 ---      -----------------      ---      --------  
| B |--->|socket1 stream   |--->|   |--->|Pantalla|  
|   |    |  Pantalla       |<---|   |--->|        |  
 ---      -----------------     | A |     --------
 ---      -----------------     |   |     --------  
| C |--->|socket2 datagram |--->|   |--->|Pantalla|  
|   |    |  Pantalla       |<---|   |--->|        |  
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

//------------------------------------------------------------//
   int des_socket, sck_server, rx_socket, largo, cont;
      char buffer_rx[256];
      struct sockaddr_in struct_direccion = {};
      int SockEscucha, SockConexion;

      int SockEscucha, sck_server, rx_socket, largo, cont;
char buffer_rx[256];

int main(int argc,
  const char * argv[]) {

  if (argc != 3) {
    argv[1] = "2000";
    argv[2] = "2001";
  }

  switch (fork()) {

    case -1: //ERROR
      write(STDOUT_FILENO, "Error al crear hijo\n", sizeof("Error al crear hijo\n"));
      return -1;
      break;

    case 0:
      //*Hijo Socket Stream
   

      //****************** 1 *******************//
      //-- socket(): Crear el socket -----------//
      SockEscucha = socket(AF_INET, SOCK_STREAM, 0);

      if ((SockEscucha) < 0) {
        printf("Stream: ERROR en funcion socket()\n");
        exit(-1);
      }

      printf("Stream; Paso 1: Servidor creo socket\n");

      //****************** 2 *******************//

      //-- preparar el address:port -------------//
      struct_direccion.sin_family = AF_INET;
      struct_direccion.sin_addr.s_addr = htonl(INADDR_ANY); // asigna una IP de la maquina
      struct_direccion.sin_port = htons(atoi(argv[1])); // puerto

      //-- bind(): asociamos el socket a la direccion------//

      if (bind(SockEscucha, (struct sockaddr * ) & struct_direccion, sizeof(struct sockaddr_in)) < 0) {
        printf("Stream:  ERROR en funcion bind()\n");
        exit(-1);
      }

      printf("Stream: Paso 2: Asociar bind() \n");

      //****************** 3 *******************//
      //-- listen(): Permitir hasta 1 conexion pendiente --//

      if ((listen(SockEscucha, 1)) < 0) {
        printf("Stream: ERROR en funcion listen()\n");
        exit(-1);
      }

      printf("Stream: Paso 3: Permitir conexiones listen()\n");

      while (1) {

        //****************** 4 *******************//
        //-- accept(): se bloquea hasta que entre una conexion --//

        printf("Stream: Paso 4: Bloqueo hasta que entre conexion accept()\n");
        cont = 0;
        SockConexion = accept(SockEscucha, NULL, 0);

        if (SockConexion >= 0) {
          if (cont == 0) {
            printf("Stream: Desbloqueo de accept, entro conexion: %d\n", SockConexion);
            send(SockConexion, "Stream: Bienvenido al servidor\n", 23, 0);
            cont = 1;
          }

          while ((rx_socket = read(SockConexion, buffer_rx, sizeof(buffer_rx))) > 0) { //lee del socket    
            write(SockConexion, buffer_rx, rx_socket); //escribe en socket
            write(STDOUT_FILENO, "Stream: cliente:--> ", 12); //escribe leyenda en pantalla
            write(STDOUT_FILENO, buffer_rx, rx_socket); //escribe lo leido del socket
          }
        } else {
          printf("Stream: Error en la conexion\n");
        }
      }

      //****************** 5 *******************//
      //------------cierrar la conexion --------// 

      close(SockConexion);

      return 0;

      break;

    default:
      //*Padre Socket Datagram
      int SockEscucha, sck_server, rx_socket, largo, cont;
      char buffer_rx[256];
      struct sockaddr_in struct_direccion = {};

//****************** 1 *******************//
  //-- socket(): Crear el socket -----------//
    SockEscucha=socket(AF_INET, SOCK_DGRAM, 0);
    
    if(( SockEscucha)<0) {
        printf ("Data: ERROR en funcion socket()\n");
    exit(-1);     } 
    
    printf ("Data: Paso 1: creo socket servidor\n");

  //----- preparar el address:port -------------//
    struct_direccion.sin_family = AF_INET;
    struct_direccion.sin_addr.s_addr = htonl(INADDR_ANY);  // asigna una IP de la maquina
    struct_direccion.sin_port = htons(atoi (argv[2]));     // port pasado por argumento

  //****************** 2 *******************//
  //-- bind(): asociamos el socket a la direccion ------//

    if (bind (SockEscucha, (struct sockaddr *)&struct_direccion,sizeof(struct sockaddr_in))<0) {
        printf ("Data: ERROR en funcion bind()\n");
    exit(-1);     } 

    printf ("Data: Paso 2: Asociar bind() \n");

  //****************** 3 *******************//
  //-- servidor espera a recibir algo ---
        printf("Data: vamos a leer el\n");

    while(1) {  //lee del socket    

      int len=sizeof(struct_direccion);
      rx_socket = recvfrom(SockEscucha, buffer_rx, sizeof (buffer_rx), 0,(struct sockaddr*)&struct_direccion,&len);            //recibe del socket 

      if (rx_socket>0) {
        printf("Data: entre\n");

          write ( STDOUT_FILENO , "Data: cliente:--> ", 12);      //escribe leyenda en pantalla
          write ( STDOUT_FILENO , buffer_rx, rx_socket);      //escribe lo leido del socket
          sendto(SockEscucha, buffer_rx, rx_socket, 0, (struct sockaddr *) &struct_direccion, sizeof(struct_direccion)) ;                  //envia al socket lo que recibio

      }
  }

  //****************** 4 *******************//
  //------------cierrar la conexion --------// 

    close(sck_server); 

      return 0;

      break;
  }





  exit(0);

}