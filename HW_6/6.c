#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    char* dirName = argv[1];

    readDir(getenv("PWD"));

    printf("\n");

    readDir(dirName);

    return 0;
}

void readDir(char* dirName) {
    DIR *dir;
    dir = opendir(dirName);

    if (!dir) {
        printf("Error in reading current directory\n");
        return -1;
    }

    struct dirent *dirStruct;
    while ((dirStruct = readdir(dir)) != NULL) {
        printf("%s\n", dirStruct->d_name);
    }

    closedir(dir);
}