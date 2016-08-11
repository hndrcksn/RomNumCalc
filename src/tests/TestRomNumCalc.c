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
    ck_assert(isCleanValidString("XX"));
    ck_assert(isCleanValidString("XXX"));
    ck_assert(isCleanValidString("C"));
    ck_assert(isCleanValidString("CC"));
    ck_assert(isCleanValidString("CCC"));
    ck_assert(isCleanValidString("M"));
    ck_assert(isCleanValidString("MM"));
    ck_assert(!isCleanValidString("XXXX"));
    ck_assert(isCleanValidString("V"));
    ck_assert(!isCleanValidString("VV"));
    ck_assert(isCleanValidString("L"));
    ck_assert(!isCleanValidString("LL"));
    ck_assert(isCleanValidString("D"));
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
    ck_assert(isCleanValidString("II"));
    ck_assert(isCleanValidString("III"));
    ck_assert(isCleanValidString("IV"));
    ck_assert(isCleanValidString("V"));
    ck_assert(isCleanValidString("VI"));
    ck_assert(isCleanValidString("VII"));
    ck_assert(isCleanValidString("VIII"));
    ck_assert(isCleanValidString("IX"));
    ck_assert(isCleanValidString("X"));
    ck_assert(isCleanValidString("XI"));
    ck_assert(isCleanValidString("XII"));
    ck_assert(isCleanValidString("XIII"));
    ck_assert(isCleanValidString("XIV"));
    ck_assert(isCleanValidString("XV"));
    ck_assert(isCleanValidString("XVI"));
    ck_assert(isCleanValidString("XVII"));
    ck_assert(isCleanValidString("XVIII"));
    ck_assert(isCleanValidString("XIX"));
    ck_assert(isCleanValidString("XX"));
    ck_assert(isCleanValidString("XXI"));
    ck_assert(isCleanValidString("XXII"));
    ck_assert(isCleanValidString("XXIII"));
    ck_assert(isCleanValidString("XXIV"));
    ck_assert(isCleanValidString("XXV"));
    ck_assert(isCleanValidString("XXVI"));
    ck_assert(isCleanValidString("XXVII"));
    ck_assert(isCleanValidString("XXVIII"));
    ck_assert(isCleanValidString("XXIX"));
    ck_assert(isCleanValidString("XXX"));
    ck_assert(isCleanValidString("XXXI"));
    ck_assert(isCleanValidString("XXXII"));
    ck_assert(isCleanValidString("XXXIII"));
    ck_assert(isCleanValidString("XXXIV"));
    ck_assert(isCleanValidString("XXXV"));
    ck_assert(isCleanValidString("XXXVI"));
    ck_assert(isCleanValidString("XXXVII"));
    ck_assert(isCleanValidString("XXXVIII"));
    ck_assert(isCleanValidString("XXXIX"));
    ck_assert(!isCleanValidString("XXXX"));
    ck_assert(isCleanValidString("XL"));
    ck_assert(isCleanValidString("XLI"));

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
    ck_assert(isCleanValidString("C"));
    ck_assert(isCleanValidString("CI"));

    // Skip to more complicated number
    ck_assert(isCleanValidString("CXLIX"));
    ck_assert(!isCleanValidString("CXLXI"));
    ck_assert(isCleanValidString("CL"));
    ck_assert(isCleanValidString("CLI"));

    // Skip to more complicated number
    ck_assert(isCleanValidString("CXCIX"));
    ck_assert(isCleanValidString("CC"));
    ck_assert(isCleanValidString("CCI"));

    // Skip to more complicated number
    ck_assert(isCleanValidString("CDXCIX"));
    ck_assert(isCleanValidString("D"));
    ck_assert(isCleanValidString("DI"));

    // Skip to more complicated number
    ck_assert(isCleanValidString("DXCIX"));
    ck_assert(isCleanValidString("DL"));
    ck_assert(isCleanValidString("DLI"));

    // Skip to more complicated number
    ck_assert(isCleanValidString("CMXCIX"));
    ck_assert(isCleanValidString("M"));
    ck_assert(isCleanValidString("MI"));

    // Skip to more complicated number
    ck_assert(isCleanValidString("MCMXCIX"));
    ck_assert(isCleanValidString("MM"));
    ck_assert(isCleanValidString("MMI"));

    // Skip to more complicated number
    ck_assert(isCleanValidString("MMCMXCIX"));
    ck_assert(isCleanValidString("MMM"));
    ck_assert(isCleanValidString("MMMI"));

    // Skip to more complicated number
    ck_assert(isCleanValidString("MMMCMXCVII"));
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
    char output[32];
    memset(output, 0, 32);

    ck_assert_str_eq(addition("XX", "II", output), "XXII");
    ck_assert_str_eq(addition("II", "XX", output), "XXII");
    ck_assert_str_eq(addition("XIX", "C", output), "CXIX");
    ck_assert_str_eq(addition("VI", "V", output), "XI");
    ck_assert_str_eq(addition("V", "VI", output), "XI");
    ck_assert_str_eq(addition("IV", "V", output), "IX");
    ck_assert_str_eq(addition("V", "IV", output), "IX");
    ck_assert_str_eq(addition("IV", "IV", output), "VIII");
    ck_assert_str_eq(addition("I", "III", output), "IV");
    ck_assert_str_eq(addition("VIII", "VIII", output), "XVI");
    ck_assert_str_eq(addition("VIII", "VII", output), "XV");
    ck_assert_str_eq(addition("VII", "VII", output), "XIV");
    ck_assert_str_eq(addition("III", "I", output), "IV");
    ck_assert_str_eq(addition("CC", "XX", output), "CCXX");
    ck_assert_str_eq(addition("XX", "CC", output), "CCXX");
    ck_assert_str_eq(addition("CXC", "M", output), "MCXC");
    ck_assert_str_eq(addition("LX", "L", output), "CX");
    ck_assert_str_eq(addition("L", "LX", output), "CX");
    ck_assert_str_eq(addition("XL", "L", output), "XC");
    ck_assert_str_eq(addition("XL", "XL", output), "LXXX");
    ck_assert_str_eq(addition("X", "XXX", output), "XL");
    ck_assert_str_eq(addition("LXXX", "LXXX", output), "CLX");
    ck_assert_str_eq(addition("LXXX", "LXX", output), "CL");
    ck_assert_str_eq(addition("LXX", "LXX", output), "CXL");
    ck_assert_str_eq(addition("MM", "CC", output), "MMCC");
    ck_assert_str_eq(addition("CC", "MM", output), "MMCC");
    ck_assert_str_eq(addition("MCM", "M", output), "MMCM");
    ck_assert_str_eq(addition("DC", "D", output), "MC");
    ck_assert_str_eq(addition("D", "DC", output), "MC");
    ck_assert_str_eq(addition("CD", "D", output), "CM");
    ck_assert_str_eq(addition("CD", "CD", output), "DCCC");
    ck_assert_str_eq(addition("C", "CCC", output), "CD");
    ck_assert_str_eq(addition("DCCC", "DCCC", output), "MDC");
    ck_assert_str_eq(addition("DCCC", "DCC", output), "MD");
    ck_assert_str_eq(addition("DCC", "DCC", output), "MCD");
    ck_assert_str_eq(addition("XX", "MM", output), "MMXX");
    ck_assert_str_eq(addition("MM", "XX", output), "MMXX");
    ck_assert_str_eq(addition("MXIX", "C", output), "MCXIX");
    ck_assert_str_eq(addition("MVI", "V", output), "MXI");
    ck_assert_str_eq(addition("V", "MVI", output), "MXI");
    ck_assert_str_eq(addition("MV", "V", output), "MX");
    ck_assert_str_eq(addition("V", "MV", output), "MX");
    ck_assert_str_eq(addition("MV", "MV", output), "MMX");
    ck_assert_str_eq(addition("M", "MMC", output), "MMMC");
    ck_assert_str_eq(addition("MMVIII", "MCVIII", output), "MMMCXVI");
    ck_assert_str_eq(addition("MVIII", "MMLXXVII", output), "MMMLXXXV");
    ck_assert_str_eq(addition("MVII", "MCMLXVII", output), "MMCMLXXIV");
    ck_assert_str_eq(addition("MCM", "M", output), "MMCM");
    ck_assert_str_eq(addition("XLVII", "XXIX", output), "LXXVI");
//    ck_assert(isCleanValidString("LCCX"));
}
END_TEST

START_TEST (testSubtractionValidation)
{
    char output[32];
    memset(output, 0, 32);

    ck_assert_str_eq(subtraction("III", "I", output), "II");
    ck_assert_str_eq(subtraction("III", "II", output), "I");
    ck_assert_str_eq(subtraction("IV", "I", output), "III");
    ck_assert_str_eq(subtraction("XIV", "I", output), "XIII");
    ck_assert_str_eq(subtraction("IV", "II", output), "II");
    ck_assert_str_eq(subtraction("IV", "III", output), "I");
    ck_assert_str_eq(subtraction("IX", "IV", output), "V");
    ck_assert_str_eq(subtraction("XIV", "III", output), "XI");
    ck_assert_str_eq(subtraction("XIV", "IV", output), "X");
    ck_assert_str_eq(subtraction("XIV", "V", output), "IX");
    ck_assert_str_eq(subtraction("XXXIV", "V", output), "XXIX");
    ck_assert_str_eq(subtraction("XXXI", "II", output), "XXIX");
    ck_assert_str_eq(subtraction("XXXV", "VI", output), "XXIX");
    ck_assert_str_eq(subtraction("XX", "VIII", output), "XII");
    ck_assert_str_eq(subtraction("IV", "I", output), "III");
    ck_assert_str_eq(subtraction("CC", "I", output), "CXCIX");
    ck_assert_str_eq(subtraction("MMM", "I", output), "MMCMXCIX");
    ck_assert_str_eq(subtraction("MCM", "M", output), "CM");
    ck_assert_str_eq(subtraction("VI", "II", output), "IV");
    ck_assert_str_eq(subtraction("XC", "XX", output), "LXX");
    ck_assert_str_eq(subtraction("D", "C", output), "CD");
    ck_assert_str_eq(subtraction("MMM", "M", output), "MM");
    ck_assert_str_eq(subtraction("MMMDXCVI", "MXXII", output), "MMDLXXIV");
    ck_assert_str_eq(subtraction("I", "II", output), "-I");
    ck_assert_str_eq(subtraction("IV", "V", output), "-I");
    ck_assert_str_eq(subtraction("I", "III", output), "-II");
    ck_assert_str_eq(subtraction("I", "MMM", output), "-MMCMXCIX");
    ck_assert_str_eq(subtraction("XLVII", "XXIX", output), "XVIII");
    ck_assert_str_eq(subtraction("XXIX", "XLVII", output), "-XVIII");
    ck_assert_str_eq(subtraction("IX", "IX", output), "");
    ck_assert_str_eq(subtraction("M", "MMC", output), "-MC");
    ck_assert_str_eq(subtraction("MMC", "M", output), "MC");
    ck_assert_str_eq(subtraction("MMVIII", "MCVIII", output), "CM");
    ck_assert_str_eq(subtraction("MVIII", "MMLXXVII", output), "-MLXIX");
    ck_assert_str_eq(subtraction("MMLXXVII", "MVIII", output), "MLXIX");
    ck_assert_str_eq(subtraction("MCMLXVII", "MVII", output), "CMLX");
    ck_assert_str_eq(subtraction("MVII", "MCMLXVII", output), "-CMLX");
    ck_assert_str_eq(subtraction("MCM", "M", output), "CM");
    ck_assert_str_eq(subtraction("M", "MCM", output), "-CM");
    ck_assert_str_eq(subtraction("XLVII", "XXIX", output), "XVIII");
    ck_assert_str_eq(subtraction("XXIX", "XLVII", output), "-XVIII");
//    ck_assert(isCleanValidString("LCCX"));
}
END_TEST

START_TEST (testComparison)
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
//    ck_assert_int_eq(1, 0);
}
END_TEST

START_TEST (testNegation)
{
    char output[32];
    memset(output, 0, 32);

    ck_assert_str_eq(addition("I", "I", output), "II");
//    ck_assert_str_eq(addition("-I", "I", output), "");
//    ck_assert_str_eq(addition("I", "-I", output), "");
//    ck_assert_str_eq(addition("-I", "-I", output), "-II");

    ck_assert_str_eq(subtraction("I", "I", output), "");
//    ck_assert_str_eq(subtraction("-I", "I", output), "-II");
//    ck_assert_str_eq(subtraction("I", "-I", output), "II");
//    ck_assert_str_eq(subtraction("-I", "-I", output), "");

    ck_assert_int_eq(romStrCmp("I", "I"), 0);
//    ck_assert_int_eq(romStrCmp("-I", "I"), -1);
//    ck_assert_int_eq(romStrCmp("I", "-I"), 1);
//    ck_assert_int_eq(romStrCmp("-I", "-I"), 0);
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
    tcase_add_test (tc_comparison, testComparison);
    suite_add_tcase (s, tc_comparison);

    /* Negation test case */
    TCase *tc_negation = tcase_create ("Negation");
    // Test proper handling of negative input
    tcase_add_test (tc_negation, testNegation);
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

