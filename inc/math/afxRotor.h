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

#ifndef AFX_ROTOR_H
#define AFX_ROTOR_H

#include "qwadro/inc/math/afxQuaternion.h"

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

AFXINL void AfxMakeRotor(afxRotor r, afxV3d const from, afxV3d const to);

AFXINL void AfxMakeRotorFromBiv3d(afxRotor r, afxV3d const plane, afxReal /*rad*/angle);

AFXINL void AfxMultiplyRotor(afxRotor r, afxRotor const a, afxRotor const b);

AFXINL void AfxRotateVectorByRotor(afxV3d v, afxRotor const p, afxV3d const x);

AFXINL void AfxConjugateRotor(afxRotor r, afxRotor const in);

AFXINL afxReal afxMagRotorSq(afxRotor const r);

AFXINL afxReal afxMagRotor(afxRotor const r);

AFXINL void AfxNormalizeRotor(afxRotor r, afxRotor const in);

AFXINL void AfxMakeM3dFromRotor(afxM3d m, afxRotor const r);

AFXINL void AfxRotateRotor(afxRotor r, afxRotor const other);

AFXINL void AfxGeoRotor(afxRotor r, afxV3d const a, afxV3d const b);

#endif//AFX_ROTOR_H
