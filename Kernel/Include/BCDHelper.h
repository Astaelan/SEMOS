#ifndef _BCDHELPER_H
#define _BCDHELPER_H

extern "C" {
#include "types.h"
}

static inline BYTE BCDHelper_FromBCD(BYTE value)
{ return (BYTE)(((value >> 4) & 0x0F) * 10 + (value & 0x0F)); }

#endif