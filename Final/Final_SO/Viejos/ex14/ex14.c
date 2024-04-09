/////////////////////////EJERICIO 14//////////////////////////////////////////////////////////////
/*
Tres procesos (A, B y C) se comunican por medio de una Cola de Mensaje.
1) El proceso C envia una señal (SIGUSR1) al proceso A. El proceso A escribe en la Cola de Mensaje "0123456789" y termina
2) El proceso C espera a que termine el procesos A, lee de la Cola de Mensaje y muestra lo leido.
3) El proceso C envia una señal (SIGUSR2) al proceso B. El proceso B escribe en la Cola de Mensaje "ABCDEFGHIJ" y termina.
4) El proceso C espera a que termine el procesos B, lee de la Cola de Mensaje, muestra lo leido y elimina la cola de mensajes.

Considerar los procesos con la siguiente jerarquia: Proceso C (padre), Proceso B (hijo1 de C), Proceso A (hijo2 de C)
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>    // Define pid_t
#include <unistd.h>       // Define fork, getpid y getppid
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h> 

//compilar con: gcc -o ex14 ex14.c -lrt

#define MENSAJE1 "0123456789"
#define MENSAJE2 "ABCDEFGHIJ"
#define MQ_PATH "/MQ_TD3" 
int err, leido;
char buff[1024];   
mqd_t mqd; 		//descriptor
struct mq_attr attr, attr_rcv;
int a,estado;

void escribe_A(int a){

   printf("\nProceso A, mi PID es %d\n",getpid());
   //abrimos la cola de mensajes como solo escritura
   mqd = mq_open(MQ_PATH, O_WRONLY | O_CREAT, 0666, &attr); //creacion de la cola en modo solo escritura
   if (mqd < 0) {
      	printf ("Proceso A: error en mq_open()");      
      	exit(-1) ;}
   err = mq_send(mqd, MENSAJE1, strlen(MENSAJE1)+1, 1);  //strlen nos da la longitud de una cadena
   if(err == -1){
         printf ("Proceso A: error en mq_send()");
         exit(-1);}
 
   printf("\nProceso A: Mensaje enviado\n");
   exit(0);

}

void escribe_B(int a){

   printf("\nProceso B, mi PID es %d\n",getpid());
   //abrimos la cola de mensajes como solo escritura
   mqd = mq_open(MQ_PATH, O_WRONLY | O_CREAT, 0666, &attr); //creacion de la cola en modo solo escritura
   if (mqd < 0) {
      	printf ("Proceso B: error en mq_open()");      
      	exit(-1) ;}
   err = mq_send(mqd, MENSAJE2, strlen(MENSAJE2)+1, 1);  //strlen nos da la longitud de una cadena
   if(err == -1){
         printf ("Proceso B: error en mq_send()");
         exit(-1);}
 
   printf("\nProceso B: Mensaje enviado\n");
   exit(0);

}

int main (){
	
	//señales
	signal(SIGUSR1, escribe_A);
   	signal(SIGUSR2, escribe_B);
	// Definicion de parametros de la cola de mensajes
	attr.mq_msgsize = sizeof(buff); //tamaño maximo de mensaje (en bytes)
   	attr.mq_maxmsg = 5; //numero maximo de mensajes en la cola
	
	pid_t pid;
	pid = fork();
	switch(pid) {
		case(0):
			while(1);
		break;
		
		case(-1):
			printf ("Proceso A: Error, no se creo el proceso B");
			return(-1);
		break;
		
		default:
			printf ("Proceso C, mi PID es %d\n", getpid());

   			mqd = mq_open(MQ_PATH, O_RDONLY | O_CREAT, 0666, &attr); //creacion de la cola en modo solo lectura
   			if (mqd < 0) {
      			printf ("Proceso C: error en mq_open()");      
      			exit(-1) ;}

   			printf("Proceso C: Cola de mensajes creada\n");
   		
   			kill(pid, SIGUSR1); // Envio de señal al proceso A
   		
   			wait(NULL); //esperamos a que termine el proceso A
   		
   			// Se lee de cola de mensajes
   			leido = mq_receive(mqd, buff, attr.mq_msgsize, 0);  
			if(leido == -1){
		 	printf ("error en mq_receive()\n");
		 	exit(-1);
			}
   			printf("\nProceso C: Mensaje leido: %s\n",buff);
   			//-----------------------------------------------//
   		
   			//ahora tenemos que comunicar el proceso B y C, primero lo tenemos que crear
   		
   			pid = fork();
   			switch(pid) {
   				case(-1):
					printf ("Proceso A: Error, no se creo el proceso B");
					return(-1);
				break;
				
				case(0):
					while(1);
				break;
				
				default:
					printf ("\nProceso C de nuevo\n");
		   		
		   			kill(pid, SIGUSR2); // Envio de señal al proceso B
		   		
		   			wait(NULL); //esperamos a que termine el proceso B
		   		
		   			// Se lee de cola de mensajes
		   			leido = mq_receive(mqd, buff, attr.mq_msgsize, 0);  
					if(leido == -1){
				 		printf ("error en mq_receive()\n");
				 		exit(-1);
						}
						
		   			printf("\nProceso C: Mensaje leido: %s\n",buff);
		   			
		   			printf("Proceso C: borrando cola de mensajes\n");
                  			mq_unlink(MQ_PATH); // Se borra la cola
                  			exit(0);
                  		break;
   			}
   			exit(0);
		break;
	}
	exit(0);
}

