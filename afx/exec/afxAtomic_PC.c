/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#endif

#include "../dev/afxDevCoreBase.h"

#ifdef AFX_OS_WIN

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

#ifdef AFX_OS_WIN64
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

#ifdef AFX_OS_WIN64
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

#if defined( AFX_ISA_X86 ) || ( defined( AFX_ISA_ARM ) && ( AFX_ISA_ARM <= 7 ) )
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
