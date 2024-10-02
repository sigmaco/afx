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

#ifndef AFX_EXTENT_H
#define AFX_EXTENT_H

#include "qwadro/inc/math/afxScalar.h"

AFX afxWhd const AFX_WHD_ZERO;
AFX afxWhd const AFX_WHD_IDENTITY;

#define AfxWhd(x_, y_, z_) (afxWhd const){ (afxNat)x_, (afxNat)y_, (afxNat)z_ }

AFXINL void     AfxWhdReset(afxWhd whd);

AFXINL void     AfxWhdFill(afxWhd whd, afxNat value);

AFXINL void     AfxWhdSet(afxWhd whd, afxNat x, afxNat y, afxNat z);

AFXINL void     AfxWhdCopy(afxWhd whd, afxWhd const in);

AFXINL void     AfxWhdMin(afxWhd whd, afxWhd const a, afxWhd const b);
AFXINL void     AfxWhdMax(afxWhd whd, afxWhd const a, afxWhd const b);
AFXINL void     AfxWhdClamp(afxWhd whd, afxWhd const in, afxWhd const min, afxWhd const max);
AFXINL void     AfxWhdClampOffset(afxWhd whd, afxWhd const origin, afxWhd const extent);

AFXINL afxBool  AfxWhdIsZero(afxWhd const whd);
AFXINL afxBool  AfxWhdAreEquals(afxWhd const whd, afxWhd const other);
AFXINL afxBool  AfxWhdIsLessThan(afxWhd const whd, afxWhd const other);
AFXINL afxBool  AfxWhdIsLessOrEqual(afxWhd const whd, afxWhd const other);

AFXINL void     AfxWhdAdd(afxWhd whd, afxWhd const a, afxWhd const b);
AFXINL void     AfxWhdSub(afxWhd whd, afxWhd const a, afxWhd const b);

AFXINL void     AfxWhdHalf(afxWhd whd, afxWhd const in);
AFXINL void     AfxWhdDiv(afxWhd whd, afxWhd const in, afxNat div);
AFXINL void     AfxWhdMod(afxWhd whd, afxWhd const in, afxNat div);
AFXINL void     AfxWhdScale(afxWhd whd, afxWhd const in, afxNat s);

AFXINL afxNat   AfxWhdSum(afxWhd const whd);
AFXINL afxNat   AfxWhdMag(afxWhd const whd);
AFXINL afxNat   AfxWhdDot(afxWhd const whd, afxWhd const other);

AFX afxError    _AfxAssertWhd(afxWhd total_, afxWhd base_, afxWhd range_, afxHere const hint);
#define         AfxAssertWhd(whd_) AfxAssert4(whd_,whd_[0],whd_[1],whd_[2])

#endif//AFX_EXTENT_H
