#include "csapp.h"

/*
* mmapcopy - uses mmap to copy file fd1 to fd2
*/
void mmapcopy(int fd1, int size, int fd2) {
    char *bufp; /* ptr to memory-mapped VM area */
    bufp = Mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd1, 0);
    write(fd2, bufp, size);
    return;

}

void problem914(){
    struct stat stat;
    int fd1 = Open("hello.txt", O_RDWR, 0);
    fstat(fd1, &stat);
    char *file1 = Mmap(NULL, stat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd1, 0);
    *(file1) = 'j';
}

/* mmapcopy driver */
int main(int argc, char **argv) {

    problem914();
//    struct stat stat;
//    int fd1, fd2;/* Check for required command-line argument */
//    if (argc != 3) {
//        printf("usage: %s <filename> <filename>\n", argv[0]);
//        exit(0);
//    }
//
///* Copy the input argument to stdout */
//    fd1 = Open(argv[1], O_RDONLY, 0);
//    fd2 = Open(argv[2], O_WRONLY | O_CREAT, 0644);
//    fstat(fd1, &stat);
//    mmapcopy(fd1, stat.st_size, fd2);
//    exit(0);
}


