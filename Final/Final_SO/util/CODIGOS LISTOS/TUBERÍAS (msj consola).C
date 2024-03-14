//SE DEFINE WRITE COMO 1 Y READ COMO 0
#define WRITE 1 
#define READ 0 

-------------------------------------------------------------------------------------------------------------------------------------------------

//DECLARO VARIABLES
   int fd[2];//fd[2] para almacenar los descriptores de archivo(identificador numérico utilizado para acceder a un recurso) de la tubería
   int leido;//leido para almacenar el número de bytes leídos 
   char buff[80];//buff[80] para almacenar los datos leídos o escritos
-------------------------------------------------------------------------------------------------------------------------------------------------

//CREACIÓN DE TUBERÍA
pipe(fd);
-------------------------------------------------------------------------------------------------------------------------------------------------

//SE CIERRA EL LADO DE ESCRITURA
close(fd[WRITE]);  
-------------------------------------------------------------------------------------------------------------------------------------------------

//LEER PIPE
eido = read(fd[READ], buff, sizeof(buff));
-------------------------------------------------------------------------------------------------------------------------------------------------

//IMPRIMO MENSAJE
write (STDOUT_FILENO, "Hijo, leido de la tuberia \"", sizeof("Hijo, leido de la tuberia \""));
                                    //Se utiliza sizeof() para garantizar que se escriban todos los bytes del mensaje, incluido el carácter nulo que indica el final de la cadena.
write (STDOUT_FILENO, buff, leido);
                                    //STDOUT_FILENO: Este descriptor se utiliza para referirse al flujo de salida estándar del sistema, que generalmente está asociado a la pantalla o consola.
-------------------------------------------------------------------------------------------------------------------------------------------------

//CIERRO LA LECTURA
close(fd[READ]);   
-------------------------------------------------------------------------------------------------------------------------------------------------

//LECTURA DE TECLADO Y GUARDADO EN PIPE
printf("Ingrese una cadena de caracteres por consola: \n");
leido = read(STDIN_FILENO, buff, sizeof(buff)); //STDIN_FILENO: Este descriptor se utiliza para referirse al flujo de entrada estándar del sistema, que generalmente está asociado al teclado
-------------------------------------------------------------------------------------------------------------------------------------------------

//ESCRITURA DE TUBERÍA
write(fd[WRITE], buff, leido-1);
-------------------------------------------------------------------------------------------------------------------------------------------------

//CIERRO LA ESCRITURA
close(fd[WRITE]);  