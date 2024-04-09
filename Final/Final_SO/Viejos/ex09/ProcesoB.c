/////////////////////////EJERICIO 9//////////////////////////////////////////////////////////////
/*
Tres procesos NO relacionados (A,B,C) se comunican de la siguiente forma:
A y B se comunican por medio de un FIFO
A y C se comunican por medio de un socket datagrama 
1) El proceso A crea un socket datagrama servidor
2) El proceso A crea y abre la FIFO con permisos de solo escritura 
3) El proceso B abre la FIFO con permisos de solo lectura 
4) El proceso C crea un socket datagrama cliente 
5) El proceso C esta en una espera activa y cuando recibe la señal SIGTSTP(Ctrl + z) le envía al servidor el mensaje "ABCDEFGHIJ"
6) El proceso A (servidor) cuando recibe algo en por el socket lo lee lo muestra en pantalla y lo escribe en la FIFO 
7) El Proceso B está en una espera activa y al recibir la señal SIGTSTP(Ctrl + z) leer la FIFO y muestra lo leído

El proceso A debe ser el primero en ejecutarse
El proceso B debe ejecutarse segundo
El proceso C debe ejecutarse tercero

       ---              ---                ---  
      | C |            | A |              | B |
       ---              ---                ---
       |     ------     | |     ------     | |     --------  
       |--->|socket|--->| |--->|FIFO  |--->| |--->|pantalla|  
             ------             ------              --------

   C --> /_socket_/--> A --> /_FIFO_/ --> C --> /_Pantalla_/ 
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include<sys/wait.h> 

#define FIFO_PATH "/tmp/MI_FIFO1"
int err, fifo_d, i;
char buff[80];
int leido;

void leer_mensaje(int a){
   //proceso B lee la fifo
   leido = read(fifo_d, buff, sizeof(buff));   
   	if(leido == -1){
      	write(STDOUT_FILENO, "\nProceso B: Error al leer en FIFO", sizeof("\nProceso B: Error al leer en FIFO"));
   	}else {
      	write(STDOUT_FILENO, "\nProceso B: Leido del FIFO: ", sizeof("\nProceso B: Leido del FIFO: "));
      	write(STDOUT_FILENO, buff, leido-1);
      	write(STDOUT_FILENO, "\n", sizeof("\n"));
   	}
}

void termina(int a){ 

   write(STDOUT_FILENO, "\nProceso B termino\n", sizeof("\nProceso B termino\n"));
   close(fifo_d);
   exit(0);

}

int main(){                 
   
   printf ("\nProceso B, pid es %d\n", getpid());
   signal(SIGTSTP, leer_mensaje);
   signal(SIGINT, termina);
   
   // FIFO puede ser leida, escrita y ejecutada por: 
   err = mkfifo(FIFO_PATH, 0777); // el resto de los usuarios del sistema.
   
   write(STDOUT_FILENO, "\nEntrando Proceso B", sizeof("\nEntrando Proceso B"));

   fifo_d = open(FIFO_PATH, O_RDONLY, 0); //fifo abierta solo para lectura
   while(1){};

}
