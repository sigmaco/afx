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
    Logarithmic Functions
    Functions that deal with logarithms.

    Natural Logarithm: ln
    Logarithm Base 10: log10
    Logarithm Base 2: log2
    General Logarithm: log (with specified base)
*/

#ifndef AFX_LOGARITHMIC_H
#define AFX_LOGARITHMIC_H

#include "qwadro/inc/math/afxVector.h"

// Returns the natural logarithm of x; returns the value y which satisfies the equation x = e^y.
// Results are undefined if x <= 0.

AFXINL afxReal64    AfxLog(afxReal64 s);
AFXINL afxReal      AfxLogf(afxReal s);

// Returns the base 2 logarithm of x; returns the value y which satisfies the equation x = 2^y.
// Results are undefined if x <= 0.

AFXINL afxReal64    AfxLog2(afxReal64 s);
AFXINL afxReal      AfxLog2f(afxReal s);

AFXINL afxReal64    AfxLog10(afxReal64 s);
AFXINL afxReal      AfxLog10f(afxReal s);


AFXINL void         AfxV2dLog(afxV2d v, afxV2d const in);
AFXINL void         AfxV3dLog(afxV3d v, afxV3d const in);
AFXINL void         AfxV4dLog(afxV4d v, afxV4d const in);

AFXINL void         AfxV2dLog2(afxV2d v, afxV2d const in);
AFXINL void         AfxV3dLog2(afxV3d v, afxV3d const in);
AFXINL void         AfxV4dLog2(afxV4d v, afxV4d const in);

AFXINL void         AfxV2dLog10(afxV2d v, afxV2d const in);
AFXINL void         AfxV3dLog10(afxV3d v, afxV3d const in);
AFXINL void         AfxV4dLog10(afxV4d v, afxV4d const in);


#endif//AFX_LOGARITHMIC_H
