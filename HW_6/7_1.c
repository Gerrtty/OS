#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

/***
 * Отсортировать в заданном каталоге (аргумент 1 командной строки)
 * и во всех его подкаталогах файлы по следующим критериям
 * (аргумент 2 командной строки, задаётся в виде целого числа):
 * 1 – по размеру файла, 2 – по имени файла.
 * Записать файлы в порядке сортировки в новый каталог
 * (аргумент 3 командной строки).
 * После записи каждого файла выводить на консоль
 * полный путь каталога, имя файла, размер файла.
 ***/

char* output_dir = "";
char* input_path = "";
int point = 1;
int len_input_path = 1;

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
    printf("%s\n", str);
}

struct MYFILE get_file(struct MYFILE myfile) {

    char* absolut_path_to_file = concat(concat(myfile.file_path, "/"), myfile.file_name);
    myfile.absolut_path = absolut_path_to_file;
    int res = stat(absolut_path_to_file, &myfile.file_info);

    if (res != 0) {
        println("Error in get file");
    }

    return myfile;
}

int is_file(struct MYFILE myfile) {
    return S_ISREG(myfile.file_info.st_mode);
}

void sort_files(struct MYFILE list_files[], int len, int sort) {

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

void write_file(struct MYFILE myfile, char *new_path) {

    FILE *file_in, *file_out;
    char *file_name_in, *file_name_out;
    char buf[1024];
    file_name_in = concat(concat(myfile.file_path, "/"), myfile.file_name);
    file_name_out = new_path;

    file_in = fopen(file_name_in, "r");
    file_out = fopen(file_name_out, "w");
    while(fgets(buf, 1024, file_in) != NULL) {
        fprintf(file_out, "%s", buf);
    }
    fclose(file_in);
    fclose(file_out);
    fprintf(stdout,
            "%s - %s - %li byte\n",
            file_name_out, myfile.file_name,
            myfile.file_info.st_size);
}

int get_files_count_in_dir(char* path_to_dir) {

    int count_files_in_dir = 0;

    DIR* dir;
    dir = opendir(path_to_dir);
    struct dirent *one_file;

    while ((one_file = readdir(dir)) != NULL) {
        count_files_in_dir++;
    }

    closedir(dir);

    // - .
    // - ..
    return count_files_in_dir - 2;
}

int is_system(char* name_file) {
    if (strlen(name_file) == 1 && name_file[0] == '.') {
        return 1;
    }

    if (strlen(name_file) == 2 && name_file[0] == '.' && name_file[1] == '.') {
        return 1;
    }

    return 0;
}

char* get_new_file_path(char* input_dir_path,
                        char* out_dir_path,
                        char* abs_file_path) {
    int len_of_out_directory_path = strlen(out_dir_path);
    int len_of_input_directory_path = strlen(input_dir_path);

    int local_file_path_len = strlen(abs_file_path) - len_of_input_directory_path;

    char *out_file_path = (char*) malloc(len_of_out_directory_path + local_file_path_len);

    for (int i = 0; i < len_of_out_directory_path; i++) {
        out_file_path[i] = out_dir_path[i];
    }

    int h = 0;
    for (int i = 0; i < local_file_path_len; i++) {
        out_file_path[len_of_out_directory_path + i] = abs_file_path[len_of_input_directory_path + h++];
    }

    return out_file_path;
}

void get_files(char *path, int key) {

    const int size = get_files_count_in_dir(path);

    struct MYFILE list_files[size];

    DIR* dir;
    struct dirent *one_file;
    dir = opendir(path);
    char *name_file;

    int i = 0;

    // считываем все файлы в директории
    // и складываем их в массив
    while ((one_file = readdir(dir)) != NULL) {
        name_file = one_file->d_name;

        // if name_file == "." or name_file == ".."
        if (is_system(name_file)) {
            continue;
        }

        struct MYFILE myfile;
        myfile.file_name = name_file;
        myfile.file_path = path;
        list_files[i++] = get_file(myfile);
    }

    // сортируем
    sort_files(list_files, i, key);

    for (int j = 0; j < i; j++) {

        char* new_file_path = get_new_file_path(input_path, output_dir, list_files[j].absolut_path);

        if (!is_file(list_files[j])) {
            mkdir(new_file_path, list_files[j].file_info.st_mode);
            get_files(list_files[j].absolut_path, key);
        }

        else {
            write_file(list_files[j], new_file_path);
        }
    }

}

int main(int argc, char *argv[]) {

    char* input_dir = argv[1];
    int key = atoi(argv[2]);
    char* out_dir = argv[3];

    output_dir = out_dir;
    point = strlen(output_dir);
    input_path = input_dir;
    len_input_path = strlen(input_path);

    get_files(input_path, key);

    return 0;
}