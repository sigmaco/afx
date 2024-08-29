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

#ifndef AFX_ATOMIC_H
#define AFX_ATOMIC_H

#include "qwadro/inc/base/afxDebug.h"

#ifdef AFX_OS_WIN
typedef volatile long       afxAtom32;
typedef volatile long long  afxAtom64;
typedef volatile void*      afxAtomPtr;
#else // LINUX
typedef volatile    int32_t	afxAtom32;
typedef volatile    int64_t	afxAtom64;
typedef volatile    void*   afxAtomPtr;
#endif

AFX_STATIC_ASSERT(sizeof(afxAtom32) == 4, "");
AFX_STATIC_ASSERT(sizeof(afxAtom64) == 8, "");

// 32-bit wide

// reads a value from an atomic object.
AFXINL afxInt32     AfxLoadAtom32(afxAtom32* src);
AFXINL afxInt64     AfxLoadAtom64(afxAtom64* src);
AFXINL void*        AfxLoadAtomPtr(afxAtomPtr* src);

// stores a value in an atomic object.
AFXINL void         AfxStoreAtom32(afxAtom32* dst, afxInt32 val);
AFXINL void         AfxStoreAtom64(afxAtom64* dsr, afxInt64 val);
AFXINL void         AfxStoreAtomPtr(afxAtomPtr* dst, void* val);

AFXINL afxInt32     AfxIncAtom32(afxAtom32* val);
AFXINL afxInt64     AfxIncAtom64(afxAtom64* val);

// atomic addition.
AFXINL afxInt32     AfxAddAtom32(afxAtom32* dst, afxInt32 val);
AFXINL afxInt64     AfxAddAtom64(afxAtom64* dst, afxInt64 val);

AFXINL afxInt32     AfxDecAtom32(afxAtom32* val);
AFXINL afxInt64     AfxDecAtom64(afxAtom64* val);

// atomic subtraction.
AFXINL afxInt32     AfxSubAtom32(afxAtom32* dst, afxInt32 val);
AFXINL afxInt64     AfxSubAtom64(afxAtom64* dst, afxInt64 val);

// swaps a value with the value of an atomic object.
AFXINL afxInt32     AfxExchangeAtom32(afxAtom32* dst, afxInt32 val);
AFXINL afxInt64     AfxExchangeAtom64(afxAtom64* dst, afxInt64 val);
AFXINL void*        AfxExchangeAtomPtr(afxAtomPtr* dst, void* val);

// atomic bitwise AND.
AFXINL afxInt32     AfxAndAtom32(afxAtom32* dst, afxInt32 val);
AFXINL afxInt64     AfxAndAtom64(afxAtom64* dst, afxInt64 val);

// atomic bitwise OR.
AFXINL afxInt32     AfxOrAtom32(afxAtom32* dst, afxInt32 val);
AFXINL afxInt64     AfxOrAtom64(afxAtom64* dst, afxInt64 val);

// swaps a value with an atomic object if the old value is what is expected, otherwise reads the old value.
AFXINL afxBool      AfxCasAtom32(afxAtom32* dst, afxInt32* expected, afxInt32 proposed);
AFXINL afxBool      AfxCasAtom64(afxAtom64* dst, afxInt64* expected, afxInt64 proposed);
AFXINL afxBool      AfxCasAtomPtr(afxAtomPtr* dst, void** expected, void* proposed);

#endif//AFX_ATOMIC_H
