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

#include "qwadro/inc/math/afxExponential.h"

/***********************************************************************
 * EXPONENTIAL FUNCTIONS                                               *
 *                                                                     *
 * These all operate component-wise. The description is per component. *
 ***********************************************************************/

// Returns the natural exponentiation of x; e^x.

_AFXINL afxReal64 AfxExp(afxReal64 s)
{
    return exp(s);
}

_AFXINL afxReal AfxExpf(afxReal s)
{
    return expf(s);
}

// Returns 2 raised to the x power; 2^x.

_AFXINL afxReal64 AfxExp2(afxReal64 s)
{
    return exp2(s);
}

_AFXINL afxReal AfxExp2f(afxReal s)
{
    return exp2f(s);
}

_AFXINL afxReal64 AfxExp10(afxReal64 s)
{
    return AfxPow(10.0, s);
}

_AFXINL afxReal AfxExp10f(afxReal s)
{
    return AfxPowf(10.f, s);
}

// Exp

_AFXINL void AfxV2dExp(afxV2d v, afxV2d const in)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxExpf(in[0]);
    v[1] = AfxExpf(in[1]);
}

_AFXINL void AfxV3dExp(afxV3d v, afxV3d const in)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxExpf(in[0]);
    v[1] = AfxExpf(in[1]);
    v[2] = AfxExpf(in[2]);
}

_AFXINL void AfxV4dExp(afxV4d v, afxV4d const in)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxExpf(in[0]);
    v[1] = AfxExpf(in[1]);
    v[2] = AfxExpf(in[2]);
    v[3] = AfxExpf(in[3]);
}

// Exp2

_AFXINL void AfxV2dExp2(afxV2d v, afxV2d const in)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxExp2f(in[0]);
    v[1] = AfxExp2f(in[1]);
}

_AFXINL void AfxV3dExp2(afxV3d v, afxV3d const in)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxExp2f(in[0]);
    v[1] = AfxExp2f(in[1]);
    v[2] = AfxExp2f(in[2]);
}

_AFXINL void AfxV4dExp2(afxV4d v, afxV4d const in)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxExp2f(in[0]);
    v[1] = AfxExp2f(in[1]);
    v[2] = AfxExp2f(in[2]);
    v[3] = AfxExp2f(in[3]);
}

// Exp10

_AFXINL void AfxV2dExp10(afxV2d v, afxV2d const in)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxExp10f(in[0]);
    v[1] = AfxExp10f(in[1]);
}

_AFXINL void AfxV3dExp10(afxV3d v, afxV3d const in)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxExp10f(in[0]);
    v[1] = AfxExp10f(in[1]);
    v[2] = AfxExp10f(in[2]);
}

_AFXINL void AfxV4dExp10(afxV4d v, afxV4d const in)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxExp10f(in[0]);
    v[1] = AfxExp10f(in[1]);
    v[2] = AfxExp10f(in[2]);
    v[3] = AfxExp10f(in[3]);
}

// Returns x raised to the y power; x^y.
// Results are undefined if x < 0 or if x = 0 and y <= 0.

_AFXINL afxReal64 AfxPow(afxReal64 base, afxReal64 exp)
{
    return AfxExp(exp * AfxLog(base));
}

_AFXINL afxReal AfxPowf(afxReal base, afxReal exp)
{
    return AfxExpf(exp * AfxLogf(base));
}

// Pow

_AFXINL void AfxV2dPow(afxV2d v, afxV2d const in)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxPowf(in[0], in[0]);
    v[1] = AfxPowf(in[1], in[1]);
}

_AFXINL void AfxV3dPow(afxV3d v, afxV3d const in)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxPowf(in[0], in[0]);
    v[1] = AfxPowf(in[1], in[1]);
    v[2] = AfxPowf(in[2], in[2]);
}

_AFXINL void AfxV4dPow(afxV4d v, afxV4d const in)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxPowf(in[0], in[0]);
    v[1] = AfxPowf(in[1], in[1]);
    v[2] = AfxPowf(in[2], in[2]);
    v[3] = AfxPowf(in[3], in[3]);
}
