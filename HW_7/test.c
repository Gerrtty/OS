#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <proc_service.h>

void create(int n) {

    for(int i = 0; i < n; i++) {
        if(fork() == 0) {
            // printf("Дочерний поток с id %d id родительского потока = %d\n", getpid(), getppid());

            int t = (i + 1) * 200;

            while (1) {

                // для времени
                // -------
                time_t current_time;

                struct tm* time_info;
                char timeString[9];

                struct timeval curTime;
                gettimeofday(&curTime, NULL);
                int milli = curTime.tv_usec / 1000;

                time(&current_time);
                time_info = localtime(&current_time);

                strftime(timeString, sizeof(timeString), "%H:%M:%S", time_info);
                //------

                if (milli % t == 0) {
                    printf("Дочерний поток с id %d id родительского потока = %d, время = %s:%d\n",
                           getpid(), getppid(), timeString, milli);
                }
            }
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
            // printf("Дочерний поток с id %d id родительского потока = %d\n", getpid(), getppid());
            create(n2);

            int t = (i + 1) * 200;

            while (1) {

                // для времени
                // -------
                time_t current_time;

                struct tm* time_info;
                char timeString[9];

                struct timeval curTime;
                gettimeofday(&curTime, NULL);
                int milli = curTime.tv_usec / 1000;

                time(&current_time);
                time_info = localtime(&current_time);

                strftime(timeString, sizeof(timeString), "%H:%M:%S", time_info);
                //------

                if (milli % t == 0) {
                    printf("Дочерний поток с id %d id родительского потока = %d, время = %s:%d\n",
                           getpid(), getppid(), timeString, milli);
                }
            }

            exit(0);
        }
    }
}