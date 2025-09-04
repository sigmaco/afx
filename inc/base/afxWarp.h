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

AFX_DEFINE_UNION(afxWarp)
{
    struct { afxUnit w, h, d; };
    struct { afxInt x, y, z; };
};

//AFX_STATIC_ASSERT(sizeof(afxWarp) == AFX_SIMD_ALIGNMENT, "");

AFX_DEFINE_UNION(afxOffset)
{
    afxInt x, y, z; // , lodIdx; // padding reused to store the LOD used.
};

AFX_DEFINE_UNION(afxExtent)
{
    afxUnit w, h, d; // , lodIdx; // padding reused to store the LOD used.
};

#define AFX_WHD_ZERO (afxWarp const){ { 0, 0, 0 }  };
#define AFX_WHD_IDENTITY (afxWarp const){ { 1, 1, 1 } };

#define AfxWhd(w_, h_, d_) (afxWarp){ .w = (afxUnit)w_, .h = (afxUnit)h_, .d = (afxUnit)d_ }
#define AfxExtent(w_, h_, d_) (afxWarp){ .w = (afxUnit)w_, .h = (afxUnit)h_, .d = (afxUnit)d_ }
#define AfxOffset(x_, y_, z_) (afxWarp){ .x = (afxUnit)x_, .y = (afxUnit)y_, .z = (afxUnit)z_ }

AFXINL void     AfxWhdReset(afxWarp* whd);

AFXINL void     AfxWhdFill(afxWarp* whd, afxUnit value);

AFXINL void     AfxWhdSet(afxWarp* whd, afxUnit x, afxUnit y, afxUnit z);

AFXINL void     AfxWhdCopy(afxWarp* whd, afxWarp const in);

AFXINL void     AfxWhdMin(afxWarp* whd, afxWarp const a, afxWarp const b);
AFXINL void     AfxWhdMax(afxWarp* whd, afxWarp const a, afxWarp const b);
AFXINL void     AfxWhdClamp(afxWarp* whd, afxWarp const in, afxWarp const min, afxWarp const max);
AFXINL void     AfxWhdClampOffset(afxWarp* whd, afxWarp const origin, afxWarp const extent);

AFXINL afxBool  AfxWhdIsZero(afxWarp const whd);
AFXINL afxBool  AfxWhdAreEquals(afxWarp const whd, afxWarp const other);
AFXINL afxBool  AfxWhdIsLessThan(afxWarp const whd, afxWarp const other);
AFXINL afxBool  AfxWhdIsLessOrEqual(afxWarp const whd, afxWarp const other);

AFXINL void     AfxWhdAdd(afxWarp* whd, afxWarp const a, afxWarp const b);
AFXINL void     AfxWhdSub(afxWarp* whd, afxWarp const a, afxWarp const b);

AFXINL void     AfxWhdHalf(afxWarp* whd, afxWarp const in);
AFXINL void     AfxWhdDiv(afxWarp* whd, afxWarp const in, afxUnit div);
AFXINL void     AfxWhdMod(afxWarp* whd, afxWarp const in, afxUnit div);
AFXINL void     AfxWhdScale(afxWarp* whd, afxWarp const in, afxUnit s);

AFXINL afxUnit   AfxWhdSum(afxWarp const whd);
AFXINL afxUnit   AfxWhdMag(afxWarp const whd);
AFXINL afxUnit   AfxWhdDot(afxWarp const whd, afxWarp const other);

AFX afxError    _AfxAssertWhd(afxWarp total_, afxWarp base_, afxWarp range_, afxHere const hint);
#define         AfxAssertWhd(whd_) AFX_ASSERT3(whd_.w,whd_.h,whd_.d)

#endif//AFX_EXTENT_H
