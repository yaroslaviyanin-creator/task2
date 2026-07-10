/*
lib_main.c - главный модуль библиотеки. 
Янин Ярослав Иванович
Группа МК-101
*/


#include "lib_main.h"
#include <stdio.h>
#include <malloc.h>

// vstrlen - длина виртуальной строки
int vstrlen(const char* s) { 
    return strlen(s);
}


// vstr_symbol - значение символа по индексу vindex в строке s 
// vindex - виртуальный индекс символа в строке s
char vstr_symbol(const char* s, int vindex) {
    return s[vindex];
}

// Заполняем массив mas_index значением по умолчанию = -1
void init_mas_index(TIndex* mas_index, size_t searchlen) {
    for (int i = 0; i <= searchlen; i++) {
        mas_index[i].abs_index = -1;
        mas_index[i].search_index = -1;
    }
}

// Ищем в массиве mas_index первый пустой элемент (содержащий в полях значение -1)
// Возвращаем его индекс
size_t empty_index_mas_index(TIndex* mas_index, size_t searchlen) {
    int j = 0;
    while ((mas_index[j].abs_index != -1) && (j <= searchlen)) j++;
    return j;
}

// Уплотняем массив (сдвигаем все непустые элементы вперёд на место пустых)
void compact_array(TIndex* mas_index, size_t searchlen) {
    int cur = empty_index_mas_index(mas_index, searchlen);   // Находим первый пустой элемент
    // И начинаем смещать следующие непустые элементы вперед массива на место пустых элементов
    for (int i = cur+1; i <= searchlen; i++) {
        if (mas_index[i].abs_index != -1) {
            // Перемещаем элемент вперед на пустую позицию
            mas_index[cur].abs_index = mas_index[i].abs_index;
            mas_index[cur].search_index = mas_index[i].search_index;
            // Очищаем элемент, из которого перемещали
            mas_index[i].abs_index = -1;
            mas_index[i].search_index = -1;
            cur++;                  // cur - позиция в массиве, куда будет записан следующий не пустой элемент
        }
    }
 }


int process_file(const char* in_path, const char* out_path, const char* search, const char* replace) {
    FILE* in_file = fopen(in_path, "rb");
    if (in_file == NULL) {
        fprintf(stderr, "Error: Cannot open input file %s\n", in_path);
        return 1;
    }

    FILE* out_file = fopen(out_path, "wb");
    if (out_file == NULL) {
        fprintf(stderr, "Error: Cannot create output file %s\n", out_path);
        fclose(in_file);
        return 1;
    }

    printf("Files opened successfully.\n");
    
    char buf[N];                // buf - считываемый блок (массив) символов из файла 
    size_t b_read;              // b_read - количество считанных символов в блоке
    size_t i;                   // i - переменная цикла
    char b;                     // b - текущий символ

    //*****************************************************//
    // Работаем с блоками размера N байт из входного файла //
    //*****************************************************//
    
    // Считываем блоки, пока не дойдем до конца
    while ((b_read = fread(buf, 1, N, in_file)) > 0) {
        for (i = 0; i < b_read; i++) {
            b = buf[i];                        // b - текущий (рассматриваемый) символ из считанного блока 

            // Записываем символ в выходной файл
            fwrite(&b, 1, 1, out_file);

            //...
        }
    }

    fclose(in_file);
    fclose(out_file);
    return 0;
}