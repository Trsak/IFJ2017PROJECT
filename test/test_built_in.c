#include "built_in.h"
#include "unity.h"

void test_built_in_chr(void) {
    char *A = chr(65);
    TEST_ASSERT_EQUAL_STRING(A, "A");
    free(A);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_built_in_chr);
    return UNITY_END();
}