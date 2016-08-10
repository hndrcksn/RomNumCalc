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

    addition("XX", "II", output);
    ck_assert_str_eq(output, "XXII");
    addition("II", "XX", output);
    ck_assert_str_eq(output, "XXII");
    addition("XIX", "C", output);
    ck_assert_str_eq(output, "CXIX");
    addition("VI", "V", output);
    ck_assert_str_eq(output, "XI");
    addition("V", "VI", output);
    ck_assert_str_eq(output, "XI");
    addition("IV", "V", output);
    ck_assert_str_eq(output, "IX");
    addition("V", "IV", output);
    ck_assert_str_eq(output, "IX");
    addition("IV", "IV", output);
    ck_assert_str_eq(output, "VIII");
    addition("I", "III", output);
    ck_assert_str_eq(output, "IV");
    addition("VIII", "VIII", output);
    ck_assert_str_eq(output, "XVI");
    addition("VIII", "VII", output);
    ck_assert_str_eq(output, "XV");
    addition("VII", "VII", output);
    ck_assert_str_eq(output, "XIV");
    addition("III", "I", output);
    ck_assert_str_eq(output, "IV");
    addition("CC", "XX", output);
    ck_assert_str_eq(output, "CCXX");
    addition("XX", "CC", output);
    ck_assert_str_eq(output, "CCXX");
    addition("CXC", "M", output);
    ck_assert_str_eq(output, "MCXC");
    addition("LX", "L", output);
    ck_assert_str_eq(output, "CX");
    addition("L", "LX", output);
    ck_assert_str_eq(output, "CX");
    addition("XL", "L", output);
    ck_assert_str_eq(output, "XC");
    addition("XL", "XL", output);
    ck_assert_str_eq(output, "LXXX");
    addition("X", "XXX", output);
    ck_assert_str_eq(output, "XL");
    addition("LXXX", "LXXX", output);
    ck_assert_str_eq(output, "CLX");
    addition("LXXX", "LXX", output);
    ck_assert_str_eq(output, "CL");
    addition("LXX", "LXX", output);
    ck_assert_str_eq(output, "CXL");
    addition("MM", "CC", output);
    ck_assert_str_eq(output, "MMCC");
    addition("CC", "MM", output);
    ck_assert_str_eq(output, "MMCC");
    addition("MCM", "M", output);
    ck_assert_str_eq(output, "MMCM");
    addition("DC", "D", output);
    ck_assert_str_eq(output, "MC");
    addition("D", "DC", output);
    ck_assert_str_eq(output, "MC");
    addition("CD", "D", output);
    ck_assert_str_eq(output, "CM");
    addition("CD", "CD", output);
    ck_assert_str_eq(output, "DCCC");
    addition("C", "CCC", output);
    ck_assert_str_eq(output, "CD");
    addition("DCCC", "DCCC", output);
    ck_assert_str_eq(output, "MDC");
    addition("DCCC", "DCC", output);
    ck_assert_str_eq(output, "MD");
    addition("DCC", "DCC", output);
    ck_assert_str_eq(output, "MCD");
    addition("XX", "MM", output);
    ck_assert_str_eq(output, "MMXX");
    addition("MM", "XX", output);
    ck_assert_str_eq(output, "MMXX");
    addition("MXIX", "C", output);
    ck_assert_str_eq(output, "MCXIX");
    addition("MVI", "V", output);
    ck_assert_str_eq(output, "MXI");
    addition("V", "MVI", output);
    ck_assert_str_eq(output, "MXI");
    addition("MV", "V", output);
    ck_assert_str_eq(output, "MX");
    addition("V", "MV", output);
    ck_assert_str_eq(output, "MX");
    addition("MV", "MV", output);
    ck_assert_str_eq(output, "MMX");
    addition("M", "MMC", output);
    ck_assert_str_eq(output, "MMMC");
    addition("MMVIII", "MCVIII", output);
    ck_assert_str_eq(output, "MMMCXVI");
    addition("MVIII", "MMLXXVII", output);
    ck_assert_str_eq(output, "MMMLXXXV");
    addition("MVII", "MCMLXVII", output);
    ck_assert_str_eq(output, "MMCMLXXIV");
    addition("MCM", "M", output);
    ck_assert_str_eq(output, "MMCM");
    addition("XLVII", "XXIX", output);
    ck_assert_str_eq(output, "LXXVI");
//    ck_assert(isCleanValidString("LCCX"));
}
END_TEST

START_TEST (testSubtractionValidation)
{
    char output[32];
    memset(output, 0, 32);

    subtraction("III", "I", output);
    ck_assert_str_eq(output, "II");
    subtraction("III", "II", output);
    ck_assert_str_eq(output, "I");
    subtraction("IV", "I", output);
    ck_assert_str_eq(output, "III");
    subtraction("XIV", "I", output);
    ck_assert_str_eq(output, "XIII");
    subtraction("IV", "II", output);
    ck_assert_str_eq(output, "II");
    subtraction("IV", "III", output);
    ck_assert_str_eq(output, "I");
    subtraction("IX", "IV", output);
    ck_assert_str_eq(output, "V");
    subtraction("XIV", "III", output);
    ck_assert_str_eq(output, "XI");
    subtraction("XIV", "IV", output);
    ck_assert_str_eq(output, "X");
    subtraction("XIV", "V", output);
    ck_assert_str_eq(output, "IX");
    subtraction("XXXIV", "V", output);
    ck_assert_str_eq(output, "XXIX");
    subtraction("XXXI", "II", output);
    ck_assert_str_eq(output, "XXIX");
    subtraction("XXXV", "VI", output);
    ck_assert_str_eq(output, "XXIX");
    subtraction("XX", "VIII", output);
    ck_assert_str_eq(output, "XII");
    subtraction("IV", "I", output);
    ck_assert_str_eq(output, "III");
    subtraction("CC", "I", output);
    ck_assert_str_eq(output, "CXCIX");
    subtraction("MMM", "I", output);
    ck_assert_str_eq(output, "MMCMXCIX");
    subtraction("MCM", "M", output);
    ck_assert_str_eq(output, "CM");
    subtraction("VI", "II", output);
    ck_assert_str_eq(output, "IV");
    subtraction("XC", "XX", output);
    ck_assert_str_eq(output, "LXX");
    subtraction("D", "C", output);
    ck_assert_str_eq(output, "CD");
    subtraction("MMM", "M", output);
    ck_assert_str_eq(output, "MM");
    subtraction("MMMDXCVI", "MXXII", output);
    ck_assert_str_eq(output, "MMDLXXIV");
    subtraction("I", "II", output);
    ck_assert_str_eq(output, "-I");
    subtraction("IV", "V", output);
    ck_assert_str_eq(output, "-I");
    subtraction("I", "III", output);
    ck_assert_str_eq(output, "-II");
    subtraction("I", "MMM", output);
    ck_assert_str_eq(output, "-MMCMXCIX");
    subtraction("XLVII", "XXIX", output);
    ck_assert_str_eq(output, "XVIII");
    subtraction("XXIX", "XLVII", output);
    ck_assert_str_eq(output, "-XVIII");
    subtraction("IX", "IX", output);
    ck_assert_str_eq(output, "");
    subtraction("M", "MMC", output);
    ck_assert_str_eq(output, "-MC");
    subtraction("MMC", "M", output);
    ck_assert_str_eq(output, "MC");
    subtraction("MMVIII", "MCVIII", output);
    ck_assert_str_eq(output, "CM");
    subtraction("MVIII", "MMLXXVII", output);
    ck_assert_str_eq(output, "-MLXIX");
    subtraction("MMLXXVII", "MVIII", output);
    ck_assert_str_eq(output, "MLXIX");
    subtraction("MCMLXVII", "MVII", output);
    ck_assert_str_eq(output, "CMLX");
    subtraction("MVII", "MCMLXVII", output);
    ck_assert_str_eq(output, "-CMLX");
    subtraction("MCM", "M", output);
    ck_assert_str_eq(output, "CM");
    subtraction("M", "MCM", output);
    ck_assert_str_eq(output, "-CM");
    subtraction("XLVII", "XXIX", output);
    ck_assert_str_eq(output, "XVIII");
    subtraction("XXIX", "XLVII", output);
    ck_assert_str_eq(output, "-XVIII");
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

