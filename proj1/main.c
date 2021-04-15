#include <stdio.h>
#include <stdlib.h>

void explode_bomb() {
    printf("bomb exploded\n");
    exit(1);
}

//int phase_5(int num1, int num2) {
void phase_5(const char *input) {
    int num1;
    int num2;
    if (sscanf(input, "%d %d", &num1, &num2) <= 1) {
        explode_bomb();
    }

    int rsi[] = {10, 2, 14, 7, 8, 12, 15, 11, 0, 4, 1, 13, 3, 9, 6, 5};

    int eax = num1;
    int edxIndex = 0;
    int ecx = 0;
    for (edxIndex = 0; eax != 15; edxIndex++){
        int rax = eax;
        printf("run: %d array value: rsi[%d]:%d\n", edxIndex, rax, rsi[rax]);
        eax = rsi[rax];
        ecx += eax;
    }
    printf("loop done\n");
    printf("eax: %d\nedx (index): %d\necx: %d\n", eax, edxIndex, ecx);

    if (edxIndex != 15 || ecx != num2){
        explode_bomb();
    }

//    return (edxIndex == 15) && (ecx == num2);
}

void phase_6(const char *input){
    int rsi[6];
    if (sscanf(input, "%d %d %d %d %d %d", &rsi[0], &rsi[1], &rsi[2], &rsi[3], &rsi[4], &rsi[5]) < 6) {
        explode_bomb();
    }
}


int main() {
    //phase_5("5 115");
    phase_6("1 2 3 4 5 6");
//    int success = 0;
//    for (int testNum = 0; testNum < 15 && !success; testNum++){
//        for (int testNum2 = 0; testNum2 < 115 && !success; testNum2++){
//            if (phase_5(testNum,testNum2)){
//                success = 1;
//                printf("solution found num1: %d, num2: %d\n", testNum, testNum2);
//            }
//        }
//
//    }

    return 0;
}
