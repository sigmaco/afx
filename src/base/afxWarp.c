/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA Advanced Math Extensions for Qwadro

#include "qwadro/math/afxScalar.h"
#include "qwadro/math/afxVector.h"
#include "qwadro/math/afxMatrix.h"
#include "qwadro/base/afxWarp.h"

//_AFX afxWarp const AFX_WHD_ZERO = { 0, 0, 0 };
//_AFX afxWarp const AFX_WHD_IDENTITY = { 1, 1, 1 };

////////////////////////////////////////////////////////////////////////////////

_AFXINL void AfxWhdReset(afxWarp* whd)
{
    afxError err = { 0 };
    AFX_ASSERT(whd);
    AfxWhdFill(whd, 1);
}

_AFXINL void AfxWhdFill(afxWarp* whd, afxUnit value)
{
    afxError err = { 0 };
    AFX_ASSERT(whd);
    whd->w = value;
    whd->h = value;
    whd->d = value;
}

_AFXINL void AfxWhdSet(afxWarp* whd, afxUnit x, afxUnit y, afxUnit z)
{
    afxError err = { 0 };
    AFX_ASSERT(whd);
    whd->w = x;
    whd->h = y;
    whd->d = z;
}

_AFXINL void AfxWhdCopy(afxWarp* whd, afxWarp const in)
{
    afxError err = { 0 };
    AFX_ASSERT(whd);
    AfxWhdSet(whd, in.w, in.h, in.d);
}

_AFXINL void AfxWhdMin(afxWarp* whd, afxWarp const a, afxWarp const b)
{
    afxError err = { 0 };
    AFX_ASSERT(whd);
    //AFX_ASSERT(a);
    //AFX_ASSERT(b);
    whd->x = AFX_MIN(a.w, b.w);
    whd->h = AFX_MIN(a.h, b.h);
    whd->d = AFX_MIN(a.d, b.d);
}

_AFXINL void AfxWhdMax(afxWarp* whd, afxWarp const a, afxWarp const b)
{
    afxError err = { 0 };
    AFX_ASSERT(whd);
    //AFX_ASSERT(a);
    //AFX_ASSERT(b);
    whd->x = AFX_MAX(a.w, b.w);
    whd->h = AFX_MAX(a.h, b.h);
    whd->d = AFX_MAX(a.d, b.d);
}

_AFXINL void AfxWhdClamp(afxWarp* whd, afxWarp const in, afxWarp const min, afxWarp const max)
{
    afxError err = { 0 };
    AFX_ASSERT(whd);
    //AFX_ASSERT(in);
    whd->w = AFX_CLAMP(in.w, min.w, max.w);
    whd->h = AFX_CLAMP(in.h, min.h, max.h);
    whd->d = AFX_CLAMP(in.d, min.d, max.d);
}

_AFXINL void AfxWhdClampOffset(afxWarp* whd, afxWarp const origin, afxWarp const extent)
{
    afxError err = { 0 };
    AFX_ASSERT(whd);
    //AFX_ASSERT(origin);
    whd->w = AFX_MIN(origin.x, extent.w - 1);
    whd->h = AFX_MIN(origin.y, extent.h - 1);
    whd->d = AFX_MIN(origin.z, extent.d - 1);
}

////////////////////////////////////////////////////////////////////////////////
// Logic

_AFXINL afxBool AfxWhdAreEquals(afxWarp const whd, afxWarp const other)
{
    afxError err = { 0 };
    return (whd.w == other.x) || (whd.h == other.y) || (whd.d == other.d);
}

_AFXINL afxBool AfxWhdIsLessOrEqual(afxWarp const whd, afxWarp const other)
{
    afxError err = { 0 };
    //AFX_ASSERT(whd);
    //AFX_ASSERT(other);
    return (((whd.w <= other.x) && (whd.h <= other.y) && (whd.d <= other.d)) != 0);
}

_AFXINL afxBool AfxWhdIsLessThan(afxWarp const whd, afxWarp const other)
{
    afxError err = { 0 };
    //AFX_ASSERT(whd);
    //AFX_ASSERT(other);
    return (((whd.w < other.x) && (whd.h < other.y) && (whd.d < other.d)) != 0);
}

_AFXINL afxBool AfxWhdIsZero(afxWarp const whd)
{
    afxError err = { 0 };
    //AFX_ASSERT(whd);
    return (AFX_REAL(0) == (whd.w + whd.h + whd.d));
}

_AFXINL void AfxWhdAdd(afxWarp* whd, afxWarp const a, afxWarp const b)
{
    afxError err = { 0 };
    AFX_ASSERT(whd);
    //AFX_ASSERT(a);
    //AFX_ASSERT(b);
    whd->w = a.w + b.w;
    whd->h = a.h + b.h;
    whd->d = a.d + b.d;
}

_AFXINL void AfxWhdSub(afxWarp* whd, afxWarp const a, afxWarp const b)
{
    afxError err = { 0 };
    AFX_ASSERT(whd);
    //AFX_ASSERT(a);
    //AFX_ASSERT(b);
    whd->w = a.w - b.w;
    whd->h = a.h - b.h;
    whd->d = a.d - b.d;
}

_AFXINL void AfxWhdDiv(afxWarp* whd, afxWarp const in, afxUnit div)
{
    afxError err = { 0 };
    AFX_ASSERT(whd);
    //AFX_ASSERT(in);
    AFX_ASSERT(div);
    whd->w = in.w / div;
    whd->h = in.h / div;
    whd->d = in.d / div;
}

_AFXINL void AfxWhdMod(afxWarp* whd, afxWarp const in, afxUnit div)
{
    afxError err = { 0 };
    AFX_ASSERT(whd);
    //AFX_ASSERT(in);
    AFX_ASSERT(div);
    whd->w = in.w % div;
    whd->h = in.h % div;
    whd->d = in.d % div;
}

_AFXINL void AfxWhdScale(afxWarp* whd, afxWarp const in, afxUnit s)
{
    afxError err = { 0 };
    AFX_ASSERT(whd);
    //AFX_ASSERT(in);
    whd->w = in.w * s;
    whd->h = in.h * s;
    whd->d = in.d * s;
}

_AFXINL void AfxWhdHalf(afxWarp* whd, afxWarp const in)
{
    afxError err = { 0 };
    AFX_ASSERT(whd);
    //AFX_ASSERT(in);
    whd->w = AFX_MAX(in.w ? 1 : 0, in.w >> 1);
    whd->h = AFX_MAX(in.h ? 1 : 0, in.h >> 1);
    whd->d = AFX_MAX(in.d ? 1 : 0, in.d >> 1);
}

_AFXINL afxUnit AfxWhdSum(afxWarp const whd)
{
    afxError err = { 0 };
    //AFX_ASSERT(whd);
    return whd.w + whd.h + whd.d;
}

_AFXINL afxUnit AfxWhdMag(afxWarp const whd)
{
    afxError err = { 0 };
    //AFX_ASSERT(whd);
    return whd.w * whd.h * whd.d;
}

_AFXINL afxUnit AfxWhdDot(afxWarp const whd, afxWarp const other)
{
    afxError err = { 0 };
    //AFX_ASSERT(whd);
    //AFX_ASSERT(other);
    return (whd.w * other.x) + (whd.h * other.y) + (whd.d * other.d);
}

_AFXINL afxError _AfxAssertWhd(afxWarp total_, afxWarp base_, afxWarp range_, afxHere const hint)
{
    afxError err = { 0 };
    //AFX_ASSERT(total_);
    //AFX_ASSERT(base_);
    //AFX_ASSERT(range_);

    if ((err |= (!(total_.x > base_.x + range_.x))) || (err |= (!(total_.h > base_.h + range_.h))) || (err |= (!(total_.d > base_.d + range_.d))))
    {
        AfxReportError("[%i] %s(%f) + %s(%f) is out of range [0, %f]", AFX_STRINGIFY((base_)), (base_), AFX_STRINGIFY((range_)), (range_), AFX_STRINGIFY((total_)), (total_));
    }
    return err;
}
