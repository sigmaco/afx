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

#include "qwadro/math/afxMathDefs.h"
#include "qwadro/math/afxVector.h"
#include "qwadro/math/afxMatrix.h"

// Memory layout: row-major

_AFX afxV2d const AFX_V2D_X = { AfxScalar(1), AfxScalar(0) };
_AFX afxV3d const AFX_V3D_X = { AfxScalar(1), AfxScalar(0), AfxScalar(0) };
_AFX afxV4d const AFX_V4D_X = { AfxScalar(1), AfxScalar(0), AfxScalar(0), AfxScalar(0) };
_AFX afxV2d const AFX_V2D_X_NEG = { AfxScalar(-1), AfxScalar(0) };
_AFX afxV3d const AFX_V3D_X_NEG = { AfxScalar(-1), AfxScalar(0), AfxScalar(0) };
_AFX afxV4d const AFX_V4D_X_NEG = { AfxScalar(-1), AfxScalar(0), AfxScalar(0), AfxScalar(0) };

_AFX afxV2d const AFX_V2D_Y = { AfxScalar(0), AfxScalar(1) };
_AFX afxV3d const AFX_V3D_Y = { AfxScalar(0), AfxScalar(1), AfxScalar(0) };
_AFX afxV4d const AFX_V4D_Y = { AfxScalar(0), AfxScalar(1), AfxScalar(0), AfxScalar(0) };
_AFX afxV2d const AFX_V2D_Y_NEG = { AfxScalar(0), AfxScalar(-1) };
_AFX afxV3d const AFX_V3D_Y_NEG = { AfxScalar(0), AfxScalar(-1), AfxScalar(0) };
_AFX afxV4d const AFX_V4D_Y_NEG = { AfxScalar(0), AfxScalar(-1), AfxScalar(0), AfxScalar(0) };

_AFX afxV3d const AFX_V3D_Z = { AfxScalar(0), AfxScalar(0), AfxScalar(1) };
_AFX afxV4d const AFX_V4D_Z = { AfxScalar(0), AfxScalar(0), AfxScalar(1), AfxScalar(0) };
_AFX afxV3d const AFX_V3D_Z_NEG = { AfxScalar(0), AfxScalar(0), AfxScalar(-1) };
_AFX afxV4d const AFX_V4D_Z_NEG = { AfxScalar(0), AfxScalar(0), AfxScalar(-1), AfxScalar(0) };

_AFX afxV4d const AFX_V4D_W = { AfxScalar(0), AfxScalar(0), AfxScalar(0), AfxScalar(1) };
_AFX afxV4d const AFX_V4D_W_NEG = { AfxScalar(0), AfxScalar(0), AfxScalar(0), AfxScalar(-1) };

_AFX afxV4d const AFX_V4D_XYZ = { AfxScalar(1), AfxScalar(1), AfxScalar(1), AfxScalar(0) };

_AFX afxV2d const AFX_V2D_ONE = { AfxScalar(1), AfxScalar(1) };
_AFX afxV3d const AFX_V3D_ONE = { AfxScalar(1), AfxScalar(1), AfxScalar(1) };
_AFX afxV4d const AFX_V4D_ONE = { AfxScalar(1), AfxScalar(1), AfxScalar(1), AfxScalar(1) };

_AFX afxV4d const AFX_V4D_ONE_NEG = { AfxScalar(-1), AfxScalar(-1), AfxScalar(-1), AfxScalar(-1) };

_AFX afxV2d const AFX_V2D_ZERO = { AfxScalar(0), AfxScalar(0) };
_AFX afxV3d const AFX_V3D_ZERO = { AfxScalar(0), AfxScalar(0), AfxScalar(0) };
_AFX afxV4d const AFX_V4D_ZERO = { AfxScalar(0), AfxScalar(0), AfxScalar(0), AfxScalar(0) };

_AFX afxV2d const AFX_V2D_IDENTITY = { AfxScalar(0), AfxScalar(0) };
_AFX afxV3d const AFX_V3D_IDENTITY = { AfxScalar(0), AfxScalar(0), AfxScalar(0) };
_AFX afxV4d const AFX_V4D_IDENTITY = { AfxScalar(0), AfxScalar(0), AfxScalar(0), AfxScalar(1) };


////////////////////////////////////////////////////////////////////////////////
// Initialization                                                             //
////////////////////////////////////////////////////////////////////////////////

_AFXINL void AfxFillV2d(afxV2d v, afxReal value)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    v[0] = value;
    v[1] = value;
}

_AFXINL void AfxFillV3d(afxV3d v, afxReal value)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    v[0] = value;
    v[1] = value;
    v[2] = value;
}

_AFXINL void AfxFillV4d(afxV4d v, afxReal value)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    v[0] = value;
    v[1] = value;
    v[2] = value;
    v[3] = value;
}

// Zero

_AFXINL void AfxV2dZero(afxV2d v)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxV2dCopy(v, AFX_V2D_ZERO);
}

_AFXINL void AfxV3dZero(afxV3d v)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxV3dCopy(v, AFX_V3D_ZERO);
}

_AFXINL void AfxV4dZero(afxV4d v)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxV4dCopy(v, AFX_V4D_ZERO);
}

// Set

_AFXINL void AfxV2dSet(afxV2d v, afxReal x, afxReal y)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    v[0] = x;
    v[1] = y;
}

_AFXINL void AfxV3dSet(afxV3d v, afxReal x, afxReal y, afxReal z)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    v[0] = x;
    v[1] = y;
    v[2] = z;
}

_AFXINL void AfxV4dSet(afxV4d v, afxReal x, afxReal y, afxReal z, afxReal w)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    v[0] = x;
    v[1] = y;
    v[2] = z;
    v[3] = w;
}

// MakeIdentity

_AFXINL void AfxResetV4d(afxV4d v)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
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
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(other);
    return  (AfxScalar(v[0] < other[0] + AFX_EPSILON && v[0] > other[0] - AFX_EPSILON)) +
            (AfxScalar(v[1] < other[1] + AFX_EPSILON && v[1] > other[1] - AFX_EPSILON));
}

_AFXINL afxBool AfxV3dAreEquals(afxV3d const v, afxV3d const other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(other);
    return  (AfxScalar(v[0] < other[0] + AFX_EPSILON && v[0] > other[0] - AFX_EPSILON)) +
            (AfxScalar(v[1] < other[1] + AFX_EPSILON && v[1] > other[1] - AFX_EPSILON)) +
            (AfxScalar(v[2] < other[2] + AFX_EPSILON && v[2] > other[2] - AFX_EPSILON));
}

_AFXINL afxBool AfxV4dAreEquals(afxV4d const v, afxV4d const other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(other);
    return  (AfxScalar(v[0] < other[0] + AFX_EPSILON && v[0] > other[0] - AFX_EPSILON)) +
            (AfxScalar(v[1] < other[1] + AFX_EPSILON && v[1] > other[1] - AFX_EPSILON)) +
            (AfxScalar(v[2] < other[2] + AFX_EPSILON && v[2] > other[2] - AFX_EPSILON)) +
            (AfxScalar(v[3] < other[3] + AFX_EPSILON && v[3] > other[3] - AFX_EPSILON));
}

// IsInBounds

_AFXINL afxBool AfxV2dIsInBounds(afxV2d const v, afxV2d const bounds)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(bounds);
    return (((v[0] <= bounds[0] && v[0] >= -bounds[0]) && (v[1] <= bounds[1] && v[1] >= -bounds[1])) != 0);
}

_AFXINL afxBool AfxV3dIsInBounds(afxV3d const v, afxV3d const bounds)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(bounds);
    return (((v[0] <= bounds[0] && v[0] >= -bounds[0]) && (v[1] <= bounds[1] && v[1] >= -bounds[1]) && (v[2] <= bounds[2] && v[2] >= -bounds[2])) != 0);
}

_AFXINL afxBool AfxV4dIsInBounds(afxV4d const v, afxV4d const bounds)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(bounds);
    return (((v[0] <= bounds[0] && v[0] >= -bounds[0]) && (v[1] <= bounds[1] && v[1] >= -bounds[1]) && (v[2] <= bounds[2] && v[2] >= -bounds[2]) && (v[3] <= bounds[3] && v[3] >= -bounds[3])) != 0);
}

// IsLessOrEqual

_AFXINL afxBool AfxV2dIsLessOrEqual(afxV2d const v, afxV2d const other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(other);
    return (((v[0] <= other[0]) && (v[1] <= other[1])) != 0);
}

_AFXINL afxBool AfxV3dIsLessOrEqual(afxV3d const v, afxV3d const other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(other);
    return (((v[0] <= other[0]) && (v[1] <= other[1]) && (v[2] <= other[2])) != 0);
}

_AFXINL afxBool AfxV4dIsLessOrEqual(afxV4d const v, afxV4d const other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(other);
    return (((v[0] <= other[0]) && (v[1] <= other[1]) && (v[2] <= other[2]) && (v[3] <= other[3])) != 0);
}

// IsLessThan

_AFXINL afxBool AfxV2dIsLessThan(afxV2d const v, afxV2d const other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(other);
    return (((v[0] < other[0]) && (v[1] < other[1])) != 0);
}

_AFXINL afxBool AfxV3dIsLessThan(afxV3d const v, afxV3d const other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(other);
    return (((v[0] < other[0]) && (v[1] < other[1]) && (v[2] < other[2])) != 0);
}

_AFXINL afxBool AfxV4dIsLessThan(afxV4d const v, afxV4d const other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(other);
    return (((v[0] < other[0]) && (v[1] < other[1]) && (v[2] < other[2]) && (v[3] < other[3])) != 0);
}

// IsZero

_AFXINL afxBool AfxV2dIsZero(afxV2d const v)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return (AfxScalar(0) == (v[0] + v[1]));
}

_AFXINL afxBool AfxV3dIsZero(afxV3d const v)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return (AfxScalar(0) == (v[0] + v[1] + v[2]));
}

_AFXINL afxBool AfxV4dIsZero(afxV4d const v)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return (AfxScalar(0) == (v[0] + v[1] + v[2] + v[3]));
}

// IsInfinite

_AFXINL afxBool AfxV2dIsInfinite(afxV2d const v)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return (AfxIsInfinite(v[0]) ? 0xFFFFFFFFU : 0) | (AfxIsInfinite(v[1]) ? 0xFFFFFFFFU : 0);
}

_AFXINL afxBool AfxV3dIsInfinite(afxV3d const v)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return (AfxIsInfinite(v[0]) ? 0xFFFFFFFFU : 0) | (AfxIsInfinite(v[1]) ? 0xFFFFFFFFU : 0) | (AfxIsInfinite(v[2]) ? 0xFFFFFFFFU : 0);
}

_AFXINL afxBool AfxV4dIsInfinite(afxV4d const v)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return ((AfxIsInfinite(v[0])) ? 0xFFFFFFFFU : 0) | (AfxIsInfinite(v[1]) ? 0xFFFFFFFFU : 0) | (AfxIsInfinite(v[2]) ? 0xFFFFFFFFU : 0) | (AfxIsInfinite(v[3]) ? 0xFFFFFFFFU : 0);
}

// IsNormalized

_AFXINL afxBool AfxV3dIsNormalized(afxV3d const v)
{
    // From Crytek

    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    afxReal f = AfxV3dMag(v);
    return f >= 0.95f && f <= 1.05f;
}

// IsNan

_AFXINL afxBool AfxV2dIsNan(afxV2d const v)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return (AfxIsNan(v[0]) || AfxIsNan(v[1]));
}

_AFXINL afxBool AfxV3dIsNan(afxV3d const v)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return (AfxIsNan(v[0]) || AfxIsNan(v[1]) || AfxIsNan(v[2]));
}

_AFXINL afxBool AfxV4dIsNan(afxV4d const v)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return (AfxIsNan(v[0]) || AfxIsNan(v[1]) || AfxIsNan(v[2]) || AfxIsNan(v[3]));
}



////////////////////////////////////////////////////////////////////////////////
// Transferance                                                               //
////////////////////////////////////////////////////////////////////////////////

// Copy

_AFXINL void AfxV2dCopy(afxV2d v, afxV2d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    AfxAssertDiffSoft(v, in);
    v[0] = in[0];
    v[1] = in[1];
}

_AFXINL void AfxV3dCopy(afxV3d v, afxV3d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    AfxAssertDiffSoft(v, in);
    v[0] = in[0];
    v[1] = in[1];
    v[2] = in[2];
}

_AFXINL void AfxV4dCopy(afxV4d v, afxV4d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    AfxAssertDiffSoft(v, in);
    v[0] = in[0];
    v[1] = in[1];
    v[2] = in[2];
    v[3] = in[3];
}

_AFXINL void AfxCopyArrayedV2d(afxNat cnt, afxV2d const in[], afxV2d out[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(in);
    AfxAssertDiffSoft(out, in);

    for (afxNat i = 0; i < cnt; i++)
        AfxV2dCopy(out[i], in[i]);
}

_AFXINL void AfxCopyArrayedV3d(afxNat cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(in);
    AfxAssertDiffSoft(out, in);

    for (afxNat i = 0; i < cnt; i++)
        AfxV3dCopy(out[i], in[i]);
}

_AFXINL void AfxCopyArrayedV4d(afxNat cnt, afxV4d const in[], afxV4d out[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(in);
    AfxAssertDiffSoft(out, in);

    for (afxNat i = 0; i < cnt; i++)
        AfxV4dCopy(out[i], in[i]);
}

_AFXINL void AfxV3dCopyAtv2d(afxV3d v, afxV2d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    AfxAssertDiffSoft(v, in);
    v[0] = in[0];
    v[1] = in[1];
    v[2] = AfxScalar(1);
}

_AFXINL void AfxV4dCopyAtv3d(afxV4d v, afxV3d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    AfxAssertDiffSoft(v, in);
    v[0] = in[0];
    v[1] = in[1];
    v[2] = in[2];
    v[3] = AfxScalar(1);
}

// Make (Combined Copy & Promote/Demote operations)

_AFXINL void AfxV3dCopyV2d(afxV3d v, afxV2d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    AfxAssertDiffSoft(v, in);
    v[0] = in[0];
    v[1] = in[1];
    v[2] = AfxScalar(0);
}

_AFXINL void AfxV4dCopyV2d(afxV4d v, afxV2d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    AfxAssertDiffSoft(v, in);
    v[0] = in[0];
    v[1] = in[1];
    v[2] = AfxScalar(0);
    v[3] = AfxScalar(0);
}

_AFXINL void AfxV4dCopyV3d(afxV4d v, afxV3d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    AfxAssertDiffSoft(v, in);
    v[0] = in[0];
    v[1] = in[1];
    v[2] = in[2];
    v[3] = AfxScalar(0);
}

// Swap

_AFXINL void AfxSwapV2d(afxV2d v, afxV2d other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(other);
    AfxAssert(v != other);
    afxV2d tmp;
    AfxV2dCopy(tmp, v);
    AfxV2dCopy(other, tmp);
    AfxV2dCopy(v, other);
}

_AFXINL void AfxSwapV3d(afxV3d v, afxV3d other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(other);
    AfxAssert(v != other);
    afxV3d tmp;
    AfxV3dCopy(tmp, v);
    AfxV3dCopy(other, tmp);
    AfxV3dCopy(v, other);
}

_AFXINL void AfxSwapV4d(afxV4d v, afxV4d other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(other);
    AfxAssert(v != other);
    afxV4d tmp;
    AfxV4dCopy(tmp, v);
    AfxV4dCopy(other, tmp);
    AfxV4dCopy(v, other);
}

////////////////////////////////////////////////////////////////////////////////
// Clamping                                                                   //
////////////////////////////////////////////////////////////////////////////////

// Clamp

_AFXINL void AfxClampV2d(afxV2d v, afxV2d const in, afxV2d const min, afxV2d const max)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    AfxAssert(min);
    AfxAssert(max);
    AfxAssert(min[0] <= max[0]);
    AfxAssert(min[1] <= max[1]);

    AfxMaxV2d(v, min, in);
    AfxMinV2d(v, max, v);
}

_AFXINL void AfxClampV3d(afxV3d v, afxV3d const in, afxV3d const min, afxV3d const max)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    AfxAssert(min);
    AfxAssert(max);
    AfxAssert(min[0] <= max[0]);
    AfxAssert(min[1] <= max[1]);
    AfxAssert(min[2] <= max[2]);

    AfxMaxV3d(v, min, in);
    AfxMinV3d(v, max, v);
}

_AFXINL void AfxClampV4d(afxV4d v, afxV4d const in, afxV4d const min, afxV4d const max)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    AfxAssert(min);
    AfxAssert(max);
    AfxAssert(min[0] <= max[0]);
    AfxAssert(min[1] <= max[1]);
    AfxAssert(min[2] <= max[2]);
    AfxAssert(min[3] <= max[3]);

    AfxMaxV4d(v, min, in);
    AfxMinV4d(v, max, v);
}

// Truncate

_AFXINL void AfxTruncateV2d(afxV2d v, afxV2d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);

    v[0] = AfxIsNan(in[0]) ? 0x7FC00000 : ((AfxAbs(in[0]) < 8388608.0f) ? (afxReal)(((afxInt32)(in[0]))) : in[0]);
    v[1] = AfxIsNan(in[1]) ? 0x7FC00000 : ((AfxAbs(in[1]) < 8388608.0f) ? (afxReal)(((afxInt32)(in[1]))) : in[1]);
}

_AFXINL void AfxTruncateV3d(afxV3d v, afxV3d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);

    v[0] = AfxIsNan(in[0]) ? 0x7FC00000 : ((AfxAbs(in[0]) < 8388608.0f) ? (afxReal)(((afxInt32)(in[0]))) : in[0]);
    v[1] = AfxIsNan(in[1]) ? 0x7FC00000 : ((AfxAbs(in[1]) < 8388608.0f) ? (afxReal)(((afxInt32)(in[1]))) : in[1]);
    v[2] = AfxIsNan(in[2]) ? 0x7FC00000 : ((AfxAbs(in[2]) < 8388608.0f) ? (afxReal)(((afxInt32)(in[2]))) : in[2]);
}

_AFXINL void AfxTruncateV4d(afxV4d v, afxV4d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);

    v[0] = AfxIsNan(in[0]) ? 0x7FC00000 : ((AfxAbs(in[0]) < 8388608.0f) ? (afxReal)(((afxInt32)(in[0]))) : in[0]);
    v[1] = AfxIsNan(in[1]) ? 0x7FC00000 : ((AfxAbs(in[1]) < 8388608.0f) ? (afxReal)(((afxInt32)(in[1]))) : in[1]);
    v[2] = AfxIsNan(in[2]) ? 0x7FC00000 : ((AfxAbs(in[2]) < 8388608.0f) ? (afxReal)(((afxInt32)(in[2]))) : in[2]);
    v[3] = AfxIsNan(in[3]) ? 0x7FC00000 : ((AfxAbs(in[3]) < 8388608.0f) ? (afxReal)(((afxInt32)(in[3]))) : in[3]);
}

// Saturate

_AFXINL void AfxSaturateV2d(afxV2d v, afxV2d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);

    AfxClampV2d(v, in, AFX_V4D_ZERO, AFX_V4D_ONE);
}

_AFXINL void AfxSaturateV3d(afxV3d v, afxV3d const in)
{
    // Should be compatible with XMVECTOR XMVectorSaturate(FXMVECTOR V)

    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);

    AfxClampV3d(v, in, AFX_V4D_ZERO, AFX_V4D_ONE);
}

_AFXINL void AfxSaturateV4d(afxV4d v, afxV4d const in)
{
    // Should be compatible with XMVECTOR XMVectorSaturate(FXMVECTOR V)

    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);

    AfxClampV4d(v, in, AFX_V4D_ZERO, AFX_V4D_ONE);
}

// Mini

_AFXINL void AfxMinV2d(afxV2d v, afxV2d const a, afxV2d const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);
    v[0] = a[0] < b[0] ? a[0] : b[0];
    v[1] = a[1] < b[1] ? a[1] : b[1];
}

_AFXINL void AfxMinV3d(afxV3d v, afxV3d const a, afxV3d const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);
    v[0] = a[0] < b[0] ? a[0] : b[0];
    v[1] = a[1] < b[1] ? a[1] : b[1];
    v[2] = a[2] < b[2] ? a[2] : b[2];
}

_AFXINL void AfxMinV4d(afxV4d v, afxV4d const a, afxV4d const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);
    v[0] = a[0] < b[0] ? a[0] : b[0];
    v[1] = a[1] < b[1] ? a[1] : b[1];
    v[2] = a[2] < b[2] ? a[2] : b[2];
    v[3] = a[3] < b[3] ? a[3] : b[3];
}

// Maxi

_AFXINL void AfxMaxV2d(afxV2d v, afxV2d const a, afxV2d const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);
    v[0] = a[0] > b[0] ? a[0] : b[0];
    v[1] = a[1] > b[1] ? a[1] : b[1];
}

_AFXINL void AfxMaxV3d(afxV3d v, afxV3d const a, afxV3d const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);
    v[0] = a[0] > b[0] ? a[0] : b[0];
    v[1] = a[1] > b[1] ? a[1] : b[1];
    v[2] = a[2] > b[2] ? a[2] : b[2];
}

_AFXINL void AfxMaxV4d(afxV4d v, afxV4d const a, afxV4d const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);
    v[0] = a[0] > b[0] ? a[0] : b[0];
    v[1] = a[1] > b[1] ? a[1] : b[1];
    v[2] = a[2] > b[2] ? a[2] : b[2];
    v[3] = a[3] > b[3] ? a[3] : b[3];
}

// Abs

_AFXINL void AfxAbsV2d(afxV2d v, afxV2d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxAbs(in[0]);
    v[1] = AfxAbs(in[1]);
}

_AFXINL void AfxAbsV3d(afxV3d v, afxV3d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxAbs(in[0]);
    v[1] = AfxAbs(in[1]);
    v[2] = AfxAbs(in[2]);
}

_AFXINL void AfxAbsV4d(afxV4d v, afxV4d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxAbs(in[0]);
    v[1] = AfxAbs(in[1]);
    v[2] = AfxAbs(in[2]);
    v[3] = AfxAbs(in[3]);
}

// Ceil

_AFXINL void AfxCeilV2d(afxV2d v, afxV2d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxCeilf(in[0]);
    v[1] = AfxCeilf(in[1]);
}

_AFXINL void AfxCeilV3d(afxV3d v, afxV3d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxCeilf(in[0]);
    v[1] = AfxCeilf(in[1]);
    v[2] = AfxCeilf(in[2]);
}

_AFXINL void AfxCeilV4d(afxV4d v, afxV4d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxCeilf(in[0]);
    v[1] = AfxCeilf(in[1]);
    v[2] = AfxCeilf(in[2]);
    v[3] = AfxCeilf(in[3]);
}

// Floor

_AFXINL void AfxFloorV2d(afxV2d v, afxV2d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxFloorf(in[0]);
    v[1] = AfxFloorf(in[1]);
}

_AFXINL void AfxFloorV3d(afxV3d v, afxV3d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxFloorf(in[0]);
    v[1] = AfxFloorf(in[1]);
    v[2] = AfxFloorf(in[2]);
}

_AFXINL void AfxFloorV4d(afxV4d v, afxV4d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxFloorf(in[0]);
    v[1] = AfxFloorf(in[1]);
    v[2] = AfxFloorf(in[2]);
    v[3] = AfxFloorf(in[3]);
}

// Recip

_AFXINL void AfxRecipV2d(afxV2d v, afxV2d const in)
{
    // Should be compatible with XMVECTOR XMVectorReciprocal(FXMVECTOR V)

    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    AfxDivV2d(v, AFX_V2D_ONE, in);
}

_AFXINL void AfxRecipV3d(afxV3d v, afxV3d const in)
{
    // Should be compatible with XMVECTOR XMVectorReciprocal(FXMVECTOR V)

    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    AfxDivV3d(v, AFX_V3D_ONE, in);
}

_AFXINL void AfxRecipV4d(afxV4d v, afxV4d const in)
{
    // Should be compatible with XMVECTOR XMVectorReciprocal(FXMVECTOR V)

    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    AfxDivV4d(v, AFX_V4D_ONE, in);
}

// Normalize

_AFXINL afxReal AfxV2dNormalize(afxV2d v, afxV2d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    afxReal len = AfxV2dMag(in);

    if (len > 0)
        len = 1.f / len;

    AfxV2dScale(v, in, len);
    return len;
}

_AFXINL afxReal AfxV3dNormalize(afxV3d v, afxV3d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    afxReal len = AfxV3dMag(in);

    if (len > 0)
        len = 1.f / len;

    AfxV3dScale(v, in, len);
    return len;
}

_AFXINL afxReal AfxV4dNormalize(afxV4d v, afxV4d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    afxReal len = AfxV4dMag(in);

    if (len > 0)
        len = 1.f / len;

    AfxV4dScale(v, in, len);
    return len;
}

// NormalizeV*d

_AFXINL afxReal AfxV3dNormalizeV4d(afxV3d v, afxV4d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    afxReal len = AfxV4dMag(in);

    if (len > 0)
        len = 1.f / len;

    AfxV3dScale(v, in, len);
    return len;
}

_AFXINL afxReal AfxV4dNormalizeV3d(afxV4d v, afxV3d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    afxReal len = AfxV3dMag(in);

    if (len > 0)
        len = 1.f / len;

    AfxV4dScale(v, in, len);
    return len;
}

// NormalizeEst

_AFXINL afxReal AfxV2dNormalizeEstimated(afxV2d v, afxV2d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    afxReal len = AfxV2dMagRecip(in);
    AfxV2dScale(v, in, len);
    return len;
}

_AFXINL afxReal AfxV3dNormalizeEstimated(afxV3d v, afxV3d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    afxReal len = AfxV3dMagRecip(in);
    AfxV3dScale(v, in, len);
    return len;
}

_AFXINL afxReal AfxV4dNormalizeEstimated(afxV4d v, afxV4d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    afxReal len = AfxV4dMagRecip(in);
    AfxV4dScale(v, in, len);
    return len;
}

_AFXINL void AfxNormalizeArrayedV2d(afxNat cnt, afxV2d const in[], afxV2d out[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
        AfxV2dNormalize(out[i], in[i]);
}

_AFXINL void AfxNormalizeArrayedV3d(afxNat cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
        AfxV3dNormalize(out[i], in[i]);
}

_AFXINL void AfxNormalizeArrayedV4d(afxNat cnt, afxV4d const in[], afxV4d out[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
        AfxV4dNormalize(out[i], in[i]);
}

_AFXINL void AfxZeroOrNormalizeArrayedV2d(afxNat cnt, afxV2d const in[], afxV2d out[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxReal len = AfxV2dMag(in[i]);

        if (len <= 0.0000099999997f)
            AfxV2dZero(out[i]);
        else
            AfxV2dScale(out[i], in[i], 1.f / len);
    }
}

_AFXINL void AfxZeroOrNormalizeArrayedV3d(afxNat cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxReal len = AfxV3dMag(in[i]);
        
        if (len <= 0.0000099999997f)
            AfxV3dZero(out[i]);
        else
            AfxV3dScale(out[i], in[i], 1.f / len);
    }
}

_AFXINL void AfxZeroOrNormalizeArrayedV4d(afxNat cnt, afxV4d const in[], afxV4d out[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxReal len = AfxV4dMag(in[i]);

        if (len <= 0.0000099999997f)
            AfxV4dZero(out[i]);
        else
            AfxV4dScale(out[i], in[i], 1.f / len);
    }
}

////////////////////////////////////////////////////////////////////////////////
// Arithmetica                                                                //
////////////////////////////////////////////////////////////////////////////////

// Add

_AFXINL void AfxV2dAdd(afxV2d v, afxV2d const a, afxV2d const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);
    v[0] = a[0] + b[0];
    v[1] = a[1] + b[1];
}

_AFXINL void AfxV3dAdd(afxV3d v, afxV3d const a, afxV3d const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);
    v[0] = a[0] + b[0];
    v[1] = a[1] + b[1];
    v[2] = a[2] + b[2];
}

_AFXINL void AfxV4dAdd(afxV4d v, afxV4d const a, afxV4d const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);
    v[0] = a[0] + b[0];
    v[1] = a[1] + b[1];
    v[2] = a[2] + b[2];
    v[3] = a[3] + b[3];
}

// AddScaled

_AFXINL void AfxAddScaledV2d(afxV2d v, afxV2d const a, afxV2d const b, afxReal lambda)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    v[0] = a[0] + b[0] * lambda;
    v[1] = a[1] + b[1] * lambda;
}

_AFXINL void AfxAddScaledV3d(afxV3d v, afxV3d const a, afxV3d const b, afxReal lambda)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    v[0] = a[0] + b[0] * lambda;
    v[1] = a[1] + b[1] * lambda;
    v[2] = a[2] + b[2] * lambda;
}

_AFXINL void AfxAddScaledV4d(afxV4d v, afxV4d const a, afxV4d const b, afxReal lambda)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    v[0] = a[0] + b[0] * lambda;
    v[1] = a[1] + b[1] * lambda;
    v[2] = a[2] + b[2] * lambda;
    v[3] = a[3] + b[3] * lambda;
}

// Sub

_AFXINL void AfxV2dSub(afxV2d v, afxV2d const a, afxV2d const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);
    v[0] = a[0] - b[0];
    v[1] = a[1] - b[1];
}

_AFXINL void AfxV3dSub(afxV3d v, afxV3d const a, afxV3d const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);
    v[0] = a[0] - b[0];
    v[1] = a[1] - b[1];
    v[2] = a[2] - b[2];
}

_AFXINL void AfxV4dSub(afxV4d v, afxV4d const a, afxV4d const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);
    v[0] = a[0] - b[0];
    v[1] = a[1] - b[1];
    v[2] = a[2] - b[2];
    v[3] = a[3] - b[3];
}

// Div

_AFXINL void AfxDivV2d(afxV2d v, afxV2d const a, afxV2d const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);
    v[0] = a[0] / b[0];
    v[1] = a[1] / b[1];
}

_AFXINL void AfxDivV3d(afxV3d v, afxV3d const a, afxV3d const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);
    v[0] = a[0] / b[0];
    v[1] = a[1] / b[1];
    v[2] = a[2] / b[2];
}

_AFXINL void AfxDivV4d(afxV4d v, afxV4d const a, afxV4d const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);
    v[0] = a[0] / b[0];
    v[1] = a[1] / b[1];
    v[2] = a[2] / b[2];
    v[3] = a[3] / b[3];
}

// Mod

_AFXINL void AfxModV2d(afxV2d v, afxV2d const a, afxV2d const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);

    AfxDivV2d(v, a, b);
    AfxTruncateV2d(v, v);
    AfxResubV2d(v, b, v, a);
}

_AFXINL void AfxModV3d(afxV3d v, afxV3d const a, afxV3d const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);

    AfxDivV3d(v, a, b);
    AfxTruncateV3d(v, v);
    AfxResubV3d(v, b, v, a);
}

_AFXINL void AfxModV4d(afxV4d v, afxV4d const a, afxV4d const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);

    AfxDivV4d(v, a, b);
    AfxTruncateV4d(v, v);
    AfxResubV4d(v, b, v, a);
}

// Scale

_AFXINL void AfxV2dScale(afxV2d v, afxV2d const a, afxReal lambda)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    v[0] = a[0] * lambda;
    v[1] = a[1] * lambda;
}

_AFXINL void AfxV3dScale(afxV3d v, afxV3d const a, afxReal lambda)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    v[0] = a[0] * lambda;
    v[1] = a[1] * lambda;
    v[2] = a[2] * lambda;
}

_AFXINL void AfxV4dScale(afxV4d v, afxV4d const a, afxReal lambda)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    v[0] = a[0] * lambda;
    v[1] = a[1] * lambda;
    v[2] = a[2] * lambda;
    v[3] = a[3] * lambda;
}

// Half

_AFXINL void AfxHalfV2d(afxV2d v, afxV2d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = in[0] * 0.5f;
    v[1] = in[1] * 0.5f;
}

_AFXINL void AfxHalfV3d(afxV3d v, afxV3d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = in[0] * 0.5f;
    v[1] = in[1] * 0.5f;
    v[2] = in[2] * 0.5f;
}

_AFXINL void AfxHalfV4d(afxV4d v, afxV4d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = in[0] * 0.5f;
    v[1] = in[1] * 0.5f;
    v[2] = in[2] * 0.5f;
    v[3] = in[3] * 0.5f;
}

// Multiply

_AFXINL void AfxMultiplyV2d(afxV2d v, afxV2d const a, afxV2d const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);
    v[0] = a[0] * b[0];
    v[1] = a[1] * b[1];
}

_AFXINL void AfxMultiplyV3d(afxV3d v, afxV3d const a, afxV3d const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);
    v[0] = a[0] * b[0];
    v[1] = a[1] * b[1];
    v[2] = a[2] * b[2];
}

_AFXINL void AfxMultiplyV4d(afxV4d v, afxV4d const a, afxV4d const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);
    v[0] = a[0] * b[0];
    v[1] = a[1] * b[1];
    v[2] = a[2] * b[2];
    v[3] = a[3] * b[3];
}

// Mad

_AFXINL void AfxMadV2d(afxV2d v, afxV2d const in, afxV2d const mul, afxV2d const plus)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    AfxAssert(mul);
    AfxAssert(plus);
    v[0] = in[0] * mul[0] + plus[0];
    v[1] = in[1] * mul[1] + plus[1];
}

_AFXINL void AfxMadV3d(afxV3d v, afxV3d const in, afxV3d const mul, afxV3d const plus)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    AfxAssert(mul);
    AfxAssert(plus);
    v[0] = in[0] * mul[0] + plus[0];
    v[1] = in[1] * mul[1] + plus[1];
    v[2] = in[2] * mul[2] + plus[2];
}

_AFXINL void AfxMadV4d(afxV4d v, afxV4d const in, afxV4d const mul, afxV4d const plus)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    AfxAssert(mul);
    AfxAssert(plus);
    v[0] = in[0] * mul[0] + plus[0];
    v[1] = in[1] * mul[1] + plus[1];
    v[2] = in[2] * mul[2] + plus[2];
    v[3] = in[3] * mul[3] + plus[3];
}

// Combine

_AFXINL void AfxCombineV2d(afxV2d v, afxReal lambda1, afxV2d const a, afxReal lambda2, afxV2d const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);

    v[0] = lambda1 * a[0] + lambda2 * b[0];
    v[1] = lambda1 * a[1] + lambda2 * b[1];
}

_AFXINL void AfxCombineV3d(afxV3d v, afxReal lambda1, afxV3d const a, afxReal lambda2, afxV3d const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);

    v[0] = lambda1 * a[0] + lambda2 * b[0];
    v[1] = lambda1 * a[1] + lambda2 * b[1];
    v[2] = lambda1 * a[2] + lambda2 * b[2];
}

_AFXINL void AfxCombineV4d(afxV4d v, afxReal lambda1, afxV4d const a, afxReal lambda2, afxV4d const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);

    v[0] = lambda1 * a[0] + lambda2 * b[0];
    v[1] = lambda1 * a[1] + lambda2 * b[1];
    v[2] = lambda1 * a[2] + lambda2 * b[2];
    v[3] = lambda1 * a[3] + lambda2 * b[3];
}

// Resub

_AFXINL void AfxResubV2d(afxV2d v, afxV2d const a, afxV2d const b, afxV2d const c)
{
    // Should be compatible with AfxXMVectorNegativeMultiplySubtract

    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);
    AfxAssert(c);

    v[0] = c[0] - (a[0] * b[0]);
    v[1] = c[1] - (a[1] * b[1]);
}

_AFXINL void AfxResubV3d(afxV3d v, afxV3d const a, afxV3d const b, afxV3d const c)
{
    // Should be compatible with AfxXMVectorNegativeMultiplySubtract

    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);
    AfxAssert(c);

    v[0] = c[0] - (a[0] * b[0]);
    v[1] = c[1] - (a[1] * b[1]);
    v[2] = c[2] - (a[2] * b[2]);
}

_AFXINL void AfxResubV4d(afxV4d v, afxV4d const a, afxV4d const b, afxV4d const c)
{
    // Should be compatible with AfxXMVectorNegativeMultiplySubtract

    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);
    AfxAssert(c);

    v[0] = c[0] - (a[0] * b[0]);
    v[1] = c[1] - (a[1] * b[1]);
    v[2] = c[2] - (a[2] * b[2]);
    v[3] = c[3] - (a[3] * b[3]);
}

// Lerp

_AFXINL void AfxLerpV2d(afxV2d v, afxV2d const x, afxV2d const y, afxReal t)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(x);
    AfxAssert(y);

    // x + t * (y - x)
#if 0
    v[0] = x[0] + t * (y[0] - x[0]);
    v[1] = x[1] + t * (y[1] - x[1]);
#else
    v[0] = x[0] * (1.f - t) + t * y[0];
    v[1] = x[1] * (1.f - t) + t * y[1];
#endif
}

_AFXINL void AfxLerpV3d(afxV3d v, afxV3d const x, afxV3d const y, afxReal t)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(x);
    AfxAssert(y);

    // x + t * (y - x)

    // x + t * (y - x)
#if 0
    v[0] = x[0] + t * (y[0] - x[0]);
    v[1] = x[1] + t * (y[1] - x[1]);
    v[2] = x[2] + t * (y[2] - x[2]);
#else
    v[0] = x[0] * (1.f - t) + t * y[0];
    v[1] = x[1] * (1.f - t) + t * y[1];
    v[2] = x[2] * (1.f - t) + t * y[2];
#endif
}

_AFXINL void AfxLerpV4d(afxV4d v, afxV4d const x, afxV4d const y, afxReal t)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(x);
    AfxAssert(y);
    
    // x + t * (y - x)
#if 0
    v[0] = x[0] + t * (y[0] - x[0]);
    v[1] = x[1] + t * (y[1] - x[1]);
    v[2] = x[2] + t * (y[2] - x[2]);
    v[3] = x[3] + t * (y[3] - x[3]);
#else
    v[0] = x[0] * (1.f - t) + t * y[0];
    v[1] = x[1] * (1.f - t) + t * y[1];
    v[2] = x[2] * (1.f - t) + t * y[2];
    v[3] = x[3] * (1.f - t) + t * y[3];
#endif
}

_AFXINL void AfxMixV2d(afxV2d v, afxV2d const x, afxV2d const y, afxReal t)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(x);
    AfxAssert(y);
    AfxLerpV2d(v, x, y, t);
}

_AFXINL void AfxMixV3d(afxV3d v, afxV3d const x, afxV3d const y, afxReal t)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(x);
    AfxAssert(y);
    AfxLerpV3d(v, x, y, t);
}

_AFXINL void AfxMixV4d(afxV4d v, afxV4d const x, afxV4d const y, afxReal t)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(x);
    AfxAssert(y);
    AfxLerpV4d(v, x, y, t);
}

////////////////////////////////////////////////////////////////////////////////
// Algebra                                                                    //
////////////////////////////////////////////////////////////////////////////////

// Square root

_AFXINL void AfxSqrtV2d(afxV2d v, afxV2d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxSqrtf(in[0]);
    v[1] = AfxSqrtf(in[1]);
}

_AFXINL void AfxSqrtV3d(afxV3d v, afxV3d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxSqrtf(in[0]);
    v[1] = AfxSqrtf(in[1]);
    v[2] = AfxSqrtf(in[2]);
}

_AFXINL void AfxSqrtV4d(afxV4d v, afxV4d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxSqrtf(in[0]);
    v[1] = AfxSqrtf(in[1]);
    v[2] = AfxSqrtf(in[2]);
    v[3] = AfxSqrtf(in[3]);
}

// Reciprocal square root

_AFXINL void AfxRsqrtV2d(afxV2d v, afxV2d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxRsqrtf(in[0]);
    v[1] = AfxRsqrtf(in[1]);
}

_AFXINL void AfxRsqrtV3d(afxV3d v, afxV3d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxRsqrtf(in[0]);
    v[1] = AfxRsqrtf(in[1]);
    v[2] = AfxRsqrtf(in[2]);
}

_AFXINL void AfxRsqrtV4d(afxV4d v, afxV4d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxRsqrtf(in[0]);
    v[1] = AfxRsqrtf(in[1]);
    v[2] = AfxRsqrtf(in[2]);
    v[3] = AfxRsqrtf(in[3]);
}

// Sine

_AFXINL void AfxSinV2d(afxV2d v, afxV2d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxSinf(in[0]);
    v[1] = AfxSinf(in[1]);
}

_AFXINL void AfxSinV3d(afxV3d v, afxV3d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxSinf(in[0]);
    v[1] = AfxSinf(in[1]);
    v[2] = AfxSinf(in[2]);
}

_AFXINL void AfxSinV4d(afxV4d v, afxV4d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxSinf(in[0]);
    v[1] = AfxSinf(in[1]);
    v[2] = AfxSinf(in[2]);
    v[3] = AfxSinf(in[3]);
}

// Cosine

_AFXINL void AfxCosV2d(afxV2d v, afxV2d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxCosf(in[0]);
    v[1] = AfxCosf(in[1]);
}

_AFXINL void AfxCosV3d(afxV3d v, afxV3d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxCosf(in[0]);
    v[1] = AfxCosf(in[1]);
    v[2] = AfxCosf(in[2]);
}

_AFXINL void AfxCosV4d(afxV4d v, afxV4d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxCosf(in[0]);
    v[1] = AfxCosf(in[1]);
    v[2] = AfxCosf(in[2]);
    v[3] = AfxCosf(in[3]);
}

// Arc cosine

_AFXINL void AfxAcosV2d(afxV2d v, afxV2d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxAcosf(in[0]);
    v[1] = AfxAcosf(in[1]);
}

_AFXINL void AfxAcosV3d(afxV3d v, afxV3d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxAcosf(in[0]);
    v[1] = AfxAcosf(in[1]);
    v[2] = AfxAcosf(in[2]);
}

_AFXINL void AfxAcosV4d(afxV4d v, afxV4d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxAcosf(in[0]);
    v[1] = AfxAcosf(in[1]);
    v[2] = AfxAcosf(in[2]);
    v[3] = AfxAcosf(in[3]);
}

// Tangent

_AFXINL void AfxTanV2d(afxV2d v, afxV2d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxTanf(in[0]);
    v[1] = AfxTanf(in[1]);
}

_AFXINL void AfxTanV3d(afxV3d v, afxV3d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxTanf(in[0]);
    v[1] = AfxTanf(in[1]);
    v[2] = AfxTanf(in[2]);
}

_AFXINL void AfxTanV4d(afxV4d v, afxV4d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxTanf(in[0]);
    v[1] = AfxTanf(in[1]);
    v[2] = AfxTanf(in[2]);
    v[3] = AfxTanf(in[3]);
}

// Arc tangent

_AFXINL void AfxAtanV2d(afxV2d v, afxV2d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxAtanf(in[0]);
    v[1] = AfxAtanf(in[1]);
}

_AFXINL void AfxAtanV3d(afxV3d v, afxV3d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxAtanf(in[0]);
    v[1] = AfxAtanf(in[1]);
    v[2] = AfxAtanf(in[2]);
}

_AFXINL void AfxAtanV4d(afxV4d v, afxV4d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxAtanf(in[0]);
    v[1] = AfxAtanf(in[1]);
    v[2] = AfxAtanf(in[2]);
    v[3] = AfxAtanf(in[3]);
}

// Log

_AFXINL void AfxLogV2d(afxV2d v, afxV2d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxLogf(in[0]);
    v[1] = AfxLogf(in[1]);
}

_AFXINL void AfxLogV3d(afxV3d v, afxV3d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxLogf(in[0]);
    v[1] = AfxLogf(in[1]);
    v[2] = AfxLogf(in[2]);
}

_AFXINL void AfxLogV4d(afxV4d v, afxV4d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxLogf(in[0]);
    v[1] = AfxLogf(in[1]);
    v[2] = AfxLogf(in[2]);
    v[3] = AfxLogf(in[3]);
}

// Log2

_AFXINL void AfxV2dLog2(afxV2d v, afxV2d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxLog2f(in[0]);
    v[1] = AfxLog2f(in[1]);
}

_AFXINL void AfxV3dLog2(afxV3d v, afxV3d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxLog2f(in[0]);
    v[1] = AfxLog2f(in[1]);
    v[2] = AfxLog2f(in[2]);
}

_AFXINL void AfxV4dLog2(afxV4d v, afxV4d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxLog2f(in[0]);
    v[1] = AfxLog2f(in[1]);
    v[2] = AfxLog2f(in[2]);
    v[3] = AfxLog2f(in[3]);
}

// Log10

_AFXINL void AfxV2dLog10(afxV2d v, afxV2d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxLog10f(in[0]);
    v[1] = AfxLog10f(in[1]);
}

_AFXINL void AfxV3dLog10(afxV3d v, afxV3d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxLog10f(in[0]);
    v[1] = AfxLog10f(in[1]);
    v[2] = AfxLog10f(in[2]);
}

_AFXINL void AfxV4dLog10(afxV4d v, afxV4d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxLog10f(in[0]);
    v[1] = AfxLog10f(in[1]);
    v[2] = AfxLog10f(in[2]);
    v[3] = AfxLog10f(in[3]);
}

// Pow

_AFXINL void AfxV2dPow(afxV2d v, afxV2d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxPowf(in[0], in[0]);
    v[1] = AfxPowf(in[1], in[1]);
}

_AFXINL void AfxV3dPow(afxV3d v, afxV3d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxPowf(in[0], in[0]);
    v[1] = AfxPowf(in[1], in[1]);
    v[2] = AfxPowf(in[2], in[2]);
}

_AFXINL void AfxV4dPow(afxV4d v, afxV4d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxPowf(in[0], in[0]);
    v[1] = AfxPowf(in[1], in[1]);
    v[2] = AfxPowf(in[2], in[2]);
    v[3] = AfxPowf(in[3], in[3]);
}

// Exp

_AFXINL void AfxV2dExp(afxV2d v, afxV2d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxExpf(in[0]);
    v[1] = AfxExpf(in[1]);
}

_AFXINL void AfxV3dExp(afxV3d v, afxV3d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxExpf(in[0]);
    v[1] = AfxExpf(in[1]);
    v[2] = AfxExpf(in[2]);
}

_AFXINL void AfxV4dExp(afxV4d v, afxV4d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxExpf(in[0]);
    v[1] = AfxExpf(in[1]);
    v[2] = AfxExpf(in[2]);
    v[3] = AfxExpf(in[3]);
}

// Exp2

_AFXINL void AfxV2dExp2(afxV2d v, afxV2d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxExp2f(in[0]);
    v[1] = AfxExp2f(in[1]);
}

_AFXINL void AfxV3dExp2(afxV3d v, afxV3d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxExp2f(in[0]);
    v[1] = AfxExp2f(in[1]);
    v[2] = AfxExp2f(in[2]);
}

_AFXINL void AfxV4dExp2(afxV4d v, afxV4d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxExp2f(in[0]);
    v[1] = AfxExp2f(in[1]);
    v[2] = AfxExp2f(in[2]);
    v[3] = AfxExp2f(in[3]);
}

// Exp10

_AFXINL void AfxV2dExp10(afxV2d v, afxV2d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxExp10f(in[0]);
    v[1] = AfxExp10f(in[1]);
}

_AFXINL void AfxV3dExp10(afxV3d v, afxV3d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxExp10f(in[0]);
    v[1] = AfxExp10f(in[1]);
    v[2] = AfxExp10f(in[2]);
}

_AFXINL void AfxV4dExp10(afxV4d v, afxV4d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxExp10f(in[0]);
    v[1] = AfxExp10f(in[1]);
    v[2] = AfxExp10f( in[2]);
    v[3] = AfxExp10f(in[3]);
}

// Negate

_AFXINL void AfxV2dNeg(afxV2d v, afxV2d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = -(in[0]);
    v[1] = -(in[1]);
}

_AFXINL void AfxV3dNeg(afxV3d v, afxV3d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = -(in[0]);
    v[1] = -(in[1]);
    v[2] = -(in[2]);
}

_AFXINL void AfxV4dNeg(afxV4d v, afxV4d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = -(in[0]);
    v[1] = -(in[1]);
    v[2] = -(in[2]);
    v[3] = -(in[3]);
}

_AFXINL void AfxV4dNegAffine(afxV4d v, afxV4d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = -(in[0]);
    v[1] = -(in[1]);
    v[2] = -(in[2]);
    v[3] =  (in[3]);
}

// Sum

_AFXINL afxReal AfxV2dSum(afxV2d const v)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return v[0] + v[1];
}

_AFXINL afxReal AfxV3dSum(afxV3d const v)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return v[0] + v[1] + v[2];
}

_AFXINL afxReal AfxV4dSum(afxV4d const v)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return v[0] + v[1] + v[2] + v[3];
}

// Dot

_AFXINL afxReal AfxV2dDot(afxV2d const v, afxV2d const other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(other);
    return (v[0] * other[0]) + (v[1] * other[1]);
}

_AFXINL afxReal AfxV3dDot(afxV3d const v, afxV3d const other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(other);
    return (v[0] * other[0]) + (v[1] * other[1]) + (v[2] * other[2]);
}

_AFXINL afxReal AfxV4dDot(afxV4d const v, afxV4d const other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(other);
    return (v[0] * other[0]) + (v[1] * other[1]) + (v[2] * other[2]) + (v[3] * other[3]);
}

// Cross

_AFXINL void AfxV2dCross(afxV2d v, afxV2d const a, afxV2d const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);
    AfxAssert(v != a);
    AfxAssert(v != b);

    // [ V1.x*V2.y - V1.y*V2.x, V1.x*V2.y - V1.y*V2.x ]

    v[1] = (v[0] = (a[0] * b[1]) - (a[1] * b[0]));
}

_AFXINL void AfxV3dCross(afxV3d v, afxV3d const a, afxV3d const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);
    AfxAssert(v != a);
    AfxAssert(v != b);

    v[0] = (a[1] * b[2]) - (a[2] * b[1]);
    v[1] = (a[2] * b[0]) - (a[0] * b[2]);
    v[2] = (a[0] * b[1]) - (a[1] * b[0]);
}

_AFXINL void AfxV4dCross(afxV4d v, afxV4d const a, afxV4d const b, afxV4d const c)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);
    AfxAssert(v != a);
    AfxAssert(v != b);

    v[0] = (((b[2] * c[3]) - (b[3] * c[2])) * a[1]) - (((b[1] * c[3]) - (b[3] * c[1])) * a[2]) + (((b[1] * c[2]) - (b[2] * c[1])) * a[3]);
    v[1] = (((b[3] * c[2]) - (b[2] * c[3])) * a[0]) - (((b[3] * c[0]) - (b[0] * c[3])) * a[2]) + (((b[2] * c[0]) - (b[0] * c[2])) * a[3]);
    v[2] = (((b[1] * c[3]) - (b[3] * c[1])) * a[0]) - (((b[0] * c[3]) - (b[3] * c[0])) * a[1]) + (((b[0] * c[1]) - (b[1] * c[0])) * a[3]);
    v[3] = (((b[2] * c[1]) - (b[1] * c[2])) * a[0]) - (((b[2] * c[0]) - (b[0] * c[2])) * a[1]) + (((b[1] * c[0]) - (b[0] * c[1])) * a[2]);
}

// Norm

_AFXINL afxReal AfxV2dSq(afxV2d const v)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return AfxV2dDot(v, v);
}

_AFXINL afxReal AfxV3dSq(afxV3d const v)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return AfxV3dDot(v, v);
}

_AFXINL afxReal AfxV4dSq(afxV4d const v)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return AfxV4dDot(v, v);
}

// Magnitude

_AFXINL afxReal AfxV2dMag(afxV2d const v)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return AfxSqrtf(AfxV2dSq(v));
}

_AFXINL afxReal AfxV3dMag(afxV3d const v)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return AfxSqrtf(AfxV3dSq(v));
}

_AFXINL afxReal AfxV4dMag(afxV4d const v)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return AfxSqrtf(AfxV4dSq(v));
}

// MagnitudeRecip

_AFXINL afxReal AfxV2dMagRecip(afxV2d const v)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return AfxRsqrtf(AfxV2dSq(v));
}

_AFXINL afxReal AfxV3dMagRecip(afxV3d const v)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return AfxRsqrtf(AfxV3dSq(v));
}

_AFXINL afxReal AfxV4dMagRecip(afxV4d const v)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return AfxRsqrtf(AfxV4dSq(v));
}

////////////////////////////////////////////////////////////////////////////////
// VECTOR TRANSFORMATION METHODS                                              //
////////////////////////////////////////////////////////////////////////////////

// PostMultiply
// With post-multiplication, a 4x4 matrix multiplied with a 4x1 column vector took the dot product of each row of the matrix with the vector.

_AFXINL void AfxV2dPostMultiplyM2d(afxV2d v, afxM2d const m, afxV2d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(in);
    AfxAssert(v);

    afxV4d t;
    AfxV2dCopy(t, in); // batch requires a intermediate

    v[0] = (m[0][0] * t[0]) + (m[0][1] * t[1]);
    v[1] = (m[1][0] * t[0]) + (m[1][1] * t[1]);
}

_AFXINL void AfxV3dPostMultiplyM3d(afxV3d v, afxM3d const m, afxV3d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(m);
    AfxAssert(in);

    // With post-multiplication, a 4x4 matrix multiplied with a 4x1 column vector took the dot product of each row of the matrix with the vector.
    afxV4d t;
    AfxV3dCopy(t, in); // batch requires a intermediate

    v[0] = (m[0][0] * t[0]) + (m[0][1] * t[1]) + (m[0][2] * t[2]);
    v[1] = (m[1][0] * t[0]) + (m[1][1] * t[1]) + (m[1][2] * t[2]);
    v[2] = (m[2][0] * t[0]) + (m[2][1] * t[1]) + (m[2][2] * t[2]);
}

_AFXINL void AfxV4dPostMultiplyM4d(afxV4d v, afxM4d const m, afxV4d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(m);
    AfxAssert(in);

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
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(m);
    AfxAssert(in);

    // With post-multiplication, a 4x4 matrix multiplied with a 4x1 column vector took the dot product of each row of the matrix with the vector.
    afxV4d t;
    AfxV3dCopy(t, in); // batch requires a intermediate

    v[0] = (m[0][0] * t[0]) + (m[0][1] * t[1]) + (m[0][2] * t[2]);
    v[1] = (m[1][0] * t[0]) + (m[1][1] * t[1]) + (m[1][2] * t[2]);
    v[2] = (m[2][0] * t[0]) + (m[2][1] * t[1]) + (m[2][2] * t[2]);
}

_AFXINL void AfxV4dPostMultiplyM3d(afxV4d v, afxM3d const m, afxV4d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(m);
    AfxAssert(in);

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
    // Compatible with TransposeVectorTransform4x3(float *Dest, float D3, const float *Transform)

    Dest[0] = Transform[0][0] * Dest[0] + Transform[0][1] * Dest[1] + Transform[0][2] * Dest[2] + D3 * Transform[3][0];
    Dest[1] = Transform[1][0] * Dest[0] + Transform[1][1] * Dest[1] + Transform[1][2] * Dest[2] + D3 * Transform[3][1];
    Dest[2] = Transform[2][0] * Dest[0] + Transform[2][1] * Dest[1] + Transform[2][2] * Dest[2] + D3 * Transform[3][2];
}

_AFXINL void AfxV2dPostMultiplyAtm4d(afxV2d v, afxM4d const m, afxV2d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(in);
    AfxAssert(v);

    // With post-multiplication, a 4x4 matrix multiplied with a 4x1 column vector took the dot product of each row of the matrix with the vector.
    afxV4d t;
    AfxV3dCopy(t, in); // batch requires a intermediate

    v[0] = (m[0][0] * t[0]) + (m[0][1] * t[1]) + (m[0][2]) + (m[0][3]);
    v[1] = (m[1][0] * t[0]) + (m[1][1] * t[1]) + (m[1][2]) + (m[1][3]);
}

_AFXINL void AfxV3dPostMultiplyAtm4d(afxV3d v, afxM4d const m, afxV3d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(m);
    AfxAssert(in);

    // With post-multiplication, a 4x4 matrix multiplied with a 4x1 column vector took the dot product of each row of the matrix with the vector.
    afxV4d t;
    AfxV3dCopy(t, in); // batch requires a intermediate

    v[0] = (m[0][0] * t[0]) + (m[0][1] * t[1]) + (m[0][2] * t[2]) + (m[0][3]);
    v[1] = (m[1][0] * t[0]) + (m[1][1] * t[1]) + (m[1][2] * t[2]) + (m[1][3]);
    v[2] = (m[2][0] * t[0]) + (m[2][1] * t[1]) + (m[2][2] * t[2]) + (m[2][3]);
}

_AFXINL void AfxV4dPostMultiplyAtm4d(afxV4d v, afxM4d const m, afxV4d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(m);
    AfxAssert(in);

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
    afxError err = AFX_ERR_NONE;
    AfxAssert(in);
    AfxAssert(m);
    AfxAssert(v);

    // With pre-multiplication, the dot product is with the vector and each column of the matrix (since the matrix is now on the right side of the multiplication operator).
    afxV4d t;
    AfxV2dCopy(t, in); // batch requires a intermediate

    v[0] = (t[0] * m[0][0]) + (t[1] * m[1][0]);
    v[1] = (t[0] * m[0][1]) + (t[1] * m[1][1]);
}

_AFXINL void AfxV3dPreMultiplyM3d(afxV3d v, afxV3d const in, afxM3d const m)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(in);
    AfxAssert(m);
    AfxAssert(v);

    // With pre-multiplication, the dot product is with the vector and each column of the matrix (since the matrix is now on the right side of the multiplication operator).
    afxV4d t;
    AfxV3dCopy(t, in); // batch requires a intermediate

    v[0] = (t[0] * m[0][0]) + (t[1] * m[1][0]) + (t[2] * m[2][0]);
    v[1] = (t[0] * m[0][1]) + (t[1] * m[1][1]) + (t[2] * m[2][1]);
    v[2] = (t[0] * m[0][2]) + (t[1] * m[1][2]) + (t[2] * m[2][2]);
}

_AFXINL void AfxV4dPreMultiplyM4d(afxV4d v, afxV4d const in, afxM4d const m)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(in);
    AfxAssert(m);
    AfxAssert(v);

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
    afxError err = AFX_ERR_NONE;
    AfxAssert(in);
    AfxAssert(m);
    AfxAssert(v);

    // With pre-multiplication, the dot product is with the vector and each column of the matrix (since the matrix is now on the right side of the multiplication operator).
    afxV4d t;
    AfxV3dCopy(t, in); // batch requires a intermediate

    v[0] = (t[0] * m[0][0]) + (t[1] * m[1][0]) + (t[2] * m[2][0]);
    v[1] = (t[0] * m[0][1]) + (t[1] * m[1][1]) + (t[2] * m[2][1]);
    v[2] = (t[0] * m[0][2]) + (t[1] * m[1][2]) + (t[2] * m[2][2]);
}

_AFXINL void AfxV4dPreMultiplyM3d(afxV4d v, afxV4d const in, afxM3d const m)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(in);
    AfxAssert(m);
    AfxAssert(v);

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
    afxError err = AFX_ERR_NONE;
    AfxAssert(in);
    AfxAssert(m);
    AfxAssert(v);

    // With pre-multiplication, the dot product is with the vector and each column of the matrix (since the matrix is now on the right side of the multiplication operator).
    afxV4d t;
    AfxV2dCopy(t, in); // batch requires a intermediate

    v[0] = (t[0] * m[0][0]) + (t[1] * m[1][0]) + (m[2][0]) + (m[3][0]);
    v[1] = (t[0] * m[0][1]) + (t[1] * m[1][1]) + (m[2][1]) + (m[3][1]);
}

_AFXINL void AfxV3dPreMultiplyAtm4d(afxV3d v, afxV3d const in, afxM4d const m)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(in);
    AfxAssert(m);
    AfxAssert(v);

    // With pre-multiplication, the dot product is with the vector and each column of the matrix (since the matrix is now on the right side of the multiplication operator).
    afxV4d t;
    AfxV3dCopy(t, in); // batch requires a intermediate

    v[0] = (t[0] * m[0][0]) + (t[1] * m[1][0]) + (t[2] * m[2][0]) + (m[3][0]);
    v[1] = (t[0] * m[0][1]) + (t[1] * m[1][1]) + (t[2] * m[2][1]) + (m[3][1]);
    v[2] = (t[0] * m[0][2]) + (t[1] * m[1][2]) + (t[2] * m[2][2]) + (m[3][2]);
}

_AFXINL void AfxV4dPreMultiplyAtm4d(afxV4d v, afxV4d const in, afxM4d const m)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(in);
    AfxAssert(m);
    AfxAssert(v);

    // With pre-multiplication, the dot product is with the vector and each column of the matrix (since the matrix is now on the right side of the multiplication operator).
    afxV4d t;
    AfxV4dCopy(t, in); // batch requires a intermediate

    v[0] = (t[0] * m[0][0]) + (t[1] * m[1][0]) + (t[2] * m[2][0]) + (m[3][0]);
    v[1] = (t[0] * m[0][1]) + (t[1] * m[1][1]) + (t[2] * m[2][1]) + (m[3][1]);
    v[2] = (t[0] * m[0][2]) + (t[1] * m[1][2]) + (t[2] * m[2][2]) + (m[3][2]);
    v[3] = 1.f;
}

// Assimilate

_AFXINL void AfxAssimilateAtv3d(afxM3d const ltm, afxV4d const atv, afxNat cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = NIL;
    AfxAssert(atv);
    AfxAssert(ltm);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    AfxM3dPostMultiplyV3d(ltm, cnt, in, out);

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxV3dAdd(out[i], atv, out[i]);
    }
}

_AFXINL void AfxAssimilateAtv4d(afxM3d const ltm, afxV4d const atv, afxNat cnt, afxV4d const in[], afxV4d out[])
{
    // inspired on AfxAssimilateAtv3d
    afxError err = NIL;
    AfxAssert(atv);
    AfxAssert(ltm);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    AfxM3dPostMultiplyLtv4d(ltm, cnt, in, out);

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxV3dAdd(out[i], atv, out[i]);
        out[i][3] = 1.f;
    }
}
