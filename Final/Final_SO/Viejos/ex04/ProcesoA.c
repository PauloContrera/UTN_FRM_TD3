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
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

//PIPE
#define BUFF_SIZE 80
#define MENSAJE1 "0123456789"
#define MENSAJE2 "ABCDEFGHIJ"
int ipc[2],ipc2[2], proc;
int leido, leido2, success;
char buff[BUFF_SIZE] = {0}, buff2[BUFF_SIZE] = {0}; 
//fifo
#define FIFO_PATH "/tmp/MI_FIFO1"
int err, fifo_d;
char buff[80];
int leido;  

void escribe_mensaje1(int a){ 

   write(STDOUT_FILENO, "\nProceso B escribe en pipe 1\n", sizeof("\nProceso B escribe en pipe 1\n"));
   strncpy(buff, MENSAJE1, sizeof(MENSAJE1)); //copia contenido de MENSAJE1 a buff
   write(ipc[1], buff, sizeof(MENSAJE1)); //escribe contenido de buff en tuberia 1
   write(STDOUT_FILENO, "\nProceso B termino\n", sizeof("\nProceso B termino\n"));
   exit(0);

}

void escribe_mensaje2(int a){ 

   write(STDOUT_FILENO, "\nProceso C escribe en pipe 2\n", sizeof("\nProceso C escribe en pipe 2\n"));
   strncpy(buff2, MENSAJE2, sizeof(MENSAJE2)); //copia contenido de MENSAJE2 a buff
   write(ipc2[1], buff2, sizeof(MENSAJE2)); //escribe contenido de buff en tuberia 2
   write(STDOUT_FILENO, "\nProceso C termino\n", sizeof("\nProceso C termino\n"));
   exit(0);

}


int main(int argc , char const * argv[])
{  


   if (argc != 2){
       argv[1]="2000";
   }
   
   printf("Soy el Proceso A: %d \n ",getpid()); 
   //FIFO
   // FIFO puede ser leida, escrita y ejecutada por: 
   err = mkfifo(FIFO_PATH, 0777); // el resto de los usuarios del sistema.
   write(STDOUT_FILENO, "\nFIFO lista\n", sizeof("\nFIFO lista\n"));

   fifo_d = open(FIFO_PATH, O_WRONLY, 0); //fifo solo de escritura
   //-------------------------------------------------------------------------//
   //PIPE
   pid_t pid;
   pipe(ipc);
   pipe(ipc2);
   pid = fork(); //creamos el proceso B
   if(pid == 0){
   	printf("Soy el proceso B, mi pid %d y el de mi padre %d\n",getpid(), getppid());
   	//solo deja abierta la escritura de la ipc, lo demas lo cerramos porque no nos interesa
   	close(ipc[0]);  //cierra lectura tuberia 1
   	close(ipc2[0]); //cierra lectura tuberia 2
   	close(ipc2[1]); //cierra escritura tuberia 2
   	signal(SIGUSR1, escribe_mensaje1);  //mensaje1
   	while(1){	//para que no termine
   	}
   }
   else{
   pid = fork(); //creamos el proceso C
   if(pid == 0){
   	printf("Soy el proceso C, mi pid %d y el de mi padre %d\n",getpid(), getppid());
   	//solo deja abierta la escritura de la ipc2, lo demas lo cerramos porque no nos interesa
   	close(ipc2[0]); //cierra lectura tuberia 2
   	close(ipc[0]); //cierra lectura tuberia 1
   	close(ipc[1]); //cierra escritura tuberia 2
   	signal(SIGUSR2, escribe_mensaje2);  //mensaje1
   	while(1){	//para que no termine
   	}
   }
   close(ipc[1]); //padre cierra escritura tuberia 1
   close(ipc2[1]); //padre cierra escritura tuberia 2
   wait(NULL); //espera a que termine el proceso B
   
   leido = read(ipc[0], buff, sizeof(buff)); //se lee de la tuberia 1
   if(leido < 1){	//verifica si hay error
   write (STDOUT_FILENO, "\nError al leer tuberia 1", sizeof("\nError al leer tuberia 1"));
   }
   //se escribe el contenido de la tuberia 1 en FIFO
   err = write(fifo_d, buff, sizeof(buff));
   write(STDOUT_FILENO, "\nProceso A: escrito en FIFO lo de tuberia 1\n", sizeof("\nProceso A: escrito en FIFO lo de tuberia 1\n"));
   write(STDOUT_FILENO, MENSAJE1, sizeof(MENSAJE1));
   
   wait(NULL); //espera a que termine el proceso C
   
   leido = read(ipc2[0], buff2, sizeof(buff2)); //se lee de la tuberia 2
   if(leido < 1){	//verifica si hay error
   write (STDOUT_FILENO, "\nError al leer tuberia 2", sizeof("\nError al leer tuberia 2"));
   }
   //se escribe el contenido de la tuberia 2 en FIFO
   err = write(fifo_d, buff2, sizeof(buff2));
   write(STDOUT_FILENO, "\nProceso A: escrito en FIFO lo de tuberia 2\n", sizeof("\nProceso A: escrito en FIFO lo de tuberia 2\n"));
   write(STDOUT_FILENO, MENSAJE2, sizeof(MENSAJE2));
  //fin PIPE
  
 }

//------------cierrar la conexion --------// 
   write (STDOUT_FILENO, "\nFinaliza el proceso A\n", sizeof("\nFinaliza el proceso A\n"));
   close(ipc[0]);  //padre cierra lectura tuberia 1
   close(ipc2[0]); //padre cierra lectura tuberia 2
   close(fifo_d);  //padre cierra fifo

   return 0;   
}
