#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>

int read_symbol() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~ ( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newt );
    ch = getc(stdin);
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
    return ch;
}

int main( int argc, char* argv[]) {

    FILE* input_file;
    // открываем файл
    input_file = fopen(argv[1],"r");
    int N = atoi(argv[2]);

    if (input_file == NULL) {
        printf("Error in file opening");
        return -1;
    }

    // считанный символ
    int letter;
    int i = 1;

    // Чтение до тех пор, пока не достигнут конец файла
    while ((letter = getc(input_file)) != EOF) {

        putc(letter, stdout);

        if (letter == '\n') {
            i++;
        }

        // if N == 0
        if (!N) {
            i--;
        }

        if (i > N) {
            while (read_symbol() == EOF) {
            }
            i = 1;
        }
    }

    // закрываем файл
    fclose(input_file);

    return 0;
}