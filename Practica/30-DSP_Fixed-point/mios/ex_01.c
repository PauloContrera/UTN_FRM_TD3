#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <limits.h>

// Para usar la biblioteca <math.h>, compilar con "-lm", por ejemplo:
// gcc ex_01.c -o ex_01 -lm

int main(void)
{	
	int8_t a, b, s1, s2;
	int16_t c, d;
	 
	a = 127;
	b = 127;
	
	c = a + b;	
	d = a * b;
	
	s1 = (-8) >> 2;
	s2 = (-1) >> 5;

	printf("INT_MAX = %d \n" , INT_MAX);
	printf("INT_MIN = %d \n" , INT_MIN);
	
	printf("c = %d \n", c );
	printf("d = %d \n", d );
	printf("s1 = %d \n", s1 );
	printf("s2 = %d \n", s2 );

	return 0;
}

//1-Los valores de las variables 'c' y 'd' no son correctos. Esto se debe a que son variables de 8 bits, por lo que su rango de representacion es de -128 a 127. Estas variables no son capaces de representar numeros que esten fuera de este rango
//2-Para solucionar esto, debemos guardar el resultado de las operaciones en variables que tengan una mayor cantidad de bits. En el caso de la suma se debe guardar el resultado en una variable de n+1 bits(minimo), mientras que el resultado de una multiplicacion se debe guardar en una variable de 2*n bits.
//3-PREGUNTAR NO ENTENDI QUE SE HACE EN ESAS VARIABLES
