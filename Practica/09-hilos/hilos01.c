/*  
 * Ejercicio 1 del TP Hilos
   Compile y ejecute el programa hilos01.c.
   Compile el programa gcc -o hilos01 hilos01.c -lpthread
   Ejecute ./ hilos01
   Analice el código ejemplo de una creación de hilo.
 */


// *Incluye las librerias, en especial pthread.h que es la libreria de hilos 
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


// *Define la funcion hola, que sera llamada por el hilo
void *hola(void * nro) {
  
   //espera 2 segundos
   sleep(2); 
   // imprime el numero del hilo, enviado en el valor de la funcion *nro, 
   // el nro lo fuerza a ser un entero, dado que esta declaro como void
   printf("Hola, soy el hilo %d\n", * (int*) nro);
   // llama a pthread_exit(NULL), para finalizar el hilo
   pthread_exit(NULL);
   //exit(0);
}

int main() {

   pthread_t hilo[1];  // Se declara un arreglo de hilos
   int rc, t;         // Se declaran variables para el código de retorno y el número de hilo

   // Se establece el numero de hilo (=0) y se anuncia por pritntf
   t = 0;
   printf("Main creando el hilo nro %d\n", t);

   /*     
   Se utiliza pthread_create para crear un nuevo hilo, pasando como argumentos 
   el identificador del hilo (&hilo[0]), 
   atributos del hilo (en este caso se utiliza NULL para utilizar los valores predeterminados), 
   la función a ejecutar (hola) y 
   el argumento que se le pasa a la función ((void *)(&t)). 
   El argumento se pasa como un puntero al número de hilo, 
   por lo que se realiza un casting a (void *) antes de pasarlo.
   */
   /*
   !Es importante aclarar que casting a (void *) es para pasar el valor de t a un puntero generico, por restrincciones de c
   !Y luego en la funcion hola, se vuelve a pasar a entero (int *)
   */
   rc = pthread_create(&hilo[0], NULL, hola , (void *)(&t)  );


   // Hace un chequeo para saber que el hilo fue creado correctamente, =0 ok, >0 error  
   if (rc != 0){
         printf("ERROR; pthread_create() = %d\n", rc);
         exit(-1);        
   };
   // Hilo creado a esta altura, imprime para saber que el hilo ya se esta ejecutando
   printf("Espera a que termine hilo\n");


   /*
   Se utiliza pthread_join para esperar a que el hilo finalice. 
   En este caso, se espera al hilo número 0 (hilo[0]) y 
   se pasa NULL como puntero de salida para obtener el valor de retorno del hilo.
   */
   pthread_join(hilo[0],NULL);

   // Avisa que el hilo main va ser borrado, eliminando todos los procesos posteriores
   printf("Termina hilo main\n");
   pthread_exit(NULL);
   
   // !Este proceso nunca se ejecuta, dado que pthread_exit(NULL); mato al proceso principal
   printf("Este mensaje nunca se va a mostrar\n"); //Esto es un agregado para hacer una prueba
   return 0;
   
}
