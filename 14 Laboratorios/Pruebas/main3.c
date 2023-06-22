1- La tarea vTask1 debe enviar a través de una cola de mensajes FIFO el valor entero 123 de manera continua.
2- En el caso de que la cola esté llena, vTask1 se debe bloquear por tiempo indefinido.
3- La tarea vTask2 debe recibir los datos enviados por vTask1
4- En el caso de que la cola esté vacía, vTask2 se debe bloquear por tiempo indefinido.
QueueHandle_t xQueue;
int main( void )
{
UBaseType_t uxQueueLength = 10;
UBaseType_t uxItemSize = sizeof( int32_t );
xQueue = [xQueueCreate]( [uxQueueLength, uxItemSize] );
/*Creación de tareas, inicio planificador*/
for( ;; );
return 0;
}
void vTask1 ( void *pvParameters )
{
int32_t lValueToSend = 123;
const TickType_t xDelay = [pdMS_TO_TICKS]( 1000 );
for ( ;; ) {
[xQueueSendToBack]( xQueue, [&lValueToSend, portMAX_DELAY] );
}
}
void vTask2 ( void *pvParameters )
{
int32_t lReceivedValue;
const TickType_t xDelay = [pdMS_TO_TICKS]( 1000 );
for ( ;; ) {
[xQueueReceive]( xQueue, [&lReceivedValue, portMAX_DELAY] );
vPrintString( "Dato recibido\r\n" );
}
}