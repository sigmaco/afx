/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#include "afx/math/afxMathDefs.h"
#include "afx/math/afxScalar.h"

// MakeRotation
// Memory layout:

// AddAxialRotation
// Memory layout: looks like it is row-major

_AFXINL void AfxM3dAccumulateAxialRotation(afxM3d m, afxV3d const axis, afxReal radians)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(axis);

    afxM3d tmp, tmp2;
    AfxM3dCopy(tmp, m);
    AfxM3dMakeAxialRotation(tmp2, axis, radians);
    AfxM3dMultiply(tmp2, tmp, m);
}

_AFXINL void AfxM4dAccumulateAxialRotation(afxM4d m, afxV3d const axis, afxReal radians)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(axis);

    afxM4d tmp, tmp2;
    AfxM4dCopy(tmp, m);
    AfxM4dMakeAxialRotation(tmp2, axis, radians);
    AfxM4dMultiply(tmp2, tmp, m);
}
