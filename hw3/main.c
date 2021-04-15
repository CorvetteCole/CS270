#include <stdio.h>
#include <stdlib.h>

void good_echo(){

    const int BUFFER_LENGTH = 10;

    char buffer[BUFFER_LENGTH]; // buffer for holding characters before line is expanded
    char *line = 0;  // pointer towards characters
    size_t currentLength = 0; // current size of line

    while (fgets(buffer, sizeof(buffer), stdin) != NULL){
        line = realloc(line, currentLength + BUFFER_LENGTH - 1); // allocate more space to line
        for (int i = 0; i < BUFFER_LENGTH - 1 && buffer[i] != '\0'; i++){
            *(line + currentLength + i) = buffer[i];
        }
        currentLength += BUFFER_LENGTH - 1;
    }

    puts(line); // prints line
    free(line); // remove line var from memory
}


long pcount_r(unsigned long x){
    if (x==0){
        return 0;
    } else {
        return (x & 1) + pcount_r(x >> 1);
    }
}

long switch_prob(long x, long n) {
    long result = x;
    switch (n) {
        case 60:
        case 62:
            result = x * 8;
            break;
        case 63:
            result = x >> 3;
            break;
        case 64:
            result <<= 4;
            result -= x;
            x = result;
        case 65:
            x *= x;
        default:
            result = x + 75;
            break;
    }
    return result;
}


int main() {
    //printf("%ld\n", pcount_r(0x00000012));
//    for (int i = 0; i < 1200; i++){
//        printf("%c", 'j');
//    }
    //printf("\n");
    good_echo();
    return 0;
}
