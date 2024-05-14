#include <sys/socket.h>
//****************** 1 *******************//
int main(int argc, const char *argv[])
{	
    if (argc != 2)
    {
        argv[1] = "2000";
    }
	printf("Soy proceso C, Mi pid es %d \n", getpid());

    //****************** 1 *******************//
    //-- socket(): Crear el socket -----------//
    SockEscucha = socket(AF_INET, SOCK_DGRAM, 0);

    if ((SockEscucha) < 0)
    {
        printf("ERROR en funcion socket()\n");
        exit(-1);
    }

    printf("Paso 1: creo socket servidor\n");

--------------------------------------------------------------------------------------------------------------------------------------------
//****************** 2 *******************//
//----- preparar el address:port (INDICO PROTOCOLO E IP PARA QUE SE CONECTE CLIENTE)-------------//
    struct_direccion.sin_family = AF_INET;//indica que se está utilizando IPv4
    struct_direccion.sin_addr.s_addr = htonl(INADDR_ANY); // asigna una IP de la maquina
    struct_direccion.sin_port = htons(atoi(argv[1]));     // port pasado por argumento//
--------------------------------------------------------------------------------------------------------------------------------------------
//-- bind(): asociamos el socket a la direccion ------//
if (bind(SockEscucha, (struct sockaddr *)&struct_direccion, sizeof(struct sockaddr_in)) < 0)
    {
        printf("ERROR en funcion bind()\n");
        exit(-1);
    }

    printf("Paso 2: Asociar bind() \n");
--------------------------------------------------------------------------------------------------------------------------------------------
//****************** 3 *******************//
    //-- servidor espera a recibir algo ---
    while (1)
    { // lee del socket

        int len = sizeof(struct_direccion);
        rx_socket = recvfrom(SockEscucha, buffer_rx, sizeof(buffer_rx), 0, (struct sockaddr *)&struct_direccion, &len); // recibe del socket
        write(STDOUT_FILENO, "cliente:--> ", 12);   // escribe leyenda en pantalla
            write(STDOUT_FILENO, buffer_rx, rx_socket); // escribe lo leido del socket
            write(0, "\n", sizeof("\n"));
             // Verificar si se recibió un mensaje de cierre del cliente
        if (strcmp(buffer_rx, "exit") == 0) {
            printf("El cliente ha cerrado la conexión. Cerrando el servidor.\n");
            break; // Salir del bucle while
       
      
     
    }
     exit(0); 
    //****************** 4 *******************//
    //------------cierrar la conexion --------//

    close(SockEscucha);

    return 0;
