#include "unity.h"
#include "lib_main.h"

void setUp(void) {}
void tearDown(void) {}

// Проверка валидных hex-символов
void test_ishexcode_valid(void) {
    TEST_ASSERT_EQUAL(1, ishexcode('a'));
    TEST_ASSERT_EQUAL(1, ishexcode('F'));
    TEST_ASSERT_EQUAL(1, ishexcode('5'));
}

// Проверка невалидных hex-символов
void test_ishexcode_invalid(void) {
    TEST_ASSERT_EQUAL(0, ishexcode('g'));
    TEST_ASSERT_EQUAL(0, ishexcode('Z'));
    TEST_ASSERT_EQUAL(0, ishexcode('-'));
}

// Проверка конвертации hex в десятичное число
void test_hextodec_conversion(void) {
    TEST_ASSERT_EQUAL(10, hextodec('a'));
    TEST_ASSERT_EQUAL(15, hextodec('F'));
    TEST_ASSERT_EQUAL(5, hextodec('5'));
}

// Виртуальная длина обычной строки
void test_vstrlen_normal_string(void) {
    TEST_ASSERT_EQUAL(5, vstrlen("hello"));
}

// Виртуальная длина строки с hex-кодом
void test_vstrlen_with_hex(void) {
    TEST_ASSERT_EQUAL(3, vstrlen("a\\x0Ab"));
}

// Извлечение обычного символа
void test_vstr_symbol_normal(void) {
    TEST_ASSERT_EQUAL('h', vstr_symbol("hello", 0));
    TEST_ASSERT_EQUAL('o', vstr_symbol("hello", 4));
}

// Извлечение hex-символа
void test_vstr_symbol_hex(void) {
    TEST_ASSERT_EQUAL(10, vstr_symbol("\\x0A", 0)); // \n = 10
    TEST_ASSERT_EQUAL('\\', vstr_symbol("\\\\", 0)); 
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_ishexcode_valid);
    RUN_TEST(test_ishexcode_invalid);
    RUN_TEST(test_hextodec_conversion);
    RUN_TEST(test_vstrlen_normal_string);
    RUN_TEST(test_vstrlen_with_hex);
    RUN_TEST(test_vstr_symbol_normal);
    RUN_TEST(test_vstr_symbol_hex);
    return UNITY_END();
}