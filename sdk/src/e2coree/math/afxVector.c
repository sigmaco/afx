/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#include "afx/math/afxMathDefs.h"
#include "afx/math/afxReal.h"
#include "afx/math/afxVector.h"
#include "afx/math/afxMatrix.h"

// Memory layout: row-major

_AFX afxV2d const AFX_V2D_10 = { AfxScalar(1), AfxScalar(0) };
_AFX afxV3d const AFX_V3D_100 = { AfxScalar(1), AfxScalar(0), AfxScalar(0) };
_AFX afxV4d const AFX_V4D_1000 = { AfxScalar(1), AfxScalar(0), AfxScalar(0), AfxScalar(0) };
_AFX afxV2d const AFX_V2D_10_NEG = { AfxScalar(-1), AfxScalar(0) };
_AFX afxV3d const AFX_V3D_100_NEG = { AfxScalar(-1), AfxScalar(0), AfxScalar(0) };
_AFX afxV4d const AFX_V4D_1000_NEG = { AfxScalar(-1), AfxScalar(0), AfxScalar(0), AfxScalar(0) };

_AFX afxV2d const AFX_V2D_01 = { AfxScalar(0), AfxScalar(1) };
_AFX afxV3d const AFX_V3D_010 = { AfxScalar(0), AfxScalar(1), AfxScalar(0) };
_AFX afxV4d const AFX_V4D_010 = { AfxScalar(0), AfxScalar(1), AfxScalar(0), AfxScalar(0) };
_AFX afxV2d const AFX_V2D_01_NEG = { AfxScalar(0), AfxScalar(-1) };
_AFX afxV3d const AFX_V3D_010_NEG = { AfxScalar(0), AfxScalar(-1), AfxScalar(0) };
_AFX afxV4d const AFX_V4D_0100_NEG = { AfxScalar(0), AfxScalar(-1), AfxScalar(0), AfxScalar(0) };

_AFX afxV3d const AFX_V3D_001 = { AfxScalar(0), AfxScalar(0), AfxScalar(1) };
_AFX afxV4d const AFX_V4D_0010 = { AfxScalar(0), AfxScalar(0), AfxScalar(1), AfxScalar(0) };
_AFX afxV3d const AFX_V3D_001_NEG = { AfxScalar(0), AfxScalar(0), AfxScalar(-1) };
_AFX afxV4d const AFX_V4D_0010_NEG = { AfxScalar(0), AfxScalar(0), AfxScalar(-1), AfxScalar(0) };

_AFX afxV4d const AFX_V4D_0001 = { AfxScalar(0), AfxScalar(0), AfxScalar(0), AfxScalar(1) };
_AFX afxV4d const AFX_V4D_0001_NEG = { AfxScalar(0), AfxScalar(0), AfxScalar(0), AfxScalar(-1) };

_AFX afxV4d const AFX_V4D_1110 = { AfxScalar(1), AfxScalar(1), AfxScalar(1), AfxScalar(0) };

_AFX afxV2d const AFX_V2D_11 = { AfxScalar(1), AfxScalar(1) };
_AFX afxV3d const AFX_V3D_111 = { AfxScalar(1), AfxScalar(1), AfxScalar(1) };
_AFX afxV4d const AFX_V4D_1111 = { AfxScalar(1), AfxScalar(1), AfxScalar(1), AfxScalar(1) };

_AFX afxV4d const AFX_V4D_1111_NEG = { AfxScalar(-1), AfxScalar(-1), AfxScalar(-1), AfxScalar(-1) };

_AFX afxV2d const AFX_V2D_00 = { AfxScalar(0), AfxScalar(0) };
_AFX afxV3d const AFX_V3D_000 = { AfxScalar(0), AfxScalar(0), AfxScalar(0) };
_AFX afxV4d const AFX_V4D_0000 = { AfxScalar(0), AfxScalar(0), AfxScalar(0), AfxScalar(0) };


////////////////////////////////////////////////////////////////////////////////
// Initialization                                                             //
////////////////////////////////////////////////////////////////////////////////

_AFXINL void AfxFillV2d(afxReal v[2], afxReal value)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    v[0] = value;
    v[1] = value;
}

_AFXINL void AfxFillV3d(afxReal v[3], afxReal value)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    v[0] = value;
    v[1] = value;
    v[2] = value;
}

_AFXINL void AfxFillV4d(afxReal v[4], afxReal value)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    v[0] = value;
    v[1] = value;
    v[2] = value;
    v[3] = value;
}

// Zero

_AFXINL void AfxZeroV2d(afxReal v[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxCopyV2d(v, AFX_V2D_00);
}

_AFXINL void AfxZeroV3d(afxReal v[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxCopyV3d(v, AFX_V3D_000);
}

_AFXINL void AfxZeroV4d(afxReal v[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxCopyV4d(v, AFX_V4D_0000);
}

// Set

_AFXINL void AfxSetV2d(afxReal v[2], afxReal x, afxReal y)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    v[0] = x;
    v[1] = y;
}

_AFXINL void AfxSetV3d(afxReal v[3], afxReal x, afxReal y, afxReal z)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    v[0] = x;
    v[1] = y;
    v[2] = z;
}

_AFXINL void AfxSetV4d(afxReal v[4], afxReal x, afxReal y, afxReal z, afxReal w)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    v[0] = x;
    v[1] = y;
    v[2] = z;
    v[3] = w;
}

// MakeIdentity

_AFXINL void AfxResetV4d(afxReal v[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxCopyV4d(v, AFX_V4D_0001);
}

////////////////////////////////////////////////////////////////////////////////
// Logic                                                                      //
////////////////////////////////////////////////////////////////////////////////

// IsIdentity

_AFXINL afxBool AfxV4dIsIdentity(afxReal const v[4])
{
    return (v[0] + v[1] + v[2]) == 0 && (v[3] == 1);
}

// AreEquals

_AFXINL afxBool AfxV2dAreEquals(afxReal const v[2], afxReal const other[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(other);
    return  (AfxScalar(v[0] < other[0] + AFX_EPSILON && v[0] > other[0] - AFX_EPSILON)) +
            (AfxScalar(v[1] < other[1] + AFX_EPSILON && v[1] > other[1] - AFX_EPSILON));
}

_AFXINL afxBool AfxV3dAreEquals(afxReal const v[3], afxReal const other[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(other);
    return  (AfxScalar(v[0] < other[0] + AFX_EPSILON && v[0] > other[0] - AFX_EPSILON)) +
            (AfxScalar(v[1] < other[1] + AFX_EPSILON && v[1] > other[1] - AFX_EPSILON)) +
            (AfxScalar(v[2] < other[2] + AFX_EPSILON && v[2] > other[2] - AFX_EPSILON));
}

_AFXINL afxBool AfxV4dAreEquals(afxReal const v[4], afxReal const other[4])
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

_AFXINL afxBool AfxV2dIsInBounds(afxReal const v[2], afxReal const bounds[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(bounds);
    return (((v[0] <= bounds[0] && v[0] >= -bounds[0]) && (v[1] <= bounds[1] && v[1] >= -bounds[1])) != 0);
}

_AFXINL afxBool AfxV3dIsInBounds(afxReal const v[3], afxReal const bounds[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(bounds);
    return (((v[0] <= bounds[0] && v[0] >= -bounds[0]) && (v[1] <= bounds[1] && v[1] >= -bounds[1]) && (v[2] <= bounds[2] && v[2] >= -bounds[2])) != 0);
}

_AFXINL afxBool AfxV4dIsInBounds(afxReal const v[4], afxReal const bounds[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(bounds);
    return (((v[0] <= bounds[0] && v[0] >= -bounds[0]) && (v[1] <= bounds[1] && v[1] >= -bounds[1]) && (v[2] <= bounds[2] && v[2] >= -bounds[2]) && (v[3] <= bounds[3] && v[3] >= -bounds[3])) != 0);
}

// IsLessOrEqual

_AFXINL afxBool AfxV2dIsLessOrEqual(afxReal const v[2], afxReal const other[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(other);
    return (((v[0] <= other[0]) && (v[1] <= other[1])) != 0);
}

_AFXINL afxBool AfxV3dIsLessOrEqual(afxReal const v[3], afxReal const other[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(other);
    return (((v[0] <= other[0]) && (v[1] <= other[1]) && (v[2] <= other[2])) != 0);
}

_AFXINL afxBool AfxV4dIsLessOrEqual(afxReal const v[4], afxReal const other[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(other);
    return (((v[0] <= other[0]) && (v[1] <= other[1]) && (v[2] <= other[2]) && (v[3] <= other[3])) != 0);
}

// IsLessThan

_AFXINL afxBool AfxV2dIsLessThan(afxReal const v[2], afxReal const other[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(other);
    return (((v[0] < other[0]) && (v[1] < other[1])) != 0);
}

_AFXINL afxBool AfxV3dIsLessThan(afxReal const v[3], afxReal const other[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(other);
    return (((v[0] < other[0]) && (v[1] < other[1]) && (v[2] < other[2])) != 0);
}

_AFXINL afxBool AfxV4dIsLessThan(afxReal const v[4], afxReal const other[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(other);
    return (((v[0] < other[0]) && (v[1] < other[1]) && (v[2] < other[2]) && (v[3] < other[3])) != 0);
}

// IsZero

_AFXINL afxBool AfxV2dIsZero(afxReal const v[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return (AfxScalar(0) == (v[0] + v[1]));
}

_AFXINL afxBool AfxV3dIsZero(afxReal const v[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return (AfxScalar(0) == (v[0] + v[1] + v[2]));
}

_AFXINL afxBool AfxV4dIsZero(afxReal const v[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return (AfxScalar(0) == (v[0] + v[1] + v[2] + v[3]));
}

// IsInfinite

_AFXINL afxBool AfxV2dIsInfinite(afxReal const v[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return (AfxIsInfinite(v[0]) ? 0xFFFFFFFFU : 0) | (AfxIsInfinite(v[1]) ? 0xFFFFFFFFU : 0);
}

_AFXINL afxBool AfxV3dIsInfinite(afxReal const v[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return (AfxIsInfinite(v[0]) ? 0xFFFFFFFFU : 0) | (AfxIsInfinite(v[1]) ? 0xFFFFFFFFU : 0) | (AfxIsInfinite(v[2]) ? 0xFFFFFFFFU : 0);
}

_AFXINL afxBool AfxV4dIsInfinite(afxReal const v[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return (AfxIsInfinite(v[0]) ? 0xFFFFFFFFU : 0) | (AfxIsInfinite(v[1]) ? 0xFFFFFFFFU : 0) | (AfxIsInfinite(v[2]) ? 0xFFFFFFFFU : 0) | (AfxIsInfinite(v[3]) ? 0xFFFFFFFFU : 0);
}

// IsNormalized

_AFXINL afxBool AfxV3dIsNormalized(afxReal const v[3])
{
    // From Crytek

    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    afxReal f = AfxMagV3d(v);
    return f >= 0.95f && f <= 1.05f;
}

// IsNan

_AFXINL afxBool AfxV2dIsNan(afxReal const v[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return (AfxIsNan(v[0]) || AfxIsNan(v[1]));
}

_AFXINL afxBool AfxV3dIsNan(afxReal const v[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return (AfxIsNan(v[0]) || AfxIsNan(v[1]) || AfxIsNan(v[2]));
}

_AFXINL afxBool AfxV4dIsNan(afxReal const v[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return (AfxIsNan(v[0]) || AfxIsNan(v[1]) || AfxIsNan(v[2]) || AfxIsNan(v[3]));
}



////////////////////////////////////////////////////////////////////////////////
// Transferance                                                               //
////////////////////////////////////////////////////////////////////////////////

// Copy

_AFXINL void AfxCopyV2d(afxReal v[2], afxReal const in[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    AfxAssertDiffSoft(v, in);
    v[0] = in[0];
    v[1] = in[1];
}

_AFXINL void AfxCopyV3d(afxReal v[3], afxReal const in[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    AfxAssertDiffSoft(v, in);
    v[0] = in[0];
    v[1] = in[1];
    v[2] = in[2];
}

_AFXINL void AfxCopyV4d(afxReal v[4], afxReal const in[4])
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

_AFXINL void AfxCopyV2dArray(afxReal v[][2], afxReal const in[][2], afxNat cnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    AfxAssertDiffSoft(v, in);

    for (afxNat i = 0; i < cnt; i++)
        AfxCopyV2d(v[i], in[i]);
}

_AFXINL void AfxCopyV3dArray(afxReal v[][3], afxReal const in[][3], afxNat cnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    AfxAssertDiffSoft(v, in);

    for (afxNat i = 0; i < cnt; i++)
        AfxCopyV3d(v[i], in[i]);
}

_AFXINL void AfxCopyV4dArray(afxReal v[][4], afxReal const in[][4], afxNat cnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    AfxAssertDiffSoft(v, in);

    for (afxNat i = 0; i < cnt; i++)
        AfxCopyV4d(v[i], in[i]);
}

_AFXINL void AfxV3dFromAffineV2d(afxReal v[3], afxReal const in[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    AfxAssertDiffSoft(v, in);
    v[0] = in[0];
    v[1] = in[1];
    v[2] = AfxScalar(1);
}

_AFXINL void AfxV4dFromAffineV3d(afxReal v[4], afxReal const in[3])
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

_AFXINL void AfxV3dFromLinearV2d(afxReal v[3], afxReal const in[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    AfxAssertDiffSoft(v, in);
    v[0] = in[0];
    v[1] = in[1];
    v[2] = AfxScalar(0);
}

_AFXINL void AfxV4dFromV2d(afxReal v[4], afxReal const in[2])
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

_AFXINL void AfxV4dFromV3d(afxReal v[4], afxReal const in[3])
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

_AFXINL void AfxSwapV2d(afxReal v[2], afxReal other[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(other);
    AfxAssert(v != other);
    afxV2d tmp;
    AfxCopyV2d(tmp, v);
    AfxCopyV2d(other, tmp);
    AfxCopyV2d(v, other);
}

_AFXINL void AfxSwapV3d(afxReal v[3], afxReal other[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(other);
    AfxAssert(v != other);
    afxV3d tmp;
    AfxCopyV3d(tmp, v);
    AfxCopyV3d(other, tmp);
    AfxCopyV3d(v, other);
}

_AFXINL void AfxSwapV4d(afxReal v[4], afxReal other[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(other);
    AfxAssert(v != other);
    afxV4d tmp;
    AfxCopyV4d(tmp, v);
    AfxCopyV4d(other, tmp);
    AfxCopyV4d(v, other);
}

////////////////////////////////////////////////////////////////////////////////
// Clamping                                                                   //
////////////////////////////////////////////////////////////////////////////////

// Clamp

_AFXINL void AfxClampV2d(afxReal v[2], afxReal const in[2], afxReal const mini[2], afxReal const maxi[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    AfxAssert(mini);
    AfxAssert(maxi);
    AfxAssert(mini[0] <= maxi[0]);
    AfxAssert(mini[1] <= maxi[1]);

    AfxMaxiV2d(v, mini, in);
    AfxMiniV2d(v, maxi, v);
}

_AFXINL void AfxClampV3d(afxReal v[3], afxReal const in[3], afxReal const mini[3], afxReal const maxi[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    AfxAssert(mini);
    AfxAssert(maxi);
    AfxAssert(mini[0] <= maxi[0]);
    AfxAssert(mini[1] <= maxi[1]);
    AfxAssert(mini[2] <= maxi[2]);

    AfxMaxiV3d(v, mini, in);
    AfxMiniV3d(v, maxi, v);
}

_AFXINL void AfxClampV4d(afxReal v[4], afxReal const in[4], afxReal const mini[4], afxReal const maxi[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    AfxAssert(mini);
    AfxAssert(maxi);
    AfxAssert(mini[0] <= maxi[0]);
    AfxAssert(mini[1] <= maxi[1]);
    AfxAssert(mini[2] <= maxi[2]);
    AfxAssert(mini[3] <= maxi[3]);

    AfxMaxiV4d(v, mini, in);
    AfxMiniV4d(v, maxi, v);
}

// Truncate

_AFXINL void AfxTruncateV2d(afxReal v[2], afxReal const in[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);

    v[0] = AfxIsNan(in[0]) ? 0x7FC00000 : ((AfxAbs(in[0]) < 8388608.0f) ? (afxReal)(((afxInt32)(in[0]))) : in[0]);
    v[1] = AfxIsNan(in[1]) ? 0x7FC00000 : ((AfxAbs(in[1]) < 8388608.0f) ? (afxReal)(((afxInt32)(in[1]))) : in[1]);
}

_AFXINL void AfxTruncateV3d(afxReal v[3], afxReal const in[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);

    v[0] = AfxIsNan(in[0]) ? 0x7FC00000 : ((AfxAbs(in[0]) < 8388608.0f) ? (afxReal)(((afxInt32)(in[0]))) : in[0]);
    v[1] = AfxIsNan(in[1]) ? 0x7FC00000 : ((AfxAbs(in[1]) < 8388608.0f) ? (afxReal)(((afxInt32)(in[1]))) : in[1]);
    v[2] = AfxIsNan(in[2]) ? 0x7FC00000 : ((AfxAbs(in[2]) < 8388608.0f) ? (afxReal)(((afxInt32)(in[2]))) : in[2]);
}

_AFXINL void AfxTruncateV4d(afxReal v[4], afxReal const in[4])
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

_AFXINL void AfxGetSaturatedV2d(afxReal v[2], afxReal const in[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);

    AfxClampV2d(v, in, AFX_V4D_0000, AFX_V4D_1111);
}

_AFXINL void AfxGetSaturatedV3d(afxReal v[3], afxReal const in[3])
{
    // Should be compatible with XMVECTOR XMVectorSaturate(FXMVECTOR V)

    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);

    AfxClampV3d(v, in, AFX_V4D_0000, AFX_V4D_1111);
}

_AFXINL void AfxGetSaturatedV4d(afxReal v[4], afxReal const in[4])
{
    // Should be compatible with XMVECTOR XMVectorSaturate(FXMVECTOR V)

    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);

    AfxClampV4d(v, in, AFX_V4D_0000, AFX_V4D_1111);
}

// Mini

_AFXINL void AfxMiniV2d(afxReal v[2], afxReal const a[2], afxReal const b[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);
    v[0] = a[0] < b[0] ? a[0] : b[0];
    v[1] = a[1] < b[1] ? a[1] : b[1];
}

_AFXINL void AfxMiniV3d(afxReal v[3], afxReal const a[3], afxReal const b[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);
    v[0] = a[0] < b[0] ? a[0] : b[0];
    v[1] = a[1] < b[1] ? a[1] : b[1];
    v[2] = a[2] < b[2] ? a[2] : b[2];
}

_AFXINL void AfxMiniV4d(afxReal v[4], afxReal const a[4], afxReal const b[4])
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

_AFXINL void AfxMaxiV2d(afxReal v[2], afxReal const a[2], afxReal const b[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);
    v[0] = a[0] > b[0] ? a[0] : b[0];
    v[1] = a[1] > b[1] ? a[1] : b[1];
}

_AFXINL void AfxMaxiV3d(afxReal v[3], afxReal const a[3], afxReal const b[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);
    v[0] = a[0] > b[0] ? a[0] : b[0];
    v[1] = a[1] > b[1] ? a[1] : b[1];
    v[2] = a[2] > b[2] ? a[2] : b[2];
}

_AFXINL void AfxMaxiV4d(afxReal v[4], afxReal const a[4], afxReal const b[4])
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

_AFXINL void AfxAbsV2d(afxReal v[2], afxReal const in[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxAbs(in[0]);
    v[1] = AfxAbs(in[1]);
}

_AFXINL void AfxAbsV3d(afxReal v[3], afxReal const in[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxAbs(in[0]);
    v[1] = AfxAbs(in[1]);
    v[2] = AfxAbs(in[2]);
}

_AFXINL void AfxAbsV4d(afxReal v[4], afxReal const in[4])
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

_AFXINL void AfxCeilV2d(afxReal v[2], afxReal const in[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxCeil(in[0]);
    v[1] = AfxCeil(in[1]);
}

_AFXINL void AfxCeilV3d(afxReal v[3], afxReal const in[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxCeil(in[0]);
    v[1] = AfxCeil(in[1]);
    v[2] = AfxCeil(in[2]);
}

_AFXINL void AfxCeilV4d(afxReal v[4], afxReal const in[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxCeil(in[0]);
    v[1] = AfxCeil(in[1]);
    v[2] = AfxCeil(in[2]);
    v[3] = AfxCeil(in[3]);
}

// Floor

_AFXINL void AfxFloorV2d(afxReal v[2], afxReal const in[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxFloor(in[0]);
    v[1] = AfxFloor(in[1]);
}

_AFXINL void AfxFloorV3d(afxReal v[3], afxReal const in[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxFloor(in[0]);
    v[1] = AfxFloor(in[1]);
    v[2] = AfxFloor(in[2]);
}

_AFXINL void AfxFloorV4d(afxReal v[4], afxReal const in[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxFloor(in[0]);
    v[1] = AfxFloor(in[1]);
    v[2] = AfxFloor(in[2]);
    v[3] = AfxFloor(in[3]);
}

// Recip

_AFXINL void AfxRecipV2d(afxReal v[2], afxReal const in[2])
{
    // Should be compatible with XMVECTOR XMVectorReciprocal(FXMVECTOR V)

    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    AfxDivV2d(v, AFX_V2D_11, in);
}

_AFXINL void AfxRecipV3d(afxReal v[3], afxReal const in[3])
{
    // Should be compatible with XMVECTOR XMVectorReciprocal(FXMVECTOR V)

    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    AfxDivV3d(v, AFX_V3D_111, in);
}

_AFXINL void AfxRecipV4d(afxReal v[4], afxReal const in[4])
{
    // Should be compatible with XMVECTOR XMVectorReciprocal(FXMVECTOR V)

    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    AfxDivV4d(v, AFX_V4D_1111, in);
}

// Normalize

_AFXINL afxReal AfxGetNormalizedV2d(afxReal v[2], afxReal const in[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    afxReal len = AfxMagV2d(in);

    if (len > 0)
        len = 1.f / len;

    AfxScaleV2d(v, in, len);
    return len;
}

_AFXINL afxReal AfxGetNormalizedV3d(afxReal v[3], afxReal const in[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    afxReal len = AfxMagV3d(in);

    if (len > 0)
        len = 1.f / len;

    AfxScaleV3d(v, in, len);
    return len;
}

_AFXINL afxReal AfxGetNormalizedV4d(afxReal v[4], afxReal const in[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    afxReal len = AfxMagV4d(in);

    if (len > 0)
        len = 1.f / len;

    AfxScaleV4d(v, in, len);
    return len;
}

// NormalizeV*d

_AFXINL afxReal AfxGetNormalizedV3dFromV4d(afxReal v[3], afxReal const in[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    afxReal len = AfxMagV4d(in);

    if (len > 0)
        len = 1.f / len;

    AfxScaleV3d(v, in, len);
    return len;
}

_AFXINL afxReal AfxGetNormalizedV4dFromV3d(afxReal v[4], afxReal const in[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    afxReal len = AfxMagV3d(in);

    if (len > 0)
        len = 1.f / len;

    AfxScaleV4d(v, in, len);
    return len;
}

// NormalizeEst

_AFXINL afxReal AfxEstimateNormalizedV2d(afxReal v[2], afxReal const in[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    afxReal len = AfxMagV2dRecip(in);
    AfxScaleV2d(v, in, len);
    return len;
}

_AFXINL afxReal AfxEstimateNormalizedV3d(afxReal v[3], afxReal const in[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    afxReal len = AfxMagV3dRecip(in);
    AfxScaleV3d(v, in, len);
    return len;
}

_AFXINL afxReal AfxEstimateNormalizedV4d(afxReal v[4], afxReal const in[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    afxReal len = AfxMagV4dRecip(in);
    AfxScaleV4d(v, in, len);
    return len;
}

////////////////////////////////////////////////////////////////////////////////
// Arithmetica                                                                //
////////////////////////////////////////////////////////////////////////////////

// Add

_AFXINL void AfxAddV2d(afxReal v[2], afxReal const a[2], afxReal const b[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);
    v[0] = a[0] + b[0];
    v[1] = a[1] + b[1];
}

_AFXINL void AfxAddV3d(afxReal v[3], afxReal const a[3], afxReal const b[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);
    v[0] = a[0] + b[0];
    v[1] = a[1] + b[1];
    v[2] = a[2] + b[2];
}

_AFXINL void AfxAddV4d(afxReal v[4], afxReal const a[4], afxReal const b[4])
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

// Sub

_AFXINL void AfxSubV2d(afxReal v[2], afxReal const a[2], afxReal const b[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);
    v[0] = a[0] - b[0];
    v[1] = a[1] - b[1];
}

_AFXINL void AfxSubV3d(afxReal v[3], afxReal const a[3], afxReal const b[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);
    v[0] = a[0] - b[0];
    v[1] = a[1] - b[1];
    v[2] = a[2] - b[2];
}

_AFXINL void AfxSubV4d(afxReal v[4], afxReal const a[4], afxReal const b[4])
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

_AFXINL void AfxDivV2d(afxReal v[2], afxReal const a[2], afxReal const b[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);
    v[0] = a[0] / b[0];
    v[1] = a[1] / b[1];
}

_AFXINL void AfxDivV3d(afxReal v[3], afxReal const a[3], afxReal const b[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);
    v[0] = a[0] / b[0];
    v[1] = a[1] / b[1];
    v[2] = a[2] / b[2];
}

_AFXINL void AfxDivV4d(afxReal v[4], afxReal const a[4], afxReal const b[4])
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

_AFXINL void AfxModV2d(afxReal v[2], afxReal const a[2], afxReal const b[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);

    AfxDivV2d(v, a, b);
    AfxTruncateV2d(v, v);
    AfxNemusV2d(v, b, v, a);
}

_AFXINL void AfxModV3d(afxReal v[3], afxReal const a[3], afxReal const b[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);

    AfxDivV3d(v, a, b);
    AfxTruncateV3d(v, v);
    AfxNemusV3d(v, b, v, a);
}

_AFXINL void AfxModV4d(afxReal v[4], afxReal const a[4], afxReal const b[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);

    AfxDivV4d(v, a, b);
    AfxTruncateV4d(v, v);
    AfxNemusV4d(v, b, v, a);
}

// Scale

_AFXINL void AfxScaleV2d(afxReal v[2], afxReal const a[2], afxReal lambda)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    v[0] = a[0] * lambda;
    v[1] = a[1] * lambda;
}

_AFXINL void AfxScaleV3d(afxReal v[3], afxReal const a[3], afxReal lambda)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    v[0] = a[0] * lambda;
    v[1] = a[1] * lambda;
    v[2] = a[2] * lambda;
}

_AFXINL void AfxScaleV4d(afxReal v[4], afxReal const a[4], afxReal lambda)
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

_AFXINL void AfxHalfV2d(afxReal v[2], afxReal const in[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = in[0] * 0.5f;
    v[1] = in[1] * 0.5f;
}

_AFXINL void AfxHalfV3d(afxReal v[3], afxReal const in[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = in[0] * 0.5f;
    v[1] = in[1] * 0.5f;
    v[2] = in[2] * 0.5f;
}

_AFXINL void AfxHalfV4d(afxReal v[4], afxReal const in[4])
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

_AFXINL void AfxMultiplyV2d(afxReal v[2], afxReal const a[2], afxReal const b[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);
    v[0] = a[0] * b[0];
    v[1] = a[1] * b[1];
}

_AFXINL void AfxMultiplyV3d(afxReal v[3], afxReal const a[3], afxReal const b[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);
    v[0] = a[0] * b[0];
    v[1] = a[1] * b[1];
    v[2] = a[2] * b[2];
}

_AFXINL void AfxMultiplyV4d(afxReal v[4], afxReal const a[4], afxReal const b[4])
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

_AFXINL void AfxMadV2d(afxReal v[2], afxReal const in[2], afxReal const mul[2], afxReal const plus[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    AfxAssert(mul);
    AfxAssert(plus);
    v[0] = in[0] * mul[0] + plus[0];
    v[1] = in[1] * mul[1] + plus[1];
}

_AFXINL void AfxMadV3d(afxReal v[3], afxReal const in[3], afxReal const mul[3], afxReal const plus[3])
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

_AFXINL void AfxMadV4d(afxReal v[4], afxReal const in[4], afxReal const mul[4], afxReal const plus[4])
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

// Nemus

_AFXINL void AfxNemusV2d(afxReal v[2], afxReal const a[2], afxReal const b[2], afxReal const c[2])
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

_AFXINL void AfxNemusV3d(afxReal v[3], afxReal const a[3], afxReal const b[3], afxReal const c[3])
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

_AFXINL void AfxNemusV4d(afxReal v[4], afxReal const a[4], afxReal const b[4], afxReal const c[4])
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

_AFXINL void AfxLerpV2d(afxReal v[2], afxReal const a[2], afxReal const b[2], afxReal t)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);

    // a + t * (b - a)
#if 0
    v[0] = a[0] + t * (b[0] - a[0]);
    v[1] = a[1] + t * (b[1] - a[1]);
#else
    v[0] = a[0] * (1.f - t) + t * b[0];
    v[1] = a[1] * (1.f - t) + t * b[1];
#endif
}

_AFXINL void AfxLerpV3d(afxReal v[3], afxReal const a[3], afxReal const b[3], afxReal t)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);

    // a + t * (b - a)

    // a + t * (b - a)
#if 0
    v[0] = a[0] + t * (b[0] - a[0]);
    v[1] = a[1] + t * (b[1] - a[1]);
    v[2] = a[2] + t * (b[2] - a[2]);
#else
    v[0] = a[0] * (1.f - t) + t * b[0];
    v[1] = a[1] * (1.f - t) + t * b[1];
    v[2] = a[2] * (1.f - t) + t * b[2];
#endif
}

_AFXINL void AfxLerpV4d(afxReal v[4], afxReal const a[4], afxReal const b[4], afxReal t)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);
    
    // a + t * (b - a)
#if 0
    v[0] = a[0] + t * (b[0] - a[0]);
    v[1] = a[1] + t * (b[1] - a[1]);
    v[2] = a[2] + t * (b[2] - a[2]);
    v[3] = a[3] + t * (b[3] - a[3]);
#else
    v[0] = a[0] * (1.f - t) + t * b[0];
    v[1] = a[1] * (1.f - t) + t * b[1];
    v[2] = a[2] * (1.f - t) + t * b[2];
    v[3] = a[3] * (1.f - t) + t * b[3];
#endif
}

////////////////////////////////////////////////////////////////////////////////
// Algebra                                                                    //
////////////////////////////////////////////////////////////////////////////////

// Square root

_AFXINL void AfxSqrtV2d(afxReal v[2], afxReal const in[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxSqrt(in[0]);
    v[1] = AfxSqrt(in[1]);
}

_AFXINL void AfxSqrtV3d(afxReal v[3], afxReal const in[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxSqrt(in[0]);
    v[1] = AfxSqrt(in[1]);
    v[2] = AfxSqrt(in[2]);
}

_AFXINL void AfxSqrtV4d(afxReal v[4], afxReal const in[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxSqrt(in[0]);
    v[1] = AfxSqrt(in[1]);
    v[2] = AfxSqrt(in[2]);
    v[3] = AfxSqrt(in[3]);
}

// Reciprocal square root

_AFXINL void AfxRsqrtV2d(afxReal v[2], afxReal const in[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxRsqrt(in[0]);
    v[1] = AfxRsqrt(in[1]);
}

_AFXINL void AfxRsqrtV3d(afxReal v[3], afxReal const in[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxRsqrt(in[0]);
    v[1] = AfxRsqrt(in[1]);
    v[2] = AfxRsqrt(in[2]);
}

_AFXINL void AfxRsqrtV4d(afxReal v[4], afxReal const in[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxRsqrt(in[0]);
    v[1] = AfxRsqrt(in[1]);
    v[2] = AfxRsqrt(in[2]);
    v[3] = AfxRsqrt(in[3]);
}

// Sine

_AFXINL void AfxSinV2d(afxReal v[2], afxReal const in[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxSin(in[0]);
    v[1] = AfxSin(in[1]);
}

_AFXINL void AfxSinV3d(afxReal v[3], afxReal const in[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxSin(in[0]);
    v[1] = AfxSin(in[1]);
    v[2] = AfxSin(in[2]);
}

_AFXINL void AfxSinV4d(afxReal v[4], afxReal const in[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxSin(in[0]);
    v[1] = AfxSin(in[1]);
    v[2] = AfxSin(in[2]);
    v[3] = AfxSin(in[3]);
}

// Cosine

_AFXINL void AfxCosV2d(afxReal v[2], afxReal const in[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxCos(in[0]);
    v[1] = AfxCos(in[1]);
}

_AFXINL void AfxCosV3d(afxReal v[3], afxReal const in[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxCos(in[0]);
    v[1] = AfxCos(in[1]);
    v[2] = AfxCos(in[2]);
}

_AFXINL void AfxCosV4d(afxReal v[4], afxReal const in[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxCos(in[0]);
    v[1] = AfxCos(in[1]);
    v[2] = AfxCos(in[2]);
    v[3] = AfxCos(in[3]);
}

// Arc cosine

_AFXINL void AfxAcosV2d(afxReal v[2], afxReal const in[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxAcos(in[0]);
    v[1] = AfxAcos(in[1]);
}

_AFXINL void AfxAcosV3d(afxReal v[3], afxReal const in[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxAcos(in[0]);
    v[1] = AfxAcos(in[1]);
    v[2] = AfxAcos(in[2]);
}

_AFXINL void AfxAcosV4d(afxReal v[4], afxReal const in[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxAcos(in[0]);
    v[1] = AfxAcos(in[1]);
    v[2] = AfxAcos(in[2]);
    v[3] = AfxAcos(in[3]);
}

// Tangent

_AFXINL void AfxTanV2d(afxReal v[2], afxReal const in[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxTan(in[0]);
    v[1] = AfxTan(in[1]);
}

_AFXINL void AfxTanV3d(afxReal v[3], afxReal const in[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxTan(in[0]);
    v[1] = AfxTan(in[1]);
    v[2] = AfxTan(in[2]);
}

_AFXINL void AfxTanV4d(afxReal v[4], afxReal const in[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxTan(in[0]);
    v[1] = AfxTan(in[1]);
    v[2] = AfxTan(in[2]);
    v[3] = AfxTan(in[3]);
}

// Arc tangent

_AFXINL void AfxAtanV2d(afxReal v[2], afxReal const in[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxAtan(in[0]);
    v[1] = AfxAtan(in[1]);
}

_AFXINL void AfxAtanV3d(afxReal v[3], afxReal const in[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxAtan(in[0]);
    v[1] = AfxAtan(in[1]);
    v[2] = AfxAtan(in[2]);
}

_AFXINL void AfxAtanV4d(afxReal v[4], afxReal const in[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxAtan(in[0]);
    v[1] = AfxAtan(in[1]);
    v[2] = AfxAtan(in[2]);
    v[3] = AfxAtan(in[3]);
}

// Log

_AFXINL void AfxLogV2d(afxReal v[2], afxReal const in[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxLog(in[0]);
    v[1] = AfxLog(in[1]);
}

_AFXINL void AfxLogV3d(afxReal v[3], afxReal const in[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxLog(in[0]);
    v[1] = AfxLog(in[1]);
    v[2] = AfxLog(in[2]);
}

_AFXINL void AfxLogV4d(afxReal v[4], afxReal const in[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxLog(in[0]);
    v[1] = AfxLog(in[1]);
    v[2] = AfxLog(in[2]);
    v[3] = AfxLog(in[3]);
}

// Log2

_AFXINL void AfxLog2V2d(afxReal v[2], afxReal const in[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxLog2(in[0]);
    v[1] = AfxLog2(in[1]);
}

_AFXINL void AfxLog2V3d(afxReal v[3], afxReal const in[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxLog2(in[0]);
    v[1] = AfxLog2(in[1]);
    v[2] = AfxLog2(in[2]);
}

_AFXINL void AfxLog2V4d(afxReal v[4], afxReal const in[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxLog2(in[0]);
    v[1] = AfxLog2(in[1]);
    v[2] = AfxLog2(in[2]);
    v[3] = AfxLog2(in[3]);
}

// Log10

_AFXINL void AfxLog10V2d(afxReal v[2], afxReal const in[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxLog10(in[0]);
    v[1] = AfxLog10(in[1]);
}

_AFXINL void AfxLog10V3d(afxReal v[3], afxReal const in[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxLog10(in[0]);
    v[1] = AfxLog10(in[1]);
    v[2] = AfxLog10(in[2]);
}

_AFXINL void AfxLog10V4d(afxReal v[4], afxReal const in[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxLog10(in[0]);
    v[1] = AfxLog10(in[1]);
    v[2] = AfxLog10(in[2]);
    v[3] = AfxLog10(in[3]);
}

// Pow

_AFXINL void AfxPowV2d(afxReal v[2], afxReal const in[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxPow(in[0], in[0]);
    v[1] = AfxPow(in[1], in[1]);
}

_AFXINL void AfxPowV3d(afxReal v[3], afxReal const in[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxPow(in[0], in[0]);
    v[1] = AfxPow(in[1], in[1]);
    v[2] = AfxPow(in[2], in[2]);
}

_AFXINL void AfxPowV4d(afxReal v[4], afxReal const in[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxPow(in[0], in[0]);
    v[1] = AfxPow(in[1], in[1]);
    v[2] = AfxPow(in[2], in[2]);
    v[3] = AfxPow(in[3], in[3]);
}

// Exp

_AFXINL void AfxExpV2d(afxReal v[2], afxReal const in[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxExp(in[0]);
    v[1] = AfxExp(in[1]);
}

_AFXINL void AfxExpV3d(afxReal v[3], afxReal const in[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxExp(in[0]);
    v[1] = AfxExp(in[1]);
    v[2] = AfxExp(in[2]);
}

_AFXINL void AfxExpV4d(afxReal v[4], afxReal const in[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxExp(in[0]);
    v[1] = AfxExp(in[1]);
    v[2] = AfxExp(in[2]);
    v[3] = AfxExp(in[3]);
}

// Exp2

_AFXINL void AfxExp2V2d(afxReal v[2], afxReal const in[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxExp2(in[0]);
    v[1] = AfxExp2(in[1]);
}

_AFXINL void AfxExp2V3d(afxReal v[3], afxReal const in[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxExp2(in[0]);
    v[1] = AfxExp2(in[1]);
    v[2] = AfxExp2(in[2]);
}

_AFXINL void AfxExp2V4d(afxReal v[4], afxReal const in[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxExp2(in[0]);
    v[1] = AfxExp2(in[1]);
    v[2] = AfxExp2(in[2]);
    v[3] = AfxExp2(in[3]);
}

// Exp10

_AFXINL void AfxExp10V2d(afxReal v[2], afxReal const in[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxExp10(in[0]);
    v[1] = AfxExp10(in[1]);
}

_AFXINL void AfxExp10V3d(afxReal v[3], afxReal const in[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxExp10(in[0]);
    v[1] = AfxExp10(in[1]);
    v[2] = AfxExp10(in[2]);
}

_AFXINL void AfxExp10V4d(afxReal v[4], afxReal const in[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = AfxExp10(in[0]);
    v[1] = AfxExp10(in[1]);
    v[2] = AfxExp10( in[2]);
    v[3] = AfxExp10(in[3]);
}

// Negate

_AFXINL void AfxNegateV2d(afxReal v[2], afxReal const in[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = -(in[0]);
    v[1] = -(in[1]);
}

_AFXINL void AfxNegateV3d(afxReal v[3], afxReal const in[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = -(in[0]);
    v[1] = -(in[1]);
    v[2] = -(in[2]);
}

_AFXINL void AfxNegateV4d(afxReal v[4], afxReal const in[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    v[0] = -(in[0]);
    v[1] = -(in[1]);
    v[2] = -(in[2]);
    v[3] = -(in[3]);
}

_AFXINL void AfxNegateAffineV4d(afxReal v[4], afxReal const in[4])
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

_AFXINL afxReal AfxSumV2d(afxReal const v[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return v[0] + v[1];
}

_AFXINL afxReal AfxSumV3d(afxReal const v[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return v[0] + v[1] + v[2];
}

_AFXINL afxReal AfxSumV4d(afxReal const v[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return v[0] + v[1] + v[2] + v[3];
}

// Dot

_AFXINL afxReal AfxDotV2d(afxReal const v[2], afxReal const other[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(other);
    return (v[0] * other[0]) + (v[1] * other[1]);
}

_AFXINL afxReal AfxDotV3d(afxReal const v[3], afxReal const other[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(other);
    return (v[0] * other[0]) + (v[1] * other[1]) + (v[2] * other[2]);
}

_AFXINL afxReal AfxDotV4d(afxReal const v[4], afxReal const other[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(other);
    return (v[0] * other[0]) + (v[1] * other[1]) + (v[2] * other[2]) + (v[3] * other[3]);
}

// Cross

_AFXINL void AfxCrossV2d(afxReal v[2], afxReal const a[2], afxReal const b[2])
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

_AFXINL void AfxCrossV3d(afxReal v[3], afxReal const a[3], afxReal const b[3])
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

_AFXINL void AfxCrossV4d(afxReal v[4], afxReal const a[4], afxReal const b[4], afxReal const c[4])
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

_AFXINL afxReal AfxMagV2dSq(afxReal const v[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return AfxDotV2d(v, v);
}

_AFXINL afxReal AfxMagV3dSq(afxReal const v[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return AfxDotV3d(v, v);
}

_AFXINL afxReal AfxMagV4dSq(afxReal const v[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return AfxDotV4d(v, v);
}

// Magnitude

_AFXINL afxReal AfxMagV2d(afxReal const v[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return AfxSqrt(AfxMagV2dSq(v));
}

_AFXINL afxReal AfxMagV3d(afxReal const v[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return AfxSqrt(AfxMagV3dSq(v));
}

_AFXINL afxReal AfxMagV4d(afxReal const v[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return AfxSqrt(AfxMagV4dSq(v));
}

// MagnitudeRecip

_AFXINL afxReal AfxMagV2dRecip(afxReal const v[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return AfxRsqrt(AfxMagV2dSq(v));
}

_AFXINL afxReal AfxMagV3dRecip(afxReal const v[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return AfxRsqrt(AfxMagV3dSq(v));
}

_AFXINL afxReal AfxMagV4dRecip(afxReal const v[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return AfxRsqrt(AfxMagV4dSq(v));
}

////////////////////////////////////////////////////////////////////////////////
// Affine transformation                                                      //
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Trigonometry                                                               //
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// VECTOR TRANSFORMATION METHODS                                              //
////////////////////////////////////////////////////////////////////////////////

// Transform

_AFXINL void AfxTransformV2d(afxReal v[2], afxReal const m[2][2], afxReal const in[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(in);
    AfxAssert(v);
    AfxAssert(v != in);

    v[0] = (m[0][0] * in[0]) + (m[0][1] * in[1]);
    v[1] = (m[1][0] * in[0]) + (m[1][1] * in[1]);
}

_AFXINL void AfxTransformV3d(afxReal v[3], afxReal const m[3][3], afxReal const in[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(m);
    AfxAssert(in);
    AfxAssert(v != in);

    v[0] = (m[0][0] * in[0]) + (m[0][1] * in[1]) + (m[0][2] * in[2]);
    v[1] = (m[1][0] * in[0]) + (m[1][1] * in[1]) + (m[1][2] * in[2]);
    v[2] = (m[2][0] * in[0]) + (m[2][1] * in[1]) + (m[2][2] * in[2]);
}

_AFXINL void AfxTransformV4d(afxReal v[4], afxReal const m[4][4], afxReal const in[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(m);
    AfxAssert(in);
    AfxAssert(v != in);

    v[0] = (m[0][0] * in[0]) + (m[0][1] * in[1]) + (m[0][2] * in[2]) + (m[0][3] * in[3]);
    v[1] = (m[1][0] * in[0]) + (m[1][1] * in[1]) + (m[1][2] * in[2]) + (m[1][3] * in[3]);
    v[2] = (m[2][0] * in[0]) + (m[2][1] * in[1]) + (m[2][2] * in[2]) + (m[2][3] * in[3]);
    v[3] = (m[3][0] * in[0]) + (m[3][1] * in[1]) + (m[3][2] * in[2]) + (m[3][3] * in[3]);
}

// TransformPoint/Normal/Affine

_AFXINL void AfxTransformPointV2d(afxReal v[2], afxReal const m[4][4], afxReal const in[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(in);
    AfxAssert(v);
    AfxAssert(v != in);

    v[0] = (m[0][0] * in[0]) + (m[0][1] * in[1]) + m[0][2] + m[0][3];
    v[1] = (m[1][0] * in[0]) + (m[1][1] * in[1]) + m[1][2] + m[1][3];
}

_AFXINL void AfxTransformPointV3d(afxReal v[3], afxReal const m[4][4], afxReal const in[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(m);
    AfxAssert(in);
    AfxAssert(v != in);

    v[0] = (m[0][0] * in[0]) + (m[0][1] * in[1]) + (m[0][2] * in[2]) + m[2][3];
    v[1] = (m[1][0] * in[0]) + (m[1][1] * in[1]) + (m[1][2] * in[2]) + m[2][3];
    v[2] = (m[2][0] * in[0]) + (m[2][1] * in[1]) + (m[2][2] * in[2]) + m[2][3];
}

_AFXINL void AfxTransformNormalV3d(afxReal v[3], afxReal const m[4][4], afxReal const in[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(m);
    AfxAssert(in);
    AfxAssert(v != in);

    v[0] = (m[0][0] * in[0]) + (m[0][1] * in[1]) + (m[0][2] * in[2]);
    v[1] = (m[1][0] * in[0]) + (m[1][1] * in[1]) + (m[1][2] * in[2]);
    v[2] = (m[2][0] * in[0]) + (m[2][1] * in[1]) + (m[2][2] * in[2]);
}

_AFXINL void AfxTransformNormalV4d(afxReal v[4], afxReal const m[3][3], afxReal const in[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(m);
    AfxAssert(in);
    AfxAssert(v != in);

    v[0] = (m[0][0] * in[0]) + (m[0][1] * in[1]) + (m[0][2] * in[2]);
    v[1] = (m[1][0] * in[0]) + (m[1][1] * in[1]) + (m[1][2] * in[2]);
    v[2] = (m[2][0] * in[0]) + (m[2][1] * in[1]) + (m[2][2] * in[2]);
    v[3] = 0.f;
}

_AFXINL void AfxTransformAffineV4d(afxReal v[4], afxReal const m[4][4], afxReal const in[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(m);
    AfxAssert(in);
    AfxAssert(v != in);

    v[0] = (m[0][0] * in[0]) + (m[0][1] * in[1]) + (m[0][2] * in[2]) + m[0][3];
    v[1] = (m[1][0] * in[0]) + (m[1][1] * in[1]) + (m[1][2] * in[2]) + m[1][3];
    v[2] = (m[2][0] * in[0]) + (m[2][1] * in[1]) + (m[2][2] * in[2]) + m[2][3];
    v[3] = 1.f;
}
// TransformTransposed

_AFXINL void AfxTransformTransposedV2d(afxReal v[2], afxReal const in[2], afxReal const m[2][2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(in);
    AfxAssert(m);
    AfxAssert(v);
    AfxAssert(v != in);

    // multiply column vector 2D by matrix 2x2
    v[0] = (in[0] * m[0][0]) + (in[1] * m[1][0]);
    v[1] = (in[0] * m[0][1]) + (in[1] * m[1][1]);
}

_AFXINL void AfxTransformTransposedV3d(afxReal v[3], afxReal const in[3], afxReal const m[3][3])
{
    // Computes the transpose of the product of two matrices.

    afxError err = AFX_ERR_NONE;
    AfxAssert(in);
    AfxAssert(m);
    AfxAssert(v);
    AfxAssert(v != in);

    // multiply column vector 3D by matrix 3x3
    v[0] = (in[0] * m[0][0]) + (in[1] * m[1][0]) + (in[2] * m[2][0]);
    v[1] = (in[0] * m[0][1]) + (in[1] * m[1][1]) + (in[2] * m[2][1]);
    v[2] = (in[0] * m[0][2]) + (in[1] * m[1][2]) + (in[2] * m[2][2]);
}

_AFXINL void AfxTransformTransposedV4d(afxReal v[4], afxReal const in[4], afxReal const m[4][4])
{
    // Computes the transpose of the product of two matrices.

    afxError err = AFX_ERR_NONE;
    AfxAssert(in);
    AfxAssert(m);
    AfxAssert(v);
    AfxAssert(v != in);

    // multiply column vector 4D by matrix 4x4
    v[0] = (in[0] * m[0][0]) + (in[1] * m[1][0]) + (in[2] * m[2][0]) + (in[3] * m[3][0]);
    v[1] = (in[0] * m[0][1]) + (in[1] * m[1][1]) + (in[2] * m[2][1]) + (in[3] * m[3][1]);
    v[2] = (in[0] * m[0][2]) + (in[1] * m[1][2]) + (in[2] * m[2][2]) + (in[3] * m[3][2]);
    v[3] = (in[0] * m[0][3]) + (in[1] * m[1][3]) + (in[2] * m[2][3]) + (in[3] * m[3][3]);
}

// TransformTransposedPoint/Normal/Affine

_AFXINL void AfxTransformTransposedPointV2d(afxReal v[2], afxReal const in[2], afxReal const m[4][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(in);
    AfxAssert(m);
    AfxAssert(v);
    AfxAssert(v != in);

    // multiply column vector 2D by matrix 4x4
    v[0] = (in[0] * m[0][0]) + (in[1] * m[1][0]) + m[2][0] + m[3][0];
    v[1] = (in[0] * m[0][1]) + (in[1] * m[1][1]) + m[2][1] + m[3][1];
}

_AFXINL void AfxTransformTransposedPointV3d(afxReal v[3], afxReal const in[3], afxReal const m[4][4])
{
    // Computes the transpose of the product of two matrices.

    afxError err = AFX_ERR_NONE;
    AfxAssert(in);
    AfxAssert(m);
    AfxAssert(v);
    AfxAssert(v != in);

    // multiply column vector 3D by matrix 4x4
    v[0] = (in[0] * m[0][0]) + (in[1] * m[1][0]) + (in[2] * m[2][0]) + m[3][0];
    v[1] = (in[0] * m[0][1]) + (in[1] * m[1][1]) + (in[2] * m[2][1]) + m[3][1];
    v[2] = (in[0] * m[0][2]) + (in[1] * m[1][2]) + (in[2] * m[2][2]) + m[3][2];
}

_AFXINL void AfxTransformTransposedNormalV3d(afxReal v[3], afxReal const in[3], afxReal const m[4][4])
{
    // Computes the transpose of the product of two matrices.

    afxError err = AFX_ERR_NONE;
    AfxAssert(in);
    AfxAssert(m);
    AfxAssert(v);
    AfxAssert(v != in);

    // multiply column vector 3D by matrix 4x4
    v[0] = (in[0] * m[0][0]) + (in[1] * m[1][0]) + (in[2] * m[2][0]);
    v[1] = (in[0] * m[0][1]) + (in[1] * m[1][1]) + (in[2] * m[2][1]);
    v[2] = (in[0] * m[0][2]) + (in[1] * m[1][2]) + (in[2] * m[2][2]);
}

_AFXINL void AfxTransformTransposedNormalV4d(afxReal v[4], afxReal const in[4], afxReal const m[3][3])
{
    // Computes the transpose of the product of two matrices.

    afxError err = AFX_ERR_NONE;
    AfxAssert(in);
    AfxAssert(m);
    AfxAssert(v);
    AfxAssert(v != in);

    // multiply column vector 4D by matrix 3x3
    v[0] = (in[0] * m[0][0]) + (in[1] * m[1][0]) + (in[2] * m[2][0]);
    v[1] = (in[0] * m[0][1]) + (in[1] * m[1][1]) + (in[2] * m[2][1]);
    v[2] = (in[0] * m[0][2]) + (in[1] * m[1][2]) + (in[2] * m[2][2]);
    v[3] = 0.f;
}

_AFXINL void AfxTransformTransposedAffineV4d(afxReal v[4], afxReal const in[4], afxReal const m[4][4])
{
    // Computes the transpose of the product of two matrices.

    afxError err = AFX_ERR_NONE;
    AfxAssert(in);
    AfxAssert(m);
    AfxAssert(v);
    AfxAssert(v != in);

    // multiply column vector 4D by matrix 4x3
    v[0] = (in[0] * m[0][0]) + (in[1] * m[1][0]) + (in[2] * m[2][0]) + (/*in[3] * */m[3][0]);
    v[1] = (in[0] * m[0][1]) + (in[1] * m[1][1]) + (in[2] * m[2][1]) + (/*in[3] * */m[3][1]);
    v[2] = (in[0] * m[0][2]) + (in[1] * m[1][2]) + (in[2] * m[2][2]) + (/*in[3] * */m[3][2]);
    v[3] = 1.f;
}

// Assimilate

_AFXINL void AfxAssimilatePointV3d(afxReal const lt[3][3], afxReal const at[3], afxNat cnt, afxReal const in[][3], afxReal out[][3])
{
    // Should be compatible with void InPlaceSimilarityTransformPosition(const float *Affine3, const float *Linear3x3, float *Position3)
    afxError err = NIL;
    AfxAssert(at);
    AfxAssert(lt);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    AfxTransformArrayedV3d(lt, cnt, in, out);

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxAddV3d(out[i], at, out[i]);
    }
}

_AFXINL void AfxAssimilatePointV4d(afxReal const lt[3][3], afxReal const at[3], afxNat cnt, afxReal const in[][4], afxReal out[][4])
{
    // inspired on AfxAssimilatePointV3d
    afxError err = NIL;
    AfxAssert(at);
    AfxAssert(lt);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    AfxTransformArrayedNormalV4d(lt, cnt, in, out);

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxAddV3d(out[i], at, out[i]);
        out[i][3] = 1.f;
    }
}
