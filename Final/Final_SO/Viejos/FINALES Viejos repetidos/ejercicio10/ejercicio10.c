/////////////////////////EJERICIO 10//////////////////////////////////////////////////////////////
/*
Dos hilos (A y B) de un proceso se comunican por medio de una Tuberia.
El hilo A escribe en la Tuberia "0123456789" en forma atomica.
El hilo B escribe en la misma Tuberia "ABCDEFGHIJ" en forma atomica.
El hilo main lee los mensajes desde esta Tuberia y muestra lo leido en consola.
*/

// Comando: gcc -o ejercicio10 ejercicio10.c -lpthread

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define MENSAJE_A "0123456789"
#define MENSAJE_B "ABCDEFGHIJ"

int ipc[2];

pthread_mutex_t mtx=PTHREAD_MUTEX_INITIALIZER; // inicializacion estatica del mutex

void *hiloA()
{
   int s = 0;
   //sleep(2);

   s=pthread_mutex_lock(&mtx);  
	if (s!=0){
      printf("ERROR; pthread_mutex() = %d\n", s);
      exit(-1);   }
    
    write (STDOUT_FILENO, "\nHILO A ESCRIBIENDO", sizeof("\nHILO A ESCRIBIENDO"));
    write(ipc[1], MENSAJE_A, sizeof(MENSAJE_A)); // Zona crítica

   s=pthread_mutex_unlock(&mtx);  
	if (s!=0){
      printf("ERROR; pthread_mutex() = %d\n", s);
      exit(-1);   }

   pthread_exit(NULL);
}

void *hiloB ()
{
   int s = 0;
   //sleep(2);
   
   s=pthread_mutex_lock(&mtx);  
	if (s!=0){
      printf("ERROR; pthread_mutex() = %d\n", s);
      exit(-1);   }

    write (STDOUT_FILENO, "\nHILO B ESCRIBIENDO", sizeof("\nHILO B ESCRIBIENDO"));
    write(ipc[1], MENSAJE_B, sizeof(MENSAJE_B)); // Zona crítica

   s=pthread_mutex_unlock(&mtx);  
	if (s!=0){
      printf("ERROR; pthread_mutex() = %d\n", s);
      exit(-1);   }
   
   pthread_exit(NULL);
}

int main()
{
   pthread_t hilo[2];
   int rc, leido;
   char buff[80];

    pipe(ipc);
   
    rc = pthread_create(&hilo[0], NULL, hiloA, NULL );
    if (rc){
        printf("ERROR; pthread_create() = %d\n", rc);
        exit(-1);
    }

    rc = pthread_create(&hilo[1], NULL, hiloB, NULL );
    if (rc){
        printf("ERROR; pthread_create() = %d\n", rc);
        exit(-1);
    }

    pthread_join(hilo[0],NULL);
    pthread_join(hilo[1],NULL);
    
    leido = read(ipc[0], buff, sizeof(buff));
    if(leido < 1){
            write (STDOUT_FILENO, "\nError al leer tuberia", sizeof("\nError al leer tuberia"));
        }else {

            // Escribir en consola
            write (STDOUT_FILENO, "\nLeido de la tuberia: ", sizeof("\nLeido de la tuberia: "));
            write (STDOUT_FILENO, buff, leido-1);
            write (STDOUT_FILENO, "\n", sizeof("\n"));
        }
    

    pthread_exit(NULL);

      
   exit(0);
}
