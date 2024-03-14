//DEFINICION DE FIFO Y MENSAJE
#define MENSAJE "HOLA PROCESO HIJO"
#define FIFO_PATH "/tmp/MI_FIFO"

//DEFINO VARIABLES
int err, fifo_d;
   char buff[80];
   int leido;

// SI LA FIFO EXISTE LA BORRO
unlink(FIFO_PATH); 

//ESTABLECE LOS PERMISOS PARA EL FIFO UTILIZANDO MÁSCARAS DE BITS.
 err = mkfifo(FIFO_PATH, S_IRUSR | S_IWUSR | S_IXUSR|  // el usuario que la creo
                           S_IRGRP | S_IWGRP | S_IXGRP|  // el grupo al que pertenece el usuario
                           S_IROTH | S_IWOTH | S_IXOTH); // el resto de los usuarios del sistema

//TAMBIEN ESTABLECE LOS PERMISOS PARA EL FIFO .
err = mkfifo(FIFO_PATH, 0777);

//CONFIRMACION DE CREACION DE FIFO
if(err == -1) {
      write (STDOUT_FILENO, "Error al crear FIFO, la FIFO ya existe\n", sizeof("Error al crear FIFO, la FIFO ya existe\n"));
   }else {
      write (STDOUT_FILENO, "FIFO creada correctamente\n", sizeof("FIFO creada correctamente\n"));
   }

   // PARA ABRIR FIFO	
   fifo_d = open(FIFO_PATH, O_RDONLY,0); //O_NONBLOCK, AQUI SE ABRE PARA LECTURA
   fifo_d = open(FIFO_PATH, O_WRONLY, 0);//AQUI SE ABRE PARA ESCRITURA

   //CONFIRMACIÓN DE APERTURA DE FIFO
   if(fifo_d == -1){
            write (STDOUT_FILENO, "Hijo, error al abrir FIFO\n", sizeof("Hijo: error al abrir FIFO\n"));
            return -1;
         }else {
            write (STDOUT_FILENO, "Hijo, FIFO abierta correctamente\n", sizeof("Hijo: FIFO abierta correctamente\n"));
         }

    // ESCRIBIR EN FIFO
  err = write(fifo_d, MENSAJE, sizeof(MENSAJE));
  //COMPROBACIÓN DE ESCRITURA
   if(err == -1) {
            write(STDOUT_FILENO, "Padre, FIFO vacia\n", sizeof("Padre, FIFO vacia\n"));
         } else {
            write(STDOUT_FILENO, "Padre, escrito MENSAJE en FIFO\n", sizeof("Padre, escrito MENSAJE en FIFO\n"));
         }

//CIERRO FIFO
close(fifo_d);

   //LEER FIFO
 leido = read(fifo_d, buff, sizeof(buff));
//COMPROBACIÓN DE LECTURA
 if(leido < 1){
            write(STDOUT_FILENO, "Hijo, FIFO vacia\n", sizeof("Hijo, FIFO vacia\n"));
         }else {
            write(STDOUT_FILENO, "Hijo, leido de la FIFO \"", sizeof("Hijo, leido de la FIFO \""));
            write(STDOUT_FILENO, buff, leido-1);
            write(STDOUT_FILENO, "\"\n", sizeof("\"\n"));
         }

//CIERRO FIFO
close(fifo_d);