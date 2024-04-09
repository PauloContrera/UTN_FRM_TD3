/////////////////////// EJERCICIO 2 ///////////////////////////////////////////////////////////////
/*
Tres procesos (A,B,C) NO relacionados se comunican de la siguiente forma:
A y B se comunican por medio de un socket datagrama
A y C se comunican por medio de una FIFO

1) El proceso B crea el socket datagrama servidor
2) El proceso A crea el socket datagrama cliente y la FIFO
3) El proceso A cuando recibe la señal SIGUSR1 escribe en el socket "0123456789" 
4) El proceso A cuando recibe la señal SIGUSR2 escribe en la FIFO "ABCDEFGHIJ"
5) El proceso B lee el socket y lo recibido lo muestra en pantalla
6) El proceso C lee la FIFO y lo recibido lo muestra en pantalla

El proceso B debe ser el primero en comenzar en una consola
El proceso A debe ser el segundo en comenzar en otra consola
El proceso C debe ser el tercero en comenzar en otra consola
Las señales se envían desde otra consola


 ---      ---------------       ---      --------  
|   |--->|Socket datagrma |--->| B |--->|Pantalla|  
| A |     ----------------      ---      --------
|   |     ----------------      ---      --------  
|   |--->|     FIFO	     |--->| C |--->|Pantalla|  
 ---      ----------------      ---      --------
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>
#include <signal.h>

// SOCKET
#define MENSAJE_SCK "0123456789"
char buff_tcl[256], buff_sck[56];
struct sockaddr_in direccion={};
int sockfd, lee_tcl, lee_sck, conn;
// ---------------------------------

// FIFO
#define MENSAJE "ABCDEFGHIJ"
#define FIFO_PATH "/tmp/MI_FIFO"

int err, fifo_d,lee;
char buff[80];
int leido;
// ----------------------------------

void manejador_senial_1(int a)
{
   //-- envia al socket datos -----------------//
   write(STDOUT_FILENO,"\nESCRIBIENDO EN SOCKET\n",sizeof("\nESCRIBIENDO EN SOCKET\n")); 
   sendto(sockfd, MENSAJE_SCK, strlen(MENSAJE_SCK)+1, 0, (struct sockaddr *) &direccion, sizeof(direccion)); 

   //****************** 3 *******************//
   //------------cierrar la conexion --------// 

   close(sockfd);

}

void manejador_senial_2(int a)
{
        // Se escribe en el FIFO
         err = write(fifo_d, MENSAJE, sizeof(MENSAJE));
         if(err == -1) {
            write(STDOUT_FILENO, "\nProceso A: Error al escribir en FIFO", sizeof("\nProceso A: Error al escribir en FIFO"));
         } else {
            write(STDOUT_FILENO,"\nESCRIBIENDO EN FIFO\n",sizeof("\nESCRIBIENDO EN FIFO\n"));
         }

         // Se cierra el FIFO
         close(fifo_d);
}

int main(int argc , char const * argv[])
{

   if (argc != 2){
       argv[1]="2000";
   }
   
   printf ("Soy el proceso A, mi pid es %d\n", getpid());
   
// SIGNAL

  signal(SIGUSR1, manejador_senial_1);  
  signal(SIGUSR2, manejador_senial_2); 

// ---------------------------------- 

// FIFO
   
   // FIFO puede ser leida, escrita y ejecutada por: 
   err = mkfifo(FIFO_PATH, 0777); // el resto de los usuarios del sistema.
   if(err == -1) {
      write(STDOUT_FILENO, "\nError al crear FIFO, la FIFO ya existe", sizeof("\nError al crear FIFO, la FIFO ya existe"));
   }else {
      write(STDOUT_FILENO, "\nFIFO creado correctamente\n", sizeof("\nFIFO creado correctamente\n"));
   }

         fifo_d = open(FIFO_PATH, O_WRONLY, 0); // abierto para solo escritura
         if(fifo_d == -1){
            write(STDOUT_FILENO, "\nError al abrir FIFO ", sizeof("\nError al abrir FIFO "));
            return -1;
         }else {
            write(STDOUT_FILENO, "\nProceso A: FIFO abierto correctamente", sizeof("\nProceso A: FIFO abierto correctamente"));
         } 

// ----------------------------------

// SOCKET

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
   direccion.sin_port=htons(atoi(argv[1])); 
//   inet_aton("localhost", &direccion.sin_addr); //convierte la direccion de Internet a una cadena de caracteres en la notacion estandar, Localhost = 127.0.0.1
   inet_aton("127.0.0.1", &direccion.sin_addr); //convierte la direccion IP a una cadena de caracteres en la notacion estandar
                                                //como el servidor esta en el mismo host usamos Localhost = 127.0.0.1

   printf ("Cliente va a enviar datos a IP: 127.0.0.1 Puerto: %s\n",  argv[1]);

// ----------------------------------

   while(1){};

   return 0;   
}
