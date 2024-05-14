
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
    // como el servidor esta en el mismo host usamos Localhost = 127.0.0.1
	printf("Cliente va a enviar datos a IP: 127.0.0.1 Puerto: %s\n", argv[1]);
	printf("El proceso A crea el socket datagrama cliente\n");
    //ENVÍA EL MENSAJE 2 AL SERVIDOR
	sendto(sockfd, MENSAJE_2, sizeof(MENSAJE_2), 0, (struct sockaddr *)&direccion, sizeof(direccion));