#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>


char* concat(char *str1, char *str2);


struct file {
    /*
     * Структура файл.
     * @ file_name - Название файла;
     * @ file_info - Информация о файле;
    */
    char *file_path;
    char *file_name;
    struct stat file_info;
};

int get_file(struct file this, char *path, char *file_name) {
    /*
     * Получение информации о файлах,
     * при возникновении ошибки возвращает -1,
     * В случе успеха возвращает 0.
    */
    this.file_path = path;
    this.file_name = file_name;
    int result = stat(concat(this.file_path, this.file_name), &this.file_info);
    if (result != 0) {
        fprintf(stderr,
                "Ошибка открытия файла. Пропускается файл - %s%s\n",
                this.file_path,
                this.file_name);
        errno = -1;
    }
    else {
        errno = 0;
    }
    return errno;
}

int get_files(struct file *list_files[], char *path);
void sort_files(struct file *list_files[], int len, int sort);
int write_files(struct file *list_files[], int len, char *new_path);


int main(int argc, char *argv[]) {
    //*********************
    char *from;
    int key = 0;
    char *to;
    if (chdir(argv[1]) == 0) {
        from = argv[1];
    }
    else {
        fprintf(stderr, "Ошибка при открытии каталога '%s'\n", argv[1]);
        return 1;
    }
    if (atoi(argv[2]) > 0) {
        key = atoi(argv[2]);
    }
    else {
        fprintf(stderr, "Не веный ключ для сортировки\n");
        return 1;
    }
    if (chdir(argv[3]) == 0) {
        to = argv[3];
    }
    else {
        fprintf(stderr, "Ошибка при открытии каталога '%s'\n", argv[3]);
        return 1;
    }
    if (argc < 4) {
        fprintf(stderr, "Не хватает аргументов\n");
        return 1;
    }
    int result = 0;
    struct file **files;
    files = (struct file**)malloc(sizeof(struct file*));
    result = get_files(files, from);

    if (result != -1) {
        sort_files(files, result, key);
    }
    else {
        fprintf(stderr, "Ошибка при открытии каталога %s", from);
        return 1;
    }
    result = write_files(&files, result, to);
    if (result == -1) {
        fprintf(stderr, "Ошибка записи файлов в каталог %s", to);
        return 1;
    }
    return 0;
}

int get_files(struct file *list_files[], char *path) {
    /*
     * Чтение данных из директории
     * и запись данных в массив для хранения.
    */
    DIR *dir;
    struct dirent *one_file;
    dir = opendir(path);
    int len = 0;
    int result;
    int i = 0;
    char *name_file;
    while ((one_file = readdir(dir)) != NULL) {
        name_file = one_file->d_name;
        if (name_file[0] == '.') {
            continue;
        }

        result = get_file(*list_files[i], path, name_file);
        if (result != -1) {
            i++;
        }
    }
    len = i;
    if (len > 0) {
        errno = 0;
        return len;
    }
    else {
        errno = -1;
        return errno;
    }
}

void sort_files(struct file *list_files[], int len, int sort) {
    /*
     * Сортировка файлов по ключу
     * (по размеру - 1, по имени - 2)
    */
    struct file *tmp;
    switch (sort) {
        case 1:{
            for (int i = 0; i < len - 1; i++){
                for (int j = i + 1; j < len; j++) {
                    if (list_files[i]->file_info.st_size > list_files[j]->file_info.st_size) {
                        tmp = list_files[i];
                        list_files[i] = list_files[j];
                        list_files[j] = tmp;
                    }
                }
            }
            break;
        }
        case 2:{
            for (int i = 0; i < len - 1; i++){
                for (int j = i + 1; j < len; j++) {
                    if (list_files[i]->file_name > list_files[j]->file_name) {
                        tmp = list_files[i];
                        list_files[i] = list_files[j];
                        list_files[j] = tmp;
                    }
                }
            }
            break;
        }
    }
    errno = 0;
}

int write_files(struct file *list_files[], int len, char *new_path) {
    /*
     * Копирование файлов и вывод результата.
    */
    FILE *file_in, *file_out;
    char *file_name_in, *file_name_out;
    char buf[1024];
    for (int i = 0; i < len; i++) {
        file_name_in = concat(list_files[i]->file_path, list_files[i]->file_name);
        file_name_out = concat(new_path, list_files[i]->file_name);
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
                list_files[i]->file_name,
                list_files[i]->file_info.st_size);
    }
    errno = 0;
    return errno;
}

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