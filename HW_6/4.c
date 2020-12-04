#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

int main(int argc[], char *argv[], char *envp[]) {
    char* fileName = argv[1];
    int N = atoi(argv[2]);

    FILE *f;
    f = fopen(fileName, "r");

    if (f == NULL) {
        printf("Error in opening file\n");
    }

    int letter;
    int k = 1;

    short flag;
    while (flag) {
        for (int i = 0; i < N; i++) {
            letter = getc(f);

            if (letter == EOF) {
                flag = 0;
                break;
            }

            putc(letter, stdout);

        }

        if (flag) {
            int c = readSymbol();
        }
        else {
            putc('\n', stdout);
        }

    }

    fclose(f);

    return 0;
}

int readSymbol() {
    int c;

    // read symbol
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag&= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    c = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return c;
}