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

#include "qwadro/inc/math/afxScalar.h"
#include "qwadro/inc/math/afxVector.h"
#include "qwadro/inc/math/afxMatrix.h"
#include "qwadro/inc/math/afxWhd.h"

_AFX afxWhd const AFX_WHD_IDENTITY = { 1, 1, 1 };

////////////////////////////////////////////////////////////////////////////////

_AFXINL void AfxWhdReset(afxWhd whd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(whd);
    AfxWhdFill(whd, 1);
}

_AFXINL void AfxWhdFill(afxWhd whd, afxNat value)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(whd);
    whd[0] = value;
    whd[1] = value;
    whd[2] = value;
}

_AFXINL void AfxWhdSet(afxWhd whd, afxNat x, afxNat y, afxNat z)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(whd);
    whd[0] = x;
    whd[1] = y;
    whd[2] = z;
}

_AFXINL void AfxWhdCopy(afxWhd whd, afxWhd const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(whd);
    AfxWhdSet(whd, in[0], in[1], in[2]);
}

_AFXINL void AfxWhdMin(afxWhd whd, afxWhd const a, afxWhd const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(whd);
    AfxAssert(a);
    AfxAssert(b);
    whd[0] = AfxMin(a[0], b[0]);
    whd[1] = AfxMin(a[1], b[1]);
    whd[2] = AfxMin(a[2], b[2]);
}

_AFXINL void AfxWhdMax(afxWhd whd, afxWhd const a, afxWhd const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(whd);
    AfxAssert(a);
    AfxAssert(b);
    whd[0] = AfxMax(a[0], b[0]);
    whd[1] = AfxMax(a[1], b[1]);
    whd[2] = AfxMax(a[2], b[2]);
}

_AFXINL void AfxWhdClamp(afxWhd whd, afxWhd const in, afxWhd const min, afxWhd const max)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(whd);
    AfxAssert(in);
    whd[0] = AfxClamp(in[0], min[0], max[0]);
    whd[1] = AfxClamp(in[1], min[1], max[1]);
    whd[2] = AfxClamp(in[2], min[2], max[2]);
}

////////////////////////////////////////////////////////////////////////////////
// Logic

_AFXINL afxBool AfxWhdAreEquals(afxWhd const whd, afxWhd const other)
{
    afxError err = AFX_ERR_NONE;
    return (whd[0] == other[0]) || (whd[1] == other[1]) || (whd[2] == other[2]);
}

_AFXINL afxBool AfxWhdIsLessOrEqual(afxWhd const whd, afxWhd const other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(whd);
    AfxAssert(other);
    return (((whd[0] <= other[0]) && (whd[1] <= other[1]) && (whd[2] <= other[2])) != 0);
}

_AFXINL afxBool AfxWhdIsLessThan(afxWhd const whd, afxWhd const other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(whd);
    AfxAssert(other);
    return (((whd[0] < other[0]) && (whd[1] < other[1]) && (whd[2] < other[2])) != 0);
}

_AFXINL afxBool AfxWhdIsZero(afxWhd const whd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(whd);
    return (AfxScalar(0) == (whd[0] + whd[1] + whd[2]));
}

_AFXINL void AfxWhdAdd(afxWhd whd, afxWhd const a, afxWhd const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(whd);
    AfxAssert(a);
    AfxAssert(b);
    whd[0] = a[0] + b[0];
    whd[1] = a[1] + b[1];
    whd[2] = a[2] + b[2];
}

_AFXINL void AfxWhdSub(afxWhd whd, afxWhd const a, afxWhd const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(whd);
    AfxAssert(a);
    AfxAssert(b);
    whd[0] = a[0] - b[0];
    whd[1] = a[1] - b[1];
    whd[2] = a[2] - b[2];
}

_AFXINL void AfxWhdDiv(afxWhd whd, afxWhd const in, afxNat div)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(whd);
    AfxAssert(in);
    AfxAssert(div);
    whd[0] = in[0] / div;
    whd[1] = in[1] / div;
    whd[2] = in[2] / div;
}

_AFXINL void AfxWhdMod(afxWhd whd, afxWhd const in, afxNat div)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(whd);
    AfxAssert(in);
    AfxAssert(div);
    whd[0] = in[0] % div;
    whd[1] = in[1] % div;
    whd[2] = in[2] % div;
}

_AFXINL void AfxWhdScale(afxWhd whd, afxWhd const in, afxNat s)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(whd);
    AfxAssert(in);
    whd[0] = in[0] * s;
    whd[1] = in[1] * s;
    whd[2] = in[2] * s;
}

_AFXINL void AfxWhdHalf(afxWhd whd, afxWhd const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(whd);
    AfxAssert(in);
    whd[0] = in[0] / 2;
    whd[1] = in[1] / 2;
    whd[2] = in[2] / 2;
}

_AFXINL afxNat AfxWhdSum(afxWhd const whd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(whd);
    return whd[0] + whd[1] + whd[2];
}

_AFXINL afxNat AfxWhdMag(afxWhd const whd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(whd);
    return whd[0] * whd[1] * whd[2];
}

_AFXINL afxNat AfxWhdDot(afxWhd const whd, afxWhd const other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(whd);
    AfxAssert(other);
    return (whd[0] * other[0]) + (whd[1] * other[1]) + (whd[2] * other[2]);
}

_AFXINL afxError _AfxAssertWhd(afxWhd total_, afxWhd base_, afxWhd range_, afxHere const hint)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(total_);
    AfxAssert(base_);
    AfxAssert(range_);

    for (afxNat i = 0; i < 3; i++)
    {
        if ((err |= (!(total_[i] > base_[i] + range_[i]))))
        {
            AfxLogError("[%i] %s(%f) + %s(%f) is out of range [0, %f]", AFX_STRINGIFY((base_)), (base_), AFX_STRINGIFY((range_)), (range_), AFX_STRINGIFY((total_)), (total_));
        }
    }
    return err;
}
