/*
Compile el siguiente código en C en su PC:
    gcc -o Compilado/DSP3 Ejercicio3.c
    ./DSP3

Ejercicio 3
Multiplique dos números en Q21.10. El resultado debe ser ajustado según los dos esquemas de
redondeos estudiados.
1. Cree una función que implemente redondeo por truncación. 
int32_t truncation(int64_t X)
2. Cree una función que implemente redondeo al valor más cercano.
int32_t rounding(int64_t X)
1
3.  Compare el resultado de cada esquema de redondeo con el resultado que obtendría usando
números en formato double.    
*/




#include <stdio.h>

// Función para convertir de punto fijo (Q23.8) a punto flotante
float fx2fp(int q23_8) {
    return (float)q23_8 / 256.0; // Dividir por 256 para convertir de Q23.8 a punto flotante
}

// Función para convertir de punto flotante a punto fijo (Q23.8)
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
