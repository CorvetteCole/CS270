#include <stdio.h>
#include "csapp.h"


void *thread(void *vargp) {
    printf("Hello, world\n");
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Please include one argument for number of threads \n");
        return 0;
    }
    int n = atoi(argv[1]);
    pthread_t tid[n];
    for (int i = 0; i < n; i++) {
        pthread_create(&tid[i], NULL, thread, NULL);
        pthread_join(tid[i], NULL);
    }
}
