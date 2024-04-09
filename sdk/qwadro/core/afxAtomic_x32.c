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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

#include "../dep/tinycthread/tinycthread.h"
#include "qwadro/core/afxSystem.h"

_AFXINL int32_t AfxLoadAtom32(afxAtom32* pSrc) { int32_t ret; __atomic_load((int32_t*)pSrc, &ret, __ATOMIC_ACQUIRE); return ret; }
_AFXINL void AfxStoreAtom32(afxAtom32* pDest, int32_t value) { __atomic_store((int32_t*)pDest, &value, __ATOMIC_SEQ_CST); }
_AFXINL int32_t AfxIncAtom32(afxAtom32* pValue) { return __sync_add_and_fetch((int32_t*)pValue, 1); }
_AFXINL int32_t AfxDecAtom32(afxAtom32* pValue) { return __sync_sub_and_fetch((int32_t*)pValue, 1); }
_AFXINL int32_t AfxExchangeAtom32(afxAtom32* pDest, int32_t value) { return __sync_lock_test_and_set((int32_t*)pDest, value); }
_AFXINL int32_t AfxAddAtom32(afxAtom32* pDest, int32_t value) { return __sync_add_and_fetch((int32_t*)pDest, value); }
_AFXINL int32_t AfxSubAtom32(afxAtom32* pDest, int32_t value) { return __sync_sub_and_fetch((int32_t*)pDest, value); }
_AFXINL int32_t AfxAndAtom32(afxAtom32* pDest, int32_t value) { return __sync_and_and_fetch((int32_t*)pDest, value); }
_AFXINL int32_t AfxOrAtom32(afxAtom32* pDest, int32_t value) { return __sync_or_and_fetch((int32_t*)pDest, value); }
_AFXINL int AfxCasAtom32(afxAtom32* pDest, int32_t proposed, int32_t expected) { return __sync_bool_compare_and_swap((int32_t*)pDest, expected, proposed); }

#if defined( AFX_ISA_X86 ) || ( defined( AFX_ISA_ARM ) && ( AFX_ISA_ARM <= 7 ) )
_AFXINL int64_t AfxLoadAtom64(afxAtom64* pSrc) { return __sync_val_compare_and_swap(pSrc, 0, 0); }
_AFXINL void AfxStoreAtom64(afxAtom64* pDest, int64_t value) { int64_t tmp; do { tmp = *pDest; } while (__sync_val_compare_and_swap(pDest, tmp, value) != tmp); }
#else
_AFXINL int64_t AfxLoadAtom64(afxAtom64* pSrc) { int64_t ret; __atomic_load((int64_t*)pSrc, &ret, __ATOMIC_ACQUIRE); return ret; }
_AFXINL void AfxStoreAtom64(afxAtom64* pDest, int64_t value) { __atomic_store((int64_t*)pDest, &value, __ATOMIC_SEQ_CST); }
#endif

_AFXINL int64_t AfxIncAtom64(afxAtom64* pValue) { return __sync_add_and_fetch((int64_t*)pValue, 1); }
_AFXINL int64_t AfxDecAtom64(afxAtom64* pValue) { return __sync_sub_and_fetch((int64_t*)pValue, 1); }
_AFXINL int64_t AfxExchangeAtom64(afxAtom64* pDest, int64_t value) { return __sync_lock_test_and_set((int64_t*)pDest, value); }
_AFXINL int64_t AfxAddAtom64(afxAtom64* pDest, int64_t value) { return __sync_add_and_fetch((int64_t*)pDest, value); }
_AFXINL int64_t AfxSubAtom64(afxAtom64* pDest, int64_t value) { return __sync_sub_and_fetch((int64_t*)pDest, value); }
_AFXINL int64_t AfxAndAtom64(afxAtom64* pDest, int64_t value) { return __sync_and_and_fetch((int64_t*)pDest, value); }
_AFXINL int64_t AfxOrAtom64(afxAtom64* pDest, int64_t value) { return __sync_or_and_fetch((int64_t*)pDest, value); }
_AFXINL int AfxCasAtom64(afxAtom64* pDest, int64_t proposed, int64_t expected) { return __sync_bool_compare_and_swap((int64_t*)pDest, expected, proposed); }

_AFXINL void* AfxLoadAtomPtr(afxAtomPtr* pSrc) { size_t ret; __atomic_load((size_t*)pSrc, &ret, __ATOMIC_ACQUIRE); return (void*)ret; }
_AFXINL void AfxStoreAtomPtr(afxAtomPtr* pDest, void* value) { __atomic_store((size_t*)pDest, (size_t*)&value, __ATOMIC_SEQ_CST); }
_AFXINL void* AfxExchangeAtomPtr(afxAtomPtr* pDest, void* value) { return (void*)__sync_lock_test_and_set((void**)pDest, value); }
_AFXINL int AfxCasAtomPtr(afxAtomPtr* pDest, void* proposed, void* expected) { return __sync_bool_compare_and_swap((void**)pDest, expected, proposed); }
