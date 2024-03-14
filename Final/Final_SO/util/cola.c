/*
!    gcc -o comp/mq01 mq01.c -lrt
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <mqueue.h>

#define MENSAJE "DATA PARA OTROS PROCESOS"
#define MQ_PATH "/MQ_TD3"  // El nombre de la cola de mensajes debe empezar con "/"



int main() {
   
   int err, leido;
   char buff[1024];   
  

//* Se crea cola de mensajes  
   // Se fijan algunos parametros de la cola de mensajes antes de crearla
   mqd_t mqd; 
   struct mq_attr attr, attr_rcv;
   attr.mq_msgsize = sizeof(buff);
   attr.mq_maxmsg = 5;
    
   mqd = mq_open(MQ_PATH, O_RDWR | O_CREAT, 0777, &attr); 
   /*mqd = mq_open(
      MQ_PATH, 
      O_RDWR | O_CREAT, --> Intenta abrir como lectura y escritura pero (| significa o) sino la crea
      0777, 
      &attr); */
   if(mqd < 0) {
      write (STDOUT_FILENO, "Error al crear Cola\n", sizeof("Error al crear Cola\n"));
      // Puede que ya exista si hay error
   }else {
      write (STDOUT_FILENO, "Cola creada correctamente\n", sizeof("Cola creada correctamente\n"));
   } 
   
// *Escritura
   err = mq_send(mqd, MENSAJE, strlen(MENSAJE)+1, 1);  //strlen nos da la longitud de una cadena
   /* mq_send(descriptor, mensaje, tamaño del mensaje, prioridad);
   err = mq_send( ->Para enviar 
      mqd, -> Nombre de la cola 
      MENSAJE, ->Mensaje xD
      strlen(MENSAJE)+1, -> Longitud del mensaje (porque mas Ni idea)
      1 -> prioriedad, mientras mas alto, mas alta la prioridad (Lo atiende primero)
      );
   */
   if(err < 0) {
      write (STDOUT_FILENO, "Error al enviar mensaje\n", sizeof("Error al enviar mensaje\n"));
   }else {
      write (STDOUT_FILENO, "Mensaje enviado\n", sizeof("Mensaje enviado\n"));
   } 

//*Lectura

            //Funcion para saber que hay en la cola 
            if (mq_getattr(mqd, &attr_rcv) == -1){
               printf ("error en mq_getattr()\n");
            }
            else{
               printf("Nro max. de mensajes en cola de mensajes: %ld\n", attr_rcv.mq_maxmsg);
               printf("Longitud max. de mensaje: %ld\n", attr_rcv.mq_msgsize);
               printf("Nros de mensajes pendientes en cola de mensajes: %ld\n", attr_rcv.mq_curmsgs);
            }

   leido = mq_receive(mqd, buff, attr_rcv.mq_msgsize, 0);
   /* mq_receive(descriptor, mensaje, tamaño máximo,prioridad);
      leido = mq_receive(
      mqd, 
      buff, 
      attr_rcv.mq_msgsize, 
      0 ->
      );*/
         if(leido < 0) {
            write (STDOUT_FILENO, "Error al leer mensaje\n", sizeof("Error al leer mensaje\n"));
         }else {
            write (STDOUT_FILENO, "Mensaje leido:\n", sizeof("Mensaje leido\n:"));
            write(STDOUT_FILENO, buff, leido-1);
         }
   
// *Se cierra cola de mensajes
   err = mq_close(mqd);
   if(err < 0) {
      write (STDOUT_FILENO, "Error al cerrar la cola\n", sizeof("Error al cerrar la cola\n"));
   }else {
      write (STDOUT_FILENO, "Cola de mensajes cerrada\n", sizeof("Cola de mensajes cerrada\n"));
   } 
 
   
// *Se elimina cola de mensajes
   err = mq_unlink(MQ_PATH);
   if(err == -1){
      printf ("error en mq_unlink()\n");
      exit(-1);   
   }
   if(err < 0) {
      write (STDOUT_FILENO, "Error al eliminar la cola\n", sizeof("Error al eliminar la cola\n"));
   }else {
      write (STDOUT_FILENO, "Cola de mensajes eliminada\n", sizeof("Cola de mensajes eliminada\n"));
   } 
      
   exit(0);
}
