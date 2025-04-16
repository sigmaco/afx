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

#include "qwadro/inc/math/afxLogarithmic.h"

 // Returns the natural logarithm of x; returns the value y which satisfies the equation x = e^y.
 // Results are undefined if x <= 0.

_AFXINL afxReal64 AfxLog(afxReal64 s)
{
    return log(s);
}

_AFXINL afxReal AfxLogf(afxReal s)
{
    return logf(s);
}

// Returns the base 2 logarithm of x; returns the value y which satisfies the equation x = 2^y.
// Results are undefined if x <= 0.

_AFXINL afxReal64 AfxLog2(afxReal64 s)
{
    return log2(s);
}

_AFXINL afxReal AfxLog2f(afxReal s)
{
    return log2f(s);
}

_AFXINL afxReal64 AfxLog10(afxReal64 s)
{
    return log10(s);
}

_AFXINL afxReal AfxLog10f(afxReal s)
{
    return log10f(s);
}

// Log

_AFXINL void AfxV2dLog(afxV2d v, afxV2d const in)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxLogf(in[0]);
    v[1] = AfxLogf(in[1]);
}

_AFXINL void AfxV3dLog(afxV3d v, afxV3d const in)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxLogf(in[0]);
    v[1] = AfxLogf(in[1]);
    v[2] = AfxLogf(in[2]);
}

_AFXINL void AfxV4dLog(afxV4d v, afxV4d const in)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxLogf(in[0]);
    v[1] = AfxLogf(in[1]);
    v[2] = AfxLogf(in[2]);
    v[3] = AfxLogf(in[3]);
}

// Log2

_AFXINL void AfxV2dLog2(afxV2d v, afxV2d const in)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxLog2f(in[0]);
    v[1] = AfxLog2f(in[1]);
}

_AFXINL void AfxV3dLog2(afxV3d v, afxV3d const in)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxLog2f(in[0]);
    v[1] = AfxLog2f(in[1]);
    v[2] = AfxLog2f(in[2]);
}

_AFXINL void AfxV4dLog2(afxV4d v, afxV4d const in)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxLog2f(in[0]);
    v[1] = AfxLog2f(in[1]);
    v[2] = AfxLog2f(in[2]);
    v[3] = AfxLog2f(in[3]);
}

// Log10

_AFXINL void AfxV2dLog10(afxV2d v, afxV2d const in)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxLog10f(in[0]);
    v[1] = AfxLog10f(in[1]);
}

_AFXINL void AfxV3dLog10(afxV3d v, afxV3d const in)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxLog10f(in[0]);
    v[1] = AfxLog10f(in[1]);
    v[2] = AfxLog10f(in[2]);
}

_AFXINL void AfxV4dLog10(afxV4d v, afxV4d const in)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxLog10f(in[0]);
    v[1] = AfxLog10f(in[1]);
    v[2] = AfxLog10f(in[2]);
    v[3] = AfxLog10f(in[3]);
}
