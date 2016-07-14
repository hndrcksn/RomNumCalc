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
    char output[32];
    char *outValue = NULL;
    memset(output, 0, 32);
//    int input = 1332;
//    outValue = intToRomNum(input,  &output[0]);

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
//memset(output, 0, 32);
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
//    ck_assert(valString("LCCX"));
}
END_TEST

START_TEST (testSubtractionValidation)
{
    char output[32];
    char *outValue = NULL;
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
    ck_assert_str_eq(output, "CM");//
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
//    ck_assert(valString("LCCX"));
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
    // String output attempt
    tcase_add_test (tc_parsing, testIntToRomNumStringConversion);
    tcase_add_test (tc_parsing, testStringValidation);
    suite_add_tcase (s, tc_parsing);

    TCase *tc_calculation = tcase_create ("Calculation");
    // Addition and Subtraction tests
    tcase_add_test (tc_calculation, testAdditionValidation);
    tcase_add_test (tc_calculation, testSubtractionValidation);
    suite_add_tcase (s, tc_calculation);

    TCase *tc_comparison = tcase_create ("Comparison");
    // Compare number string values
    tcase_add_test (tc_comparison, testComparison);
    suite_add_tcase (s, tc_comparison);

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

