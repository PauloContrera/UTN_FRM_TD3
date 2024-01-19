/*
Compile el siguiente código en C en su PC:
    gcc -o Compilado/DSP5_Explicacion Ejercicio5_Explicacion.c
    ./DSP5_Explicacion

^Explicacion:
"MAC" se refiere a "Multiply and ACcumulate" (Multiplicar y Acumular). 
Representa la operación de convolución y es la operación aritmética básica en 
el Procesamiento Digital de Señales (DSP)

*La operación MAC combina la multiplicación y la adición en una sola operación. 
Esto es útil en DSP para realizar cálculos más eficientes, como en el procesamiento de señales, 
donde estas operaciones son comunes. 

Sin embargo, es importante tener en cuenta que las operaciones MAC pueden generar 
!desbordamientos (overflows) o subdesbordamientos (underflows) en los valores resultantes.

*/
#include <stdio.h>
#include <stdint.h>

#define K 10  

int main() {
    int32_t a[K] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int32_t b[K] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    int64_t c = 0;
    for (int i = 0; i < K; i++)
    {
        c = c + ((int64_t)a[i] * (int64_t)b[i]);
    }

    printf("El resultado de la convolución es: %ld\n", c);

    return 0;
}
