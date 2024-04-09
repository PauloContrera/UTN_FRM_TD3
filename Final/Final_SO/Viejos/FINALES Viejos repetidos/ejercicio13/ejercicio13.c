/////////////////////////EJERICIO 13//////////////////////////////////////////////////////////////
/*
Tres hilos (main, A y B) de un proceso se comunican por medio de una Cola de Mensaje.
1) El hilo A escribe en la Cola de Mensaje "0123456789" en forma atomica y termina.
2) El hilo B espera que el hilo A termine
3) El hilo B escribe en la Cola de Mensaje "ABCDEFGHIJ" en forma atomica y termina.
4) El hilo main espera que termine el Hilo B.
5) El hilo main lee los mensajes de la cola de mensajes muestra lo leido y elimima la cola de mensajes.
*/

// Comando: gcc -o ejercicio13 ejercicio13.c -lpthread -lrt

#include <mqueue.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

#define MENSAJE_A "0123456789"
#define MENSAJE_B "ABCDEFGHIJ"
#define MQ_PATH "/MQ_TD3"  // El nombre de la cola de mensajes debe empezar con "/"

mqd_t mqd; // se guarda el descriptor de la cola de mensajes//***********************
struct mq_attr attr; //***********************

pthread_t hilo[2];

void *hiloA()
{
   int err;

    write (STDOUT_FILENO, "\nHILO A ESCRIBIENDO", sizeof("\nHILO A ESCRIBIENDO"));
    // Escribe en cola de mensajes
    err = mq_send(mqd, MENSAJE_A, strlen(MENSAJE_A)+1, 0);  //strlen nos da la longitud de una cadena //***********************
    if(err == -1){
        printf ("error en mq_send()");
        exit(-1);    }

   pthread_exit(NULL);
}

void *hiloB ()
{
   // Espera a que termine el hilo A
   pthread_join(hilo[0],NULL);

   int err;
   
    write (STDOUT_FILENO, "\nHILO B ESCRIBIENDO", sizeof("\nHILO B ESCRIBIENDO"));
    // Escribe en cola de mensajes
    err = mq_send(mqd, MENSAJE_B, strlen(MENSAJE_B)+1, 0);  //strlen nos da la longitud de una cadena //***********************
    if(err == -1){
        printf ("error en mq_send()");
        exit(-1);    }
   
   pthread_exit(NULL);
}

int main()
{
   int rc, leido, err;
   char buff[1024];

    // Se fijan algunos parametros de la cola de mensajes antes de crearla
    attr.mq_msgsize = sizeof(buff); //***********************
    attr.mq_maxmsg = 5; //***********************

    // Se crea cola de mensajes
    mqd = mq_open(MQ_PATH, O_RDWR | O_CREAT, 0777, &attr);  //***********************
    if (mqd < 0) {
        printf ("error en mq_open()");   
        exit(-1); }

    // Se crea el hilo A
    rc = pthread_create(&hilo[0], NULL, hiloA, NULL );
    if (rc){
        printf("ERROR; pthread_create() = %d\n", rc);
        exit(-1);
    }

    // Se crea el hilo B
    rc = pthread_create(&hilo[1], NULL, hiloB, NULL );
    if (rc){
        printf("ERROR; pthread_create() = %d\n", rc);
        exit(-1);
    }

    // Se espera que termine el hilo B
    pthread_join(hilo[1],NULL);

    for(int i = 0; i < 2; i++){
    leido = mq_receive(mqd, buff, attr.mq_msgsize, 0); //dónde está el cero puedo paber la prioridad del mensaje 
    if (( leido < 0 )){
        printf ("error en mq_receive()");
        exit(-1); 
        }else {
            // Escribir en consola
            write (STDOUT_FILENO, "\nLeido de la tuberia: ", sizeof("\nLeido de la tuberia: "));
            write (STDOUT_FILENO, buff, leido-1);
        }
    }
    
    // Se cierra cola de mensajes
    err = mq_close(mqd); //***********************
    if (( err < 0 )){
        printf ("error en mq_close()");
        exit(-1);   }

        printf("\nCola de mensajes cerrada (%d)\n", err);
    
    // Se elimina cola de mensajes
    err = mq_unlink(MQ_PATH); //***********************
    if(err == -1){
        printf ("error en mq_unlink()");
        exit(-1);   }

        printf("Cola de mensajes eliminada (%d)\n", err);

    pthread_exit(NULL);
      
   exit(0);
}