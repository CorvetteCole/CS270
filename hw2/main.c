#include <stdio.h>
#include <stdlib.h>

long arith(long x, long y, long z){
    long t1 = x + y;
    long t2 = z + t1;
    long t3 = x + 4;
    long t4 = y * 48;
    long t5 = t3 + t4;
    long rval = t2 * t5;
    return rval;
}
void explode_bomb()
{
    printf("explode bomb.\n");
    exit(1);
}

//%rdi - first
//%rsi - second
//%rdx - third
//%rcx - fourth

unsigned func4(int val, unsigned esi)
{
    unsigned ret = 0;

    int ecx = val;
    int eax = ecx;

    // val >= 14
    if (val > 14)
        return 0;

    ecx -= esi;
    ecx >>= 1;
    ecx += esi;
    if (val < ecx){
        ret = func4(val, ecx - 1);
    } else if (val > ecx){
        ret = func4(val, ecx + 1);
    } else {
        return ;
    }
}

void phase_4(const char *input)
{
    unsigned num1, num2;

    if (sscanf(input, "%u %u", &num1, &num2) != 2)
        explode_bomb();


}

int main()
{
    printf("%d\n", func4(14, 0));
    //phase_4("14 3");
    //phase_4("60 3");
    //phase_4("80 4");
    //printf("success.\n");
    return 0;
}
//int main() {
//    printf("value: %ld\n", arith(3,4,8));
//    printf("value: %ld\n", (4294967296L*2147483648L) + 1);
//    return 0;
//}
