//DECLARA SEMÁFORO SIN NOMBRE
sem_t sem;

--------------------------------------------------------------

//INICIALIZA SEMÁFORO
s=sem_init(&sem, 0, 0); //SEGUNDO PARÁMETRO =0, el semáforo se inicializará como no compartido entre procesos o hilos.
                        //TERCER VALOR= 0, El valor inicial del semáforo

--------------------------------------------------------------
//LEER VALOR DE SEMÁFORO
sem_getvalue(&sem, &sval);                

--------------------------------------------------------------
//INCREMENTAR SEMÁFORO
s=sem_post(&sem);

--------------------------------------------------------------
//DECREMENTAR SEMÁFORO
s = sem_wait(&sem);

--------------------------------------------------------------
//ELIMINAR SEMÁFOTO
s=sem_destroy(&sem);