/*
/////////// final_62 ///////////////////// 

Tres procesos A, B y C se comunican de la siguiente forma:
B y A se comunican por medio de un socket1 stream
C y A se comunican por medio de un socket2 datagrama

1) El proceso A crea el socket1 stream servidor y el socket2 datagrama servidor
2) el proceso A debe ser capaz de atender los dos sockets en forma simultanea(utilizar hilos / hijos)
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
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <wait.h>
#include <arpa/inet.h> 

int leido;
char buff[40];
char buff_tcl[256], buff_sck[56];
struct sockaddr_in direccion={};
int sockfd, lee_tcl, lee_sck, conn;


char buff_tcl1[256], buff_sck1[56];
struct sockaddr_in direccion1={};
int sockfd1, lee_tcl1, lee_sck1, s;


#define MENSAJE_A "0123456789"
#define MENSAJE_B "ABCDEFGHIJ"

//------------------------------------------------------------//

//------------------------------------------------------------//
void manejador_SIGUSR2(int b){
    printf("Señal SIGUSR2 recibida\n");
	sendto(sockfd,  MENSAJE_B, sizeof(MENSAJE_B), 0, (struct sockaddr *) &direccion, sizeof(direccion));
  
}
//------------------------------------------------------------//
 char buff_tcl[256], buff_sck[56];
   int sockfd, lee_tcl, lee_sck, conn;

//------------------------------------------------------------//
int main(int argc, const char *argv[]) {

   struct sockaddr_in direccion={};


 if (argc != 3){
       argv[1]="2000";
       argv[2]="2001";
   } 



//****************** 1 *******************//
 //-- socket(): Crear el socket -----------//

   sockfd=socket(AF_INET, SOCK_DGRAM, 0);

   if (sockfd==-1) { 
       printf ("ERROR en funcion socket()\n");
   exit(-1);     } 

   printf ("Paso 1: Se creo socket cliente\n");

   
//****************** 2 *******************//


//-- preparar el address:port del host servidor------//
   direccion.sin_family= AF_INET;
   direccion.sin_port=htons(atoi(argv[2])); 
//   inet_aton("localhost", &direccion.sin_addr); //convierte la direccion de Internet a una cadena de caracteres en la notacion estandar, Localhost = 127.0.0.1
   inet_aton("127.0.0.1", &direccion.sin_addr); //convierte la direccion IP a una cadena de caracteres en la notacion estandar
                                                //como el servidor esta en el mismo host usamos Localhost = 127.0.0.1

   printf ("Cliente va a enviar datos a IP: 127.0.0.1 Puerto: %s\n",  argv[2]);


//-- envia al socket datos --




     // Obtener y mostrar el PID del proceso actual
    pid_t pid = getpid();
    printf("Soy el proceso C, Mi PID es : %d\n", pid);

    // Configurar el manejador de señales para SIGUSR2
    signal(SIGUSR2, manejador_SIGUSR2);

    printf("Esperando la señal SIGUSR2 para imprimir un mensaje...\n");

    // Mantener el programa en ejecución
      //-- envia al socket datos --
  while(1){ 
 
    if (( lee_tcl = read(STDIN_FILENO, buff_tcl, sizeof (buff_tcl))) > 0) {     // lee de teclado
        sendto(sockfd, buff_tcl, lee_tcl, 0, (struct sockaddr *) &direccion, sizeof(direccion)); 
    }

   //-- recibe datos del socket -- 
     int len=sizeof(direccion);
     lee_sck = recvfrom(sockfd, buff_sck, sizeof (buff_sck), 0,(struct sockaddr *) &direccion,&len);
     write (STDOUT_FILENO, "Servidor:--> ", 13);               //escribe leyenda en pantalla
     write (STDOUT_FILENO, buff_sck, lee_sck);        //escribe lo leido del socket

  }

       close(sockfd);

   return 0;   









  	exit(0);

















  	exit(0);





}
//------------------------------------------------------------//






