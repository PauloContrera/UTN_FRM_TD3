

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

// COLA DE MENSAJES
#define MQ_PATH "/MQ_TD3" 
int err, leido;
char buff[1024];    
mqd_t mqd; 
struct mq_attr attr, attr_rcv;
// ---------------------------------


int main(int argc , char const * argv[])
{

   if (argc != 2){
       argv[1]="2000";
   }
   
   printf ("\nSoy el proceso B, mi pid es %d\n", getpid());

// COLA DE MENSAJES
   
   err = mq_unlink(MQ_PATH); //si existe la cola de mensajes, la borra

   attr.mq_msgsize = sizeof(buff);
   attr.mq_maxmsg = 5;

   mqd = mq_open(MQ_PATH, O_WRONLY | O_CREAT, 0666, &attr); //O_WRONLY: solo escritura | O_CREAT: si no existe la crea la cola de mensajes

   if (mqd < 0) {
      write(STDOUT_FILENO, "\nerror en mq_open()\n", sizeof("\nerror en mq_open()\n"));      
      exit(-1) ;}

   write(STDOUT_FILENO, "\nCola de mensajes creada\n", sizeof("\nCola de mensajes creada\n"));

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
        write ( STDOUT_FILENO , "\nPROCESO B: cliente:--> ", sizeof("\nROCESO B: cliente:--> "));      //escribe leyenda en pantalla
        write ( STDOUT_FILENO , buffer_rx, rx_socket);      //escribe lo leido del socket
     }

     write(STDOUT_FILENO,"\nPROCESO A ESCRIBIENDO EN COLA DE MENSAJES\n",sizeof("\nPROCESO A ESCRIBIENDO EN COLA DE MENSAJES\n"));
     err = mq_send(mqd, buffer_rx, strlen(buffer_rx)+1, 0);  //strlen nos da la longitud de una cadena
      if(err == -1){
         printf ("error en mq_send()");
         exit(-1);}
   } 

// ----------------------------------

   return 0;   
}