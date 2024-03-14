//funcion que se hará dentro del hilo
void *hola(void * nro) {
  
   sleep(2);   // El hilo espera 2 segundos
   printf("Hola, soy el hilo %d\n", * (int*) nro);   // Se imprime un mensaje que indica el número del hilo
   pthread_exit(NULL);   // Se finaliza el hilo

}
------------------------------------------------------------------------------------------------------------------------

//DECLARO ARRAY DE HILOS Y VARIABLES
pthread_t hilo[1];
int rc, t;// Declaración de variables para almacenar el código de retorno de pthread_create y el número de hilo
 t = 0;   // Inicialización del número de hilo
------------------------------------------------------------------------------------------------------------------------

//CREACIÓN DE HILOS
rc = pthread_create(&hilo[0], NULL, hola , (void *)(&t)  );
------------------------------------------------------------------------------------------------------------------------

//ESPERA DE FINALIZACIÓN DEL HILO
pthread_join(hilo[0],NULL);
------------------------------------------------------------------------------------------------------------------------

//FINALIZO EL HILO
pthread_exit(NULL);
