#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char *argv[], char *envp[]) {
    char* fileName1 = argv[1];
    char* fileName2 = argv[2];

    FILE *f1;
    FILE *f2;

    // file opening
    f1 = fopen(fileName1, "r");
    f2 = fopen(fileName2,"w");

    // checking file opening errors
    if (f1 == NULL || f2 == NULL) {
        printf("Error in file opening\n");
        return -1;
    }

    // read and write
    int letter;
    while ((letter = getc(f1)) != EOF) {
        putc(letter, f2);
    }

    // closing files
    fclose(f2);
    fclose(f1);

    // get first file property values
    struct stat buf;
    stat(fileName1, &buf);

    if (chmod(fileName2, buf.st_mode) == 0) {
        printf("Access rights recorded\n");
    }
    else {
        printf("Failed to write access rights\n");
    }

    return 0;
}