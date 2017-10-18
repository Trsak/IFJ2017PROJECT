#include "unity.h"
#include "unity_fixture.h"
#include "built_in.h"

TEST_GROUP(BuiltInCode);

TEST_SETUP(ProductionCode)
{
}

TEST_TEAR_DOWN(ProductionCode)
{
}

TEST(BuiltInCode, function_chr)
{
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

TEST(BuiltInCode, function_asc)
{
char str[] = "Testovaci";

TEST_ASSERT_EQUAL_INT(84, asc(str, 0));
TEST_ASSERT_EQUAL_INT(105, asc(str, 8));
TEST_ASSERT_EQUAL_INT(65, asc("AB", 0));
TEST_ASSERT_EQUAL_INT(66, asc("AB", 1));
TEST_ASSERT_EQUAL_INT(0, asc(str, -1));
TEST_ASSERT_EQUAL_INT(0, asc(str, -25));
TEST_ASSERT_EQUAL_INT(0, asc(str, 9));
TEST_ASSERT_EQUAL_INT(0, asc(str, 12));
TEST_ASSERT_EQUAL_INT(0, asc(str, 369));
}

TEST(BuiltInCode, function_subStr)
{
char str[] = "Testovaci";

char *test;

test = subStr(str, 0, 5);
TEST_ASSERT_EQUAL_STRING("", test);
free(test);

test = subStr(str, -2, 5);
TEST_ASSERT_EQUAL_STRING("", test);
free(test);

test = subStr("", 1, 1);
TEST_ASSERT_EQUAL_STRING("", test);
free(test);

test = subStr(str, 1, 0);
TEST_ASSERT_EQUAL_STRING("", test);
free(test);

test = subStr(str, 1, 0);
TEST_ASSERT_EQUAL_STRING("", test);
free(test);

test = subStr(str, 1, -2);
TEST_ASSERT_EQUAL_STRING("Testovaci", test);
free(test);

test = subStr(str, 3, -2);
TEST_ASSERT_EQUAL_STRING("stovaci", test);
free(test);

test = subStr(str, 1, 50);
TEST_ASSERT_EQUAL_STRING("Testovaci", test);
free(test);

test = subStr(str, 3, 50);
TEST_ASSERT_EQUAL_STRING("stovaci", test);
free(test);

test = subStr(str, 3, 1);
TEST_ASSERT_EQUAL_STRING("s", test);
free(test);

test = subStr(str, 3, 2);
TEST_ASSERT_EQUAL_STRING("st", test);
free(test);

test = subStr(str, 1, 2);
TEST_ASSERT_EQUAL_STRING("Te", test);
free(test);

test = subStr(str, 4, 4);
TEST_ASSERT_EQUAL_STRING("tova", test);
free(test);
}
