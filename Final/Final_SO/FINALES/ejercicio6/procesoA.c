/////////////////////////EJERICIO 6//////////////////////////////////////////////////////////////
/*
Cuatro procesos (A,B,C,D) relacionados de la siguiente forma:
A padre de B y padre de C

Los procesos se comunican de la siguiente forma:
A y B se comunican por medio de una FIFO
A y C se comunican por medio de una FIFO
A y D se comunican por medio de una cola de mensajes

1) El proceso A crea la cola de mensajes, crea al proceso B y al proceso C 
2) El proceso B cuando recibe la señal SIGUSR1 escribe en el FIFO1 el mensaje "0123456789" y termina.
3) El proceso C cuando recibe la señal SIGUSR2 escribe en el FIFO2 el mensaje "ABCDEFGHIJ" y termina.
4) El proceso A espera a que termine el proceso B, lee el FIFO1 y lo leído lo envía a la cola de mensajes 
5) El proceso A espera a que termine el proceso C, lee el FIFO2 y lo leído lo envía a la cola de mensajes 
6) El proceso D lee los mensajes de la cola mensajes (2 mensajes) y los muestra en pantalla

El proceso A debe ser el primero en comenzar en una consola
El proceso D comienza luego de A otra consola
Las señales se envían desde otra consola


       --- 
      | B |
       --- 
       |     -------      ---      --------      ---      --------  
       |--->| FIFO1 |--->|   |    | Cola   |    |   |    |        |  
       |     -------     | A |--->|Mensajes|--->| D |--->|pantalla|
       |     -------     |   |--->|        |--->|   |--->|        | 
       |--->| FIFO2 |--->|   |     --------      ---      --------  
       |     -------      ---
       ---              
      | C |
       --- 
*/
/////////////////////////EJERICIO SI TENEMOS TIEMPO //////////////////////////////////////////////////////////////
/* Con SIGUSR1 se produce y con SIGUSR2 se consume, el consumidor cada vez que tiene un dato lo envia mediante tcp */


// COMANDO PARA COMPILAR: gcc -o procesoA procesoA.c -lrt

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



// COLA DE MENSAJES
#define MQ_PATH "/MQ_TD3" 
int err, leido;
char buff[1024];    
mqd_t mqd; 
struct mq_attr attr, attr_rcv;
// ---------------------------------

// FIFO
#define MENSAJE_B "0123456789"
#define MENSAJE_C "ABCDEFGHIJ"
#define FIFO_PATH1 "/tmp/MI_FIFO_1"
#define FIFO_PATH2 "/tmp/MI_FIFO_2"
int err, fifo_d_1, fifo_d_2,lee;
int leido;
// ----------------------------------

void manejador_senial_1(int a)
{
   write(STDOUT_FILENO,"\nPROCESO B ESCRIBIENDO EN PIPE\n",sizeof("\nPROCESO B ESCRIBIENDO EN PIPE\n")); 
   strncpy(buff, MENSAJE_B, sizeof(MENSAJE_B)); 
   err = write(fifo_d_1, buff, sizeof(buff));
   if(err == -1) {
      write(STDOUT_FILENO, "\nProceso B: Error al escribir en FIFO", sizeof("\nProceso B: Error al escribir en FIFO"));
   }
   write(STDOUT_FILENO,"\nPROCESO B TERMINADO\n",sizeof("\nPROCESO B TERMINADO\n")); 
   exit(0);
}

void manejador_senial_2(int a)
{
   write(STDOUT_FILENO,"\nPROCESO C ESCRIBIENDO EN PIPE\n",sizeof("\nPROCESO C ESCRIBIENDO EN PIPE\n"));
   strncpy(buff, MENSAJE_C, sizeof(MENSAJE_C)); 
   err = write(fifo_d_2, buff, sizeof(buff));
   if(err == -1) {
      write(STDOUT_FILENO, "\nProceso C: Error al escribir en FIFO", sizeof("\nProceso C: Error al escribir en FIFO"));
   }
   write(STDOUT_FILENO,"\nPROCESO B TERMINADO\n",sizeof("\nPROCESO B TERMINADO\n")); 
   exit(0);
}

int main(int argc , char const * argv[])
{

   if (argc != 2){
       argv[1]="2000";
   }
   
   printf ("Soy el proceso A\n");

// PROCESOS

   pid_t pid;

	pid = fork();	
   if(pid == 0){
      printf ("Soy el proceso B, mi pid es %d y mi padre es %d\n", getpid(), getppid());
      // FIFO
      // FIFO puede ser leida, escrita y ejecutada por: 
         err = mkfifo(FIFO_PATH1, 0777); // el resto de los usuarios del sistema.
         if(err == -1) {
            write(STDOUT_FILENO, "\nError al crear FIFO_1, la FIFO_1 ya existe\n", sizeof("\nError al crear FIFO_1, la FIFO_1 ya existe\n"));
         }else {
            write(STDOUT_FILENO, "\nFIFO_1 creado correctamente\n", sizeof("\nFIFO_1 creado correctamente\n"));
         }

               fifo_d_1 = open(FIFO_PATH1, O_WRONLY, 0); // abierto para solo escritura
               if(fifo_d_1 == -1){
                  write(STDOUT_FILENO, "\nError al abrir FIFO_1 \n", sizeof("\nError al abrir FIFO_1 \n"));
                  return -1;
               }
      // ---------------------------------- 
      // SIGNAL
      signal(SIGUSR1, manejador_senial_1);
      while (1){};
   }
   else{
      pid = fork();
      if(pid == 0){
         printf ("Soy el proceso C, mi pid es %d y mi padre es %d\n", getpid(), getppid());
         // FIFO_2
         // FIFO puede ser leida, escrita y ejecutada por: 
            err = mkfifo(FIFO_PATH2, 0777); // el resto de los usuarios del sistema.
            if(err == -1) {
               write(STDOUT_FILENO, "\nError al crear FIFO_2, la FIFO_2 ya existe\n", sizeof("\nError al crear FIFO_2, la FIFO_2 ya existe\n"));
            }else {
               write(STDOUT_FILENO, "\nFIFO_2 creado correctamente\n", sizeof("\nFIFO_2 creado correctamente\n"));
            }

                  fifo_d_2 = open(FIFO_PATH2, O_WRONLY, 0); // abierto para solo escritura
                  if(fifo_d_2 == -1){
                     write(STDOUT_FILENO, "\nError al abrir FIFO_2 \n", sizeof("\nError al abrir FIFO_2 \n"));
                     return -1;
                  }    
         // ----------------------------------
         // SIGNAL
         signal(SIGUSR2, manejador_senial_2);
         while (1){};
      }
   }


// FIFO_1
   
   // FIFO puede ser leida, escrita y ejecutada por: 
   err = mkfifo(FIFO_PATH1, 0777); // el resto de los usuarios del sistema.
   if(err == -1) {
      write(STDOUT_FILENO, "\nError al crear FIFO_1, la FIFO_1 ya existe\n", sizeof("\nError al crear FIFO_1, la FIFO_1 ya existe\n"));
   }else {
      write(STDOUT_FILENO, "\nFIFO_1 creado correctamente\n", sizeof("\nFIFO_1 creado correctamente\n"));
   }

         fifo_d_1 = open(FIFO_PATH1, O_RDONLY, 0); // abierto para solo escritura
         if(fifo_d_1 == -1){
            write(STDOUT_FILENO, "\nError al abrir FIFO_1 \n", sizeof("\nError al abrir FIFO_1 \n"));
            return -1;
         }else {
            write(STDOUT_FILENO, "\nProceso A: FIFO_1 abierto correctamente\n", sizeof("\nProceso A: FIFO_1 abierto correctamente\n"));
         } 

// ----------------------------------

// FIFO_2
   
   // FIFO puede ser leida, escrita y ejecutada por: 
   err = mkfifo(FIFO_PATH2, 0777); // el resto de los usuarios del sistema.
   if(err == -1) {
      write(STDOUT_FILENO, "\nError al crear FIFO_2, la FIFO_2 ya existe\n", sizeof("\nError al crear FIFO_2, la FIFO_2 ya existe\n"));
   }else {
      write(STDOUT_FILENO, "\nFIFO_2 creado correctamente\n", sizeof("\nFIFO_2 creado correctamente\n"));
   }

         fifo_d_2 = open(FIFO_PATH2, O_RDONLY, 0); // abierto para solo escritura
         if(fifo_d_2 == -1){
            write(STDOUT_FILENO, "\nError al abrir FIFO_2 \n", sizeof("\nError al abrir FIFO_2 \n"));
            return -1;
         }else {
            write(STDOUT_FILENO, "\nProceso A: FIFO_2 abierto correctamente\n", sizeof("\nProceso A: FIFO_2 abierto correctamente\n"));
         }  

// ----------------------------------

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

   wait(NULL);
   lee = read(fifo_d_1, buff, sizeof(buff));
   if(lee == -1)
   {
   write(STDOUT_FILENO, "\nProceso A: Error al leer en FIFO", sizeof("\nProceso D: Error al leer en FIFO"));
   }
   
   // Se escribe en el COLA DE MENSAJES
   write(STDOUT_FILENO,"\nESCRIBIENDO EN COLA DE MENSAJES\n",sizeof("\nESCRIBIENDO EN COLA DE MENSAJES\n"));
   err = mq_send(mqd, buff, sizeof(buff), 0);  //strlen nos da la longitud de una cadena
      if(err == -1){
         write(STDOUT_FILENO, "\nProceso A: Error al escribir en cola de mensajes\n", sizeof("\nProceso A: Error al escribir en cola de mensajes\n"));
         exit(-1);
      }

   wait(NULL);
   lee = read(fifo_d_2, buff, sizeof(buff));
   if(lee == -1)
   {
   write(STDOUT_FILENO, "\nProceso A: Error al leer en FIFO", sizeof("\nProceso D: Error al leer en FIFO"));
   }

    // Se escribe en el COLA DE MENSAJES
   write(STDOUT_FILENO,"\nESCRIBIENDO EN COLA DE MENSAJES\n",sizeof("\nESCRIBIENDO EN COLA DE MENSAJES\n"));
   err = mq_send(mqd, buff, sizeof(buff), 1);  //strlen nos da la longitud de una cadena
   if(err == -1){
      write(STDOUT_FILENO, "\nProceso A: Error al escribir en cola de mensajes\n", sizeof("\nProceso A: Error al escribir en cola de mensajes\n"));
      exit(-1);
   }
   
   //------------cierra el fifo y tuberías--------//
   mq_close(mqd);
   close(fifo_d_1);    
	unlink(FIFO_PATH1);    //si la FIFO existe la borro 
   close(fifo_d_2);    
	unlink(FIFO_PATH2);    //si la FIFO existe la borro  
// ----------------------------------

   return 0;   
}
