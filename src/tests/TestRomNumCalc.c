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

    // Skip to more complicated number
    numeralReplace(romNum, 149, "CXLIX");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 150, "CL");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 151, "CLI");
    ck_assert(isValid(romNum));

    // Skip to more complicated number
    numeralReplace(romNum, 199, "CXCIX");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 200, "CC");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 201, "CCI");
    ck_assert(isValid(romNum));

    // Skip to more complicated number
    numeralReplace(romNum, 499, "CDXCIX");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 500, "D");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 501, "DI");
    ck_assert(isValid(romNum));

    // Skip to more complicated number
    numeralReplace(romNum, 599, "DXCIX");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 550, "DL");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 551, "DLI");
    ck_assert(isValid(romNum));

    // Skip to more complicated number
    numeralReplace(romNum, 999, "CMXCIX");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 1000, "M");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 1001, "MI");
    ck_assert(isValid(romNum));

    // Skip to more complicated number
    numeralReplace(romNum, 1999, "MCMXCIX");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 2000, "MM");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 2001, "MMI");
    ck_assert(isValid(romNum));

    // Skip to more complicated number
    numeralReplace(romNum, 2999, "MMCMXCIX");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 3000, "MMM");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 3001, "MMMI");
    ck_assert(isValid(romNum));

    // Skip to more complicated number
    numeralReplace(romNum, 3997, "MMMCMXCVII");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 3998, "MMMCMXCVIII");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 3999, "MMMCMXCIX");
    ck_assert(isValid(romNum));

    numeralReplace(romNum, 54, "LIV");
    ck_assert(isValid(romNum));
    numeralDestroy(romNum);
}
END_TEST

START_TEST (testIntToRomNumStringConversion)
{
    char output[255];
    char *outValue = NULL;
    int input = 1332;
    outValue = intToRomNum(input,  &output[0]);
    ck_assert_str_eq(outValue, "MCCCXXXII");

    input = 1;
    outValue = intToRomNum(input,  &output[0]);
    ck_assert_str_eq(outValue, "I");

    input = 2;
    outValue = intToRomNum(input,  &output[0]);
    ck_assert_str_eq(outValue, "II");

    input = 3;
    outValue = intToRomNum(input,  &output[0]);
    ck_assert_str_eq(outValue, "III");

    input = 4;
    outValue = intToRomNum(input,  &output[0]);
    ck_assert_str_eq(outValue, "IV");

    input = 5;
    outValue = intToRomNum(input,  &output[0]);
    ck_assert_str_eq(outValue, "V");

    input = 6;
    outValue = intToRomNum(input,  &output[0]);
    ck_assert_str_eq(outValue, "VI");

    input = 7;
    outValue = intToRomNum(input,  &output[0]);
    ck_assert_str_eq(outValue, "VII");

    input = 8;
    outValue = intToRomNum(input,  &output[0]);
    ck_assert_str_eq(outValue, "VIII");

    input = 9;
    outValue = intToRomNum(input,  &output[0]);
    ck_assert_str_eq(outValue, "IX");

    input = 10;
    outValue = intToRomNum(input,  &output[0]);
    ck_assert_str_eq(outValue, "X");

    input = 11;
    outValue = intToRomNum(input,  &output[0]);
    ck_assert_str_eq(outValue, "XI");

    input = 14;
    outValue = intToRomNum(input,  &output[0]);
    ck_assert_str_eq(outValue, "XIV");

    input = 19;
    outValue = intToRomNum(input,  &output[0]);
    ck_assert_str_eq(outValue, "XIX");

    input = 20;
    outValue = intToRomNum(input,  &output[0]);
    ck_assert_str_eq(outValue, "XX");

    input = 21;
    outValue = intToRomNum(input,  &output[0]);
    ck_assert_str_eq(outValue, "XXI");

    input = 25;
    outValue = intToRomNum(input,  &output[0]);
    ck_assert_str_eq(outValue, "XXV");

    input = 29;
    outValue = intToRomNum(input,  &output[0]);
    ck_assert_str_eq(outValue, "XXIX");

    input = 30;
    outValue = intToRomNum(input,  &output[0]);
    ck_assert_str_eq(outValue, "XXX");

    input = 31;
    outValue = intToRomNum(input,  &output[0]);
    ck_assert_str_eq(outValue, "XXXI");

    input = 39;
    outValue = intToRomNum(input,  &output[0]);
    ck_assert_str_eq(outValue, "XXXIX");

    input = 40;
    outValue = intToRomNum(input,  &output[0]);
    ck_assert_str_eq(outValue, "XL");

    input = 41;
    outValue = intToRomNum(input,  &output[0]);
    ck_assert_str_eq(outValue, "XLI");

    input = 49;
    outValue = intToRomNum(input,  &output[0]);
    ck_assert_str_eq(outValue, "XLIX");

    input = 50;
    outValue = intToRomNum(input,  &output[0]);
    ck_assert_str_eq(outValue, "L");

    input = 51;
    outValue = intToRomNum(input,  &output[0]);
    ck_assert_str_eq(outValue, "LI");

    input = 99;
    outValue = intToRomNum(input,  &output[0]);
    ck_assert_str_eq(outValue, "XCIX");

    input = 100;
    outValue = intToRomNum(input,  &output[0]);
    ck_assert_str_eq(outValue, "C");

    input = 101;
    outValue = intToRomNum(input,  &output[0]);
    ck_assert_str_eq(outValue, "CI");
}
END_TEST

START_TEST (testStringValidation)
{
    char output[255];
    char *outValue = NULL;
    memset(output, 0, 255);
    int input = 1332;
    outValue = intToRomNum(input,  &output[0]);
    ck_assert(valString("MCCCXXXII"));
    ck_assert(valString("MCDLXXVIII"));
    ck_assert(valString("MMCMXCIX"));
    ck_assert(valString("I"));
    ck_assert(valString("X"));
}
END_TEST

START_TEST (testAdditionValidation)
{
    char output[255];
    char *outValue = NULL;
    memset(output, 0, 255);
//    int input = 1332;
//    outValue = intToRomNum(input,  &output[0]);
//    ck_assert(valString("MCCCXXXII"));
//    ck_assert(valString("MCDLXXVIII"));
//    ck_assert(valString("MMCMXCIX"));
//    ck_assert(valString("I"));
//    ck_assert(valString("X"));

////addition("XX", "II", output);
////addition("II", "XX", output);
////addition("XIX", "C", output);
//addition("VI", "V", output);
////addition("V", "VI", output);
//addition("IV", "V", output);
////addition("V", "IV", output);
//addition("IV", "IV", output);
//addition("I", "III", output);
//addition("VIII", "VIII", output);
//addition("VIII", "VII", output);
//addition("VII", "VII", output);
//addition("VII", "VII", output);
//addition("III", "I", output);

//addition("CC", "XX", output);
//addition("XX", "CC", output);
//addition("CXC", "M", output);
addition("LX", "L", output);
//addition("L", "LX", output);
addition("XL", "L", output);
//addition("L", "XL", output);
addition("XL", "XL", output);
addition("X", "XXX", output);
addition("LXXX", "LXXX", output);
addition("LXXX", "LXX", output);
addition("LXX", "LXX", output);

//addition("MM", "CC", output);
//addition("CC", "MM", output);
//addition("MCM", "M", output);
addition("DC", "D", output);
//addition("D", "DC", output);
addition("CD", "D", output);
//addition("D", "CD", output);
addition("CD", "CD", output);
addition("C", "CCC", output);
addition("DCCC", "DCCC", output);
addition("DCCC", "DCC", output);
addition("DCC", "DCC", output);

//addition("XX", "MM", output);
//addition("MM", "XX", output);
//addition("MXIX", "C", output);
addition("MVI", "V", output);
//addition("V", "MVI", output);
addition("IMV", "V", output);
//addition("V", "MV", output);
addition("MV", "MV", output);
addition("M", "MMC", output);
addition("MMVIII", "MCVIII", output);
addition("MMVIII", "MMLXXVII", output);
addition("MVII", "MCMLXVII", output);
//    ck_assert(valString("LCCX"));
}
END_TEST

START_TEST (testSubtractionValidation)
{
    char output[255];
    char *outValue = NULL;
    memset(output, 0, 255);
//
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
/*    subtraction("IV", "III", output);
    ck_assert_str_eq(output, "I");
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
    ck_assert_str_eq(output, "XII");*/
//    ck_assert(valString("LCCX"));
}
END_TEST

Suite *romNumTestSuite (void)
{
    Suite *s = suite_create ("RomNumeral");

    /* Core test case */
    TCase *tc_core = tcase_create ("Core");
    tcase_add_test (tc_core, testNumeralCreate);
//    tcase_add_test (tc_core, testFirstCharInNumeralStringIs_I);
//    tcase_add_test (tc_core, testSecondCharInNumeralStringIs_V);
    suite_add_tcase (s, tc_core);

    TCase *tc_parsing = tcase_create ("Parsing");
//    tcase_add_test (tc_parsing, testEachCharInNumeralStringIsClean);
//    tcase_add_test (tc_parsing, testFindDirtyCharInNumeralString);
//    tcase_add_test (tc_parsing, testIsSequenceInNumeralStringValid);
//    tcase_add_test (tc_parsing, testIsSubtractiveSequenceInNumeralStringValid);
//    tcase_add_test (tc_parsing, testIsNumeralStringValid);
    // String output attempt
//    tcase_add_test (tc_parsing, testIntToRomNumStringConversion);
//    tcase_add_test (tc_parsing, testStringValidation);
//    tcase_add_test (tc_parsing, testAdditionValidation);
    tcase_add_test (tc_parsing, testSubtractionValidation);
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

