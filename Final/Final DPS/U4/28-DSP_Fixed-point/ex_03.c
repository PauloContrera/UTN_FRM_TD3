#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


int32_t truncation(int64_t X){
	int32_t z;
	
	z = (int32_t) (X >> 10);
	return z;
}

int32_t roundoff(int64_t X){
    int32_t z;

    z = (X + (1 << 9));

    return truncation(z);
}

void main(){

    float a,b;
    int32_t result1,result2,trunc,roff;
    int64_t total;

    printf("Dos numeros a multiplicar...\n");

    printf("a = 2.45902\n");
    printf("b = 4.5678\n");

    //Conversion.
    printf("Conversion...\n");

    result1 = round(2.45902 * (1 << 10)); ;
    result2 = round(4.5678 * (1 << 10)); ;
    
    printf("Convert_a = %d\nConvert_b = %d\n",result1,result2);

    printf("Multiplicacion...\n");

    total = result1 * result2;

    printf("Resultado sin redondeo = %ld\n",total);
    
    trunc = truncation(total);
    
    printf("Resultado truncado = %d\n",trunc);

    roff = roundoff(total);

    printf("Resultado redondeado = %d\n",roff);
}
