/* Ejercicio 1 del TP mutex: uso de mutex */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


/* Variables Globales */
int t,total,s ; 

pthread_mutex_t mtx=PTHREAD_MUTEX_INITIALIZER; // inicializacion estatica del mutex


void *HILO0(void *nro) {

/* Variables Locales*/
int numero;

	numero= *(int*)nro;
	
	s=pthread_mutex_lock(&mtx); 
   	if (s!=0){
      printf("ERROR; pthread_mutex() = %d\n", s);
      exit(-1);   }

   	total=total+numero;
   	printf("Hilo 0, recibe nro=%d, total= %d\n",numero, total);

   	s=pthread_mutex_unlock(&mtx); 
   	if (s!=0){
      printf("ERROR; pthread_mutex() = %d\n", s);
      exit(-1);    }

   	pthread_exit(NULL);
   	
}

int main() {

/* Variables Locales*/
pthread_t hilo;
int rc ;

	total=20;
	t=5;

	printf("Total= %d\n",total);
 
	printf("Main crea a hilo 0\n");
    
	rc = pthread_create(&hilo, NULL, HILO0 , (void *)&t  ); 
	if (rc){
       printf("ERROR; pthread_create() = %d\n", rc);
       exit(-1);    }

	s=pthread_mutex_lock(&mtx);  
	if (s!=0){
      printf("ERROR; pthread_mutex() = %d\n", s);
      exit(-1);   }

	total=total+t;

	s=pthread_mutex_unlock(&mtx); 
	if (s!=0){
      printf("ERROR; pthread_mutex() = %d\n", s);
      exit(-1);    }
 
 	pthread_join(hilo,NULL); //espero al hilo

	printf("Total final= %d\n",total);

	pthread_exit(NULL);
	
}

/*
		*Ej1
*Analice, compile y ejecute el programa mutex00.c
	*Compile el programa gcc -o mutex00 mutex00.c -lpthread
	*Ejecute./mutex00
	
	El programa main() crea un hilo y le asigna la funcion hilo 0
	donde le envia un numero en este caso =5, y este lo suma la variable global total

	El ejercicio usa mutex para el control de variables globales total, donde el suma 5 en la funcion hilo total
	y en el programa principal, obteniendo un total de 30 al final

	!Problemas que tiene el programa: al ejecutar un hilo antes de abrir el mutex, el mutex es tomato algunas veces por el hilo, 
	! y otras veces por el programa main, se recomienda abrir el muutex antes de iniciar el hilo

*/