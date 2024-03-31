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

// This section is part of SIGMA GL/2.

#ifndef AFX_COLOR_H
#define AFX_COLOR_H

#include "qwadro/draw/afxDrawDefs.h"
#include "qwadro/math/afxVector.h"

typedef afxV4d      afxSimd(afxColor);

typedef afxNat8     afxSimd(afxRgb8[3]);
typedef afxNat8     afxSimd(afxRgba8[4]);

typedef enum afxColorSpace
{
    afxColorSpace_LINEAR = AFX_BIT_OFFSET(0),
    afxColorSpace_SRGB = AFX_BIT_OFFSET(1)
} afxColorSpace;

typedef enum afxColorSwizzle
{
    afxColorSwizzle_ZERO,
    afxColorSwizzle_ONE,
    afxColorSwizzle_A,
    afxColorSwizzle_R,
    afxColorSwizzle_G,
    afxColorSwizzle_B
} afxColorSwizzle;

AFX_DEFINE_STRUCT(afxColorSwizzling)
{
    afxColorSwizzle r, g, b, a;
};

#define AfxColor(x_, y_, z_, w_) (afxColor const){ (afxReal)x_, (afxReal)y_, (afxReal)z_, (afxReal)w_ }

#define AfxPackArgb32(a, r, g, b) (afxNat32)(((a) << 24) | ((r) << 16) | ((g) << 8) | (b)))

AVX afxColorSwizzling const AFX_STD_COLOR_SWIZZLING;

AVXINL void     AfxResetColor(afxColor c);

AVXINL void     AfxCopyColor(afxColor c, afxColor const from);

AVXINL void     AfxSetColor(afxColor c, afxReal r, afxReal g, afxReal b, afxReal a);

AVXINL void     AfxAddColor(afxColor c, afxColor const a, afxColor const b);

AVXINL void     AfxSubColor(afxColor c, afxColor const a, afxColor const b);

AVXINL void     AfxScaleColor(afxColor c, afxColor const from, afxReal lambda);

AVXINL void     AfxToRgba8(afxColor const c, afxRgba8 dst);
AVXINL void     AfxFromRgba8(afxColor c, afxRgba8 const in);

#endif//AFX_COLOR_H
