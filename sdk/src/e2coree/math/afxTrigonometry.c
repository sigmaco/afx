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

#include "qwadro/math/afxTrigonometry.h"

_AFXINL void AfxComputeTriangleNormal(afxReal const a[3], afxReal const b[3], afxReal const c[3], afxReal n[3])
{
    afxV3d t, t2;
    AfxSubV3d(t, b, a);
    AfxSubV3d(t2, c, a);
    AfxCrossV3d(n, t, t2);
    AfxNormalV3d(n, n);
}

// BaryCentric

_AFXINL void AfxBarycentricV2d(afxReal v[2], afxReal const a[2], afxReal const b[2], afxReal const c[2], afxReal f, afxReal g)
{
    // Result = Position0 + f * (Position1 - Position0) + g * (Position2 - Position0)
    // Adapted from DirectXMath. Works?

    afxV2d p10;
    AfxSubV2d(p10, b, a);

    afxV2d fv;
    AfxFillV2d(fv, f);

    afxV2d p20;
    AfxSubV2d(p20, c, a);

    afxV2d gv;
    AfxFillV2d(gv, g);

    afxV2d tmp;
    AfxMadV2d(tmp, p10, fv, a);
    AfxMadV2d(v, p20, gv, tmp);
}

_AFXINL void AfxBarycentricV3d(afxReal v[3], afxReal const a[3], afxReal const b[3], afxReal const c[3], afxReal f, afxReal g)
{
    // Result = Position0 + f * (Position1 - Position0) + g * (Position2 - Position0)
    // From DirectXMath
    // https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxvec4barycentric

    afxV3d p10;
    AfxSubV3d(p10, b, a);

    afxV3d fv;
    AfxFillV3d(fv, f);

    afxV3d p20;
    AfxSubV3d(p20, c, a);

    afxV3d gv;
    AfxFillV3d(gv, g);

    afxV3d tmp;
    AfxMadV3d(tmp, p10, fv, a);
    AfxMadV3d(v, p20, gv, tmp);
}

_AFXINL void AfxBarycentricV4d(afxReal v[4], afxReal const a[4], afxReal const b[4], afxReal const c[4], afxReal f, afxReal g)
{
    // Result = Position0 + f * (Position1 - Position0) + g * (Position2 - Position0)
    // From DirectXMath
    // https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxvec4barycentric

    afxV4d p10;
    AfxSubV4d(p10, b, a);

    afxV4d fv;
    AfxFillV4d(fv, f);

    afxV4d p20;
    AfxSubV4d(p20, c, a);

    afxV4d gv;
    AfxFillV4d(gv, g);

    afxV4d tmp;
    AfxMadV4d(tmp, p10, fv, a);
    AfxMadV4d(v, p20, gv, tmp);
}

// BaryCentric2

_AFXINL void AfxBarycentricV2d2(afxReal v[2], afxReal const a[2], afxReal const b[2], afxReal const c[2], afxReal const f[2], afxReal const g[2])
{
    // Result = Position0 + f * (Position1 - Position0) + g * (Position2 - Position0)
    // From DirectXMath

    afxV2d p10;
    AfxSubV2d(p10, b, a);

    afxV2d p20;
    AfxSubV2d(p20, c, a);

    afxV2d tmp;
    AfxMadV2d(tmp, p10, f, a);
    AfxMadV2d(v, p20, g, tmp);
}

_AFXINL void AfxBarycentricV3d2(afxReal v[3], afxReal const a[3], afxReal const b[3], afxReal const c[3], afxReal const f[3], afxReal const g[3])
{
    // Result = Position0 + f * (Position1 - Position0) + g * (Position2 - Position0)
    // From DirectXMath

    afxV3d p10;
    AfxSubV3d(p10, b, a);

    afxV3d p20;
    AfxSubV3d(p20, c, a);

    afxV3d tmp;
    AfxMadV3d(tmp, p10, f, a);
    AfxMadV3d(v, p20, g, tmp);
}

_AFXINL void AfxBarycentricV4d2(afxReal v[4], afxReal const a[4], afxReal const b[4], afxReal const c[4], afxReal const f[4], afxReal const g[4])
{
    // Result = Position0 + f * (Position1 - Position0) + g * (Position2 - Position0)
    // From DirectXMath

    afxV4d p10;
    AfxSubV4d(p10, b, a);

    afxV4d p20;
    AfxSubV4d(p20, c, a);

    afxV4d tmp;
    AfxMadV4d(tmp, p10, f, a);
    AfxMadV4d(v, p20, g, tmp);
}
