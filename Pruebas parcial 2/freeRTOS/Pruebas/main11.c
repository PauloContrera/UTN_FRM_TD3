1- Cree dos tareas: vTask1 y vTask2.
2- vTask1, creada en primer lugar, periódicamente modifica la prioridad de vTask2 para que esta pueda ejecutarse.
TaskHandle_t xTaskHandle;
int main( void )
{
[xTaskCreate]( [vTask1], NULL, configMINIMAL_STACK_SIZE, NULL, [tskIDLE_PRIORITY+2], [NULL]);
[xTaskCreate]( [vTask2], NULL, configMINIMAL_STACK_SIZE, NULL, [tskIDLE_PRIORITY+1], [&xTaskHandle]);
[vTaskStartScheduler()];
for( ;; );
return 0;
}
void vTask1 ( void *pvParameters )
{ ... }
void vTask2 ( void *pvParameters )
{ ... }