/*
Compile el siguiente código en C en su PC:
    gcc -o Compilado/DSP5 Ejercicio5.c
    ./DSP5

^Ejercicio 5
    *1. Escriba un programa en C que implemente la operación MAC en punto fijo para los siguientes vectores: 
        float X[5] = {1.1, 2.2, 3.3, 4.4, 5.5 } ;
        float Y[5] = {6.6, 7.7, 8.8, 9.9, 10.10};

    *2. Convierta ambos vectores a formato Q21.10.

    *3. Implemente la operación MAC con dos enfoques:

        1. Multiplique 2 números de 32 bits y redondee por truncación antes de sumar cada resultados.
        2. Multiplique 2 números de 64 bits y solo redondee luego de sumar todos los números.

            for(i=0; i < 5; i++)  { 
                acum_32a += (int32_t) ( truncation( (int64_t) ( A[i] * B[i] ) ) );
                acum_64  += (int64_t) ( (int64_t) ( A[i] * B[i] ) );  
            }

            acum_32b += (int32_t) acum_64;
    
    *4. Compare ambos resultados en punto fijo con el que se obtiene al operar en formato double.
        double X[5] = {1.1, 2.2, 3.3, 4.4, 5.5 };
        double Y[5] = {6.6, 7.7, 8.8, 9.9, 10.10};

        for(i=0; i < 5; i++){ 
            acum_db += A[i] * B[i];
        }
    
    *5. Finalmente, compare los valores de  acum_32a, acum_32b y acum_db.  
*/

#include <math.h>
#include <stdio.h>
#include <stdint.h>




//^ Convierta ambos vectores a formato Q21.10. (2)
int32_t Q21_10(float num) {
    //! FX → INT para Qm.n
    // k = 1 << (n-1);
    // y = (x + k) >> n;

    int k = 1 << 9; // *(n-1), N es 10
    int32_t y = (int32_t)((num * k) ); // Escalado por 1024 y convertido a entero
    return y;
}
//^ Truncar la multiplicacion (3.1)
int truncation(int64_t x) {
    //! Truncation para Qm.n
    //int floor(int x) {
    //return x & ˜((1<<n)-1);
    //}
    int n=8;
    // return x& ~((1 << n) - 1);
    return (int32_t) (x>>n);
}



int main() {
    //~1.Implemente la operación MAC en punto fijo para los siguientes vectores
    float X[5] = {1.1, 2.2, 3.3, 4.4, 5.5};
    float Y[5] = {6.6, 7.7, 8.8, 9.9, 10.10};

    //~2.Convierta ambos vectores a formato Q21.10.
    int32_t A[5], B[5]; 
    for (int i = 0; i < 5; i++) {
        A[i] = Q21_10(X[i]);
        B[i] = Q21_10(Y[i]);
    }
    
        //! X → A
        //! Y → B
    
    //~3.Implemente la operación MAC con dos enfoques:

    int32_t acum_32a = 0;
    int32_t acum_32b = 0;
    double acum_64 = 0;
   
    
    for(int i=0; i < 5; i++)  { 
        //*1. Multiplique 2 números de 32 bits y redondee por truncación antes de sumar cada resultados.
                acum_32a += (int32_t) ( truncation( (int64_t) ( A[i] * B[i] ) ) );
                //acum_32a += (int32_t) ( truncation( (int64_t) ( X[i] * Y[i] ) ) );

        //*2. Multiplique 2 números de 64 bits y solo redondee luego de sumar todos los números.
                acum_64  += (int64_t) ( (int64_t) ( A[i] * B[i] ) );  
                //acum_64  += (int64_t) ( (int64_t) ( X[i] * Y[i] ) );  
                //acum_64  += (double) ( (double) ( A[i] * B[i] ) );  

            }

    // Acumulación de 32 bits sobre los resultados de 64 bits
    acum_32b += (int32_t) acum_64;


    //~4. Compare ambos resultados en punto fijo con el que se obtiene al operar en formato double.
    double X_d[5] = {1.1, 2.2, 3.3, 4.4, 5.5 };
    double Y_d[5] = {6.6, 7.7, 8.8, 9.9, 10.10};
    double acum_db = 0;

        for(int i=0; i < 5; i++){ 
            acum_db += X_d[i] * Y_d[i];
        }

   
    // Comparación de resultados
    printf("Resultados en punto fijo (acum_32a): %d\n", acum_32a);
    printf("Resultados en punto fijo (acum_32b): %d\n", acum_32b);
    printf("Resultados en punto flotante (acum_db): %f\n", acum_db);

    return 0;
}

/*
*Resultados en punto fijo (acum_32a): 38986
*Resultados en punto fijo (acum_32b): 38988
*Resultados en punto flotante (acum_db): 152.350000

*acum_32a y acum_32b 

Utilizan diferentes enfoques para calcular el valor MAC en punto fijo.

acum_32a utiliza la función truncation para redondear cada producto antes de acumularlo. 
Puede provocar pérdida de precisión debido a los redondeos sucesivos.

acum_32b acumula todos los productos de 64 bits primero y luego redondea el resultado final. 
Más precisión, ya que los redondeos se realizan una sola vez sobre la suma total.

!redondear antes de la suma (acum_32a) vs redondear después de la suma (acum_32b).


*Resultados en punto flotante (acum_db): 
Realiza todas las operaciones en formato de punto flotante sin redondeos 
proporciona el resultado más preciso en punto flotante.


^Conclusiones
Las diferencias entre estos resultados se deben a las diferencias en la representación y el redondeo en punto fijo y punto flotante. 
En punto fijo, los redondeos pueden introducir pequeños errores acumulativos debido a redondeos intermedios, mientras que en punto flotante, 
la representación en coma flotante permite una mayor precisión en los cálculos. 
Estas diferencias son comunes al comparar cálculos en diferentes formatos numéricos.

*/