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

// Memory layout: row-major

_AFX afxV2d const AFX_V2D_X = { AFX_REAL(1), AFX_REAL(0) };
_AFX afxV3d const AFX_V3D_X = { AFX_REAL(1), AFX_REAL(0), AFX_REAL(0) };
_AFX afxV4d const AFX_V4D_X = { AFX_REAL(1), AFX_REAL(0), AFX_REAL(0), AFX_REAL(0) };
_AFX afxV2d const AFX_V2D_X_NEG = { AFX_REAL(-1), AFX_REAL(0) };
_AFX afxV3d const AFX_V3D_X_NEG = { AFX_REAL(-1), AFX_REAL(0), AFX_REAL(0) };
_AFX afxV4d const AFX_V4D_X_NEG = { AFX_REAL(-1), AFX_REAL(0), AFX_REAL(0), AFX_REAL(0) };

_AFX afxV2d const AFX_V2D_Y = { AFX_REAL(0), AFX_REAL(1) };
_AFX afxV3d const AFX_V3D_Y = { AFX_REAL(0), AFX_REAL(1), AFX_REAL(0) };
_AFX afxV4d const AFX_V4D_Y = { AFX_REAL(0), AFX_REAL(1), AFX_REAL(0), AFX_REAL(0) };
_AFX afxV2d const AFX_V2D_Y_NEG = { AFX_REAL(0), AFX_REAL(-1) };
_AFX afxV3d const AFX_V3D_Y_NEG = { AFX_REAL(0), AFX_REAL(-1), AFX_REAL(0) };
_AFX afxV4d const AFX_V4D_Y_NEG = { AFX_REAL(0), AFX_REAL(-1), AFX_REAL(0), AFX_REAL(0) };

_AFX afxV3d const AFX_V3D_Z = { AFX_REAL(0), AFX_REAL(0), AFX_REAL(1) };
_AFX afxV4d const AFX_V4D_Z = { AFX_REAL(0), AFX_REAL(0), AFX_REAL(1), AFX_REAL(0) };
_AFX afxV3d const AFX_V3D_Z_NEG = { AFX_REAL(0), AFX_REAL(0), AFX_REAL(-1) };
_AFX afxV4d const AFX_V4D_Z_NEG = { AFX_REAL(0), AFX_REAL(0), AFX_REAL(-1), AFX_REAL(0) };

_AFX afxV4d const AFX_V4D_W = { AFX_REAL(0), AFX_REAL(0), AFX_REAL(0), AFX_REAL(1) };
_AFX afxV4d const AFX_V4D_W_NEG = { AFX_REAL(0), AFX_REAL(0), AFX_REAL(0), AFX_REAL(-1) };

_AFX afxV4d const AFX_V4D_XYZ = { AFX_REAL(1), AFX_REAL(1), AFX_REAL(1), AFX_REAL(0) };

_AFX afxV2d const AFX_V2D_ONE = { AFX_REAL(1), AFX_REAL(1) };
_AFX afxV3d const AFX_V3D_ONE = { AFX_REAL(1), AFX_REAL(1), AFX_REAL(1) };
_AFX afxV4d const AFX_V4D_ONE = { AFX_REAL(1), AFX_REAL(1), AFX_REAL(1), AFX_REAL(1) };

_AFX afxV4d const AFX_V4D_ONE_NEG = { AFX_REAL(-1), AFX_REAL(-1), AFX_REAL(-1), AFX_REAL(-1) };

_AFX afxV2d const AFX_V2D_ZERO = { AFX_REAL(0), AFX_REAL(0) };
_AFX afxV3d const AFX_V3D_ZERO = { AFX_REAL(0), AFX_REAL(0), AFX_REAL(0) };
_AFX afxV4d const AFX_V4D_ZERO = { AFX_REAL(0), AFX_REAL(0), AFX_REAL(0), AFX_REAL(0) };

_AFX afxV2d const AFX_V2D_IDENTITY = { AFX_REAL(0), AFX_REAL(0) };
_AFX afxV3d const AFX_V3D_IDENTITY = { AFX_REAL(0), AFX_REAL(0), AFX_REAL(0) };
_AFX afxV4d const AFX_V4D_IDENTITY = { AFX_REAL(0), AFX_REAL(0), AFX_REAL(0), AFX_REAL(1) };


////////////////////////////////////////////////////////////////////////////////
// Initialization                                                             //
////////////////////////////////////////////////////////////////////////////////

_AFXINL void AfxFillV2d(afxV2d v, afxReal value)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    v[0] = value;
    v[1] = value;
}

_AFXINL void AfxFillV3d(afxV3d v, afxReal value)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    v[0] = value;
    v[1] = value;
    v[2] = value;
}

_AFXINL void AfxFillV4d(afxV4d v, afxReal value)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    v[0] = value;
    v[1] = value;
    v[2] = value;
    v[3] = value;
}

// Zero

_AFXINL void AfxV2dZero(afxV2d v)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AfxV2dCopy(v, AFX_V2D_ZERO);
}

_AFXINL void AfxV3dZero(afxV3d v)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AfxV3dCopy(v, AFX_V3D_ZERO);
}

_AFXINL void AfxV4dZero(afxV4d v)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AfxV4dCopy(v, AFX_V4D_ZERO);
}

// Set

_AFXINL void AfxV2dSet(afxV2d v, afxReal x, afxReal y)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    v[0] = x;
    v[1] = y;
}

_AFXINL void AfxV3dSet(afxV3d v, afxReal x, afxReal y, afxReal z)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    v[0] = x;
    v[1] = y;
    v[2] = z;
}

_AFXINL void AfxV4dSet(afxV4d v, afxReal x, afxReal y, afxReal z, afxReal w)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    v[0] = x;
    v[1] = y;
    v[2] = z;
    v[3] = w;
}

// MakeIdentity

_AFXINL void AfxResetV4d(afxV4d v)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AfxV4dCopy(v, AFX_V4D_W);
}

////////////////////////////////////////////////////////////////////////////////
// Logic                                                                      //
////////////////////////////////////////////////////////////////////////////////

// IsIdentity

_AFXINL afxBool AfxV4dIsIdentity(afxV4d const v)
{
    return (v[0] + v[1] + v[2]) == 0 && (v[3] == 1);
}

// AreEquals

_AFXINL afxBool AfxV2dAreEquals(afxV2d const v, afxV2d const other)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(other);
    return  (AFX_REAL(v[0] < other[0] + AFX_EPSILON && v[0] > other[0] - AFX_EPSILON)) +
            (AFX_REAL(v[1] < other[1] + AFX_EPSILON && v[1] > other[1] - AFX_EPSILON));
}

_AFXINL afxBool AfxV3dAreEquals(afxV3d const v, afxV3d const other)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(other);
    return  (AFX_REAL(v[0] < other[0] + AFX_EPSILON && v[0] > other[0] - AFX_EPSILON)) +
            (AFX_REAL(v[1] < other[1] + AFX_EPSILON && v[1] > other[1] - AFX_EPSILON)) +
            (AFX_REAL(v[2] < other[2] + AFX_EPSILON && v[2] > other[2] - AFX_EPSILON));
}

_AFXINL afxBool AfxV4dAreEquals(afxV4d const v, afxV4d const other)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(other);
    return  (AFX_REAL(v[0] < other[0] + AFX_EPSILON && v[0] > other[0] - AFX_EPSILON)) +
            (AFX_REAL(v[1] < other[1] + AFX_EPSILON && v[1] > other[1] - AFX_EPSILON)) +
            (AFX_REAL(v[2] < other[2] + AFX_EPSILON && v[2] > other[2] - AFX_EPSILON)) +
            (AFX_REAL(v[3] < other[3] + AFX_EPSILON && v[3] > other[3] - AFX_EPSILON));
}

// IsInBounds

_AFXINL afxBool AfxV2dIsInBounds(afxV2d const v, afxV2d const bounds)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(bounds);
    return (((v[0] <= bounds[0] && v[0] >= -bounds[0]) && (v[1] <= bounds[1] && v[1] >= -bounds[1])) != 0);
}

_AFXINL afxBool AfxV3dIsInBounds(afxV3d const v, afxV3d const bounds)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(bounds);
    return (((v[0] <= bounds[0] && v[0] >= -bounds[0]) && (v[1] <= bounds[1] && v[1] >= -bounds[1]) && (v[2] <= bounds[2] && v[2] >= -bounds[2])) != 0);
}

_AFXINL afxBool AfxV4dIsInBounds(afxV4d const v, afxV4d const bounds)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(bounds);
    return (((v[0] <= bounds[0] && v[0] >= -bounds[0]) && (v[1] <= bounds[1] && v[1] >= -bounds[1]) && (v[2] <= bounds[2] && v[2] >= -bounds[2]) && (v[3] <= bounds[3] && v[3] >= -bounds[3])) != 0);
}

// IsLessOrEqual

_AFXINL afxBool AfxV2dIsLessOrEqual(afxV2d const v, afxV2d const other)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(other);
    return (((v[0] <= other[0]) && (v[1] <= other[1])) != 0);
}

_AFXINL afxBool AfxV3dIsLessOrEqual(afxV3d const v, afxV3d const other)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(other);
    return (((v[0] <= other[0]) && (v[1] <= other[1]) && (v[2] <= other[2])) != 0);
}

_AFXINL afxBool AfxV4dIsLessOrEqual(afxV4d const v, afxV4d const other)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(other);
    return (((v[0] <= other[0]) && (v[1] <= other[1]) && (v[2] <= other[2]) && (v[3] <= other[3])) != 0);
}

// IsLessThan

_AFXINL afxBool AfxV2dIsLessThan(afxV2d const v, afxV2d const other)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(other);
    return (((v[0] < other[0]) && (v[1] < other[1])) != 0);
}

_AFXINL afxBool AfxV3dIsLessThan(afxV3d const v, afxV3d const other)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(other);
    return (((v[0] < other[0]) && (v[1] < other[1]) && (v[2] < other[2])) != 0);
}

_AFXINL afxBool AfxV4dIsLessThan(afxV4d const v, afxV4d const other)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(other);
    return (((v[0] < other[0]) && (v[1] < other[1]) && (v[2] < other[2]) && (v[3] < other[3])) != 0);
}

// IsZero

_AFXINL afxBool AfxV2dIsZero(afxV2d const v)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    return (AFX_REAL(0) == (v[0] + v[1]));
}

_AFXINL afxBool AfxV3dIsZero(afxV3d const v)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    return (AFX_REAL(0) == (v[0] + v[1] + v[2]));
}

_AFXINL afxBool AfxV4dIsZero(afxV4d const v)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    return (AFX_REAL(0) == (v[0] + v[1] + v[2] + v[3]));
}

// IsInfinite

_AFXINL afxBool AfxV2dIsInfinite(afxV2d const v)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    return (AfxIsInfinite(v[0]) ? 0xFFFFFFFFU : 0) | (AfxIsInfinite(v[1]) ? 0xFFFFFFFFU : 0);
}

_AFXINL afxBool AfxV3dIsInfinite(afxV3d const v)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    return (AfxIsInfinite(v[0]) ? 0xFFFFFFFFU : 0) | (AfxIsInfinite(v[1]) ? 0xFFFFFFFFU : 0) | (AfxIsInfinite(v[2]) ? 0xFFFFFFFFU : 0);
}

_AFXINL afxBool AfxV4dIsInfinite(afxV4d const v)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    return ((AfxIsInfinite(v[0])) ? 0xFFFFFFFFU : 0) | (AfxIsInfinite(v[1]) ? 0xFFFFFFFFU : 0) | (AfxIsInfinite(v[2]) ? 0xFFFFFFFFU : 0) | (AfxIsInfinite(v[3]) ? 0xFFFFFFFFU : 0);
}

// IsNormalized

_AFXINL afxBool AfxV3dIsNormalized(afxV3d const v)
{
    // From Crytek

    afxError err = { 0 };
    AFX_ASSERT(v);
    afxReal f = AfxV3dMag(v);
    return f >= 0.95f && f <= 1.05f;
}

// IsNan

_AFXINL afxBool AfxV2dIsNan(afxV2d const v)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    return (AfxIsNan(v[0]) || AfxIsNan(v[1]));
}

_AFXINL afxBool AfxV3dIsNan(afxV3d const v)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    return (AfxIsNan(v[0]) || AfxIsNan(v[1]) || AfxIsNan(v[2]));
}

_AFXINL afxBool AfxV4dIsNan(afxV4d const v)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    return (AfxIsNan(v[0]) || AfxIsNan(v[1]) || AfxIsNan(v[2]) || AfxIsNan(v[3]));
}



////////////////////////////////////////////////////////////////////////////////
// Transferance                                                               //
////////////////////////////////////////////////////////////////////////////////

// Copy

_AFXINL void AfxV2dCopy(afxV2d v, afxV2d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    AFX_ASSERT_DIFF(v, in);
    v[0] = in[0];
    v[1] = in[1];
}

_AFXINL void AfxV3dCopy(afxV3d v, afxV3d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    AFX_ASSERT_DIFF(v, in);
    v[0] = in[0];
    v[1] = in[1];
    v[2] = in[2];
}

_AFXINL void AfxV4dCopy(afxV4d v, afxV4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    AFX_ASSERT_DIFF(v, in);
    v[0] = in[0];
    v[1] = in[1];
    v[2] = in[2];
    v[3] = in[3];
}

_AFXINL void AfxCopyArrayedV2d(afxUnit cnt, afxV2d const in[], afxV2d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(out);
    AFX_ASSERT(in);
    AFX_ASSERT_DIFF(out, in);

    for (afxUnit i = 0; i < cnt; i++)
        AfxV2dCopy(out[i], in[i]);
}

_AFXINL void AfxCopyArrayedV3d(afxUnit cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(out);
    AFX_ASSERT(in);
    AFX_ASSERT_DIFF(out, in);

    for (afxUnit i = 0; i < cnt; i++)
        AfxV3dCopy(out[i], in[i]);
}

_AFXINL void AfxCopyArrayedV4d(afxUnit cnt, afxV4d const in[], afxV4d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(out);
    AFX_ASSERT(in);
    AFX_ASSERT_DIFF(out, in);

    for (afxUnit i = 0; i < cnt; i++)
        AfxV4dCopy(out[i], in[i]);
}

_AFXINL void AfxV3dCopyAtv2d(afxV3d v, afxV2d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    AFX_ASSERT_DIFF(v, in);
    v[0] = in[0];
    v[1] = in[1];
    v[2] = AFX_REAL(1);
}

_AFXINL void AfxV4dCopyAtv3d(afxV4d v, afxV3d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    AFX_ASSERT_DIFF(v, in);
    v[0] = in[0];
    v[1] = in[1];
    v[2] = in[2];
    v[3] = AFX_REAL(1);
}

// Make (Combined Copy & Promote/Demote operations)

_AFXINL void AfxV3dCopyV2d(afxV3d v, afxV2d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    AFX_ASSERT_DIFF(v, in);
    v[0] = in[0];
    v[1] = in[1];
    v[2] = AFX_REAL(0);
}

_AFXINL void AfxV4dCopyV2d(afxV4d v, afxV2d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    AFX_ASSERT_DIFF(v, in);
    v[0] = in[0];
    v[1] = in[1];
    v[2] = AFX_REAL(0);
    v[3] = AFX_REAL(0);
}

_AFXINL void AfxV4dCopyV3d(afxV4d v, afxV3d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    AFX_ASSERT_DIFF(v, in);
    v[0] = in[0];
    v[1] = in[1];
    v[2] = in[2];
    v[3] = AFX_REAL(0);
}

// Swap

_AFXINL void AfxSwapV2d(afxV2d v, afxV2d other)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(other);
    AFX_ASSERT(v != other);
    afxV2d tmp;
    AfxV2dCopy(tmp, v);
    AfxV2dCopy(other, tmp);
    AfxV2dCopy(v, other);
}

_AFXINL void AfxSwapV3d(afxV3d v, afxV3d other)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(other);
    AFX_ASSERT(v != other);
    afxV3d tmp;
    AfxV3dCopy(tmp, v);
    AfxV3dCopy(other, tmp);
    AfxV3dCopy(v, other);
}

_AFXINL void AfxSwapV4d(afxV4d v, afxV4d other)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(other);
    AFX_ASSERT(v != other);
    afxV4d tmp;
    AfxV4dCopy(tmp, v);
    AfxV4dCopy(other, tmp);
    AfxV4dCopy(v, other);
}

// Normalize

_AFXINL afxReal AfxV2dNormalize(afxV2d v, afxV2d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    afxReal len = AfxV2dMag(in);

    if (len > 0)
        len = 1.f / len;

    AfxV2dScale(v, in, len);
    return len;
}

_AFXINL afxReal AfxV3dNormalize(afxV3d v, afxV3d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    afxReal len = AfxV3dMag(in);

    if (len > 0)
        len = 1.f / len;

    AfxV3dScale(v, in, len);
    return len;
}

_AFXINL afxReal AfxV4dNormalize(afxV4d v, afxV4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    afxReal len = AfxV4dMag(in);

    if (len > 0)
        len = 1.f / len;

    AfxV4dScale(v, in, len);
    return len;
}

// NormalizeV*d

_AFXINL afxReal AfxV3dNormalizeV4d(afxV3d v, afxV4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    afxReal len = AfxV4dMag(in);

    if (len > 0)
        len = 1.f / len;

    AfxV3dScale(v, in, len);
    return len;
}

_AFXINL afxReal AfxV4dNormalizeV3d(afxV4d v, afxV3d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    afxReal len = AfxV3dMag(in);

    if (len > 0)
        len = 1.f / len;

    AfxV4dScale(v, in, len);
    return len;
}

// NormalizeEst

_AFXINL afxReal AfxV2dNormalizeEstimated(afxV2d v, afxV2d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    afxReal len = AfxV2dMagRecip(in);
    AfxV2dScale(v, in, len);
    return len;
}

_AFXINL afxReal AfxV3dNormalizeEstimated(afxV3d v, afxV3d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    afxReal len = AfxV3dMagRecip(in);
    AfxV3dScale(v, in, len);
    return len;
}

_AFXINL afxReal AfxV4dNormalizeEstimated(afxV4d v, afxV4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    afxReal len = AfxV4dMagRecip(in);
    AfxV4dScale(v, in, len);
    return len;
}

_AFXINL void AfxNormalizeArrayedV2d(afxUnit cnt, afxV2d const in[], afxV2d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
        AfxV2dNormalize(out[i], in[i]);
}

_AFXINL void AfxNormalizeArrayedV3d(afxUnit cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
        AfxV3dNormalize(out[i], in[i]);
}

_AFXINL void AfxNormalizeArrayedV4d(afxUnit cnt, afxV4d const in[], afxV4d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
        AfxV4dNormalize(out[i], in[i]);
}

_AFXINL void AfxZeroOrNormalizeArrayedV2d(afxUnit cnt, afxV2d const in[], afxV2d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxReal len = AfxV2dMag(in[i]);

        if (len <= 0.0000099999997f)
            AfxV2dZero(out[i]);
        else
            AfxV2dScale(out[i], in[i], 1.f / len);
    }
}

_AFXINL void AfxZeroOrNormalizeArrayedV3d(afxUnit cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxReal len = AfxV3dMag(in[i]);
        
        if (len <= 0.0000099999997f)
            AfxV3dZero(out[i]);
        else
            AfxV3dScale(out[i], in[i], 1.f / len);
    }
}

_AFXINL void AfxZeroOrNormalizeArrayedV4d(afxUnit cnt, afxV4d const in[], afxV4d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxReal len = AfxV4dMag(in[i]);

        if (len <= 0.0000099999997f)
            AfxV4dZero(out[i]);
        else
            AfxV4dScale(out[i], in[i], 1.f / len);
    }
}

// SLERP
// Function to perform Spherical Linear Interpolation (SLERP) between two quaternions
// v(t) = ((sin((1 - t) * theta)) / sin(theta)) * x + ((sin(t) * theta) / (sin(theta))) * y

_AFXINL void AfxV2dSlerp(afxV2d v, afxV2d x, afxV2d y, afxReal t)
{
    AfxV2dNormalize(x, x);
    AfxV2dNormalize(y, y);

    afxReal dotProd = AFX_MAX(AFX_MIN(AfxV2dDot(x, y), 1.0f), -1.0f);
    
    afxReal theta_0 = AfxAcos(dotProd);
    afxReal theta = theta_0 * t;
    afxReal thetaCos = AfxCos(theta);
    afxReal thetaSin = AfxSin(theta);

    afxV2d tmp =
    {
        y[0] - x[0] * dotProd, 
        y[1] - x[1] * dotProd
    };    
    AfxV2dNormalize(tmp, tmp);
    
    v[0] = x[0] * thetaCos + tmp[0] * thetaSin;
    v[1] = x[1] * thetaCos + tmp[1] * thetaSin;
}

_AFXINL void AfxV3dSlerp(afxV3d v, afxV3d x, afxV3d y, afxReal t)
{
    AfxV3dNormalize(x, x);
    AfxV3dNormalize(y, y);

    afxReal dotProd = AFX_MAX(AFX_MIN(AfxV3dDot(x, y), 1.0f), -1.0f);

    afxReal theta_0 = AfxAcos(dotProd);
    afxReal theta = theta_0 * t;
    afxReal thetaCos = AfxCos(theta);
    afxReal thetaSin = AfxSin(theta);

    afxV3d tmp =
    {
        y[0] - x[0] * dotProd,
        y[1] - x[1] * dotProd,
        y[2] - x[2] * dotProd
    };
    AfxV3dNormalize(tmp, tmp);

    v[0] = x[0] * thetaCos + tmp[0] * thetaSin;
    v[1] = x[1] * thetaCos + tmp[1] * thetaSin;
    v[2] = x[2] * thetaCos + tmp[2] * thetaSin;
}

_AFXINL void AfxV4dSlerp(afxV4d v, afxV4d x, afxV4d y, afxReal t)
{
    AfxV4dNormalize(x, x);
    AfxV4dNormalize(y, y);

    afxReal dotProd = AFX_MAX(AFX_MIN(AfxV4dDot(x, y), 1.0f), -1.0f);

    afxReal theta_0 = AfxAcos(dotProd);
    afxReal theta = theta_0 * t;
    afxReal thetaCos = AfxCos(theta);
    afxReal thetaSin = AfxSin(theta);

    afxV4d tmp =
    {
        y[0] - x[0] * dotProd,
        y[1] - x[1] * dotProd,
        y[2] - x[2] * dotProd,
        y[3] - x[3] * dotProd
    };
    AfxV4dNormalize(tmp, tmp);

    v[0] = x[0] * thetaCos + tmp[0] * thetaSin;
    v[1] = x[1] * thetaCos + tmp[1] * thetaSin;
    v[2] = x[2] * thetaCos + tmp[2] * thetaSin;
    v[3] = x[3] * thetaCos + tmp[3] * thetaSin;
}

////////////////////////////////////////////////////////////////////////////////
// Algebra                                                                    //
////////////////////////////////////////////////////////////////////////////////

// Sum

_AFXINL afxReal AfxV2dSum(afxV2d const v)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    return v[0] + v[1];
}

_AFXINL afxReal AfxV3dSum(afxV3d const v)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    return v[0] + v[1] + v[2];
}

_AFXINL afxReal AfxV4dSum(afxV4d const v)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    return v[0] + v[1] + v[2] + v[3];
}

// Dot

_AFXINL afxReal AfxV2dDot(afxV2d const v, afxV2d const other)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(other);
    return  (v[0] * other[0]) + 
            (v[1] * other[1]);
}

_AFXINL afxReal AfxV3dDot(afxV3d const v, afxV3d const other)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(other);
    return  (v[0] * other[0]) + 
            (v[1] * other[1]) + 
            (v[2] * other[2]);
}

_AFXINL afxReal AfxV4dDot(afxV4d const v, afxV4d const other)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(other);
    return  (v[0] * other[0]) + 
            (v[1] * other[1]) + 
            (v[2] * other[2]) + 
            (v[3] * other[3]);
}

// Dist
// Function to calculate the distance between two points

_AFXINL afxReal AfxV2dDist(afxV2d const v, afxV2d const other)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(other);
    return AfxSqrtf(
                    (v[0] - other[0]) * (v[0] - other[0]) +
                    (v[1] - other[1]) * (v[1] - other[1]));
}

_AFXINL afxReal AfxV3dDist(afxV3d const v, afxV3d const other)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(other);
    return AfxSqrtf(
                    (v[0] - other[0]) * (v[0] - other[0]) + 
                    (v[1] - other[1]) * (v[1] - other[1]) + 
                    (v[2] - other[2]) * (v[2] - other[2]));
}

_AFXINL afxReal AfxV4dDist(afxV4d const v, afxV4d const other)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(other);
    return AfxSqrtf(
                    (v[0] - other[0]) * (v[0] - other[0]) +
                    (v[1] - other[1]) * (v[1] - other[1]) +
                    (v[2] - other[2]) * (v[2] - other[2]) +
                    (v[3] - other[3]) * (v[3] - other[3]));
}

// Cross

_AFXINL void AfxV2dCross(afxV2d v, afxV2d const a, afxV2d const b)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    AFX_ASSERT(v != a);
    AFX_ASSERT(v != b);

    // [ V1.x*V2.y - V1.y*V2.x, V1.x*V2.y - V1.y*V2.x ]

    v[1] = (v[0] = (a[0] * b[1]) - (a[1] * b[0]));
}

_AFXINL void AfxV3dCross(afxV3d v, afxV3d const a, afxV3d const b)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    AFX_ASSERT(v != a);
    AFX_ASSERT(v != b);

    v[0] = (a[1] * b[2]) - (a[2] * b[1]);
    v[1] = (a[2] * b[0]) - (a[0] * b[2]);
    v[2] = (a[0] * b[1]) - (a[1] * b[0]);
}

_AFXINL void AfxV4dCross(afxV4d v, afxV4d const a, afxV4d const b, afxV4d const c)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    AFX_ASSERT(v != a);
    AFX_ASSERT(v != b);

    v[0] = (((b[2] * c[3]) - (b[3] * c[2])) * a[1]) - (((b[1] * c[3]) - (b[3] * c[1])) * a[2]) + (((b[1] * c[2]) - (b[2] * c[1])) * a[3]);
    v[1] = (((b[3] * c[2]) - (b[2] * c[3])) * a[0]) - (((b[3] * c[0]) - (b[0] * c[3])) * a[2]) + (((b[2] * c[0]) - (b[0] * c[2])) * a[3]);
    v[2] = (((b[1] * c[3]) - (b[3] * c[1])) * a[0]) - (((b[0] * c[3]) - (b[3] * c[0])) * a[1]) + (((b[0] * c[1]) - (b[1] * c[0])) * a[3]);
    v[3] = (((b[2] * c[1]) - (b[1] * c[2])) * a[0]) - (((b[2] * c[0]) - (b[0] * c[2])) * a[1]) + (((b[1] * c[0]) - (b[0] * c[1])) * a[2]);
}

// Norm

_AFXINL afxReal AfxV2dSq(afxV2d const v)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    return AfxV2dDot(v, v);
}

_AFXINL afxReal AfxV3dSq(afxV3d const v)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    return AfxV3dDot(v, v);
}

_AFXINL afxReal AfxV4dSq(afxV4d const v)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    return AfxV4dDot(v, v);
}

// Magnitude

_AFXINL afxReal AfxV2dMag(afxV2d const v)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    return AfxSqrtf(AfxV2dSq(v));
}

_AFXINL afxReal AfxV3dMag(afxV3d const v)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    return AfxSqrtf(AfxV3dSq(v));
}

_AFXINL afxReal AfxV4dMag(afxV4d const v)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    return AfxSqrtf(AfxV4dSq(v));
}

// MagnitudeRecip

_AFXINL afxReal AfxV2dMagRecip(afxV2d const v)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    return AfxRsqrtf(AfxV2dSq(v));
}

_AFXINL afxReal AfxV3dMagRecip(afxV3d const v)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    return AfxRsqrtf(AfxV3dSq(v));
}

_AFXINL afxReal AfxV4dMagRecip(afxV4d const v)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    return AfxRsqrtf(AfxV4dSq(v));
}

////////////////////////////////////////////////////////////////////////////////
// VECTOR TRANSFORMATION METHODS                                              //
////////////////////////////////////////////////////////////////////////////////

// PostMultiply
// With post-multiplication, a 4x4 matrix multiplied with a 4x1 column vector took the dot product of each row of the matrix with the vector.

_AFXINL void AfxV2dPostMultiplyM2d(afxV2d v, afxM2d const m, afxV2d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(in);
    AFX_ASSERT(v);

    afxV4d t;
    AfxV2dCopy(t, in); // batch requires a intermediate

    v[0] = (m[0][0] * t[0]) + (m[0][1] * t[1]);
    v[1] = (m[1][0] * t[0]) + (m[1][1] * t[1]);
}

_AFXINL void AfxV3dPostMultiplyM3d(afxV3d v, afxM3d const m, afxV3d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(m);
    AFX_ASSERT(in);

    // With post-multiplication, a 4x4 matrix multiplied with a 4x1 column vector took the dot product of each row of the matrix with the vector.
    afxV4d t;
    AfxV3dCopy(t, in); // batch requires a intermediate

    v[0] = (m[0][0] * t[0]) + (m[0][1] * t[1]) + (m[0][2] * t[2]);
    v[1] = (m[1][0] * t[0]) + (m[1][1] * t[1]) + (m[1][2] * t[2]);
    v[2] = (m[2][0] * t[0]) + (m[2][1] * t[1]) + (m[2][2] * t[2]);
}

_AFXINL void AfxV4dPostMultiplyM4d(afxV4d v, afxM4d const m, afxV4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(m);
    AFX_ASSERT(in);

    // With post-multiplication, a 4x4 matrix multiplied with a 4x1 column vector took the dot product of each row of the matrix with the vector.
    afxV4d t;
    AfxV4dCopy(t, in); // batch requires a intermediate

    v[0] = (m[0][0] * t[0]) + (m[0][1] * t[1]) + (m[0][2] * t[2]) + (m[0][3] * t[3]);
    v[1] = (m[1][0] * t[0]) + (m[1][1] * t[1]) + (m[1][2] * t[2]) + (m[1][3] * t[3]);
    v[2] = (m[2][0] * t[0]) + (m[2][1] * t[1]) + (m[2][2] * t[2]) + (m[2][3] * t[3]);
    v[3] = (m[3][0] * t[0]) + (m[3][1] * t[1]) + (m[3][2] * t[2]) + (m[3][3] * t[3]);
}

_AFXINL void AfxV3dPostMultiplyLtm4d(afxV3d v, afxM4d const m, afxV3d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(m);
    AFX_ASSERT(in);

    // With post-multiplication, a 4x4 matrix multiplied with a 4x1 column vector took the dot product of each row of the matrix with the vector.
    afxV4d t;
    AfxV3dCopy(t, in); // batch requires a intermediate

    v[0] = (m[0][0] * t[0]) + (m[0][1] * t[1]) + (m[0][2] * t[2]);
    v[1] = (m[1][0] * t[0]) + (m[1][1] * t[1]) + (m[1][2] * t[2]);
    v[2] = (m[2][0] * t[0]) + (m[2][1] * t[1]) + (m[2][2] * t[2]);
}

_AFXINL void AfxV4dPostMultiplyM3d(afxV4d v, afxM3d const m, afxV4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(m);
    AFX_ASSERT(in);

    // With post-multiplication, a 4x4 matrix multiplied with a 4x1 column vector took the dot product of each row of the matrix with the vector.
    
    // Compatible with TransposeVectorTransform4x3(in/v, 0.0, m)

    afxV4d t;
    AfxV4dCopy(t, in); // batch requires a intermediate

    v[0] = (m[0][0] * t[0]) + (m[0][1] * t[1]) + (m[0][2] * t[2]);
    v[1] = (m[1][0] * t[0]) + (m[1][1] * t[1]) + (m[1][2] * t[2]);
    v[2] = (m[2][0] * t[0]) + (m[2][1] * t[1]) + (m[2][2] * t[2]);
    v[3] = in[3];
}

_AFXINL void TransposeVectorTransform4x3(afxV3d Dest, float D3, afxM4d const Transform)
{
    // Compatible with TransposeVectorTransform4x3

    Dest[0] = Transform[0][0] * Dest[0] + Transform[0][1] * Dest[1] + Transform[0][2] * Dest[2] + D3 * Transform[3][0];
    Dest[1] = Transform[1][0] * Dest[0] + Transform[1][1] * Dest[1] + Transform[1][2] * Dest[2] + D3 * Transform[3][1];
    Dest[2] = Transform[2][0] * Dest[0] + Transform[2][1] * Dest[1] + Transform[2][2] * Dest[2] + D3 * Transform[3][2];
}

_AFXINL void AfxV2dPostMultiplyAtm4d(afxV2d v, afxM4d const m, afxV2d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(in);
    AFX_ASSERT(v);

    // With post-multiplication, a 4x4 matrix multiplied with a 4x1 column vector took the dot product of each row of the matrix with the vector.
    afxV4d t;
    AfxV3dCopy(t, in); // batch requires a intermediate

    v[0] = (m[0][0] * t[0]) + (m[0][1] * t[1]) + (m[0][2]) + (m[0][3]);
    v[1] = (m[1][0] * t[0]) + (m[1][1] * t[1]) + (m[1][2]) + (m[1][3]);
}

_AFXINL void AfxV3dPostMultiplyAtm4d(afxV3d v, afxM4d const m, afxV3d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(m);
    AFX_ASSERT(in);

    // With post-multiplication, a 4x4 matrix multiplied with a 4x1 column vector took the dot product of each row of the matrix with the vector.
    afxV4d t;
    AfxV3dCopy(t, in); // batch requires a intermediate

    v[0] = (m[0][0] * t[0]) + (m[0][1] * t[1]) + (m[0][2] * t[2]) + (m[0][3]);
    v[1] = (m[1][0] * t[0]) + (m[1][1] * t[1]) + (m[1][2] * t[2]) + (m[1][3]);
    v[2] = (m[2][0] * t[0]) + (m[2][1] * t[1]) + (m[2][2] * t[2]) + (m[2][3]);
}

_AFXINL void AfxV4dPostMultiplyAtm4d(afxV4d v, afxM4d const m, afxV4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(m);
    AFX_ASSERT(in);

    // With post-multiplication, a 4x4 matrix multiplied with a 4x1 column vector took the dot product of each row of the matrix with the vector.
    afxV4d t;
    AfxV4dCopy(t, in); // batch requires a intermediate

    v[0] = (m[0][0] * t[0]) + (m[0][1] * t[1]) + (m[0][2] * t[2]) + (m[0][3]);
    v[1] = (m[1][0] * t[0]) + (m[1][1] * t[1]) + (m[1][2] * t[2]) + (m[1][3]);
    v[2] = (m[2][0] * t[0]) + (m[2][1] * t[1]) + (m[2][2] * t[2]) + (m[2][3]);
    v[3] = 1.f;
}

// PreMultiply

_AFXINL void AfxV2dPreMultiplyM2d(afxV2d v, afxV2d const in, afxM2d const m)
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(m);
    AFX_ASSERT(v);

    // With pre-multiplication, the dot product is with the vector and each column of the matrix (since the matrix is now on the right side of the multiplication operator).
    afxV4d t;
    AfxV2dCopy(t, in); // batch requires a intermediate

    v[0] = (t[0] * m[0][0]) + (t[1] * m[1][0]);
    v[1] = (t[0] * m[0][1]) + (t[1] * m[1][1]);
}

_AFXINL void AfxV3dPreMultiplyM3d(afxV3d v, afxV3d const in, afxM3d const m)
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(m);
    AFX_ASSERT(v);

    // With pre-multiplication, the dot product is with the vector and each column of the matrix (since the matrix is now on the right side of the multiplication operator).
    afxV4d t;
    AfxV3dCopy(t, in); // batch requires a intermediate

    v[0] = (t[0] * m[0][0]) + (t[1] * m[1][0]) + (t[2] * m[2][0]);
    v[1] = (t[0] * m[0][1]) + (t[1] * m[1][1]) + (t[2] * m[2][1]);
    v[2] = (t[0] * m[0][2]) + (t[1] * m[1][2]) + (t[2] * m[2][2]);
}

_AFXINL void AfxV4dPreMultiplyM4d(afxV4d v, afxV4d const in, afxM4d const m)
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(m);
    AFX_ASSERT(v);

    // With pre-multiplication, the dot product is with the vector and each column of the matrix (since the matrix is now on the right side of the multiplication operator).
    afxV4d t;
    AfxV4dCopy(t, in); // batch requires a intermediate

    // Compatible with TransposeVectorTransform4x4(v/in, m)

    v[0] = (t[0] * m[0][0]) + (t[1] * m[1][0]) + (t[2] * m[2][0]) + (t[3] * m[3][0]);
    v[1] = (t[0] * m[0][1]) + (t[1] * m[1][1]) + (t[2] * m[2][1]) + (t[3] * m[3][1]);
    v[2] = (t[0] * m[0][2]) + (t[1] * m[1][2]) + (t[2] * m[2][2]) + (t[3] * m[3][2]);
    v[3] = (t[0] * m[0][3]) + (t[1] * m[1][3]) + (t[2] * m[2][3]) + (t[3] * m[3][3]);
}

// TransformTransposedPoint/Normal/Affine

_AFXINL void AfxV3dPreMultiplyLtm4d(afxV3d v, afxV3d const in, afxM4d const m)
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(m);
    AFX_ASSERT(v);

    // With pre-multiplication, the dot product is with the vector and each column of the matrix (since the matrix is now on the right side of the multiplication operator).
    afxV4d t;
    AfxV3dCopy(t, in); // batch requires a intermediate

    v[0] = (t[0] * m[0][0]) + (t[1] * m[1][0]) + (t[2] * m[2][0]);
    v[1] = (t[0] * m[0][1]) + (t[1] * m[1][1]) + (t[2] * m[2][1]);
    v[2] = (t[0] * m[0][2]) + (t[1] * m[1][2]) + (t[2] * m[2][2]);
}

_AFXINL void AfxV4dPreMultiplyM3d(afxV4d v, afxV4d const in, afxM3d const m)
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(m);
    AFX_ASSERT(v);

    // With pre-multiplication, the dot product is with the vector and each column of the matrix (since the matrix is now on the right side of the multiplication operator).
    afxV4d t;
    AfxV4dCopy(t, in); // batch requires a intermediate

    v[0] = (t[0] * m[0][0]) + (t[1] * m[1][0]) + (t[2] * m[2][0]);
    v[1] = (t[0] * m[0][1]) + (t[1] * m[1][1]) + (t[2] * m[2][1]);
    v[2] = (t[0] * m[0][2]) + (t[1] * m[1][2]) + (t[2] * m[2][2]);
    v[3] = in[3];
}

_AFXINL void AfxV2dPreMultiplyAtm4d(afxV2d v, afxV2d const in, afxM4d const m)
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(m);
    AFX_ASSERT(v);

    // With pre-multiplication, the dot product is with the vector and each column of the matrix (since the matrix is now on the right side of the multiplication operator).
    afxV4d t;
    AfxV2dCopy(t, in); // batch requires a intermediate

    v[0] = (t[0] * m[0][0]) + (t[1] * m[1][0]) + (m[2][0]) + (m[3][0]);
    v[1] = (t[0] * m[0][1]) + (t[1] * m[1][1]) + (m[2][1]) + (m[3][1]);
}

_AFXINL void AfxV3dPreMultiplyAtm4d(afxV3d v, afxV3d const in, afxM4d const m)
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(m);
    AFX_ASSERT(v);

    // With pre-multiplication, the dot product is with the vector and each column of the matrix (since the matrix is now on the right side of the multiplication operator).
    afxV4d t;
    AfxV3dCopy(t, in); // batch requires a intermediate

    v[0] = (t[0] * m[0][0]) + (t[1] * m[1][0]) + (t[2] * m[2][0]) + (m[3][0]);
    v[1] = (t[0] * m[0][1]) + (t[1] * m[1][1]) + (t[2] * m[2][1]) + (m[3][1]);
    v[2] = (t[0] * m[0][2]) + (t[1] * m[1][2]) + (t[2] * m[2][2]) + (m[3][2]);
}

_AFXINL void AfxV4dPreMultiplyAtm4d(afxV4d v, afxV4d const in, afxM4d const m)
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(m);
    AFX_ASSERT(v);

    // With pre-multiplication, the dot product is with the vector and each column of the matrix (since the matrix is now on the right side of the multiplication operator).
    afxV4d t;
    AfxV4dCopy(t, in); // batch requires a intermediate

    v[0] = (t[0] * m[0][0]) + (t[1] * m[1][0]) + (t[2] * m[2][0]) + (m[3][0]);
    v[1] = (t[0] * m[0][1]) + (t[1] * m[1][1]) + (t[2] * m[2][1]) + (m[3][1]);
    v[2] = (t[0] * m[0][2]) + (t[1] * m[1][2]) + (t[2] * m[2][2]) + (m[3][2]);
    v[3] = 1.f;
}

// Assimilate

_AFXINL void AfxAssimilateAtv3d(afxM3d const ltm, afxV4d const atv, afxUnit cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(atv);
    AFX_ASSERT(ltm);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    AfxM3dPostMultiplyV3d(ltm, cnt, in, out);

    for (afxUnit i = 0; i < cnt; i++)
    {
        AfxV3dAdd(out[i], atv, out[i]);
    }
}

_AFXINL void AfxAssimilateAtv4d(afxM3d const ltm, afxV4d const atv, afxUnit cnt, afxV4d const in[], afxV4d out[])
{
    // inspired on AfxAssimilateAtv3d
    afxError err = { 0 };
    AFX_ASSERT(atv);
    AFX_ASSERT(ltm);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    AfxM3dPostMultiplyLtv4d(ltm, cnt, in, out);

    for (afxUnit i = 0; i < cnt; i++)
    {
        AfxV3dAdd(out[i], atv, out[i]);
        out[i][3] = 1.f;
    }
}
