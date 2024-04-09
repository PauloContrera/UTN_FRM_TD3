////////////////////////////////////////////////////////////////////////
/*
Tres procesos A, B y C se comunican de la siguiente forma:

A y B se comunican por medio de una FIFO
B y C se comunican por medio de un socket STREAM

1) El proceso A crea la FIFO. 
2) El proceso C envia la señal SIGUSR1
3) El proceso A al recibir la señal SIGUSR1 escribe en la FIFO un mensaje.
4) El proceso B esta esperando leer un mensaje por la FIFO. 
5) El proceso B al recibir un mensaje lo reenvia al proceso C via socket STREAM.
6) El proceso C al recibir un mensaje, muestra lo leído en pantalla. 

 ---
|   |<------------------------------------------- SIGUSR1	
|   | 												 |
|   |     ---------     ---      --------------     ---      -------- 
|   |--->|  FIFO  |--->| B |--->|socket STREAM|--->| C |--->|Pantalla|  
| A |     ---------     ---      --------------     ---      --------
|   |   			  cliente					  servidor
|   |
 ---    
                        
Compile el programa: gcc proc_a.o -o proc_a -lpthread -lrt

*/

#define _GNU_SOURCE

#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <wait.h>
#include <arpa/inet.h> 
#define FIFO_PATH "/tmp/MI_FIFO"
#define MQ_PATH "/MQ_TD3" 

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define MENSAJE "ENVIADO INFORMACION IMPORTANTE"
	 int err, fifo_d;
int fifo_d1, fifo_d2;
int cont1, cont2;
int leido1, leido2;
	  char buff[80];
         int leido;
	char buff_tcl[256], buff_sck[56], buff1[10], buff2[10];
	int c=0;
	struct sockaddr_in direccion={};
	int sockfd, lee_tcl, lee_sck, conn;
void manj1(){
	write(fifo_d1, MENSAJE, sizeof(MENSAJE));
	write(STDOUT_FILENO,"\nMensaje enviado\n", sizeof("\nMensaje enviado\n"));
	exit(0);
	}
//------------------------------------------------------------//

		
	 
		   
int main(int argc , char const * argv[]) ////La variable "argc" guarda el numero de argumentos que recibe el "main()". Es decir, cuando se ejecuta el programa "cliente.c" mediante ./cliente podemos incluir un argumento para el Numero de Puerto cuyo valor se guardará en la posición 1 del arreglo "argv". Por ejemplo: "./cliente 2000" 
   {
	switch(fork()){ 
	

      case -1:      

         write(STDOUT_FILENO, "\nError al crear B", sizeof("\nError al crear B"));

         return -1;

      break;



      case 0:



         write(STDOUT_FILENO, "\nEntrando proceso A", sizeof("\nEntrando proceso A"));
		printf ("soy proceso A, mi pid es: %d\n", getpid());

         sleep(1);

		unlink(FIFO_PATH);    //si la FIFO existe la borro                   

	   // FIFO puede ser leida, escrita y ejecutada por: 
	   err = mkfifo(FIFO_PATH, 0777); // el resto de los usuarios del sistema.
	   if(err == -1) {
	      write(STDOUT_FILENO, "\nError al crear FIFO, la FIFO ya existe", sizeof("\nError al crear FIFO, la FIFO ya existe"));
	   }else {
	      write(STDOUT_FILENO, "\nFIFO creado correctamente\n", sizeof("\nFIFO creado correctamente\n"));
	   }
         
         signal(SIGUSR1,manj1);
	printf("PID proceso A = %d\n", getpid());
	while(1);
	 break;
	
	default:
   
printf ("soy proceso B, mi pid es: %d\n", getpid());
	// Se lee FIFO
         leido = read(fifo_d1, buff, sizeof(buff));
         if(leido == -1){
            write(STDOUT_FILENO, "\nB: Error al leer en FIFO", sizeof("\nB: Error al leer en FIFO"));
         }else {
            write(STDOUT_FILENO, "\nB: Leido del FIFO: ", sizeof("\nB: Leido del FIFO: "));
            write(STDOUT_FILENO, buff, leido-1);
            write(STDOUT_FILENO, "\n", sizeof("\n"));
         }
        

//---------------- SOCKET ------------------------
   if (argc != 2) //Si el numero de argumentos recibidos por consola es distino de 2, significa que no especificamos ningun Puerto, por lo tanto se asigna el Puerto "2001".
   	{
   	argv[1]="2001";
   	} 

//****************** PASO 1 *******************//
 //-- socket(): Crear el socket -----------//

   sockfd=socket(AF_INET, SOCK_STREAM, 0); //La llamada a socket() devuelve en "sockfd" el descriptor del socket creado en caso de éxito ó -1 en caso de error. El argumento AF_INET (Via IPv4) especifica el "Dominio ó Familia" de la comunicación, el argumento SOCK_STREAM (TCP_con conexión) especifica el "Tipo de Socket" y el arguemnto 0 (por defecto) especifica el "Protocolo".

   if (sockfd==-1) //Si socket() devuelve "-1", hubo un ERROR al crear el socket.
	{ 
       	printf ("ERROR en funcion socket()\n");
   	exit(-1);     
	} 

   printf ("Paso 1: Se creo socket cliente\n");

//****************** PASO 2 *******************//

//-- preparar el address:port del host servidor------//Configuramos los campos de la estructura "direccion" que define la Direccion del socket pasivo (servidor) al que nos queremos conectar.
   direccion.sin_family= AF_INET; //Especifica el "Dominio ó Familia de la Dirección". AF_INET (via IPv4)
   direccion.sin_port=htons(atoi(argv[1])); //Especifica el "Numero de Puerto de Protocolo" al que se asocia el socket servidor.
//   inet_aton("localhost", &direccion.sin_addr); //convierte la direccion de Internet a una cadena de caracteres en la notacion estandar, Localhost = 127.0.0.1
   inet_aton("127.0.0.1", &direccion.sin_addr); //convierte la direccion IP a una cadena de caracteres en la notacion estandar. Como el servidor esta en el mismo host usamos Localhost = 127.0.0.1 y lo asignamos como "Dirección IP de la máquina (host)" a la que se asocia el socket servidor.
                                                                                                
   printf ("Cliente va a conectarse con IP:127.0.0.1 Puerto: %s\n",  argv[1]); //Se imprime el "Numero de Puerto" del socket pasivo (servidor) al que nos vamos a conectar.

//-- conectar el socket activo al socket de escucha --//
   conn=connect(sockfd, (struct sockaddr *)&direccion, sizeof (direccion)); //Se conecta el socket activo cuyo descriptor es "sockfd" al socket pasivo (servidor) cuya dirección está guardada en la estructura "direccion". El argumento "sockfd" especifica el descriptor del socket activo que va a hacer la peticion de conexion, el argumento "&direccion" especifica un puntero a la estructura de direccion del socket pasivo (servidor) y el argumento "sizeof (direccion)" especifica el Tamaño de dicha estructura. La función "connect()" devuelve en la variable "conn" un 0 en caso de éxito ó -1 si hay error.

   if (conn == -1) //Si "connect()" devolvió -1, hubo un error al intentar conectarse con el servidor.
	{   
        printf ("ERROR en funcion connect()\n");
   	exit(-1);     
	}  

   printf ("Paso 2: Connect(), cliente conectado\n");

//****************** PASO 3 *******************//
 
   while(1) //Bucle infinito para el intercambio de datos con el socket pasivo (servidor).
	{ 
   	
     	if (c == 0)
		{
		write (sockfd , buff, strlen(buff)+1); //Escribe en el socket cliente cuyo descriptor es "sockfd" el mensaje definido por "MENSAJE".
	write (STDOUT_FILENO , "\nSe envio MENSAJE al Proceso C\n", 33); //Escribe leyenda en pantalla (STDOUT_FILENO).
		}
	c = 1;
 	}
//****************** 4 *******************//
//------------cierrar la conexion --------// 

  // close(sockfd); //Se cierra el socket cliente cuyo Descriptor es "sockfd".

//******************************************************

	   
 
              
	}
  exit(0);
}

//------------------------------------------------------------//

