#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <proc_service.h>

int main() {

    pid_t pid1, pid2;

    // создаём первый дочерний процесс
    pid1 = fork();

    // для времени
    time_t current_time;

    struct tm* time_info;
    char timeString[9];

    struct timeval curTime;
    gettimeofday(&curTime, NULL);
    int milli = curTime.tv_usec / 1000;

    time(&current_time);
    time_info = localtime(&current_time);

    strftime(timeString, sizeof(timeString), "%H:%M:%S", time_info);

    // если процесс дочерний, то выводим инфу
    if (pid1 == 0) {
        printf ( "Дочерний процесс 1 с id = %d, id родительского процесса = %d\n",
                 getpid(), getppid());
        printf("время: %s:%d\n", timeString, milli);

    }

    else {
        // создаём второй дочерний процесс из родительского процесса
        pid2 = fork();

        // если процесс дочерний, то выводим инфу
        if (pid2 == 0) {
            printf ( "Дочерний процесс 2 с id = %d, id родительского процесса = %d\n",
                     getpid(), getppid());
            printf("время: %s:%d\n", timeString, milli);
        }
        else {
            printf ( "Родительсий процесс с id = %d\n" , getpid());
            printf("время: %s:%d\n", timeString, milli);
            puts("");
            system("ps -x");
        }
    }

    return 0;
}