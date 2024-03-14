//DEFINO FUNCION
void sigchld_handler(int signum) {
  pid_t pid;
   int status;
   pid = wait(&status);// Llamada a wait() para obtener el estado de finalización del hijo
   printf("Proceso hijo con PID %d finalizado. Estado de salida: %d\n", pid, status);
}
--------------------------------------------------------------------------------------------------

//establecer un manejador de señales para SIGCHLD, el programa puede responder cuando un proceso hijo termina
signal(SIGCHLD, sigchld_handler);//sigchld_handler es el nombre de la función que se ejecutará cuando se reciba la señal SIGCHLD
                                //SIGCHLD es una señal que se envía a un proceso padre cuando uno de sus procesos hijos termina o es interrumpido