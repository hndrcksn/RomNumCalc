#ifndef ROMNUMMATH_H
#define ROMNUMMATH_H

typedef struct RomNumeral RomNumeral;

RomNumeral *numeralCreate(int numValue, char *numString);
int numeralValue(RomNumeral *rN);
char *numeralString(RomNumeral *rN);
void numeralDestroy(RomNumeral *rN);

#endif /* ROMNUMMATH_H */
