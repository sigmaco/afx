#ifndef xssMath_h
#define xssMath_h

#include <math.h>
#include <stdint.h>

// A union to let us reinterpret a double as raw bits and back.
typedef union
{
  uint64_t bits64;
  uint32_t bits32[2];
  double num;
} LunaDoubleBits;

#define LUNA_DOUBLE_QNAN_POS_MIN_BITS (UINT64_C(0x7FF8000000000000))
#define LUNA_DOUBLE_QNAN_POS_MAX_BITS (UINT64_C(0x7FFFFFFFFFFFFFFF))

#define LUNA_DOUBLE_NAN (xssDoubleFromBits(LUNA_DOUBLE_QNAN_POS_MIN_BITS))

static inline double xssDoubleFromBits(uint64_t bits)
{
  LunaDoubleBits data;
  data.bits64 = bits;
  return data.num;
}

static inline uint64_t xssDoubleToBits(double num)
{
  LunaDoubleBits data;
  data.num = num;
  return data.bits64;
}

#endif
