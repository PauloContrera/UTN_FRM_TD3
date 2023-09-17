#include "FreeRTOS.h"
#include "task.h"
#include "board.h"
#include "supporting_functions.h"

#define mainDELAY_LOOP_COUNT            ( 0xffffff )

void vTask1( void *pvParameters );

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


void vTask1( void *pvParameters )
{
   const char *pcTaskName = "Task 1 is running\r\n";
   volatile uint32_t ul;

   for( ;; ) {
      /* Print out the name of this task. */
      vPrintString( pcTaskName );

      Board_LED_Toggle(0); /* Red (RGB) */
      Board_LED_Toggle(1); /* Green (RGB) */
      Board_LED_Toggle(2); /* Blue (RGB) */
      Board_LED_Toggle(3); /* Yellow */
      Board_LED_Toggle(4); /* Red */
      Board_LED_Toggle(5); /* Green */

      /* Delay for a period. */
      for( ul = 0; ul < mainDELAY_LOOP_COUNT; ul++ ) {
         /* This loop is just a very crude delay implementation.  There is
         nothing to do in here.  Later exercises will replace this crude
         loop with a proper delay/sleep function. */
      }
   }
}


int main(void)
{
}

void vTask1(void *pvParameters)
{
    TickType_t xLastWakeTime;
    const TickType_t xPeriod = pdMS_TO_TICKS(400);

    xLastWakeTime = xTaskGetTickCount();
    for (;;)
    {
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
        vPrintString("Creación vTask2\r\n");
        xTaskCreate(vTask2, "Task2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
        vTaskDelete(NULL);
    }
}

void vTask2(void *pvParameters)
{
    for (;;)
    {
        Board_LED_Toggle(5); // LED verde
        vTaskDelay(pdMS_TO_TICKS(400));
    }
}
