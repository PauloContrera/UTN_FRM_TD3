1- Vincular vTask1 y vTask2 a través de un mecanismo de sincronización.
2- vTask2 debe cambiar el estado del LED verde cuando vTask1 finaliza un ciclo de espera activa.
3- vTask2 espera indefinidamente la señal de sincronización.
SemaphoreHandle_t xMutex;
SemaphoreHandle_t xBinarySemaphore;
int main( void )
{
[xBinarySemaphore] = [xSemaphoreCreateBinary()];
/*Creación de tareas, inicio del planificador*/
for( ;; );
return 0;
}
void vTask1 ( void *pvParameters )
{
for ( ;; ) {
for ( i=0; i<DELAY_LOOP_COUNT; i++) { vPrintString( "Espera activa\r\n" ); }
[xSemaphoreGive] ([xBinarySemaphore]);
}
}
void vTask2 ( void *pvParameters )
{
const TickType_t xDelay = [pdMS_TO_TICKS]( 1000 );
{
for ( ;; ) {
if( [xSemaphoreTake]( [xBinarySemaphore, portMAX_DELAY] ) == pdTRUE) {
Board_LED_Toggle(5); //LED verde;
}else {
Board_LED_Toggle(4); //LED rojo;
}
}
}
Recuperatorio 2: Revisión del intent