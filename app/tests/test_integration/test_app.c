#include "unity.h"
#include "lib_main.h"
#include <stdio.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

// Базовая замена слова
void test_process_file_basic_replace(void) {
    FILE* f = fopen("test_in_1.txt", "wb");
    fputs("hello world", f);
    fclose(f);

    int res = process_file("test_in_1.txt", "test_out_1.txt", "world", "C");
    TEST_ASSERT_EQUAL(0, res);

    f = fopen("test_out_1.txt", "rb");
    char buf[20] = { 0 };
    fread(buf, 1, 19, f);
    fclose(f);

    TEST_ASSERT_EQUAL_STRING("hello C", buf);

    remove("test_in_1.txt");
    remove("test_out_1.txt");
}

// Замена перекрывающихся строк
void test_process_file_overlapping(void) {
    FILE* f = fopen("test_in_over.txt", "wb");
    fputs("ababa", f);
    fclose(f);

    int res = process_file("test_in_over.txt", "test_out_over.txt", "aba", "X");
    TEST_ASSERT_EQUAL(0, res);

    f = fopen("test_out_over.txt", "rb");
    char buf[20] = { 0 };
    fread(buf, 1, 19, f);
    fclose(f);

    TEST_ASSERT_EQUAL_STRING("Xba", buf);

    remove("test_in_over.txt");
    remove("test_out_over.txt");
}

// Работа с виртуальными hex-символами
void test_process_file_hex_replace(void) {
    FILE* f = fopen("test_in_hex.txt", "wb");
    fputs("A\nB", f);
    fclose(f);

    int res = process_file("test_in_hex.txt", "test_out_hex.txt", "\\x0A", "-");
    TEST_ASSERT_EQUAL(0, res);

    f = fopen("test_out_hex.txt", "rb");
    char buf[20] = { 0 };
    fread(buf, 1, 19, f);
    fclose(f);

    TEST_ASSERT_EQUAL_STRING("A-B", buf);

    remove("test_in_hex.txt");
    remove("test_out_hex.txt");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_process_file_basic_replace);
    RUN_TEST(test_process_file_overlapping);
    RUN_TEST(test_process_file_hex_replace);
    return UNITY_END();
}