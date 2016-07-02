#ifndef ROMNUMMATH_H
#define ROMNUMMATH_H

typedef struct RomNumeral RomNumeral;

RomNumeral *numeralCreate(int romValue, char *romString);
int numeralValue(RomNumeral *rN);
char *numeralString(RomNumeral *rN);
void numeralDestroy(RomNumeral *rN);

/*
#include<stdio.h>

extern unsigned int add(unsigned int a, unsigned int b);
*/
#endif /* ROMNUMMATH_H */
