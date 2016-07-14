#ifndef ROMNUMMATH_H
#define ROMNUMMATH_H

#include <stdbool.h>

typedef enum {ONES = 0, TENS, HUNS, THOU, NUM_ORDERS} OrderType;
typedef struct RomNumeral RomNumeral;
typedef struct StrHolder StrHolder;
static const int BUFRSIZE = 32;

RomNumeral *numeralCreate(int numValue, char *numString);
void numeralReplace(RomNumeral *rN, int newValue, char *newStrPtr);
int numeralValue(RomNumeral *rN);
char *numeralString(RomNumeral *rN);
void numeralDestroy(RomNumeral *rN);
bool char0Is_I(RomNumeral *rN);
bool char1Is_V(RomNumeral *rN);
bool numeralStringIsClean(RomNumeral *rN);
bool stringIsClean(const char *s);
int sequenceInRomNumeralIsValid(RomNumeral *rN);
int sequenceInStringIsValid(const char *s);
int subtractiveSequenceInRomNumeralIsValid(RomNumeral *rN);
int subtractiveSequenceInStringIsValid(const char *s);
bool isValid(RomNumeral *rN);
bool isValidString(const char *s);
int romNumToInt(const char *s);
char *intToRomNum(int num, char *s);
RomNumeral romNumAddition(RomNumeral *rN1, RomNumeral *rN2);
RomNumeral romNumSubtraction(RomNumeral *rN1, RomNumeral *rN2);

// String functions
bool valString (const char *s);
char *addition (const char *as, const char *bs, char *cs);
char *subtraction (const char *as, const char *bs, char *cs);
void attachHolder(const char *s, StrHolder *sh);
bool isStringSubtractive(const char *s);
bool addOrder(StrHolder *aH, StrHolder *bH, char *cStr, OrderType order, bool carriedOver);
bool subOrder(StrHolder *aH, StrHolder *bH, char *cStr, OrderType order, bool carriedOver);
int romStrCmpSH(StrHolder *aH, StrHolder *bH, OrderType order);
int romStrCmp(const char *as, const char *bs);
int romNumRelVal(const char *str, int length, OrderType order);

#endif /* ROMNUMMATH_H */
