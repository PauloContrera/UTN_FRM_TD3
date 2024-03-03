
///////////////////////EJERCICIO 1 ///////////////////////////////////////////////////////////////
/*
Tres procesos (A,B,C) NO relacionados se comunican de la siguiente forma:
A y B se comunican por medio de un socket datagrama
A y C se comunican por medio de una cola de mensajes

1) El proceso B crea el socket datagrama servidor
2) El proceso A crea el socket datagrama cliente y la cola de mensajes
3) El proceso A cuando recibe la señal SIGUSR1 escribe en el socket "0123456789" 
4) El proceso A cuando recibe la señal SIGUSR2 escribe en la cola de mensaje el mensaje "ABCDEFGHIJ"
5) El proceso B lee el socket y lo recibido lo muestra en pantalla
6) El proceso B lee la cola de mensaje y lo recibido lo muestra en pantalla

El proceso B debe ser el primero en comenzar en una consola
El proceso A debe ser el segundo en comenzar en otra consola
El proceso C debe ser el tercero en comenzar en otra consola
Las señales se envían desde otra consola


 ---      ---------------       ---      --------  
|   |--->|Socket datagrma |--->| B |--->|Pantalla|  
| A |     ----------------      ---      --------
|   |     ----------------      ---      --------  
|   |--->|Cola de mensajes|--->| C |--->|Pantalla|  
 ---      ----------------      ---      --------

*/

/////////////////////// EJERCICIO 2 ///////////////////////////////////////////////////////////////
/*
Tres procesos (A,B,C) NO relacionados se comunican de la siguiente forma:
A y B se comunican por medio de un socket datagrama
A y C se comunican por medio de una FIFO

1) El proceso B crea el socket datagrama servidor
2) El proceso A crea el socket datagrama cliente y la FIFO
3) El proceso A cuando recibe la señal SIGUSR1 escribe en el socket "0123456789" 
4) El proceso A cuando recibe la señal SIGUSR2 escribe en la FIFO "ABCDEFGHIJ"
5) El proceso B lee el socket y lo recibido lo muestra en pantalla
6) El proceso C lee la FIFO y lo recibido lo muestra en pantalla

El proceso B debe ser el primero en comenzar en una consola
El proceso A debe ser el segundo en comenzar en otra consola
El proceso C debe ser el tercero en comenzar en otra consola
Las señales se envían desde otra consola


 ---      ---------------       ---      --------  
|   |--->|Socket datagrma |--->| B |--->|Pantalla|  
| A |     ----------------      ---      --------
|   |     ----------------      ---      --------  
|   |--->|     FIFO	    |--->| C |--->|Pantalla|  
 ---      ----------------      ---      --------
*/

/////////////////////////EJERICIO 3 //////////////////////////////////////////////////////////////
/*

Cuatro procesos (A,B,C,D) relacionados de la siguiente forma:
A padre de B y padre de C

Los procesos se comunican de la siguiente forma:
A y B se comunican por medio de un pipe
A y C se comunican por medio de un pipe
A y D se comunican por medio de un socket datagrama

1) El proceso D crea el socket datagrama servidor
2) El proceso A crea el socket datagrama cliente, crea al proceso B y al proceso C 
3) El proceso B cuando recibe la señal SIGUSR1 escribe en el pipe1 el mensaje "0123456789" y termina.
4) El proceso C cuando recibe la señal SIGUSR2 escribe en el pipe2 el mensaje "ABCDEFGHIJ" y termina.
5) El proceso A espera a que termine el proceso B, lee el pipe1 y lo leído lo envía al socket datagrama servidor
6) El proceso A espera a que termine el proceso C, lee el pipe2 y lo leído lo envía al socket datagrama servidor
7) El proceso D lee el socket datagrama y muestra lo leído en pantalla

El proceso D debe ser el primero en comenzar en una consola
El proceso A comienza luego de D otra consola
Las señales se envían desde otra consola


       --- 
      | B |
       --- 
       |     -------      ---      --------      ---      --------  
       |--->| PIPE1 |--->|   |    | Socket |    |   |    |        |  
       |     -------     | A |--->|Datagama|--->| D |--->|pantalla|
       |     -------     |   |--->|        |--->|   |--->|        | 
       |--->| PIPE2 |--->|   |     --------      ---      --------  
       |     -------      ---
       ---              
      | C |
       --- 
*/


/////////////////////////EJERICIO 4 //////////////////////////////////////////////////////////////
/*
Cuatro procesos (A,B,C,D) relacionados de la siguiente forma:
A padre de B y padre de C

Los procesos se comunican de la siguiente forma:
A y B se comunican por medio de un pipe
A y C se comunican por medio de un pipe
A y D se comunican por medio de una FIFO

1) El proceso A crea la FIFO, crea al proceso B y al proceso C 
2) El proceso B cuando recibe la señal SIGUSR1 escribe en el pipe1 el mensaje "0123456789" y termina.
3) El proceso C cuando recibe la señal SIGUSR2 escribe en el pipe2 el mensaje "ABCDEFGHIJ" y termina.
4) El proceso A espera a que termine el proceso B, lee el pipe1 y lo leído lo envía a la FIFO
5) El proceso A espera a que termine el proceso C, lee el pipe2 y lo leído lo envía a la FIFO
6) El proceso D lee los mensajes de la FIFO (2 mensajes) y los muestra en pantalla

El proceso A debe ser el primero en comenzar en una consola
El proceso D comienza luego de A otra consola
Las señales se envían desde otra consola


       --- 
      | B |
       --- 
       |     -------      ---      --------      ---      --------  
       |--->| PIPE1 |--->|   |    |        |    |   |    |        |  
       |     -------     | A |--->|  FIFO  |--->| D |--->|pantalla|
       |     -------     |   |--->|        |--->|   |--->|        | 
       |--->| PIPE2 |--->|   |     --------      ---      --------  
       |     -------      ---
       ---              
      | C |
       --- 
*/
/////////////////////////EJERICIO 5 //////////////////////////////////////////////////////////////
/*
Cuatro procesos (A,B,C,D) relacionados de la siguiente forma:
A padre de B y padre de C

Los procesos se comunican de la siguiente forma:
A y B se comunican por medio de un pipe
A y C se comunican por medio de un pipe
A y D se comunican por medio de una cola de mensajes

1) El proceso A crea la cola de mensajes, crea al proceso B y al proceso C 
2) El proceso B cuando recibe la señal SIGUSR1 escribe en el pipe1 el mensaje "0123456789" y termina.
3) El proceso C cuando recibe la señal SIGUSR2 escribe en el pipe2 el mensaje "ABCDEFGHIJ" y termina.
4) El proceso A espera a que termine el proceso B, lee el pipe1 y lo leído lo envía a la cola de mensajes 
5) El proceso A espera a que termine el proceso C, lee el pipe2 y lo leído lo envía a la cola de mensajes 
6) El proceso D lee los mensajes de la cola mensajes (2 mensajes) y los muestra en pantalla

El proceso A debe ser el primero en comenzar en una consola
El proceso D comienza luego de A otra consola
Las señales se envían desde otra consola


       --- 
      | B |
       --- 
       |     -------      ---      --------      ---      --------  
       |--->| PIPE1 |--->|   |    | Cola   |    |   |    |        |  
       |     -------     | A |--->|Mensajes|--->| D |--->|pantalla|
       |     -------     |   |--->|        |--->|   |--->|        | 
       |--->| PIPE2 |--->|   |     --------      ---      --------  
       |     -------      ---
       ---              
      | C |
       --- 
*/

/////////////////////////EJERICIO 6//////////////////////////////////////////////////////////////
/*
Cuatro procesos (A,B,C,D) relacionados de la siguiente forma:
A padre de B y padre de C

Los procesos se comunican de la siguiente forma:
A y B se comunican por medio de una FIFO
A y C se comunican por medio de una FIFO
A y D se comunican por medio de una cola de mensajes

1) El proceso A crea la cola de mensajes, crea al proceso B y al proceso C 
2) El proceso B cuando recibe la señal SIGUSR1 escribe en el FIFO1 el mensaje "0123456789" y termina.
3) El proceso C cuando recibe la señal SIGUSR2 escribe en el FIFO2 el mensaje "ABCDEFGHIJ" y termina.
4) El proceso A espera a que termine el proceso B, lee el FIFO1 y lo leído lo envía a la cola de mensajes 
5) El proceso A espera a que termine el proceso C, lee el FIFO2 y lo leído lo envía a la cola de mensajes 
6) El proceso D lee los mensajes de la cola mensajes (2 mensajes) y los muestra en pantalla

El proceso A debe ser el primero en comenzar en una consola
El proceso D comienza luego de A otra consola
Las señales se envían desde otra consola


       --- 
      | B |
       --- 
       |     -------      ---      --------      ---      --------  
       |--->| FIFO1 |--->|   |    | Cola   |    |   |    |        |  
       |     -------     | A |--->|Mensajes|--->| D |--->|pantalla|
       |     -------     |   |--->|        |--->|   |--->|        | 
       |--->| FIFO2 |--->|   |     --------      ---      --------  
       |     -------      ---
       ---              
      | C |
       --- 
*/
/////////////////////////EJERICIO SI TENEMOS TIEMPO //////////////////////////////////////////////////////////////
/* Con SIGUSR1 se produce y con SIGUSR2 se consume, el consumidor cada vez que tiene un dato lo envia mediante tcp */


/////////////////////////EJERICIO 7//////////////////////////////////////////////////////////////
/*

Tres procesos (A,B,C) se comunican de la siguiente forma:
A y B se comunican por medio de PIPE
A y C se comunican por medio de un socket datagrama

1) El proceso A crea al proceso B
2) El proceso A crea un socket datagrama servidor
3) El proceso C (cliente) crea un socket datagrama cliente
4) El proceso C (cliente) cuando recibe la señal SIGTSTP (Ctrl+z) le envia al servidor el mensaje "ABCDEFGHIJ"
5) El proceso A (servidor) cuando recibe algo por el socket lo lee lo muestra por pantalla y se lo escribe en el PIPE
6) El proceso B al recibir  la señal SIGTSTP (Ctrl+z) lee el PIPE y muestra lo leido
7) El proceso A cuando recibe la señal SIGINT (Ctrl+c) cierra el pipe, cierra el socket y termina
8) El proceso C cuando recibe la señal SIGINT (Ctrl+c) cierra el socket y termina

       --- 		  ---		     ---
      | C |		 | A |		    | B |
       --- 		  ---		     --- 
	|     ------     | |     ------     | |     --------
	|--->|socket|--->| |--->| PIPE |--->| |--->|pantalla|
	      ------		  ------             --------

C ---> /_socket_/--> A --> /_PIPE_/ --> B --> /_pantalla_/
*/

/////////////////////////EJERICIO 8//////////////////////////////////////////////////////////////
/*
Tres procesos (A,B,C) No relacionados se comunican de la siguiente forma:
A y B se comunican por medio de una cola de mensajes
A y C se comunican por medio de un socket stream
1) El proceso A crea la cola de mensajes
2) El proceso A crea un socket stream servidor
3) El proceso C (cliente) crea un socket stream cliente 
4) El proceso C (cliente) cuando recibe la se al SIGTSTP (Ctrl+z) le env a al servidor el mensaje  "ABCDEFGHIJ"
5) El proceso B abre la cola de mensajes
6) El proceso A cuando recibe la señal SIGINT (Ctrl+c) cierra el socket y borra la cola de mensajes 
7) El proceso A (servidor) cuando recibe algo en por el socket lo lee lo muestra en pantalla y se lo env a al proceso B por medio de la cola de mensajes
8) Proceso B la cola de mensajes un mensaje y muestra lo leido
El proceso A debe ser el primero en ejecutarse. 

       ---              ---                ---  
      | C |            | A |              | B |
       ---              ---                ---
       |     ------     | |     ------     | |     --------  
       |--->|socket|--->| |--->|ColaMe|--->| |--->|pantalla|  
             ------             ------             --------

   c --> /_Socket_/--> A --> /_ColaMe_/ --> C --> /_Pantalla_/ 
 
*/

/////////////////////////EJERICIO 9//////////////////////////////////////////////////////////////
/*
Tres procesos NO relacionados (A,B,C) se comunican de la siguiente forma:
A y B se comunican por medio de un FIFO
A y C se comunican por medio de un socket datagrama 
1) El proceso A crea un socket datagrama servidor
2) El proceso A crea y abre la FIFO con permisos de solo escritura 
3) El proceso B abre la FIFO con permisos de solo lectura 
4) El proceso C crea un socket datagrama cliente 
5) El proceso C esta en una espera activa y cuando recibe la señal SIGTSTP(Ctrl + z) le envía al servidor el mensaje "ABCDEFGHIJ"
6) El proceso A (servidor) cuando recibe algo en por el socket lo lee lo muestra en pantalla y lo escribe en la FIFO 
7) El Proceso B está en una espera activa y al recibir la señal SIGTSTP(Ctrl + z) leer la FIFO y muestra lo leído

El proceso A debe ser el primero en ejecutarse
El proceso B debe ejecutarse segundo
El proceso C debe ejecutarse tercero

       ---              ---                ---  
      | C |            | A |              | B |
       ---              ---                ---
       |     ------     | |     ------     | |     --------  
       |--->|socket|--->| |--->|FIFO  |--->| |--->|pantalla|  
             ------             ------              --------

   C --> /_socket_/--> A --> /_FIFO_/ --> C --> /_Pantalla_/ 
*/

/////////////////////////EJERICIO 10//////////////////////////////////////////////////////////////
/*
Dos hilos (A y B) de un proceso se comunican por medio de una Tuberia.
El hilo A escribe en la Tuberia "0123456789" en forma atomica.
El hilo B escribe en la misma Tuberia "ABCDEFGHIJ" en forma atomica.
El hilo main lee los mensajes desde esta Tuberia y muestra lo leido en consola.
*/

/////////////////////////EJERICIO 11//////////////////////////////////////////////////////////////
/* EJERCICIO FINAL, SISTEMA OPERATIVO
  
 Un proceso crea 200 hilos.
 100 hilos se asocian con la función incrementar().
 Los 100 hilos restantes se asocian con la función decrementar().
 Se debe crear la función decrementar(), la cual debe restar la variable en lugar de sumarla.
 Al final de la ejecución de los 200 hilos la variable "total" debe valer cero (0).
 
*/

/////////////////////////EJERICIO 12//////////////////////////////////////////////////////////////
/*
Dos procesos (A y B) se comunican por medio de una Cola de Mensaje, del siguiente modo:
1) El proceso A crea la cola de mensaje y crea al proceso B.
2) Proceso B al recibir la señal SIGINT escribe en la Cola de Mensaje "0123456789" y termina.
3) Proceso A espera que B termine y lee de la Cola de Mensaje lo escrito por el proceso B y muestra lo leído y elimina la cola de mensajes.

Considerar los procesos con la siguiente jerarquía: Proceso A (padre), Proceso B (hijo de A).
*/


//Compile el programa: gcc -o name name.c -lpthread -lrt
//Ejecute el programa ./name

/////////////////////////EJERICIO 13//////////////////////////////////////////////////////////////
/*
Tres hilos (main, A y B) de un proceso se comunican por medio de una Cola de Mensaje.
1) El hilo A escribe en la Cola de Mensaje "0123456789" en forma atomica y termina.
2) El hilo B espera que el hilo A termine
3) El hilo B escribe en la Cola de Mensaje "ABCDEFGHIJ" en forma atomica y termina.
4) El hilo main espera que termine el Hilo B.
5) El hilo main lee los mensajes de la cola de mensajes muestra lo leido y elimima la cola de mensajes.
*/

/////////////////////////EJERICIO 14//////////////////////////////////////////////////////////////
/*
Tres procesos (A, B y C) se comunican por medio de una Cola de Mensaje.
1) El proceso C envia una señal (SIGUSR1) al proceso A. El proceso A escribe en la Cola de Mensaje "0123456789" y termina
2) El proceso C espera a que termine el procesos A, lee de la Cola de Mensaje y muestra lo leido.
3) El proceso C envia una señal (SIGUSR2) al proceso B. El proceso B escribe en la Cola de Mensaje "ABCDEFGHIJ" y termina.
4) El proceso C espera a que termine el procesos B, lee de la Cola de Mensaje, muestra lo leido y elimina la cola de mensajes.

Considerar los procesos con la siguiente jerarquia: Proceso C (padre), Proceso B (hijo1 de C), Proceso A (hijo2 de C)
*/

/////////////////////////EJERICIO 15//////////////////////////////////////////////////////////////
/*
Tres procesos (A, B y c) se comunican por medio de una Cola de Mensaje, del siguiente modo:
1) El Proceso C crea la cola de mensaje y crea al proceso B.
2) El Proceso B crea al proceso A.
3) Proceso A escribe en la Cola de Mensaje "0123456789" y termina.
4) Proceso B espera que A termine y lee de la Cola de Mensaje lo escrito por el proceso A y lo muestra 
5) Luego el Proceso B escribe en la Cola de Mensaje "ABCDEFGHIJ" y termina.
6) El Proceso C espera que B termine, lee la Cola de Mensaje,  muestra lo leido y elimina la cola de mensajes.

Considerar los procesos con la siguiente jerarquia: Proceso C (padre), Proceso B (hijo de C), Proceso A (hijo de B),

*/

/////////////////////////EJERICIO 16//////////////////////////////////////////////////////////////
/*
Tres procesos (A, B y C) se comunican por medio de 2 Tuberías.
1) El proceso A escribe en una Tubería "0123456789" y termina
2) El proceso B escribe en otra Tubería "ABCDEFGHIJ" y termina.
3) El proceso C lee de las Tuberías después que A y B terminaron y muestra lo leído.

Considerar los procesos con la siguiente jerarquía: Proceso C (padre), Proceso B (hijo 1 de C), Proceso A (hijo 2 de C)
*/

/////////////////////////EJERICIO 17//////////////////////////////////////////////////////////////
/*
Tres procesos (A, B y C) se comunican por medio de 2 Tuberias.
1) El proceso C envia una señal (SIGUSR1) al proceso A. El proceso A al recibir esta señal escribe en una Tuberia "0123456789" y termina
2) El proceso C lee de la Tuberia despues que A termina, muestra lo leido y desturye la tuberia.
3) El proceso C envia una señal (SIGUSR2) al proceso B. El proceso B al recibir esta señal escribe en una Tuberia "ABCDEFGHIJ" y termina
4) El proceso C lee de la Tuberia despues que B termina, muestra lo leido y destruye la tuberia.

Considerar los procesos con la siguiente jerarquia: Proceso C (padre), Proceso B (hijo 1 de C), Proceso A (hijo 2 de C)
*/

/////////////////////////EJERICIO 18//////////////////////////////////////////////////////////////

/*
Tres procesos (A,B,C) se comunican de la siguiente forma:
C y A se comunican por medio de un pipe
C y B se comunican por medio de una cola de mensajes

1) El proceso C crea el pipe 
2) El proceso C crea al proceso A.
3) El proceso C crea la cola de mensajes
4) El proceso C cuando recibe la se�al SIGUSR1 escribe en el pipe "0123456789" 
5) El proceso C cuando recibe la se�al SIGUSR2 escribe en la cola de mensajes "ABCDEFGHIJ" 
6) El proceso A lee el pipe y muestra en pantalla lo le�do
7) El proceso B abre la cola de mensajes
8) El proceso B lee el mensaje de la cola de mensajes y muestra en pantalla lo le�do

El proceso C es el encargado de crear el pipe y la cola de mensajes, es el primer proceso que se ejecuta.
Las se�ales se env�an desde otra consola

 ---      -----------------      ---      --------  
|   |--->|     pipe        |--->| A |--->|Pantalla|  
| C |     -----------------      ---      --------
|   |     -----------------      ---      --------  
|   |--->|Cola de mensajes |--->| B |--->|Pantalla|  
 ---      -----------------      ---      --------
 
*/

//Compile el programa: gcc -o name name.c -lpthread -lrt

/////////////////////////EJERICIO 19//////////////////////////////////////////////////////////////

/*
Tres procesos NO relacionados (A,B,C) se comunican de la siguiente forma:
A y B se comunican por medio de una cola de mensaje
A y C se comunican por medio de un socket datagrama 
1) El proceso A crea un socket datagrama servidor
2) El proceso A crea y abre la cola de mensajes con permisos de solo escritura 
3) El proceso B abre la cola de mensajes con permisos de solo lectura 
4) El proceso C crea un socket datagrama cliente 
5) El proceso C esta en una espera activa y cuando recibe la se�al SIGUSR1 le env�a al servidor el mensaje  "ABCDEFGHIJ"
6) El proceso A (servidor) cuando recibe algo en por el socket lo lee lo muestra en pantalla y lo escribe en la cola de mensajes
7) El Proceso B est� en una espera activa y al recibir la se�al SIGUSR1 leer la cola de mensajesy muestra lo le�do

El proceso A debe ejecutarse primero
El proceso B debe ejecutarse segundo
El proceso C debe ejecutarse tercero
Las se�ales se env�an desde otra consola

       ---              ---                  ---  
   S->| C |            | A |             S->| B |
       ---              ---                  ---
       |     ------     | |     --------     | |     --------  
       |--->|socket|--->| |--->|COLA MSJ|--->| |--->|pantalla|  
             ------             --------             --------

*/

//Compile el programa: gcc -o name name.c -lpthread -lrt

