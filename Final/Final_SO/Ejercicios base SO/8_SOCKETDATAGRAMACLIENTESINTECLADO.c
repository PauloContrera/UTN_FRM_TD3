#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_PORT 2010
#define BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1" // Dirección IP del servidor

int main(int argc, const char *argv[]) {
    int sockfd;
    struct sockaddr_in direccion;
    const char *MENSAJE_2 = "HOLISSS";
const char *exit_message = "exit";
    // Crear el socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Error en función socket()");
        exit(EXIT_FAILURE);
    }
    printf("El proceso A crea el socket datagrama cliente\n");

    // Configurar la dirección del servidor
    memset(&direccion, 0, sizeof(direccion));
    direccion.sin_family = AF_INET;
    direccion.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &direccion.sin_addr) <= 0) {
        perror("Error en función inet_pton()");
        exit(EXIT_FAILURE);
    }

    // Enviar mensaje al servidor
    if (sendto(sockfd, MENSAJE_2, strlen(MENSAJE_2), 0, (const struct sockaddr *)&direccion, sizeof(direccion)) < 0) {
        perror("Error en función sendto()");
        
    }
    printf("Cliente envió datos al servidor en IP: %s Puerto: %d\n", SERVER_IP, SERVER_PORT);

    while(1){
        sleep(1);
    }


    // Cerrar el socket
    close(sockfd);

    return 0;
}
