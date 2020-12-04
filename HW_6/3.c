#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

int main(int argc[], char *argv[], char *envp[]) {
    char* fileName = argv[1];

    // open file
    FILE *outputFile;
    outputFile = fopen(fileName, "w");

    if (outputFile == NULL) {
        printf("Error in opening file");
    }

    int c;

    while (1) {

        // read symbol
        struct termios oldt, newt;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag&= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        c = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

        // 5 is code for ctrl + E
        if (c == 5) {
            // close file
            fclose(outputFile);
            break;
        }

        // put symbol in file
        fputc(c, outputFile);

    }

    return 0;
}