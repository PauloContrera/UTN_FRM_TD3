/* Ejercicio 1 del TP de semaforos sin nombre */

//---------------- Uso de sem�foros sin nombre ----------------//

#include <stdio.h>
#include <pthread.h>    
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>

//------- variables globales

pthread_t hilo[2];
sem_t sem;


//---------------- Hilo ---------------------------//

void *HILO(){
int s;

	sleep(3);
	
	printf ("Soy el HILO voy a incrementar semaforo\n");

//------ Se incrementa sem 
// *Es necesario incrementar el semaforo, dado que su valor es =0, y el proceso final esta a la espera
//*Si el hilo no incrementa, el programa queda en bucle
	s=sem_post(&sem);
	if (s != 0) {//Comprueba que salio todo OK
		printf("ERROR sem_post()\n");
		exit(-1);     }
	
	pthread_exit (NULL);//Mata al hilo, mientras el programa termina

}

//-----------------------------------------------------------//

int main() {
	
	int s,sval,rc;
	
	printf ("Main crea el semaforo\n");

//------ inicializa el sem sin nombre 
	s=sem_init(&sem, 0, 0);
	/*
	*El semaforo es declarado en una variable global, 
	*el segundo valor (0) es el valor inicial del semaforo, viene por defecto bloqueado donde ningun proceso puede moficarlo, hasta incrementarlo
	* El tercero, al ser =0 es de uso local del programa, si es mayor a 0, puede ser usado por varios procesos, es un bardo ver SemafosNoLocal.c
	*/
	
	if (s != 0) {//Comprueba que se inicio corectamente
		printf("ERROR sem_init()\n");
		exit(-1);     }

//------lee valor de sem 
	sem_getvalue(&sem, &sval);//!Imnecesario sabiendo que es 0, porque recien se declara
	printf("Valor de semaforo: %d\n",sval);

//------ Crea los hilos 
	rc = pthread_create (&hilo[0], NULL, HILO, NULL);//Crea un hilo y le asigna la funcion HILO, sin valores por el ultimo NULL y por defecto (1 NULL)
	if (rc)    {//Comprueba que no hay error
		printf ("ERROR; pthread_create() = %d\n", rc);
		exit (-1);    }
	
	printf ("Soy el main voy a decrementar semaforo\n");

//------decremento de sem
	s = sem_wait(&sem);
	//!Este proceso se bloqueado, a la espera de que el hilo lo incremente, dado que semaforo =0
	// Por consecuencia es imnesesario esperar a que hilo termina con 	pthread_join (hilo[0], NULL);

	if (s != 0) {//Comprueba que el proceso se bloqueado, a la espera de que el hilo 
		printf("ERROR sem_wait()\n");
		exit(-1);     }

//------elimina de sem
	printf("Se elimina el semaforo\n");
	s=sem_destroy(&sem);

	if (s != 0) {//Comprueba que el semaforo murrio
		printf("ERROR sem_unlink()\n");
		exit(-1);  }

//--------------------------------------------
   
	pthread_join (hilo[0], NULL); //!No es necesario, dado que el semaforo esta bloqueado como se comento arriba
	
	printf("Fin del main() \n");
	
	pthread_exit (NULL); //Mata al hilo principal (Fin del proceso, a no ser que algun hilo se ejecute)

}


/* Ejercicio 1 del TP de semaforos sin nombre 
*Compile y ejecute el programa sem01.c.
*	Compile el programa gcc -o sem01 sem01.c -lpthread
*	Ejecute ./sem01
*	- Analice el código de una implementación de un semáforo sin nombre.

	El main crea un semaforo local 	s=sem_init(&sem, 0, 0);
	~El semaforo es declarado en una variable global, (Para el uso del hilo)
	~El segundo valor (0) es el valor inicial del semaforo, viene por defecto bloqueado donde ningun proceso puede moficarlo, hasta incrementarlo
	~El tercero, al ser =0 es de uso local del programa, si es mayor a 0, puede ser usado por varios procesos, es un bardo ver SemafosNoLocal.c
	 y a su vez main crea un hilo.

	Al ser =0 el valor del semaforo, el proceso main, intenta decrementar el valor. Al =0 queda a la espera de que el valor aumente

	El hilo, incrementa el valor del semaforo en 1, y luego se borra

	Cuando el hilo incrementa el valor, el proceso main, es liberado, y puede continuar. Espera a que el hilo muera, borra el semaforo, 
	y cierra el hilo de main() terminando

	*Conclusiones:

	s = sem_wait(&sem); decrementa el valor, y continua. Pero si este es =0, queda bloqueado y a la espera que se incremente

	s=sem_post(&sem); aumenta el valor en 1 del semaforo, habilitando el main


*/
