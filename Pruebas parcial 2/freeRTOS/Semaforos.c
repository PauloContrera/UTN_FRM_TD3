#include "FreeRTOS.h"
#include "task.h"
#include "board.h"
#include "supporting_functions.h"

#define mainDELAY_LOOP_COUNT            ( 0xffffff )


//* Los semaforos sirven para el uso compartido de variables globales, y procesos que se pisan.
//* Lo definimos, creamos, decrementamos(lo bloqueamos), y lo aumentamos (Desbloqueamos)
//*Existen las variables xSemaphoreTakeFromISR y xSemaphoreGiveFromISR, ni puta idea, no se usan
//*Estimo que son para hacer consultas, si pueden bloquean o habilitan , sino tiran un error como los try en semaforos o mutex


SemaphoreHandle_t xMutex;//!declaramos que xMutex va a ser una fifo

SemaphoreHandle_t xBinarySemaphore;//!no se usa

int main( void )
{
//*xMutex a sido definida como una varible global del semaforo y con xSemaphoreCreateMutex()creamos el semaforo

xMutex = xSemaphoreCreateMutex(); 

for( ;; );
return 0;
}

void vTask1 ( void *pvParameters )
{
const TickType_t xDelay = pdMS_TO_TICKS( 1000 );//!No se usa
for ( ;; ) {

   //Preguntamos si podemos usar el semaforo, decrementa el semaforo y lo bloquea, hasta que lo vuelve a incrementar
   //Lo hace por tiempo indefinido por eso el portMax_DELAY
if( xSemaphoreTake( xMutex, portMAX_DELAY ) == pdTRUE) {
//*xSemaphoreTake(Variable Global del semaforo, tiempo)

vPrintString( "Acceso crítico vTask1\r\n" );

xSemaphoreGive(xMutex);
//*Aumentamos el valor del semaforo, lo marcamos como libre
} else Board_LED_Toggle(5); //LED verde
}
}


void vTask2 ( void *pvParameters )
{
const TickType_t xDelay = pdMS_TO_TICKS( 1000 );

for ( ;; ) {

 //Preguntamos si podemos usar el semaforo, decrementa el semaforo y lo bloquea, hasta que lo vuelve a incrementar
//Lo hace por tiempo finito por eso el xDelay (Variable)
if( xSemaphoreTake( xMutex, xDelay ) == pdTRUE) {
//*xSemaphoreTake(Variable Global del semaforo, tiempo)

vPrintString( "Acceso crítico vTask2\r\n" );
xSemaphoreGive(xMutex);
//*Aumentamos el valor del semaforo, lo marcamos como libre

} else Board_LED_Toggle(4); //LED rojo
}
}

/*
1- Ambas tareas compiten por el uso del puerto serie, un recurso crítico.
2- vTask1 espera indefinidamente la liberación del recurso.
3- vTask2 espera 1 segundo, si no se libera el recurso cambia de estado el LED rojo
*/