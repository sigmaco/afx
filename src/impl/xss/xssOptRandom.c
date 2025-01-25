#include "xssOptRandom.h"

#if LUNA_OPT_RANDOM

#include <string.h>
#include <time.h>

#include "./xss.h"
#include "xssVm.h"

#include "xssOptRandom.xss.inc"

// Implements the well equidistributed long-period linear PRNG (WELL512a).
//
// https://en.wikipedia.org/wiki/Well_equidistributed_long-period_linear
typedef struct
{
  uint32_t state[16];
  uint32_t index;
} Well512;

// Code from: http://www.lomont.org/Math/Papers/2008/Lomont_PRNG_2008.pdf
static uint32_t advanceState(Well512* well)
{
  uint32_t a, b, c, d;
  a = well->state[well->index];
  c = well->state[(well->index + 13) & 15];
  b =  a ^ c ^ (a << 16) ^ (c << 15);
  c = well->state[(well->index + 9) & 15];
  c ^= (c >> 11);
  a = well->state[well->index] = b ^ c;
  d = a ^ ((a << 5) & 0xda442d24U);

  well->index = (well->index + 15) & 15;
  a = well->state[well->index];
  well->state[well->index] = a ^ b ^ d ^ (a << 2) ^ (b << 18) ^ (c << 28);
  return well->state[well->index];
}

static void randomAllocate(xssVm env)
{
  Well512* well = (Well512*)xssSetSlotNewForeign(env, 0, 0, sizeof(Well512));
  well->index = 0;
}

static void randomSeed0(xssVm env)
{
  Well512* well = (Well512*)xssGetSlotForeign(env, 0);

  srand((uint32_t)time(NULL));
  for (int i = 0; i < 16; i++)
  {
    well->state[i] = rand();
  }
}

static void randomSeed1(xssVm env)
{
  Well512* well = (Well512*)xssGetSlotForeign(env, 0);

  srand((uint32_t)xssGetSlotDouble(env, 1));
  for (int i = 0; i < 16; i++)
  {
    well->state[i] = rand();
  }
}

static void randomSeed16(xssVm env)
{
  Well512* well = (Well512*)xssGetSlotForeign(env, 0);

  for (int i = 0; i < 16; i++)
  {
    well->state[i] = (uint32_t)xssGetSlotDouble(env, i + 1);
  }
}

static void randomFloat(xssVm env)
{
  Well512* well = (Well512*)xssGetSlotForeign(env, 0);

  // A double has 53 bits of precision in its mantissa, and we'd like to take
  // full advantage of that, so we need 53 bits of random source data.

  // First, start with 32 random bits, shifted to the left 21 bits.
  double result = (double)advanceState(well) * (1 << 21);

  // Then add another 21 random bits.
  result += (double)(advanceState(well) & ((1 << 21) - 1));

  // Now we have a number from 0 - (2^53). Divide be the range to get a double
  // from 0 to 1.0 (half-inclusive).
  result /= 9007199254740992.0;

  xssSetSlotDouble(env, 0, result);
}

static void randomInt0(xssVm env)
{
  Well512* well = (Well512*)xssGetSlotForeign(env, 0);

  xssSetSlotDouble(env, 0, (double)advanceState(well));
}

const char* xssRandomSource()
{
  return randomModuleSource;
}

LunaForeignClassMethods xssRandomBindForeignClass(xssVm env,
                                                   const char* module,
                                                   const char* className)
{
  ASSERT(strcmp(className, "Random") == 0, "Should be in Random class.");
  LunaForeignClassMethods methods;
  methods.allocate = randomAllocate;
  methods.finalize = NULL;
  return methods;
}

LunaForeignMethodFn xssRandomBindForeignMethod(xssVm env,
                                                const char* className,
                                                bool isStatic,
                                                const char* signature)
{
  ASSERT(strcmp(className, "Random") == 0, "Should be in Random class.");
  
  if (strcmp(signature, "<allocate>") == 0) return randomAllocate;
  if (strcmp(signature, "seed_()") == 0) return randomSeed0;
  if (strcmp(signature, "seed_(_)") == 0) return randomSeed1;
  
  if (strcmp(signature, "seed_(_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_)") == 0)
  {
    return randomSeed16;
  }
  
  if (strcmp(signature, "float()") == 0) return randomFloat;
  if (strcmp(signature, "int()") == 0) return randomInt0;
  
  ASSERT(false, "Unknown method.");
  return NULL;
}

#endif
