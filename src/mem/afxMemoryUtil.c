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

//#define RPMALLOC_ALLOC 1

//#undef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#ifdef RPMALLOC_ALLOC
#include "../../dep/rpmalloc/rpmalloc.h"
#endif
#include <stdio.h>
#include <string.h>
#include <crtdbg.h>

#define _AFX_CORE_C
#define _AFX_MMU_C
#include "../exec/afxSystemDDK.h"

#ifdef _AFX_DEBUG
#   define _AFX_TEST_COPY_RANGE
#   define _AFX_TEST_COPY_BOUNDS
#define _AFX_TEST_COPY_RESULTS
#   define _AFX_TEST_ZERO_BOUNDS
#define _AFX_TEST_ZERO_RESULTS
#   define _AFX_TEST_FILL_BOUNDS
#define _AFX_TEST_FILL_RESULTS
#   define _AFX_TEST_MOVE_BOUNDS
#define _AFX_TEST_MOVE_RESULTS
#   define _AFX_TEST_FIND_BOUNDS
#define _AFX_TEST_FIND_RESULTS
#   define _AFX_TEST_COMPARE_BOUNDS
#define _AFX_TEST_COMPARE_RESULTS
#endif

#if 0
#ifdef AFX_ON_X86
#   define _AFX_COPY_WITH_SSE
#   define _AFX_ZERO_WITH_SSE
#   define _AFX_FILL_WITH_SSE
#   define _AFX_MOVE_WITH_SSE
#   define _AFX_FIND_WITH_SSE
#   define _AFX_COMPARE_WITH_SSE
#endif
#endif

#if !0
#define _AFX_DONT_REIMPLEMENT_MEMSET
#define _AFX_DONT_REIMPLEMENT_MEMCPY
#define _AFX_DONT_REIMPLEMENT_MEMMOVE
#define _AFX_DONT_IMPLEMENT_MEMCMP
#define _AFX_DONT_REIMPLEMENT_MEMCHR
#endif
#define _AFX_USE_MEMCPY_FAST_PATH TRUE
#define _AFX_USE_MEMSET_FAST_PATH TRUE
#define _AFX_USE_MEMMOVE_FAST_PATH TRUE
#define _AFX_USE_MEMCMP_FAST_PATH TRUE
#define _AFX_USE_MEMCHR_FAST_PATH TRUE


#ifdef AFX_ON_X86
_AFXINL void* AfxFillSSE(void* start, afxSize range, void const* pattern, afxSize patternSiz)
/*
    AfxFillSSE()
    Accepts any pattern size.
    Uses SIMD for all 16-byte aligned blocks.
    Handles unaligned tail bytes properly.
    Works for small or large memory regions.
*/
{
    if (patternSiz == 0 || range == 0)
        return start;

    afxByte* d = (afxByte*)start;
#if 0
    // Single byte pattern, use memset.
    if (patternSiz == 1)
    {
        return memset(dst, *(afxByte const*)pattern, range);
    }
#endif
    // Replicate pattern into a 16-byte fill block.
    afxByte fill_block[AFX_SIMD_ALIGNMENT];
    for (afxSize i = 0; i < AFX_SIMD_ALIGNMENT; ++i)
    {
        fill_block[i] = ((afxByte const*)pattern)[i % patternSiz];
    }

    __m128i fill_regs[AFX_MAX_SIMD_REGISTERS];
    __m128i fill = _mm_loadu_si128((__m128i *)fill_block);

    // Determine how many registers to use.
    // Only load as many SIMD registers as you'll use. Keeps cache clean, no wasted memory traffic.
    // Dynamically adapts to fill size and architecture. Uses full register set for large fills.
    afxSize regs_used = range / AFX_SIMD_ALIGNMENT;
    if (regs_used > AFX_MAX_SIMD_REGISTERS)
        regs_used = AFX_MAX_SIMD_REGISTERS;

    // Load only the number of registers we need
    for (afxSize i = 0; i < regs_used; ++i)
    {
        fill_regs[i] = fill;
    }

    // Align to 16-byte boundary
    while (((uintptr_t)d & 0xF) && range > 0)
    {
        *d++ = ((afxByte const*)pattern)[0];
        range--;
    }

    if (range >= AFX_SIMD_ALIGNMENT)
    {
        // Bulk fill using available registers

        afxBool aligned = (((uintptr_t)d & 0xF) == 0);
        afxSize block_size = regs_used * AFX_SIMD_ALIGNMENT;

        while (range >= block_size)
        {
            for (afxSize i = 0; i < regs_used; ++i)
            {
                if (aligned)
                    _mm_store_si128((__m128i *)(d + i * AFX_SIMD_ALIGNMENT), fill_regs[i]);       // aligned store
                else
                    _mm_storeu_si128((__m128i *)(d + i * AFX_SIMD_ALIGNMENT), fill_regs[i]);      // unaligned store
            }
            d += block_size;
            range -= block_size;
        }


        // Remaining 16-byte blocks
        while (range >= AFX_SIMD_ALIGNMENT)
        {
            _mm_storeu_si128((__m128i *)d, fill);
            d += AFX_SIMD_ALIGNMENT;
            range -= AFX_SIMD_ALIGNMENT;
        }
    }

    // Tail fill. Word-wise.
    uintptr_t tail = range;
    afxByte const* pat = pattern;

    afxUnit64 word64 = 0;
    while (tail >= sizeof(afxUnit64))
    {
        for (afxSize i = 0; i < sizeof(afxUnit64); ++i)
            word64 |= ((afxUnit64)pat[i % patternSiz]) << (i * 8);
        *(afxUnit64*)d = word64;
        d += sizeof(afxUnit64);
        tail -= sizeof(afxUnit64);
    }

    afxUnit32 word32 = 0;
    while (tail >= sizeof(afxUnit32))
    {
        for (afxSize i = 0; i < sizeof(afxUnit32); ++i)
            word32 |= ((uint32_t)pat[i % patternSiz]) << (i * 8);
        *(afxUnit32*)d = word32;
        d += sizeof(afxUnit32);
        tail -= sizeof(afxUnit32);
    }

    while (tail >= sizeof(afxUnit16))
    {
        afxUnit16 word = pat[0];
        if (patternSiz > 1)
            word |= ((afxUnit16)pat[1]) << 8;
        *(afxUnit16*)d = word;
        d += sizeof(afxUnit16);
        tail -= sizeof(afxUnit16);
    }

    if (tail) *d = pat[0];

    return start;
}
#endif//AFX_ON_X86

_AFXINL void AfxFillPattern(void* start, afxSize range, void const* pattern, afxSize patternSiz)
{
    afxError err = { 0 };

    if (!range) return;
    AFX_ASSERT((pattern && patternSiz) || !(pattern || patternSiz));
    AFX_ASSERT(start);

#ifdef _AFX_TEST_FILL_BOUNDS
    ((afxByte*)start)[range - 1];
#endif
#ifdef _AFX_FILL_WITH_SSE
    AfxFillSSE(start, range, pattern, patternSiz);
#ifdef _AFX_TEST_FILL_RESULTS
    afxByte* d = (afxByte*)start;
    afxByte const* s = (afxByte const*)pattern;
    for (afxSize i = 0; i < range; ++i)
        AFX_ASSERT(d[i] == s[i % patternSiz]);
#endif
#else
    afxByte* d = (afxByte*)start;
    afxByte const* s = (afxByte const*)pattern;
    // Fill the entire region with repeating pattern, byte by byte
    for (afxSize i = 0; i < range; ++i)
        d[i] = s[i % patternSiz];
#endif
}

_AFX void AfxFill(void* start, afxInt value, afxSize range)
{
    AfxFillPattern(start, range, (afxInt[]) { 0 }, sizeof(afxInt));
}

#ifdef AFX_ON_X86
_AFXINL void AfxZeroSSE(void* start, afxSize range)
{
    afxByte* p = (afxByte*)start;

    // 16-byte zero vector.
    __m128i zero = _mm_setzero_si128();

    // Process in 16-byte blocks (using SSE2).
    while (range >= AFX_SIMD_ALIGNMENT)
    {
        // Use aligned store if the memory is 16-byte aligned.
        _mm_storeu_si128((__m128i *)p, zero); // Always use _mm_storeu_si128, which handles both aligned and unaligned memory.
        p += AFX_SIMD_ALIGNMENT;
        range -= AFX_SIMD_ALIGNMENT;
    }

    // Handle any remaining bytes (less than 16 bytes).
    for (afxSize i = 0; i < range; ++i)
        p[i] = 0;
}
#endif//AFX_ON_X86

_AFX void AfxZero(void* start, afxSize range)
{
    afxError err = { 0 };

    if (!range) return;
    AFX_ASSERT(start);
#ifdef _AFX_TEST_ZERO_BOUNDS
    ((afxByte*)start)[range - 1] = 0;
#endif
#ifdef _AFX_ZERO_WITH_SSE
    AfxZeroSSE(start, range);
#ifdef _AFX_TEST_ZERO_RESULTS
    for (afxByte* p = start; range--; AFX_ASSERT(*p == 0), p++);
#endif
#else
    for (afxByte* p = start; range--; *p++ = 0);
#endif
}

#ifdef AFX_ON_X86
_AFXINL void AfxCopySSE(void* dst, void const* src, afxSize size)
{
    if (size == 0 || dst == src) return;

    afxByte*d = (afxByte*)dst;
    afxByte const* s = (afxByte const*)src;

    // Small copy fast path (no SIMD).
    if (size < 64)
    {
        while (size >= sizeof(afxUnit64))
        {
            *(afxUnit64*)d = *(afxUnit64 const*)s;
            d += sizeof(afxUnit64);
            s += sizeof(afxUnit64);
            size -= sizeof(afxUnit64);
        }
        while (size >= sizeof(afxUnit32))
        {
            *(afxUnit32*)d = *(afxUnit32 const*)s;
            d += sizeof(afxUnit32);
            s += sizeof(afxUnit32);
            size -= sizeof(afxUnit32);
        }
        while (size >= sizeof(afxUnit16))
        {
            *(afxUnit16*)d = *(afxUnit16 const*)s;
            d += sizeof(afxUnit16);
            s += sizeof(afxUnit16);
            size -= sizeof(afxUnit16);
        }
        if (size) *d = *s;
        return;
    }

    // Choose aligned or unaligned SIMD copy based on both pointers
    afxBool use_aligned = !((uintptr_t)d & 0xF) && !((uintptr_t)s & 0xF);

    // SIMD loop with dynamic unroll
    afxSize regs_used = size / AFX_SIMD_ALIGNMENT;
    if (regs_used > AFX_MAX_SIMD_REGISTERS)
        regs_used = AFX_MAX_SIMD_REGISTERS;

    afxSize block_size = regs_used * AFX_SIMD_ALIGNMENT;
    while (size >= block_size)
    {
        for (afxSize i = 0; i < regs_used; ++i)
        {
            __m128i temp;
            if (use_aligned)
            {
                temp = _mm_load_si128((__m128i *)(s + i * AFX_SIMD_ALIGNMENT));
                _mm_store_si128((__m128i *)(d + i * AFX_SIMD_ALIGNMENT), temp);
            }
            else
            {
                temp = _mm_loadu_si128((__m128i *)(s + i * AFX_SIMD_ALIGNMENT));
                _mm_storeu_si128((__m128i *)(d + i * AFX_SIMD_ALIGNMENT), temp);
            }
        }
        d += block_size;
        s += block_size;
        size -= block_size;
    }

    // Remaining 16-byte chunks.
    while (size >= AFX_SIMD_ALIGNMENT)
    {
        __m128i temp;
        if (use_aligned)
        {
            temp = _mm_load_si128((__m128i *)s);
            _mm_store_si128((__m128i *)d, temp);
        }
        else
        {
            temp = _mm_loadu_si128((__m128i *)s);
            _mm_storeu_si128((__m128i *)d, temp);
        }
        d += AFX_SIMD_ALIGNMENT;
        s += AFX_SIMD_ALIGNMENT;
        size -= AFX_SIMD_ALIGNMENT;
    }

    // Tail copy with word-sized stores
    while (size >= sizeof(afxUnit64))
    {
        *(afxUnit64*)d = *(afxUnit64 const*)s;
        d += sizeof(afxUnit64);
        s += sizeof(afxUnit64);
        size -= sizeof(afxUnit64);
    }
    while (size >= sizeof(afxUnit32))
    {
        *(afxUnit32*)d = *(afxUnit32 const*)s;
        d += sizeof(afxUnit32);
        s += sizeof(afxUnit32);
        size -= sizeof(afxUnit32);
    }
    while (size >= sizeof(afxUnit16))
    {
        *(afxUnit16*)d = *(afxUnit16 const*)s;
        d += sizeof(afxUnit16);
        s += sizeof(afxUnit16);
        size -= sizeof(afxUnit16);
    }
    if (size) *d = *s;

    return;
}
#endif//AFX_ON_X86

_AFX void AfxCopy(void* dst, void const* src, afxSize size)
{
    afxError err = { 0 };

    if (!size) return;
    AFX_ASSERT(src);
    AFX_ASSERT(dst);
#ifdef _AFX_TEST_COPY_BOUNDS
    // Test bounds
    *&((afxByte*)dst)[size - 1];
#endif
#ifdef _AFX_COPY_WITH_SSE
    AfxCopySSE(dst, src, size);
#ifdef _AFX_TEST_COPY_RESULTS
    afxByte* d = dst;
    afxByte const* s = src;
    while (size--)
        AFX_ASSERT(*d++ == *s++);
#endif
#else
    afxByte* d = dst;
    afxByte const* s = src;
    while (size--) *d++ = *s++;
#endif
}

#ifdef AFX_ON_X86
_AFXINL void AfxMoveSSE(void* dst, void const* src, afxSize size)
{
    if (size == 0 || dst == src)
        return;

    afxByte* d = (afxByte*)dst;
    afxByte const* s = (afxByte const*)src;

    // Small copy fast path (no SIMD)
    if (size < 64)
    {
        while (size >= sizeof(afxUnit64))
        {
            *(afxUnit64*)d = *(afxUnit64 const*)s;
            d += sizeof(afxUnit64);
            s += sizeof(afxUnit64);
            size -= sizeof(afxUnit64);
        }
        while (size >= sizeof(afxUnit32))
        {
            *(afxUnit32*)d = *(afxUnit32 const*)s;
            d += sizeof(afxUnit32);
            s += sizeof(afxUnit32);
            size -= sizeof(afxUnit32);
        }
        while (size >= sizeof(afxUnit16))
        {
            *(afxUnit16*)d = *(afxUnit16 const*)s;
            d += sizeof(afxUnit16);
            s += sizeof(afxUnit16);
            size -= sizeof(afxUnit16);
        }
        if (size) *d = *s;
        return;
    }

    // Determine the direction of copying (forward or backward)
    afxBool copy_backward = (d > s) && (d < s + size);  // Overlapping, src > dst

    // SIMD loop with dynamic unroll
    afxSize regs_used = size / AFX_SIMD_ALIGNMENT;
    if (regs_used > AFX_MAX_SIMD_REGISTERS)
        regs_used = AFX_MAX_SIMD_REGISTERS;

    afxSize block_size = regs_used * AFX_SIMD_ALIGNMENT;

    // Copy backward (overlapping region case)
    if (copy_backward)
    {
        s += size - AFX_SIMD_ALIGNMENT;  // Start copying from the end of the source
        d += size - AFX_SIMD_ALIGNMENT;

        while (size >= block_size)
        {
            for (afxSize i = 0; i < regs_used; ++i)
            {
                __m128i temp;
                if (((uintptr_t)s & 0xF) == 0 && ((uintptr_t)d & 0xF) == 0)
                {
                    temp = _mm_load_si128((__m128i *)(s - i * AFX_SIMD_ALIGNMENT));
                    _mm_store_si128((__m128i *)(d - i * AFX_SIMD_ALIGNMENT), temp);
                }
                else
                {
                    temp = _mm_loadu_si128((__m128i *)(s - i * AFX_SIMD_ALIGNMENT));
                    _mm_storeu_si128((__m128i *)(d - i * AFX_SIMD_ALIGNMENT), temp);
                }
            }
            d -= block_size;
            s -= block_size;
            size -= block_size;
        }

        // Handle the remaining bytes (tail)
        while (size >= AFX_SIMD_ALIGNMENT)
        {
            __m128i temp;
            if (((uintptr_t)s & 0xF) == 0 && ((uintptr_t)d & 0xF) == 0)
            {
                temp = _mm_load_si128((__m128i *)s);
                _mm_store_si128((__m128i *)d, temp);
            }
            else
            {
                temp = _mm_loadu_si128((__m128i *)s);
                _mm_storeu_si128((__m128i *)d, temp);
            }
            d -= AFX_SIMD_ALIGNMENT;
            s -= AFX_SIMD_ALIGNMENT;
            size -= AFX_SIMD_ALIGNMENT;
        }
    }
    else
    {
        // Copy forward (no overlap or dst ahead of src)
        while (size >= block_size)
        {
            for (afxSize i = 0; i < regs_used; ++i)
            {
                __m128i temp;
                if (((uintptr_t)s & 0xF) == 0 && ((uintptr_t)d & 0xF) == 0)
                {
                    temp = _mm_load_si128((__m128i *)(s + i * AFX_SIMD_ALIGNMENT));
                    _mm_store_si128((__m128i *)(d + i * AFX_SIMD_ALIGNMENT), temp);
                }
                else
                {
                    temp = _mm_loadu_si128((__m128i *)(s + i * AFX_SIMD_ALIGNMENT));
                    _mm_storeu_si128((__m128i *)(d + i * AFX_SIMD_ALIGNMENT), temp);
                }
            }
            d += block_size;
            s += block_size;
            size -= block_size;
        }

        // Handle the remaining bytes (tail)
        while (size >= AFX_SIMD_ALIGNMENT)
        {
            __m128i temp;
            if (((uintptr_t)s & 0xF) == 0 && ((uintptr_t)d & 0xF) == 0)
            {
                temp = _mm_load_si128((__m128i *)s);
                _mm_store_si128((__m128i *)d, temp);
            }
            else
            {
                temp = _mm_loadu_si128((__m128i *)s);
                _mm_storeu_si128((__m128i *)d, temp);
            }
            d += AFX_SIMD_ALIGNMENT;
            s += AFX_SIMD_ALIGNMENT;
            size -= AFX_SIMD_ALIGNMENT;
        }
    }

    // Tail copy with word-sized stores (for leftover bytes)
    while (size >= sizeof(afxUnit64))
    {
        *(afxUnit64*)d = *(afxUnit64 const*)s;
        d += sizeof(afxUnit64);
        s += sizeof(afxUnit64);
        size -= sizeof(afxUnit64);
    }
    while (size >= sizeof(afxUnit32))
    {
        *(afxUnit32*)d = *(afxUnit32 const*)s;
        d += sizeof(afxUnit32);
        s += sizeof(afxUnit32);
        size -= sizeof(afxUnit32);
    }
    while (size >= sizeof(afxUnit16))
    {
        *(afxUnit16*)d = *(afxUnit16 const*)s;
        d += sizeof(afxUnit16);
        s += sizeof(afxUnit16);
        size -= sizeof(afxUnit16);
    }
    if (size) *d = *s;

    return;
}
#endif//AFX_ON_X86

_AFX void AfxMove(void* dst, void const* src, afxSize size)
{
    afxError err = { 0 };

    if (!size) return;
    AFX_ASSERT(src);
    AFX_ASSERT(dst);
#ifdef _AFX_TEST_MOVE_BOUNDS
    *&((afxByte*)dst)[size - 1];
    *&((afxByte const*)src)[size - 1];
#endif
#ifdef _AFX_MOVE_WITH_SSE
    AfxMoveSSE(dst, src, size);
#ifdef _AFX_TEST_MOVE_RESULTS
    afxByte* d = (afxByte*)dst;
    afxByte const* s = (afxByte const*)src;
    for (afxSize i = 0; i < size; ++i)
        AFX_ASSERT(d[i] == s[i]);
#endif
#else
    // Simple, clean and convex implementation designed firstly to debugging.

    afxByte* d = (afxByte*)dst;
    afxByte const* s = (afxByte const*)src;

    // No overlap or dst before src - copy forward.
    if (d < s || d >= (s + size))
        for (afxSize i = 0; i < size; ++i)
            d[i] = s[i];
    else // Overlap and dst after src - copy backwards.
        for (afxSize i = size; i-- > 0; )
            d[i] = s[i];
#endif
}

#ifdef AFX_ON_X86
_AFX afxInt AfxCompareSSE(void const* a, void const* b, afxSize siz)
{
    afxByte const* pa = (afxByte const*)a;
    afxByte const* pb = (afxByte const*)b;

    // Use aligned loads if both addresses are aligned
    afxBool aligned = (((uintptr_t)pa & 0xF) == 0) && (((uintptr_t)pb & 0xF) == 0);
    afxSize const block_size = AFX_SIMD_ALIGNMENT;

    while (siz >= block_size)
    {
        afxSize usable_regs = siz / block_size;
        if (usable_regs > AFX_MAX_SIMD_REGISTERS)
            usable_regs = AFX_MAX_SIMD_REGISTERS;

        int mismatch_found = 0;
        for (afxSize i = 0; i < usable_regs; ++i)
        {
            __m128i va = aligned
                ? _mm_load_si128((const __m128i *)(pa + i * block_size))
                : _mm_loadu_si128((const __m128i *)(pa + i * block_size));
            __m128i vb = aligned
                ? _mm_load_si128((const __m128i *)(pb + i * block_size))
                : _mm_loadu_si128((const __m128i *)(pb + i * block_size));
            __m128i cmp = _mm_cmpeq_epi8(va, vb);
            if (_mm_movemask_epi8(cmp) != 0xFFFF)
            {
                // Fallback to byte check
                for (afxSize j = 0; j < block_size; ++j)
                {
                    afxSize idx = i * block_size + j;
                    if (pa[idx] != pb[idx])
                        return (afxInt)pa[idx] - (afxInt)pb[idx];
                }
                mismatch_found = 1;
                break;
            }
        }

        if (mismatch_found)
            return 0; // Already returned above

        afxSize chunk = usable_regs * block_size;
        pa += chunk;
        pb += chunk;
        siz -= chunk;
    }

    // Tail compare.
    for (afxSize i = 0; i < siz; ++i)
        if (pa[i] != pb[i])
            return (afxInt)pa[i] - (afxInt)pb[i];

    return 0;
}
#endif//AFX_ON_X86

_AFX afxInt AfxCompare(void const* a, void const* b, afxSize siz)
{
    afxError err = { 0 };
    afxInt rslt = 0;

    if (!siz) return 0;
    AFX_ASSERT(a);
    AFX_ASSERT(b);
#ifdef _AFX_TEST_COMPARE_BOUNDS
    *&((afxByte const*)a)[siz - 1];
    *&((afxByte const*)b)[siz - 1];
#endif
#ifdef _AFX_COMPARE_WITH_SSE
    rslt = AfxCompareSSE(a, b, siz);
#ifdef _AFX_TEST_COMPARE_RESULTS
    afxByte const* pa = (afxByte const*)a;
    afxByte const* pb = (afxByte const*)b;
    for (afxSize i = 0; i < siz; ++i)
    {
        if (pa[i] != pb[i])
        {
            afxInt v = (afxInt)pa[i] - (afxInt)pb[i];
            AFX_ASSERT(rslt == v);
            break;
        }
    }
#endif
#else
    // Clean and minimal convex implementation designed for debugging.
    afxByte const* pa = (afxByte const*)a;
    afxByte const* pb = (afxByte const*)b;

    for (afxSize i = 0; i < siz; ++i)
    {
        if (pa[i] != pb[i])
        {
            rslt = (afxInt)pa[i] - (afxInt)pb[i];
            break;
        }
    }
#endif
    return rslt;
}

#ifdef AFX_ON_X86
_AFX void* AfxFindSSE(void const* start, afxSize range, afxInt c)
{
    afxByte const*p = (afxByte const*)start;
    afxByte target = (afxByte)c;
    if (range == 0) return NULL;

    // Prepare comparison vector with the search byte
    __m128i vtarget = _mm_set1_epi8(target);

    // Process in blocks of 16 bytes
    while (range >= AFX_SIMD_ALIGNMENT)
    {
        afxSize usable_regs = range / AFX_SIMD_ALIGNMENT;
        if (usable_regs > AFX_MAX_SIMD_REGISTERS)
            usable_regs = AFX_MAX_SIMD_REGISTERS;

        for (afxSize i = 0; i < usable_regs; ++i)
        {
            afxByte const* chunk = p + i * AFX_SIMD_ALIGNMENT;

            // Choose load type based on alignment of current chunk
            __m128i v;
            if (((uintptr_t)chunk & 0xF) == 0) // is aligned?
            {
                v = _mm_load_si128((const __m128i *)chunk); // Aligned load
            }
            else
            {
                v = _mm_loadu_si128((const __m128i *)chunk); // Unaligned load
            }

            // Compare the loaded block to the target
            __m128i cmp = _mm_cmpeq_epi8(v, vtarget);
            int mask = _mm_movemask_epi8(cmp);

            // If any byte matches, return the pointer to the match
            if (mask != 0)
            {
                int offset = __builtin_ctz(mask); // Find the first set bit
                return (void*)(chunk + offset);
            }
        }

        afxSize chunk_size = usable_regs * AFX_SIMD_ALIGNMENT;
        p += chunk_size;
        range -= chunk_size;
    }

    // Handle tail if remaining size is less than block size
    for (afxSize i = 0; i < range; ++i)
        if (p[i] == target)
            return (void*)(p + i);

    return NULL;
}
#endif//AFX_ON_X86

_AFX void* AfxFind(void const* start, afxSize range, afxInt c)
{
    afxError err = { 0 };
    void* at = NIL;

    if (!range) return at;
    AFX_ASSERT(start);
#ifdef _AFX_TEST_FIND_BOUNDS
    *&((afxByte const*)start)[range - 1];
#endif
#ifdef _AFX_FIND_WITH_SSE
    at = AfxFindSSE(start, range, c);
#ifdef _AFX_TEST_FIND_RESULTS
    afxByte const* p = (afxByte const*)start;
    afxByte target = (afxByte)(c & 0xFF);
    for (afxSize i = 0; i < range; ++i)
    {
        if (p[i] == target)
        {
            AFX_ASSERT(at == (void*)(p + i));
            break;
        }
    }
#endif
#else
    afxByte const* p = (afxByte const*)start;
    afxByte target = (afxByte)(c & 0xFF);
    for (afxSize i = 0; i < range; ++i)
    {
        if (p[i] == target)
        {
            at = (void*)(p + i);
            break;
        }
    }
#endif
    return at;
}

// Function to perform the strided memcpy with SIMD, handling both aligned and unaligned cases inline
_AFXINL void AfxStreamSSE(afxSize cnt, void const* src, afxSize srcOffset, afxSize srcStride, void* dst, afxSize dstOffset, afxSize dstStride)
{
    afxByte*d = (afxByte*)dst + dstOffset;  // Start of destination buffer with offset
    afxByte const* s = (afxByte const*)src + srcOffset;  // Start of source buffer with offset

    // Calculate usable SIMD registers based on the total range to copy
    afxSize range = cnt * AFX_MAX(1, AFX_MIN(srcStride, dstStride));  // Total bytes to copy
    afxSize usable_regs = range / AFX_SIMD_ALIGNMENT;
    __m128i regs[16];  // Maximum of 16 SIMD registers

    // Ensure the usable registers do not exceed the number of available registers
    if (usable_regs > AFX_MAX_SIMD_REGISTERS)
        usable_regs = AFX_MAX_SIMD_REGISTERS;

    // Process the main data in blocks of the available SIMD registers
    for (afxSize i = 0; i < cnt; ++i)
    {
        // Load data from source using SIMD (either aligned or unaligned based on address)
        for (afxSize r = 0; r < usable_regs; ++r)
        {
            if (((uintptr_t)(s + r * AFX_SIMD_ALIGNMENT) & 0xF) == 0)
            {
                regs[r] = _mm_load_si128((__m128i *)(s + r * AFX_SIMD_ALIGNMENT));  // Aligned load
            }
            else
            {
                regs[r] = _mm_loadu_si128((__m128i *)(s + r * AFX_SIMD_ALIGNMENT));  // Unaligned load
            }
        }

        // Store the data to destination using SIMD (either aligned or unaligned based on address)
        for (afxSize r = 0; r < usable_regs; ++r)
        {
            if (((uintptr_t)(d + r * AFX_SIMD_ALIGNMENT) & 0xF) == 0)
            {
                _mm_store_si128((__m128i *)(d + r * AFX_SIMD_ALIGNMENT), regs[r]);  // Aligned store
            }
            else
            {
                _mm_storeu_si128((__m128i *)(d + r * AFX_SIMD_ALIGNMENT), regs[r]);  // Unaligned store
            }
        }

        // Move to the next block, based on the stride
        s += srcStride;
        d += dstStride;
    }

    // Process the remaining tail (less than 16 bytes)
    // Handle the tail if there are remaining bytes that don't fit into the SIMD block
    afxSize remaining_bytes = range % AFX_SIMD_ALIGNMENT;  // Remaining bytes after full SIMD blocks
    if (remaining_bytes > 0)
        for (afxSize i = 0; i < remaining_bytes; ++i)
            d[i] = s[i];
}

_AFX void* AfxMemset(void* dst, afxInt val, afxSize siz)
{
#ifdef _AFX_USE_MEMSET_FAST_PATH
    afxChar val2 = (afxChar)val;
    AfxFillPattern(dst, siz, &val2, sizeof(val2));
    return dst;
#else
    return memset(dst, val, siz);
#endif
}

_AFX void* AfxMemcpy(void* dst, void const* src, afxSize siz)
// Assumes no overlap between src and dst. If they overlap, behavior is undefined.
{
#ifdef _AFX_USE_MEMCPY_FAST_PATH
    AfxCopy(dst, src, siz);
    return dst;
#else
    return memcpy(dst, src, siz);
#endif
}

_AFX void* AfxMemmove(void* dst, void const* src, afxSize siz)
// Must handle overlapping memory safely, so it may need to copy backwards.
{
#ifdef _AFX_USE_MEMMOVE_FAST_PATH
    AfxMove(dst, src, siz);
    return dst;
#else
    return memmove(dst, src, siz);
#endif
}

_AFX afxInt AfxMemcmp(void const* buf1, void const* buf2, afxSize siz)
{
#ifdef _AFX_USE_MEMCMP_FAST_PATH
    return AfxCompare(buf1, buf2, siz);
#else
    return memcmp(buf1, buf2, siz);
#endif
}

_AFX void const* AfxMemchr(void const* buf, afxInt val, afxUnit cap)
{
#ifdef _AFX_USE_MEMCHR_FAST_PATH
    return AfxFind(buf, val, cap);
#else
    return memchr(buf, val, cap);
#endif
}

#ifndef _AFX_DONT_REIMPLEMENT_MEMSET
void* memset(void* dst, int val, size_t siz)
{
    afxChar val2 = (afxChar)val;
    AfxFillPattern(dst, siz, &val2, sizeof(val2));
    return dst;
}
#endif//_AFX_DONT_REIMPLEMENT_MEMSET


#ifndef _AFX_DONT_REIMPLEMENT_MEMCPY
void* memcpy(void* dst, const void* src, size_t siz)
{
    AfxCopy(dst, src, siz);
    return dst;
}
#endif//_AFX_DONT_REIMPLEMENT_MEMCPY

#ifndef _AFX_DONT_REIMPLEMENT_MEMMOVE
void* Memmove(void* dst, const void* src, size_t siz)
{
    AfxMove(dst, src, siz);
    return dst;
}
#endif//_AFX_DONT_REIMPLEMENT_MEMMOVE

#ifndef _AFX_DONT_IMPLEMENT_MEMCMP
int memcmp(void const* buf1, void const* buf2, size_t siz)
{
    return AfxCompare(buf1, buf2, siz);
}
#endif//_AFX_DONT_REIMPLEMENT_MEMCMP

#ifndef _AFX_DONT_REIMPLEMENT_MEMCHR
_Check_return_
_VCRTIMP void _CONST_RETURN* __cdecl memchr(
    _In_reads_bytes_opt_(_MaxCount) void const* _Buf,
    _In_                            int         _Val,
    _In_                            size_t      _MaxCount
)//;
//void const* memchr(void const* buf, int val, size_t cap)
{
    return AfxFind(_Buf, _MaxCount, _Val);
}
#endif//_AFX_DONT_REIMPLEMENT_MEMCHR
