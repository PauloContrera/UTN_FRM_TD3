#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h> 
#include <signal.h>

#define FRASE_A "INFORMACION IMPORTANTE A" //!Declarar como variable global
// *Compilamos gcc -o c/pipe pipe.c

int main (){

// *Creamos la pipe
// !La creamos al inicio para enlazarla con el hijo
   int fd[2];
   pid_t pid;

   int leido, escrito;
   char buff[80]; 

   pipe(fd);
        // Crea la pipe y da los descriptores fd[0] fd[1]
        //fd[0] -> Read
        //fd[1] -> Write
   //printf ("fd[0] = %d fd[1] = %d \n ", fd[0], fd[1]);   
   pid = fork();

if (pid == 0 ){ 
   // *HIJO
   //*Lectura
   close(fd[1]);  //Cerramos Escritura
    leido = read(fd[0], buff, sizeof(buff));
    if(leido < 1){
        printf("\n");
        // Escribe en consola
        write (STDOUT_FILENO, "Hijo, tuberia vacía: ", sizeof("Hijo, tuberia vacía:"));
        //write (STDOUT_FILENO, leido, sizeof(leido));
    }
    else {
       // Escribe en consola
       printf("\n");
       write (STDOUT_FILENO, "Hijo, leido de la tuberia: ", sizeof("Hijo, leido de la tuberia: "));
       write (STDOUT_FILENO, buff, leido);
    }
    close(fd[0]);//Cerramos lectura
}
else 
{
   // *Padre
   //*Escritura 

   close(fd[0]); //Cerramos lectura 
    // Se escribe en la tuberia
    strncpy(buff, FRASE_A, sizeof(FRASE_A)); //Copia la FRASE_A en buff 
    escrito=write(fd[1], buff,    sizeof(FRASE_A)); //Envia la copia
    if(escrito < 1){
       // Escribe en consola
       printf("\n");
       write (STDOUT_FILENO, "Fallo la escritura: ", sizeof("Fallo la escritura: "));
       //write (STDOUT_FILENO, leido, sizeof(leido));

    }
    else {
       // Escribe en consola
       printf("\n");
       write (STDOUT_FILENO, "Escritura perfecta", sizeof("Escritura perfecta"));
       //write (STDOUT_FILENO, buff, leido);

    }
    close(fd[1]);//Cerramos Escritura
 }


}