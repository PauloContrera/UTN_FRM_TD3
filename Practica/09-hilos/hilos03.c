/*  
 * Ejercicio 3 del TP Hilos
 Compile y ejecute el programa hilos03.c ¿Cuál es el objetivo del programa?.
Observe la salida por consola.
- Ahora, descomente la línea 19 y vuelva a compilar. ¿Qué observa por consola?
¿Por qué se ha modificado el comportamiento del programa?
- Ahora, comente la línea 61 y descomente la línea 62 y vuelva a compilar. Qué
observa por consola? ¿Por qué se ha modificado el comportamiento del programa?
 */
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

pthread_t hilo[2]; //declaran las variables globales hilo como un arreglo de hilos de tamaño 2

void * hilo0() // imprimen un mensaje en la consola y esperan 2 segundos antes de finalizar utilizando pthread_exit(NULL).
{
  //pthread_detach(pthread_self()); //Esta funcion hace que los el hilo no sea unible, por lo tanto hilo 1 no puede esperar a que termine hilo0,       
  //pthread_self() devuelve el id del hilo actual, por eso al llamar a detach se necesita el id del hilo
  printf ("Yo soy el hilo 0 \n");
  sleep(2);  
  pthread_exit (NULL);
}

void * hilo1() // imprimen un mensaje en la consola y esperan a que finalize hilo0, antes de finalizar utilizando pthread_exit(NULL).
{
  printf ("Yo soy el hilo 1 \n");
  printf("Esperando al hilo %lx \n", hilo[0]);
  
  pthread_join(hilo[0],NULL); //Espera a que termine hilo0
  printf("Terminando hilo 1 \n");
  pthread_exit (NULL);
}

int main()
{
  int rc; //almacena el código de retorno de las llamadas a pthread_create
  
  //*Crear hilo 0
  rc = pthread_create (&hilo[0], NULL, hilo0, NULL);
  if (rc) //Avisa si fallo
    {
      printf ("ERROR; pthread_create() = %d\n", rc);
      exit (-1);
    }    
  printf ("El main creando el hilo hola0, nro %lx\n",hilo[0]);
      
  //*Crear hilo 1
  rc = pthread_create (&hilo[1], NULL, hilo1, NULL);  
    if (rc) //Avisa si fallo
    {
      printf ("ERROR; pthread_create() = %d\n", rc);
      exit (-1);
    }    
  printf ("El main creando el hilo hola1, nro %lx\n",hilo[1]);
  
  pthread_join(hilo[1],NULL);
  printf ("Se ejecutaron todos los hilos\n");
  
  pthread_exit(NULL);
  //exit(0);
}
/*
 Compile y ejecute el programa hilos03.c ¿Cuál es el objetivo del programa?.
Observe la salida por consola.
* El codigo base, ejecuta 2 hilos, 
* hilo0 escribe por consola, luego en hilo1, espera a que termine hilo0 con join, 
* imprime por consola
* Y el main espera a que finalize el hilo1 (Que espera al hilo 0) para finalizar

- Ahora, descomente la línea 19 y vuelva a compilar. ¿Qué observa por consola?
¿Por qué se ha modificado el comportamiento del programa?
* Al modificar el programa, lo que hacemos esque el hilo0 no sea unible, 
* por lo tanto hilo1 no puede esperar a que hilo0 termine y hilo0 termina antes

- Ahora, comente la línea 61 y descomente la línea 62 y vuelva a compilar. Qué
observa por consola? ¿Por qué se ha modificado el comportamiento del programa?

* No se mofica el comportamiento del programa, dado que pthread_exit(NULL); mata al hilo main(),
* y si mata al hilo los procesos siguientes no se ejecutan 
* exit(0); el proceso termina el programa main
* Por lo tanto ambas maneras sirven para que el hilo main() termine

! Importante, el programa no se modifica dado que el programa main espera a que termine el hilo1, pero si esta linea se comenta
! El  pthread_exit(NULL) mata el hilo main, pero los demas hilos se siguen ejecutando (hilo1 termina despues que main)
! El exit(0) mata todos los hilos ejecutandose y no deja terminar a hilo1

~En consecuencia el uso correcto para terminar los main si se trabaja con hilo es pthread_exit(NULL);
*/
