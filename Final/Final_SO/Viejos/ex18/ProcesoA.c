/////////////////////////EJERICIO 18//////////////////////////////////////////////////////////////

/*
Tres procesos (A,B,C) se comunican de la siguiente forma:
C y A se comunican por medio de un pipe
C y B se comunican por medio de una cola de mensajes

1) El proceso C crea el pipe 
2) El proceso C crea al proceso A.
3) El proceso C crea la cola de mensajes
4) El proceso C cuando recibe la se�al SIGUSR1 escribe en el pipe "0123456789" 
5) El proceso C cuando recibe la se�al SIGUSR2 escribe en la cola de mensajes "ABCDEFGHIJ" 
6) El proceso A lee el pipe y muestra en pantalla lo le�do
7) El proceso B abre la cola de mensajes
8) El proceso B lee el mensaje de la cola de mensajes y muestra en pantalla lo le�do

El proceso C es el encargado de crear el pipe y la cola de mensajes, es el primer proceso que se ejecuta.
Las se�ales se env�an desde otra consola

 ---      -----------------      ---      --------  
|   |--->|     pipe        |--->| A |--->|Pantalla|  
| C |     -----------------      ---      --------
|   |     -----------------      ---      --------  
|   |--->|Cola de mensajes |--->| B |--->|Pantalla|  
 ---      -----------------      ---      --------
 
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

//compilar con: gcc -o ProcesoA ProcesoA.c -lrt

//PIPE
int ipc[2], proc;
int leido, leido2, success;
char buff[BUFF_SIZE] = {0};
 
//cola de mensajes
#define MQ_PATH "/MQ_TD3"
int err, leido;
mqd_t mqd; 		//descriptor
struct mq_attr attr, attr_rcv;

void escribe_mensaje1(int a){ 
   
   printf("\nProceso C: Escribiendo en la tuberia\n");
   write(ipc[1], MENSAJE1, sizeof(MENSAJE1));	//escribe en tuberia 1
   
}

void escribe_mensaje2(int a){ 

   //-- Escribe en cola de mensajes --------------------------
   err = mq_send(mqd, MENSAJE2, strlen(MENSAJE2)+1, 1);  //strlen nos da la longitud de una cadena
   if(err == -1){
        write(STDOUT_FILENO, "\nerror en mq_send()\n", sizeof("\nerror en mq_send()\n"));  }
   else {
        write(STDOUT_FILENO, "\nProceso C: Mensaje enviado por cola de mensaje\n", sizeof("\nProceso C: Mensaje enviado por cola de mensaje\n"));   }

}


int main(int argc , char const * argv[])
{  

   signal(SIGUSR1, escribe_mensaje1);  //mensaje1 en pipe
   signal(SIGUSR2, escribe_mensaje2);  //mensaje2 en cola de mensajes
   
   printf("Proceso C: PID: %d\n",getpid());
   
   pipe(ipc); //abrimos la tuberia que va a usar el proceso A 
   pid_t pid;
   
   //cola de mensajes

   mq_unlink(MQ_PATH);

   attr.mq_msgsize = sizeof(buff);
   attr.mq_maxmsg = 5;
   
   // abrir y crear cola de mensajes
    mqd = mq_open(MQ_PATH, O_WRONLY | O_CREAT , 0666, &attr);
   if (mqd < 0) {
      printf ("error en mq_open()");   
      exit(-1);  }

   printf("Proceso C: Cola de mensajes creada\n");
   //-----------------------------------------------------------------
   
   pid = fork(); //creamos el proceso A
   if(pid == 0){
   	printf("Proceso A: PID = %d\n",getpid());
   	//solo deja abierta la lectura de la ipc, lo demas lo cerramos porque no nos interesa
   	close(ipc[1]);  //cierra escritura tuberia
   }
   leido = read(ipc[0], buff, sizeof(buff)); //se lee de la tuberia
   if(leido < 1){	//verifica si hay error
   write (STDOUT_FILENO, "\nError al leer tuberia", sizeof("\nError al leer tuberia"));
   }else {
      write (STDOUT_FILENO, "\nProceso A: Leido de la tuberia: ", sizeof("\nProceso A: Leido de la tuberia: "));
      write (STDOUT_FILENO, buff, leido-1);
      write (STDOUT_FILENO, "\n", sizeof("\n"));

   while(1);	//para que no termine

   }
   close(ipc[0]); //padre cierra lectura tuberia
   
   wait(NULL); //espera a que termine el proceso B

}
