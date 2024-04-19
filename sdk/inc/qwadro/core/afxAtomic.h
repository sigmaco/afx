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

#ifndef AFX_ATOMIC_H
#define AFX_ATOMIC_H

#include "qwadro/core/afxDebug.h"

#ifdef AFX_OS_WIN
typedef volatile long       afxAtom32;
typedef volatile long long  afxAtom64;
typedef volatile void*      afxAtomPtr;
#else // LINUX
typedef volatile    int32_t	afxAtom32;
typedef volatile    int64_t	afxAtom64;
typedef volatile    void*   afxAtomPtr;
#endif

static_assert(sizeof(afxAtom32) == 4, "");
static_assert(sizeof(afxAtom64) == 8, "");

// 32-bit wide

AFXINL long         AfxLoadAtom32(afxAtom32* src);
AFXINL void         AfxStoreAtom32(afxAtom32* dst, long val);

AFXINL long         AfxIncAtom32(afxAtom32* val);
AFXINL long         AfxAddAtom32(afxAtom32* dst, long val);

AFXINL long         AfxDecAtom32(afxAtom32* val);
AFXINL long         AfxSubAtom32(afxAtom32* dst, long val);

AFXINL long         AfxExchangeAtom32(afxAtom32* dst, long val);
AFXINL long         AfxAndAtom32(afxAtom32* dst, long val);
AFXINL long         AfxOrAtom32(afxAtom32* dst, long val);
AFXINL int          AfxCasAtom32(afxAtom32* dst, long proposed, long expected);

// 64-bit wide

AFXINL long long    AfxLoadAtom64(afxAtom64* src);
AFXINL void         AfxStoreAtom64(afxAtom64* dsr, long long val);

AFXINL long long    AfxIncAtom64(afxAtom64* val);
AFXINL long long    AfxAddAtom64(afxAtom64* dst, long long val);
AFXINL long long    AfxDecAtom64(afxAtom64* val);
AFXINL long long    AfxSubAtom64(afxAtom64* dst, long long val);

AFXINL long long    AfxExchangeAtom64(afxAtom64* dst, long long val);
AFXINL long long    AfxAndAtom64(afxAtom64* dst, long long val);
AFXINL long long    AfxOrAtom64(afxAtom64* dst, long long val);
AFXINL int          AfxCasAtom64(afxAtom64* dst, long long proposed, long long expected);

// address wide

AFXINL void*        AfxLoadAtomPtr(afxAtomPtr* src);
AFXINL void         AfxStoreAtomPtr(afxAtomPtr* dst, void* val);
AFXINL void*        AfxExchangeAtomPtr(afxAtomPtr* dst, void* val);
AFXINL int          AfxCasAtomPtr(afxAtomPtr* dst, void* proposed, void* expected);

#endif//AFX_ATOMIC_H
