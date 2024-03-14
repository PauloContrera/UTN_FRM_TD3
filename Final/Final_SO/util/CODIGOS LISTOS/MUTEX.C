//INIACIALIZACIÍON ESTÁTICA DEL MUTEX GLOBAL
pthread_mutex_t mtx=PTHREAD_MUTEX_INITIALIZER;
int s ; 
//DEFINE ATRIBUTOS DE MUTEX
pthread_mutexattr_t mtxattr;
-------------------------------------------------------------

//INICIALIZA LOS ATRIBUTOS MUTEX
s=pthread_mutexattr_init(&mtxattr);
-------------------------------------------------------------

//SETEA EL TIPO EN ATRIBUTOS MUTEX
s=pthread_mutexattr_settype(&mtxattr,PTHREAD_MUTEX_NORMAL);// PTHREAD_MUTEX_NORMAL: Esto significa que los hilos se bloquean en el orden en que solicitaron el bloqueo, sin tener en cuenta ninguna prioridad o política de planificación.

-------------------------------------------------------------
//INICIALIZA EL MUTEX
s=pthread_mutex_init(&mtx,&mtxattr);

-------------------------------------------------------------
//BLOQUEO DEL MUTEX Y REALIZACIÓN DEL TAREA
s=pthread_mutex_lock(&mtx);  //SIEMPRE SE EJECUTA EN ESTE CASO LA SUMA DE 20 + 5 EN EL MAIN, PORQUE ESA OPERACION NO DEPENDE DEL HILO
    if (s!=0){
        printf("ERROR; pthread_mutex() = %d\n", s);
        exit(-1);   }
            .
            .
            .
            .
    TAREA A REALIZAR
            .
            .
            .
            .
-------------------------------------------------------------

//DESBLOQUEO DEL MUTEX
s=pthread_mutex_unlock(&mtx); 
    if (s!=0){
        printf("ERROR; pthread_mutex() = %d\n", s);
        exit(-1);    }
    
-------------------------------------------------------------
//DESTRUIR MUTEX
s=pthread_mutex_destroy(&mtx);