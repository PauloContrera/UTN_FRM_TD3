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
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include<sys/wait.h> 

#define FIFO_PATH "/tmp/MI_FIFO1"

int main(){

   int err, fifo_d, i;
   char buff[80];
   int leido;                 

   // FIFO puede ser leida, escrita y ejecutada por: 
   err = mkfifo(FIFO_PATH, 0777); // el resto de los usuarios del sistema.
   
   write(STDOUT_FILENO, "\nEntrando Proceso D", sizeof("\nEntrando Proceso D"));

   fifo_d = open(FIFO_PATH, O_RDONLY, 0); //fifo abierta solo para lectura

   // Se lee FIFO
   for(i = 0; i < 2; i++){
   	leido = read(fifo_d, buff, sizeof(buff));   
   	if(leido == -1){
      	write(STDOUT_FILENO, "\nProceso D: Error al leer en FIFO", sizeof("\nProceso D: Error al leer en FIFO"));
   	}else {
      	write(STDOUT_FILENO, "\nProceso D: Leido del FIFO: ", sizeof("\nProceso D: Leido del FIFO: "));
      	write(STDOUT_FILENO, buff, leido-1);
      	write(STDOUT_FILENO, "\n", sizeof("\n"));
   	}
   }
   close(fifo_d);
   unlink(FIFO_PATH);
   write(STDOUT_FILENO, "\nSaliendo Proceso D\n", sizeof("\nSaliendo Proceso D\n"));
   exit(0);
}
