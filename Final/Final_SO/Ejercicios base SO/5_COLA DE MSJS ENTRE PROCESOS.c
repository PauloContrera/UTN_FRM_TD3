#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

// Definir mensaje
#define MENSAJE "DATA PARA OTROS PROCESOS"
#define MQ_PATH "/MQ_TD31"

int main() {
    int err, leido;
    char buff[1024];
    mqd_t mqd;
    struct mq_attr attr, attr_rcv;

    // Fijar parámetros antes de crear la cola
    attr.mq_msgsize = sizeof(buff);
    attr.mq_maxmsg = 5;

    // Crear cola de mensajes
    mqd = mq_open(MQ_PATH, O_RDWR | O_CREAT, 0777, &attr);
    if (mqd == (mqd_t)-1) {
        perror("Error al crear la cola de mensajes");
        exit(EXIT_FAILURE);
    }

    // Escribir en la cola de mensajes
    err = mq_send(mqd, MENSAJE, strlen(MENSAJE) + 1, 1);
    if (err == -1) {
        perror("Error al enviar el mensaje a la cola de mensajes");
        exit(EXIT_FAILURE);
    }

    // Leer parámetros de la cola de mensajes
    if (mq_getattr(mqd, &attr_rcv) == -1) {
        perror("Error en mq_getattr()");
        exit(EXIT_FAILURE);
    }
    printf("Nro max. de mensajes en cola de mensajes: %ld\n", attr_rcv.mq_maxmsg);
    printf("Longitud max. de mensaje: %ld\n", attr_rcv.mq_msgsize);
    printf("Nros de mensajes pendientes en cola de mensajes: %ld\n", attr_rcv.mq_curmsgs);

    // Leer contenido de la cola de mensajes
    leido = mq_receive(mqd, buff, attr_rcv.mq_msgsize, NULL);
    if (leido < 0) {
        perror("Error al leer mensaje");
    } else {
        printf("Mensaje leído: %s\n", buff);
    }

    // Cerrar cola de mensajes
    err = mq_close(mqd);
    if (err == -1) {
        perror("Error al cerrar la cola de mensajes");
    }

    // Eliminar cola de mensajes
    err = mq_unlink(MQ_PATH);
    if (err == -1) {
        perror("Error al eliminar la cola de mensajes");
    }

    return 0;
}
