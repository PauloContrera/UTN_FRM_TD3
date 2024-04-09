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
      | C |		 | A |		    | B |
       --- 		  ---		     --- 
	|     ------     | |     ------     | |     --------
	|--->|socket|--->| |--->| PIPE |--->| |--->|pantalla|
	      ------		  ------             --------

C ---> /_socket_/--> A --> /_PIPE_/ --> B --> /_pantalla_/ 

programa SOCKET SERVIDOR de datagramas (UDP) el servidor envia al socket
lo que recibe del socket y muestra en pantalla lo recibido en 
el socket: Cliente:--> datos rx socket
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h> 
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

//PIPE
#define BUFF_SIZE 80
#define MENSAJE "ABCDEFGHIJ"
int ipc[2], proc;
int leido, success;
char buff[80];

//socket
int SockEscucha, sck_server, rx_socket, largo, cont;
char buffer_rx[256];
struct sockaddr_in struct_direccion={};

void lee_mensaje(int a){ 
   leido = read(ipc[0], buff, sizeof(buff)); //se lee de la tuberia
   if(leido < 1){	//verifica si hay error
   write (STDOUT_FILENO, "\nProceso B: Error al leer tuberia", sizeof("\nProceso B: Error al leer tuberia"));
   }
   write (STDOUT_FILENO, "\nProceso B: lee de la tuberìa\n", sizeof("\nProceso B: lee de la tuberìa\n"));
   write (STDOUT_FILENO, buff, sizeof(buff));
   write (STDOUT_FILENO, "\n",1);
}

void termina(int a){
   write (STDOUT_FILENO, "\nProceso A: terminando\n", sizeof("\nProceso A: terminando\n"));
   close(ipc[1]);
   close(SockEscucha);
   exit(0);
}

int main(int argc, const char *argv[])      {

   if (argc != 2){
       argv[1]="2000";
   }
   
   signal(SIGINT, termina);
   
   printf("Soy el Proceso A: %d \n ",getpid());
 //****************** 0 *******************//
 //Pipe
   pipe(ipc);
   pid_t pid;
   pid = fork();
   if(pid == 0){
   	printf("Soy el proceso B, mi pid %d y el de mi padre %d\n",getpid(), getppid());
   	//solo deja abierta la lectura de la ipc, porque el que escribe es el proceso A
   	close(ipc[1]);  //cierra lectura tuberia
   	signal(SIGTSTP, lee_mensaje);  //mensaje
   	while(1){};	//para que no termine
   	}
   close(ipc[0]); //Proceso A cierra lectura, solo debe escribir
   
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
        write ( STDOUT_FILENO , "cliente:--> ", 12);      //escribe leyenda en pantalla
        write ( STDOUT_FILENO , buffer_rx, rx_socket);      //escribe lo leido del socket
     }
     write(STDOUT_FILENO, "\nProceso A: escribiendo en tuberia\n", sizeof("\nProceso A: escribiendo en tuberia\n"));
     strncpy(buff, MENSAJE, sizeof(MENSAJE)); //copia contenido de MENSAJE a buff
     write(ipc[1], buff, sizeof(MENSAJE)); //escribe contenido de buff en tuberia
 }

 //****************** 4 *******************//
 //------------cierrar la conexion --------// 

   close(sck_server); 
 
  return 0;
}  
