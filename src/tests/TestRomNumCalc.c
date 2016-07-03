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

START_TEST (testSecondCharInNumeralStringIs_V)
{
    // Test if second character in the numeral string "V"
    RomNumeral *romNum;

    romNum = numeralCreate(4, "IV");
    ck_assert(char1Is_V(romNum));
    numeralDestroy(romNum);
}
END_TEST

START_TEST (testEachCharInNumeralStringIsClean)
{
    // Test that every character in the numeral string is a I, V, X, L, C, D or M and nothing else
    RomNumeral *romNum;

    romNum = numeralCreate(0, "IVXLCDMMDCLXVIIVXLCDM");
    ck_assert(numeralStringIsClean(romNum));
    numeralDestroy(romNum);
}
END_TEST

START_TEST (testFindDirtyCharInNumeralString)
{
    // Find an invalid character in the numeral string (not I, V, X, L, C, D or M)
    RomNumeral *romNum;

    romNum = numeralCreate(0, "IVXLCDMAMDCLXVIIVXLCDM");
    ck_assert(!numeralStringIsClean(romNum));
    numeralDestroy(romNum);
}
END_TEST

START_TEST (testIsSequenceInNumeralStringValid)
{
    // Is a valid sequence of I's in string?
    RomNumeral *romNum;

    romNum = numeralCreate(1, "I");
    ck_assert(sequenceInRomNumeralIsValid(romNum));
    numeralDestroy(romNum);

    romNum = numeralCreate(2, "II");
    ck_assert(sequenceInRomNumeralIsValid(romNum));
    numeralDestroy(romNum);

    romNum = numeralCreate(3, "III");
    ck_assert(sequenceInRomNumeralIsValid(romNum));
    numeralDestroy(romNum);

    romNum = numeralCreate(0, "IIII");
    ck_assert(!sequenceInRomNumeralIsValid(romNum));
    numeralDestroy(romNum);

    // Is a sequence of 5 I's valid?
    romNum = numeralCreate(0, "IIIII");
    ck_assert(!sequenceInRomNumeralIsValid(romNum));
    numeralDestroy(romNum);

    // Is a sequence of X's C's or M's valid?
    romNum = numeralCreate(10, "X");
    ck_assert(sequenceInRomNumeralIsValid(romNum));
    numeralDestroy(romNum);

    romNum = numeralCreate(20, "XX");
    ck_assert(sequenceInRomNumeralIsValid(romNum));
    numeralDestroy(romNum);

    romNum = numeralCreate(30, "XXX");
    ck_assert(sequenceInRomNumeralIsValid(romNum));
    numeralDestroy(romNum);

    romNum = numeralCreate(100, "C");
    ck_assert(sequenceInRomNumeralIsValid(romNum));
    numeralDestroy(romNum);

    romNum = numeralCreate(200, "CC");
    ck_assert(sequenceInRomNumeralIsValid(romNum));
    numeralDestroy(romNum);

    romNum = numeralCreate(300, "CCC");
    ck_assert(sequenceInRomNumeralIsValid(romNum));
    numeralDestroy(romNum);

    romNum = numeralCreate(1000, "M");
    ck_assert(sequenceInRomNumeralIsValid(romNum));
    numeralDestroy(romNum);

    romNum = numeralCreate(2000, "MM");
    ck_assert(sequenceInRomNumeralIsValid(romNum));
    numeralDestroy(romNum);

    romNum = numeralCreate(3000, "MMM");
    ck_assert(sequenceInRomNumeralIsValid(romNum));
    numeralDestroy(romNum);

    romNum = numeralCreate(0, "XXXX");
    ck_assert(!sequenceInRomNumeralIsValid(romNum));
    numeralDestroy(romNum);

    romNum = numeralCreate(5, "V");
    ck_assert(sequenceInRomNumeralIsValid(romNum));
    numeralDestroy(romNum);

    romNum = numeralCreate(0, "VV");
    ck_assert(!sequenceInRomNumeralIsValid(romNum));
    numeralDestroy(romNum);

    romNum = numeralCreate(50, "L");
    ck_assert(sequenceInRomNumeralIsValid(romNum));
    numeralDestroy(romNum);

    romNum = numeralCreate(0, "LL");
    ck_assert(!sequenceInRomNumeralIsValid(romNum));
    numeralDestroy(romNum);

    romNum = numeralCreate(500, "D");
    ck_assert(sequenceInRomNumeralIsValid(romNum));
    numeralDestroy(romNum);

    romNum = numeralCreate(0, "DD");
    ck_assert(!sequenceInRomNumeralIsValid(romNum));
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
    tcase_add_test (tc_core, testSecondCharInNumeralStringIs_V);
    tcase_add_test (tc_core, testEachCharInNumeralStringIsClean);
    tcase_add_test (tc_core, testFindDirtyCharInNumeralString);
    tcase_add_test (tc_core, testIsSequenceInNumeralStringValid);
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

