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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AFX_COLOR_H
#define AFX_COLOR_H

#include "qwadro/draw/afxDrawDefs.h"
#include "qwadro/math/afxVector.h"

typedef enum afxColorChannel
{
    afxColorChannel_R,
    afxColorChannel_G,
    afxColorChannel_B,
    afxColorChannel_A
}afxColorChannel;

typedef afxV4d      afxSimd(afxColor);

typedef afxNat8     afxSimd(afxRgb8[3]);
typedef afxNat8     afxSimd(afxRgba8[4]);

typedef enum afxColorSpace
{
    afxColorSpace_LINEAR = 1,
    afxColorSpace_SRGB
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

#define AfxPackArgb32(a, r, g, b) (afxNat32)(((a) << 24) | ((r) << 16) | ((g) << 8) | (b))

AVX afxColorSwizzling const AFX_STD_COLOR_SWIZZLING;

AVXINL void     AfxResetColor(afxColor c);

AVXINL void     AfxCopyColor(afxColor c, afxColor const from);

AVXINL void     AfxSetRgb(afxColor c, afxReal r, afxReal g, afxReal b);
AVXINL void     AfxSetRgba(afxColor c, afxReal r, afxReal g, afxReal b, afxReal a);

AVXINL void     AfxMakeRgb(afxColor col, afxByte r, afxByte g, afxByte b);
AVXINL void     AfxMakeRgba(afxColor col, afxByte r, afxByte g, afxByte b, afxByte a);

AVXINL void     AfxRepackAlpha(afxColor col, afxColor const src, afxByte a);
AVXINL void     AfxReplaceAlpha(afxColor col, afxColor const src, afxReal a);

AVXINL void     AfxAddColor(afxColor c, afxColor const a, afxColor const b);
AVXINL void     AfxSubColor(afxColor c, afxColor const a, afxColor const b);

AVXINL void     AfxScaleColor(afxColor c, afxColor const from, afxReal lambda);

AVXINL void     AfxPackRgba8(afxColor const c, afxRgba8 dst);
AVXINL void     AfxUnpackRgba8(afxColor c, afxRgba8 const in);

AVXINL void     AfxPremulAlpha(afxColor c);

AVXINL void     AfxLerpColor(afxColor col, afxColor const c0, afxColor const c1, afxReal u);

AVXINL afxReal  AfxMakeHue(afxReal h, afxReal m1, afxReal m2);
AVXINL void     AfxMakeHsl(afxColor col, afxReal h, afxReal s, afxReal l);
AVXINL void     AfxMakeHsla(afxColor col, afxReal h, afxReal s, afxReal l, afxByte a);

#endif//AFX_COLOR_H
