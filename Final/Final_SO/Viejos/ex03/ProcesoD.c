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
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>

int des_socket, sck_server, rx_socket, largo, cont;
char buffer_rx[256];
struct sockaddr_in struct_direccion={};
int SockEscucha,SockConexion;

int main(int argc, const char *argv[])      {

   if (argc != 2){
       argv[1]="2000"; } 


 //****************** 1 *******************//
 //-- socket(): Crear el socket -----------//
   SockEscucha=socket(AF_INET, SOCK_STREAM, 0);
   
   if(( SockEscucha)<0) {
       printf ("ERROR en funcion socket()\n");
   exit(-1);     } 
   
   printf ("Paso 1: Servidor creo socket\n");

 //****************** 2 *******************//

//-- preparar el address:port -------------//
   struct_direccion.sin_family = AF_INET;
   struct_direccion.sin_addr.s_addr = htonl(INADDR_ANY);  // asigna una IP de la maquina
   struct_direccion.sin_port = htons(atoi (argv[1]));     // puerto

 //-- bind(): asociamos el socket a la direccion------//

   if (bind (SockEscucha, (struct sockaddr *)&struct_direccion,sizeof(struct sockaddr_in))<0) {
       printf ("ERROR en funcion bind()\n");
   exit(-1);     } 

   printf ("Paso 2: Asociar bind() \n");

 //****************** 3 *******************//
 //-- listen(): Permitir hasta 1 conexion pendiente --//

   if ((listen(SockEscucha, 1))<0) {
       printf ("ERROR en funcion listen()\n");
   exit(-1);     } 

   printf ("Paso 3: Permitir conexiones listen()\n");

  while(1) {

 //****************** 4 *******************//
 //-- accept(): se bloquea hasta que entre una conexion --//

   printf ("\nPaso 4: Bloqueo hasta que entre conexion accept()\n");
   cont=0;     
   SockConexion=accept(SockEscucha, NULL, 0) ;

   if (SockConexion >=0) {  
      if (cont==0) {
           printf ("Desbloqueo de accept, entro conexion: %d\n",SockConexion);
           send (SockConexion ,"Bienvenido al servidor\n", 23,0 ); 
      cont=1;  }   

      while (( rx_socket = read(SockConexion, buffer_rx, sizeof (buffer_rx))) > 0) {  //lee del socket    
              write ( SockConexion ,buffer_rx, rx_socket);         //escribe en socket
              write ( STDOUT_FILENO , "\ncliente:--> ", 12);         //escribe leyenda en pantalla
              write ( STDOUT_FILENO , buffer_rx, rx_socket);      //escribe lo leido del socket
       }
    } else { printf ("Error en la conexion\n");  }
 }

 //****************** 5 *******************//
 //------------cierrar la conexion --------// 

   close(SockConexion); 
 
  return 0;
}  
