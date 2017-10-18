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
    TEST_ASSERT_EQUAL_INT(105, asc(str, 8));
    TEST_ASSERT_EQUAL_INT(0, asc(str, -1));
    TEST_ASSERT_EQUAL_INT(0, asc(str, -25));
    TEST_ASSERT_EQUAL_INT(0, asc(str, 9));
    TEST_ASSERT_EQUAL_INT(0, asc(str, 12));
    TEST_ASSERT_EQUAL_INT(0, asc(str, 369));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_built_in_chr);
    RUN_TEST(test_built_in_asc);
    return UNITY_END();
}