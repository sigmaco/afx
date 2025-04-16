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

//_AFX afxWhd const AFX_WHD_ZERO = { 0, 0, 0 };
//_AFX afxWhd const AFX_WHD_IDENTITY = { 1, 1, 1 };

////////////////////////////////////////////////////////////////////////////////

_AFXINL void AfxWhdReset(afxWhd* whd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(whd);
    AfxWhdFill(whd, 1);
}

_AFXINL void AfxWhdFill(afxWhd* whd, afxUnit value)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(whd);
    whd->w = value;
    whd->h = value;
    whd->d = value;
}

_AFXINL void AfxWhdSet(afxWhd* whd, afxUnit x, afxUnit y, afxUnit z)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(whd);
    whd->w = x;
    whd->h = y;
    whd->d = z;
}

_AFXINL void AfxWhdCopy(afxWhd* whd, afxWhd const in)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(whd);
    AfxWhdSet(whd, in.w, in.h, in.d);
}

_AFXINL void AfxWhdMin(afxWhd* whd, afxWhd const a, afxWhd const b)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(whd);
    //AFX_ASSERT(a);
    //AFX_ASSERT(b);
    whd->x = AFX_MIN(a.w, b.w);
    whd->h = AFX_MIN(a.h, b.h);
    whd->d = AFX_MIN(a.d, b.d);
}

_AFXINL void AfxWhdMax(afxWhd* whd, afxWhd const a, afxWhd const b)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(whd);
    //AFX_ASSERT(a);
    //AFX_ASSERT(b);
    whd->x = AFX_MAX(a.w, b.w);
    whd->h = AFX_MAX(a.h, b.h);
    whd->d = AFX_MAX(a.d, b.d);
}

_AFXINL void AfxWhdClamp(afxWhd* whd, afxWhd const in, afxWhd const min, afxWhd const max)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(whd);
    //AFX_ASSERT(in);
    whd->w = AFX_CLAMP(in.w, min.w, max.w);
    whd->h = AFX_CLAMP(in.h, min.h, max.h);
    whd->d = AFX_CLAMP(in.d, min.d, max.d);
}

_AFXINL void AfxWhdClampOffset(afxWhd* whd, afxWhd const origin, afxWhd const extent)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(whd);
    //AFX_ASSERT(origin);
    whd->w = AFX_MIN(origin.x, extent.w - 1);
    whd->h = AFX_MIN(origin.y, extent.h - 1);
    whd->d = AFX_MIN(origin.z, extent.d - 1);
}

////////////////////////////////////////////////////////////////////////////////
// Logic

_AFXINL afxBool AfxWhdAreEquals(afxWhd const whd, afxWhd const other)
{
    afxError err = AFX_ERR_NONE;
    return (whd.w == other.x) || (whd.h == other.y) || (whd.d == other.d);
}

_AFXINL afxBool AfxWhdIsLessOrEqual(afxWhd const whd, afxWhd const other)
{
    afxError err = AFX_ERR_NONE;
    //AFX_ASSERT(whd);
    //AFX_ASSERT(other);
    return (((whd.w <= other.x) && (whd.h <= other.y) && (whd.d <= other.d)) != 0);
}

_AFXINL afxBool AfxWhdIsLessThan(afxWhd const whd, afxWhd const other)
{
    afxError err = AFX_ERR_NONE;
    //AFX_ASSERT(whd);
    //AFX_ASSERT(other);
    return (((whd.w < other.x) && (whd.h < other.y) && (whd.d < other.d)) != 0);
}

_AFXINL afxBool AfxWhdIsZero(afxWhd const whd)
{
    afxError err = AFX_ERR_NONE;
    //AFX_ASSERT(whd);
    return (AFX_R(0) == (whd.w + whd.h + whd.d));
}

_AFXINL void AfxWhdAdd(afxWhd* whd, afxWhd const a, afxWhd const b)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(whd);
    //AFX_ASSERT(a);
    //AFX_ASSERT(b);
    whd->w = a.w + b.w;
    whd->h = a.h + b.h;
    whd->d = a.d + b.d;
}

_AFXINL void AfxWhdSub(afxWhd* whd, afxWhd const a, afxWhd const b)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(whd);
    //AFX_ASSERT(a);
    //AFX_ASSERT(b);
    whd->w = a.w - b.w;
    whd->h = a.h - b.h;
    whd->d = a.d - b.d;
}

_AFXINL void AfxWhdDiv(afxWhd* whd, afxWhd const in, afxUnit div)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(whd);
    //AFX_ASSERT(in);
    AFX_ASSERT(div);
    whd->w = in.w / div;
    whd->h = in.h / div;
    whd->d = in.d / div;
}

_AFXINL void AfxWhdMod(afxWhd* whd, afxWhd const in, afxUnit div)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(whd);
    //AFX_ASSERT(in);
    AFX_ASSERT(div);
    whd->w = in.w % div;
    whd->h = in.h % div;
    whd->d = in.d % div;
}

_AFXINL void AfxWhdScale(afxWhd* whd, afxWhd const in, afxUnit s)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(whd);
    //AFX_ASSERT(in);
    whd->w = in.w * s;
    whd->h = in.h * s;
    whd->d = in.d * s;
}

_AFXINL void AfxWhdHalf(afxWhd* whd, afxWhd const in)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(whd);
    //AFX_ASSERT(in);
    whd->w = AFX_MAX(in.w ? 1 : 0, in.w >> 1);
    whd->h = AFX_MAX(in.h ? 1 : 0, in.h >> 1);
    whd->d = AFX_MAX(in.d ? 1 : 0, in.d >> 1);
}

_AFXINL afxUnit AfxWhdSum(afxWhd const whd)
{
    afxError err = AFX_ERR_NONE;
    //AFX_ASSERT(whd);
    return whd.w + whd.h + whd.d;
}

_AFXINL afxUnit AfxWhdMag(afxWhd const whd)
{
    afxError err = AFX_ERR_NONE;
    //AFX_ASSERT(whd);
    return whd.w * whd.h * whd.d;
}

_AFXINL afxUnit AfxWhdDot(afxWhd const whd, afxWhd const other)
{
    afxError err = AFX_ERR_NONE;
    //AFX_ASSERT(whd);
    //AFX_ASSERT(other);
    return (whd.w * other.x) + (whd.h * other.y) + (whd.d * other.d);
}

_AFXINL afxError _AfxAssertWhd(afxWhd total_, afxWhd base_, afxWhd range_, afxHere const hint)
{
    afxError err = AFX_ERR_NONE;
    //AFX_ASSERT(total_);
    //AFX_ASSERT(base_);
    //AFX_ASSERT(range_);

    if ((err |= (!(total_.x > base_.x + range_.x))) || (err |= (!(total_.h > base_.h + range_.h))) || (err |= (!(total_.d > base_.d + range_.d))))
    {
        AfxReportError("[%i] %s(%f) + %s(%f) is out of range [0, %f]", AFX_STRINGIFY((base_)), (base_), AFX_STRINGIFY((range_)), (range_), AFX_STRINGIFY((total_)), (total_));
    }
    return err;
}
