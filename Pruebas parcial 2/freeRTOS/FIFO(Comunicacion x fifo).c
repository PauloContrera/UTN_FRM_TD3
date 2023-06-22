#include "FreeRTOS.h"
#include "task.h"
#include "board.h"
#include "supporting_functions.h"

#define mainDELAY_LOOP_COUNT            ( 0xffffff )

void vTask1( void *pvParameters );

//* Las Fifo se tiene que definir en las variables globales, crear en el main, y envian y reciben datos en las tareas



QueueHandle_t xQueue; //!declaramos que xQueue va a ser una fifo


int main( void )
{
   /*Crear una tarea unica o varias tareas, cambiando el nombre en el primer valor */
   xTaskCreate( vTask1,                   /* nombre de la tarea */
                (const char *)"Task 1",   /* ni idea, suele ser null */
                configMINIMAL_STACK_SIZE, /* configuraciones */
                NULL,                     /* Varibales que van ir en la funcion, en este caso nulo, las variables van con (*void)  */
                tskIDLE_PRIORITY+1,       /* Prioridades */
                NULL );                   /* ni idea */

   // !Todas las varialbles tiene que ser iniciadas con lo de abajo
   vTaskStartScheduler();

//*Datos de FIFO
UBaseType_t uxQueueLength = 10; //Longitud
UBaseType_t uxItemSize = sizeof( int32_t );//Tipos de datos creo

//*xQueue a sido definida como una varible global, de fifo y con xQueueCreate()creamos la FIFO
xQueue = xQueueCreate( uxQueueLength, uxItemSize );
//Las tareas van a usar la fifo para comunicarse entre ellas

   for( ;; );
   return 0;
}

void vTask1 ( void *pvParameters )
{
   //Tarea 1 va a enviar los datos 123
int32_t lValueToSend = 123;
const TickType_t xDelay = pdMS_TO_TICKS( 1000 );//!no es necesario
for ( ;; ) {
// la funcion(El nombre de la FIFO, datos, variable de tiempo)
xQueueSendToBack( xQueue, &lValueToSend, portMAX_DELAY );
// en este caso pide que si esta llena se bloquee por tiempo indefinido por eso portMAX_DELAY

// tambien existe xQueueSendToFront, o xQueueSendToFrontFromISR, o xQueueSendToBackFromISR. Se usa la de arriba
}
}
void vTask2 ( void *pvParameters )
{
   //Tarea 2 va a recibir los datos y los va a guardar en lReceivedValue
int32_t lReceivedValue;
const TickType_t xDelay = [pdMS_TO_TICKS]( 1000 );
for ( ;; ) {
// la funcion(El nombre de la FIFO, datos, variable de tiempo)
xQueueReceive( xQueue, &lReceivedValue, portMAX_DELAY );
// en este caso pide que si esta vacio se bloquee por tiempo indefinido por eso portMAX_DELAY

vPrintString( "Dato recibido\r\n" );
}
}


/*
1- La tarea vTask1 debe enviar a través de una cola de mensajes FIFO el valor entero 123 de manera continua.
2- En el caso de que la cola esté llena, vTask1 se debe bloquear por tiempo indefinido.
3- La tarea vTask2 debe recibir los datos enviados por vTask1.
4- En el caso de que la cola esté vacía, vTask2 se debe bloquear por tiempo indefinido.
*/