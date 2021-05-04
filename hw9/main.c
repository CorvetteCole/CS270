#include "csapp.h"
#include <dirent.h>
#include <sys/types.h>

//void test(){
//    if (Fork() == 0) {
//        int fd = open("foo.txt", O_RDONLY, 0);
//        dup2(fd, STDIN_FILENO);
//        close(fd);
//        Execve("fstatcheck", argv, envp);
//    }
//}


int main(int argc, char **argv)
{
    DIR *streamp;
    struct dirent *dep;

    streamp = Opendir(argv[1]);

    errno = 0;
    while ((dep = Readdir(streamp)) != NULL) {
        printf("Found file: %s at inode: %lu\n", dep->d_name, dep->d_ino);
    }
    if (errno != 0)
        unix_error("readdir error");

    Closedir(streamp);
    exit(0);
}

//int main (int argc, char **argv)
//{
//    struct stat stat;
//    char *type, *readok;
//    int fd;
//    fd = atoi(argv[1]);
//    Fstat(fd, &stat);
//    if (S_ISREG(stat.st_mode))
///* Determine file type */
//        type = "regular";
//    else if (S_ISDIR(stat.st_mode))
//        type = "directory";
//    else
//        type = "other";
//    if ((stat.st_mode & S_IRUSR)) /* Check read access */
//        readok = "yes";
//    else
//        readok = "no";
//
//    printf("type: %s, read: %s\n", type, readok);
//    exit(0);
//
//}