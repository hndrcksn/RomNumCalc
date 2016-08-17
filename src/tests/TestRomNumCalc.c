#include <stdlib.h>
#include <check.h>
#include "../lib/RomNumMath.h"

START_TEST (testEachCharInStringIsCleanAndValid)
{
    // Test that every character in the numeral string is a I, V, X, L, C, D or M and nothing else
    ck_assert(isCleanValidString("MMDCCXLVII"));
    ck_assert(!isCleanValidString("IVXLCDMMDCLXVIIVXLCDM"));
    ck_assert(!isCleanValidString("IIIIVVVVXXXXLLLLCCCCDDDDMMMM"));
}
END_TEST

START_TEST (testFindDirtyCharInString)
{
    // Find an invalid character in the numeral string (not I, V, X, L, C, D or M)
    ck_assert(!isCleanValidString("IVXLADMMDC4VIIVXLCQM"));
    ck_assert(!isCleanValidString("1234ADMMDC4VIIVXLQDRG"));
    ck_assert(!isCleanValidString("ivxlcdm"));
}
END_TEST

START_TEST (testIsSequenceInStringValid)
{
    // Is a valid sequence of I's in string?
    ck_assert(isCleanValidString("MCCCXXXII"));
    ck_assert(isCleanValidString("I"));
    ck_assert(isCleanValidString("II"));
    ck_assert(isCleanValidString("III"));

    // Is a sequence of 5 I's valid?
    ck_assert(!isCleanValidString("IIII"));

    // Is a sequence of 5 I's valid?
    ck_assert(!isCleanValidString("IIIII"));

    // Start with invalid character
    ck_assert(!isCleanValidString("AII"));

    // Invalid character in the middle
    ck_assert(!isCleanValidString("IAI"));

    // Invalid character at the end
    ck_assert(!isCleanValidString("IIA"));

    // Is a sequence of X's C's or M's valid?
    ck_assert(isCleanValidString("X"));
    ck_assert(isCleanValidString("XXX"));
    ck_assert(isCleanValidString("MM"));
    ck_assert(!isCleanValidString("XXXX"));
    ck_assert(isCleanValidString("V"));
    ck_assert(!isCleanValidString("VV"));
    ck_assert(isCleanValidString("L"));
    ck_assert(!isCleanValidString("DD"));
}
END_TEST

START_TEST (testIsSubtractiveSequenceInStringValid)
{
    // Handle sequences where a lower character modifies a half
    // or full magnitude higher one
    ck_assert(isStringSubtractive("IV"));
    ck_assert(isStringSubtractive("IX"));
    ck_assert(!isStringSubtractive("IL"));
    ck_assert(!isStringSubtractive("IC"));
    ck_assert(!isStringSubtractive("ID"));
    ck_assert(!isStringSubtractive("IM"));
    ck_assert(isStringSubtractive("XL"));
    ck_assert(isStringSubtractive("XC"));
    ck_assert(!isStringSubtractive("XD"));
    ck_assert(!isStringSubtractive("XM"));
    ck_assert(isStringSubtractive("CD"));
    ck_assert(isStringSubtractive("CM"));
}
END_TEST

START_TEST (testIsStringValid)
{
    // Validate a RomNumeral as having a proper Roman numeral
    // string with a matching integer
    ck_assert(isCleanValidString("I"));
    ck_assert(isCleanValidString("IV"));
    ck_assert(isCleanValidString("V"));
    ck_assert(isCleanValidString("VIII"));
    ck_assert(isCleanValidString("IX"));

    // Skip to more complicated numbers
    ck_assert(isCleanValidString("XLIX"));
    ck_assert(isCleanValidString("XL"));
    ck_assert(isCleanValidString("XLI"));

    // Skip to more complicated numbers
    ck_assert(isCleanValidString("LXXXIX"));
    ck_assert(isCleanValidString("XC"));
    ck_assert(isCleanValidString("XCI"));

    // Skip to more complicated number
    ck_assert(isCleanValidString("XCIX"));
    ck_assert(isCleanValidString("DXCIX"));

    // Skip to more complicated number
    ck_assert(isCleanValidString("CMXCIX"));
    ck_assert(isCleanValidString("MI"));

    // Skip to more complicated number
    ck_assert(isCleanValidString("MMMCMXCVIII"));
    ck_assert(isCleanValidString("MMMCMXCIX"));
    ck_assert(isCleanValidString("LIV"));
}
END_TEST

START_TEST (testStringValidation)
{
    ck_assert(isCleanValidString("MCCCXXXII"));
    ck_assert(isCleanValidString("MCDLXXVIII"));
    ck_assert(isCleanValidString("MMCMXCIX"));
    ck_assert(isCleanValidString("I"));
    ck_assert(isCleanValidString("X"));
}
END_TEST

START_TEST (testAdditionValidation)
{
    char output[BUFRSIZE];
    memset(output, 0, BUFRSIZE);

    ck_assert_str_eq(addition("XX", "II", output), "XXII");
    ck_assert_str_eq(addition("XIX", "C", output), "CXIX");
    ck_assert_str_eq(addition("VI", "V", output), "XI");
    ck_assert_str_eq(addition("V", "IV", output), "IX");
    ck_assert_str_eq(addition("IV", "IV", output), "VIII");
    ck_assert_str_eq(addition("I", "III", output), "IV");
    ck_assert_str_eq(addition("VIII", "VIII", output), "XVI");
    ck_assert_str_eq(addition("VII", "VII", output), "XIV");
    ck_assert_str_eq(addition("CXC", "M", output), "MCXC");
    ck_assert_str_eq(addition("XL", "XL", output), "LXXX");
    ck_assert_str_eq(addition("X", "XXX", output), "XL");
    ck_assert_str_eq(addition("LXXX", "LXXX", output), "CLX");
    ck_assert_str_eq(addition("DC", "D", output), "MC");
    ck_assert_str_eq(addition("C", "CCC", output), "CD");
    ck_assert_str_eq(addition("DCC", "DCC", output), "MCD");
    ck_assert_str_eq(addition("MMVIII", "MCVIII", output), "MMMCXVI");
    ck_assert_str_eq(addition("MVIII", "MMLXXVII", output), "MMMLXXXV");
    ck_assert_str_eq(addition("XLVII", "XXIX", output), "LXXVI");
}
END_TEST

START_TEST (testSubtractionValidation)
{
    char output[BUFRSIZE];
    memset(output, 0, BUFRSIZE);

    ck_assert_str_eq(subtraction("III", "I", output), "II");
    ck_assert_str_eq(subtraction("IV", "I", output), "III");
    ck_assert_str_eq(subtraction("XIV", "I", output), "XIII");
    ck_assert_str_eq(subtraction("IV", "II", output), "II");
    ck_assert_str_eq(subtraction("IX", "IV", output), "V");
    ck_assert_str_eq(subtraction("XIV", "III", output), "XI");
    ck_assert_str_eq(subtraction("XIV", "IV", output), "X");
    ck_assert_str_eq(subtraction("MMM", "I", output), "MMCMXCIX");
    ck_assert_str_eq(subtraction("MCM", "M", output), "CM");
    ck_assert_str_eq(subtraction("XC", "XX", output), "LXX");
    ck_assert_str_eq(subtraction("D", "C", output), "CD");
    ck_assert_str_eq(subtraction("MMMDXCVI", "MXXII", output), "MMDLXXIV");
    ck_assert_str_eq(subtraction("I", "III", output), "-II");
    ck_assert_str_eq(subtraction("I", "MMM", output), "-MMCMXCIX");
    ck_assert_str_eq(subtraction("XXIX", "XLVII", output), "-XVIII");
    ck_assert_str_eq(subtraction("IX", "IX", output), "");
    ck_assert_str_eq(subtraction("M", "MMC", output), "-MC");
    ck_assert_str_eq(subtraction("MVII", "MCMLXVII", output), "-CMLX");
}
END_TEST

START_TEST (testPositiveComparison)
{
    // Relative positions of Roman numerals within their magnitude
    ck_assert_int_eq(romNumRelVal("I", 1, 0), 0);
    ck_assert_int_eq(romNumRelVal("VIII", 4, 0), 7);
    ck_assert_int_eq(romNumRelVal("X", 1, 1), 0);
    ck_assert_int_eq(romNumRelVal("XL", 2, 1), 3);
    ck_assert_int_eq(romNumRelVal("CC", 2, 2), 1);
    ck_assert_int_eq(romNumRelVal("CM", 2, 2), 8);
    ck_assert_int_eq(romNumRelVal("M", 1, 3), 0);
    ck_assert_int_eq(romNumRelVal("MMM", 3, 3), 2);

    // Is the first string greater than/less than/equal to the second?
    ck_assert_int_eq(romStrCmp("III", "III"), 0);
    ck_assert_int_eq(romStrCmp("IV", "III"), 1);
    ck_assert_int_eq(romStrCmp("III", "IX"), -1);
    ck_assert_int_eq(romStrCmp("XX", "XIX"), 1);
    ck_assert_int_eq(romStrCmp("DCCCXXIX", "CM"), -1);
    ck_assert_int_eq(romStrCmp("MCM", "MCMVII"), -1);
}
END_TEST

START_TEST (testNegativeAddition)
{
    char output[BUFRSIZE];
    memset(output, 0, BUFRSIZE);

	// Simple addition
    ck_assert_str_eq(addition("I", "I", output), "II");
    ck_assert_str_eq(addition("-I", "I", output), "");
    ck_assert_str_eq(addition("I", "-I", output), "");
    ck_assert_str_eq(addition("-I", "-I", output), "-II");

	// Addition of different values within order of magnitude
    ck_assert_str_eq(addition("I", "V", output), "VI");
    ck_assert_str_eq(addition("V", "I", output), "VI");
    ck_assert_str_eq(addition("-I", "V", output), "IV");
    ck_assert_str_eq(addition("-V", "I", output), "-IV");
    ck_assert_str_eq(addition("I", "-V", output), "-IV");
    ck_assert_str_eq(addition("V", "-I", output), "IV");
    ck_assert_str_eq(addition("-I", "-V", output), "-VI");
    ck_assert_str_eq(addition("-V", "-I", output), "-VI");

	// Addition of different values across order of magnitude
    ck_assert_str_eq(addition("I", "X", output), "XI");
    ck_assert_str_eq(addition("X", "I", output), "XI");
    ck_assert_str_eq(addition("-I", "X", output), "IX");
    ck_assert_str_eq(addition("-X", "I", output), "-IX");
    ck_assert_str_eq(addition("I", "-X", output), "-IX");
    ck_assert_str_eq(addition("X", "-I", output), "IX");
    ck_assert_str_eq(addition("-I", "-X", output), "-XI");
    ck_assert_str_eq(addition("-X", "-I", output), "-XI");
}
END_TEST

START_TEST (testNegativeSubtraction)
{
    char output[BUFRSIZE];
    memset(output, 0, BUFRSIZE);

	// Simple subtraction
    ck_assert_str_eq(subtraction("I", "I", output), "");
    ck_assert_str_eq(subtraction("-I", "I", output), "-II");
    ck_assert_str_eq(subtraction("I", "-I", output), "II");
    ck_assert_str_eq(subtraction("-I", "-I", output), "");

	// Subtraction of different values within order of magnitude
    ck_assert_str_eq(subtraction("I", "V", output), "-IV");
    ck_assert_str_eq(subtraction("V", "I", output), "IV");
    ck_assert_str_eq(subtraction("-I", "V", output), "-VI");
    ck_assert_str_eq(subtraction("-V", "I", output), "-VI");
    ck_assert_str_eq(subtraction("I", "-V", output), "VI");
    ck_assert_str_eq(subtraction("V", "-I", output), "VI");
    ck_assert_str_eq(subtraction("-I", "-V", output), "IV");
    ck_assert_str_eq(subtraction("-V", "-I", output), "-IV");

	// Subtraction of different values across order of magnitude
    ck_assert_str_eq(subtraction("I", "X", output), "-IX");
    ck_assert_str_eq(subtraction("X", "I", output), "IX");
    ck_assert_str_eq(subtraction("-I", "X", output), "-XI");
    ck_assert_str_eq(subtraction("-X", "I", output), "-XI");
    ck_assert_str_eq(subtraction("I", "-X", output), "XI");
    ck_assert_str_eq(subtraction("X", "-I", output), "XI");
    ck_assert_str_eq(subtraction("-I", "-X", output), "IX");
    ck_assert_str_eq(subtraction("-X", "-I", output), "-IX");
}
END_TEST

START_TEST (testNegativeComparison)
{
    char output[BUFRSIZE];
    memset(output, 0, BUFRSIZE);

	// Simple comparison
    ck_assert_int_eq(romStrCmp("I", "I"), 0);
    ck_assert_int_eq(romStrCmp("-I", "I"), -1);
    ck_assert_int_eq(romStrCmp("I", "-I"), 1);
    ck_assert_int_eq(romStrCmp("-I", "-I"), 0);

	// Comparison of different values within order of magnitude
    ck_assert_int_eq(romStrCmp("I", "V"), -1);
    ck_assert_int_eq(romStrCmp("V", "I"), 1);
    ck_assert_int_eq(romStrCmp("-I", "V"), -1);
    ck_assert_int_eq(romStrCmp("-V", "I"), -1);
    ck_assert_int_eq(romStrCmp("I", "-V"), 1);
    ck_assert_int_eq(romStrCmp("V", "-I"), 1);
    ck_assert_int_eq(romStrCmp("-I", "-V"), 1);
    ck_assert_int_eq(romStrCmp("-V", "-I"), -1);

	// Comparison of different values across order of magnitude
    ck_assert_int_eq(romStrCmp("I", "X"), -1);
    ck_assert_int_eq(romStrCmp("X", "I"), 1);
    ck_assert_int_eq(romStrCmp("-I", "X"), -1);
    ck_assert_int_eq(romStrCmp("-X", "I"), -1);
    ck_assert_int_eq(romStrCmp("I", "-X"), 1);
    ck_assert_int_eq(romStrCmp("X", "-I"), 1);
    ck_assert_int_eq(romStrCmp("-I", "-X"), 1);
    ck_assert_int_eq(romStrCmp("-X", "-I"), -1);
}
END_TEST

Suite *romNumTestSuite (void)
{
    Suite *s = suite_create ("RomNumeral");

    /* Parsing test case */
    TCase *tc_parsing = tcase_create ("Parsing");
    tcase_add_test (tc_parsing, testEachCharInStringIsCleanAndValid);
    tcase_add_test (tc_parsing, testFindDirtyCharInString);
    tcase_add_test (tc_parsing, testIsSequenceInStringValid);
    tcase_add_test (tc_parsing, testIsSubtractiveSequenceInStringValid);
    tcase_add_test (tc_parsing, testIsStringValid);
    // String output attempt
    tcase_add_test (tc_parsing, testStringValidation);
    suite_add_tcase (s, tc_parsing);

    /* Calculation test case */
    TCase *tc_calculation = tcase_create ("Calculation");
    // Addition and Subtraction tests
    tcase_add_test (tc_calculation, testAdditionValidation);
    tcase_add_test (tc_calculation, testSubtractionValidation);
    suite_add_tcase (s, tc_calculation);

    /* Comparison test case */
    TCase *tc_comparison = tcase_create ("Comparison");
    // Compare number string values
    tcase_add_test (tc_comparison, testPositiveComparison);
    suite_add_tcase (s, tc_comparison);

    /* Negation test case */
    TCase *tc_negation = tcase_create ("Negation");
    // Test proper handling of negative input
    tcase_add_test (tc_negation, testNegativeAddition);
    tcase_add_test (tc_negation, testNegativeSubtraction);
    tcase_add_test (tc_negation, testNegativeComparison);
    suite_add_tcase (s, tc_negation);

    return s;
}

int main(void)
{
#ifdef CHECK_DEBUG
    global_debugging = true;
#endif
    int number_failed;
    Suite *s = romNumTestSuite ();
    SRunner *sr = srunner_create (s);
    srunner_run_all (sr, CK_NORMAL);
    number_failed = srunner_ntests_failed (sr);
    srunner_free (sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

