
/*
Tres procesos A, B y C se comunican de la siguiente forma:
A y B se comunican por medio de una cola de mensajes
A y C se comunican por medio de un Socket Stream

1) El proceso C crea un servidor socket stream 
2) El proceso A crea la cola de mensajes 
3) El proceso A al recibir la se�al SIGUSR1 escribe en la cola de mensajes el mensaje "0123456789"
4) El proceso A al recibir la se�al SIGUSR2 crea un cliente socket stream y escribe en el FIFO "ABCDEFGHIJ" 
5) El proceso C lee el el socket, muestra lo le�do en pantalla y termina
6) El proceso B esta esperando leer un mensaje, al recibir un mensaje en la cola de mensajes lo muestra en pantalla

El proceso C debe ser el primero en comenzar en una consola
El proceso A debe ser el segundo en comenzar en otra consola
Las se�ales se env�an desde otra consola

 ---      --------------      ---      --------  
|   |--->|cola mensajes |--->| B |--->|Pantalla|  
| A |     --------------      ---      --------
|   |     --------------      ---      --------  
|   |--->| SOCKET STREAM|--->| C |--->|Pantalla|  
 ---      --------------      ---      --------


*/

//Compile el programa: gcc -o name name.c -lpthread -lrt
//Ejecute el programa ./name


