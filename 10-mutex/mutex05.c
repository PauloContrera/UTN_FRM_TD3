/* Ejercicio 6 del TP mutex uso mutex dinamicos */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int t,total, vueltas , s ;
pthread_mutex_t mtx;
pthread_mutexattr_t mtxattr;


void *HILO0(void *nro) {
int local1 , j, numero,s;

	numero= *(int*)nro;

	s=pthread_mutex_lock(&mtx);         
   	if (s!=0){
      	printf("ERROR; pthread_mutex() = %d\n", s);
      	exit(-1);  }

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
      	exit(-1);   }

   	pthread_exit(NULL);
}

int main()   {
pthread_t hilo[5];
int rc ;
int arre[5];

	total=0;
   	vueltas=100000;

  	s=pthread_mutexattr_init(&mtxattr); //inicia el atributos mutex
 	if (s!=0){
      	printf("ERROR; pthread_mutex() = %d\n", s);
      	exit(-1);  }

  	s=pthread_mutexattr_settype(&mtxattr,PTHREAD_MUTEX_NORMAL); //SETEA e ltipo en atributos mutex
  	if (s!=0){
      	printf("ERROR; pthread_mutex() = %d\n", s);
      	exit(-1);  }

   	s=pthread_mutex_init(&mtx,&mtxattr); //inicia el mutex
  	if (s!=0){
      	printf("ERROR; pthread_mutex() = %d\n", s);
      	exit(-1);  }

   	for(t=0; t< 5 ; t++){
   		printf("El main ... creando el thread nro %d\n", t);
   		arre[t] = t;

   		rc = pthread_create(&hilo[t], NULL, HILO0 , (void *)&arre[t]  ); 
   		if (rc){
        	printf("ERROR; pthread_create() = %d\n", rc);
	        exit(-1); }
	}
	pthread_join(hilo[0],NULL);
    pthread_join(hilo[1],NULL);
    pthread_join(hilo[2],NULL);
    pthread_join(hilo[3],NULL);
    pthread_join(hilo[4],NULL);

    s=pthread_mutex_destroy(&mtx);    //destruye el mutex
  	if (s!=0){
      	printf("ERROR; pthread_mutex() = %d\n", s);
      	exit(-1);  }

    printf("Total= %d\n",total);

    pthread_exit(NULL);

}

/* Ejercicio 6 del TP mutex uso mutex dinamicos

	*Analice,   compile   y   ejecute   el   programa   mutex05.c.   ¿Cuál   es   el   objetivo   del programa? 
	*¿Qué diferencia encuentra con los programas anteriores?

	El objetivo del programa es utilizar mutex dinámicos para sincronizar el acceso a la variable global total. 
	Esto evita las condiciones de carrera y garantiza que el incremento de total se realice de manera segura en cada hilo.

	La diferencia principal con los programas anteriores es el uso de mutex dinámicos en lugar de mutex estáticos. 
	Se utilizan las funciones pthread_mutex_init y pthread_mutex_destroy para inicializar y destruir el mutex dinámico respectivamente. 
	Además, se utiliza pthread_mutex_lock y pthread_mutex_unlock para bloquear y desbloquear el mutex dentro de la función HILO0. 
	Esto garantiza la exclusión mutua y evita las condiciones de carrera que se producían en los programas anteriores.

	!Aclaracion entre mutex estatico y dinamico
~Mutex estático: 
	Un mutex estático se inicializa en tiempo de compilación y se destruye automáticamente cuando finaliza el programa. 
	Se declara y se define como una variable global o estática, y se inicializa utilizando el valor predeterminado 
	PTHREAD_MUTEX_INITIALIZER. No se requiere llamar explícitamente a funciones para inicializar o destruir el mutex estático.

~Mutex dinámico: 
	Un mutex dinámico se inicializa en tiempo de ejecución y se destruye mediante una llamada explícita a una función específica. 
	Para utilizar un mutex dinámico, se deben llamar a las funciones pthread_mutex_init para inicializarlo y pthread_mutex_destroy 
	para destruirlo. El mutex dinámico se crea en el montón (heap) y su vida útil puede ser controlada durante la ejecución del programa. 
	Esto permite una mayor flexibilidad y control sobre el mutex.

~Conclusion	
	La ventaja de utilizar mutex dinámicos es que se pueden crear y destruir en cualquier momento durante la ejecución del programa. 
	Esto es útil cuando se necesita un mutex solo en ciertas secciones del código o cuando la cantidad de mutex necesarios 
	puede variar dinámicamente. Además, el uso de mutex dinámicos permite personalizar los atributos del mutex, 
	como su tipo (normal, recursivo, etc.).
x
 */
