/////////////////////////EJERICIO 17//////////////////////////////////////////////////////////////
/*
Tres procesos (A, B y C) se comunican por medio de 2 Tuberias.
1) El proceso C envia una señal (SIGUSR1) al proceso A. El proceso A al recibir esta señal escribe en una Tuberia "0123456789" y termina
2) El proceso C lee de la Tuberia despues que A termina, muestra lo leido y desturye la tuberia.
3) El proceso C envia una señal (SIGUSR2) al proceso B. El proceso B al recibir esta señal escribe en una Tuberia "ABCDEFGHIJ" y termina
4) El proceso C lee de la Tuberia despues que B termina, muestra lo leido y destruye la tuberia.

Considerar los procesos con la siguiente jerarquia: Proceso C (padre), Proceso B (hijo 1 de C), Proceso A (hijo 2 de C)
*/

// Comando: gcc -o procesoA procesoA.c

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>    // Define pid_t
#include <unistd.h>       // Define fork, getpid y getppid
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h> 


#define MENSAJE_A "0123456789"
#define MENSAJE_B "ABCDEFGHIJ"
#define BUFF_SIZE 80

int ipc_1[2], ipc_2[2], proc;
int leido, success;
char buff[BUFF_SIZE] = {0};


void signal1(){

         printf("Proceso A escribiendo en tuberia\n");
         write(ipc_1[1], MENSAJE_A, sizeof(MENSAJE_A));
         close(ipc_1[0]);
         close(ipc_1[1]);
         exit(0);
}

void signal2(){
   
         printf("Proceso B escribiendo en tuberia\n");
         write(ipc_2[1], MENSAJE_B, sizeof(MENSAJE_B));
         close(ipc_2[0]);
         close(ipc_2[1]);
         exit(0);
}

int main (){

   pipe(ipc_1);
   pipe(ipc_2);

   // SIGNAL
   signal(SIGUSR1, signal1);
   signal(SIGUSR2, signal2);
   //-----------------------------

   pid_t pid;
	pid = fork();

   switch (pid){ 
      
      case 0:
      	while(1);
      break;
         
      default:

         kill(pid, SIGUSR1);

         wait(NULL);

         printf("Proceso C: leyendo tubería del Proceso A\n");
         leido = read(ipc_1[0], buff, sizeof(buff));
         if(leido < 1){
            write (STDOUT_FILENO, "\nError al leer tuberia", sizeof("\nError al leer tuberia"));
         }else {
            write (STDOUT_FILENO, "Leido de la tuberia: ", sizeof("Leido de la tuberia:"));
            write (STDOUT_FILENO, buff, leido-1);
            write (STDOUT_FILENO, "\n", sizeof("\n"));
         }

         // Cerramos los extremos de la tuberia para destruirla
         close(ipc_1[0]);
         close(ipc_1[1]);

         pid_t pid;
         pid = fork();

         switch (pid){ 
      
            case 0:
               while(1);
            break;

            default:
               
               kill(pid, SIGUSR2);

               wait(NULL);

               printf("Proceso C: leyendo tubería del Proceso B\n");
               leido = read(ipc_2[0], buff, sizeof(buff));
               if(leido < 1){
                  write (STDOUT_FILENO, "\nError al leer tuberia", sizeof("\nError al leer tuberia"));
               }else {
                  write (STDOUT_FILENO, "Leido de la tuberia: ", sizeof("Leido de la tuberia:"));
                  write (STDOUT_FILENO, buff, leido-1);
                  write (STDOUT_FILENO, "\n", sizeof("\n"));
               }
               
               // Cerramos los extremos de la tuberia para destruirla
               close(ipc_2[0]);
               close(ipc_2[1]);

               exit(0);
               
            break;
         }

      break;
   }
}