#include <stdlib.h>
#include <check.h>
#include "../lib/RomNumMath.h"

START_TEST (testNumeralCreate)
{
    // Simple test code to verify that check works as expected //

    RomNumeral *romNum;

    romNum = numeralCreate(4, "IV");
    ck_assert_int_eq(numeralValue(romNum), 4);
    ck_assert_str_eq(numeralString(romNum), "IV");
    numeralDestroy(romNum);
}
END_TEST

START_TEST (testFirstCharInNumeralStringIs_I)
{
    // Test if first character in the numeral string "I"
    RomNumeral *romNum;

    romNum = numeralCreate(1, "I");
    ck_assert(char0Is_I(romNum));
    numeralDestroy(romNum);
}
END_TEST

Suite *romNumTestSuite (void)
{
    Suite *s = suite_create ("RomNumeral");

    /* Core test case */
    TCase *tc_core = tcase_create ("Core");
    tcase_add_test (tc_core, testNumeralCreate);
    tcase_add_test (tc_core, testFirstCharInNumeralStringIs_I);
    suite_add_tcase (s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s = romNumTestSuite ();
    SRunner *sr = srunner_create (s);
    srunner_run_all (sr, CK_NORMAL);
    number_failed = srunner_ntests_failed (sr);
    srunner_free (sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
