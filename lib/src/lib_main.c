/*
lib_main.c - главный модуль библиотеки. 
Янин Ярослав Иванович
Группа МК-101
*/


#include "lib_main.h"
#include <stdio.h>

int process_file(const char* in_path, const char* out_path, const char* search, const char* replace) {
    FILE* in_file = fopen(in_path, "rb");
    if (in_file == NULL) {
        fprintf(stderr, "Error: Cannot open %s\n", in_path);
        return 1;
    }

    FILE* out_file = fopen(out_path, "wb");
    if (out_file == NULL) {
        fprintf(stderr, "Error: Cannot create %s\n", out_path);
        fclose(in_file); 
        return 2;
    }

    printf("Files opened\n");

    //...

    fclose(in_file);
    fclose(out_file);

    return 0;
}