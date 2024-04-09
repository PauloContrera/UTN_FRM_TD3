#include <math.h>
#include <stdio.h>

typedef char  int8_t;
typedef short int16_t;

void main(void)
{	
	int16_t a, b, c, d, s1, s2;

	a = 127;
	b = 127;

	c = a + b;
	d = a * b;

	s1 = (-8) >> 2;
	s2 = (-1) >> 5;

	printf("c = %d \n", c );
	printf("d = %d \n", d );
	printf("s1 = %d \n", s1 );
	printf("s2 = %d \n", s2 );

}

/*
1- El valor de las variables c y d NO son correctos.
Al realizar la suma, el resultado se desborda del maximo
numero que puedo representar con 8 bits (OVERFLOW).
Lo mismo sucede con la multiplicación, se produce un
UNDERFLOW.

2- La solución fue cambiar el tipo de variable: int8_t -> int16_t. 
Así, tengo un máximo de representación más grande: (-32768 a 32767).

3- Son correctos los valores. Se realiza un desplazamiento aritmético.

*/