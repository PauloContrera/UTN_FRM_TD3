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
  /* man
  ~El programa no puede ignorar la señal SIGKILL ya que esta señal no puede ser manejada o bloqueada por ningún proceso. 
  ~Al intentar hacerlo, el proceso simplemente termina inmediatamente.
  */
    signal(SIGKILL, SIG_IGN);

   signal(SIGSTOP, SIG_IGN);  //Ctrl + Z
   
   printf("Proceso PID = %d\n", getpid());   
   
  write(STDOUT_FILENO,"Me rehuso a terminar\n", sizeof("Me rehuso a terminar\n"));

   while(TRUE);
   
   exit(0);
}
