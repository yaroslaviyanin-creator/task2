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
    size_t b_count = -1;        // b_count - текущее количество считанных блоков из файла уменьшенное на 1, чтобы отсчет начинался с 0
    size_t abs_cur_index;       // abs_cur_index - абсолютное смещение текущего символа относительно начала входного файла
    size_t cur_mas_index;       // cur_mas_index - индекс текущего элемента массива mass_index
    size_t i, j, z;             // i, j, z - переменные цикла
    char b;                     // b - текущий символ
    char tmp;                   // tmp - переменная для временного символа для вывода в файл
    size_t count_symbol;        // count_symbol - количество символов для записи в выходной файл от начала исследуемой строки search
    int flag_podmena_str;       // 0 - не было замены; 1 - была замена


    int searchlen = vstrlen(search);                // searchlen - виртуальная длина искомой строки


    //***********************************************************************************************//
    // Выделяем память под одномерный массив mas_index, где количество элементов = searchlen + 1.    //
    // Элементами массива являются структуры из двух полей:                                          //
    //      - поле абсолютного индекса символа во входном файле (в процессе написания кода это поле  //
    //                                                           помогало отлаживать программу);     //
    //      - поле индекса в искомой строке search докуда совпала подстрока.                         //
    //                                                                                               //
    // И заполняем его значением по умолчанию = -1                                                   //
    //***********************************************************************************************//

    // Выделяем память под одномерный массив структур TIndex
    TIndex* mas_index = (TIndex*)calloc(searchlen + 1, sizeof(TIndex));

    if (mas_index == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for array.");
        return 1;
    }

    // Заполняем массив mas_index значением по умолчанию = -1
    init_mas_index(mas_index, searchlen);


    //*****************************************************//
    // Работаем с блоками размера N байт из входного файла //
    //*****************************************************//
    
    // Считываем блоки, пока не дойдем до конца
    while ((b_read = fread(buf, 1, N, in_file)) > 0) {
        b_count++;                             // считали очередной блок, учли его в количестве. Отсчет идёт от 0.
        for (i = 0; i < b_read; i++) {
            b = buf[i];                        // b - текущий (рассматриваемый) символ из считанного блока 
            abs_cur_index = b_count * N + i;   // абсолютное значение индекса данного символа относительно начала входного файла
            flag_podmena_str = 0;              // Предположим, что подмены строки ещё не было с текущим символом b

            j = 0;                             // Переменная цикла while

            // В цикле проходим по непустым элементам массива mas_index (по найденным подстрокам)
            while ((mas_index[j].abs_index != -1) && (j <= searchlen)) {
                // Если символ b совпал с очередным символов строки search в уже найденной подстроке
                if (b == vstr_symbol(search, mas_index[j].search_index + 1)) {
                    mas_index[j].search_index += 1;        // то в поле количества совпавших символов подстроки добавляем 1

                    // Очередной символ строки совпал, проверяем, а не закончилась ли сверяемая строка
                    // Если закончилась, то в выходной файл выводим подмену строки и очищаем массив индексов
                    if (mas_index[j].search_index == searchlen - 1) {
                        // Выводим подмену строки в выходной файл
                        for (z = 0; z < vstrlen(replace); z++) {
                            tmp = vstr_symbol(replace, z);
                            fwrite(&tmp, 1, 1, out_file);
                        }
                        init_mas_index(mas_index, searchlen);    // очищаем массив индексов
                        flag_podmena_str = 1;                    // Устнавливаем флаг, что произошла подмена строк
                    }
                    j++;    // перемещаемся к следующему элементу массива mas_index 
                }
                else {      // Текущий символ b не совпал с ожидаемым символов из исследуемой строки
                    // Записываем в выходной файл первые символы от исследуемой строки, которые вначале совпадали 
                    if (j == 0) {   // Если это была первая подстрока, то записываем в выходной файл 
                        // первые символы от исследуемой строки, которые вначале совпадали c исследуемой строкой
                        // до начала второй подстроки (количество символов для записи = count_symbol) 
                        count_symbol = mas_index[0].search_index;
                        if (mas_index[j].abs_index != -1) count_symbol = count_symbol - mas_index[1].search_index - 1;
                        // Запись в выходной файл
                        for (z = 0; z <= count_symbol; z++) {
                            tmp = vstr_symbol(search, z);
                            fwrite(&tmp, 1, 1, out_file);
                        }
                    }
                    // Очистка информации о данной (переставшей совпадать) подстроки в массиве mas_index.
                    mas_index[j].abs_index = -1;
                    mas_index[j].search_index = -1;
                    compact_array(mas_index, searchlen);        // Уплотнение массива (избавляемся от пустых мест сначала массива).
                    // Посде очистки элемента массива и его уплотнения переменная j теперь указывает на следующий по значениям элемент в этом массиве.
                    // Инкрементировать значение j нет необходимости, чтобы перейти к следующему элементу массива mas_index, 
                    // мы уже автоматически находимся на следующем элементе.
                }
            }


            // Фиксируем первые ИКС вхождений первого символа исследуемой строки во входном файле в массиве mass_index
            // ИКС - это количество символов в исследуемой строке + 1, больше данного количества подстрок одновпременно нам не нужно
            
            // Текущий символ совпал с началом исследуемой строки
            if (b == vstr_symbol(search, 0)) {
                cur_mas_index = empty_index_mas_index(mas_index, searchlen);    // Ищем первый пустой элемент в массиве mas_index
                mas_index[cur_mas_index].abs_index = abs_cur_index;             // Записываем в него информацию о новой подстроки
                mas_index[cur_mas_index].search_index = 0;
            }
            
            compact_array(mas_index, searchlen);   // Уплотнаяем массив

            // Если массив индексов пустой (нет найденных частей подстрок) и не было подмены строки, то выводим текущий символ в выходной файл
            if ((empty_index_mas_index(mas_index, searchlen) == 0) && (!flag_podmena_str)) {
                fwrite(&b, 1, 1, out_file);
            }
        }
    }

    free(mas_index);        // Освобождаем память из под одномерного массива структур 
    fclose(in_file);
    fclose(out_file);
    return 0;
}