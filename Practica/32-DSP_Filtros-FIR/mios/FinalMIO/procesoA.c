/*
**Ejercicio de Programación en C: Comunicación Entre Procesos**

Este ejercicio involucra tres procesos — A, B y C — que se comunican utilizando FIFO y socket datagram. Los procesos deben interactuar de la siguiente manera:

*- **Proceso C (Servidor Socket Datagram):** 
  - Inicia primero y establece un servidor socket datagram.

- **Proceso A (Padre, Creador de FIFO y Cliente Socket Datagram):** 
  - Este proceso inicia en segundo lugar.
  - Crea una FIFO para comunicarse con el proceso B (su proceso hijo).
  - Responde a la señal  SIGUSR1 enviada por el proceso B escribiendo el mensaje "0123456789" en la FIFO.
  - Actúa como cliente de un socket datagram, enviando el mensaje "ABCDEFGHIJ" al proceso C.

- **Proceso B (Hijo, Lector de FIFO):**
  - Es el proceso hijo de A.
  - Espera y lee un mensaje de la FIFO. Al recibirlo, lo muestra en pantalla y luego termina.

- **Secuencia de Eventos y Señales:**
  - El proceso C debe comenzar primero en una consola.
  - El proceso A se inicia en una segunda consola después de C.
  - El proceso B, siendo el hijo de A, se inicia como parte del proceso A.
  - El proceso C, al recibir la señal SIGUSR2, muestra en pantalla lo que ha leído del socket y luego termina.

**Diagrama de Comunicación:**

```
SIGUSR1
  |
 ---      ---------------      ---      --------  
|   |--->|     FIFO      |--->| B |--->|Pantalla|  
| A |     ---------------      ---      --------
|   |     ---------------      ---      --------  
|   |--->|SOCKET DATAGRAM|--->| C |--->|Pantalla|  
 ---      ---------------      ---      --------
                                |
                             SIGUSR2
```

**Instrucciones de Compilación y Ejecución:**
- Compilar el programa usando: `gcc -o name name.c -lpthread -lrt`
- Ejecutar el programa con: `./name`

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include<sys/wait.h> 
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
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
#include <netinet/in.h>
#include <unistd.h>
#include <wait.h>
#include <arpa/inet.h> 

//*VARIABLES FIFO
#define MENSAJE "0123456789"
#define FIFO_PATH "/tmp/MI_FIFO"
 int err, fifo_d;
   char buff[80];
   int leido;

//*VARIABLES SOCKET
//Creo el descriptor de socket
int socketCliente;
//Creo estructura direccion
struct sockaddr_in direccion = {};
//Variable auxiliares
int connectedSocket;
int recievedChars = 0;
char bufferSocket[50];
char IP[] = {"127.0.0.1"};


//VARIABLES IPC PIPE
int ipcPipe[2];
//int recievedChars = 0;
char recieveBufferPipe[50] = {0};  




//Padre
void mnjs1(int a){
	  write (STDOUT_FILENO, "Padre Entro\n", sizeof("Padre Entro\n"));

  fifo_d = open(FIFO_PATH, O_WRONLY, 0);
         if(fifo_d == -1){
            write(STDOUT_FILENO, "Padre, error al abrir FIFO\n", sizeof("Padre: error al abrir FIFO\n"));
            return -1;
         }else {
            write(STDOUT_FILENO, "Padre, FIFO abierta correctamente\n", sizeof("Padre: FIFO abierta correctamente\n"));
         }

         // Se escribe en el FIFO
         err = write(fifo_d, MENSAJE, sizeof(MENSAJE));
         if(err == -1) {
            write(STDOUT_FILENO, "Padre, FIFO vacia\n", sizeof("Padre, FIFO vacia\n"));
         } else {
            write(STDOUT_FILENO, "Padre, escrito MENSAJE en FIFO\n", sizeof("Padre, escrito MENSAJE en FIFO\n"));
         }
         
         close(fifo_d);
         
         wait(NULL);   
	
}

	
	
int main(int argc, const char *argv[]) {
	
	int i = 0;
	
	if (argc != 2){
		argv[1]="2000";
	} 
		  signal(SIGUSR1, mnjs1);
	
	
	printf ("-------------->Programa A<--------------\n");
	
	//***************************************************************************//		
	//*********************************** FIFO ***********************************//	 
	//***************************************************************************//		


   unlink(FIFO_PATH);    // si la FIFO existe la borro

   // FIFO puede ser leida, escrita y ejecutada por:
   err = mkfifo(FIFO_PATH, S_IRUSR | S_IWUSR | S_IXUSR|  // el usuario que la creo
                           S_IRGRP | S_IWGRP | S_IXGRP|  // el grupo al que pertenece el usuario
                           S_IROTH | S_IWOTH | S_IXOTH); // el resto de los usuarios del sistema

   //err = mkfifo(FIFO_PATH, 0777);
   if(err == -1) {
      write (STDOUT_FILENO, "Error al crear FIFO, la FIFO ya existe\n", sizeof("Error al crear FIFO, la FIFO ya existe\n"));
   }else {
      write (STDOUT_FILENO, "FIFO creada correctamente\n", sizeof("FIFO creada correctamente\n"));
   }
	//***************************************************************************//		 
	//********************************* Socket **********************************//	 
	//***************************************************************************//	
	
  	//************************************ 1 ***********************************//
	
	//-- socket(): Crear el socket DATAGRAMA----------//
	socketCliente = socket(AF_INET, SOCK_DGRAM, 0);
	
	if (socketCliente == -1) { 
		printf ("ERROR en funcion socket()\n");
		exit(-1);     } 
	
	printf ("Paso 1 Socket: Se creo socket cliente\n");
	
	//************************************ 2 ***********************************//
	
	//-- preparar el address:port del host servidor STREAM ------//
	//Tipo de red
	direccion.sin_family = AF_INET;
	
	//Transformo el puerto
	direccion.sin_port = htons(atoi(argv[1])); 
	
	//inet_aton("localhost", &direccion.sin_addr); //convierte la direccion de Internet a una cadena de caracteres en la notacion estandar, Localhost = 127.0.0.1
	inet_aton(IP, &direccion.sin_addr); //convierte la direccion IP a una cadena de caracteres en la notacion estandar como el servidor esta en el mismo host usamos Localhost = 127.0.0.1
	
	
	printf ("Cliente va a conectarse con IP:%s Puerto: %s\n", IP, argv[1]);
	


	
	//***************************************************************************//		
	//********************************** FORK ***********************************//	 
	//***************************************************************************//		
	
	switch (fork())
	{
	case -1:   // No pudo crear el proceso
		printf ("Error. No se crea proceso hijo B");
		break;
		
	case 0:   //*proceso hijo


		sleep(1);
		printf ("Soy el proceso hijo C, pid %d\n", getpid());
		
  
	 // Se abre FIFO	
         fifo_d = open(FIFO_PATH, O_RDONLY, 0); // O_NONBLOCK
         if(fifo_d == -1){
            write (STDOUT_FILENO, "Hijo, error al abrir FIFO\n", sizeof("Hijo: error al abrir FIFO\n"));
            return -1;
         }else {
            write (STDOUT_FILENO, "Hijo, FIFO abierta correctamente\n", sizeof("Hijo: FIFO abierta correctamente\n"));
         }

         // Se lee FIFO
         leido = read(fifo_d, buff, sizeof(buff));
         if(leido < 1){
            write(STDOUT_FILENO, "Hijo, FIFO vacia\n", sizeof("Hijo, FIFO vacia\n"));
         }else {
            write(STDOUT_FILENO, "Hijo, leido de la FIFO \"", sizeof("Hijo, leido de la FIFO \""));
            write(STDOUT_FILENO, buff, leido-1);
            write(STDOUT_FILENO, "\"\n", sizeof("\"\n"));
         }
		
		//Cierra lectura lado hijo
		close(fifo_d);

         exit(0);
		printf ("Cierro lectura FIFO hijo C\n");

		 
       


		
		while(1);
		
		break;
		
	default:  //proceso padre
		break;
	}
	
//*proceso padre
	sleep(2);
	

	//***************************************************************************//		
	//********************************** Envio por FIFO ***********************************//	 
	//***************************************************************************//		
	  write (STDOUT_FILENO, "Padre inicia\n", sizeof("Padre inicia\n"));
      
         sleep(2);
         	//Si recibo la señal SIGUSR1 voy al manejador mnjs1
		

         






  

	//***************************************************************************//		
	//********************************** Envio por datagrama ***********************************//	 
	//***************************************************************************//		
	
		/*recievedChars = read(ipcPipe[0], recieveBufferPipe, sizeof(recieveBufferPipe));
			printf("Padre lee de la tuberia: %s\n",recieveBufferPipe);*/





			//Env�o por datagrama
			/*
      sendto(socketCliente , recieveBufferPipe, sizeof(recieveBufferPipe), 0, (struct sockaddr *) &direccion, sizeof(direccion)); 
			
			printf("Mensaje enviado por Socket DATAGRAMA a C: %s\n", recieveBufferPipe); 
	*/
	
	
	//Cierro conexion de datagrama
	close(socketCliente);
	
	exit(0);
}
		
		
		
		
		
		
		