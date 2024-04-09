/*
**Ejercicio de Programación en C: Comunicación Entre Procesos**

Este ejercicio involucra tres procesos — A, B y C — que se comunican utilizando FIFO y socket datagram. Los procesos deben interactuar de la siguiente manera:

- **Proceso C (Servidor Socket Datagram):** 
  - Inicia primero y establece un servidor socket datagram.

- **Proceso A (Padre, Creador de FIFO y Cliente Socket Datagram):** 
  - Este proceso inicia en segundo lugar.
  - Crea una FIFO para comunicarse con el proceso B (su proceso hijo).
  - Responde a la señal SIGUSR1 enviada por el proceso B escribiendo el mensaje "0123456789" en la FIFO.
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


#define MENSAJE "0123456789"
#define MENSAJESOCKET "ABCDEFGHIJ"
#define FIFO_PATH "/tmp/MI_FIFO" //!Dirrecion de la fifo 

void signal_handler(int signum) {
        printf("Señal SIGUSR1 recibida\n");
   int err, fifo_d;

    printf("entre");
    fifo_d = open(FIFO_PATH,  O_WRONLY);         
    if(fifo_d == -1){
       write (STDOUT_FILENO, "Error al abrir FIFO\n", sizeof("Error al abrir FIFO\n"));
       return -1;
    }else {
       write (STDOUT_FILENO, "FIFO abierta correctamente\n", sizeof("FIFO abierta correctamente\n"));
    }
err = write(fifo_d, MENSAJE, sizeof(MENSAJE));
   if(err == -1) {
      write(STDOUT_FILENO, "Error al escribir en FIFO\n", sizeof("Error al escribir en FIFO\n"));
   } else {
      write(STDOUT_FILENO, "Mensaje enviado correctamente\n", sizeof("Mensaje enviado correctamente\n"));
   }
   close(fifo_d);
}

    


// *Proceso A y B
int main(int argc, const char *argv[]) {

   int err, fifo_d;
   char buff[80];



// *Boramos la fifo
   if(unlink(FIFO_PATH) == 0) {
      write (STDOUT_FILENO, "FIFO borrada correctamente", sizeof("FIFO borrada correctamente"));
   }else {
      write (STDOUT_FILENO, "No se pudo borrar la fifo", sizeof("No se pudo borrar la fifo"));
   }

// *Creamos la fifo
   //! Puede ser creada por Padre, Hijo, o proceso extra. A su ves puede ser creada en el pasado
   
   if(mkfifo(FIFO_PATH, 0777) == -1) {
      write (STDOUT_FILENO, "Error al crear FIFO\n", sizeof("Error al crear FIFO\n"));
      // Puede que ya exista si hay error
   }else {
      write (STDOUT_FILENO, "FIFO creada correctamente\n", sizeof("FIFO creada correctamente\n"));
   }
   // ~Otro metodo
      // FIFO puede ser leida, escrita y ejecutada por:
      err = mkfifo(FIFO_PATH, S_IRUSR | S_IWUSR | S_IXUSR|  // el usuario que la creo
                           S_IRGRP | S_IWGRP | S_IXGRP|  // el grupo al que pertenece el usuario
                           S_IROTH | S_IWOTH | S_IXOTH); // el resto de los usuarios del sistema



//-- creo los procesos hijo --//
      switch (fork())
      {
            case -1:   // No pudo crear el proceso
                  printf ("Error. No se crea proceso hijo");
                  break;

            case 0:   
            //*Hijo
                    printf ("Proceso Hijo Creado\n"); 
                    fifo_d = open(FIFO_PATH,  O_RDONLY);         
                    if(fifo_d == -1){
                       write (STDOUT_FILENO, "Error al abrir FIFO\n", sizeof("Error al abrir FIFO\n"));
                       return -1;
                    }else {
                       write (STDOUT_FILENO, "FIFO abierta correctamente\n", sizeof("FIFO abierta correctamente\n"));
                    }
         sleep(2);

                int leido;
                    leido = read(fifo_d, buff, sizeof(buff));
                    if(leido < 1){
                       write(STDOUT_FILENO, "FIFO vacia\n", sizeof("FIFO vacia\n"));
                    }else {
                       write(STDOUT_FILENO, "Leido de la FIFO \"", sizeof("Leido de la FIFO \""));
                       write(STDOUT_FILENO, buff, leido-1);
                       write(STDOUT_FILENO, "\"\n", sizeof("\"\n"));
                    }
                    close(fifo_d);
            break;
            default:    
            //*Padre
                // Obtener y mostrar el PID del proceso actual
                pid_t pid = getpid();
                printf("PID del padre: %d\n", pid);

                // Configurar el manejador de señales para SIGUSR1
                signal(SIGUSR1, signal_handler);

                printf("Esperando la señal SIGUSR1 para imprimir un mensaje...\n");

                // Mantener el programa en ejecución
                
            //wait(NULL); //Esperamos que el hijo termine  

             char buff_tcl[256], buff_sck[56];
   struct sockaddr_in direccion={};
   int sockfd, lee_tcl, lee_sck, conn;

   if (argc != 2){
       argv[1]="2000";
   } 

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
//-- envia al socket datos --

         
    ssize_t bytes_enviados = sendto(sockfd, MENSAJESOCKET, strlen(MENSAJESOCKET), 0, (struct sockaddr *) &direccion, sizeof(direccion));
    if (bytes_enviados == -1) {
        perror("Error al enviar los datos");
        exit(EXIT_FAILURE);
    } else {
        printf("Datos enviados con éxito\n");
    }

   close(sockfd);


 

  

//****************** 3 *******************//
//------------cierrar la conexion --------// 

         wait(NULL); //Esperamos que el hijo termine  

   return 0;   























             break;
      }
      
                  
                //while(1);

}