//DEFINO MENSAJE
#define DATA "INFORMACION IMPORTANTE"// contiene la información importante que será escrita en la tubería
#define BUFF_SIZE 80// Define el tamaño del búfer utilizado para leer y escribir en la tubería
---------------------------------------------------------------------------------------------------------------

//DEFINO VARIABLES
int ipc[2], proc;//es un array que almacenará los descriptores de archivo de la tubería
//proc se utiliza para almacenar el resultado de la creación del proceso hijo
   int leido, success;
   char buff[BUFF_SIZE] = {0};//buff es un búfer para almacenar los datos leídos de la tubería.
---------------------------------------------------------------------------------------------------------------

//CREO TUBERÍA  
pipe(ipc);
---------------------------------------------------------------------------------------------------------------

//CIERRO LECTURA
close(ipc[0]);
---------------------------------------------------------------------------------------------------------------

//COPIO MENSAJE EN EL BUFFER
 strncpy(buff, DATA, sizeof(DATA));

 //ESCRIBO EL MENSAJE
 write(ipc[1], buff, sizeof(DATA));
---------------------------------------------------------------------------------------------------------------

//CIERRO ESCRITURA
close(ipc[1]);
---------------------------------------------------------------------------------------------------------------

//leo la tubería
leido = read(ipc[0], buff, sizeof(buff));

//IMPRIMO EL MENSAJE
write (STDOUT_FILENO, "Leido de la tuberia \"", sizeof("Leido de la tuberia \""));
            write (STDOUT_FILENO, buff, leido-1);
            printf("\" por el proceso padre.\n");
