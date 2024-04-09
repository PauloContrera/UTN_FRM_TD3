#include <math.h>
#include <stdio.h>
#include <limits.h>

__int32_t saturation(__int32_t a,__int32_t b){
    // RANGO: -2147483648 a 2147483647

    __int32_t c;

    c = a + b;

    if(c >= INT_MIN && a > INT_MAX && b > INT_MAX){
        c = INT_MAX;
    }
    
    if(c <= INT_MAX && a < INT_MIN && b < INT_MIN){
        c = INT_MIN;
    }

    if((c <= INT_MAX) && (c >= INT_MIN)){
        return c;
    }

}

void main(){

    __int32_t a,b,c;

    // 1er caso.
    a = 2000000000;
    b = 1000000000;
    c = saturation(a,b);
    printf("1er caso: c = %d\n",c);

    //2do caso.
    a = -2147483648;
    b = -2147483648;
    c = saturation(a,b);
    printf("2do caso: c = %d\n");

    //3er caso.
    a = 2000000000;
    b = -1000000000;
    c = saturation(a,b);
    printf("3er caso: c = %d\n");

}