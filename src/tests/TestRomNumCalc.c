#include <check.h>
#include "../lib/RomNumMath.h"

START_TEST (test_integer_addition)
{
    /* Simple test code to verify that check works as expected */

    unsigned int num1, num2;
    num1 = 4;
    num2 = 5;
    ck_assert_int_eq(add(num1, num2), num1 + num2);
}
END_TEST

int main(void)
{
    printf("Running test suite...\n");
    return 0;
}
