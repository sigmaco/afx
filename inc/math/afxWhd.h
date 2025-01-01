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

AFX_DEFINE_UNION(afxWhd)
{
    struct { afxUnit w, h, d; };
    struct { afxUnit x, y, z; };
};

//AFX_STATIC_ASSERT(sizeof(afxWhd) == AFX_SIMD_ALIGNMENT, "");

AFX_DEFINE_UNION(afxOffset)
{
    afxInt x, y, z; // , lodIdx; // padding reused to store the LOD used.
};

AFX_DEFINE_UNION(afxExtent)
{
    afxUnit w, h, d; // , lodIdx; // padding reused to store the LOD used.
};

#define AFX_WHD_ZERO (afxWhd const){ { 0, 0, 0 }  };
#define AFX_WHD_IDENTITY (afxWhd const){ { 1, 1, 1 } };

#define AfxWhd(w_, h_, d_) (afxWhd){ .w = (afxUnit)w_, .h = (afxUnit)h_, .d = (afxUnit)d_ }
#define AfxExtent(w_, h_, d_) (afxWhd){ .w = (afxUnit)w_, .h = (afxUnit)h_, .d = (afxUnit)d_ }
#define AfxOffset(x_, y_, z_) (afxWhd){ .x = (afxUnit)x_, .y = (afxUnit)y_, .z = (afxUnit)z_ }

AFXINL void     AfxWhdReset(afxWhd* whd);

AFXINL void     AfxWhdFill(afxWhd* whd, afxUnit value);

AFXINL void     AfxWhdSet(afxWhd* whd, afxUnit x, afxUnit y, afxUnit z);

AFXINL void     AfxWhdCopy(afxWhd* whd, afxWhd const in);

AFXINL void     AfxWhdMin(afxWhd* whd, afxWhd const a, afxWhd const b);
AFXINL void     AfxWhdMax(afxWhd* whd, afxWhd const a, afxWhd const b);
AFXINL void     AfxWhdClamp(afxWhd* whd, afxWhd const in, afxWhd const min, afxWhd const max);
AFXINL void     AfxWhdClampOffset(afxWhd* whd, afxWhd const origin, afxWhd const extent);

AFXINL afxBool  AfxWhdIsZero(afxWhd const whd);
AFXINL afxBool  AfxWhdAreEquals(afxWhd const whd, afxWhd const other);
AFXINL afxBool  AfxWhdIsLessThan(afxWhd const whd, afxWhd const other);
AFXINL afxBool  AfxWhdIsLessOrEqual(afxWhd const whd, afxWhd const other);

AFXINL void     AfxWhdAdd(afxWhd* whd, afxWhd const a, afxWhd const b);
AFXINL void     AfxWhdSub(afxWhd* whd, afxWhd const a, afxWhd const b);

AFXINL void     AfxWhdHalf(afxWhd* whd, afxWhd const in);
AFXINL void     AfxWhdDiv(afxWhd* whd, afxWhd const in, afxUnit div);
AFXINL void     AfxWhdMod(afxWhd* whd, afxWhd const in, afxUnit div);
AFXINL void     AfxWhdScale(afxWhd* whd, afxWhd const in, afxUnit s);

AFXINL afxUnit   AfxWhdSum(afxWhd const whd);
AFXINL afxUnit   AfxWhdMag(afxWhd const whd);
AFXINL afxUnit   AfxWhdDot(afxWhd const whd, afxWhd const other);

AFX afxError    _AfxAssertWhd(afxWhd total_, afxWhd base_, afxWhd range_, afxHere const hint);
#define         AfxAssertWhd(whd_) AfxAssert3(whd_.w,whd_.h,whd_.d)

#endif//AFX_EXTENT_H
