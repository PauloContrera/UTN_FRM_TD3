1- Utilizando un mecanismo de sincronización, vTask1 debe cambiar el estado del LED verde cuando se recibe una interrupción
del RIT.
2- vTask1 debe esperar la señal de sincronización durante 1 segundo.
3- vTask1 es de alta prioridad. vTask 2 ejecuta una espera activa de baja prioridad.
SemaphoreHandle_t xBinarySemaphore;
SemaphoreHandle_t xMutex;
int main( void )
{ ... }
void vTask1 ( void *pvParameters )
{
const TickType_t xDelay = [pdMS_TO_TICKS]( 1000 );
for ( ;; ) {
if( [xSemaphoreTake]( [xBinarySemaphore, xDelay] ) == pdTRUE) Board_LED_Toggle(5); //LED verde
else Board_LED_Toggle(4); //LED rojo
}
}
void vTask2 ( void *pvParameters )
{
for ( ;; ) { vPrintString( "Espera activa\r\n" ); }
}
void RIT_IRQHandler(void)
{
BaseType_t xHigherPriorityTaskWoken = pdFALSE;
[xSemaphoreGiveFromISR]( [xBinarySemaphore], &xHigherPriorityTaskWoken );
if( [xHigherPriorityTaskWoken] == pdTRUE ) {
[portYIELD_FROM_ISR]( [xHigherPriorityTaskWoken] );
}
Chip_RIT_ClearInt(LPC_RITIMER);
}