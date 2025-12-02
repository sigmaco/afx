/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <stdlib.h>
#include <string.h>

#include "../exec/afxSystemDDK.h"

afxChar const* errorMsg[] =
{
    "Success",
    "Unknown",
    "Nil",
    "Zero",
    "Empty",
    "Not found",
    "Out of range",
    "Invalid",
    "Not enought",
    "Denied",
    "Violation",
    "Incompatible",
    "Corrupted",
    "Unsupported",
    "Uninitialized"
};

_AFX afxUnit32 AfxPowerOfTwo32(afxUnit32 v)
{
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
}

_AFX afxUnit AfxFlagsFindLsb(afxFlags mask)
{
    unsigned long where;
    // Search from LSB to MSB for first set bit.
    // Returns zero if no set bit is found.
    if (_BitScanForward(&where, mask))
        return (afxUnit)(where);
    return 32;
}

_AFX afxUnit AfxFlagsFindMsb(afxFlags mask)
{
    unsigned long where;
    // BitScanReverse scans from MSB to LSB for first set bit.
    // Returns 0 if no set bit is found.
    if (_BitScanReverse(&where, mask))
        return (afxUnit)(31 - where);
    return 32; // Undefined Behavior.
}

_AFXINL afxInt    AfxMinii(afxInt a, afxInt b) { return a < b ? a : b; }
_AFXINL afxInt    AfxMaxii(afxInt a, afxInt b) { return a > b ? a : b; }

_AFXINL afxReal      AfxMiniiReal(afxReal a, afxReal b) { return a < b ? a : b; }
_AFXINL afxReal      AfxMaxiiReal(afxReal a, afxReal b) { return a > b ? a : b; }

_AFXINL afxInt    AfxRandom(void) { return rand(); }
_AFXINL afxInt    AfxRandom2(afxInt mini, afxInt maxi) { return (AfxRandom() % (maxi - mini + (afxInt)1)) + mini; }

_AFXINL afxReal      AfxRandomReal(void) { return (afxReal)AfxRandom(); }
_AFXINL afxReal      AfxRandomReal2(afxReal mini, afxReal maxi) { return (afxReal)AfxRandom2((afxInt)mini, (afxInt)maxi); }

/* Standard CRC32 checksum: fast public domain implementation for
 * little-endian architectures.  Written for compilation with an
 * optimizer set to perform loop unwinding.  Outputs the checksum for
 * each file given as a command line argument.  Invalid file names and
 * files that cause errors are silently skipped.  The program reads
 * from stdin if it is called with no arguments. */

_AFXINL uint32_t crc32_for_byte(uint32_t r)
{
    for (int j = 0; j < 8; ++j)
        r = (r & 1 ? 0 : (uint32_t)0xEDB88320L) ^ r >> 1;

    return r ^ (uint32_t)0xFF000000L;
}

/* Any unsigned integer type with at least 32 bits may be used as
 * accumulator type for fast crc32-calulation, but unsigned long is
 * probably the optimal choice for most systems. */
typedef unsigned long accum_t;

_AFX void init_tables(uint32_t* table, uint32_t* wtable)
{
    for (size_t i = 0; i < 0x100; ++i)
        table[i] = crc32_for_byte(i);

    for (size_t k = 0; k < sizeof(accum_t); ++k)
    {
        for (size_t w, i = 0; i < 0x100; ++i)
        {
            for (size_t j = w = 0; j < sizeof(accum_t); ++j)
                w = table[(uint8_t)(j == k ? w ^ i : w)] ^ w >> 8;

            wtable[(k << 8) + i] = w ^ (k ? wtable[0] : 0);
        }
    }
}

_AFX void AfxAccumulateCrc32(afxUnit32 *crc, void const* data, afxSize len)
{
    static uint32_t table[0x100], wtable[0x100 * sizeof(accum_t)];
    size_t n_accum = len / sizeof(accum_t);

    if (!*table)
        init_tables(table, wtable);

    for (size_t i = 0; i < n_accum; ++i)
    {
        accum_t a = *crc ^ ((accum_t*)data)[i];

        for (size_t j = *crc = 0; j < sizeof(accum_t); ++j)
            *crc ^= wtable[(j << 8) + (uint8_t)(a >> 8 * j)];
    }

    for (size_t i = n_accum * sizeof(accum_t); i < len; ++i)
        *crc = table[(uint8_t)*crc ^ ((uint8_t*)data)[i]] ^ *crc >> 8;
}

_AFXINL afxFixed AfxFixedFromFloat(afxReal f)
{
#if !0 // RW way
    return (afxFixed)(f * 65536.0f);
#else // DX way
    return (afxFixed)((afxUnit16)((((afxUnit32)f) >> 16) & 0xffff) << 16) | (afxUnit16)(((afxUnit32)f) & 0xffff);
#endif
}

_AFXINL afxReal AfxRealFromFixed(afxFixed f)
{
#if !0 // RW way
    return ((afxReal)(((afxReal)(f)) * (1.0f / 65536.0f)));
#else // DXVA way
    return (afxReal)((((afxUnit32)f) >> 16) & 0xffff) + (afxReal)(((afxUnit32)f) & 0xffff) / 0x10000;
#endif
}

_AFXINL afxFixed AfxFixedFromInt(afxInt i)
{
    return (afxFixed)((i) << 16);
}

_AFXINL afxInt AfxIntFromFixed(afxFixed f)
{
    return ((f) >> 16);
}
