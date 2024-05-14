#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>

#define PORT 2010
#define BUFFER_SIZE 1024



int main() {

    int SockEscucha, rx_socket;
    struct sockaddr_in struct_direccion;
    char buffer_rx[BUFFER_SIZE];



    // Crear el socket
    SockEscucha = socket(AF_INET, SOCK_DGRAM, 0);
    if (SockEscucha < 0) {
        perror("Error en función socket()");
        exit(EXIT_FAILURE);
    }
    printf("Paso 1: Crear socket servidor\n");

    // Preparar la dirección IP y el puerto
    memset(&struct_direccion, 0, sizeof(struct_direccion));
    struct_direccion.sin_family = AF_INET;
    struct_direccion.sin_addr.s_addr = htonl(INADDR_ANY);
    struct_direccion.sin_port = htons(PORT);

    // Asociar el socket a la dirección
    if (bind(SockEscucha, (struct sockaddr *)&struct_direccion, sizeof(struct sockaddr_in)) < 0) {
        perror("Error en función bind()");
        exit(EXIT_FAILURE);
    }
    printf("Paso 2: Asociar bind()\n");

    // Servidor espera a recibir algo
    while (1) {
        int len = sizeof(struct_direccion);
        rx_socket = recvfrom(SockEscucha, buffer_rx, sizeof(buffer_rx), 0, (struct sockaddr *)&struct_direccion, &len);
        if (rx_socket < 0) {
            perror("Error en función recvfrom()");
            exit(EXIT_FAILURE);
        }

        // Escribir el mensaje recibido del cliente
        write(STDOUT_FILENO, "Cliente: ", 9);
        write(STDOUT_FILENO, buffer_rx, rx_socket);
        write(STDOUT_FILENO, "\n", 1);

        
      
    }

    // Cerrar la conexión
    close(SockEscucha);

    return 0;
}
