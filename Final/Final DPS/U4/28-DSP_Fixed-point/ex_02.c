#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/*double FX2FP(int x,int n){ // Conversión Punto Fijo (FX) a Punto Flotante (FP)

    double X;
 
    X = (double)x*(2^(-n));

    return X;

}

int FP2FX(double X,int n){ // Conversión Punto Flotante (FP) a Punto Fijo (FX)

    int x;

    return round(X * (1<<n)); 
}*/

int main(){

    int a,n,rx;
    double b,rp;

    n = 8;
    a = 5;
    b = 12.9;

/*  rx = FP2FX(b,n);
    rp = FX2FP(a,n);

    printf("Conversion Punto Fijo - Punto Flotante -> rx = %d\n",rx);
    printf("Conversion Punto Flotante - Punto Fijo -> rp = %f\n",rp);
*/
    //Conversion Punto Fijo - Punto Flotante.

    rp = (double)(a);
    printf("Conversión: %f\n",rp);

    //Conversion Punto Flotante - Punto Fijo.

    rx = round(2.4515 * (pow(2,10)));
    printf("Conversión: %d\n",rx);

    return 0;
}
