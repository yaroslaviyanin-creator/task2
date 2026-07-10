/*
main.c - главный модуль программы. 
Янин Ярослав Иванович
Группа МК-101
*/

#include <stdio.h>
#include <stdlib.h>
#include "lib_main.h"

int main(int argc, char* argv[]) {
    //input_file, output_file, search_seq, replace_seq
    if (argc != 5) {
        fprintf(stderr, "Error: Invalid arguments.\nArguments: <input_file> <output_file> <search_seq> <replace_seq>\n");
        return 1;
    }

    char* input_file = argv[1];
    char* output_file = argv[2];
    char* search_seq = argv[3];
    char* replace_seq = argv[4];


    int result = process_file(input_file, output_file, search_seq, replace_seq);
    if (result != 0) {
        return result;
    }

    return 0;
}