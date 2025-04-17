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

/*
    Exponential Functions
    Functions that compute the exponential of a number.

    Exponential: exp, exp2, exp10 (depending on the base)

    Logarithmic Exponential: log1p (for log(1+x))
*/

#ifndef AFX_EXPONENTIAL_H
#define AFX_EXPONENTIAL_H

#include "qwadro/inc/math/afxVector.h"


// Returns the natural exponentiation of x; e^x.

AFXINL afxReal64    AfxExp(afxReal64 s);
AFXINL afxReal      AfxExpf(afxReal s);

// Returns 2 raised to the x power; 2^x.

AFXINL afxReal64    AfxExp2(afxReal64 s);
AFXINL afxReal      AfxExp2f(afxReal s);

AFXINL afxReal64    AfxExp10(afxReal64 s);
AFXINL afxReal      AfxExp10f(afxReal s);

AFXINL void         AfxV2dExp(afxV2d v, afxV2d const in);
AFXINL void         AfxV3dExp(afxV3d v, afxV3d const in);
AFXINL void         AfxV4dExp(afxV4d v, afxV4d const in);

AFXINL void         AfxV2dExp2(afxV2d v, afxV2d const in);
AFXINL void         AfxV3dExp2(afxV3d v, afxV3d const in);
AFXINL void         AfxV4dExp2(afxV4d v, afxV4d const in);

AFXINL void         AfxV2dExp10(afxV2d v, afxV2d const in);
AFXINL void         AfxV3dExp10(afxV3d v, afxV3d const in);
AFXINL void         AfxV4dExp10(afxV4d v, afxV4d const in);

 // Returns x raised to the y power; x^y.
 // Results are undefined if x < 0 or if x = 0 and y <= 0.

AFXINL afxReal64    AfxPow(afxReal64 base, afxReal64 exp);
AFXINL afxReal      AfxPowf(afxReal base, afxReal exp);

AFXINL void         AfxV2dPow(afxV2d v, afxV2d const in);
AFXINL void         AfxV3dPow(afxV3d v, afxV3d const in);
AFXINL void         AfxV4dPow(afxV4d v, afxV4d const in);


#endif//AFX_EXPONENTIAL_H
