#include <stdio.h>

int main() {
    printf("%d", 65 ^ 17);
    return 0;
}

// Part A
int anyBitOfXEquals1(int x){
    // check if x is not 0 (decimal) so therefore contains a bit that is 1
    return !!x;
}

// Part B
int anyBitOfXEquals0(int x){
    // check if the inverse of x is not 0 (decimal) so therefore contains a bit that is 0
    return !!~x;
}

// Part C
int anyBitInLSBEquals1(int x){
    return !!(x & 0xFF);
}

// Part D
int anyBitInMSBEquals0(int x){
    return !!(~x & 0xFF000000);
}