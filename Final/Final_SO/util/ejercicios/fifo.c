/*
 * Ejercicio 1 del TP FIFO  Tipica implementacion de una FIFO.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include<sys/wait.h> 

#define MENSAJE "HOLA PROCESO HIJO"
#define FIFO_PATH "/tmp/MI_FIFO"

// *Compilamos gcc -o c/fifo fifo.c

int main(){

   int err, fifo_d;
   char buff[80];
   int leido;

  
// *Boramos la fifo
   if(unlink(FIFO_PATH) == 0) {
      write (STDOUT_FILENO, "FIFO borrada correctamente", sizeof("FIFO borrada correctamente"));
   }else {
      write (STDOUT_FILENO, "No se pudo borrar la fifo", sizeof("No se pudo borrar la fifo"));
   }


// *Creamos la fifo
   //! Puede ser creada por Padre, Hijo, o proceso extra. A su ves puede ser creada en el pasado
   
   if(mkfifo(FIFO_PATH, 0777) == -1) {
      write (STDOUT_FILENO, "Error al crear FIFO\n", sizeof("Error al crear FIFO\n"));
      // Puede que ya exista si hay error
   }else {
      write (STDOUT_FILENO, "FIFO creada correctamente\n", sizeof("FIFO creada correctamente\n"));
   }

   switch (fork()){

      case -1:   //ERROR
         write (STDOUT_FILENO, "Error al crear hijo\n", sizeof("Error al crear hijo\n"));
         return -1;
      break;

      case 0:      
      //*Hijo
         write (STDOUT_FILENO, "Hijo inicia\n", sizeof("Hijo inicia\n"));
         sleep(2);
		  // *Abrimos la fifo
         fifo_d = open(FIFO_PATH,  O_RDONLY); // O_NONBLOCK
         if(fifo_d == -1){
            write (STDOUT_FILENO, "Error al abrir FIFO\n", sizeof("Error al abrir FIFO\n"));
         }else {
            write (STDOUT_FILENO, "FIFO abierta correctamente\n", sizeof("FIFO abierta correctamente\n"));
         }

         //*Lectura
            leido = read(fifo_d, buff, sizeof(buff));
            if(leido < 1){
               write(STDOUT_FILENO, "FIFO vacia\n", sizeof("FIFO vacia\n"));
            }else {
               write(STDOUT_FILENO, "Leido de la FIFO \"", sizeof("Leido de la FIFO \""));
               write(STDOUT_FILENO, buff, leido-1);
               write(STDOUT_FILENO, "\"\n", sizeof("\"\n"));
            }
            close(fifo_d);

         write (0, "Hijo termina\n", sizeof("Hijo termina\n")); 
         exit(0);

      break;
      
      default:    
      //*Padre
         write (STDOUT_FILENO, "Padre inicia\n", sizeof("Padre inicia\n"));
         sleep(3);

         // *Abrimos la fifo
         fifo_d = open(FIFO_PATH,  O_WRONLY); // O_NONBLOCK
         if(fifo_d == -1){
            write (STDOUT_FILENO, "Error al abrir FIFO\n", sizeof("Error al abrir FIFO\n"));
         }else {
            write (STDOUT_FILENO, "FIFO abierta correctamente\n", sizeof("FIFO abierta correctamente\n"));
         }


         //*Escritura
            err = write(fifo_d, MENSAJE, sizeof(MENSAJE));
            if(err == -1) {
               write(STDOUT_FILENO, "Error al escribir en FIFO\n", sizeof("Error al escribir en FIFO\n"));
            } else {
               write(STDOUT_FILENO, "Mensaje enviado correctamente\n", sizeof("Mensaje enviado correctamente\n"));
            }
            close(fifo_d);
         
         wait(NULL); //Esperamos que el hijo termine  

         write (0, "Padre termina\n", sizeof("Padre termina\n")); 
               
      break;   
   }
   
   // *Boramos la fifo
   if(unlink(FIFO_PATH) == 0) {
      write (STDOUT_FILENO, "FIFO borrada correctamente", sizeof("FIFO borrada correctamente"));
   }else {
      write (STDOUT_FILENO, "No se pudo borrar la fifo", sizeof("No se pudo borrar la fifo"));
   }
      
   exit(0);

}
