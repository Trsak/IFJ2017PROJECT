#include "built_in.h"
#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP(BuiltInCode);

TEST_SETUP(BuiltInCode)
{
}

TEST_TEAR_DOWN(BuiltInCode)
{
}

TEST(BuiltInCode, CHR_BUILTIN_FUNCTION)
{
    char *A = chr(65);
    TEST_ASSERT_EQUAL_STRING(A, "A");
    free(A);
}