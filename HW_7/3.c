#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

void create(int n) {

    for(int i = 0; i < n; i++) {
        if(fork() == 0) {
            printf("Дочерний поток с id %d id родительского потока = %d\n",getpid(),getppid());
            exit(0);
        }
    }
}

int main(int argc, char *argv[]) {

    char *p1;
    int n1;

    long conv1 = strtol(argv[1], &p1, 10);
    n1 = conv1;

    char *p2;
    int n2;

    long conv2 = strtol(argv[2], &p2, 10);
    n2 = conv2;

    for(int i = 0; i < n1; i++) {
        if(fork() == 0) {
            printf("Дочерний поток с id %d id родительского потока = %d\n",getpid(),getppid());
            create(n2);
            exit(0);
        }
    }
}