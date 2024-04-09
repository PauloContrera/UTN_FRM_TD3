/////////////////////////EJERICIO 3 //////////////////////////////////////////////////////////////
/*

Cuatro procesos (A,B,C,D) relacionados de la siguiente forma:
A padre de B y padre de C

Los procesos se comunican de la siguiente forma:
A y B se comunican por medio de un pipe
A y C se comunican por medio de un pipe
A y D se comunican por medio de un socket datagrama

1) El proceso D crea el socket datagrama servidor
2) El proceso A crea el socket datagrama cliente, crea al proceso B y al proceso C 
3) El proceso B cuando recibe la señal SIGUSR1 escribe en el pipe1 el mensaje "0123456789" y termina.
4) El proceso C cuando recibe la señal SIGUSR2 escribe en el pipe2 el mensaje "ABCDEFGHIJ" y termina.
5) El proceso A espera a que termine el proceso B, lee el pipe1 y lo leído lo envía al socket datagrama servidor
6) El proceso A espera a que termine el proceso C, lee el pipe2 y lo leído lo envía al socket datagrama servidor
7) El proceso D lee el socket datagrama y muestra lo leído en pantalla

El proceso D debe ser el primero en comenzar en una consola
El proceso A comienza luego de D otra consola
Las señales se envían desde otra consola


       --- 
      | B |
       --- 
       |     -------      ---      --------      ---      --------  
       |--->| PIPE1 |--->|   |    | Socket |    |   |    |        |  
       |     -------     | A |--->|Datagama|--->| D |--->|pantalla|
       |     -------     |   |--->|        |--->|   |--->|        | 
       |--->| PIPE2 |--->|   |     --------      ---      --------  
       |     -------      ---
       ---              
      | C |
       --- 
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MENSAJE1 "0123456789"
#define MENSAJE2 "ABCDEFGHIJ"
#define BUFF_SIZE 80
//PIPE
int ipc[2],ipc2[2], proc;
int leido, leido2, success;
char buff[BUFF_SIZE] = {0}, buff2[BUFF_SIZE] = {0}; 
//socket
char buff_tcl[256], buff_sck[256];
struct sockaddr_in direccion={};
int sockfd, lee_tcl, lee_sck, conn;

void escribe_mensaje1(int a){ 

   write(STDOUT_FILENO, "\nProceso B escribe en pipe 1\n", sizeof("\nProceso B escribe en pipe 1\n"));
   strncpy(buff, MENSAJE1, sizeof(MENSAJE1)); //copia contenido de MENSAJE1 a buff
   write(ipc[1], buff, sizeof(MENSAJE1)); //escribe contenido de buff en tuberia 1
   write(STDOUT_FILENO, "\nProceso B termino\n", sizeof("\nProceso B termino\n"));
   exit(0);

}

void escribe_mensaje2(int a){ 

   write(STDOUT_FILENO, "\nProceso C escribe en pipe 2\n", sizeof("\nProceso C escribe en pipe 2\n"));
   strncpy(buff2, MENSAJE2, sizeof(MENSAJE2)); //copia contenido de MENSAJE2 a buff
   write(ipc2[1], buff2, sizeof(MENSAJE2)); //escribe contenido de buff en tuberia 2
   write(STDOUT_FILENO, "\nProceso C termino\n", sizeof("\nProceso C termino\n"));
   exit(0);

}


int main(int argc , char const * argv[])
{  


   if (argc != 2){
       argv[1]="2000";
   }
   
   printf("Soy el proceso: %d \n ",getpid()); 

//****************** 1 *******************//
 //-- socket(): Crear el socket -----------//

   sockfd=socket(AF_INET, SOCK_STREAM, 0);

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
                                                                                                
   printf ("Cliente va a conectarse con IP:127.0.0.1 Puerto: %s\n",  argv[1]);

//-- conectar el socket activo al socket de escucha --//
   conn=connect(sockfd, (struct sockaddr *)&direccion, sizeof (direccion));

   if (conn == -1) {   
        printf ("ERROR en funcion connect()\n");
   exit(-1);     }  

   printf ("Paso 2: Connect(), cliente conectado\n");

//****************** 3 *******************//
   //PIPE
   pid_t pid;
   pipe(ipc);
   pipe(ipc2);
   pid = fork(); //creamos el proceso B
   if(pid == 0){
   	printf("Soy el proceso B, mi pid %d y el de mi padre %d\n",getpid(), getppid());
   	//solo deja abierta la escritura de la ipc, lo demas lo cerramos porque no nos interesa
   	close(ipc[0]);  //cierra lectura tuberia 1
   	close(ipc2[0]); //cierra lectura tuberia 2
   	close(ipc2[1]); //cierra escritura tuberia 2
   	signal(SIGUSR1, escribe_mensaje1);  //mensaje1
   	while(1){	//para que no termine
   	}
   }
   else{
   pid = fork(); //creamos el proceso C
   if(pid == 0){
   	printf("Soy el proceso C, mi pid %d y el de mi padre %d\n",getpid(), getppid());
   	//solo deja abierta la escritura de la ipc2, lo demas lo cerramos porque no nos interesa
   	close(ipc2[0]); //cierra lectura tuberia 2
   	close(ipc[0]); //cierra lectura tuberia 1
   	close(ipc[1]); //cierra escritura tuberia 2
   	signal(SIGUSR2, escribe_mensaje2);  //mensaje1
   	while(1){	//para que no termine
   	}
   }
   close(ipc[1]); //padre cierra escritura tuberia 1
   close(ipc2[1]); //padre cierra escritura tuberia 2
   wait(NULL); //espera a que termine el proceso B
   
   leido = read(ipc[0], buff, sizeof(buff)); //se lee de la tuberia 1
   if(leido < 1){	//verifica si hay error
   write (STDOUT_FILENO, "\nError al leer tuberia 1", sizeof("\nError al leer tuberia 1"));
   }
   //se escribe el contenido de la tuberia 1 en socket
   write(STDOUT_FILENO,"\nEscribiendo en socket lo de tuberia 1\n",sizeof("\nEscribiendo en socket lo de tuberia 1\n"));
   //write ( sockfd , buff, sizeof(buff)); esto lo usamos con socket stream
   sendto(sockfd, buff, sizeof(buff), 0, (struct sockaddr *) &direccion, sizeof(direccion)); //socket datagrama
   
   wait(NULL); //espera a que termine el proceso C
   
   leido = read(ipc2[0], buff2, sizeof(buff2)); //se lee de la tuberia 2
   if(leido < 1){	//verifica si hay error
   write (STDOUT_FILENO, "\nError al leer tuberia 2", sizeof("\nError al leer tuberia 2"));
   }
   //se escribe el contenido de la tuberia 1 en socket
   write(STDOUT_FILENO,"\nEscribiendo en socket lo de tuberia 2\n",sizeof("\nEscribiendo en socket lo de tuberia 2\n"));
   //write ( sockfd , buff2, sizeof(buff2)); esto lo usamos con socket stream
   sendto(sockfd, buff2, sizeof(buff2), 0, (struct sockaddr *) &direccion, sizeof(direccion)); //socket datagrama
  //fin PIPE
  

 }
//****************** 4 *******************//
//------------cierrar la conexion --------// 
   write (STDOUT_FILENO, "\nFinaliza el proceso A\n", sizeof("\nFinaliza el proceso A\n"));
   close(sockfd);
   close(ipc[0]); //padre cierra lectura tuberia 1
   close(ipc2[0]); //padre cierra lectura tuberia 2

   return 0;   
}
