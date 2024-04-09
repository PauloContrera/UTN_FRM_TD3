#include "FreeRTOS.h"
#include "task.h"
#include "board.h"
#include "supporting_functions.h"

#define mainDELAY_LOOP_COUNT            ( 0xffffff )

void vTask1( void *pvParameters );

// *Para bloquear tareas por tiempo indefido, se usa portMAX_DELAY
// las tareas se bloquean si el medio que utilizan esta vacio al recibir datos o esta lleno al enviar datos

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
   for( ;; );
   return 0;
}

void vTask1 ( void *pvParameters )
{
int32_t lValueToSend = 123;
const TickType_t xDelay = pdMS_TO_TICKS( 1000 );//!Esto no es necesario, porque no se usa
for ( ;; ) {
xQueueSendToBack( xQueue, &lValueToSend, portMAX_DELAY );
// xQueueSendToBack, y xQueue es de tuberia fifo
}
}
