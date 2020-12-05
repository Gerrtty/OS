#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

/// 1 вариант

char* concat(char *str1, char *str2) {

    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    char *result = (char*)malloc(len1 + len2 + 1);

    if (!result) {
        fprintf(stderr, "Ошибка выделения памяти!\n");
        _Exit(-1);
    }

    memcpy(result, str1, len1);
    memcpy(result + len1, str2, len2+1);

    return result;
}

struct file {
    int size;
    char* name;
};

int main(int argc, char *argv[]) {
    char* dirName = argv[1];
    int mod = atoi(argv[2]);
    char* newDirName = argv[3];

    mode_t mode = 0777;

    if (mod == 1) {
        DIR *dir;
        dir = opendir(dirName);

        if (!dir) {
            printf("Error in reading current directory\n");
            return -1;
        }

        struct dirent *dirStruct;

        struct file files[1000];

        while ((dirStruct = readdir(dir)) != NULL) {

            if (dirStruct->d_name[0] != '.') {

                struct stat buf;
                stat(dirStruct->d_name, &buf);
                printf("%s\n", dirStruct->d_name);
                char c[1] = {'/'};
                // get absolute path
                char* name = concat(concat(dirName, c), dirStruct->d_name);
                printf("%s\n", name);

                DIR *d;
                d = opendir(name);

                if (d != NULL) {
                    char* newDirNames = concat(concat(newDirName, c), dirStruct->d_name);
                    printf("%s", dirStruct->d_name);
                    printf("file is directory and new dir name is %s\n", newDirNames);
                    mkdir(newDirNames, mode);
                }

                if (d == NULL) {
                    printf("file is not directory\n");
                    struct stat st;
                    stat(name, &st);
                    int size = st.st_size;
                    printf("This size = %i\n", size);

                    struct file f;
                    f.name = name;
                    f.size = size;

                }

            }
        }

        closedir(dir);
    }

    if (mod == 2) {

    }

    return 0;
}