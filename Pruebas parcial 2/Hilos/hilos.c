#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
 /*
 1- El hilo main crea el hilo1.
2- El hilo main debe esperar al hilo1 antes de crear el hilo0.
3- El hilo main crea al hilo 0.
4- El hilo main debe esperar al hilo0 antes de acceder a las variables total y total1.
5- El hilo main antes de terminar muestra el valor de las variables total y total1.

Completar:
 */
pthread_t h[2];
int a[5]={0}, total, total1;

void *hilo0 (void * nro) {
int d, t;
    d = *(int*)nro;
    for(t=0; t < 100000 ; t++){
        total=total+d;
        total1=total1+a[2];
    }
     pthread_exit(NULL);
}
void *hilo1 (void * nro) {
int d, t;
    d = *(int*)nro;
    for(t=0; t < 10000 ; t++){
        total=total+a[3];
        total1=total1+d;
    }
    pthread_exit(NULL);
}
int main() {
int j;
    total=10000;
    total1=100000;
    a[0]=10;
    a[1]=1;
    a[3]=2;
     
    pthread_create(&h[1], NULL, hilo1, (void *) (&a[1]) );
        pthread_join(h[1],NULL);
    pthread_create(&h[0], NULL, hilo0, (void *) (&a[0]) );
        pthread_join(h[0],NULL);

    for(j=0; j < 100000 ; j++){
         total=total+a[3];
         total1=total1+a[1];  
    }
    printf("Total= %d, Total1= %d\n",total,total1);
    pthread_exit(NULL);
}