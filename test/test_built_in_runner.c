#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(BuiltInCode)
{
RUN_TEST_CASE(BuiltInCode, function_chr);
RUN_TEST_CASE(BuiltInCode, function_asc);
RUN_TEST_CASE(BuiltInCode, function_subStr);
}