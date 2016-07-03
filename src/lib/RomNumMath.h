#ifndef ROMNUMMATH_H
#define ROMNUMMATH_H

#include <stdbool.h>

typedef struct RomNumeral RomNumeral;

RomNumeral *numeralCreate(int numValue, char *numString);
int numeralValue(RomNumeral *rN);
char *numeralString(RomNumeral *rN);
void numeralDestroy(RomNumeral *rN);
bool char0Is_I(RomNumeral *rN);
bool char1Is_V(RomNumeral *rN);
bool numeralStringIsClean(RomNumeral *rN);
bool sequenceInRomNumeralIsValid(RomNumeral *rN);
bool sequenceInStringIsValid(const char *s);
bool subtractiveSequenceInRomNumeralIsValid(RomNumeral *rN);
bool subtractiveSequenceInStringIsValid(const char *s);
bool isValid(RomNumeral *rN);
bool isValidString(const char *s);
int parseToInt(const char *s);

#endif /* ROMNUMMATH_H */
