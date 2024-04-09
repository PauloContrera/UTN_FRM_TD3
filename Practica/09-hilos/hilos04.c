/*  
 * Ejercicio 4 del TP Hilos
 * Abajo explicacion del codigo
 */
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int suma=0;

void *hilo1 (void * nro)
{
   int numero, temp;
   
   numero = *(int*)nro;
   temp = suma;
   //usleep(300);
   //pthread_yield();
   temp = temp + 1 ;
   suma = temp;
   
   printf("Ejecutando hilo %d\n", numero);
   pthread_exit(NULL);
}

int main()
{
   pthread_t hilo[100];
   int rc, t, arre[100];

   for(t=0; t<100; t++){
      printf("Creando el hilo %d\n", t);
      arre[t] = t;
      rc = pthread_create(&hilo[t], NULL, hilo1 , (void *)(&arre[t]) );
      if (rc){
         printf("ERROR; pthread_create() = %d\n", rc);
         exit(-1);
      }
   }

   int i;
   for (i=0; i<100; i++){
       while(pthread_join(hilo[i], NULL));
   }
  
   printf("Valor de globlal = %d\n", suma);
      
   exit(0);
}

/*  
 * Ejercicio 4 del TP Hilos
*Abra en un editor el programar hilos04.c. ¿Cuál es el propósito del programa?.
*Compile el programa y ejecútelo varias veces. ¿Con qué valor finaliza la variable
*global suma?
El propósito del programa es crear 100 hilos en paralelo y realizar una suma en la variable global suma. 
Cada hilo incrementa la variable suma en 1. Al finalizar la ejecución de todos los hilos, 
se imprime el valor de suma.

el programa termina con la variable global suma=100

*-Ahora, descomente la línea 18 y vuelva a compilar. Ejecute varias veces el
*programa. ¿Con qué valor finaliza la variable global suma? ¿Presenta el mismo
*valor que en el caso anterior? Si se observan cambios, ¿a qué se deben?.
Al ejecutar la variable mas rapido con el uslepp la variable no tiene tiempo suficioente para ejercutarse
El hilo se ejecuta mas rapido y el hilo es terminado antes que la variable pueda sumarse
EL valor varia dado que pocas veces puede completar la sumarse

*- Realice la misma operación que en el punto anterior comentando la línea 18 y
*descomentando la línea 19.

 */