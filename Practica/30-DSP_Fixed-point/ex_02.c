
#include <math.h>
#include <stdio.h>
#include <stdint.h>

// Para usar la biblioteca <math.h>, compilar con "-lm", por ejemplo:
// gcc ex_02.c -o ex_02 -lm

#define FRACTION_BITS 8

int32_t fp2fx(float a, int32_t n) {
    // Escalamos y redondeamos a Qn.m
    return (int32_t)(a * (1 << n) + 0.5);
}

float fx2fp(int32_t X, int32_t n) {
    // Convertimos de Qn.m a punto flotante
    return (float)X / (1 << n);
}

void main(void)
{	
	float_t a, b;
	int32_t X;
	
	a = 2.4515;
	X = fp2fx(a, FRACTION_BITS);
	b = fx2fp(X, FRACTION_BITS);
		 
	printf("a = %f \n", a );
	printf("X = %d \n", X );
	printf("b = %f \n", b );
	printf("diff = %f \n", a - b );
}
