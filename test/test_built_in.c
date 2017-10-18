#include "built_in.h"
#include "unity.h"

void test_built_in_chr(void) {
    char *A = chr(65);
    TEST_ASSERT_EQUAL_STRING("A", A);
    free(A);

    char *B = chr(66);
    TEST_ASSERT_EQUAL_STRING("B", B);
    free(B);

    char *zero = chr(48);
    TEST_ASSERT_EQUAL_STRING("0", zero);
    free(zero);
}

void test_built_in_asc(void) {
    char str[] = "Testovaci";

    TEST_ASSERT_EQUAL_INT(84, asc(str, 0));
    TEST_ASSERT_EQUAL_INT(105, asc(st, 8));
    TEST_ASSERT_EQUAL_INT(65, asc("AB", 0));
    TEST_ASSERT_EQUAL_INT(66, asc("AB", 1));
    TEST_ASSERT_EQUAL_INT(0, asc(str, -1));
    TEST_ASSERT_EQUAL_INT(0, asc(str, -25));
    TEST_ASSERT_EQUAL_INT(0, asc(str, 9));
    TEST_ASSERT_EQUAL_INT(0, asc(str, 12));
    TEST_ASSERT_EQUAL_INT(0, asc(str, 369));
}

void test_built_in_subStr(void) {
    char str[] = "Testovaci";

    char *test = subStr(str, 0, 5);
    TEST_ASSERT_EQUAL_STRING("", test);
    free(test);

    char *test = subStr(str, -2, 5);
    TEST_ASSERT_EQUAL_STRING("", test);
    free(test);

    char *test = subStr("", 1, 1);
    TEST_ASSERT_EQUAL_STRING("", test);
    free(test);

    char *test = subStr(str, 1, 0);
    TEST_ASSERT_EQUAL_STRING("", test);
    free(test);

    char *test = subStr(str, 1, 0);
    TEST_ASSERT_EQUAL_STRING("", test);
    free(test);

    char *test = subStr(str, 1, -2);
    TEST_ASSERT_EQUAL_STRING("Testovaci", test);
    free(test);

    char *test = subStr(str, 3, -2);
    TEST_ASSERT_EQUAL_STRING("stovaci", test);
    free(test);

    char *test = subStr(str, 1, 50);
    TEST_ASSERT_EQUAL_STRING("Testovaci", test);
    free(test);

    char *test = subStr(str, 3, 50);
    TEST_ASSERT_EQUAL_STRING("stovaci", test);
    free(test);

    char *test = subStr(str, 3, 1);
    TEST_ASSERT_EQUAL_STRING("s", test);
    free(test);

    char *test = subStr(str, 3, 2);
    TEST_ASSERT_EQUAL_STRING("st", test);
    free(test);

    char *test = subStr(str, 1, 2);
    TEST_ASSERT_EQUAL_STRING("Te", test);
    free(test);

    char *test = subStr(str, 4, 4);
    TEST_ASSERT_EQUAL_STRING("tova", test);
    free(test);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_built_in_chr);
    RUN_TEST(test_built_in_asc);
    RUN_TEST(test_built_in_subStr);
    return UNITY_END();
}