/////////////////////////EJERICIO 16//////////////////////////////////////////////////////////////
/*
Tres procesos (A, B y C) se comunican por medio de 2 Tuberías.
1) El proceso A escribe en una Tubería "0123456789" y termina
2) El proceso B escribe en otra Tubería "ABCDEFGHIJ" y termina.
3) El proceso C lee de las Tuberías después que A y B terminaron y muestra lo leído.

Considerar los procesos con la siguiente jerarquía: Proceso C (padre), Proceso B (hijo 1 de C), Proceso A (hijo 2 de C)
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

int main (){

   int ipc_1[2], ipc_2[2], proc;
   int leido, success;
   char buff[BUFF_SIZE] = {0};

   pipe(ipc_1);
   pipe(ipc_2);

   switch (fork()){ 
      
      case 0:
      	 printf("Proceso A escribiendo en tuberia\n");
         //close(ipc[1]);//Nunca se escribe la tubería
         //strncpy(buff, DATA, sizeof(DATA)); 
         write(ipc_1[1], MENSAJE_A, sizeof(MENSAJE_A));
         exit(0);
      break;
         
      default:

         switch (fork()){ 
      
            case 0:
               printf("Proceso B escribiendo en tuberia\n");
               //close(ipc[1]);//Nunca se escribe la tubería
               //strncpy(buff, DATA, sizeof(DATA)); 
               write(ipc_2[1], MENSAJE_B, sizeof(MENSAJE_B));
               exit(0);
               
            default:

               wait(NULL);
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

               printf("Proceso C: leyendo tubería del Proceso B\n");
               leido = read(ipc_2[0], buff, sizeof(buff));
               if(leido < 1){
                  write (STDOUT_FILENO, "\nError al leer tuberia", sizeof("\nError al leer tuberia"));
               }else {
                  write (STDOUT_FILENO, "Leido de la tuberia: ", sizeof("Leido de la tuberia:"));
                  write (STDOUT_FILENO, buff, leido-1);
                  write (STDOUT_FILENO, "\n", sizeof("\n"));
               }

               exit(0);
         }

      break;
   }
}