1- Ambas tareas compiten por el uso del puerto serie, un recurso crítico.
2- vTask1 espera indefinidamente la liberación del recurso.
3- vTask2 espera 1 segundo, si no se libera el recurso cambia de estado el LED rojo.
SemaphoreHandle_t xMutex;
SemaphoreHandle_t xBinarySemaphore;
int main( void )
{
[xMutex] = [xSemaphoreCreateMutex()];
/*Creación de tareas, habilitación de interrupciones, inicio planificador*/
for( ;; );
return 0;
}
void vTask1 ( void *pvParameters )
{
const TickType_t xDelay = [pdMS_TO_TICKS]( 1000 );
for ( ;; ) {
if( [xSemaphoreTake]( [xMutex, portMAX_DELAY] ) == pdTRUE) {
vPrintString( "Acceso crítico vTask1\r\n" );
[xSemaphoreGive]([xMutex]);
} else Board_LED_Toggle(5); //LED verde
}
}
void vTask2 ( void *pvParameters )
{
const TickType_t xDelay = [pdMS_TO_TICKS]( 1000 );
for ( ;; ) {
if( [xSemaphoreTake]( [xMutex, xDelay] ) == pdTRUE) {
vPrintString( "Acceso crítico vTask2\r\n" );
[xSemaphoreGive]([xMutex]);
} else Board_LED_Toggle(4); //LED rojo
}
}