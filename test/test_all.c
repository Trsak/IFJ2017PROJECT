#include "unity.h"
#include "test_built_in.c"

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_built_in_chr);
    RUN_TEST(test_built_in_asc);
    RUN_TEST(test_built_in_subStr);
    return UNITY_END();
}