#ifndef ROMNUMMATH_H
#define ROMNUMMATH_H

#include <stdbool.h>

typedef struct RomNumeral RomNumeral;

RomNumeral *numeralCreate(int numValue, char *numString);
void numeralReplace(RomNumeral *rN, int newValue, char *newStrPtr);
int numeralValue(RomNumeral *rN);
char *numeralString(RomNumeral *rN);
void numeralDestroy(RomNumeral *rN);
bool char0Is_I(RomNumeral *rN);
bool char1Is_V(RomNumeral *rN);
bool numeralStringIsClean(RomNumeral *rN);
int sequenceInRomNumeralIsValid(RomNumeral *rN);
int sequenceInStringIsValid(const char *s);
int subtractiveSequenceInRomNumeralIsValid(RomNumeral *rN);
int subtractiveSequenceInStringIsValid(const char *s);
bool isValid(RomNumeral *rN);
bool isValidString(const char *s);
int parseToInt(const char *s);

#endif /* ROMNUMMATH_H */
