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
 *                       (c) 2017 SIGMA, Engitech, Scitech, Serpro
 *                             <https://sigmaco.org/qwadro/>
 */

#include "qwadro/core/afxSystem.h"

#ifdef AFX_OS_WIN
#   define WIN32_LEAN_AND_MEAN 1
#   include <Windows.h>
#endif

_AFXINL void AfxStoreAtom32(afxAtom32* dst, afxInt32 value) { InterlockedExchange(dst, value); }
_AFXINL afxInt32 AfxLoadAtom32(afxAtom32* src) { return *src; }
_AFXINL afxInt32 AfxIncAtom32(afxAtom32* val) { return InterlockedExchangeAdd(val, 1) + 1; }
_AFXINL afxInt32 AfxDecAtom32(afxAtom32* val) { return InterlockedExchangeAdd(val, -1) - 1; }
_AFXINL afxInt32 AfxExchangeAtom32(afxAtom32* dst, afxInt32 value) { return InterlockedExchange(dst, value); }
_AFXINL afxInt32 AfxAddAtom32(afxAtom32* dst, afxInt32 value) { return InterlockedExchangeAdd(dst, value) + value; }
_AFXINL afxInt32 AfxSubAtom32(afxAtom32* dst, afxInt32 value) { return InterlockedExchangeAdd(dst, -value) - value; }
_AFXINL afxInt32 AfxAndAtom32(afxAtom32* dst, afxInt32 value) { return InterlockedAnd(dst, value) & value; }
_AFXINL afxInt32 AfxOrAtom32(afxAtom32* dst, afxInt32 value) { return InterlockedOr(dst, value) | value; }
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
_AFXINL afxBool AfxCasAtom64(afxAtom64* dst, afxInt64* expected, afxInt64 proposed) { return InterlockedCompareExchange64(dst, proposed, *expected) == *expected ? 1 : 0; }

_AFXINL void* AfxLoadAtomPtr(afxAtomPtr* src) { return (void*)*src; }

#ifdef AFX_OS_WIN64
_AFXINL void AfxStoreAtomPtr(afxAtomPtr* dst, void* value) { InterlockedExchangePointer((volatile PVOID*)dst, value); }
_AFXINL void* AfxExchangeAtomPtr(afxAtomPtr* dst, void* value) { return InterlockedExchangePointer((volatile PVOID*)dst, value); }
_AFXINL afxBool AfxCasAtomPtr(afxAtomPtr* dst, void* expected, void* proposed) { return InterlockedCompareExchangePointer((volatile PVOID*)dst, proposed, expected) == expected ? 1 : 0; }
#else
_AFXINL void AfxStoreAtomPtr(afxAtomPtr* dst, void* value) { InterlockedExchangePointer((void**)dst, value); }
_AFXINL void* AfxExchangeAtomPtr(afxAtomPtr* dst, void* value) { return InterlockedExchangePointer((void**)dst, value); }
_AFXINL afxBool AfxCasAtomPtr(afxAtomPtr* dst, void** expected, void* proposed) { return InterlockedCompareExchangePointer((void**)dst, proposed, *expected) == *expected ? 1 : 0; }
#endif
