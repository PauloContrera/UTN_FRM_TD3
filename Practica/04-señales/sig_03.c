#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#include <stdbool.h>  // Agregamos la librería correspondiente

#define TRUE true     // Definimos la constante TRUE

/*
void manejador_senial(int a){
   
}
*/

int main ()
{
  /*
  ~Cuando se ejecuta la combinación de teclas CONTROL+Z, el programa entra en modo suspensión (Stopped) y deja de ejecutarse. 
  ~Esto se debe a que la señal SIGTSTP no fue ignorada en el programa.
  ~
  ~Por otro lado, al ejecutar la combinación de teclas CONTROL+C, el programa termina su ejecución abruptamente y se interrumpe su ejecución. 
  ~Esto se debe a que la señal SIGINT es enviada al programa y no fue manejada en el código.
  */

  signal(SIGKILL, SIG_IGN);
   //signal(SIGSTOP, SIG_IGN);  //Ctrl + Z
   
   printf("Proceso PID = %d\n", getpid());   
   
   while(TRUE);
   
   exit(0);

   
}
