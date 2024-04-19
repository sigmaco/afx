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

_AFXINL long AfxLoadAtom32(afxAtom32* src) { return *src; }
_AFXINL void AfxStoreAtom32(afxAtom32* dst, long value) { InterlockedExchange(dst, value); }
_AFXINL long AfxIncAtom32(afxAtom32* val) { return InterlockedExchangeAdd(val, 1) + 1; }
_AFXINL long AfxDecAtom32(afxAtom32* val) { return InterlockedExchangeAdd(val, -1) - 1; }
_AFXINL long AfxExchangeAtom32(afxAtom32* dst, long value) { return InterlockedExchange(dst, value); }
_AFXINL long AfxAddAtom32(afxAtom32* dst, long value) { return InterlockedExchangeAdd(dst, value) + value; }
_AFXINL long AfxSubAtom32(afxAtom32* dst, long value) { return InterlockedExchangeAdd(dst, -value) - value; }
_AFXINL long AfxAndAtom32(afxAtom32* dst, long value) { return InterlockedAnd(dst, value) & value; }
_AFXINL long AfxOrAtom32(afxAtom32* dst, long value) { return InterlockedOr(dst, value) | value; }
_AFXINL int AfxCasAtom32(afxAtom32* dst, long proposed, long expected) { return InterlockedCompareExchange(dst, proposed, expected) == expected ? 1 : 0; }

#ifdef AFX_OS_WIN64
_AFXINL long long AfxLoadAtom64(afxAtom64* src) { return *src; }
#else
_AFXINL long long AfxLoadAtom64(afxAtom64* src) { return InterlockedCompareExchange64(src, 0, 0); }
#endif

_AFXINL void AfxStoreAtom64(afxAtom64* dst, long long value) { InterlockedExchange64(dst, value); }
_AFXINL long long AfxIncAtom64(afxAtom64* val) { return InterlockedExchangeAdd64(val, 1) + 1; }
_AFXINL long long AfxDecAtom64(afxAtom64* val) { return InterlockedExchangeAdd64(val, -1) - 1; }
_AFXINL long long AfxExchangeAtom64(afxAtom64* dst, long long value) { return InterlockedExchange64(dst, value); }
_AFXINL long long AfxAddAtom64(afxAtom64* dst, long long value) { return InterlockedExchangeAdd64(dst, value) + value; }
_AFXINL long long AfxSubAtom64(afxAtom64* dst, long long value) { return InterlockedExchangeAdd64(dst, -value) - value; }
_AFXINL long long AfxAndAtom64(afxAtom64* dst, long long value) { return InterlockedAnd64(dst, value) & value; }
_AFXINL long long AfxOrAtom64(afxAtom64* dst, long long value) { return InterlockedOr64(dst, value) | value; }
_AFXINL int AfxCasAtom64(afxAtom64* dst, long long proposed, long long expected) { return InterlockedCompareExchange64(dst, proposed, expected) == expected ? 1 : 0; }

_AFXINL void* AfxLoadAtomPtr(afxAtomPtr* src) { return (void*)*src; }

#ifdef AFX_OS_WIN64
_AFXINL void AfxStoreAtomPtr(afxAtomPtr* dst, void* value) { InterlockedExchangePointer((volatile PVOID*)dst, value); }
_AFXINL void* AfxExchangeAtomPtr(afxAtomPtr* dst, void* value) { return InterlockedExchangePointer((volatile PVOID*)dst, value); }
_AFXINL int AfxCasAtomPtr(afxAtomPtr* dst, void* proposed, void* expected) { return InterlockedCompareExchangePointer((volatile PVOID*)dst, proposed, expected) == expected ? 1 : 0; }
#else
_AFXINL void AfxStoreAtomPtr(afxAtomPtr* dst, void* value) { InterlockedExchangePointer((void**)dst, value); }
_AFXINL void* AfxExchangeAtomPtr(afxAtomPtr* dst, void* value) { return InterlockedExchangePointer((void**)dst, value); }
_AFXINL int AfxCasAtomPtr(afxAtomPtr* dst, void* proposed, void* expected) { return InterlockedCompareExchangePointer((void**)dst, proposed, expected) == expected ? 1 : 0; }
#endif
