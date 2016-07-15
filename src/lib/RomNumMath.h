#ifndef ROMNUMMATH_H
#define ROMNUMMATH_H

#include <stdbool.h>

// Enums, structs and consts.
typedef enum {ONES = 0, TENS, HUNS, THOU, NUM_ORDERS} OrderType;
typedef struct StrHolder StrHolder;
static const int BUFRSIZE = 32;

// String functions
char *intToRomNum(int num, char *s);
bool isCleanValidString (const char *s);
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
