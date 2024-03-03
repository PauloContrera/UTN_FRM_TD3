

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>


#define MENSAJE "ABCDEFGHIJ"
struct sockaddr_in direccion={};
int sockfd;

void manejador_senial_1()
{
   //-- envia al socket datos --//
   sendto(sockfd, MENSAJE, strlen(MENSAJE)+1, 0, (struct sockaddr *) &direccion, sizeof(direccion)); 

}

int main(int argc , char const * argv[])
{

   if (argc != 2){
       argv[1]="2000";
   } 

printf ("\nSoy el proceso B, mi pid es %d\n", getpid());

// SIGNAL
signal(SIGTSTP, manejador_senial_1);
// --------------------------------------

// SOCKET

//****************** 1 *******************//
 //-- socket(): Crear el socket -----------//

   sockfd=socket(AF_INET, SOCK_DGRAM, 0);

   if (sockfd==-1) { 
       printf ("ERROR en funcion socket()\n");
   exit(-1);     } 

   printf ("Paso 1: Se creo socket cliente\n");

   
//****************** 2 *******************//
//-- preparar el address:port del host servidor------//
   direccion.sin_family= AF_INET;
   direccion.sin_port=htons(atoi(argv[1])); 
//   inet_aton("localhost", &direccion.sin_addr); //convierte la direccion de Internet a una cadena de caracteres en la notacion estandar, Localhost = 127.0.0.1
   inet_aton("127.0.0.1", &direccion.sin_addr); //convierte la direccion IP a una cadena de caracteres en la notacion estandar
                                                //como el servidor esta en el mismo host usamos Localhost = 127.0.0.1

   printf ("Cliente va a enviar datos a IP: 127.0.0.1 Puerto: %s\n",  argv[1]);

//--------------------------------------------------

   while(1){};

   return 0;   
}
