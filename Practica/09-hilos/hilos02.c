/*  
 * Ejercicio 2 del TP Hilos
Compile y ejecute el programa hilos02.c.
- Analice el código. ¿Qué se debería ver por consola y qué es lo que se observa?
- ¿Por qué no se ejecutará el printf de la línea 39?
- Descomente la línea 30 y comente la 29 y vuelva a compilar. ¿Qué observa por
 consola? ¿Por qué se ha modificado el comportamiento del programa?
 */
 
// *Incluye las librerias, en especial pthread.h que es la libreria de hilos 
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

// *Variables Globales, t es del lazo for (No cambia donde sea declarada)
int t = 0 ;
int arre[5]={0};

// *Define la funcion hola, que sera llamada por el hilo, donde en nro es el numero del hilo
void *hola(void * nro) {
   //espera 2 segundos
   sleep(2); 
   // imprime el numero del hilo, enviado en el valor de la funcion *nro, 
   // el nro lo fuerza a ser un entero, dado que esta declaro como void, xq c solo permite enviar void en hilos
   printf("Hola, soy el hilo %d\n", * (int*) nro);
   // llama a pthread_exit(NULL), para finalizar el hilo
   pthread_exit(NULL);
}

int main() {
   pthread_t hilo[5];  // Se declara un arreglo de hilos
   int rc;         // Se declaran variables para el código de retorno (No necesario)

   // Crea un lazo for con la variable T (Global), para crear 5 hilos, estos hilos se ejercutan en paralelo
  for(t=0; t < 5 ; t++){

   // Avisa que esta creando el hilo y guardando el arreglo en un vector
   printf("Main creando el hilo nro %d\n", t);
   arre[t] = t;
     

   // !Problematica, si usamos la linea 46 los hilos solo se crean en el ultimo valor de de t (=5)
   // !En cambio si usamos la linea 47 los hilos se crean de 0 a 4 sin problema

   //  rc = pthread_create(&hilo[t], NULL, hola , (void *)(&t)  );
     rc = pthread_create(&hilo[t], NULL, hola , (void *) (&arre[t]) );
     
   /* 
      !Explicando el error
      El programa aún se compilará y ejecutará sin errores. 
      Sin embargo, el comportamiento del programa se modificará. 
      Ahora, todos los hilos imprimirán el número 5 en lugar de los valores del 0 al 4. 
      Esto se debe a que la variable t en el bucle for se está pasando como argumento al hilo mediante el puntero &t
      , pero la variable t sigue siendo modificada por el bucle antes de que cada hilo tenga la oportunidad de leer su valor. 
      Como resultado, cuando los hilos se ejecutan y leen el valor de t, todos obtienen el valor final de t, que es 5.
   */

   // Hace un chequeo para saber que el hilo fue creado correctamente, =0 ok, >0 error  
     if (rc != 0){
         printf("ERROR; pthread_create() = %d\n", rc);
         exit(-1);
      }
   }

   
   // Eliminando el hilo main() y todos sus procesos posteriores
   pthread_exit(NULL);

   // !Este proceso nunca se ejecuta, dado que pthread_exit(NULL); mato al proceso principal
   printf("Esto no aparece!!!\n");
   
}
