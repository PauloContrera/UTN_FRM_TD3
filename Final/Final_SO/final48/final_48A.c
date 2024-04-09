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
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <wait.h>
#include <arpa/inet.h>

#define MEM_COM "MEM_COM"

#define semaforo1 "/sema1"
#define sema_lugar "/sem_lugar"
sem_t *sem1;
sem_t *sem_dato, *sem_lugar; // Semaforos

int fd, error, leido, largo, s;
char *ptr;
char buff[1024], dato;
struct stat sb;

//------------------------------------------------------------//
int main()
{
    //------ crea el sem_lugar
    sem_lugar = sem_open(sema_lugar, O_CREAT, 0666, 0);
    if (sem_lugar == SEM_FAILED)
    {
        printf("ERROR sem_open()\n");
        exit(-1);
    }

    //--- Crea la memoria compartida, y obtiene el descriptor
    fd = shm_open(MEM_COM, O_RDWR, 0777);
    if (fd == -1)
    {
        printf("Error en shm_open, %d,\n", fd);
        exit(-1);
    }

    printf("Objeto de memoria abierto\n");

    //--- Se mapea la memoria compartida al espacio de memoria del proceso
    //    Devuelve un puntero al �rea reservada
    //ptr = mmap(NULL, 10, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    ptr = mmap(NULL, 10, PROT_READ, MAP_SHARED, fd, 0);
    if (ptr == (void *)-1)
    {
        printf("Error en mmap %p\n", ptr);
        exit(-1);
    }

    printf("Memoria mapeada\n");

    s = sem_wait(sem_lugar); // Decrementa sem_lugar o esperar si igual a 0
    if (s != 0)
    {
        printf("ERROR; sem_wait() = %d\n", s);
        exit(-1);
    }

    //memcpy(&dato, ptr, sizeof(dato)); // se lee el dato en la posicion ptr
    printf("Dato leido por A: %s \n", ptr);

    exit(0);
}

//------------------------------------------------------------//
