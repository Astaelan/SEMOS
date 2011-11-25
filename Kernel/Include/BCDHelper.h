#pragma once

static inline uint8_t BCDHelper_FromBCD(uint8_t value)
{ return (((value >> 4) & 0x0F) * 10 + (value & 0x0F)); }
