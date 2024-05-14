//DEFINIR MENSAJE
#define MENSAJE "DATA PARA OTROS PROCESOS"//MENSAJE QUE SE LEE Y ESCRIBE EN COLA DE MESNAJES
#define MQ_PATH "/MQ_TD31"  // El nombre de la cola de mensajes debe empezar con "/"
------------------------------------------------------------------------------------------------------------------------

//DEFINO VARIABLES
int err, leido;
char buff[1024]; 
------------------------------------------------------------------------------------------------------------------------

//DECLARO VARIABLE DEL TIPO DESCRIPTOR COLA DE MESNAJE
mqd_t mqd;
//DEFINO ARTRIBUTOS DE LA COLA
 struct mq_attr attr, attr_rcv;
------------------------------------------------------------------------------------------------------------------------

//FIJO PARÁMETROS ANTES DE CREARLA
attr.mq_msgsize = sizeof(buff);//Tamaño máximo de los mensajes que se pueden enviar o recibir en la cola de mensajes.
attr.mq_maxmsg = 5;//Establece el número máximo de mensajes que la cola de mensajes puede conteneR
------------------------------------------------------------------------------------------------------------------------

//CREO COLA DE MENSAJES
mqd = mq_open(MQ_PATH, O_RDWR | O_CREAT, 0777, &attr);//en mqd se guarda un valor
------------------------------------------------------------------------------------------------------------------------

//ESCRIBO EN LA COLA DE MENSAJES
err = mq_send(mqd, MENSAJE, strlen(MENSAJE)+1, 1);  //strlen nos da la longitud de una cadena
                                                    //Se suma 1 al resultado de strlen(MENSAJE) para incluir el carácter nulo de terminación y asegurarse que sea enviado
                                                    //1: En cuanto a la prioridad un valor de prioridad más alto indica un mensaje más importante
------------------------------------------------------------------------------------------------------------------------

//LEER PARÁMETROS DE LA COLA DE MENSAJES
 if (mq_getattr(mqd, &attr_rcv) == -1){
      printf ("error en mq_getattr()\n");
      exit(-1); }      
   printf("Nro max. de mensajes en cola de mensajes: %ld\n", attr_rcv.mq_maxmsg);
   printf("Longitud max. de mensaje: %ld\n", attr_rcv.mq_msgsize);
   printf("Nros de mensajes pendientes en cola de mensajes: %ld\n", attr_rcv.mq_curmsgs);
------------------------------------------------------------------------------------------------------------------------

//LEER CONTENIDO DE COLA DE MENSAJES
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

------------------------------------------------------------------------------------------------------------------------

//CERRAR COLA DE MESNAJES
err = mq_close(mqd);
------------------------------------------------------------------------------------------------------------------------

//ELIMINAR COLA DE MENSAJES
err = mq_unlink(MQ_PATH);

