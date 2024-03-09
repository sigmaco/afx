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


#include "qwadro/math/afxTransform.h"
#include "qwadro/sim/afxSimulation.h"
#include "qwadro/math/afxMatrix.h"
#include "qwadro/sim/awxAsset.h"

_AFXINL void AfxRectZero(afxRect *rect)
{
    rect->x = 0;
    rect->y = 0;
    rect->w = 1;
    rect->h = 1;
}

_AFXINL void AfxRectCopy(afxRect *rect, afxRect const *src)
{
    rect->x = src->x;
    rect->y = src->y;
    rect->w = src->w;
    rect->h = src->h;
}

_AFXINL afxReal AfxToNdc(afxReal x, afxReal total)
{
    return x * (AfxScalar(1) / total);
}

_AFXINL afxReal AfxFromNdc(afxReal x, afxReal total)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(AfxScalar(0) <= x);
    AfxAssert(AfxScalar(1) >= x);
    return x * total;
}

_AFXINL void AfxExtractV2dInNdc(afxV2d v, afxV2d const b, afxV2d const total)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(b);
    AfxAssert(total);
    v[0] = AfxToNdc(b[0], total[0]);
    v[1] = AfxToNdc(b[1], total[1]);
}

_AFXINL void AfxExtractV2dOutNdc(afxV2d v, afxV2d const b, afxV2d const total)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(b);
    AfxAssert(total);
    v[0] = AfxFromNdc(b[0], total[0]);
    v[1] = AfxFromNdc(b[1], total[1]);
}

_AFXINL void AfxMakeV2dNdc(afxV2d v, afxV2d const total)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(total);
    AfxAssert(v != total);
    v[0] = AfxToNdc(v[0], total[0]);
    v[1] = AfxToNdc(v[1], total[1]);
}

_AFXINL void AfxUnmakeV2dNdc(afxV2d v, afxV2d const total)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(total);
    AfxAssert(v != total);
    v[0] = AfxFromNdc(v[0], total[0]);
    v[1] = AfxFromNdc(v[1], total[1]);
}
