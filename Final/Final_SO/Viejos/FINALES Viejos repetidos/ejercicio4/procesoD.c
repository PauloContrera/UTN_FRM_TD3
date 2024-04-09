#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include<sys/wait.h> 

#define FIFO_PATH "/tmp/MI_FIFO"

int main(){

   int err, fifo_d,lee;
   char buff[80];
   int leido;
                

   // FIFO puede ser leida, escrita y ejecutada por: 
   err = mkfifo(FIFO_PATH, 0777); // el resto de los usuarios del sistema.
   if(err == -1) {
      write(STDOUT_FILENO, "\nError al crear FIFO, la FIFO ya existe", sizeof("\nError al crear FIFO, la FIFO ya existe"));
   }else {
      write(STDOUT_FILENO, "\nFIFO creado correctamente\n", sizeof("\nFIFO creado correctamente\n"));
   }


         fifo_d = open(FIFO_PATH, O_RDONLY, 0); // abierto para solo escritura
         if(fifo_d == -1){
            write(STDOUT_FILENO, "\nError al abrir FIFO ", sizeof("\nError al abrir FIFO "));
            return -1;
         }else {
            write(STDOUT_FILENO, "\nProceso D: FIFO abierto correctamente", sizeof("\nProceso B: FIFO abierto correctamente"));
         }     

	 //Se lee en la fifo
    for(int i = 0; i < 2; i++){
	 lee = read(fifo_d, buff, sizeof(buff));
	 if(lee == -1)
	 {
	 write(STDOUT_FILENO, "\nProceso D: Error al leer en FIFO", sizeof("\nProceso D: Error al leer en FIFO"));
	 }
	 else {
            write(STDOUT_FILENO, "\nProceso D: Leido del FIFO: ", sizeof("\nProceso D: Leido del FIFO: "));
            write(STDOUT_FILENO, buff, lee-1);
            write(STDOUT_FILENO, "\n", sizeof("\n"));
         }
   }
         

    close(fifo_d);
         
	 unlink(FIFO_PATH);    //si la FIFO existe la borro   
	   
    exit(0);
 }