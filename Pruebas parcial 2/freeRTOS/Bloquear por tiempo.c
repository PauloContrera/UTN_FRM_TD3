#include "FreeRTOS.h"
#include "task.h"
#include "board.h"
#include "supporting_functions.h"

#define mainDELAY_LOOP_COUNT            ( 0xffffff )

void vTask1( void *pvParameters );

//La tarea debe cambiar el estado del LED rojo, enviar por puerto serie el texto recibido como parámetro en su creación y bloquearse durante 800ms.
//la funcion es continua no perriodca, prioridad baja

// *Para bloquear tareas por tiempo definido, tenemos que pasar los milisegundos a tips con pdMS_TO_TICKS(milisegundos)
// const TickType_t xDelay = pdMS_TO_TICKS( 800 );
// La variable pasa los segundos y los almacena en xDelay
// *Y vTaskDelay() es la funcion que bloquea por un tiempo determinado
// vTaskDelay(xDelay);


int main( void )
{
   /*Crear una tarea unica o varias tareas, cambiando el nombre en el primer valor */
xTaskCreate( vTask1, NULL, configMINIMAL_STACK_SIZE, (void*)pvTaskParameters, tskIDLE_PRIORITY+1,NULL );
   // !Todas las varialbles tiene que ser iniciadas con lo de abajo
   vTaskStartScheduler();
   for( ;; );
   return 0;
}

void vTask1 ( void *pvParameters )
{
char *pcTaskName;
const TickType_t xDelay = pdMS_TO_TICKS( 800 );
pcTaskName = ( char * ) [pvParameters];
[for( ;; )]
{
vPrintString( pcTaskName );
Board_LED_Toggle(4); //LED rojo;
vTaskDelay( xDelay);
}
}