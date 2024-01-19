
#include <math.h>
#include <stdio.h>
#include <stdint.h>

// Para usar la biblioteca <math.h>, compilar con "-lm", por ejemplo:
// gcc ex_02.c -o ex_02 -lm

#define FRACTION_BITS 8

int32_t fp2fx (float_t a, int32_t n) //Punto flotante a Punto Fijo
{

	int32_t b;
	b = (int32_t)(a * (1 << n));
	
	return b;

}

float_t fx2fp (int32_t X, int32_t n) //Punto fijo a Punto Flotante
{
 
	float_t b;
	b = ((float_t)(X) / (1 << n));

	return b;

}

void main(void)
{	
	float_t a, b;
	int32_t X;
	
	a = 2.4515;
	X = fp2fx(a, FRACTION_BITS);
	b = fx2fp(X, FRACTION_BITS);
		
	printf("a = %f \n", a );
	printf("X = %d   'Punto Flotante a Punto Fijo'\n", X );
	printf("b = %f   'Punto Fijo a Punto Flotante'\n", b );
	printf("diff = %f \n", a - b );
}
