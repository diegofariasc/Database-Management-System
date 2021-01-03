#ifndef __TYPES_H__
#define __TYPES_H__

#include <limits.h>

// ------------------------------------------------------------
// FIXED LENGTH TYPES
// ------------------------------------------------------------
#define MINITINT        0x00
#define SHORTINT        0x01
#define INTEGER         0x02
#define LARGEINT        0x03
#define UMINITINT       0x04
#define USHORTINT       0x05
#define UINTEGER        0x06
#define ULARGEINT       0x07
#define FLOAT           0x08
#define DOUBLE          0x09
#define CHAR            0x0A
#define BOOLEAN         0x0B

// ------------------------------------------------------------
// USER-DEFINED LENGTH TYPES
// ------------------------------------------------------------
#define SET             0x10
#define STRING          0x11

// ------------------------------------------------------------
// INTERNAL TYPES AND VALUES
// ------------------------------------------------------------
typedef unsigned long long disk_pointer;
#define NULL_DISK_POINTER ULLONG_MAX

// Definition of variable sizes
const unsigned int VARSIZES[] = {1, 2, 4, 8, 1, 2, 4, 8, 4, 8, 1, 1};

// ------------------------------------------------------------
// VARIABLES USED IN METHODS FOR INTERPRETING TUPLES
// ------------------------------------------------------------
# define DECLARE_INTERPRETATION_VARS(NAME)      \
char                    miniintVar_##NAME;      \
short                   shortintVar_##NAME;     \
int                     integerVar_##NAME;      \
long long int           largeintVar_##NAME;     \
unsigned char           uminiintVar_##NAME;     \
unsigned short          ushortintVar_##NAME;    \
unsigned int            uintegerVar_##NAME;     \
unsigned long long int  ulargeintVar_##NAME;    \
float                   floatVar_##NAME;        \
double                  doubleVar_##NAME;       \
char                    charVar_##NAME;         \
bool                    booleanVar_##NAME;

#endif