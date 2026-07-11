/*
lib_main.c - главный модуль библиотеки. 
Янин Ярослав Иванович
Группа МК-101
*/


#include "lib_main.h"
#include <malloc.h>

// vstrlen - длина виртуальной строки
int vstrlen(const char* s) { 
    int i = 0;              // переменная цикла (счетчик виртуальных символов)
    // Пока находятся виртуальные символы в строке, выполняем цикл
    while ((i < strlen(s)) && (vstr_symbol(s, i) != -1)) i++;
    return i;
}

// Проверка символа на принадлежность к алфавиту 16 с.с.
int ishexcode(const char c) {
    char ch = tolower(c);               // Преобразует заглавные латинские буквы в строчные
    int  a = ch >= '0' && ch <= '9';    // a = 1, если символ ch цифра в диапазоне 0..9
    int  b = ch >= 'a' && ch <= 'f';    // b = 1, если символ ch символ 'a', 'b', 'c', 'd', 'e' или 'f'
    if (a || b) return 1;
    else return 0;
}

// Превращение символа из 16 с.с. в число 10 с.с.
int hextodec(const char c) {
    char ch = tolower(c);               // Преобразует заглавные латинские буквы в строчные
    int chislo;
    if (ch >= '0' && ch <= '9') chislo = ch - '0';       // Превращение символа цифры в число
    if (ch >= 'a' && ch <= 'f') chislo = ch - 'a' + 10;  // Превращение символа буквы в число
    return chislo;
}

// Функция вывода текстовых сообщений ошибок
void print_error(const int r) {
    switch (r) {
    case -2:     // Ошибка: -2. Не хватает символа после '\'.
        fprintf(stderr, ". Error: Missing character after \'\\\'.\n");
        break;
    case -3:     // Ошибка: -3. Неверный символ после '\'. Ожидается 'x', 'X' или '\'.
        fprintf(stderr, ". Error: Invalid character after \'\\\'. Expected \'x\', \'X\', or \'\\\'.\n");
        break;
    case -4:     // Ошибка: -4. Не хватает цифр для 16-тиричного кода символа. Нужно 2 цифры.
        fprintf(stderr, ". Error: There are not enough digits for the 16-digit character code. You need 2 digits.\n");
        break;
    case -5:     // Ошибка: -5. Неверные цифры 16-тиричного кода символа.
        fprintf(stderr, ". Error: Incorrect digits in the 16-bit character code.\n");
        break;
    default:
        // Неизвестная ошибка. Если ни одно из значений case не совпало.
        fprintf(stderr, "Error: Unknown error.\n");
        break;
    }
    exit(1);  // Завершение программы
}

// vstr_symbol - значение виртуального символа по виртуальному индексу vindex в строке s 
char vstr_symbol(const char* s, int vindex) {
    int i = 0;                      // переменная цикла
    int cur_index = 0;              // текущий индекс символа
    char cur_char;                  // текущий символ

    // Пока строка не закончилась или не найден нужный по счету виртуальный символ
    while ((i < strlen(s)) && (cur_index <= vindex)) {
        
        if (s[i] == '\\') {                 // Если текущий символ '\'
            if (i + 1 < strlen(s)) {        // и есть ещё один символ после него.
                if (s[i + 1] == '\\') {     // Если следующий символ после '\' тоже '\',
                    cur_char = '\\';        // то это код обратного слеша.
                    i += 2;                 // Сдвигаем переменную цикла на 2 позиции
                }
                else if (tolower(s[i + 1]) == 'x') {    // Если следующий символ после '\' является 'x' или 'X', то это начало 16-тиричного кода символа.            
                    if (i + 3 < strlen(s)) {            // и есть ещё два символа после.
                        if (ishexcode(s[i + 2]) && ishexcode(s[i + 3])) {               // Если эти символы являются цифрами в 16 с.с.,
                            cur_char = hextodec(s[i + 2]) * 16 + hextodec(s[i + 3]);    // то вычисляем значение 16-тиричного кода
                            i += 4;                                                     // и сдвигаем переменную цикла на 4 позиции.
                        }
                        else print_error(-5); // Ошибка: -5. Неверные цифры 16-тиричного кода символа.
                    }
                    else print_error(-4);     // Ошибка: -4. Не хватает цифр для 16-тиричного кода символа. Нужно 2 цифры. 
                }
                else print_error(-3);         // Ошибка: -3. Неверный символ после '\'. Ожидается 'x', 'X' или '\'. 
            }
            else print_error(-2);             // Ошибка: -2. Не хватает символа после '\'.
        }
        
        else {      // Иначе текущий символ остается без изменений
            cur_char = s[i];
            i++;
        }
        if (cur_index == vindex) return cur_char;   // Если нашли нужный по счету виртуальный символ
        cur_index++;
    }
    return -1;
}

// Заполняем массив структур mas_index значением по умолчанию = -1
void init_mas_index(TIndex* mas_index, size_t searchlen) {
    for (int i = 0; i <= searchlen; i++) {
        mas_index[i].abs_index = -1;            // абсолютный индекс символа во входном файле
        mas_index[i].search_index = -1;         // индекс в искомой строке search докуда совпала подстрока
    }
}

// Ищем в массиве mas_index индекс первого пустого элемента (содержащего в полях значения -1)
size_t empty_index_mas_index(TIndex* mas_index, size_t searchlen) {
    int j = 0;
    while ((mas_index[j].abs_index != -1) && (j <= searchlen)) j++;
    return j;
}

// Уплотняем массив (сдвигаем все непустые элементы в начало массива на место пустых)
void compact_array(TIndex* mas_index, size_t searchlen) {
    int cur = empty_index_mas_index(mas_index, searchlen);   // Находим первый пустой элемент
    // И начинаем смещать следующие непустые элементы в начало массива на место пустых элементов
    for (int i = cur+1; i <= searchlen; i++) {
        if (mas_index[i].abs_index != -1) {     // Если текущий элемент пустой
            // Перемещаем элемент в начало массива на пустую позицию
            mas_index[cur].abs_index = mas_index[i].abs_index;
            mas_index[cur].search_index = mas_index[i].search_index;
            // Очищаем содержимое элемента, из которого перемещали
            mas_index[i].abs_index = -1;
            mas_index[i].search_index = -1;
            cur++;                  // cur - позиция в массиве, куда будет записан следующий не пустой элемент
        }
    }
 }

// Функция для отладки. Вывод массива mas_index на экран
void print_array(TIndex* mas_index, size_t searchlen) {
    for (int y = 0; y <= searchlen; y++)
        printf("i = %d  [ %zd ] [ %zd ]\n", y, mas_index[y].abs_index, mas_index[y].search_index);
}

// Посимвольный вывод строки для замены в выходной файл
int replace_to_file(FILE* out_file, const char* replace, const size_t replacelen) {
    char tmp;                               // tmp - переменная для временного символа для вывода в файл
    for (size_t z = 0; z < replacelen; z++) {
        tmp = vstr_symbol(replace, z);      // взятие из строки для замены вирутального символа по позиции z
        fwrite(&tmp, 1, 1, out_file);       // запись этого символа в файл
    }
    return 1;   // Возращаемое значение 1 будет использоваться для установки флага подмены в 1 (flag_podmena_str = 1)
}



int process_file(const char* in_path, const char* out_path, const char* search, const char* replace) {
    
    // Проверяем корректность введенных строк в виде аргументов, получая их виртуальные длины
    printf("The search string");
    size_t searchlen  = vstrlen(search);               // searchlen - виртуальная длина искомой строки
    if (searchlen >= 0) printf(" is correct.\n");
    
    printf("The replace string");
    size_t replacelen = vstrlen(replace);              // replacelen - виртуальная длина строки для замены
    if (replacelen >= 0) printf(" is correct.\n");
    
    
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
    
    // Считываем блоки, пока не дойдем до конца входного файла
    while ((b_read = fread(buf, 1, N, in_file)) > 0) {
        b_count++;                             // считали очередной блок, учли его в количестве. Отсчет идёт от 0.
        for (i = 0; i < b_read; i++) {
            b = buf[i];                        // b - текущий (рассматриваемый) символ из считанного блока 
            abs_cur_index = b_count * N + i;   // абсолютное значение индекса данного символа относительно начала входного файла
            flag_podmena_str = 0;              // Предположим, что подмены строки ещё не было с текущим символом b

            j = 0;                             // Переменная цикла while

            // В цикле проходим по непустым элементам массива mas_index (по найденным ранее подстрокам)
            while ((mas_index[j].abs_index != -1) && (j <= searchlen)) {
                // Если символ b совпал с очередным символов строки search в уже найденной подстроке,
                if (b == vstr_symbol(search, mas_index[j].search_index + 1)) {
                    mas_index[j].search_index += 1;        // то в поле количества совпавших символов подстроки добавляем 1
                    
                    if (mas_index[j].search_index == searchlen-1) {  // и проверяем, а не закончилась ли сверяемая строка.
                        // Выводим подмену строки в выходной файл и устнавливаем флаг, что произошла подмена строк
                        flag_podmena_str = replace_to_file(out_file, replace, replacelen); 
                        init_mas_index(mas_index, searchlen);           // очищаем массив индексов
                    }
                    j++;    // перемещаемся к следующему элементу массива mas_index 
                }
                else {      // Текущий символ b не совпал с ожидаемым символов из исследуемой строки.
                    if (j == 0) {   // Если это была первая подстрока, то записываем в выходной файл первые символы от исследуемой строки,
                        // которые в начале совпадали c исследуемой строкой, до начала второй подстроки, если она есть.
                        count_symbol = mas_index[0].search_index;           // count_symbol - количество символов для записи из первой подстроки
                        if (mas_index[1].abs_index != -1) count_symbol = count_symbol - mas_index[1].search_index - 1;  // есть вторая подстрока
                        replace_to_file(out_file, search, count_symbol+1);    // Запись в выходной файл
                    }
                    // Очистка информации о данной (переставшей совпадать) подстроки в массиве mas_index.
                    mas_index[j].abs_index = -1;
                    mas_index[j].search_index = -1;
                    
                    compact_array(mas_index, searchlen);        // Уплотнение массива (избавляемся от пустых мест с начала массива).
                   
                    // Инкрементировать значение j нет необходимости, чтобы перейти к следующему элементу массива mas_index, 
                    // мы уже автоматически находимся на следующем элементе посде очистки элемента массива и его уплотнения.
                }
            }
                     
            // Текущий символ совпал с началом исследуемой строки 
            if (b == vstr_symbol(search, 0)) {
                if (searchlen == 1) {   // Если длина исследуемой строки = 1
                    // Выводим строку для замены в выходной файл и устнавливаем флаг, что произошла подмена строк
                    flag_podmena_str = replace_to_file(out_file, replace, replacelen); 
                }
                else if (!flag_podmena_str) {    // Длина исследуемой строки > 1 и не было подмены строки ранее с этим символом
                    cur_mas_index = empty_index_mas_index(mas_index, searchlen);    // Ищем первый пустой элемент в массиве mas_index
                    mas_index[cur_mas_index].abs_index = abs_cur_index;             // Записываем в него информацию о новой подстроки
                    mas_index[cur_mas_index].search_index = 0;
                }
            }
            
            compact_array(mas_index, searchlen);   // Уплотняем массив

            // Если массив индексов пустой (нет найденных частей подстрок) и не было подмены строки, то выводим текущий символ в выходной файл
            if ((empty_index_mas_index(mas_index, searchlen) == 0) && (!flag_podmena_str)) {
                fwrite(&b, 1, 1, out_file);
            }
        }
    }

    // Если при завершении входного файла массив индексов mas_index не пустой, то переписываем в выходной файл начало исследуемой строки.
    // Переписываем столько символов, сколько было найденно при совпаденииях в первом элементе массива mas_index[0].
    if (empty_index_mas_index(mas_index, searchlen) != 0) {
        replace_to_file(out_file, search, mas_index[0].search_index + 1);    // Запись в выходной файл
    }

    free(mas_index);        // Освобождаем память из под одномерного массива структур 
    fclose(in_file);
    fclose(out_file);
    return 0;
}