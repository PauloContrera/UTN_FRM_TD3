/////////////////////////EJERICIO 11//////////////////////////////////////////////////////////////
/* EJERCICIO FINAL, SISTEMA OPERATIVO
  
 Un proceso crea 200 hilos.
 100 hilos se asocian con la función incrementar().
 Los 100 hilos restantes se asocian con la función decrementar().
 Se debe crear la función decrementar(), la cual debe restar la variable en lugar de sumarla.
 Al final de la ejecución de los 200 hilos la variable "total" debe valer cero (0).
 
*/
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

//compilar usando: gcc -o ex11 ex11.c -lpthread

int s,total=0;
pthread_t hilo[200];
pthread_mutex_t mtx=PTHREAD_MUTEX_INITIALIZER; // inicializacion estatica del mutex

void *incrementar (void * nro)
{
   int numero;

   numero = *(int*)nro;
   //comienzo zona critica
   s=pthread_mutex_lock(&mtx);         
   if (s!=0){
	printf("ERROR; pthread_mutex() = %d\n", s);
	exit(-1);   }
	
   total++;
   
   s=pthread_mutex_unlock(&mtx);      
   if (s!=0){
	printf("ERROR; pthread_mutex() = %d\n", s);
	exit(-1);    }
   //fin zona critica
   printf("Ejecutando hilo %d\n", numero);
   pthread_exit(NULL);
}

void *decrementar (void * nro)
{
   int numero;

   numero = *(int*)nro;
   //comienzo zona critica
   s=pthread_mutex_lock(&mtx);         
   if (s!=0){
	printf("ERROR; pthread_mutex() = %d\n", s);
	exit(-1);   }
	
   total--;
   
   s=pthread_mutex_unlock(&mtx);      
   if (s!=0){
	printf("ERROR; pthread_mutex() = %d\n", s);
	exit(-1);    }
   //fin zona critica
   printf("Ejecutando hilo %d\n", numero);
   pthread_exit(NULL);
}

int main()
{
   pthread_t hilo[200];
   int rc, t, arre[200];

   for(t=0; t<100; t++){
      printf("Creando el hilo %d\n", t);
      arre[t] = t;
      rc = pthread_create(&hilo[t], NULL, incrementar , (void *)(&arre[t]) );
      if (rc){
         printf("ERROR; pthread_create() = %d\n", rc);
         exit(-1);
      }
   }
   for(t=100; t<200; t++){
      printf("Creando el hilo %d\n", t);
      arre[t] = t;
      rc = pthread_create(&hilo[t], NULL, decrementar , (void *)(&arre[t]) );
      if (rc){
         printf("ERROR; pthread_create() = %d\n", rc);
         exit(-1);
      }
   }

   int i;
   for (i=0; i<200; i++){
       while(pthread_join(hilo[i], NULL));
   }
  
   printf("Valor de globlal = %d\n", total);
      
   exit(0);
}
