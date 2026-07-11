/*
lib_main.h - заголовочный файл модуля lib_main.c. 
Янин Ярослав Иванович
Группа МК-101
*/

#pragma once
#define N 4     // N - размер блока для считывания из файла

// vstrlen - длина виртуальной строки
int vstrlen(const char* s);

// vstr_symbol - значение виртуального символа по виртуальному индексу vindex в строке s 
char vstr_symbol(const char* s, int vindex);

// Структура элемента массива mas_index
typedef struct {
    size_t abs_index;       // абсолютный индекс символа во входном файле
    size_t search_index;    // индекс в искомой строке search докуда совпала подстрока
} TIndex;

// Заполняем массив структур mas_index значением по умолчанию = -1
void init_mas_index(TIndex* mas_index, size_t searchlen);

// Ищем в массиве mas_index индекс первого пустого элемента (содержащего в полях значения -1)
size_t empty_index_mas_index(TIndex* mas_index, size_t searchlen);

// Уплотняем массив (сдвигаем все непустые элементы в начало массива на место пустых)
void compact_array(TIndex* mas_index, size_t searchlen);

// Основная логика
int process_file(const char* in_path, const char* out_path, const char* search, const char* replace);
