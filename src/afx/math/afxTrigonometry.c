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

// This code is part of SIGMA Foundation Math <https://sigmaco.org/math>

#include "qwadro/math/afxTrigonometry.h"

_AFXINL void AfxComputeTriangleNormal(afxV3d const a, afxV3d const b, afxV3d const c, afxV3d n)
{
    afxV3d t, t2;
    AfxSubV3d(t, b, a);
    AfxSubV3d(t2, c, a);
    AfxCrossV3d(n, t, t2);
    AfxNormalizeV3d(n, n);
}

// BaryCentric

_AFXINL void AfxBarycentricV2d(afxV2d v, afxV2d const a, afxV2d const b, afxV2d const c, afxReal f, afxReal g)
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

_AFXINL void AfxBarycentricV3d(afxV3d v, afxV3d const a, afxV3d const b, afxV3d const c, afxReal f, afxReal g)
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

_AFXINL void AfxBarycentricV4d(afxV4d v, afxV4d const a, afxV4d const b, afxV4d const c, afxReal f, afxReal g)
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

_AFXINL void AfxBarycentricV2d2(afxV2d v, afxV2d const a, afxV2d const b, afxV2d const c, afxV2d const f, afxV2d const g)
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

_AFXINL void AfxBarycentricV3d2(afxV3d v, afxV3d const a, afxV3d const b, afxV3d const c, afxV3d const f, afxV3d const g)
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

_AFXINL void AfxBarycentricV4d2(afxV4d v, afxV4d const a, afxV4d const b, afxV4d const c, afxV4d const f, afxV4d const g)
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
