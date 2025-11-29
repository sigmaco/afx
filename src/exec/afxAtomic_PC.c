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

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#endif

#if (defined(_MSC_VER) && !defined(__clang__))
#define USE_ATOMIC_WORKAROUND_MSVC
#else
#include <stdatomic.h> // compiles on Clang. Yes, even if marked as non-existing in VS.
#endif

#include "afxSystemDDK.h"

#ifdef AFX_ON_WINDOWS

_AFXINL void AfxStoreAtom32(afxAtom32* dst, afxInt32 value) { InterlockedExchange(dst, value); }
_AFXINL afxInt32 AfxLoadAtom32(afxAtom32* src) { return *src; }
_AFXINL afxInt32 AfxIncAtom32(afxAtom32* val) { return InterlockedExchangeAdd(val, 1) + 1; }
_AFXINL afxInt32 AfxDecAtom32(afxAtom32* val) { return InterlockedExchangeAdd(val, -1) - 1; }
_AFXINL afxInt32 AfxExchangeAtom32(afxAtom32* dst, afxInt32 value) { return InterlockedExchange(dst, value); }
_AFXINL afxInt32 AfxAddAtom32(afxAtom32* dst, afxInt32 value) { return InterlockedExchangeAdd(dst, value) + value; }
_AFXINL afxInt32 AfxSubAtom32(afxAtom32* dst, afxInt32 value) { return InterlockedExchangeAdd(dst, -value) - value; }
_AFXINL afxInt32 AfxAndAtom32(afxAtom32* dst, afxInt32 value) { return _InterlockedAnd(dst, value) & value; }
_AFXINL afxInt32 AfxOrAtom32(afxAtom32* dst, afxInt32 value) { return _InterlockedOr(dst, value) | value; }
_AFXINL afxBool AfxCasAtom32(afxAtom32* dst, afxInt32* expected, afxInt32 proposed) { return InterlockedCompareExchange(dst, proposed, *expected) == *expected ? 1 : 0; }

#ifdef AFX_ON_WIN64
_AFXINL afxInt64 AfxLoadAtom64(afxAtom64* src) { return *src; }
#else
_AFXINL afxInt64 AfxLoadAtom64(afxAtom64* src) { return InterlockedCompareExchange64(src, 0, 0); }
#endif

_AFXINL void AfxStoreAtom64(afxAtom64* dst, afxInt64 value) { InterlockedExchange64(dst, value); }
_AFXINL afxInt64 AfxIncAtom64(afxAtom64* val) { return InterlockedExchangeAdd64(val, 1) + 1; }
_AFXINL afxInt64 AfxDecAtom64(afxAtom64* val) { return InterlockedExchangeAdd64(val, -1) - 1; }
_AFXINL afxInt64 AfxExchangeAtom64(afxAtom64* dst, afxInt64 value) { return InterlockedExchange64(dst, value); }
_AFXINL afxInt64 AfxAddAtom64(afxAtom64* dst, afxInt64 value) { return InterlockedExchangeAdd64(dst, value) + value; }
_AFXINL afxInt64 AfxSubAtom64(afxAtom64* dst, afxInt64 value) { return InterlockedExchangeAdd64(dst, -value) - value; }
_AFXINL afxInt64 AfxAndAtom64(afxAtom64* dst, afxInt64 value) { return InterlockedAnd64(dst, value) & value; }
_AFXINL afxInt64 AfxOrAtom64(afxAtom64* dst, afxInt64 value) { return InterlockedOr64(dst, value) | value; }
_AFXINL afxBool AfxCasAtom64(afxAtom64* dst, afxInt64* expected, afxInt64 proposed) { afxInt64 exp = *expected;  return InterlockedCompareExchange64(dst, proposed, exp) == exp ? 1 : 0; }

_AFXINL void* AfxLoadAtomPtr(afxAtomPtr* src) { return (void*)*src; }

#ifdef AFX_ON_WIN64
_AFXINL void AfxStoreAtomPtr(afxAtomPtr* dst, void* value) { InterlockedExchangePointer((volatile PVOID*)dst, value); }
_AFXINL void* AfxExchangeAtomPtr(afxAtomPtr* dst, void* value) { return InterlockedExchangePointer((volatile PVOID*)dst, value); }
_AFXINL afxBool AfxCasAtomPtr(afxAtomPtr* dst, void** expected, void* proposed) { return InterlockedCompareExchangePointer((volatile PVOID*)dst, proposed, *expected) == *expected ? 1 : 0; }
#else
_AFXINL void AfxStoreAtomPtr(afxAtomPtr* dst, void* value) { InterlockedExchangePointer((void**)dst, value); }
_AFXINL void* AfxExchangeAtomPtr(afxAtomPtr* dst, void* value) { return InterlockedExchangePointer((void**)dst, value); }
_AFXINL afxBool AfxCasAtomPtr(afxAtomPtr* dst, void** expected, void* proposed) { return InterlockedCompareExchangePointer((void**)dst, proposed, *expected) == *expected ? 1 : 0; }
#endif

#else // we assume it will be Linux

_AFXINL afxInt32 AfxLoadAtom32(afxAtom32* pSrc) { int32_t ret; __atomic_load((int32_t*)pSrc, &ret, __ATOMIC_ACQUIRE); return ret; }
_AFXINL void AfxStoreAtom32(afxAtom32* pDest, afxInt32 value) { __atomic_store((int32_t*)pDest, &value, __ATOMIC_SEQ_CST); }
_AFXINL afxInt32 AfxIncAtom32(afxAtom32* pValue) { return __sync_add_and_fetch((int32_t*)pValue, 1); }
_AFXINL afxInt32 AfxDecAtom32(afxAtom32* pValue) { return __sync_sub_and_fetch((int32_t*)pValue, 1); }
_AFXINL afxInt32 AfxExchangeAtom32(afxAtom32* pDest, afxInt32 value) { return __sync_lock_test_and_set((int32_t*)pDest, value); }
_AFXINL afxInt32 AfxAddAtom32(afxAtom32* pDest, afxInt32 value) { return __sync_add_and_fetch((int32_t*)pDest, value); }
_AFXINL afxInt32 AfxSubAtom32(afxAtom32* pDest, afxInt32 value) { return __sync_sub_and_fetch((int32_t*)pDest, value); }
_AFXINL afxInt32 AfxAndAtom32(afxAtom32* pDest, afxInt32 value) { return __sync_and_and_fetch((int32_t*)pDest, value); }
_AFXINL afxInt32 AfxOrAtom32(afxAtom32* pDest, afxInt32 value) { return __sync_or_and_fetch((int32_t*)pDest, value); }
_AFXINL afxBool AfxCasAtom32(afxAtom32* pDest, afxInt32* expected, afxInt32 proposed) { return __sync_bool_compare_and_swap((int32_t*)pDest, *expected, proposed); }

#if defined( AFX_ON_X86 ) || ( defined( AFX_ISA_ARM ) && ( AFX_ISA_ARM <= 7 ) )
_AFXINL afxInt64 AfxLoadAtom64(afxAtom64* pSrc) { return __sync_val_compare_and_swap(pSrc, 0, 0); }
_AFXINL void AfxStoreAtom64(afxAtom64* pDest, afxInt64 value) { int64_t tmp; do { tmp = *pDest; } while (__sync_val_compare_and_swap(pDest, tmp, value) != tmp); }
#else
_AFXINL afxInt64 AfxLoadAtom64(afxAtom64* pSrc) { afxInt64 ret; __atomic_load((int64_t*)pSrc, &ret, __ATOMIC_ACQUIRE); return ret; }
_AFXINL void AfxStoreAtom64(afxAtom64* pDest, afxInt64 value) { __atomic_store((int64_t*)pDest, &value, __ATOMIC_SEQ_CST); }
#endif

_AFXINL afxInt64 AfxIncAtom64(afxAtom64* pValue) { return __sync_add_and_fetch((int64_t*)pValue, 1); }
_AFXINL afxInt64 AfxDecAtom64(afxAtom64* pValue) { return __sync_sub_and_fetch((int64_t*)pValue, 1); }
_AFXINL afxInt64 AfxExchangeAtom64(afxAtom64* pDest, afxInt64 value) { return __sync_lock_test_and_set((int64_t*)pDest, value); }
_AFXINL afxInt64 AfxAddAtom64(afxAtom64* pDest, afxInt64 value) { return __sync_add_and_fetch((int64_t*)pDest, value); }
_AFXINL afxInt64 AfxSubAtom64(afxAtom64* pDest, afxInt64 value) { return __sync_sub_and_fetch((int64_t*)pDest, value); }
_AFXINL afxInt64 AfxAndAtom64(afxAtom64* pDest, afxInt64 value) { return __sync_and_and_fetch((int64_t*)pDest, value); }
_AFXINL afxInt64 AfxOrAtom64(afxAtom64* pDest, afxInt64 value) { return __sync_or_and_fetch((int64_t*)pDest, value); }
_AFXINL afxBool AfxCasAtom64(afxAtom64* pDest, afxInt64* expected, afxInt64 proposed) { return __sync_bool_compare_and_swap((int64_t*)pDest, *expected, proposed); }

_AFXINL void* AfxLoadAtomPtr(afxAtomPtr* pSrc) { size_t ret; __atomic_load((size_t*)pSrc, &ret, __ATOMIC_ACQUIRE); return (void*)ret; }
_AFXINL void AfxStoreAtomPtr(afxAtomPtr* pDest, void* value) { __atomic_store((size_t*)pDest, (size_t*)&value, __ATOMIC_SEQ_CST); }
_AFXINL void* AfxExchangeAtomPtr(afxAtomPtr* pDest, void* value) { return (void*)__sync_lock_test_and_set((void**)pDest, value); }
_AFXINL afxBool AfxCasAtomPtr(afxAtomPtr* pDest, void** expected, void* proposed) { return __sync_bool_compare_and_swap((void**)pDest, *expected, proposed); }

#endif

AFX_STATIC_ASSERT(sizeof(LONG) == sizeof(afxInt32), "");

_AFX void AfxAtomicInit32(volatile afxInt32 *obj, afxInt32 desired)
{
#if USE_ATOMIC_WORKAROUND_MSVC
    // Atomically set the value of obj to desired (this is a simple initialization)
    InterlockedExchange((volatile LONG*)obj, (LONG)desired);
#else
    atomic_init((volatile _Atomic afxInt32*)obj, desired);
#endif
}

_AFX void AfxAtomicInit64(volatile afxInt64 *obj, afxInt64 desired)
{
#if USE_ATOMIC_WORKAROUND_MSVC
    // For 64-bit long long, we would use InterlockedExchange64 if available (for 64-bit systems)
    InterlockedExchange64((volatile LONGLONG*)obj, desired);
#else
    atomic_init((volatile _Atomic afxInt64*)obj, desired);
#endif
}

_AFX afxInt32 AfxAtomicLoad32(const volatile afxInt32* obj, afxMemoryOrder order)
{
#if USE_ATOMIC_WORKAROUND_MSVC
    afxInt32 result = 0;

    // Check the memory order and handle accordingly.
    switch (order) {
    case afxMemoryOrder_RELAXED:
        // No ordering required
        result = InterlockedCompareExchange((volatile LONG*)obj, 0, 0);
        break;
    case afxMemoryOrder_CONSUME:
    case afxMemoryOrder_ACQUIRE:
        // acquire order semantics - enforce load ordering
        result = InterlockedCompareExchange((volatile LONG*)obj, 0, 0);
        // Optionally ensure proper compiler reordering or acquire semantics
        break;
    case afxMemoryOrder_RELEASE:
    case afxMemoryOrder_ACQ_REL:
        // These would typically be paired with stores and won't be used here.
        break;
    case afxMemoryOrder_SEQ_CST:
        // Seq_cst ordering, the strictest; use Interlocked functions as needed
        result = InterlockedCompareExchange((volatile LONG*)obj, 0, 0);
        break;
    default:
        // Default behavior for unsupported memory order, should not happen.
        break;
    }
    return result;
#else
    return atomic_load_explicit((const volatile _Atomic afxInt32*)obj, ((order == afxMemoryOrder_SEQ_CST) ? memory_order_seq_cst : (order - 1)));
#endif
}

_AFX afxInt64 AfxAtomicLoad64(const volatile afxInt64* obj, afxMemoryOrder order)
{
#if USE_ATOMIC_WORKAROUND_MSVC
    afxInt64 result = 0;

    // Check the memory order and handle accordingly.
    switch (order) {
    case afxMemoryOrder_RELAXED:
        // No ordering required
        result = InterlockedCompareExchange64((volatile LONG64*)obj, 0, 0);
        break;
    case afxMemoryOrder_CONSUME:
    case afxMemoryOrder_ACQUIRE:
        // acquire order semantics - enforce load ordering
        result = InterlockedCompareExchange64((volatile LONG64*)obj, 0, 0);
        // Optionally ensure proper compiler reordering or acquire semantics
        break;
    case afxMemoryOrder_RELEASE:
    case afxMemoryOrder_ACQ_REL:
        // These would typically be paired with stores and won't be used here.
        break;
    case afxMemoryOrder_SEQ_CST:
        // Seq_cst ordering, the strictest; use Interlocked functions as needed
        result = InterlockedCompareExchange64((volatile LONG64*)obj, 0, 0);
        break;
    default:
        // Default behavior for unsupported memory order, should not happen.
        break;
    }
    return result;
#else
    return atomic_load_explicit((const volatile _Atomic afxInt64*)obj, ((order == afxMemoryOrder_SEQ_CST) ? memory_order_seq_cst : (order - 1)));
#endif
}

/*
    atomic_store_explicit()

    Atomically replaces the value of the atomic variable pointed to by obj with desired. 
    The operation is atomic write operation.

    The first version orders memory accesses according to memory_order_seq_cst, 
    the second version orders memory accesses according to order. 
    Order must be one of memory_order_relaxed, memory_order_release or memory_order_seq_cst. 
    Otherwise the behavior is undefined.
*/

_AFX void AfxAtomicStore32(volatile afxInt32* obj, afxInt32 value, afxMemoryOrder order)
{
#if USE_ATOMIC_WORKAROUND_MSVC
    switch (order) {
    case afxMemoryOrder_RELAXED:
        // No ordering required, just perform the atomic store
        InterlockedExchange((volatile LONG*)obj, value);
        break;
    case afxMemoryOrder_CONSUME:
    case afxMemoryOrder_ACQUIRE:
    case afxMemoryOrder_ACQ_REL:
        // In general, stores with these memory orders don't need specific changes,
        // but we can enforce proper ordering with acquire or release semantics.
        // On Windows, InterlockedExchange itself provides sufficient atomicity.
        InterlockedExchange((volatile LONG*)obj, value);
        break;
    case afxMemoryOrder_RELEASE:
        // Ensure that previous writes are not reordered after this store.
        InterlockedExchange((volatile LONG*)obj, value);
        // No additional mechanism needed for release; InterlockedExchange ensures ordering
        break;
    case afxMemoryOrder_SEQ_CST:
        // Sequential consistency ordering (strongest), enforce global ordering
        InterlockedExchange((volatile LONG*)obj, value);
        break;
    default:
        // Handle cases where memory order is not recognized (should not happen)
        break;
    }
#else
    return atomic_store_explicit((volatile _Atomic afxInt32*)obj, value, ((order == afxMemoryOrder_SEQ_CST) ? memory_order_seq_cst : (order - 1)));
#endif
}

_AFX void AfxAtomicStore64(volatile afxInt64* obj, afxInt64 value, afxMemoryOrder order)
{
#if USE_ATOMIC_WORKAROUND_MSVC
    switch (order) {
    case afxMemoryOrder_RELAXED:
        // No ordering required, just perform the atomic store
        InterlockedExchange64((volatile LONGLONG*)obj, value);
        break;
    case afxMemoryOrder_CONSUME:
    case afxMemoryOrder_ACQUIRE:
    case afxMemoryOrder_ACQ_REL:
        // In general, stores with these memory orders don't need specific changes,
        // but we can enforce proper ordering with acquire or release semantics.
        // On Windows, InterlockedExchange itself provides sufficient atomicity.
        InterlockedExchange64((volatile LONGLONG*)obj, value);
        break;
    case afxMemoryOrder_RELEASE:
        // Ensure that previous writes are not reordered after this store.
        InterlockedExchange64((volatile LONGLONG*)obj, value);
        // No additional mechanism needed for release; InterlockedExchange ensures ordering
        break;
    case afxMemoryOrder_SEQ_CST:
        // Sequential consistency ordering (strongest), enforce global ordering
        InterlockedExchange64((volatile LONGLONG*)obj, value);
        break;
    default:
        // Handle cases where memory order is not recognized (should not happen)
        break;
    }
#else
    return atomic_store_explicit((volatile _Atomic afxInt64*)obj, value, ((order == afxMemoryOrder_SEQ_CST) ? memory_order_seq_cst : (order - 1)));
#endif
}

_AFX afxInt32 AfxAtomicExchange32(volatile afxInt32* obj, afxInt32 value, afxMemoryOrder order)
{
#if USE_ATOMIC_WORKAROUND_MSVC
    long old_value = 0;

    switch (order) {
    case afxMemoryOrder_RELAXED:
        // No ordering required, just perform the atomic exchange
        old_value = InterlockedExchange((volatile LONG*)obj, value);
        break;
    case afxMemoryOrder_CONSUME:
    case afxMemoryOrder_ACQUIRE:
    case afxMemoryOrder_ACQ_REL:
        // For these memory orders, InterlockedExchange already provides the necessary atomicity
        old_value = InterlockedExchange((volatile LONG*)obj, value);
        break;
    case afxMemoryOrder_RELEASE:
        // Ensure that previous writes are not reordered after the atomic exchange
        old_value = InterlockedExchange((volatile LONG*)obj, value);
        break;
    case afxMemoryOrder_SEQ_CST:
        // Sequential consistency ordering (strongest), enforce global ordering
        old_value = InterlockedExchange((volatile LONG*)obj, value);
        break;
    default:
        // Handle cases where memory order is not recognized (should not happen)
        break;
    }
    return old_value;
#else
    return atomic_exchange_explicit((volatile _Atomic afxInt32*)obj, value, ((order == afxMemoryOrder_SEQ_CST) ? memory_order_seq_cst : (order - 1)));
#endif
}

_AFX afxInt64 AfxAtomicExchange64(volatile afxInt64* obj, afxInt64 value, afxMemoryOrder order)
{
#if USE_ATOMIC_WORKAROUND_MSVC
    afxInt64 old_value = 0;

    switch (order) {
    case afxMemoryOrder_RELAXED:
        // No ordering required, just perform the atomic exchange
        old_value = InterlockedExchange64((volatile LONGLONG*)obj, value);
        break;
    case afxMemoryOrder_CONSUME:
    case afxMemoryOrder_ACQUIRE:
    case afxMemoryOrder_ACQ_REL:
        // For these memory orders, InterlockedExchange already provides the necessary atomicity
        old_value = InterlockedExchange64((volatile LONGLONG*)obj, value);
        break;
    case afxMemoryOrder_RELEASE:
        // Ensure that previous writes are not reordered after the atomic exchange
        old_value = InterlockedExchange64((volatile LONGLONG*)obj, value);
        break;
    case afxMemoryOrder_SEQ_CST:
        // Sequential consistency ordering (strongest), enforce global ordering
        old_value = InterlockedExchange64((volatile LONGLONG*)obj, value);
        break;
    default:
        // Handle cases where memory order is not recognized (should not happen)
        break;
    }
    return old_value;
#else
    return atomic_exchange_explicit((volatile _Atomic afxInt64*)obj, value, ((order == afxMemoryOrder_SEQ_CST) ? memory_order_seq_cst : (order - 1)));
#endif
}

_AFX afxBool AfxAtomicCompareExchangeStrong32(volatile afxInt32* obj, afxInt32* expected, afxInt32 desired, afxMemoryOrder succ, afxMemoryOrder fail)
{
#if USE_ATOMIC_WORKAROUND_MSVC
    (void)succ;
    (void)fail;
    afxInt32 current = *expected;
    afxInt32 old_value = InterlockedCompareExchange((volatile LONG*)obj, desired, current);

    // Check if the compare-and-swap succeeded (i.e., the values matched)
    if (old_value == current)
    {
        // If successful, update the expected value to the new value
        *expected = desired;
        return TRUE;
    }
    else
    {
        // If not successful, update the expected value to the current value
        *expected = old_value;
        return FALSE;
    }
#else
    return atomic_compare_exchange_strong_explicit((volatile _Atomic afxInt32*)obj, expected, desired, ((succ == afxMemoryOrder_SEQ_CST) ? memory_order_seq_cst : (succ - 1)), ((fail == afxMemoryOrder_SEQ_CST) ? memory_order_seq_cst : (fail - 1)));
#endif
}

_AFX afxBool AfxAtomicCompareExchangeStrong64(volatile afxInt64* obj, afxInt64* expected, afxInt64 desired, afxMemoryOrder succ, afxMemoryOrder fail)
{
#if USE_ATOMIC_WORKAROUND_MSVC
    (void)succ;
    (void)fail;
    afxInt64 current = *expected;
    afxInt64 old_value = InterlockedCompareExchange64((volatile LONG64*)obj, desired, current);

    // Check if the compare-and-swap succeeded (i.e., the values matched)
    if (old_value == current)
    {
        // If successful, update the expected value to the new value
        *expected = desired;
        return TRUE;
    }
    else
    {
        // If not successful, update the expected value to the current value
        *expected = old_value;
        return FALSE;
    }
#else
    return atomic_compare_exchange_strong_explicit((volatile _Atomic afxInt64*)obj, expected, desired, ((succ == afxMemoryOrder_SEQ_CST) ? memory_order_seq_cst : (succ - 1)), ((fail == afxMemoryOrder_SEQ_CST) ? memory_order_seq_cst : (fail - 1)));
#endif
}

_AFX afxBool AfxAtomicCompareExchangeWeak32(volatile afxInt32* obj, afxInt32* expected, afxInt32 desired, afxMemoryOrder succ, afxMemoryOrder fail)
{
#if USE_ATOMIC_WORKAROUND_MSVC
    (void)succ;
    (void)fail;
    afxInt32 current = *expected;
    afxInt32 old_value = InterlockedCompareExchange((volatile LONG*)obj, desired, current);

    // Check if the compare-and-swap succeeded (i.e., the values matched)
    if (old_value == current)
    {
        // If successful, update the expected value to the new value
        *expected = desired;
        return TRUE;
    }
    else
    {
        // If not successful, update the expected value to the current value
        *expected = old_value;
        return FALSE;
    }
#else
    return atomic_compare_exchange_weak_explicit((volatile _Atomic afxInt32*)obj, expected, desired, ((succ == afxMemoryOrder_SEQ_CST) ? memory_order_seq_cst : (succ - 1)), ((fail == afxMemoryOrder_SEQ_CST) ? memory_order_seq_cst : (fail - 1)));
#endif
}

_AFX afxBool AfxAtomicCompareExchangeWeak64(volatile afxInt64* obj, afxInt64* expected, afxInt64 desired, afxMemoryOrder succ, afxMemoryOrder fail)
{
#if USE_ATOMIC_WORKAROUND_MSVC
    (void)succ;
    (void)fail;
    afxInt64 current = *expected;
    afxInt64 old_value = InterlockedCompareExchange64((volatile LONG64*)obj, desired, current);

    // Check if the compare-and-swap succeeded (i.e., the values matched)
    if (old_value == current)
    {
        // If successful, update the expected value to the new value
        *expected = desired;
        return TRUE;
    }
    else
    {
        // If not successful, update the expected value to the current value
        *expected = old_value;
        return FALSE;
    }
#else
    return atomic_compare_exchange_weak_explicit((volatile _Atomic afxInt64*)obj, expected, desired, ((succ == afxMemoryOrder_SEQ_CST) ? memory_order_seq_cst : (succ - 1)), ((fail == afxMemoryOrder_SEQ_CST) ? memory_order_seq_cst : (fail - 1)));
#endif
}

_AFX afxInt32 AfxAtomicFetchAdd32(volatile afxInt32* obj, afxInt32 value, afxMemoryOrder order)
{
#if USE_ATOMIC_WORKAROUND_MSVC
    afxInt32 old_value;

    switch (order)
    {
    case afxMemoryOrder_RELAXED:
        // No ordering required, just perform the atomic fetch-and-add
        old_value = InterlockedExchangeAdd((volatile LONG*)obj, value);
        break;
    case afxMemoryOrder_CONSUME:
    case afxMemoryOrder_ACQUIRE:
    case afxMemoryOrder_ACQ_REL:
        // For these memory orders, InterlockedExchangeAdd already provides the necessary atomicity
        old_value = InterlockedExchangeAdd((volatile LONG*)obj, value);
        break;
    case afxMemoryOrder_RELEASE:
        // Ensure that previous writes are not reordered after the atomic fetch-and-add
        old_value = InterlockedExchangeAdd((volatile LONG*)obj, value);
        break;
    case afxMemoryOrder_SEQ_CST:
        // Sequential consistency ordering (strongest), enforce global ordering
        old_value = InterlockedExchangeAdd((volatile LONG*)obj, value);
        break;
    default:
        // Handle cases where memory order is not recognized (should not happen)
        break;
    }
    return old_value;
#else
    return atomic_fetch_add_explicit((volatile _Atomic afxInt32*)obj, value, ((order == afxMemoryOrder_SEQ_CST) ? memory_order_seq_cst : (order - 1)));
#endif
}

_AFX afxInt64 AfxAtomicFetchAdd64(volatile afxInt64* obj, afxInt64 value, afxMemoryOrder order)
{
#if USE_ATOMIC_WORKAROUND_MSVC
    afxInt64 old_value;

    switch (order)
    {
    case afxMemoryOrder_RELAXED:
        // No ordering required, just perform the atomic fetch-and-add
        old_value = InterlockedExchangeAdd64((volatile LONGLONG*)obj, value);
        break;
    case afxMemoryOrder_CONSUME:
    case afxMemoryOrder_ACQUIRE:
    case afxMemoryOrder_ACQ_REL:
        // For these memory orders, InterlockedExchangeAdd already provides the necessary atomicity
        old_value = InterlockedExchangeAdd64((volatile LONGLONG*)obj, value);
        break;
    case afxMemoryOrder_RELEASE:
        // Ensure that previous writes are not reordered after the atomic fetch-and-add
        old_value = InterlockedExchangeAdd64((volatile LONGLONG*)obj, value);
        break;
    case afxMemoryOrder_SEQ_CST:
        // Sequential consistency ordering (strongest), enforce global ordering
        old_value = InterlockedExchangeAdd64((volatile LONGLONG*)obj, value);
        break;
    default:
        // Handle cases where memory order is not recognized (should not happen)
        break;
    }
    return old_value;
#else
    return atomic_fetch_add_explicit((volatile _Atomic afxInt64*)obj, value, ((order == afxMemoryOrder_SEQ_CST) ? memory_order_seq_cst : (order - 1)));
#endif
}

_AFX afxInt32 AfxAtomicFetchSub32(volatile afxInt32* obj, afxInt32 value, afxMemoryOrder order)
{
#if USE_ATOMIC_WORKAROUND_MSVC
    afxInt32 old_value;

    // Handle different memory orders
    switch (order) {
    case afxMemoryOrder_RELAXED:
        // No ordering required, just perform the atomic fetch-and-subtract
        old_value = InterlockedExchangeAdd((volatile LONG*)obj, -value);
        break;
    case afxMemoryOrder_CONSUME:
    case afxMemoryOrder_ACQUIRE:
    case afxMemoryOrder_ACQ_REL:
        // For these memory orders, InterlockedExchangeAdd already provides the necessary atomicity
        old_value = InterlockedExchangeAdd((volatile LONG*)obj, -value);
        break;
    case afxMemoryOrder_RELEASE:
        // Ensure that previous writes are not reordered after the atomic fetch-and-subtract
        old_value = InterlockedExchangeAdd((volatile LONG*)obj, -value);
        break;
    case afxMemoryOrder_SEQ_CST:
        // Sequential consistency ordering (strongest), enforce global ordering
        old_value = InterlockedExchangeAdd((volatile LONG*)obj, -value);
        break;
    default:
        // Handle cases where memory order is not recognized (should not happen)
        break;
    }
    return old_value;
#else
    return atomic_fetch_sub_explicit((volatile _Atomic afxInt32*)obj, value, ((order == afxMemoryOrder_SEQ_CST) ? memory_order_seq_cst : (order - 1)));
#endif
}

_AFX afxInt64 AfxAtomicFetchSub64(volatile afxInt64* obj, afxInt64 value, afxMemoryOrder order)
{
#if USE_ATOMIC_WORKAROUND_MSVC
    afxInt64 old_value;

    // Handle different memory orders
    switch (order) {
    case afxMemoryOrder_RELAXED:
        // No ordering required, just perform the atomic fetch-and-subtract
        old_value = InterlockedExchangeAdd64((volatile LONGLONG*)obj, -value);
        break;
    case afxMemoryOrder_CONSUME:
    case afxMemoryOrder_ACQUIRE:
    case afxMemoryOrder_ACQ_REL:
        // For these memory orders, InterlockedExchangeAdd already provides the necessary atomicity
        old_value = InterlockedExchangeAdd64((volatile LONGLONG*)obj, -value);
        break;
    case afxMemoryOrder_RELEASE:
        // Ensure that previous writes are not reordered after the atomic fetch-and-subtract
        old_value = InterlockedExchangeAdd64((volatile LONGLONG*)obj, -value);
        break;
    case afxMemoryOrder_SEQ_CST:
        // Sequential consistency ordering (strongest), enforce global ordering
        old_value = InterlockedExchangeAdd64((volatile LONGLONG*)obj, -value);
        break;
    default:
        // Handle cases where memory order is not recognized (should not happen)
        break;
    }
    return old_value;
#else
    return atomic_fetch_sub_explicit((volatile _Atomic afxInt64*)obj, value, ((order == afxMemoryOrder_SEQ_CST) ? memory_order_seq_cst : (order - 1)));
#endif
}

_AFX afxInt32 AfxAtomicFetchOr32(volatile afxInt32* obj, afxInt32 value, afxMemoryOrder order)
{
#if USE_ATOMIC_WORKAROUND_MSVC
    afxInt32 old_value;

    // Handle different memory orders
    switch (order)
    {
    case afxMemoryOrder_RELAXED:
        // No ordering required, just perform the atomic fetch-and-or
        old_value = InterlockedOr((volatile LONG*)obj, value);
        break;
    case afxMemoryOrder_CONSUME:
    case afxMemoryOrder_ACQUIRE:
    case afxMemoryOrder_ACQ_REL:
        // For these memory orders, InterlockedOr already provides the necessary atomicity
        old_value = InterlockedOr((volatile LONG*)obj, value);
        break;
    case afxMemoryOrder_RELEASE:
        // Ensure that previous writes are not reordered after the atomic fetch-and-or
        old_value = InterlockedOr((volatile LONG*)obj, value);
        break;
    case afxMemoryOrder_SEQ_CST:
        // Sequential consistency ordering (strongest), enforce global ordering
        old_value = InterlockedOr((volatile LONG*)obj, value);
        break;
    default:
        // Handle cases where memory order is not recognized (should not happen)
        break;
    }
    return old_value;
#else
    return atomic_fetch_or_explicit((volatile _Atomic afxInt32*)obj, value, ((order == afxMemoryOrder_SEQ_CST) ? memory_order_seq_cst : (order - 1)));
#endif
}

_AFX afxInt64 AfxAtomicFetchOr64(volatile afxInt64* obj, afxInt64 value, afxMemoryOrder order)
{
#if USE_ATOMIC_WORKAROUND_MSVC
    afxInt64 old_value;

    // Handle different memory orders
    switch (order)
    {
    case afxMemoryOrder_RELAXED:
        // No ordering required, just perform the atomic fetch-and-or
        old_value = InterlockedOr64((volatile LONGLONG*)obj, value);
        break;
    case afxMemoryOrder_CONSUME:
    case afxMemoryOrder_ACQUIRE:
    case afxMemoryOrder_ACQ_REL:
        // For these memory orders, InterlockedOr already provides the necessary atomicity
        old_value = InterlockedOr64((volatile LONGLONG*)obj, value);
        break;
    case afxMemoryOrder_RELEASE:
        // Ensure that previous writes are not reordered after the atomic fetch-and-or
        old_value = InterlockedOr64((volatile LONGLONG*)obj, value);
        break;
    case afxMemoryOrder_SEQ_CST:
        // Sequential consistency ordering (strongest), enforce global ordering
        old_value = InterlockedOr64((volatile LONGLONG*)obj, value);
        break;
    default:
        // Handle cases where memory order is not recognized (should not happen)
        break;
    }
    return old_value;
#else
    return atomic_fetch_or_explicit((volatile _Atomic afxInt64*)obj, value, ((order == afxMemoryOrder_SEQ_CST) ? memory_order_seq_cst : (order - 1)));
#endif
}

_AFX afxInt32 AfxAtomicFetchXor32(volatile afxInt32* obj, afxInt32 value, afxMemoryOrder order)
{
#if USE_ATOMIC_WORKAROUND_MSVC
    afxInt32 old_value;

    // Handle different memory orders
    switch (order) {
    case afxMemoryOrder_RELAXED:
        // No ordering required, just perform the atomic fetch-and-xor
        old_value = InterlockedXor((volatile LONG*)obj, value);
        break;
    case afxMemoryOrder_CONSUME:
    case afxMemoryOrder_ACQUIRE:
    case afxMemoryOrder_ACQ_REL:
        // For these memory orders, InterlockedXor already provides the necessary atomicity
        old_value = InterlockedXor((volatile LONG*)obj, value);
        break;
    case afxMemoryOrder_RELEASE:
        // Ensure that previous writes are not reordered after the atomic fetch-and-xor
        old_value = InterlockedXor((volatile LONG*)obj, value);
        break;
    case afxMemoryOrder_SEQ_CST:
        // Sequential consistency ordering (strongest), enforce global ordering
        old_value = InterlockedXor((volatile LONG*)obj, value);
        break;
    default:
        // Handle cases where memory order is not recognized (should not happen)
        break;
    }
    return old_value;
#else
    return atomic_fetch_xor_explicit((volatile _Atomic afxInt32*)obj, value, ((order == afxMemoryOrder_SEQ_CST) ? memory_order_seq_cst : (order - 1)));
#endif
}

_AFX afxInt64 AfxAtomicFetchXor64(volatile afxInt64* obj, afxInt64 value, afxMemoryOrder order)
{
#if USE_ATOMIC_WORKAROUND_MSVC
    afxInt64 old_value;

    // Handle different memory orders
    switch (order) {
    case afxMemoryOrder_RELAXED:
        // No ordering required, just perform the atomic fetch-and-xor
        old_value = InterlockedXor64((volatile LONG64*)obj, value);
        break;
    case afxMemoryOrder_CONSUME:
    case afxMemoryOrder_ACQUIRE:
    case afxMemoryOrder_ACQ_REL:
        // For these memory orders, InterlockedXor already provides the necessary atomicity
        old_value = InterlockedXor64((volatile LONG64*)obj, value);
        break;
    case afxMemoryOrder_RELEASE:
        // Ensure that previous writes are not reordered after the atomic fetch-and-xor
        old_value = InterlockedXor64((volatile LONG64*)obj, value);
        break;
    case afxMemoryOrder_SEQ_CST:
        // Sequential consistency ordering (strongest), enforce global ordering
        old_value = InterlockedXor64((volatile LONG64*)obj, value);
        break;
    default:
        // Handle cases where memory order is not recognized (should not happen)
        break;
    }
    return old_value;
#else
    return atomic_fetch_xor_explicit((volatile _Atomic afxInt64*)obj, value, ((order == afxMemoryOrder_SEQ_CST) ? memory_order_seq_cst : (order - 1)));
#endif
}

_AFX afxInt32 AfxAtomicFetchAnd32(volatile afxInt32* obj, afxInt32 value, afxMemoryOrder order)
{
#if USE_ATOMIC_WORKAROUND_MSVC
    afxInt32 old_value;

    // Handle different memory orders
    switch (order) {
    case afxMemoryOrder_RELAXED:
        // No ordering required, just perform the atomic fetch-and operation
        old_value = InterlockedAnd((volatile LONG*)obj, value);
        break;
    case afxMemoryOrder_CONSUME:
    case afxMemoryOrder_ACQUIRE:
    case afxMemoryOrder_ACQ_REL:
        // For these memory orders, InterlockedAnd already provides atomicity
        old_value = InterlockedAnd((volatile LONG*)obj, value);
        break;
    case afxMemoryOrder_RELEASE:
        // Ensure that previous writes are not reordered after the atomic fetch-and
        old_value = InterlockedAnd((volatile LONG*)obj, value);
        break;
    case afxMemoryOrder_SEQ_CST:
        // Sequential consistency ordering (strongest), enforce global ordering
        old_value = InterlockedAnd((volatile LONG*)obj, value);
        break;
    default:
        // Handle cases where memory order is not recognized (should not happen)
        break;
    }
    return old_value;
#else
return atomic_fetch_and_explicit((volatile _Atomic afxInt32*)obj, value, ((order == afxMemoryOrder_SEQ_CST) ? memory_order_seq_cst : (order - 1)));
#endif
}

_AFX afxInt64 AfxAtomicFetchAnd64(volatile afxInt64* obj, afxInt64 value, afxMemoryOrder order)
{
#if USE_ATOMIC_WORKAROUND_MSVC
    afxInt64 old_value;

    // Handle different memory orders
    switch (order) {
    case afxMemoryOrder_RELAXED:
        // No ordering required, just perform the atomic fetch-and operation
        old_value = InterlockedAnd64((volatile LONG64*)obj, value);
        break;
    case afxMemoryOrder_CONSUME:
    case afxMemoryOrder_ACQUIRE:
    case afxMemoryOrder_ACQ_REL:
        // For these memory orders, InterlockedAnd already provides atomicity
        old_value = InterlockedAnd64((volatile LONG64*)obj, value);
        break;
    case afxMemoryOrder_RELEASE:
        // Ensure that previous writes are not reordered after the atomic fetch-and
        old_value = InterlockedAnd64((volatile LONG64*)obj, value);
        break;
    case afxMemoryOrder_SEQ_CST:
        // Sequential consistency ordering (strongest), enforce global ordering
        old_value = InterlockedAnd64((volatile LONG64*)obj, value);
        break;
    default:
        // Handle cases where memory order is not recognized (should not happen)
        break;
    }
    return old_value;
#else
    return atomic_fetch_and_explicit((volatile _Atomic afxInt64*)obj, value, ((order == afxMemoryOrder_SEQ_CST) ? memory_order_seq_cst : (order - 1)));
#endif
}

_AFX void AfxAtomicFlagClear(volatile afxBool* obj, afxMemoryOrder order)
{
#if USE_ATOMIC_WORKAROUND_MSVC
    switch (order) {
    case afxMemoryOrder_RELAXED:
        // No ordering required, just clear the atomic flag
        InterlockedExchange((volatile LONG*)obj, 0);
        break;
    case afxMemoryOrder_CONSUME:
    case afxMemoryOrder_ACQUIRE:
    case afxMemoryOrder_ACQ_REL:
        // These memory orders require ensuring that previous operations are completed before clearing the flag
        InterlockedExchange((volatile LONG*)obj, 0);
        break;
    case afxMemoryOrder_RELEASE:
        // Release semantics: Ensure previous writes are visible before clearing the flag
        InterlockedExchange((volatile LONG*)obj, 0);
        break;
    case afxMemoryOrder_SEQ_CST:
        // Sequential consistency ordering (strongest), enforce global ordering
        InterlockedExchange((volatile LONG*)obj, 0);
        break;
    default:
        // Handle any unrecognized memory orders (should not happen)
        break;
    }
#else
    atomic_flag_clear_explicit((volatile atomic_flag*)obj, ((order == afxMemoryOrder_SEQ_CST) ? memory_order_seq_cst : (order - 1)));
#endif
}

_AFX afxBool AfxAtomicFlagTestAndSet(volatile afxBool* obj, afxMemoryOrder order)
{
#if USE_ATOMIC_WORKAROUND_MSVC
    afxBool old_value;

    switch (order) {
    case afxMemoryOrder_RELAXED:
        // No ordering required, just set the atomic flag to true and get the previous value
        old_value = InterlockedExchange((volatile LONG*)obj, 1) != 0;
        break;

    case afxMemoryOrder_CONSUME:
    case afxMemoryOrder_ACQUIRE:
    case afxMemoryOrder_ACQ_REL:
        // These memory orders ensure proper synchronization but InterlockedExchange already ensures atomicity
        old_value = InterlockedExchange((volatile LONG*)obj, 1) != 0;
        break;

    case afxMemoryOrder_RELEASE:
        // Release semantics: Ensure that all previous writes are visible before setting the flag
        old_value = InterlockedExchange((volatile LONG*)obj, 1) != 0;
        break;

    case afxMemoryOrder_SEQ_CST:
        // Sequential consistency ordering (strongest), enforce global ordering
        old_value = InterlockedExchange((volatile LONG*)obj, 1) != 0;
        break;

    default:
        // Handle cases where memory order is not recognized (should not happen)
        break;
    }
    return old_value;
#else
    return atomic_flag_test_and_set_explicit((volatile atomic_flag*)obj, ((order == afxMemoryOrder_SEQ_CST) ? memory_order_seq_cst : (order - 1)));
#endif
}

_AFX void AfxAtomicSignalFence(afxMemoryOrder order)
{
#if USE_ATOMIC_WORKAROUND_MSVC
    (void)order;
    // Use MemoryBarrier to enforce memory ordering. 
    // This ensures that operations before the fence are not reordered after the fence.
    MemoryBarrier();
#else
    atomic_signal_fence(((order == afxMemoryOrder_SEQ_CST) ? memory_order_seq_cst : (order - 1)));
#endif
}

_AFX void AfxAtomicThreadFence(afxMemoryOrder order)
{
#if USE_ATOMIC_WORKAROUND_MSVC
    (void)order;
    // MemoryBarrier ensures that all previous memory operations are completed before
    // any subsequent operations. This provides full synchronization for the thread.
    MemoryBarrier();
#else
    atomic_thread_fence(((order == afxMemoryOrder_SEQ_CST) ? memory_order_seq_cst : (order - 1)));
#endif
}
