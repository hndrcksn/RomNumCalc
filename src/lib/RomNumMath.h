#ifndef ROMNUMMATH_H
#define ROMNUMMATH_H

#include <stdbool.h>
#include <stdarg.h>

// Enums, structs and consts.
typedef enum {ONES = 0, TENS, HUNS, THOU, NUM_ORDERS} OrderType;
typedef enum {X1 = 0, X5, X10, NUM_BASES} BaseType;
typedef enum {P1 = 0, P5, NUM_PTRS} PtrType;
typedef struct BaseCounter BaseCounter;
typedef struct StrHolder StrHolder;
static const int BUFRSIZE = 32;

// Represents bad input strings
const int BAD_NUMERAL_A = -999;
const int BAD_NUMERAL_B = -998;

// External global
extern bool global_debugging;

// Debug printf
void debug_printf(const char* format, ...);

// String functions
bool isCleanValidString (const char *s);
char *addition (const char *as, const char *bs, char *cs);
char *subtraction (const char *as, const char *bs, char *cs);
void attachHolder(const char *s, StrHolder *sh, bool isNegative);
bool isStringSubtractive(const char *s);
bool addOrder(StrHolder *aH, StrHolder *bH, char *cStr, OrderType order, bool carriedOver);
bool subOrder(StrHolder *aH, StrHolder *bH, char *cStr, OrderType order, bool carriedOver);
int romStrCmpSH(StrHolder *aH, StrHolder *bH, OrderType order);
int romStrCmp(const char *as, const char *bs);
int romNumRelVal(const char *str, int length, OrderType order);
char *prependStr(char *str, char c);
void tallyChar(StrHolder *sH, OrderType order, BaseCounter *bC);
void handleSubtractives(BaseCounter *bC);
bool convertFromSubtractive(BaseCounter *bC);
void convertToSubtractive(BaseCounter *bC);
void adjustX1Tally(BaseCounter *bC);
void adjustX5Tally(BaseCounter *bC);
void postProcAddOrder(OrderType order, const BaseCounter *bC, bool *outCarry, char *outStr);
void postProcSubOrder(OrderType order, const BaseCounter *bC, char *outStr);
bool reconcileNegatives(BaseCounter *bC);
void attachP1Ptrs(OrderType order, StrHolder *sH);
void attachP5Ptrs(OrderType order, StrHolder *sH);

#endif /* ROMNUMMATH_H */
