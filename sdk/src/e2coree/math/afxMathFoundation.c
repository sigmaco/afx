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
#include "afx/math/afxScalar.h"

_AFX afxV2d const AFX_V2D_X = { AfxScalar(1), AfxScalar(0) };
_AFX afxV3d const AFX_V3D_X = { AfxScalar(1), AfxScalar(0), AfxScalar(0) };
_AFX afxV4d const AFX_V4D_X = { AfxScalar(1), AfxScalar(0), AfxScalar(0), AfxScalar(0) };
_AFX afxV2d const AFX_V2D_XI = { AfxScalar(-1), AfxScalar(0) };
_AFX afxV3d const AFX_V3D_XI = { AfxScalar(-1), AfxScalar(0), AfxScalar(0) };
_AFX afxV4d const AFX_V4D_XI = { AfxScalar(-1), AfxScalar(0), AfxScalar(0), AfxScalar(0) };

_AFX afxV2d const AFX_V2D_Y = { AfxScalar(0), AfxScalar(1) };
_AFX afxV3d const AFX_V3D_Y = { AfxScalar(0), AfxScalar(1), AfxScalar(0) };
_AFX afxV4d const AFX_V4D_Y = { AfxScalar(0), AfxScalar(1), AfxScalar(0), AfxScalar(0) };
_AFX afxV2d const AFX_V2D_YI = { AfxScalar(0), AfxScalar(-1) };
_AFX afxV3d const AFX_V3D_YI = { AfxScalar(0), AfxScalar(-1), AfxScalar(0) };
_AFX afxV4d const AFX_V4D_YI = { AfxScalar(0), AfxScalar(-1), AfxScalar(0), AfxScalar(0) };

_AFX afxV3d const AFX_V3D_Z = { AfxScalar(0), AfxScalar(0), AfxScalar(1) };
_AFX afxV4d const AFX_V4D_Z = { AfxScalar(0), AfxScalar(0), AfxScalar(1), AfxScalar(0) };
_AFX afxV3d const AFX_V3D_ZI = { AfxScalar(0), AfxScalar(0), AfxScalar(-1) };
_AFX afxV4d const AFX_V4D_ZI = { AfxScalar(0), AfxScalar(0), AfxScalar(-1), AfxScalar(0) };

_AFX afxV4d const AFX_V4D_W = { AfxScalar(0), AfxScalar(0), AfxScalar(0), AfxScalar(1) };
_AFX afxV4d const AFX_V4D_WI = { AfxScalar(0), AfxScalar(0), AfxScalar(0), AfxScalar(-1) };

_AFX afxV2d const AFX_V2D_ONE = { AfxScalar(1), AfxScalar(1) };
_AFX afxV3d const AFX_V3D_ONE = { AfxScalar(1), AfxScalar(1), AfxScalar(1) };
_AFX afxV4d const AFX_V4D_ONE = { AfxScalar(1), AfxScalar(1), AfxScalar(1), AfxScalar(1) };

_AFX afxV2d const AFX_V2D_ZERO = { AfxScalar(0), AfxScalar(0) };
_AFX afxV3d const AFX_V3D_ZERO = { AfxScalar(0), AfxScalar(0), AfxScalar(0) };
_AFX afxV4d const AFX_V4D_ZERO = { AfxScalar(0), AfxScalar(0), AfxScalar(0), AfxScalar(0) };

_AFX afxQuat const AFX_QUAT_ZERO = { AfxScalar(0), AfxScalar(0), AfxScalar(0), AfxScalar(0) };

_AFX afxM2d const AFX_M2D_ZERO =
{
    { AfxScalar(0), AfxScalar(0) },
    { AfxScalar(0), AfxScalar(0) }
};

_AFX afxM3d const AFX_M3D_ZERO =
{
    { AfxScalar(0), AfxScalar(0), AfxScalar(0) },
    { AfxScalar(0), AfxScalar(0), AfxScalar(0) },
    { AfxScalar(0), AfxScalar(0), AfxScalar(0) }
};

_AFX afxM4d const AFX_M4D_ZERO =
{
    { AfxScalar(0), AfxScalar(0), AfxScalar(0), AfxScalar(0) },
    { AfxScalar(0), AfxScalar(0), AfxScalar(0), AfxScalar(0) },
    { AfxScalar(0), AfxScalar(0), AfxScalar(0), AfxScalar(0) },
    { AfxScalar(0), AfxScalar(0), AfxScalar(0), AfxScalar(0) }
};

_AFXINL afxBool AfxV2dIsEqual(afxV2d const v, afxV2d const other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(other);
    return  (AfxScalar(v[0] < other[0] + AFX_EPSILON && v[0] > other[0] - AFX_EPSILON)) + 
            (AfxScalar(v[1] < other[1] + AFX_EPSILON && v[1] > other[1] - AFX_EPSILON));
}

_AFXINL afxBool AfxV3dIsEqual(afxV3d const v, afxV3d const other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(other);
    return  (AfxScalar(v[0] < other[0] + AFX_EPSILON && v[0] > other[0] - AFX_EPSILON)) +
            (AfxScalar(v[1] < other[1] + AFX_EPSILON && v[1] > other[1] - AFX_EPSILON)) +
            (AfxScalar(v[2] < other[2] + AFX_EPSILON && v[2] > other[2] - AFX_EPSILON));
}

_AFXINL afxBool AfxV4dIsEqual(afxV4d const v, afxV4d const other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(other);
    return  (AfxScalar(v[0] < other[0] + AFX_EPSILON && v[0] > other[0] - AFX_EPSILON)) +
            (AfxScalar(v[1] < other[1] + AFX_EPSILON && v[1] > other[1] - AFX_EPSILON)) +
            (AfxScalar(v[2] < other[2] + AFX_EPSILON && v[2] > other[2] - AFX_EPSILON)) +
            (AfxScalar(v[3] < other[3] + AFX_EPSILON && v[3] > other[3] - AFX_EPSILON));
}

_AFXINL afxBool AfxQuatIsEqual(afxQuat const q, afxQuat const other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(q);
    AfxAssert(other);
    return AfxV4dIsEqual(q, other);
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

_AFXINL void AfxQuatSet(afxQuat q, afxReal x, afxReal y, afxReal z, afxReal w)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(q);
    q[0] = x;
    q[1] = y;
    q[2] = z;
    q[3] = w;
}

// SetScalar

_AFXINL void AfxV2dSetScalar(afxV2d v, afxReal value)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    v[0] = value;
    v[1] = value;
}

_AFXINL void AfxV3dSetScalar(afxV3d v, afxReal value)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    v[0] = value;
    v[1] = value;
    v[2] = value;
}

_AFXINL void AfxV4dSetScalar(afxV4d v, afxReal value)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    v[0] = value;
    v[1] = value;
    v[2] = value;
    v[3] = value;
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

_AFXINL void AfxQuatZero(afxQuat q)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(q);
    AfxQuatCopy(q, AFX_QUAT_ZERO);
}

_AFXINL void AfxM2dZero(afxM2d m)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxM2dCopy(m, AFX_M2D_ZERO);
}

_AFXINL void AfxM3dZero(afxM3d m)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxM3dCopy(m, AFX_M3D_ZERO);
}

_AFXINL void AfxM4dZero(afxM4d m)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxM4dCopy(m, AFX_M4D_ZERO);
}

// Min

_AFXINL void AfxV2dMin(afxV2d const v, afxV2d const other, afxV2d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxAssert(other);
    out[0] = v[0] < other[0] ? v[0] : other[0];
    out[1] = v[1] < other[1] ? v[1] : other[1];
}

_AFXINL void AfxV3dMin(afxV3d const v, afxV3d const other, afxV3d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxAssert(other);
    out[0] = v[0] < other[0] ? v[0] : other[0];
    out[1] = v[1] < other[1] ? v[1] : other[1];
    out[2] = v[2] < other[2] ? v[2] : other[2];
}

_AFXINL void AfxV4dMin(afxV4d const v, afxV4d const other, afxV4d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxAssert(other);
    out[0] = v[0] < other[0] ? v[0] : other[0];
    out[1] = v[1] < other[1] ? v[1] : other[1];
    out[2] = v[2] < other[2] ? v[2] : other[2];
    out[3] = v[3] < other[3] ? v[3] : other[3];
}

// Max

_AFXINL void AfxV2dMax(afxV2d const v, afxV2d const other, afxV2d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxAssert(other);
    out[0] = v[0] > other[0] ? v[0] : other[0];
    out[1] = v[1] > other[1] ? v[1] : other[1];
}

_AFXINL void AfxV3dMax(afxV3d const v, afxV3d const other, afxV3d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxAssert(other);
    out[0] = v[0] > other[0] ? v[0] : other[0];
    out[1] = v[1] > other[1] ? v[1] : other[1];
    out[2] = v[2] > other[2] ? v[2] : other[2];
}

_AFXINL void AfxV4dMax(afxV4d const v, afxV4d const other, afxV4d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxAssert(other);
    out[0] = v[0] > other[0] ? v[0] : other[0];
    out[1] = v[1] > other[1] ? v[1] : other[1];
    out[2] = v[2] > other[2] ? v[2] : other[2];
    out[3] = v[3] > other[3] ? v[3] : other[3];
}


// Ceil

_AFXINL void AfxV2dCeil(afxV2d const v, afxV2d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    out[0] = AfxCeil(v[0]);
    out[1] = AfxCeil(v[1]);
}

_AFXINL void AfxV3dCeil(afxV3d const v, afxV3d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    out[0] = AfxCeil(v[0]);
    out[1] = AfxCeil(v[1]);
    out[2] = AfxCeil(v[2]);
}

_AFXINL void AfxV4dCeil(afxV4d const v, afxV4d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    out[0] = AfxCeil(v[0]);
    out[1] = AfxCeil(v[1]);
    out[2] = AfxCeil(v[2]);
    out[3] = AfxCeil(v[3]);
}

// Floor

_AFXINL void AfxV2dFloor(afxV2d const v, afxV2d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    out[0] = AfxFloor(v[0]);
    out[1] = AfxFloor(v[1]);
}

_AFXINL void AfxV3dFloor(afxV3d const v, afxV3d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    out[0] = AfxFloor(v[0]);
    out[1] = AfxFloor(v[1]);
    out[2] = AfxFloor(v[2]);
}

_AFXINL void AfxV4dFloor(afxV4d const v, afxV4d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    out[0] = AfxFloor(v[0]);
    out[1] = AfxFloor(v[1]);
    out[2] = AfxFloor(v[2]);
    out[3] = AfxFloor(v[3]);
}

// Recip

_AFXINL void AfxV2dRecip(afxV2d const v, afxV2d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxV2dDiv(AFX_V2D_ONE, v, out);
}

_AFXINL void AfxV3dRecip(afxV3d const v, afxV3d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxV3dDiv(AFX_V3D_ONE, v, out);
}

_AFXINL void AfxV4dRecip(afxV4d const v, afxV4d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxV4dDiv(AFX_V4D_ONE, v, out);
}

// Add

_AFXINL void AfxV2dAdd(afxV2d const v, afxV2d const other, afxV2d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxAssert(other);
    out[0] = v[0] + other[0];
    out[1] = v[1] + other[1];
}

_AFXINL void AfxV3dAdd(afxV3d const v, afxV3d const other, afxV3d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxAssert(other);
    out[0] = v[0] + other[0];
    out[1] = v[1] + other[1];
    out[2] = v[2] + other[2];
}

_AFXINL void AfxV4dAdd(afxV4d const v, afxV4d const other, afxV4d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxAssert(other);
    out[0] = v[0] + other[0];
    out[1] = v[1] + other[1];
    out[2] = v[2] + other[2];
    out[3] = v[3] + other[3];
}

_AFXINL void AfxQuatAdd(afxQuat const q, afxQuat const other, afxQuat out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(q);
    AfxV4dAdd(q, other, out);
}

// Sub

_AFXINL void AfxV2dSub(afxV2d const v, afxV2d const other, afxV2d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxAssert(other);
    out[0] = v[0] - other[0];
    out[1] = v[1] - other[1];
}

_AFXINL void AfxV3dSub(afxV3d const v, afxV3d const other, afxV3d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxAssert(other);
    out[0] = v[0] - other[0];
    out[1] = v[1] - other[1];
    out[2] = v[2] - other[2];
}

_AFXINL void AfxV4dSub(afxV4d const v, afxV4d const other, afxV4d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxAssert(other);
    out[0] = v[0] - other[0];
    out[1] = v[1] - other[1];
    out[2] = v[2] - other[2];
    out[3] = v[3] - other[3];
}

_AFXINL void AfxQuatSub(afxQuat const q, afxQuat const other, afxQuat out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(other);
    AfxAssert(q);
    AfxV4dSub(q, other, out);
}

// Div

_AFXINL void AfxV2dDiv(afxV2d const v, afxV2d const other, afxV2d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxAssert(other);
    out[0] = v[0] / other[0];
    out[1] = v[1] / other[1];
}

_AFXINL void AfxV3dDiv(afxV3d const v, afxV3d const other, afxV3d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxAssert(other);
    out[0] = v[0] / other[0];
    out[1] = v[1] / other[1];
    out[2] = v[2] / other[2];
}

_AFXINL void AfxV4dDiv(afxV4d const v, afxV4d const other, afxV4d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxAssert(other);
    out[0] = v[0] / other[0];
    out[1] = v[1] / other[1];
    out[2] = v[2] / other[2];
    out[3] = v[3] / other[3];
}

// Scale

_AFXINL void AfxV2dScale(afxV2d const v, afxReal scale, afxV2d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    out[0] = v[0] * scale;
    out[1] = v[1] * scale;
}

_AFXINL void AfxV3dScale(afxV3d const v, afxReal scale, afxV3d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    out[0] = v[0] * scale;
    out[1] = v[1] * scale;
    out[2] = v[2] * scale;
}

_AFXINL void AfxV4dScale(afxV4d const v, afxReal scale, afxV4d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    out[0] = v[0] * scale;
    out[1] = v[1] * scale;
    out[2] = v[2] * scale;
    out[3] = v[3] * scale;
}

_AFXINL void AfxQuatScale(afxQuat const q, afxReal scale, afxQuat out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(q);
    AfxAssert(out);
    out[0] = q[0] * scale;
    out[1] = q[1] * scale;
    out[2] = q[2] * scale;
    out[3] = q[3] * scale;
}

// Madd

_AFXINL void AfxV2dMadd(afxV2d const v, afxV2d const other, afxV2d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxAssert(other);
    out[0] += v[0] * other[0];
    out[1] += v[1] * other[1];
}

_AFXINL void AfxV3dMadd(afxV3d const v, afxV3d const other, afxV3d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxAssert(other);
    out[0] += v[0] * other[0];
    out[1] += v[1] * other[1];
    out[2] += v[2] * other[2];
}

_AFXINL void AfxV4dMadd(afxV4d const v, afxV4d const other, afxV4d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxAssert(other);
    out[0] += v[0] * other[0];
    out[1] += v[1] * other[1];
    out[2] += v[2] * other[2];
    out[3] += v[3] * other[3];
}

// CombineMulAndAdd

_AFXINL void AfxV2dCombineMulAndAdd(afxV2d const v, afxV2d const mul, afxV2d const plus, afxV2d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxAssert(mul);
    AfxAssert(plus);
    out[0] = v[0] * mul[0] + plus[0];
    out[1] = v[1] * mul[1] + plus[1];
}

_AFXINL void AfxV3dCombineMulAndAdd(afxV3d const v, afxV3d const mul, afxV3d const plus, afxV3d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxAssert(mul);
    AfxAssert(plus);
    out[0] = v[0] * mul[0] + plus[0];
    out[1] = v[1] * mul[1] + plus[1];
    out[2] = v[2] * mul[2] + plus[2];
}

_AFXINL void AfxV4dCombineMulAndAdd(afxV4d const v, afxV4d const mul, afxV4d const plus, afxV4d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxAssert(mul);
    AfxAssert(plus);
    out[0] = v[0] * mul[0] + plus[0];
    out[1] = v[1] * mul[1] + plus[1];
    out[2] = v[2] * mul[2] + plus[2];
    out[3] = v[3] * mul[3] + plus[3];
}

// Sqrt

_AFXINL void AfxV2dSqrt(afxV2d const v, afxV2d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    out[0] = AfxSqrt(v[0]);
    out[1] = AfxSqrt(v[1]);
}

_AFXINL void AfxV3dSqrt(afxV3d const v, afxV3d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    out[0] = AfxSqrt(v[0]);
    out[1] = AfxSqrt(v[1]);
    out[2] = AfxSqrt(v[2]);
}

_AFXINL void AfxV4dSqrt(afxV4d const v, afxV4d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    out[0] = AfxSqrt(v[0]);
    out[1] = AfxSqrt(v[1]);
    out[2] = AfxSqrt(v[2]);
    out[3] = AfxSqrt(v[3]);
}

// SqrtRecip

_AFXINL void AfxV2dSqrtRecip(afxV2d const v, afxV2d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    out[0] = AfxRecipSqrt(v[0]);
    out[1] = AfxRecipSqrt(v[1]);
}

_AFXINL void AfxV3dSqrtRecip(afxV3d const v, afxV3d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    out[0] = AfxRecipSqrt(v[0]);
    out[1] = AfxRecipSqrt(v[1]);
    out[2] = AfxRecipSqrt(v[2]);
}

_AFXINL void AfxV4dSqrtRecip(afxV4d const v, afxV4d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    out[0] = AfxRecipSqrt(v[0]);
    out[1] = AfxRecipSqrt(v[1]);
    out[2] = AfxRecipSqrt(v[2]);
    out[3] = AfxRecipSqrt(v[3]);
}

// Sine

_AFXINL void AfxV2dSin(afxV2d const v, afxV2d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    out[0] = AfxSin(v[0]);
    out[1] = AfxSin(v[1]);
}

_AFXINL void AfxV3dSin(afxV3d const v, afxV3d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    out[0] = AfxSin(v[0]);
    out[1] = AfxSin(v[1]);
    out[2] = AfxSin(v[2]);
}

_AFXINL void AfxV4dSin(afxV4d const v, afxV4d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    out[0] = AfxSin(v[0]);
    out[1] = AfxSin(v[1]);
    out[2] = AfxSin(v[2]);
    out[3] = AfxSin(v[3]);
}

// Cosine

_AFXINL void AfxV2dCos(afxV2d const v, afxV2d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    out[0] = AfxCos(v[0]);
    out[1] = AfxCos(v[1]);
}

_AFXINL void AfxV3dCos(afxV3d const v, afxV3d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    out[0] = AfxCos(v[0]);
    out[1] = AfxCos(v[1]);
    out[2] = AfxCos(v[2]);
}

_AFXINL void AfxV4dCos(afxV4d const v, afxV4d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    out[0] = AfxCos(v[0]);
    out[1] = AfxCos(v[1]);
    out[2] = AfxCos(v[2]);
    out[3] = AfxCos(v[3]);
}

_AFXINL void AfxV2dAcos(afxV2d const v, afxV2d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    out[0] = AfxAcos(v[0]);
    out[1] = AfxAcos(v[1]);
}

_AFXINL void AfxV3dAcos(afxV3d const v, afxV3d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    out[0] = AfxAcos(v[0]);
    out[1] = AfxAcos(v[1]);
    out[2] = AfxAcos(v[2]);
}

_AFXINL void AfxV4dAcos(afxV4d const v, afxV4d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    out[0] = AfxAcos(v[0]);
    out[1] = AfxAcos(v[1]);
    out[2] = AfxAcos(v[2]);
    out[3] = AfxAcos(v[3]);
}

// Tangent

_AFXINL void AfxV2dTan(afxV2d const v, afxV2d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    out[0] = AfxTan(v[0]);
    out[1] = AfxTan(v[1]);
}

_AFXINL void AfxV3dTan(afxV3d const v, afxV3d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    out[0] = AfxTan(v[0]);
    out[1] = AfxTan(v[1]);
    out[2] = AfxTan(v[2]);
}

_AFXINL void AfxV4dTan(afxV4d const v, afxV4d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    out[0] = AfxTan(v[0]);
    out[1] = AfxTan(v[1]);
    out[2] = AfxTan(v[2]);
    out[3] = AfxTan(v[3]);
}

_AFXINL void AfxV2dAtan(afxV2d const v, afxV2d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    out[0] = AfxAtan(v[0]);
    out[1] = AfxAtan(v[1]);
}

_AFXINL void AfxV3dAtan(afxV3d const v, afxV3d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    out[0] = AfxAtan(v[0]);
    out[1] = AfxAtan(v[1]);
    out[2] = AfxAtan(v[2]);
}

_AFXINL void AfxV4dAtan(afxV4d const v, afxV4d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    out[0] = AfxAtan(v[0]);
    out[1] = AfxAtan(v[1]);
    out[2] = AfxAtan(v[2]);
    out[3] = AfxAtan(v[3]);
}

// Lerp

_AFXINL void AfxV2dLerp(afxV2d const v, afxV2d const other, afxReal s, afxV2d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxAssert(other);
    out[0] = v[0] + s * (other[0] - v[0]);
    out[1] = v[1] + s * (other[1] - v[1]);
}

_AFXINL void AfxV3dLerp(afxV3d const v, afxV3d const other, afxReal s, afxV3d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxAssert(other);
    out[0] = v[0] + s * (other[0] - v[0]);
    out[1] = v[1] + s * (other[1] - v[1]);
    out[2] = v[2] + s * (other[2] - v[2]);
}

_AFXINL void AfxV4dLerp(afxV4d const v, afxV4d const other, afxReal s, afxV4d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxAssert(other);
    out[0] = v[0] + s * (other[0] - v[0]);
    out[1] = v[1] + s * (other[1] - v[1]);
    out[2] = v[2] + s * (other[2] - v[2]);
    out[3] = v[3] + s * (other[3] - v[3]);
}

_AFXINL void AfxQuatLerp(afxQuat const q, afxQuat const other, afxReal percent, afxQuat out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(q);
    AfxAssert(other);
    AfxAssert(out);

    if (AfxRealIsEqual(percent, AfxScalar(0))) AfxQuatCopy(out, q);
    else
    {
        if (AfxRealIsEqual(percent, AfxScalar(1))) AfxQuatCopy(out, other);
        else
        {
            afxReal f1 = AfxScalar(1) - percent, f2 = percent;

            // mul & add
            out[3] = f1 * q[3] + f2 * other[3];
            out[0] = f1 * q[0] + f2 * other[0];
            out[1] = f1 * q[1] + f2 * other[1];
            out[2] = f1 * q[2] + f2 * other[2];
        }
    }
}

// Slerp

_AFXINL void AfxQuatSlerp(afxQuat const q, afxQuat const other, afxReal percent, afxQuat out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(q);
    AfxAssert(other);
    AfxAssert(out);

    if (AfxRealIsEqual(percent, AfxScalar(0))) AfxQuatCopy(out, q);
    else
    {
        if (AfxRealIsEqual(percent, AfxScalar(1))) AfxQuatCopy(out, other);
        else
        {
            // if they are close out parallel, use LERP, This avoids div/0. At small angles, the slerp q lerp are the same.
            afxReal dot = AfxQuatDot(q, other);

            if (AfxRealIsEqual(dot, AfxScalar(1))) AfxQuatLerp(q, out, percent, out);
            else
            {
                // if dot is negative, they are "pointing" away from one another, use the shortest arc instead (reverse end q start)
                // This has the effect of changing the direction of travel around the sphere beginning with "end" q going the other way around the sphere.

                if (dot < AfxScalar(0))
                {
                    afxQuat neg;
                    AfxQuatNegate(q, neg);
                    AfxQuatSlerp(q, neg, percent, out);
                    AfxQuatNegate(out, out);
                }
                else
                {
                    // keep the dot product in the range that acos canv handle (shouldn't get here)
                    dot = AfxClamp(AfxScalar(-1), dot, AfxScalar(1));
                    afxReal theta = AfxAcos(dot); // the angle between start q end in radians
                    afxReal s = AfxSin(theta), f1 = AfxSin((AfxScalar(1) - percent) * theta) / s, f2 = AfxSin(percent * theta) / s; // compute negative q positive

                    // mul & add
                    out[3] = f1 * q[3] + f2 * other[3];
                    out[0] = f1 * q[0] + f2 * other[0];
                    out[1] = f1 * q[1] + f2 * other[1];
                    out[2] = f1 * q[2] + f2 * other[2];
                }
            }
        }
    }
}
