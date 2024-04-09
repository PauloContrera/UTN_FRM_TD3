/////////////////////////EJERICIO 4 //////////////////////////////////////////////////////////////
/*
Cuatro procesos (A,B,C,D) relacionados de la siguiente forma:
A padre de B y padre de C

Los procesos se comunican de la siguiente forma:
A y B se comunican por medio de un pipe
A y C se comunican por medio de un pipe
A y D se comunican por medio de una FIFO

1) El proceso A crea la FIFO, crea al proceso B y al proceso C 
2) El proceso B cuando recibe la señal SIGUSR1 escribe en el pipe1 el mensaje "0123456789" y termina.
3) El proceso C cuando recibe la señal SIGUSR2 escribe en el pipe2 el mensaje "ABCDEFGHIJ" y termina.
4) El proceso A espera a que termine el proceso B, lee el pipe1 y lo leído lo envía a la FIFO
5) El proceso A espera a que termine el proceso C, lee el pipe2 y lo leído lo envía a la FIFO
6) El proceso D lee los mensajes de la FIFO (2 mensajes) y los muestra en pantalla

El proceso A debe ser el primero en comenzar en una consola
El proceso D comienza luego de A otra consola
Las señales se envían desde otra consola


       --- 
      | B |
       --- 
       |     -------      ---      --------      ---      --------  
       |--->| PIPE1 |--->|   |    |        |    |   |    |        |  
       |     -------     | A |--->|  FIFO  |--->| D |--->|pantalla|
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



// FIFO
#define FIFO_PATH "/tmp/MI_FIFO"
int err, fifo_d,lee;
char buff[80];
int leido;
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
   exit(0);
}

void manejador_senial_2(int a)
{
   write(STDOUT_FILENO,"\nPROCESO C ESCRIBIENDO EN PIPE\n",sizeof("\nPROCESO C ESCRIBIENDO EN PIPE\n"));
   strncpy(buff, MENSAJE_C, sizeof(MENSAJE_C)); 
   write(ipc_2[1], buff, sizeof(MENSAJE_C));
   write(STDOUT_FILENO,"\nPROCESO C TERMINADO\n",sizeof("\nPROCESO C TERMINADO\n"));
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

// FIFO
   
   // FIFO puede ser leida, escrita y ejecutada por: 
   err = mkfifo(FIFO_PATH, 0777); // el resto de los usuarios del sistema.
   if(err == -1) {
      write(STDOUT_FILENO, "\nError al crear FIFO, la FIFO ya existe\n", sizeof("\nError al crear FIFO, la FIFO ya existe\n"));
   }else {
      write(STDOUT_FILENO, "\nFIFO creado correctamente\n", sizeof("\nFIFO creado correctamente\n"));
   }

         fifo_d = open(FIFO_PATH, O_WRONLY, 0); // abierto para solo escritura
         if(fifo_d == -1){
            write(STDOUT_FILENO, "\nError al abrir FIFO \n", sizeof("\nError al abrir FIFO \n"));
            return -1;
         }else {
            write(STDOUT_FILENO, "\nProceso A: FIFO abierto correctamente\n", sizeof("\nProceso A: FIFO abierto correctamente\n"));
         } 

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

   // Se escribe en el FIFO
   err = write(fifo_d, buff, sizeof(buff));
   if(err == -1) {
      write(STDOUT_FILENO, "\nProceso A: Error al escribir en FIFO", sizeof("\nProceso A: Error al escribir en FIFO"));
   } else {
      write(STDOUT_FILENO,"\nESCRIBIENDO EN FIFO\n",sizeof("\nESCRIBIENDO EN FIFO\n"));
   }

   wait(NULL);
   leido = read(ipc_2[0], buff, sizeof(buff));
   if(leido < 1){
      write (STDOUT_FILENO, "\nError al leer tuberia", sizeof("\nError al leer tuberia"));
   }

   // Se escribe en el FIFO
   err = write(fifo_d, buff, sizeof(buff));
   if(err == -1) {
      write(STDOUT_FILENO, "\nProceso A: Error al escribir en FIFO", sizeof("\nProceso A: Error al escribir en FIFO"));
   } else {
      write(STDOUT_FILENO,"\nPROCESO A: ESCRIBIENDO EN FIFO\n",sizeof("\nPROCESO A: ESCRIBIENDO EN FIFO\n"));
   }
   
   //------------cierra el fifo y tuberías--------//
   close(fifo_d);
   close(ipc_1[0]);
   close(ipc_2[0]);
// ----------------------------------

   return 0;   
}
