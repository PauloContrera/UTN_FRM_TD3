/////////// final_48 /////////////////////
/*
Tres procesos (A,B,C) NO relacionados se comunican de la siguiente forma:
C y A se comunican por medio de una memoria compartida, sincronizada con un sem�foro
C y B se comunican por medio de una cola de mensajes


1) El proceso C crea el sem�foro lo inicializa en el valor adecuado
2) El proceso C crea el objeto de memoria compartida,lo dimensiona y mapea el objeto de memoria compartida en el espacio virtual de direcciones del proceso.
3) El proceso C crea la cola de mensajes
4) El proceso C cuando recibe la se�al SIGUSR1 escribe en la Memoria Compartida "0123456789" y le avisa al proceso A por medio del sem�foro
5) El proceso C cuando recibe la se�al SIGUSR2 escribe en la cola de mensajes "ABCDEFGHIJ"
6) El proceso A abre el objeto de memoria compartida y mapea el objeto de memoria compartida en el espacio virtual de direcciones del proceso.
7) El proceso A esta en una espera a que C escriba la memoria compartida y le avise que est� lista para ser le�da y cuando lee la memoria muestra lo le�do y termina
8) El proceso B abre la cola de mensajes
9) El proceso B lee el mensaje de la cola de mensajes y termina

El proceso C es el encargado de crear la cola de mensajes, memoria compartida y el sem�foro, es el primer proceso que se ejecuta.

Las se�ales se env�an desde otra consola

 ---      -----------------      ---      --------
|   |--->|Memoria comartida|--->| A |--->|Pantalla|
| C |     -----------------      ---      --------
|   |     -----------------      ---      --------
|   |--->|Cola de mensajes |--->| B |--->|Pantalla|
 ---      -----------------      ---      --------

*/

// Compile el programa: gcc -o name name.c -lpthread -lrt
// Ejecute el programa ./name

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
#include <netinet/in.h>
#include <unistd.h>
#include <wait.h>
#include <arpa/inet.h>

#define MENSAJE_A "0123456789"
#define MENSAJE_B "ABCDEFGHIJ"

#define MQ_PATH "/MQ_TD3"

#define MEM_COM "MEM_COM"
#define semaforo1 "/sema1"
#define sema_lugar "/sem_lugar"

sem_t *sem1;

sem_t *sem_dato, *sem_lugar; // Semaforos
int *ptr;
struct stat sb;
int fd, error, leido, largo;
int dato, i, pos, s, contador;

int err, leido;
char buff[1024];
mqd_t mqd;
struct mq_attr attr, attr_rcv;

//------------------------------------------------------------//
void mnjs1(int a)
{
  // Escribe en la Memoria Compartida "0123456789" y le avisa al proceso A por medio del sem�foro

  /*s = sem_wait(sem_lugar); // Decrementa sem_lugar o esperar si igual a 0
  if (s != 0)
  {
    printf("ERROR; sem_wait() = %d\n", s);
    exit(-1);
  }
  sleep(1);*/

  memcpy((ptr), &MENSAJE_A, sizeof(MENSAJE_A));
  
  s = sem_post(sem_lugar); // Incrementa sem_lugar
  if (s != 0)
  {
    printf("ERROR sem_post()\n");
    exit(-1);
  }
}
//------------------------------------------------------------//
void mnjs2(int b)
{
  // Escribe en la cola de mensajes "ABCDEFGHIJ"
  // Se escribe por la cola de mensajes:
  err = mq_send(mqd, MENSAJE_B, strlen(MENSAJE_B) + 1, 1); // strlen nos da la longitud de una cadena
  if (err == -1)
  {
    printf("error en mq_send()\n");
    exit(-1);
  }

  printf("Mensaje enviado (%d)\n", err);
}
//------------------------------------------------------------//

//------------------------------------------------------------//

//------------------------------------------------------------//
int main()
{
  signal(SIGUSR1, mnjs1);
  signal(SIGUSR2, mnjs2);
  printf("Proceso A PID = %d\n", getpid());

  //------ crea el sem_lugar
  sem_lugar = sem_open(sema_lugar, O_CREAT, 0666, 0);
  if (sem_lugar == SEM_FAILED)
  {
    printf("ERROR sem_open()\n");
    exit(-1);
  }

  //--- Crea la memoria compartida, y obtiene el descriptor
  fd = shm_open(MEM_COM, O_RDWR | O_CREAT, 0777);
  if (fd == -1)
  {
    printf("Error en shm_open, %d,\n", fd);
    exit(-1);
  }

  printf("Objeto de memoria creado\n");

  //--- Se dimensiona la memoria y se pone a cero
  largo = 1024;
  error = ftruncate(fd, largo);
  if (error == -1)
  {
    printf("Error en ftruncate\n", error);
    exit(-1);
  }

  //--- Se mapea la memoria compartida al espacio de memoria del proceso
  //    Devuelve un puntero al area reservada
  ptr = mmap(NULL, 10, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (ptr == (void *)-1)
  {
    printf("Error en mmap %p\n", ptr);
    exit(-1);
  }

  printf("Memoria mapeada en: %p\n", ptr);

  // Se fijan algunos parametros de la cola de mensajes antes de crearla
  attr.mq_msgsize = sizeof(buff);
  attr.mq_maxmsg = 5;

  // Se crea cola de mensajes
  mqd = mq_open(MQ_PATH, O_RDWR | O_CREAT, 0777, &attr);
  if (mqd < 0)
  {
    printf("error en mq_open()\n");
    exit(-1);
  }

  printf("Cola de mensajes creada\n");

  while (1)
  {
  }

  pthread_exit(0);
}

//------------------------------------------------------------//
