/////////// final_51 /////////////////////
/*
Tres procesos (A,B,C) se comunican de la siguiente forma:
C y A se comunican por medio de un pipe
C y B se comunican por medio de una cola de mensajes

1) El proceso C crea el pipe 
2) El proceso C crea al proceso A.
3) El proceso C crea la cola de mensajes
4) El proceso C cuando recibe la se�al SIGUSR1 escribe en el pipe "0123456789" 
5) El proceso C cuando recibe la se�al SIGUSR2 escribe en la cola de mensajes "ABCDEFGHIJ" 
6) El proceso A lee el pipe y muestra en pantalla lo le�do
7) El proceso B abre la cola de mensajes
8) El proceso B lee el mensaje de la cola de mensajes y muestra en pantalla lo le�do

El proceso C es el encargado de crear el pipe y la cola de mensajes, es el primer proceso que se ejecuta.
Las se�ales se env�an desde otra consola


 ---      -----------------      ---      --------  
|   |--->|     pipe        |--->| A |--->|Pantalla|  
| C |     -----------------      ---      --------
|   |     -----------------      ---      --------  
|   |--->|Cola de mensajes |--->| B |--->|Pantalla|  
 ---      -----------------      ---      --------
 
*/

//Compile el programa: gcc -o name name.c -lpthread -lrt
//Ejecute el programa ./name


#include <errno.h>

#include <fcntl.h>

#include <mqueue.h>

#include <netinet/in.h>

#include <pthread.h>

#include <signal.h>

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <semaphore.h>

#include <sys/mman.h>

#include <sys/shm.h>

#include <sys/stat.h>

#include <sys/types.h>

#include <netinet/in.h>

#include <sys/socket.h>

#include <unistd.h>

#include <wait.h>

#include <arpa/inet.h>


#define MENSAJE_A "0123456789"
#define MENSAJE_B "ABCDEFGHIJ"

# define MQ_PATH "/MQ_TD3"

int escrito;
int fd[2];
int err, leido;
            char buff[1024];
              mqd_t mqd;
            struct mq_attr attr, attr_rcv;
            

//------------------------------------------------------------//
void mnjs1(int a) {

    strncpy(buff, MENSAJE_A, sizeof(MENSAJE_A)); //Copia la MENSAJE_A en buff 
    escrito = write(fd[1], buff, sizeof(MENSAJE_A)); //Envia la copia
    if (escrito < 1) {
        // Escribe en consola
        printf("\n");
        write(STDOUT_FILENO, "Fallo la escritura: ", sizeof("Fallo la escritura: "));
        //write (STDOUT_FILENO, leido, sizeof(leido));

    } else {
        // Escribe en consola
        printf("\n");
        write(STDOUT_FILENO, "Escritura perfecta", sizeof("Escritura perfecta"));
        //write (STDOUT_FILENO, buff, leido);

    }

}
//------------------------------------------------------------//
void mnjs2(int b) {

err = mq_send(mqd, MENSAJE_B, strlen(MENSAJE_B)+1, 1);  //strlen nos da la longitud de una cadena
   /* mq_send(descriptor, mensaje, tamaño del mensaje, prioridad);
   err = mq_send( ->Para enviar 
      mqd, -> Nombre de la cola 
      MENSAJE, ->Mensaje xD
      strlen(MENSAJE)+1, -> Longitud del mensaje (porque mas Ni idea)
      1 -> prioriedad, mientras mas alto, mas alta la prioridad (Lo atiende primero)
      );
   */
   if(err < 0) {
      write (STDOUT_FILENO, "Error al enviar mensaje\n", sizeof("Error al enviar mensaje\n"));
   }else {
      write (STDOUT_FILENO, "Mensaje enviado\n", sizeof("Mensaje enviado\n"));
   } 

}
//------------------------------------------------------------//


//------------------------------------------------------------//
int main() {


    // *Creamos la pipe
    // !La creamos al inicio para enlazarla con el hijo
    pipe(fd);
    // Crea la pipe y da los descriptores fd[0] fd[1]
    //fd[0] -> Read
    //fd[1] -> Write
    //printf ("fd[0] = %d fd[1] = %d \n ", fd[0], fd[1]);  



    switch (fork()) {
        case -1: // No pudo crear el proceso
            printf("Error. No se crea proceso hijo");
            break;

        case 0: //proceso hijo
            printf("Soy el proceso Hijo A, pid: %d\n", getpid());
            close(fd[1]); //Cerramos Escritura
            while (1) {
                leido = read(fd[0], buff, sizeof(buff));
                if (leido < 1) {
                    printf("\n");
                    // Escribe en consola
                    write(STDOUT_FILENO, "Hijo, tuberia vacía: ", sizeof("Hijo, tuberia vacía:"));
                    //write (STDOUT_FILENO, leido, sizeof(leido));
                } else {
                    // Escribe en consola
                    printf("\n");
                    write(STDOUT_FILENO, "Hijo, leido de la tuberia: ", sizeof("Hijo, leido de la tuberia: "));
                    write(STDOUT_FILENO, buff, leido);
                }
            }

            close(fd[0]); //Cerramos lectura


            break;

        default: //proceso padre
            printf("Soy el proceso Padrer C, pid: %d\n", getpid());
            close(fd[0]); //Cerramos PIPE lectura 
            attr.mq_msgsize = sizeof(buff);
            attr.mq_maxmsg = 5;
            //* Se crea cola de mensajes  
            // Se fijan algunos parametros de la cola de mensajes antes de crearla
            mqd = mq_open(MQ_PATH, O_RDWR | O_CREAT, 0777, & attr);
            /*mqd = mq_open(
               MQ_PATH, 
               O_RDWR | O_CREAT, --> Intenta abrir como lectura y escritura pero (| significa o) sino la crea
               0777, 
               &attr); */
            if (mqd < 0) {
                write(STDOUT_FILENO, "Error al crear Cola\n", sizeof("Error al crear Cola\n"));
                // Puede que ya exista si hay error
            } else {
                write(STDOUT_FILENO, "Cola creada correctamente\n", sizeof("Cola creada correctamente\n"));
            }

            // Configurar el manejador de señales para SIGUSR1
            signal(SIGUSR1, mnjs1);
            signal(SIGUSR2, mnjs2);

            printf("Esperando la señal SIGUSR1 para imprimir un mensaje...\n");
            while (1) {
                sleep(1);
            }
    }
    exit(0);







}
//------------------------------------------------------------//