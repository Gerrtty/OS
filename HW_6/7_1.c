#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

char* concat(char *str1, char *str2) {

    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);

    // выделяем память на результирующую строку
    char *result = (char*) malloc(len1 + len2);

    // копируем первую строку в result (в начало)
    memcpy(result, str1, len1);
    // копируем вторую строку в result (в место где заканчивается вторая строка)
    memcpy(result + len1, str2, len2);

    return result;
}

struct MYFILE {
    char *file_path;
    char *file_name;
    char *absolut_path;
    struct stat file_info;
};

void println(char* str) {
    printf(str);
    printf("\n");
}

struct MYFILE get_file(struct MYFILE myfile, char *path, char *file_name) {

    myfile.file_path = path;
    myfile.file_name = file_name;

    char* absolut_path_to_file = concat(concat(myfile.file_path, "/"), myfile.file_name);
    myfile.absolut_path = absolut_path_to_file;
    int res = stat(absolut_path_to_file, &myfile.file_info);

    if (res != 0) {
        println("Error in get file");
    }

    return myfile;
}

int is_dir(char* fileName) {
    for (int i = 0; i < strlen(fileName); i++) {
        if (fileName[i] == '.') {
            return 0;
        }
    }
    return 1;
}

void sort_files(struct MYFILE list_files[], int len, int sort) {

    /*
     * Сортировка файлов по ключу
     * (по размеру - 1, по имени - 2)
    */

    int bool = 0;

    struct MYFILE tmp;

    for (int i = 0; i < len - 1; i++) {
        for (int j = i + 1; j < len; j++) {

            switch (sort) {
                case 1: {
                    bool = list_files[i].file_info.st_size > list_files[j].file_info.st_size;
                    break;
                }
                case 2: {
                    bool = list_files[i].file_name > list_files[j].file_name;
                    break;
                }
            }

            if (bool) {
                tmp = list_files[i];
                list_files[i] = list_files[j];
                list_files[j] = tmp;
            }
        }
    }

}

void write_files(struct MYFILE list_files[], int len, char *new_path) {

    FILE *file_in, *file_out;
    char *file_name_in, *file_name_out;
    char buf[1024];
    for (int i = 0; i < len; i++) {
        file_name_in = concat(concat(list_files[i].file_path, "/"), list_files[i].file_name);

        if (is_dir(file_name_in)) {
            continue;
        }

        file_name_out = concat(concat(new_path, "/"), list_files[i].file_name);
        file_in = fopen(file_name_in, "r");
        file_out = fopen(file_name_out, "w");
        while(fgets(buf, 1024, file_in) != NULL) {
            fprintf(file_out, "%s", buf);
        }
        fclose(file_in);
        fclose(file_out);
        fprintf(stdout,
                "%s%s - %li byte\n",
                new_path,
                list_files[i].file_name,
                list_files[i].file_info.st_size);
    }
}

int k = 0;

void get_files(struct MYFILE list_files[], char *path) {

    DIR* dir;
    struct dirent *one_file;
    dir = opendir(path);
    int result;
    char *name_file;

    while ((one_file = readdir(dir)) != NULL) {
        name_file = one_file->d_name;
        if (name_file[0] == '.') {
            continue;
        }
        struct MYFILE myfile;
        myfile.file_name = name_file;
        myfile.file_path = path;
        list_files[k++] = get_file(myfile, path, name_file);
        if (is_dir(list_files[k - 1].absolut_path)) {

            println("-----");
            println(list_files[k - 1].file_name);
            println(list_files[k - 1].file_path);
            println("------");

            mkdir("/home/yuliya/Desktop/out/2", list_files[k-1].file_info.st_mode);

            get_files(list_files, list_files[k - 1].absolut_path);
        }
    }

}

int main() {

    struct MYFILE list_files[10];

    get_files(list_files, "/home/yuliya/Desktop/test");
    // sort_files(list_files, k, 1);
    // write_files(list_files, k, "/home/yuliya/Desktop/out/");

    for (int i = 0; i < k; i++) {
        println(list_files[i].file_name);
    }

    return 0;
}