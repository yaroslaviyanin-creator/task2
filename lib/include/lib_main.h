/*
lib_main.h - заголовочный файл модуля lib_main.c. 
Янин Ярослав Иванович
Группа МК-101
*/

#pragma once
#define N 4

// vstrlen - длина виртуальной строки
int vstrlen(const char* s);

// vstr_symbol - значение символа по индексу vindex в строке s 
// vindex - виртуальный индекс символа в строке s
char vstr_symbol(const char* s, int vindex);

// Структура элемента массива mas_index
typedef struct {
    size_t abs_index;       // абсолютный индекс символа во входном файле
    size_t search_index;    // индекс в искомой строке search докуда совпала подстрока
} TIndex;

// Заполняем массив mas_index значением по умолчанию = -1
void init_mas_index(TIndex* mas_index, size_t searchlen)

// Ищем в массиве mas_index первый пустой элемент (содержащий в полях значение -1)
// Возвращаем его индекс
size_t empty_index_mas_index(TIndex* mas_index, size_t searchlen)

// Уплотняем массив (сдвигаем все непустые элементы вперёд на место пустых)
void compact_array(TIndex* mas_index, size_t searchlen)

// Основная логика
int process_file(const char* in_path, const char* out_path, const char* search, const char* replace);
