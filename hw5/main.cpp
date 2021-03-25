#include <iostream>
#include <unistd.h>

using namespace std;

pid_t Fork(void) {
    pid_t retval;
    if ((retval = fork()) < 0)
        exit(1);
    return retval;
}


int main() {
    if (Fork()==0) {
        printf("hello\n");
    }
    Fork();
    printf("hello\n");
    return 0;
}
//void test(){
//    if (fork()) {
//        fork(); fork();
//        printf("Example\n");
//        return;
//    }
//    return;
//}
//
//int main() {
//    test();
//    printf("Example t\n");
//    exit(0);
//}

