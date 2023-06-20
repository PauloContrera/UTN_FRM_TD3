#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

sem_t *sem;

void *HILO() {
    sleep(3);
    printf("Soy el HILO, voy a incrementar el semáforo\n");
    sem_post(sem);
    pthread_exit(NULL);
}

int main() {
    int sval, rc;
    
    // Crea el semáforo sin nombre
    sem = sem_open("/my_semaphore", O_CREAT | O_EXCL, 0644, 0);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }
    
    // Lee el valor del semáforo
    sem_getvalue(sem, &sval);
    printf("Valor del semáforo: %d\n", sval);
    
    // Crea el hilo
    pthread_t hilo;
    rc = pthread_create(&hilo, NULL, HILO, NULL);
    if (rc) {
        printf("ERROR; pthread_create() = %d\n", rc);
        exit(-1);
    }
    
    printf("Soy el main, voy a decrementar el semáforo\n");
    
    // Decrementa el semáforo
    sem_wait(sem);
    
    // Destruye el semáforo
    sem_close(sem);
    sem_unlink("/my_semaphore");
    
    // Espera a que el hilo termine
    pthread_join(hilo, NULL);
    
    printf("Fin del main()\n");
    
    pthread_exit(NULL);
}
