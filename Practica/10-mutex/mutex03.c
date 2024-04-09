/* Ejercicio 4 del TP mutex 
 acceso a variables compartidas con y sin uso mutex */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/* Variables Globales */
int t,total, vueltas,s ;

pthread_mutex_t mtx=PTHREAD_MUTEX_INITIALIZER; // inicializacion estatica del mutex

void *HILO0(void *nro) {
int local1 , j, numero;

	numero= *(int*)nro;

	sleep(2);
	
	s=pthread_mutex_lock(&mtx);         
	if (s!=0){
		printf("ERROR; pthread_mutex() = %d\n", s);
		exit(-1);   }

	printf("Hilo= %d decrementa mutex\n", numero);

	for(j=0;j< vueltas;j++){
		local1 = total;
		local1++;
		total=local1;
	}

	printf("Hola soy, el thread %d, total= %d\n", numero,total);
	printf("Hilo= %d incrementa mutex\n", numero);

	s=pthread_mutex_unlock(&mtx);      
	if (s!=0){
		printf("ERROR; pthread_mutex() = %d\n", s);
		exit(-1);    
		}

	pthread_exit(NULL);
	
}

void *HILO1(void *nro) {

int local1 , j, numero;

	numero= *(int*)nro;

	sleep(2);

	for(j=0;j< vueltas;j++){
		local1 = total;
		local1++;
		total=local1;
	}

	printf("Hola soy, el thread %d, total= %d\n", numero,total);
	printf("Hilo= %d incrementa mutex\n", numero);

	pthread_exit(NULL);
}

int main() {

int rc ;
int arre[5];
pthread_t hilo[5];

	total=0;
	vueltas=1000000;

	t=0;
	printf("El main ... creando el thread nro %d\n", t);
	arre[t] = t;

	rc = pthread_create(&hilo[t], NULL, HILO0 , (void *)&arre[t]  );
	if (rc){
		printf("ERROR; pthread_create() = %d\n", rc);
		exit(-1);}

	t=1;
	printf("El main ... creando el thread nro %d\n", t);
	arre[t] = t;

	rc = pthread_create(&hilo[t], NULL, HILO1 , (void *)&arre[t]  );
	if (rc){
		printf("ERROR; pthread_create() = %d\n", rc);
		exit(-1);}

	pthread_join(hilo[0],NULL);
	pthread_join(hilo[1],NULL);

	printf("Total= %d\n",total);

	pthread_exit(NULL);

}

/* Ejercicio 4 del TP mutex: acceso a variables compartidas con y sin uso mutex 

*Analice,   compile   y   ejecute   el   programa   mutex03.c.     ¿Cuál   es   el   propósito   del programa?. 
	*- Ejecute varias veces ./mutex03  

	El codigo crea 2 hilos que incrementan la variable globlar de la misma manera que mutex01 y mutex 02
	El hilo 0 usa mutex y el hilo 1 no, esto trae el mismo problema que mutex01

*¿Cuál debería ser el valor de la  variable  global  total y con qué valor finaliza la variable global total?  
*¿Cómo solucionaría el problema?

	El valor de la variable global debe ser 200000 y finaliza con valores distintos dado que hilo 1 interfiere cuando 
	hilo 0 esta ejecutando la variblae
	Solucion agregar mutex a hilo1, se agregara en mutex03edit.c

*/