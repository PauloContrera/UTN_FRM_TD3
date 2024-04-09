/////////////////////////EJERICIO 10//////////////////////////////////////////////////////////////
/*
Dos hilos (A y B) de un proceso se comunican por medio de una Tuberia.
El hilo A escribe en la Tuberia "0123456789" en forma atomica.
El hilo B escribe en la misma Tuberia "ABCDEFGHIJ" en forma atomica.
El hilo main lee los mensajes desde esta Tuberia y muestra lo leido en consola.
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

// Compilar usando gcc -o ex10 ex10.c -lpthread

//PIPE
#define BUFF_SIZE 80
#define MENSAJE1 "0123456789"
#define MENSAJE2 "ABCDEFGHIJ"
int ipc[2];

pthread_mutex_t mtx=PTHREAD_MUTEX_INITIALIZER; // inicializacion estatica del mutex

void *hiloA(){ 
   int s=0;
   sleep(2);
	
   s=pthread_mutex_lock(&mtx);         
   if (s!=0){
	printf("ERROR; pthread_mutex() = %d\n", s);
	exit(-1);   }

   printf("\nHilo A decrementa mutex\n");
   
   write (STDOUT_FILENO, "\nHilo A escribiendo\n", sizeof("\nHilo A escribiendo\n"));
   write(ipc[1], MENSAJE1, sizeof(MENSAJE1)); // Zona crítica
   
   printf("\nHilo A incrementa mutex\n");

   s=pthread_mutex_unlock(&mtx);      
   if (s!=0){
	printf("ERROR; pthread_mutex() = %d\n", s);
	exit(-1);    }

   pthread_exit(NULL);
}

void *hiloB(){ 
   int s=0;
   sleep(2);
	
   s=pthread_mutex_lock(&mtx);         
   if (s!=0){
	printf("ERROR; pthread_mutex() = %d\n", s);
	exit(-1);   }

   printf("\nHilo B decrementa mutex\n");
   
   write (STDOUT_FILENO, "\nHilo B escribiendo\n", sizeof("\nHilo B escribiendo\n"));
   write(ipc[1], MENSAJE2, sizeof(MENSAJE2)); // Zona crítica
   
   printf("\nHilo B incrementa mutex\n");

   s=pthread_mutex_unlock(&mtx);      
   if (s!=0){
	printf("ERROR; pthread_mutex() = %d\n", s);
	exit(-1);    }

   pthread_exit(NULL);
}


int main(int argc , char const * argv[])
{  

   pthread_t hilo[2];  //creamos los dos hilos
   int rc ;
   int leido;
   char buff[80];
   printf("Soy el Proceso A: %d\n ",getpid());
   
   //Procesos
   
   pid_t pid;
   pipe(ipc);
   rc = pthread_create(&hilo[0], NULL, hiloA , NULL  ); //creamos el hilo A
   if (rc){
   	printf("ERROR; pthread_create() = %d\n", rc);
   	exit(-1);}
   
   rc = pthread_create(&hilo[1], NULL, hiloB , NULL  ); //creamos el hilo B
   if (rc){
   	printf("ERROR; pthread_create() = %d\n", rc);
   	exit(-1);}
        
   pthread_join(hilo[0],NULL);
   pthread_join(hilo[1],NULL);
   leido = read(ipc[0], buff, sizeof(buff)); //se lee de la tuberia
   if(leido < 1){	//verifica si hay error
   write (STDOUT_FILENO, "\nError al leer tuberia 1", sizeof("\nError al leer tuberia 1"));
   } else{
   	write (STDOUT_FILENO, "\nLeido de la tuberìa\n", sizeof("\nLeido de la tuberìa\n"));
   	write (STDOUT_FILENO, buff, leido-1);
   	write (STDOUT_FILENO, "\n",1);
   }  
  pthread_exit(NULL);
  exit(0);
 }
