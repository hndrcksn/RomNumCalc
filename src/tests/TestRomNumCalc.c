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
    ck_assert_int_eq(sequenceInRomNumeralIsValid(romNum), 1);

    numeralReplace(romNum, 2, "II");
    ck_assert_int_eq(sequenceInRomNumeralIsValid(romNum), 2);

    numeralReplace(romNum, 3, "III");
    ck_assert_int_eq(sequenceInRomNumeralIsValid(romNum), 3);

    numeralReplace(romNum, 0, "IIII");
    ck_assert_int_eq(sequenceInRomNumeralIsValid(romNum), 0);

    // Is a sequence of 5 I's valid?
    numeralReplace(romNum, 0, "IIIII");
    ck_assert_int_eq(sequenceInRomNumeralIsValid(romNum), 0);

    // Start with invalid character
    numeralReplace(romNum, 0, "AII");
    ck_assert_int_eq(sequenceInRomNumeralIsValid(romNum), 0);

    // Invalid character in the middle
    numeralReplace(romNum, 0, "IAI");
    ck_assert_int_eq(sequenceInRomNumeralIsValid(romNum), 0);

    // Invalid character at the end
    numeralReplace(romNum, 0, "IIA");
    ck_assert_int_eq(sequenceInRomNumeralIsValid(romNum), 0);

    // Is a sequence of X's C's or M's valid?
    numeralReplace(romNum, 10, "X");
    ck_assert_int_eq(sequenceInRomNumeralIsValid(romNum), 1);

    numeralReplace(romNum, 20, "XX");
    ck_assert_int_eq(sequenceInRomNumeralIsValid(romNum), 2);

    numeralReplace(romNum, 30, "XXX");
    ck_assert_int_eq(sequenceInRomNumeralIsValid(romNum), 3);

    numeralReplace(romNum, 100, "C");
    ck_assert_int_eq(sequenceInRomNumeralIsValid(romNum), 1);

    numeralReplace(romNum, 200, "CC");
    ck_assert_int_eq(sequenceInRomNumeralIsValid(romNum), 2);

    numeralReplace(romNum, 300, "CCC");
    ck_assert_int_eq(sequenceInRomNumeralIsValid(romNum), 3);

    numeralReplace(romNum, 1000, "M");
    ck_assert_int_eq(sequenceInRomNumeralIsValid(romNum), 1);

    numeralReplace(romNum, 2000, "MM");
    ck_assert_int_eq(sequenceInRomNumeralIsValid(romNum), 2);

    numeralReplace(romNum, 3000, "MMM");
    ck_assert_int_eq(sequenceInRomNumeralIsValid(romNum), 3);

    numeralReplace(romNum, 0, "XXXX");
    ck_assert_int_eq(sequenceInRomNumeralIsValid(romNum), 0);

    numeralReplace(romNum, 5, "V");
    ck_assert_int_eq(sequenceInRomNumeralIsValid(romNum), 1);

    numeralReplace(romNum, 0, "VV");
    ck_assert_int_eq(sequenceInRomNumeralIsValid(romNum), 0);

    numeralReplace(romNum, 50, "L");
    ck_assert_int_eq(sequenceInRomNumeralIsValid(romNum), 1);

    numeralReplace(romNum, 0, "LL");
    ck_assert_int_eq(sequenceInRomNumeralIsValid(romNum), 0);

    numeralReplace(romNum, 500, "D");
    ck_assert_int_eq(sequenceInRomNumeralIsValid(romNum), 1);

    numeralReplace(romNum, 0, "DD");
    ck_assert_int_eq(sequenceInRomNumeralIsValid(romNum), 0);
    numeralDestroy(romNum);
}
END_TEST

START_TEST (testIsSubtractiveSequenceInNumeralStringValid)
{
    // Handle sequences where a lower character modifies a half
    // or full magnitude higher one
    RomNumeral *romNum;

    romNum = numeralCreate(4, "IV");
    ck_assert_int_eq(subtractiveSequenceInRomNumeralIsValid(romNum), 4);

    numeralReplace(romNum, 9, "IX");
    ck_assert_int_eq(subtractiveSequenceInRomNumeralIsValid(romNum), 9);

    numeralReplace(romNum, 0, "IL");
    ck_assert_int_eq(subtractiveSequenceInRomNumeralIsValid(romNum), 0);

    numeralReplace(romNum, 0, "IC");
    ck_assert_int_eq(subtractiveSequenceInRomNumeralIsValid(romNum), 0);

    numeralReplace(romNum, 0, "ID");
    ck_assert_int_eq(subtractiveSequenceInRomNumeralIsValid(romNum), 0);

    numeralReplace(romNum, 0, "IM");
    ck_assert_int_eq(subtractiveSequenceInRomNumeralIsValid(romNum), 0);

    numeralReplace(romNum, 40, "XL");
    ck_assert_int_eq(subtractiveSequenceInRomNumeralIsValid(romNum), 40);

    numeralReplace(romNum, 90, "XC");
    ck_assert_int_eq(subtractiveSequenceInRomNumeralIsValid(romNum), 90);

    numeralReplace(romNum, 0, "XD");
    ck_assert_int_eq(subtractiveSequenceInRomNumeralIsValid(romNum), 0);

    numeralReplace(romNum, 0, "XM");
    ck_assert_int_eq(subtractiveSequenceInRomNumeralIsValid(romNum), 0);

    numeralReplace(romNum, 400, "CD");
    ck_assert_int_eq(subtractiveSequenceInRomNumeralIsValid(romNum), 400);

    numeralReplace(romNum, 900, "CM");
    ck_assert_int_eq(subtractiveSequenceInRomNumeralIsValid(romNum), 900);
    numeralDestroy(romNum);
}
END_TEST

START_TEST (testIsNumeralStringValid)
{
    // Validate a RomNumeral as having a proper Roman numeral
    // string with a matching integer
    RomNumeral *romNum;

    romNum = numeralCreate(1, "I");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 2, "II");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 3, "III");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 0, "IIII");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 4, "IV");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 5, "V");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 6, "VI");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 7, "VII");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 8, "VIII");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 9, "IX");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 10, "X");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 11, "XI");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 12, "XII");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 13, "XIII");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 14, "XIV");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 15, "XV");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 16, "XVI");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 17, "XVII");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 18, "XVIII");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 19, "XIX");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 20, "XX");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 21, "XXI");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 22, "XXII");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 23, "XXIII");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 24, "XXIV");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 25, "XXV");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 26, "XXVI");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 27, "XXVII");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 28, "XXVIII");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 29, "XXIX");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 30, "XXX");
    ck_assert(isValid(romNum));

    // Skip to more complicated numbers
    numeralReplace(romNum, 39, "XXXIX");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 40, "XL");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 41, "XLI");
    ck_assert(isValid(romNum));

    // Skip to more complicated numbers
    numeralReplace(romNum, 49, "XLIX");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 50, "L");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 51, "LI");
    ck_assert(isValid(romNum));

    // Skip to more complicated numbers
    numeralReplace(romNum, 89, "LXXXIX");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 90, "XC");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 91, "XCI");
    ck_assert(isValid(romNum));

    // Skip to more complicated number
    numeralReplace(romNum, 99, "XCIX");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 100, "C");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 101, "CI");
    ck_assert(isValid(romNum));
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
    suite_add_tcase (s, tc_core);

    TCase *tc_parsing = tcase_create ("Parsing");
    tcase_add_test (tc_parsing, testEachCharInNumeralStringIsClean);
    tcase_add_test (tc_parsing, testFindDirtyCharInNumeralString);
    tcase_add_test (tc_parsing, testIsSequenceInNumeralStringValid);
    tcase_add_test (tc_parsing, testIsSubtractiveSequenceInNumeralStringValid);
    tcase_add_test (tc_parsing, testIsNumeralStringValid);
    suite_add_tcase (s, tc_parsing);

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

