
/////////// final_45 /////////////////////
/*
Cuatro procesos (A,B,C,D) relacionados de la siguiente forma:
A padre de B y padre de C

Los procesos se comunican de la siguiente forma:
A y B se comunican por medio de un pipe
A y C se comunican por medio de un pipe
A y D se comunican por medio de una cola de mensajes

1) El proceso A crea la cola de mensajes, crea al proceso B y al proceso C 
2) El proceso B cuando recibe la se�al SIGUSR1 escribe en el pipe1 el mensaje "0123456789" y termina.
3) El proceso C cuando recibe la se�al SIGUSR2 escribe en el pipe2 el mensaje "ABCDEFGHIJ" y termina.
4) El proceso A espera a que termine el proceso B, lee el pipe1 y lo le�do lo env�a a la cola de mensajes 
5) El proceso A espera a que termine el proceso C, lee el pipe2 y lo le�do lo env�a a la cola de mensajes 
6) El proceso D lee los mensajes de la cola mensajes (2 mensajes) y los muestra en pantalla

El proceso A debe ser el primero en comenzar en una consola
El proceso D comienza luego de A otra consola
Las se�ales se env�an desde otra consola


       --- 
      | B |
       --- 
       |     -------      ---      --------      ---      --------  
       |--->| PIPE1 |--->|   |    | Cola   |    |   |    |        |  
       |     -------     | A |--->|Mensajes|--->| D |--->|pantalla|
       |     -------     |   |--->|        |--->|   |--->|        | 
       |--->| PIPE2 |--->|   |     --------      ---      --------  
       |     -------      ---
       ---              
      | C |
       --- 
*/

//Compile el programa: gcc -o name name.c -lpthread -lrt
//Compile el programa: gcc -o file/procA final_45A.c -lpthread -lrt
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
#include <sys/socket.h>
#include <unistd.h>
#include <wait.h>
#include <arpa/inet.h> 


#define MQ_PATH "/MQ_TD3" 


#define MENSAJE_A "0123456789"
#define MENSAJE_B "ABCDEFGHIJ"

//------------------------------------------------------------//

int escrito;
int fb[2];
int fc[2];
int err, err2, leido;
            char buff[1024];
            char buff2[1024];
              mqd_t mqd;
            struct mq_attr attr, attr_rcv;

//------------------------------------------------------------//
void mnjs1(int a) {

 // Se escribe en la tuberia
    strncpy(buff, MENSAJE_A, sizeof(MENSAJE_A)); //Copia la FRASE_A en buff 
    escrito=write(fb[1], buff,    sizeof(MENSAJE_A)); //Envia la copia
    if(escrito < 1){
       // Escribe en consola
       printf("\n");
       write (STDOUT_FILENO, "Fallo la escritura: ", sizeof("Fallo la escritura: "));
       //write (STDOUT_FILENO, leido, sizeof(leido));

    }
    else {
       // Escribe en consola
       printf("\n");
       write (STDOUT_FILENO, "Escritura perfecta", sizeof("Escritura perfecta"));
       //write (STDOUT_FILENO, buff, leido);

    }
            close(fb[1]); //Cerramos escritura 
   exit(0);


}
//------------------------------------------------------------//
void mnjs2(int a) {


 // Se escribe en la tuberia
    strncpy(buff, MENSAJE_B, sizeof(MENSAJE_B)); //Copia la FRASE_A en buff 
    escrito=write(fc[1], buff,    sizeof(MENSAJE_B)); //Envia la copia
    if(escrito < 1){
       // Escribe en consola
       printf("\n");
       write (STDOUT_FILENO, "Fallo la escritura: ", sizeof("Fallo la escritura: "));
       //write (STDOUT_FILENO, leido, sizeof(leido));

    }
    else {
       // Escribe en consola
       printf("\n");
       write (STDOUT_FILENO, "Escritura perfecta", sizeof("Escritura perfecta"));
       //write (STDOUT_FILENO, buff, leido);

    }
            close(fc[1]); //Cerramos escritura 
   exit(0);


}


//------------------------------------------------------------//
int main() {
   

    // *Creamos la pipe
    // !La creamos al inicio para enlazarla con el hijo
   pipe(fb);
   pipe(fc);



    switch (fork()) {
        case -1: // No pudo crear el proceso
            printf("Error. No se crea proceso hijo");
            break;

        case 0: //proceso hijo
            printf("Soy el proceso Hijo B, pid: %d\n", getpid());
            close(fb[0]); //Cerramos lectura 
            signal(SIGUSR1, mnjs1);
            printf("Esperando la señal SIGUSR1 para imprimir un mensaje...\n");
            while (1) {
                sleep(1);
            }



            break;

        default: //proceso padre
            switch (fork()) {
                case -1: // No pudo crear el proceso
                    printf("Error. No se crea proceso hijo");
                    break;

                case 0: //proceso hijo
                    printf("Soy el proceso Hijo C, pid: %d\n", getpid());
                     close(fc[0]); //Cerramos lectura 
                     signal(SIGUSR2, mnjs2);
                     printf("Esperando la señal SIGUSR2 para imprimir un mensaje...\n");
                     while (1) {
                         sleep(1);
                     }  
                  break;

                default: //proceso padre
                  printf("Soy el proceso Padrer A, pid: %d\n", getpid());

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


                  wait(NULL);
                  wait(NULL);


                  close(fb[1]);  //Cerramos Escritura
                   leido = read(fb[0], buff, sizeof(buff));
                   if(leido < 1){
                       printf("\n");
                       // Escribe en consola
                       write (STDOUT_FILENO, "tuberia vacía: ", sizeof("tuberia vacía:"));
                       //write (STDOUT_FILENO, leido, sizeof(leido));
                   }
                   else {
                      // Escribe en consola
                      printf("\n");
                      write (STDOUT_FILENO, "\nProceso A, leido de la tuberia de proceso B: ", sizeof("\nProceso A, leido de la tuberia de proceso B:"));
                      write (STDOUT_FILENO, buff, leido);
                   }
                   close(fb[0]);//Cerramos lectura

                   err = mq_send(mqd, buff, strlen(buff)+1, 1);  //strlen nos da la longitud de una cadena
  
                     if(err < 0) {
                        write (STDOUT_FILENO, "Error al enviar mensaje\n", sizeof("Error al enviar mensaje\n"));
                     }else {
                        write (STDOUT_FILENO, "Mensaje enviado 1 por cola\n", sizeof("Mensaje enviado 1 por cola\n"));
                     }



                    close(fc[1]);  //Cerramos Escritura
                   leido = read(fc[0], buff2, sizeof(buff2));
                   if(leido < 1){
                       printf("\n");
                       // Escribe en consola
                       write (STDOUT_FILENO, "tuberia vacía: ", sizeof("tuberia vacía:"));
                       //write (STDOUT_FILENO, leido, sizeof(leido));
                   }
                   else {
                      // Escribe en consola
                      printf("\n");
                      write (STDOUT_FILENO, "\nProceso A, leido de la tuberia de proceso C: ", sizeof("\nProceso A, leido de la tuberia de proceso C:"));
                      write (STDOUT_FILENO, buff2, leido);
                   }
                   close(fc[0]);//Cerramos lectura

                     err2 = mq_send(mqd, buff2, strlen(buff2)+1, 1);  //strlen nos da la longitud de una cadena
  
                     if(err < 0) {
                        write (STDOUT_FILENO, "Error al enviar mensaje\n", sizeof("Error al enviar mensaje\n"));
                     }else {
                        write (STDOUT_FILENO, "Mensaje enviado 2 por cola\n", sizeof("Mensaje enviado 2 por cola\n"));
                     }
               
            }
           
           
    }
    exit(0);
}

//------------------------------------------------------------//

