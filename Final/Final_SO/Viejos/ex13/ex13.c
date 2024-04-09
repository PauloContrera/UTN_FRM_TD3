/////////////////////////EJERICIO 13//////////////////////////////////////////////////////////////
/*
Tres hilos (main, A y B) de un proceso se comunican por medio de una Cola de Mensaje.
1) El hilo A escribe en la Cola de Mensaje "0123456789" en forma atomica y termina.
2) El hilo B espera que el hilo A termine
3) El hilo B escribe en la Cola de Mensaje "ABCDEFGHIJ" en forma atomica y termina.
4) El hilo main espera que termine el Hilo B.
5) El hilo main lee los mensajes de la cola de mensajes muestra lo leido y elimima la cola de mensajes.
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
#include <pthread.h>

// Compilar usando gcc -o ex13 ex13.c -lpthread -lrt

//Cola de mensajes
#define MENSAJE1 "0123456789"
#define MENSAJE2 "ABCDEFGHIJ"
#define MQ_PATH "/MQ_TD3" 
int err, leido;
char buff[1024];   
mqd_t mqd; 		//descriptor
struct mq_attr attr, attr_rcv;
int a,estado;

pthread_t hilo[2]; //declaramos los dos hilos

void *hiloA(){ 
   printf("\nHilo A: Escribiendo\n");
   //enviamos por la cola de mensaje
   err = mq_send(mqd, MENSAJE1, strlen(MENSAJE1)+1, 1);  //strlen nos da la longitud de una cadena
      if(err == -1){
         printf ("Hilo A: error en mq_send()");
         exit(-1);}
 
      printf("Hilo A: Mensaje enviado\n");
      printf("Hilo A: Termino\n");

   pthread_exit(NULL);
}

void *hiloB(){ 
   
   pthread_join(hilo[0],NULL); //espera a que termine el hilo A
	
   printf("\nHilo B: Escribiendo\n");
   //enviamos por la cola de mensaje
   err = mq_send(mqd, MENSAJE2, strlen(MENSAJE2)+1, 1);  //strlen nos da la longitud de una cadena
      if(err == -1){
         printf ("Hilo A: error en mq_send()");
         exit(-1);}
 
      printf("Hilo B: Mensaje enviado\n");
      printf("Hilo B: Termino\n");

   pthread_exit(NULL);
}


int main(int argc , char const * argv[])
{  

   pthread_t hilo[2];  //creamos los dos hilos
   int rc ;
   int leido;
   char buff[80];
   printf("Soy el Hilo main: %d\n ",getpid());
   //creamos la cola de mensajes
   attr.mq_msgsize = sizeof(buff); //tamaño maximo de mensaje (en bytes)
   attr.mq_maxmsg = 5; //numero maximo de mensajes en la cola
   mqd = mq_open(MQ_PATH, O_RDWR | O_CREAT, 0666, &attr); //creacion de la cola en modo lectura/escritura
   
   //Creacion de los hilos
   
   pid_t pid;
   rc = pthread_create(&hilo[0], NULL, hiloA , NULL  ); //creamos el hilo A
   if (rc){
   	printf("ERROR; pthread_create() = %d\n", rc);
   	exit(-1);}
   
   rc = pthread_create(&hilo[1], NULL, hiloB , NULL  ); //creamos el hilo B
   if (rc){
   	printf("ERROR; pthread_create() = %d\n", rc);
   	exit(-1);}
        
   pthread_join(hilo[1],NULL); //esperamos a que termine el hilo B, que a su vez espera al hilo A
   
   // Se lee de cola de mensajes
   for(int i = 0; i < 2; i++){
   leido = mq_receive(mqd, buff, attr.mq_msgsize, 0);  
   if(leido == -1){
 	printf ("error en mq_receive()\n");
 	exit(-1);
	}else{
   	 write (STDOUT_FILENO, "\nMain: Leido de la cola: ", sizeof("\nMain: Leido de la cola: "));
         write (STDOUT_FILENO, buff, leido-1);
         write (STDOUT_FILENO, "\n", 1);
   }
   }
   printf("\nMain: Termino\n");
   pthread_exit(NULL);
   exit(0);
 }
