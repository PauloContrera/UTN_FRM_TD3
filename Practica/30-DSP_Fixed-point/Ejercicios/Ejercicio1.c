/*
Ejercicio 2
Cree 2 funciones en C:
1. Una función para pasar de punto fijo a punto flotante, fx2fp( ).
2. Una función para pasar de punto flotante a punto fijo, fp2fx( ).
3. Verifique el correcto funcionamiento haciendo,
b = fx2fp( fp2fx( 2.4515) ) 
4. Compare b con 2.4515 para Q23.8 y Q21.10

    gcc -o Compilado/DSP2 Ejercicio2.c
    ./DSP2
*/

#include <stdio.h>

//! 1.Función para convertir de punto fijo (Q23.8) a punto flotante
float fx2fp(int q23_8) {
    return (float)q23_8 / 256.0; // Dividir por 256 para convertir de Q23.8 a punto flotante
}

//! 2.Función para convertir de punto flotante a punto fijo (Q23.8)
int fp2fx(float q21_10) {
    return (int)(q21_10 * 256); // Multiplicar por 256 para convertir de punto flotante a Q23.8
}

int main() {
    float input_fp = 2.4515;
    int q23_8 = fp2fx(input_fp);
    float result_fp = fx2fp(q23_8);

    printf("Original valor de punto flotante: %f\n", input_fp);
    printf("Valor de punto fijo (Q23.8): %d\n", q23_8);
    printf("Valor de punto flotante resultante: %f\n", result_fp);

    return 0;
}
