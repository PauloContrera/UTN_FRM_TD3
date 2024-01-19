
#include <math.h>
#include <stdio.h>
#include <stdint.h>

// Para usar la biblioteca <math.h>, compilar con "-lm", por ejemplo:
// gcc ex_02.c -o ex_02 -lm

#define FRACTION_BITS 10

int32_t fp2fx (float_t a, int32_t n) //Punto Flotante a Punto Fijo
{
	int32_t b;
	b = (int32_t)(a * ( 1 << n ));
	
	return b;
}

float_t fx2fp (int32_t X, int32_t n) //Punto Fijo a Punto Flotante
{
	float_t b;
	b = ((float_t)(X) / ( 1 << n ));
	
	return b;
}



int32_t truncation (int64_t X, int32_t n)
{
	int32_t a;
	a = (int32_t)( X >> n );
	
	return a;
}


int32_t roundoff (int64_t X, int32_t n)
{
	int32_t a;
	
	X = ((int32_t) (X) + (1 << (n - 1) ) );		//Le sumo una precision
	a = (int32_t)( X >> n );		//Lo trunco
	
	return a;
}

int main(void)
{	
	float_t a, b, real;
	int32_t c, d, e, f;

	a = 2.451527;
	b = 3.566775;

	printf("a = %f \n", a );
	printf("b = %f \n", b );

	real = a * b ;
	printf("a * b = %f \n",real);

	c = fp2fx(a, FRACTION_BITS); //Paso 'a' Punto Fijo
	d = fp2fx(b, FRACTION_BITS);  //Paso 'b' Punto Fijo
	e = truncation ((int64_t)c * (int64_t)d, FRACTION_BITS); //Realizo "truncacion" de c * d
	f = roundoff  ((int64_t)c * (int64_t)d, FRACTION_BITS); //Realizo "redondeo" de c * d
	a = fx2fp(e, FRACTION_BITS); //Paso el resultado de la "truncacion" a Punto Flotante
	b = fx2fp(f, FRACTION_BITS); //Paso el resultado del "redondeo" a Punto Flotante

	printf("c = %d \n", c );
	printf("d = %d \n", d );
	printf("e = %d \n", e );
	printf("f = %d \n", f );
	printf("truncado = %f \n", a );
	printf("redondeado = %f \n", b );

	printf("diff con truncacion = %f \n", a - real);
	printf("diff con redondeo = %f \n", b - real);
	
	return 0;
}
