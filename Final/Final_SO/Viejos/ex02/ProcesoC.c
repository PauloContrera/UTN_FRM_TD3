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
|   |--->|     FIFO	    |--->| C |--->|Pantalla|  
 ---      ----------------      ---      --------
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

   int err, fifo_d;
   char buff[80];
   int leido;                 

   // FIFO puede ser leida, escrita y ejecutada por: 
   err = mkfifo(FIFO_PATH, 0777); // el resto de los usuarios del sistema.
   
   write(STDOUT_FILENO, "\nEntrando Proceso C", sizeof("\nEntrando Proceso C"));

   fifo_d = open(FIFO_PATH, O_RDONLY, 0); //fifo abierta solo para lectura

   // Se lee FIFO
   leido = read(fifo_d, buff, sizeof(buff));   
   if(leido == -1){
      write(STDOUT_FILENO, "\nProceso C: Error al leer en FIFO", sizeof("\nProceso C: Error al leer en FIFO"));
   }else {
      write(STDOUT_FILENO, "\nProceso C: Leido del FIFO: ", sizeof("\nProceso C: Leido del FIFO: "));
      write(STDOUT_FILENO, buff, leido-1);
      write(STDOUT_FILENO, "\n", sizeof("\n"));
   }

   close(fifo_d);
   unlink(FIFO_PATH);
   write(STDOUT_FILENO, "\nSaliendo Proceso C\n", sizeof("\nSaliendo Proceso C\n"));
   exit(0);
}
