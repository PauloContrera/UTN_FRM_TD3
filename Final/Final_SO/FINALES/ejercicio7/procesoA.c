/////////////////////////EJERICIO 7//////////////////////////////////////////////////////////////
/*

Tres procesos (A,B,C) se comunican de la siguiente forma:
A y B se comunican por medio de PIPE
A y C se comunican por medio de un socket datagrama

1) El proceso A crea al proceso B
2) El proceso A crea un socket datagrama servidor
3) El proceso C (cliente) crea un socket datagrama cliente
4) El proceso C (cliente) cuando recibe la señal SIGTSTP (Ctrl+z) le envia al servidor el mensaje "ABCDEFGHIJ"
5) El proceso A (servidor) cuando recibe algo por el socket lo lee lo muestra por pantalla y se lo escribe en el PIPE
6) El proceso B al recibir  la señal SIGTSTP (Ctrl+z) lee el PIPE y muestra lo leido
7) El proceso A cuando recibe la señal SIGINT (Ctrl+c) cierra el pipe, cierra el socket y termina
8) El proceso C cuando recibe la señal SIGINT (Ctrl+c) cierra el socket y termina

       --- 		  ---		     ---
      | C |		 | A |		 | B |
       --- 		  ---		     --- 
	|     ------     | |     ------     | |     --------
	|--->|socket|--->| |--->| PIPE |--->| |--->|pantalla|
	      ------		  ------             --------

C ---> /_socket_/--> A --> /_PIPE_/ --> B --> /_pantalla_/
*/

// COMANDO PARA COMPILAR: gcc -o procesoA procesoA.c

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>    // Define pid_t
#include <unistd.h>       // Define fork, getpid y getppid
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h> 



// SOCKET
int SockEscucha, sck_server, rx_socket, largo, cont;
char buffer_rx[256];
struct sockaddr_in struct_direccion={};
// ---------------------------------

// PIPE
#define MENSAJE "ABCDEFGHIJ"
int ipc_1[2], proc;
int leido;
char buff[80];
// ----------------------------------

void manejador_senial_1(int a)
{
   leido = read(ipc_1[0], buff, sizeof(buff));
   if(leido < 1){
      write (STDOUT_FILENO, "\nError al leer tuberia", sizeof("\nError al leer tuberia"));
   }
   write (STDOUT_FILENO, "\nPROCESO B LEE DE LA TUBERÍA: ", sizeof("\nPROCESO B LEE DE LA TUBERÍA: "));
   write (STDOUT_FILENO, buff, leido);
   write (STDOUT_FILENO, "\n", 1);
      
}

void manejador_senial_2(int a)
{
   write (STDOUT_FILENO, "\nPROCESO A TERMINANDO ", sizeof("\nPROCESO A TERMINANDO "));
   close(ipc_1[1]);
   close(SockEscucha);
   exit(0);
}

int main(int argc , char const * argv[])
{

   if (argc != 2){
       argv[1]="2000";
   }
   
   printf ("\nSoy el proceso A, mi pid es %d\n", getpid());


// PIPE 
pipe(ipc_1);
// ----------------------------------

// PROCESOS

   pid_t pid;

	pid = fork();	
   if(pid == 0){
      printf ("\nSoy el proceso B, mi pid es %dn", getpid());
      // PIPE
      close(ipc_1[1]);  
      // SIGNAL
      signal(SIGTSTP, manejador_senial_1);
      while (1){};
   }

   printf ("pid de mi hijo PROCESO B es %d\n", pid);

// ----------------------------------

// SIGNAL
   signal(SIGINT, manejador_senial_2);
// ----------------------------------

// PIPE
close(ipc_1[0]);
// ----------------------------------

// SOCKET

 //****************** 1 *******************//
 //-- socket(): Crear el socket -----------//
   SockEscucha=socket(AF_INET, SOCK_DGRAM, 0);
   
   if(( SockEscucha)<0) {
       printf ("ERROR en funcion socket()\n");
   exit(-1);     } 
   
   printf ("Paso 1: creo socket servidor\n");

//----- preparar el address:port -------------//
   struct_direccion.sin_family = AF_INET;
   struct_direccion.sin_addr.s_addr = htonl(INADDR_ANY);  // asigna una IP de la maquina
   struct_direccion.sin_port = htons(atoi (argv[1]));     // port pasado por argumento

 //****************** 2 *******************//
 //-- bind(): asociamos el socket a la direccion ------//

   if (bind (SockEscucha, (struct sockaddr *)&struct_direccion,sizeof(struct sockaddr_in))<0) {
       printf ("ERROR en funcion bind()\n");
   exit(-1);     } 

   printf ("Paso 2: Asociar bind() \n");

//****************** 3 *******************//
//-- servidor espera a recibir algo ---
   while(1) {  //lee del socket    

     int len=sizeof(struct_direccion);
     rx_socket = recvfrom(SockEscucha, buffer_rx, sizeof (buffer_rx), 0,(struct sockaddr*)&struct_direccion,&len);            //recibe del socket 

     if (rx_socket>0) {
        write ( STDOUT_FILENO , "\nROCESO A: cliente:--> ", sizeof("\nROCESO A: cliente:--> "));      //escribe leyenda en pantalla
        write ( STDOUT_FILENO , buffer_rx, rx_socket);      //escribe lo leido del socket
     }

     write(STDOUT_FILENO,"\nPROCESO A ESCRIBIENDO EN PIPE\n",sizeof("\nPROCESO A ESCRIBIENDO EN PIPE\n"));
     strncpy(buff, MENSAJE, sizeof(MENSAJE)); 
     write(ipc_1[1], buff, sizeof(MENSAJE));
   } 

// ----------------------------------

   return 0;   
}