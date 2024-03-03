/////////////////////////EJERICIO 3 //////////////////////////////////////////////////////////////
/*

Cuatro procesos (A,B,C,D) relacionados de la siguiente forma:
A padre de B y padre de C

Los procesos se comunican de la siguiente forma:
A y B se comunican por medio de un pipe
A y C se comunican por medio de un pipe
A y D se comunican por medio de un socket datagrama

1) El proceso D crea el socket datagrama servidor
2) El proceso A crea el socket datagrama cliente, crea al proceso B y al proceso C 
3) El proceso B cuando recibe la señal SIGUSR1 escribe en el pipe1 el mensaje "0123456789" y termina.
4) El proceso C cuando recibe la señal SIGUSR2 escribe en el pipe2 el mensaje "ABCDEFGHIJ" y termina.
5) El proceso A espera a que termine el proceso B, lee el pipe1 y lo leído lo envía al socket datagrama servidor
6) El proceso A espera a que termine el proceso C, lee el pipe2 y lo leído lo envía al socket datagrama servidor
7) El proceso D lee el socket datagrama y muestra lo leído en pantalla

El proceso D debe ser el primero en comenzar en una consola
El proceso A comienza luego de D otra consola
Las señales se envían desde otra consola


       --- 
      | B |
       --- 
       |     -------      ---      --------      ---      --------  
       |--->| PIPE1 |--->|   |    | Socket |    |   |    |        |  
       |     -------     | A |--->|Datagama|--->| D |--->|pantalla|
       |     -------     |   |--->|        |--->|   |--->|        | 
       |--->| PIPE2 |--->|   |     --------      ---      --------  
       |     -------      ---
       ---              
      | C |
       --- 
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>    // Define pid_t
#include <unistd.h>       // Define fork, getpid y getppid
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h> 
#include <stdio.h>



// SOCKET
char buff_tcl[256], buff_sck[56];
struct sockaddr_in direccion={};
int sockfd, lee_tcl, lee_sck, conn;
// ---------------------------------

// PIPE
#define MENSAJE_B "0123456789"
#define MENSAJE_C "ABCDEFGHIJ"
int ipc_1[2], ipc_2[2], proc;
int leido;
char buff[80];
// ----------------------------------

void manejador_senial_1(int a)
{
   write(STDOUT_FILENO,"\nPROCESO B ESCRIBIENDO EN PIPE\n",sizeof("\nPROCESO B ESCRIBIENDO EN PIPE\n")); 
   strncpy(buff, MENSAJE_B, sizeof(MENSAJE_B)); 
   write(ipc_1[1], buff, sizeof(MENSAJE_B));
   write(STDOUT_FILENO,"\nPROCESO B TERMINADO\n",sizeof("\nPROCESO B TERMINADO\n")); 
   //close(ipc_1[1]); 
   exit(0);
}

void manejador_senial_2(int a)
{
   write(STDOUT_FILENO,"\nPROCESO C ESCRIBIENDO EN PIPE\n",sizeof("\nPROCESO C ESCRIBIENDO EN PIPE\n"));
   strncpy(buff, MENSAJE_C, sizeof(MENSAJE_C)); 
   write(ipc_2[1], buff, sizeof(MENSAJE_C));
   write(STDOUT_FILENO,"\nPROCESO C TERMINADO\n",sizeof("\nPROCESO C TERMINADO\n"));
   //close(ipc_2[1]); 
   exit(0);
}

int main(int argc , char const * argv[])
{

   if (argc != 2){
       argv[1]="2000";
   }
   
   printf ("Soy el proceso A\n");
   

// PIPE
   
pipe(ipc_1);
pipe(ipc_2);

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

// PROCESOS

   pid_t pid;

	pid = fork();	
   if(pid == 0){
      printf ("Soy el proceso B, mi pid es %d y mi padre es %d\n", getpid(), getppid());
      // PIPE
      close(ipc_1[0]); 
      close(ipc_2[0]);
      close(ipc_2[1]);  
      // SIGNAL
      signal(SIGUSR1, manejador_senial_1);
      while (1){};
   }
   else{
      pid = fork();
      if(pid == 0){
         printf ("Soy el proceso C, mi pid es %d y mi padre es %d\n", getpid(), getppid());
         // PIPE
         close(ipc_2[0]); 
         close(ipc_1[0]);
         close(ipc_1[1]); 
         // SIGNAL
         signal(SIGUSR2, manejador_senial_2);
         while (1){};
      }
      /*else{
         wait(NULL);
         wait(NULL);
      }*/
   }
   close(ipc_1[1]); 
   close(ipc_2[1]);

   wait(NULL);
   leido = read(ipc_1[0], buff, sizeof(buff));
   if(leido < 1){
      write (STDOUT_FILENO, "\nError al leer tuberia", sizeof("\nError al leer tuberia"));
   }

   //-- envia al socket datos -----------------//
   write(STDOUT_FILENO,"\nESCRIBIENDO EN SOCKET\n",sizeof("\nESCRIBIENDO EN SOCKET\n")); 
   sendto(sockfd, buff, sizeof(buff), 0, (struct sockaddr *) &direccion, sizeof(direccion)); 

   wait(NULL);
   leido = read(ipc_2[0], buff, sizeof(buff));
   if(leido < 1){
      write (STDOUT_FILENO, "\nError al leer tuberia", sizeof("\nError al leer tuberia"));
   }

   //-- envia al socket datos -----------------//
   write(STDOUT_FILENO,"\nESCRIBIENDO EN SOCKET\n",sizeof("\nESCRIBIENDO EN SOCKET\n")); 
   sendto(sockfd, buff, sizeof(buff), 0, (struct sockaddr *) &direccion, sizeof(direccion)); 
   
   //------------cierra la conexion y tuberías--------// 
   close(sockfd);
   close(ipc_1[0]);
   close(ipc_2[0]);
// ----------------------------------

   return 0;   
}
