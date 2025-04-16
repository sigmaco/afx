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

#include "qwadro/inc/math/afxTrigonometry.h"


////////////////////////////////////////////////////////////////////////////////
// Arithmetica                                                                //
////////////////////////////////////////////////////////////////////////////////

// Add

_AFXINL void AfxV2dAdd(afxV2d v, afxV2d const a, afxV2d const b)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    v[0] = a[0] + b[0];
    v[1] = a[1] + b[1];
}

_AFXINL void AfxV3dAdd(afxV3d v, afxV3d const a, afxV3d const b)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    v[0] = a[0] + b[0];
    v[1] = a[1] + b[1];
    v[2] = a[2] + b[2];
}

_AFXINL void AfxV4dAdd(afxV4d v, afxV4d const a, afxV4d const b)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    v[0] = a[0] + b[0];
    v[1] = a[1] + b[1];
    v[2] = a[2] + b[2];
    v[3] = a[3] + b[3];
}

// Sub

_AFXINL void AfxV2dSub(afxV2d v, afxV2d const a, afxV2d const b)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    v[0] = a[0] - b[0];
    v[1] = a[1] - b[1];
}

_AFXINL void AfxV3dSub(afxV3d v, afxV3d const a, afxV3d const b)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    v[0] = a[0] - b[0];
    v[1] = a[1] - b[1];
    v[2] = a[2] - b[2];
}

_AFXINL void AfxV4dSub(afxV4d v, afxV4d const a, afxV4d const b)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    v[0] = a[0] - b[0];
    v[1] = a[1] - b[1];
    v[2] = a[2] - b[2];
    v[3] = a[3] - b[3];
}

// Div

_AFXINL void AfxDivV2d(afxV2d v, afxV2d const a, afxV2d const b)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    v[0] = a[0] / b[0];
    v[1] = a[1] / b[1];
}

_AFXINL void AfxV3dDiv(afxV3d v, afxV3d const a, afxV3d const b)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    v[0] = a[0] / b[0];
    v[1] = a[1] / b[1];
    v[2] = a[2] / b[2];
}

_AFXINL void AfxDivV4d(afxV4d v, afxV4d const a, afxV4d const b)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    v[0] = a[0] / b[0];
    v[1] = a[1] / b[1];
    v[2] = a[2] / b[2];
    v[3] = a[3] / b[3];
}

// Divs

_AFXINL void AfxV2dDivs(afxV2d v, afxV2d const a, afxReal div)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(div);
    v[0] = a[0] / div;
    v[1] = a[1] / div;
}

_AFXINL void AfxV3dDivs(afxV3d v, afxV3d const a, afxReal div)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(div);
    v[0] = a[0] / div;
    v[1] = a[1] / div;
    v[2] = a[2] / div;
}

_AFXINL void AfxV4dDivs(afxV4d v, afxV4d const a, afxReal div)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(div);
    v[0] = a[0] / div;
    v[1] = a[1] / div;
    v[2] = a[2] / div;
    v[3] = a[3] / div;
}

// Mod

_AFXINL void AfxModV2d(afxV2d v, afxV2d const a, afxV2d const b)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);

    AfxDivV2d(v, a, b);
    AfxTruncateV2d(v, v);
    AfxResubV2d(v, b, v, a);
}

_AFXINL void AfxModV3d(afxV3d v, afxV3d const a, afxV3d const b)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);

    AfxV3dDiv(v, a, b);
    AfxTruncateV3d(v, v);
    AfxResubV3d(v, b, v, a);
}

_AFXINL void AfxModV4d(afxV4d v, afxV4d const a, afxV4d const b)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);

    AfxDivV4d(v, a, b);
    AfxTruncateV4d(v, v);
    AfxResubV4d(v, b, v, a);
}

// Scale

_AFXINL void AfxV2dScale(afxV2d v, afxV2d const a, afxReal lambda)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    v[0] = a[0] * lambda;
    v[1] = a[1] * lambda;
}

_AFXINL void AfxV3dScale(afxV3d v, afxV3d const a, afxReal lambda)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    v[0] = a[0] * lambda;
    v[1] = a[1] * lambda;
    v[2] = a[2] * lambda;
}

_AFXINL void AfxV4dScale(afxV4d v, afxV4d const a, afxReal lambda)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    v[0] = a[0] * lambda;
    v[1] = a[1] * lambda;
    v[2] = a[2] * lambda;
    v[3] = a[3] * lambda;
}

// Half

_AFXINL void AfxHalfV2d(afxV2d v, afxV2d const in)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = in[0] * 0.5f;
    v[1] = in[1] * 0.5f;
}

_AFXINL void AfxHalfV3d(afxV3d v, afxV3d const in)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = in[0] * 0.5f;
    v[1] = in[1] * 0.5f;
    v[2] = in[2] * 0.5f;
}

_AFXINL void AfxHalfV4d(afxV4d v, afxV4d const in)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = in[0] * 0.5f;
    v[1] = in[1] * 0.5f;
    v[2] = in[2] * 0.5f;
    v[3] = in[3] * 0.5f;
}

// Multiply

_AFXINL void AfxV2dMultiply(afxV2d v, afxV2d const a, afxV2d const b)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    v[0] = a[0] * b[0];
    v[1] = a[1] * b[1];
}

_AFXINL void AfxV3dMultiply(afxV3d v, afxV3d const a, afxV3d const b)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    v[0] = a[0] * b[0];
    v[1] = a[1] * b[1];
    v[2] = a[2] * b[2];
}

_AFXINL void AfxV4dMultiply(afxV4d v, afxV4d const a, afxV4d const b)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    v[0] = a[0] * b[0];
    v[1] = a[1] * b[1];
    v[2] = a[2] * b[2];
    v[3] = a[3] * b[3];
}

// Mad

_AFXINL void AfxV2dMad(afxV2d v, afxV2d const a, afxV2d const b, afxV2d const c)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    AFX_ASSERT(c);
    v[0] = (a[0] * b[0]) + c[0];
    v[1] = (a[1] * b[1]) + c[1];
}

_AFXINL void AfxV3dMad(afxV3d v, afxV3d const a, afxV3d const b, afxV3d const c)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    AFX_ASSERT(c);
    v[0] = (a[0] * b[0]) + c[0];
    v[1] = (a[1] * b[1]) + c[1];
    v[2] = (a[2] * b[2]) + c[2];
}

_AFXINL void AfxV4dMad(afxV4d v, afxV4d const a, afxV4d const b, afxV4d const c)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    AFX_ASSERT(c);
    v[0] = (a[0] * b[0]) + c[0];
    v[1] = (a[1] * b[1]) + c[1];
    v[2] = (a[2] * b[2]) + c[2];
    v[3] = (a[3] * b[3]) + c[3];
}

// AddScaled

_AFXINL void AfxV2dMads(afxV2d v, afxReal lambda, afxV2d const b, afxV2d const c)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(b);
    AFX_ASSERT(c);
    v[0] = (lambda * b[0]) + c[0];
    v[1] = (lambda * b[1]) + c[1];
}

_AFXINL void AfxV3dMads(afxV3d v, afxReal lambda, afxV3d const b, afxV3d const c)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(b);
    AFX_ASSERT(c);
    v[0] = (lambda * b[0]) + c[0];
    v[1] = (lambda * b[1]) + c[1];
    v[2] = (lambda * b[2]) + c[2];
}

_AFXINL void AfxV4dMads(afxV4d v, afxReal lambda, afxV4d const b, afxV4d const c)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(b);
    AFX_ASSERT(c);
    v[0] = (lambda * b[0]) + c[0];
    v[1] = (lambda * b[1]) + c[1];
    v[2] = (lambda * b[2]) + c[2];
    v[3] = (lambda * b[3]) + c[3];
}

// Resub

_AFXINL void AfxResubV2d(afxV2d v, afxV2d const a, afxV2d const b, afxV2d const c)
{
    // Should be compatible with XMVectorNegativeMultiplySubtract

    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    AFX_ASSERT(c);

    v[0] = c[0] - (a[0] * b[0]);
    v[1] = c[1] - (a[1] * b[1]);
}

_AFXINL void AfxResubV3d(afxV3d v, afxV3d const a, afxV3d const b, afxV3d const c)
{
    // Should be compatible with XMVectorNegativeMultiplySubtract

    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    AFX_ASSERT(c);

    v[0] = c[0] - (a[0] * b[0]);
    v[1] = c[1] - (a[1] * b[1]);
    v[2] = c[2] - (a[2] * b[2]);
}

_AFXINL void AfxResubV4d(afxV4d v, afxV4d const a, afxV4d const b, afxV4d const c)
{
    // Should be compatible with XMVectorNegativeMultiplySubtract

    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    AFX_ASSERT(c);

    v[0] = c[0] - (a[0] * b[0]);
    v[1] = c[1] - (a[1] * b[1]);
    v[2] = c[2] - (a[2] * b[2]);
    v[3] = c[3] - (a[3] * b[3]);
}

// Mix
// The MIX function is conceptually similar to LERP but may involve additional functionality, such as more complex blending modes or different blending parameters. It is often used in shader programming and graphics.
// v = x * (1 - t) + y * t

_AFXINL void AfxV2dMix(afxV2d v, afxV2d const x, afxV2d const y, afxReal t)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT3(v, x, y);

    v[0] = x[0] * (1.f - t) + y[0] * t;
    v[1] = x[1] * (1.f - t) + y[1] * t;
}

_AFXINL void AfxV3dMix(afxV3d v, afxV3d const x, afxV3d const y, afxReal t)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT3(v, x, y);

    v[0] = x[0] * (1.f - t) + y[0] * t;
    v[1] = x[1] * (1.f - t) + y[1] * t;
    v[2] = x[2] * (1.f - t) + y[2] * t;
}

_AFXINL void AfxV4dMix(afxV4d v, afxV4d const x, afxV4d const y, afxReal t)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT3(v, x, y);

    v[0] = x[0] * (1.f - t) + y[0] * t;
    v[1] = x[1] * (1.f - t) + y[1] * t;
    v[2] = x[2] * (1.f - t) + y[2] * t;
    v[3] = x[3] * (1.f - t) + y[3] * t;
}

// Lerp
// LERP is a method to interpolate linearly between two values. In the context of 4D vectors, it calculates the intermediate vector between a start and end vector based on a factor t ranging from 0.0 to 1.0.
// v = x + t * (y - x)

_AFXINL void AfxV2dLerp(afxV2d v, afxV2d const x, afxV2d const y, afxReal t)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT3(v, x, y);

    v[0] = x[0] + t * (y[0] - x[0]);
    v[1] = x[1] + t * (y[1] - x[1]);
}

_AFXINL void AfxV3dLerp(afxV3d v, afxV3d const x, afxV3d const y, afxReal t)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT3(v, x, y);

    v[0] = x[0] + t * (y[0] - x[0]);
    v[1] = x[1] + t * (y[1] - x[1]);
    v[2] = x[2] + t * (y[2] - x[2]);
}

_AFXINL void AfxV4dLerp(afxV4d v, afxV4d const x, afxV4d const y, afxReal t)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT3(v, x, y);

    v[0] = x[0] + t * (y[0] - x[0]);
    v[1] = x[1] + t * (y[1] - x[1]);
    v[2] = x[2] + t * (y[2] - x[2]);
    v[3] = x[3] + t * (y[3] - x[3]);
}


