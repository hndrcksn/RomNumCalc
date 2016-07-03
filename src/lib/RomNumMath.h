#ifndef ROMNUMMATH_H
#define ROMNUMMATH_H

#include <stdbool.h>

typedef struct RomNumeral RomNumeral;

RomNumeral *numeralCreate(int numValue, char *numString);
int numeralValue(RomNumeral *rN);
char *numeralString(RomNumeral *rN);
void numeralDestroy(RomNumeral *rN);
bool char0Is_I(RomNumeral *rN);

#endif /* ROMNUMMATH_H */
