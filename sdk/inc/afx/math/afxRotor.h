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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_ROTOR_H
#define AFX_ROTOR_H

#include "afxMathDefs.h"

AFX afxRotor AFX_ROTOR_IDENTITY;

// Bivector

AFXINL void AfxSetBiv3d(afxV3d biv, afxReal b01, afxReal b02, afxReal b12);

AFXINL void AfxWedgeBiv3d(afxV3d biv, afxV3d const u, afxV3d const v);

// Rotor

AFXINL void AfxResetRotor(afxRotor r);

AFXINL void AfxZeroRotor(afxRotor r);

AFXINL void AfxSetRotor(afxRotor r, afxReal b01, afxReal b02, afxReal b12, afxReal a);

AFXINL void AfxResetRotor(afxRotor r);

AFXINL void AfxZeroRotor(afxRotor r);

AFXINL void AfxSetRotor(afxRotor r, afxReal b01, afxReal b02, afxReal b12, afxReal a);

AFXINL void AfxMakeRotor(afxRotor r, afxReal const from[3], afxReal const to[3]);

AFXINL void AfxMakeRotorFromBiv3d(afxRotor r, afxV3d const plane, afxReal /*rad*/angle);

AFXINL void AfxMultiplyRotor(afxRotor r, afxRotor const a, afxRotor const b);

AFXINL void AfxRotateVectorByRotor(afxV3d v, afxRotor const p, afxReal const x[3]);

AFXINL void AfxConjugateRotor(afxRotor r, afxRotor const in);

AFXINL afxReal afxMagRotorSq(afxRotor const r);

AFXINL afxReal afxMagRotor(afxRotor const r);

AFXINL void AfxNormalizeRotor(afxRotor r, afxRotor const in);

AFXINL void AfxM3dFromRotor(afxM3d m, afxRotor const r);

AFXINL void AfxRotateRotor(afxRotor r, afxRotor const other);

AFXINL void AfxGeoRotor(afxRotor r, afxV3d const a, afxV3d const b);

#endif//AFX_ROTOR_H