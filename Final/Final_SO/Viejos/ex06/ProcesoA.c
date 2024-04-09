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

//FIFO
#define FIFO_PATH1 "/tmp/MI_FIFO1"	//direccion fifo 1
#define FIFO_PATH2 "/tmp/MI_FIFO2"	//direccion fifo 2
#define MENSAJE1 "0123456789"
#define MENSAJE2 "ABCDEFGHIJ"
int err, fifo_d1,fifo_d2;
char buff[80];
int leido;

//cola de mensajes
#define MQ_PATH "/MQ_TD3"
int err, leido;
mqd_t mqd; 		//descriptor
struct mq_attr attr, attr_rcv;

void escribe_mensaje1(int a){ 

   write(STDOUT_FILENO, "\nProceso B escribe en FIFO 1\n", sizeof("\nProceso B escribe en FIFO 1\n"));
   strncpy(buff, MENSAJE1, sizeof(MENSAJE1)); //copia contenido de MENSAJE1 a buff
   err = write(fifo_d1, buff, sizeof(buff));  //escribe en fifo 1 el contenido de buff
   if(err == -1) {
      write(STDOUT_FILENO, "\nProceso B: Error al escribir en FIFO 1", sizeof("\nProceso B: Error al escribir en FIFO 1"));
   }
   write(STDOUT_FILENO, "\nProceso B termino\n", sizeof("\nProceso B termino\n"));
   exit(0);

}

void escribe_mensaje2(int a){ 

   write(STDOUT_FILENO, "\nProceso C escribe en FIFO 2\n", sizeof("\nProceso C escribe en FIFO 2\n"));
   strncpy(buff, MENSAJE2, sizeof(MENSAJE2)); //copia contenido de MENSAJE2 a buff
   err = write(fifo_d2, buff, sizeof(buff));  //escribe en fifo 1 el contenido de buff
   if(err == -1) {
      write(STDOUT_FILENO, "\nProceso C: Error al escribir en FIFO 2", sizeof("\nProceso C: Error al escribir en FIFO 2"));
   }
   write(STDOUT_FILENO, "\nProceso C termino\n", sizeof("\nProceso C termino\n"));
   exit(0);

}


int main(int argc , char const * argv[])
{  


   if (argc != 2){
       argv[1]="2000";
   }
   
   printf("Soy el Proceso A: %d \n ",getpid());
   
   //CREACION COLA DE MENSAJES
   
   mq_unlink(MQ_PATH); //si existe la cola de mensajes, la borra

   attr.mq_msgsize = sizeof(buff);
   attr.mq_maxmsg = 5;
   
   // abrir y crear cola de mensajes
    mqd = mq_open(MQ_PATH, O_RDWR | O_CREAT , 0666, &attr); //O_WRONLY: solo escritura | O_CREAT: si no existe la crea la cola de mensajes
   if (mqd < 0) {
      printf ("error en mq_open()");   
      exit(-1);  }

   printf("\nCola de mensajes creada\n");

 
  /*if (mq_getattr(mqd, &attr_rcv) == -1) {		por si pide los atributos
      printf ("error en mq_getattr()");
      exit(-1); }
   printf("Nro max. de mensajes en cola de mensajes: %ld\n", attr_rcv.mq_maxmsg);
   printf("Longitud max. de mensaje: %ld\n", attr_rcv.mq_msgsize);
   printf("Nros de mensajes pendientes en cola de mensajes: %ld\n", attr_rcv.mq_curmsgs); */
   
   //-------------------------------------------------------------------------//
   
   //Procesos B y C
   
   pid_t pid;

   pid = fork(); //creamos el proceso B
   if(pid == 0){
   	printf("\nSoy el proceso B, mi pid %d y el de mi padre %d\n",getpid(), getppid());
   	
   	//FIFO 1 del proceso B
   	
   	// FIFO puede ser leida, escrita y ejecutada por: 
   	err = mkfifo(FIFO_PATH1, 0777); // el resto de los usuarios del sistema.
   	if(err == -1) {
            write(STDOUT_FILENO, "\nProceso B: Error al crear FIFO 1, la FIFO 1 ya existe\n", sizeof("\nProceso B: Error al crear FIFO 1, la FIFO 1 ya existe\n"));
         }else {
            write(STDOUT_FILENO, "\nProceso B: FIFO 1 creado correctamente\n", sizeof("\nProceso B: FIFO 1 creado correctamente\n"));
         }
   	fifo_d1 = open(FIFO_PATH1, O_WRONLY, 0); //fifo abierta solo para escritura
   	signal(SIGUSR1, escribe_mensaje1);  //mensaje1
   	while(1){	//para que no termine
   	}
   }
   else{
   
   pid = fork(); //creamos el proceso C
   if(pid == 0){
   	printf("\nSoy el proceso C, mi pid %d y el de mi padre %d\n",getpid(), getppid());
   	
    	//FIFO 2 del proceso C
    	
   	// FIFO puede ser leida, escrita y ejecutada por: 
   	err = mkfifo(FIFO_PATH2, 0777); // el resto de los usuarios del sistema.
   	if(err == -1) {
            write(STDOUT_FILENO, "\nProceso C: Error al crear FIFO 2, la FIFO 2 ya existe\n", sizeof("\nProceso C: Error al crear FIFO 2, la FIFO 2 ya existe\n"));
         }else {
            write(STDOUT_FILENO, "\nProceso C: FIFO 2 creado correctamente\n", sizeof("\nProceso C: FIFO 2 creado correctamente\n"));
         }
   	fifo_d2 = open(FIFO_PATH2, O_WRONLY, 0); //fifo abierta solo para escritura
   	signal(SIGUSR2, escribe_mensaje2);  //mensaje1
   	while(1){	//para que no termine
   	}
   }
   //abrimos la fifo 1 en el proceso A
   	// FIFO puede ser leida, escrita y ejecutada por: 
   	err = mkfifo(FIFO_PATH1, 0777); // el resto de los usuarios del sistema.
   
   	write(STDOUT_FILENO, "\nProceso A: FIFO 1 creado correctamente\n", sizeof("\nProceso A: FIFO 1 creado correctamente\n"));

   	fifo_d1 = open(FIFO_PATH1, O_RDONLY, 0); //fifo abierta solo para lectura
   
   //abrimos la fifo 2 en el proceso A
      	// FIFO puede ser leida, escrita y ejecutada por: 
   	err = mkfifo(FIFO_PATH2, 0777); // el resto de los usuarios del sistema.
   
   	write(STDOUT_FILENO, "\nProceso A: FIFO 2 creado correctamente\n", sizeof("\nProceso A: FIFO 2 creado correctamente\n"));

   	fifo_d2 = open(FIFO_PATH2, O_RDONLY, 0); //fifo abierta solo para lectura
   
   //--------------------------------------------------------------------------------------
   
   //Leemos las fifos y enviamos los mensajes a la cola de mensajes
   
   wait(NULL); //espera a que termine el proceso B
   
   leido = read(fifo_d1, buff, sizeof(buff)); //se lee de la fifo 1
   if(leido < 1){	//verifica si hay error
   write (STDOUT_FILENO, "\nProceso A: Error al leer FIFO 1", sizeof("\nProceso A: Error al leer FIFO 1"));
   }
   //se escribe el contenido de la FIFO 1 en la cola
   err = mq_send(mqd, buff, strlen(buff)+1, 1);  //strlen nos da la longitud de una cadena
   if(err == -1){
        write(STDOUT_FILENO, "\nerror en mq_send()\n", sizeof("\nerror en mq_send()\n"));  }
   else {
        write(STDOUT_FILENO, "\nProceso A: Mensaje1 enviado\n", sizeof("\nProceso A: Mensaje1 enviado\n"));   }
        
        
   wait(NULL); //espera a que termine el proceso C
   
   leido = read(fifo_d2, buff, sizeof(buff)); //se lee de la tuberia 2
   if(leido < 1){	//verifica si hay error
   write (STDOUT_FILENO, "\nProceso A: Error al leer FIFO 2", sizeof("\nProceso A: Error al leer FIFO 2"));
   }
   //se escribe el contenido de la tuberia 2 en la cola
   err = mq_send(mqd, buff, strlen(buff)+1, 1);  //strlen nos da la longitud de una cadena
   if(err == -1){
        write(STDOUT_FILENO, "\nerror en mq_send()\n", sizeof("\nerror en mq_send()\n"));  }
   else {
        write(STDOUT_FILENO, "\nProceso A: Mensaje2 enviado\n", sizeof("\nProceso A: Mensaje2 enviado\n"));   }
        
  
  
 }

//------------cierrar la conexion --------// 
   write (STDOUT_FILENO, "\nFinaliza el proceso A\n", sizeof("\nFinaliza el proceso A\n"));
   close(fifo_d1);    	  //borramos FIFO 1
   unlink(FIFO_PATH1);    //si la FIFO 1 existe la borro 
   close(fifo_d2);       //borramos FIFO 1
   unlink(FIFO_PATH2);    //si la FIFO 2 existe la borro
   mq_close(mqd);  //cierra cola de mensajes

   return 0;   
}
