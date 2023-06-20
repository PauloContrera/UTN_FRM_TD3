/*
 * PARCIAL 1: programa para ser ejecutado en el proceso padre.
 * 
 * Comando para compilar:
 * 
 * 	gcc -c padre.c && gcc hijo.o padre.o -o padre

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

#define MENSAJE "HOLA HIJA MIA"

int hijo (int fd[2]);

int main(void) 
{
	int fd[2];
	int pid;
	char tx_buffer[20] = MENSAJE;
	
    if (pipe(fd) == -1)
    {
        perror("pipe");
        return -1;
    }
    
    if ((pid = fork()) == -1)
    {
        perror("fork");
        return -1;
    }
    
// codigo que ejecuta el hijo
	if (pid==0) 
	{ 
        hijo(fd);

        return 0;
    }
// codigo que ejecuta el padre   
	else
	{
		printf("Proceso Padre en ejecucion... \n"); 
		
		sleep(1);
				
		// **** RESOLUCION DEL EJERCICIO *******************************
    aca va todo 
// ** RESOLUCION DEL EJERCICIO ***********
    
	// ~Tuberias
	write (fd[1], tx_buffer, sizeof(tx_buffer));	//!Escritura 
	// leido = read (fd[0], tx_buffer, sizeof(tx_buffer)); //!Lectura de tuberia
	//write(STDOUT_FILENO, tx_buffer, leido-1); //!imprime en consola


	//~FIFO
	/* Difine carpera de de la fifo, colocar arriba donde se declara variables
		#define FIFO_PATH "/tmp/MI_FIFO"
	*/
		/* 
			*Lectura 
        fifo_d = open(FIFO_PATH, O_WRONLY, 0); //!Abrimos la fifo
        leido = read(fifo_d, tx_buffer, sizeof(tx_buffer)); 
        write(STDOUT_FILENO, tx_buffer, leido-1); //!imprime en consola
        close(fifo_d); //!Cerramos la fifo
		*/

		/*
			*Escritura
		fifo_d = open(FIFO_PATH, O_WRONLY, 0); //!Abrimos la fifo
        write(fifo_d, tx_buffer, sizeof(tx_buffer));
        close(fifo_d); //!Cerramos la fifo
		*/

		//~Cola Mensajes
			/*

		//*Parametros antes de la cola(Cambiar)
			int err, leido;
   			char buff[1024];   
   			mqd_t mqd; 
   			struct mq_attr attr, attr_rcv;
   			// Se fijan algunos parametros de la cola de mensajes antes de crearla
   			attr.mq_msgsize = sizeof(buff);
   			attr.mq_maxmsg = 5;


	    mqd = mq_open(MQ_PATH, O_RDWR | O_CREAT, 0777, &attr); 
  	 	mq_send(mqd, MENSAJE, strlen(MENSAJE)+1, 1);  //strlen nos da la longitud de una cadena

   		leido = mq_receive(mqd, buff, attr_rcv.mq_msgsize, 0);
   		printf("Mensaje leido: %s\n", buff);
   		mq_close(mqd);
		
			*/
		

		// *********************
		// *************************************************************
		
		wait(NULL);  
		exit(0);
	}
}
