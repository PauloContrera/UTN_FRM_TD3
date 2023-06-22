1- La tarea vTask1 debe ejecutarse una única vez y debe ser la primera en ejecutarse.
2- vTask2 debe ser la primer tarea en crearse.
3- vTask2 debe pasar a estado "Lista" exactamente cada 300ms y cambiar el estado del LED verde.
int main( void )
{
[xTaskCreate]( [vTask2], NULL, configMINIMAL_STACK_SIZE, NULL, [tskIDLE_PRIORITY+1], NULL );
[xTaskCreate]( [vTask1], NULL, configMINIMAL_STACK_SIZE, NULL, [tskIDLE_PRIORITY+2], NULL );
[vTaskStartScheduler()];
for( ;; );
return 0;
}
void vTask1 ( void *pvParameters )
{
for ( ;; ) {
vPrintString( "Ejecución única\r\n" );;
[vTaskDelete( NULL )];
}
}
void vTask2 ( void *pvParameters )
{
TickType_t xLastWakeTime;
const TickType_t xPeriod = [pdMS_TO_TICKS]( 300 );
xLastWakeTime = [xTaskGetTickCount()];
for ( ;; ) {
Board_LED_Toggle(5); //LED verde;
[vTaskDelayUntil]( [&xLastWakeTime, xPeriod] );
}
}