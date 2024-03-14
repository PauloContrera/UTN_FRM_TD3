#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include<sys/wait.h> 

#define MENSAJE "HOLA PROCESO HIJO"
#define FIFO_PATH "/tmp/MI_FIFO" //!Dirrecion de la fifo 

int main (){

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
   // ~Otro metodo
      // FIFO puede ser leida, escrita y ejecutada por:
      err = mkfifo(FIFO_PATH, S_IRUSR | S_IWUSR | S_IXUSR|  // el usuario que la creo
                           S_IRGRP | S_IWGRP | S_IXGRP|  // el grupo al que pertenece el usuario
                           S_IROTH | S_IWOTH | S_IXOTH); // el resto de los usuarios del sistema

// *Abrimos la fifo
   //!Tiene que ser abierta por cada proceso, y con las culiadades de lectura o escritura

   int err, fifo_d;
   //!Conviene elegir O_RDONLY o O_WRONLY dado que siempre se trabaja con bloqueo
   fifo_d = open(FIFO_PATH,  O_RDONLY | O_NONBLOCK);  //Lectura No bloqueante (NO se bloquea sino no tiene mensaje la fifo)
   fifo_d = open(FIFO_PATH,  O_RDONLY);               //Lectura  Bloqueante (se bloquea sino no tiene mensaje la fifo)
   fifo_d = open(FIFO_PATH,  O_WRONLY | O_NONBLOCK);  //Escritura No bloqueante (NO se bloquea si la fifo esta llena)
   fifo_d = open(FIFO_PATH,  O_WRONLY);               //Escritura  Bloqueante (se bloquea si la fifo esta llena)
   fifo_d = open(FIFO_PATH, O_WRONLY, 0);             //Lectura y Escritura (NO se bloquea esperando a que los procesos se abran)

   if(fifo_d == -1){
      write (STDOUT_FILENO, "Error al abrir FIFO\n", sizeof("Error al abrir FIFO\n"));
      return -1;
   }else {
      write (STDOUT_FILENO, "FIFO abierta correctamente\n", sizeof("FIFO abierta correctamente\n"));
   }


//*Lectura
   int leido;

   leido = read(fifo_d, buff, sizeof(buff));
   if(leido < 1){
      write(STDOUT_FILENO, "FIFO vacia\n", sizeof("FIFO vacia\n"));
   }else {
      write(STDOUT_FILENO, "Leido de la FIFO \"", sizeof("Leido de la FIFO \""));
      write(STDOUT_FILENO, buff, leido-1);
      write(STDOUT_FILENO, "\"\n", sizeof("\"\n"));
   }
   close(fifo_d);



//*Escritura
   #define MENSAJE "HOLA PROCESO HIJO" //!Declarar como variable global

   err = write(fifo_d, MENSAJE, sizeof(MENSAJE));
   if(err == -1) {
      write(STDOUT_FILENO, "Error al escribir en FIFO\n", sizeof("Error al escribir en FIFO\n"));
   } else {
      write(STDOUT_FILENO, "Mensaje enviado correctamente\n", sizeof("Mensaje enviado correctamente\n"));
   }
   close(fifo_d);

}