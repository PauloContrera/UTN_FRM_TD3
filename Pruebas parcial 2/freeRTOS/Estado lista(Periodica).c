
/* FreeRTOS.org includes. */
#include "FreeRTOS.h"
#include "task.h"

/* LPCOpen includes*/
#include "board.h"

/* Demo includes. */
#include "supporting_functions.h"

/* Used as a loop counter to create a very crude delay. */
#define mainDELAY_LOOP_COUNT            ( 0xffffff )


// *Las tareas periodicas tiene que tener la prioridad mas alta
//tarea 2 es la que usa lista, la funcion es periodica por 


int main( void )
{

xTaskCreate(vTask1, NULL, configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+2, NULL );
xTaskCreate(vTask2, NULL, configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+3, NULL );
vTaskStartScheduler();
for( ;; );
return 0;


   /*Crear una tarea unica o varias tareas, cambiando el nombre en el primer valor */
   xTaskCreate( vTask1,                   /* nombre de la tarea */
                (const char *)"Task 1",   /* ni idea, suele ser null */
                configMINIMAL_STACK_SIZE, /* configuraciones */
                NULL,                     /* Varibales que van ir en la funcion, en este caso nulo, las variables van con (*void)  */
                tskIDLE_PRIORITY+5,       /* Prioridades, al ser oeruiduca tiene que ser la mas alta */
                NULL );                   /* ni idea */

   // !Todas las varialbles tiene que ser iniciadas con lo de abajo
   vTaskStartScheduler();
   for( ;; );
   return 0;
}
void vTask2 ( void *pvParameters )
{
TickType_t xLastWakeTime; //Declaramos un variable, donde vamos almacenar los datos del procreso (Menor a 750ms)
const TickType_t xPeriod = pdMS_TO_TICKS( 750 ); //Ponemos el maximo tiempo, la funcion va a ser periodica, pasamos los 750ms a ciclos de maquinas
xLastWakeTime = xTaskGetTickCount();//xTaskGetTickCount() nos devuelve la duracion del ultimo proceso
for( ;; ) {
Board_LED_Toggle(5); //LED verde;
vTaskDelayUntil( &xLastWakeTime, xPeriod );// Repite la tarea ciclica, cada 750ms
// Se envian primero el tiempo del ultimo ciclo, y el programa rellena hasta llegar a 750ms
}
}
void vTask1 ( void *pvParameters )
{
for( ;; ) {
vPrintString( "Tarea continua en ejecución\r\n" );;
}
}


/*
1- Posea una tarea vTask1 que se ejecute de manera continua e informe este estado por el puerto serie.
2- vTask1 se debe crear en primer lugar.
3- Posea una tarea vTask2 que pase a estado "Lista" exactamente cada 750ms y cambie el estado del LED verde.
*/