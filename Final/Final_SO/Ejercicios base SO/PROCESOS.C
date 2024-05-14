//DEFINO VARIABLES
pid_t pid;
int i;
------------------------------------------------------------------------------------------------------------------------

//CREO HIJO
pid = fork();
	printf ("Mi pid es %d y el pid de papa es %d. fork() devolvio %d\n", getpid(), getppid(), pid);

------------------------------------------------------------------------------------------------------------------------
//CONFIRMNACIÓN DE PADRE E HIJO
if (pid < 0) {
        fprintf(stderr, "fork falló\n");
        exit(1);
        
    } else if (pid == 0) { // proceso hijo
        printf("Proceso hijo  (PID=%d) está corriendo.\n", getpid());
        
        sleep(1); 
        
        printf("Proceso hijo  (PID=%d) está saliendo.\n", getpid());

        exit(0);
    
    } else { // proceso padre
        
        printf("Proceso padre (PID=%d) está corriendo.\n", getpid());
        
        sleep(10); 
                
        printf("Proceso padre (PID=%d) está saliendo.\n", getpid());
        
        //~ wait(NULL);//PARA ESPEERAR QUE HIJO TERMINE
    }