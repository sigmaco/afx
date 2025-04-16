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

// This code is part of SIGMA Advanced Math Extensions for Qwadro

#ifndef AFX_SCALAR_H
#define AFX_SCALAR_H

#include "qwadro/inc/math/afxMathDefs.h"

AFXINL afxBool      AfxIsGreaterThanEpsilon(afxReal value);

AFXINL afxBool      AfxIsNan(afxReal value);

AFXINL afxBool      AfxIsInfinite(afxReal value);

AFXINL afxBool      AfxRealIsEquivalent(afxReal a, afxReal b);

AFXINL afxBool      AfxRealIsEqual(afxReal a, afxReal b, afxReal epsilon);

AFXINL afxReal64    AfxRecip(afxReal64 value);
AFXINL afxReal      AfxRecipf(afxReal value);

AFXINL afxReal64    AfxLerp(afxReal64 a, afxReal64 b, afxReal64 time);
AFXINL afxReal      AfxLerpf(afxReal a, afxReal b, afxReal time);

AFXINL afxReal64    AfxClampd(afxReal64 value, afxReal64 min, afxReal64 max);
AFXINL afxReal      AfxClampf(afxReal value, afxReal min, afxReal max);

AFXINL afxReal64    AfxMod(afxReal64 x, afxReal64 y);
AFXINL afxReal      AfxModf(afxReal x, afxReal y);

AFXINL afxReal64    AfxSq(afxReal64 s);
AFXINL afxReal      AfxSqf(afxReal s);

AFXINL afxReal64    AfxFloor(afxReal64 s);
AFXINL afxReal      AfxFloorf(afxReal s);

AFXINL afxReal64    AfxCeil(afxReal64 s);
AFXINL afxReal      AfxCeilf(afxReal s);


// Returns sqrt(x).
// Results are undefined if x < 0.

AFXINL afxReal64    AfxSqrt(afxReal64 s);
AFXINL afxReal      AfxSqrtf(afxReal s);

// Returns 1 / sqrt(x).
// Results are undefined if x <= 0.

AFXINL afxReal64    AfxRsqrt(afxReal64 s);
AFXINL afxReal      AfxRsqrtf(afxReal s);

AFXINL afxReal64    AfxNdc(afxReal64 x, afxReal64 total);
AFXINL afxReal      AfxNdcf(afxReal x, afxReal total);

AFXINL afxReal64    AfxUnndc(afxReal64 x, afxReal64 total);
AFXINL afxReal      AfxUnndcf(afxReal x, afxReal total);

AFXINL afxInt16     AfxReal32ToSnorm(afxReal v);

#endif//AFX_SCALAR_H
