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

// Memory layout: row-major

_AFX afxV4d const AFX_V4D_IDENTITY = { AfxScalar(0), AfxScalar(0), AfxScalar(0), AfxScalar(1) };
_AFX afxQuat const AFX_QUAT_IDENTITY = { AfxScalar(0), AfxScalar(0), AfxScalar(0), AfxScalar(1) };

_AFX afxM2d const AFX_M2D_IDENTITY =
{
    { AfxScalar(1), AfxScalar(0) },
    { AfxScalar(0), AfxScalar(1) },
};

_AFX afxM3d const AFX_M3D_IDENTITY =
{
    { AfxScalar(1), AfxScalar(0), AfxScalar(0) },
    { AfxScalar(0), AfxScalar(1), AfxScalar(0) },
    { AfxScalar(0), AfxScalar(0), AfxScalar(1) }
};

_AFX afxM4d const AFX_M4D_IDENTITY =
{
    { AfxScalar(1), AfxScalar(0), AfxScalar(0), AfxScalar(0) },
    { AfxScalar(0), AfxScalar(1), AfxScalar(0), AfxScalar(0) },
    { AfxScalar(0), AfxScalar(0), AfxScalar(1), AfxScalar(0) },
    { AfxScalar(0), AfxScalar(0), AfxScalar(0), AfxScalar(1) }
};

// IsIdentity

_AFXINL afxBool AfxV4dIsIdentity(afxV4d const v)
{
    return (v[0] + v[1] + v[2]) == 0 && (v[3] == 1);
}

_AFXINL afxBool AfxQuatIsIdentity(afxQuat const q)
{
    return (q[0] + q[1] + q[2]) == 0 && (q[3] == 1);
}

_AFXINL afxBool AfxM2dIsIdentity(afxM2d const m)
{
    afxBool flag = 1;

    for (afxNat i = 0; flag && i < 2; i++)
        for (afxNat j = 0; flag && j < 2; j++)
            if (m[i][j] != 1 && m[j][i] != 0)
                flag = 0;

    return flag;
}

_AFXINL afxBool AfxM3dIsIdentity(afxM3d const m)
{
    afxBool flag = 1;

    for (afxNat i = 0; flag && i < 3; i++)
        for (afxNat j = 0; flag && j < 3; j++)
            if (m[i][j] != 1 && m[j][i] != 0)
                flag = 0;

    return flag;
}

_AFXINL afxBool AfxM4dIsIdentity(afxM4d const m)
{
    afxBool flag = 1;

    for (afxNat i = 0; flag && i < 4; i++)
        for (afxNat j = 0; flag && j < 4; j++)
            if (m[i][j] != 1 && m[j][i] != 0)
                flag = 0;

    return flag;
}

// MakeIdentity

_AFXINL void AfxV4dMakeIdentity(afxV4d v)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxV4dCopy(v, AFX_V4D_IDENTITY);
}

_AFXINL void AfxQuatMakeIdentity(afxQuat q)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(q);
    AfxQuatCopy(q, AFX_QUAT_IDENTITY);
}

_AFXINL void AfxM2dMakeIdentity(afxM2d m)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxM2dCopy(m, AFX_M2D_IDENTITY);
}

_AFXINL void AfxM3dMakeIdentity(afxM3d m)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxM3dCopy(m, AFX_M3D_IDENTITY);
}

_AFXINL void AfxM4dMakeIdentity(afxM4d m)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxM4dCopy(m, AFX_M4D_IDENTITY);
}

// Negate

_AFXINL void AfxV2dNegate(afxV2d const v, afxV2d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    out[0] = -(v[0]);
    out[1] = -(v[1]);
}

_AFXINL void AfxV3dNegate(afxV3d const v, afxV3d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    out[0] = -(v[0]);
    out[1] = -(v[1]);
    out[2] = -(v[2]);
}

_AFXINL void AfxV4dNegate(afxV4d const v, afxV4d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    out[0] = -(v[0]);
    out[1] = -(v[1]);
    out[2] = -(v[2]);
    out[3] = -(v[3]);
}

_AFXINL void AfxV4dNegatePoint(afxV4d const v, afxV4d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    out[0] = -(v[0]);
    out[1] = -(v[1]);
    out[2] = -(v[2]);
    out[3] =  (v[3]);
}

_AFXINL void AfxQuatNegate(afxQuat const q, afxQuat out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(q);
    AfxV4dNegate(q, out);
}

_AFXINL void AfxQuatConjugate(afxQuat const q, afxQuat out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(q);
    out[0] = -q[0];
    out[1] = -q[1];
    out[2] = -q[2];
    out[3] =  q[3]; // w is preserved
}

_AFXINL void AfxQuatInvert(afxQuat const q, afxQuat out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(q);
    AfxAssert(out);

    afxReal norm = AfxQuatNorm(q);

    if (!AfxRealIsEqual(norm, 0))
    {
        AfxQuatConjugate(q, out);

        if (!AfxRealIsEqual(norm, 1.f))
        {
            afxV4d v = { norm, norm, norm, norm };
            AfxV4dDiv(out, v, out);
            return; // avoid fallback copy in the end.
        }
    }
    AfxQuatCopy(out, q);
}

_AFXINL afxReal AfxM3dInvert(afxM3d const m, afxM3d out)
{
    // Inspired on void __cdecl granny::MatrixInvert3x3(float *DestInit, const float *SourceInit)
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(out);
    AfxAssert(m != out);

    afxReal det = (m[1][1] * m[2][2] - m[2][1] * m[1][2]) * m[0][0] - (m[2][2] * m[1][0] - m[2][0] * m[1][2]) * m[0][1] + (m[2][1] * m[1][0] - m[2][0] * m[1][1]) * m[0][2];

    if (det != 0.0)
    {
        det = 1.0 / det;
        out[0][0] =   (m[1][1] * m[2][2] - m[2][1] * m[1][2]) * det;
        out[1][0] = -((m[2][2] * m[1][0] - m[2][0] * m[1][2]) * det);
        out[2][0] =   (m[2][1] * m[1][0] - m[2][0] * m[1][1]) * det;
        out[0][1] = -((m[0][1] * m[2][2] - m[2][1] * m[0][2]) * det);
        out[1][1] =   (m[2][2] * m[0][0] - m[2][0] * m[0][2]) * det;
        out[2][1] = -((m[2][1] * m[0][0] - m[2][0] * m[0][1]) * det);
        out[0][2] =   (m[0][1] * m[1][2] - m[1][1] * m[0][2]) * det;
        out[1][2] = -((m[1][2] * m[0][0] - m[0][2] * m[1][0]) * det);
        out[2][2] =   (m[1][1] * m[0][0] - m[0][1] * m[1][0]) * det;
    }
    else
    {
        AfxM3dCopy(out, m);
    }
    return det;
}

// Invert
// Memory layout: hybrid

_AFXINL afxReal AfxM4dInvert(afxM4d const m, afxM4d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(out);
    AfxAssert(m != out);

    // From StackOverflow, by wangzhe, at https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix

    out[0][0] =  m[1][1] * m[2][2] * m[3][3] - m[1][1] * m[2][3] * m[3][2] - m[2][1] * m[1][2] * m[3][3] + m[2][1] * m[1][3] * m[3][2] + m[3][1] * m[1][2] * m[2][3] - m[3][1] * m[1][3] * m[2][2];
    out[1][0] = -m[1][0] * m[2][2] * m[3][3] + m[1][0] * m[2][3] * m[3][2] + m[2][0] * m[1][2] * m[3][3] - m[2][0] * m[1][3] * m[3][2] - m[3][0] * m[1][2] * m[2][3] + m[3][0] * m[1][3] * m[2][2];
    out[2][0] =  m[1][0] * m[2][1] * m[3][3] - m[1][0] * m[2][3] * m[3][1] - m[2][0] * m[1][1] * m[3][3] + m[2][0] * m[1][3] * m[3][1] + m[3][0] * m[1][1] * m[2][3] - m[3][0] * m[1][3] * m[2][1];
    out[3][0] = -m[1][0] * m[2][1] * m[3][2] + m[1][0] * m[2][2] * m[3][1] + m[2][0] * m[1][1] * m[3][2] - m[2][0] * m[1][2] * m[3][1] - m[3][0] * m[1][1] * m[2][2] + m[3][0] * m[1][2] * m[2][1];
    out[0][1] = -m[0][1] * m[2][2] * m[3][3] + m[0][1] * m[2][3] * m[3][2] + m[2][1] * m[0][2] * m[3][3] - m[2][1] * m[0][3] * m[3][2] - m[3][1] * m[0][2] * m[2][3] + m[3][1] * m[0][3] * m[2][2];
    out[1][1] =  m[0][0] * m[2][2] * m[3][3] - m[0][0] * m[2][3] * m[3][2] - m[2][0] * m[0][2] * m[3][3] + m[2][0] * m[0][3] * m[3][2] + m[3][0] * m[0][2] * m[2][3] - m[3][0] * m[0][3] * m[2][2];
    out[2][1] = -m[0][0] * m[2][1] * m[3][3] + m[0][0] * m[2][3] * m[3][1] + m[2][0] * m[0][1] * m[3][3] - m[2][0] * m[0][3] * m[3][1] - m[3][0] * m[0][1] * m[2][3] + m[3][0] * m[0][3] * m[2][1];
    out[3][1] =  m[0][0] * m[2][1] * m[3][2] - m[0][0] * m[2][2] * m[3][1] - m[2][0] * m[0][1] * m[3][2] + m[2][0] * m[0][2] * m[3][1] + m[3][0] * m[0][1] * m[2][2] - m[3][0] * m[0][2] * m[2][1];
    out[0][2] =  m[0][1] * m[1][2] * m[3][3] - m[0][1] * m[1][3] * m[3][2] - m[1][1] * m[0][2] * m[3][3] + m[1][1] * m[0][3] * m[3][2] + m[3][1] * m[0][2] * m[1][3] - m[3][1] * m[0][3] * m[1][2];
    out[1][2] = -m[0][0] * m[1][2] * m[3][3] + m[0][0] * m[1][3] * m[3][2] + m[1][0] * m[0][2] * m[3][3] - m[1][0] * m[0][3] * m[3][2] - m[3][0] * m[0][2] * m[1][3] + m[3][0] * m[0][3] * m[1][2];
    out[2][2] =  m[0][0] * m[1][1] * m[3][3] - m[0][0] * m[1][3] * m[3][1] - m[1][0] * m[0][1] * m[3][3] + m[1][0] * m[0][3] * m[3][1] + m[3][0] * m[0][1] * m[1][3] - m[3][0] * m[0][3] * m[1][1];
    out[3][2] = -m[0][0] * m[1][1] * m[3][2] + m[0][0] * m[1][2] * m[3][1] + m[1][0] * m[0][1] * m[3][2] - m[1][0] * m[0][2] * m[3][1] - m[3][0] * m[0][1] * m[1][2] + m[3][0] * m[0][2] * m[1][1];
    out[0][3] = -m[0][1] * m[1][2] * m[2][3] + m[0][1] * m[1][3] * m[2][2] + m[1][1] * m[0][2] * m[2][3] - m[1][1] * m[0][3] * m[2][2] - m[2][1] * m[0][2] * m[1][3] + m[2][1] * m[0][3] * m[1][2];
    out[1][3] =  m[0][0] * m[1][2] * m[2][3] - m[0][0] * m[1][3] * m[2][2] - m[1][0] * m[0][2] * m[2][3] + m[1][0] * m[0][3] * m[2][2] + m[2][0] * m[0][2] * m[1][3] - m[2][0] * m[0][3] * m[1][2];
    out[2][3] = -m[0][0] * m[1][1] * m[2][3] + m[0][0] * m[1][3] * m[2][1] + m[1][0] * m[0][1] * m[2][3] - m[1][0] * m[0][3] * m[2][1] - m[2][0] * m[0][1] * m[1][3] + m[2][0] * m[0][3] * m[1][1];
    out[3][3] =  m[0][0] * m[1][1] * m[2][2] - m[0][0] * m[1][2] * m[2][1] - m[1][0] * m[0][1] * m[2][2] + m[1][0] * m[0][2] * m[2][1] + m[2][0] * m[0][1] * m[1][2] - m[2][0] * m[0][2] * m[1][1];

    afxReal det = m[0][0] * out[0][0] + m[0][1] * out[1][0] + m[0][2] * out[2][0] + m[0][3] * out[3][0];

    if (det)
    {
        det = 1.0 / det;

        for (afxNat i = 0; i < 4; i++)
            for (afxNat j = 0; j < 4; j++)
                out[i][j] = out[i][j] * det;
    }
    return det;
}

_AFXINL afxReal AfxM4dInvert4x3(afxM4d const m, afxM4d out)
{
    // Inspired on void __cdecl granny::MatrixInvert4x3(float *DestInit, const float *SourceInit)
    AfxM4dCopy(out, m);

    afxReal det = (m[1][1] * m[2][2] - m[2][1] * m[1][2]) * m[0][0] - (m[2][2] * m[0][1] - m[2][1] * m[0][2]) * m[1][0] + (m[1][2] * m[0][1] - m[1][1] * m[0][2]) * m[2][0];

    if (det)
    {
        afxReal recip = 1.0 / det;
        out[0][0] =   (m[1][1] * m[2][2] - m[2][1] * m[1][2]) * recip;
        out[0][1] = -((m[2][2] * m[0][1] - m[2][1] * m[0][2]) * recip);
        out[0][2] =   (m[1][2] * m[0][1] - m[1][1] * m[0][2]) * recip;
        out[1][0] = -((m[1][0] * m[2][2] - m[2][0] * m[1][2]) * recip);
        out[1][1] =   (m[2][2] * m[0][0] - m[2][0] * m[0][2]) * recip;
        out[1][2] = -((m[1][2] * m[0][0] - m[0][2] * m[1][0]) * recip);
        out[2][0] =   (m[2][1] * m[1][0] - m[2][0] * m[1][1]) * recip;
        out[2][1] = -((m[2][1] * m[0][0] - m[2][0] * m[0][1]) * recip);
        out[2][2] =   (m[1][1] * m[0][0] - m[1][0] * m[0][1]) * recip;
    }
    out[0][3] = 0.f;
    out[1][3] = 0.f;
    out[2][3] = 0.f;
    out[3][0] = -( out[2][0] * m[3][2] + out[1][0] * m[3][1]  +  out[0][0] * m[3][0]);
    out[3][1] = -(m[2][1] * m[3][2] + out[0][1] * m[3][0]  +  out[1][1] * m[3][1]);
    out[3][2] = -((out[2][2] * m[3][2] + out[0][2] * m[3][0]) + (out[1][2] * m[3][1]));
    out[3][3] = 1.f;
    return det;
}

// Multiply

_AFXINL void AfxV2dMultiply(afxV2d const v, afxV2d const other, afxV2d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxAssert(other);
    out[0] = v[0] * other[0];
    out[1] = v[1] * other[1];
}

_AFXINL void AfxV3dMultiply(afxV3d const v, afxV3d const other, afxV3d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxAssert(other);
    out[0] = v[0] * other[0];
    out[1] = v[1] * other[1];
    out[2] = v[2] * other[2];
}

_AFXINL void AfxV4dMultiply(afxV4d const v, afxV4d const other, afxV4d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxAssert(other);
    out[0] = v[0] * other[0];
    out[1] = v[1] * other[1];
    out[2] = v[2] * other[2];
    out[3] = v[3] * other[3];
}

_AFXINL void AfxQuatMultiply(afxQuat const q, afxQuat const other, afxQuat out)
{
    // From DXMath
    // The result represents the rotation Q1 followed by the rotation Q2 to be consistent with MatrixMulplity concatenation since this function is typically used to concatenate quaternions that represent rotations(i.e.it returns Q2*Q1).
    // Returns the product Q2*Q1 (which is the concatenation of q rotation Q1 followed by the rotation Q2)
    afxError err = AFX_ERR_NONE;
    AfxAssert(q);
    AfxAssert(other);
    AfxAssert(q != other);
    AfxAssert(out);
    out[0] = (other[3] * q[0]) + (other[0] * q[3]) + (other[1] * q[2]) - (other[2] * q[1]);
    out[1] = (other[3] * q[1]) - (other[0] * q[2]) + (other[1] * q[3]) + (other[2] * q[0]);
    out[2] = (other[3] * q[2]) + (other[0] * q[1]) - (other[1] * q[0]) + (other[2] * q[3]);
    out[3] = (other[3] * q[3]) - (other[0] * q[0]) - (other[1] * q[1]) - (other[2] * q[2]);
}

_AFXINL void AfxM2dMultiply(afxM2d const m, afxM2d const other, afxM2d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(m);
    AfxAssert(other);
    AfxAssert(m != other);
    AfxAssert(other != out);
    afxV2d v;

    AfxV2dCopy(v, m[0]);
    out[0][0] = (other[0][0] * v[0]) + (other[1][0] * v[1]);
    out[0][1] = (other[0][1] * v[0]) + (other[1][1] * v[1]);

    AfxV2dCopy(v, m[1]);
    out[1][0] = (other[0][0] * v[0]) + (other[1][0] * v[1]);
    out[1][1] = (other[0][1] * v[0]) + (other[1][1] * v[1]);
}

_AFXINL void AfxM3dMultiply(afxM3d const m, afxM3d const other, afxM3d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(m);
    AfxAssert(other);
    AfxAssert(m != other);
    AfxAssert(other != out);
    afxV3d v;

    AfxV3dCopy(v, m[0]);
    out[0][0] = (other[0][0] * v[0]) + (other[1][0] * v[1]) + (other[2][0] * v[2]);
    out[0][1] = (other[0][1] * v[0]) + (other[1][1] * v[1]) + (other[2][1] * v[2]);
    out[0][2] = (other[0][2] * v[0]) + (other[1][2] * v[1]) + (other[2][2] * v[2]);

    AfxV3dCopy(v, m[1]);
    out[1][0] = (other[0][0] * v[0]) + (other[1][0] * v[1]) + (other[2][0] * v[2]);
    out[1][1] = (other[0][1] * v[0]) + (other[1][1] * v[1]) + (other[2][1] * v[2]);
    out[1][2] = (other[0][2] * v[0]) + (other[1][2] * v[1]) + (other[2][2] * v[2]);

    AfxV3dCopy(v, m[2]);
    out[2][0] = (other[0][0] * v[0]) + (other[1][0] * v[1]) + (other[2][0] * v[2]);
    out[2][1] = (other[0][1] * v[0]) + (other[1][1] * v[1]) + (other[2][1] * v[2]);
    out[2][2] = (other[0][2] * v[0]) + (other[1][2] * v[1]) + (other[2][2] * v[2]);
}

_AFXINL void AfxM4dMultiply(afxM4d const m, afxM4d const other, afxM4d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(m);
    AfxAssert(other);
    AfxAssert(m != other);
    AfxAssert(other != out);
    afxV4d v;

    AfxV4dCopy(v, m[0]);
    out[0][0] = (other[0][0] * v[0]) + (other[1][0] * v[1]) + (other[2][0] * v[2]) + (other[3][0] * v[3]);
    out[0][1] = (other[0][1] * v[0]) + (other[1][1] * v[1]) + (other[2][1] * v[2]) + (other[3][1] * v[3]);
    out[0][2] = (other[0][2] * v[0]) + (other[1][2] * v[1]) + (other[2][2] * v[2]) + (other[3][2] * v[3]);
    out[0][3] = (other[0][3] * v[0]) + (other[1][3] * v[1]) + (other[2][3] * v[2]) + (other[3][3] * v[3]);
    
    AfxV4dCopy(v, m[1]);
    out[1][0] = (other[0][0] * v[0]) + (other[1][0] * v[1]) + (other[2][0] * v[2]) + (other[3][0] * v[3]);
    out[1][1] = (other[0][1] * v[0]) + (other[1][1] * v[1]) + (other[2][1] * v[2]) + (other[3][1] * v[3]);
    out[1][2] = (other[0][2] * v[0]) + (other[1][2] * v[1]) + (other[2][2] * v[2]) + (other[3][2] * v[3]);
    out[1][3] = (other[0][3] * v[0]) + (other[1][3] * v[1]) + (other[2][3] * v[2]) + (other[3][3] * v[3]);
    
    AfxV4dCopy(v, m[2]);
    out[2][0] = (other[0][0] * v[0]) + (other[1][0] * v[1]) + (other[2][0] * v[2]) + (other[3][0] * v[3]);
    out[2][1] = (other[0][1] * v[0]) + (other[1][1] * v[1]) + (other[2][1] * v[2]) + (other[3][1] * v[3]);
    out[2][2] = (other[0][2] * v[0]) + (other[1][2] * v[1]) + (other[2][2] * v[2]) + (other[3][2] * v[3]);
    out[2][3] = (other[0][3] * v[0]) + (other[1][3] * v[1]) + (other[2][3] * v[2]) + (other[3][3] * v[3]);
    
    AfxV4dCopy(v, m[3]);
    out[3][0] = (other[0][0] * v[0]) + (other[1][0] * v[1]) + (other[2][0] * v[2]) + (other[3][0] * v[3]);
    out[3][1] = (other[0][1] * v[0]) + (other[1][1] * v[1]) + (other[2][1] * v[2]) + (other[3][1] * v[3]);
    out[3][2] = (other[0][2] * v[0]) + (other[1][2] * v[1]) + (other[2][2] * v[2]) + (other[3][2] * v[3]);
    out[3][3] = (other[0][3] * v[0]) + (other[1][3] * v[1]) + (other[2][3] * v[2]) + (other[3][3] * v[3]);
}

_AFXINL void AfxM4dMultiply4x3(afxM4d const m, afxM4d const other, afxM4d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(m);
    AfxAssert(other);
    AfxAssert(m != other);
    AfxAssert(other != out);
    afxV4d v;

    AfxV4dCopy(v, m[0]);
    out[0][0] = (other[0][0] * v[0]) + (other[1][0] * v[1]) + (other[2][0] * v[2]);
    out[0][1] = (other[0][1] * v[0]) + (other[1][1] * v[1]) + (other[2][1] * v[2]);
    out[0][2] = (other[0][2] * v[0]) + (other[1][2] * v[1]) + (other[2][2] * v[2]);
    out[0][3] = 0.f;

    AfxV4dCopy(v, m[1]);
    out[1][0] = (other[0][0] * v[0]) + (other[1][0] * v[1]) + (other[2][0] * v[2]);
    out[1][1] = (other[0][1] * v[0]) + (other[1][1] * v[1]) + (other[2][1] * v[2]);
    out[1][2] = (other[0][2] * v[0]) + (other[1][2] * v[1]) + (other[2][2] * v[2]);
    out[1][3] = 0.f;

    AfxV4dCopy(v, m[2]);
    out[2][0] = (other[0][0] * v[0]) + (other[1][0] * v[1]) + (other[2][0] * v[2]);
    out[2][1] = (other[0][1] * v[0]) + (other[1][1] * v[1]) + (other[2][1] * v[2]);
    out[2][2] = (other[0][2] * v[0]) + (other[1][2] * v[1]) + (other[2][2] * v[2]);
    out[2][3] = 0.f;

    AfxV4dCopy(v, m[3]);
    out[3][0] = (other[0][0] * v[0]) + (other[1][0] * v[1]) + (other[2][0] * v[2]) + (other[3][0]);
    out[3][1] = (other[0][1] * v[0]) + (other[1][1] * v[1]) + (other[2][1] * v[2]) + (other[3][1]);
    out[3][2] = (other[0][2] * v[0]) + (other[1][2] * v[1]) + (other[2][2] * v[2]) + (other[3][2]);
    out[3][3] = 1.f;
}

_AFXINL void AfxV2dMultiplyM2d(afxV2d const v, afxM2d const m, afxV2d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(m);
    AfxAssert(out);
    AfxAssert(out != v);
    out[0] = (v[0] * m[0][0]) + (v[1] * m[1][0]);
    out[1] = (v[0] * m[0][1]) + (v[1] * m[1][1]);
}

_AFXINL void AfxV3dMultiplyM3d(afxV3d const v, afxM3d const m, afxV3d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(m);
    AfxAssert(out);
    AfxAssert(out != v);
    out[0] = (v[0] * m[0][0]) + (v[1] * m[1][0]) + (v[2] * m[2][0]);
    out[1] = (v[0] * m[0][1]) + (v[1] * m[1][1]) + (v[2] * m[2][1]);
    out[2] = (v[0] * m[0][2]) + (v[1] * m[1][2]) + (v[2] * m[2][2]);
}

_AFXINL void AfxV2dMultiplyM4d(afxV2d const v, afxM4d const m, afxV2d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(m);
    AfxAssert(out);
    AfxAssert(out != v);
    out[0] = (v[0] * m[0][0]) + (v[1] * m[1][0]) + m[2][0] + m[3][0];
    out[1] = (v[0] * m[0][1]) + (v[1] * m[1][1]) + m[2][1] + m[3][1];
}

_AFXINL void AfxV3dMultiplyM4d(afxV3d const v, afxM4d const m, afxV3d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(m);
    AfxAssert(out);
    AfxAssert(out != v);
    out[0] = (v[0] * m[0][0]) + (v[1] * m[1][0]) + (v[2] * m[2][0]) + m[3][0];
    out[1] = (v[0] * m[0][1]) + (v[1] * m[1][1]) + (v[2] * m[2][1]) + m[3][1];
    out[2] = (v[0] * m[0][2]) + (v[1] * m[1][2]) + (v[2] * m[2][2]) + m[3][2];
}

_AFXINL void AfxV4dMultiplyM4d(afxV4d const v, afxM4d const m, afxV4d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(m);
    AfxAssert(out);
    AfxAssert(out != v);
    // it is a post-multiply once matrix is postponed v operation.
    out[0] = (v[0] * m[0][0]) + (v[1] * m[1][0]) + (v[2] * m[2][0]) + (v[3] * m[3][0]);
    out[1] = (v[0] * m[0][1]) + (v[1] * m[1][1]) + (v[2] * m[2][1]) + (v[3] * m[3][1]);
    out[2] = (v[0] * m[0][2]) + (v[1] * m[1][2]) + (v[2] * m[2][2]) + (v[3] * m[3][2]);
    out[3] = (v[0] * m[0][3]) + (v[1] * m[1][3]) + (v[2] * m[2][3]) + (v[3] * m[3][3]);
}

// MultiplyTransposed

_AFXINL void AfxM2dMultiplyTransposed(afxM2d const m, afxM2d const other, afxM2d out)
{
    // Computes the transpose of the product of two matrices.
    // WARNING: Qwadro uses row-major matrices. Using this function mean compute on colum-major matrix.
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(m);
    AfxAssert(other);
    AfxAssert(m != other);
    AfxAssert(other != out);
    afxV2d v;

    AfxV2dSet(v, other[0][0], other[1][0]);
    out[0][0] = (m[0][0] * v[0]) + (m[0][1] * v[1]);
    out[0][1] = (m[1][0] * v[0]) + (m[1][1] * v[1]);

    AfxV2dSet(v, other[0][1], other[1][1]);
    out[1][0] = (m[0][0] * v[0]) + (m[0][1] * v[1]);
    out[1][1] = (m[1][0] * v[0]) + (m[1][1] * v[1]);
}

_AFXINL void AfxM3dMultiplyTransposed(afxM3d const m, afxM3d const other, afxM3d out)
{
    // Computes the transpose of the product of two matrices.
    // WARNING: Qwadro uses row-major matrices. Using this function mean compute on colum-major matrix.
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(m);
    AfxAssert(other);
    AfxAssert(m != other);
    AfxAssert(other != out);
    afxV3d v;

    AfxV3dSet(v, other[0][0], other[1][0], other[2][0]);
    out[0][0] = (m[0][0] * v[0]) + (m[0][1] * v[1]) + (m[0][2] * v[2]);
    out[0][1] = (m[1][0] * v[0]) + (m[1][1] * v[1]) + (m[1][2] * v[2]);
    out[0][2] = (m[2][0] * v[0]) + (m[2][1] * v[1]) + (m[2][2] * v[2]);

    AfxV3dSet(v, other[0][1], other[1][1], other[2][1]);
    out[1][0] = (m[0][0] * v[0]) + (m[0][1] * v[1]) + (m[0][2] * v[2]);
    out[1][1] = (m[1][0] * v[0]) + (m[1][1] * v[1]) + (m[1][2] * v[2]);
    out[1][2] = (m[2][0] * v[0]) + (m[2][1] * v[1]) + (m[2][2] * v[2]);

    AfxV3dSet(v, other[0][2], other[1][2], other[2][2]);
    out[2][0] = (m[0][0] * v[0]) + (m[0][1] * v[1]) + (m[0][2] * v[2]);
    out[2][1] = (m[1][0] * v[0]) + (m[1][1] * v[1]) + (m[1][2] * v[2]);
    out[2][2] = (m[2][0] * v[0]) + (m[2][1] * v[1]) + (m[2][2] * v[2]);
}

_AFXINL void AfxM4dMultiplyTransposed(afxM4d const m, afxM4d const other, afxM4d out)
{
    // Computes the transpose of the product of two matrices.
    // WARNING: Qwadro uses row-major matrices. Using this function mean compute on colum-major matrix.
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(m);
    AfxAssert(other);
    AfxAssert(m != other);
    AfxAssert(other != out);
    afxV4d v;

    AfxV4dSet(v, other[0][0], other[1][0], other[2][0], other[3][0]);
    out[0][0] = (m[0][0] * v[0]) + (m[0][1] * v[1]) + (m[0][2] * v[2]) + (m[0][3] * v[3]);
    out[0][1] = (m[1][0] * v[0]) + (m[1][1] * v[1]) + (m[1][2] * v[2]) + (m[1][3] * v[3]);
    out[0][2] = (m[2][0] * v[0]) + (m[2][1] * v[1]) + (m[2][2] * v[2]) + (m[2][3] * v[3]);
    out[0][3] = (m[3][0] * v[0]) + (m[3][1] * v[1]) + (m[3][2] * v[2]) + (m[3][3] * v[3]);
    
    AfxV4dSet(v, other[0][1], other[1][1], other[2][1], other[3][1]);
    out[1][0] = (m[0][0] * v[0]) + (m[0][1] * v[1]) + (m[0][2] * v[2]) + (m[0][3] * v[3]);
    out[1][1] = (m[1][0] * v[0]) + (m[1][1] * v[1]) + (m[1][2] * v[2]) + (m[1][3] * v[3]);
    out[1][2] = (m[2][0] * v[0]) + (m[2][1] * v[1]) + (m[2][2] * v[2]) + (m[2][3] * v[3]);
    out[1][3] = (m[3][0] * v[0]) + (m[3][1] * v[1]) + (m[3][2] * v[2]) + (m[3][3] * v[3]);
    
    AfxV4dSet(v, other[0][2], other[1][2], other[2][2], other[3][2]);
    out[2][0] = (m[0][0] * v[0]) + (m[0][1] * v[1]) + (m[0][2] * v[2]) + (m[0][3] * v[3]);
    out[2][1] = (m[1][0] * v[0]) + (m[1][1] * v[1]) + (m[1][2] * v[2]) + (m[1][3] * v[3]);
    out[2][2] = (m[2][0] * v[0]) + (m[2][1] * v[1]) + (m[2][2] * v[2]) + (m[2][3] * v[3]);
    out[2][3] = (m[3][0] * v[0]) + (m[3][1] * v[1]) + (m[3][2] * v[2]) + (m[3][3] * v[3]);
    
    AfxV4dSet(v, other[0][3], other[1][3], other[2][3], other[3][3]);
    out[3][0] = (m[0][0] * v[0]) + (m[0][1] * v[1]) + (m[0][2] * v[2]) + (m[0][3] * v[3]);
    out[3][1] = (m[1][0] * v[0]) + (m[1][1] * v[1]) + (m[1][2] * v[2]) + (m[1][3] * v[3]);
    out[3][2] = (m[2][0] * v[0]) + (m[2][1] * v[1]) + (m[2][2] * v[2]) + (m[2][3] * v[3]);
    out[3][3] = (m[3][0] * v[0]) + (m[3][1] * v[1]) + (m[3][2] * v[2]) + (m[3][3] * v[3]);
}

_AFXINL void AfxV3dMultiplyTransposedM3d(afxV3d const v, afxM3d const m, afxV3d out)
{
    // Computes the transpose of the product of two matrices.
    // WARNING: Qwadro uses row-major matrices. Using this function mean compute on colum-major matrix.
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(m);
    AfxAssert(v);
    AfxAssert(m != (void*)v);
    AfxAssert(v != (void*)out);

    out[0] = (m[0][0] * v[0]) + (m[0][1] * v[1]) + (m[0][2] * v[2]);
    out[1] = (m[1][0] * v[0]) + (m[1][1] * v[1]) + (m[1][2] * v[2]);
    out[2] = (m[2][0] * v[0]) + (m[2][1] * v[1]) + (m[2][2] * v[2]);
}

_AFXINL void AfxV3dMultiplyTransposedM4d(afxV3d const v, afxM4d const m, afxV3d out)
{
    // Computes the transpose of the product of two matrices.
    // WARNING: Qwadro uses row-major matrices. Using this function mean compute on colum-major matrix.
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(m);
    AfxAssert(v);
    AfxAssert(m != (void*)v);
    AfxAssert(v != (void*)out);

    out[0] = (m[0][0] * v[0]) + (m[0][1] * v[1]) + (m[0][2] * v[2]) + (m[0][3] * v[3]);
    out[1] = (m[1][0] * v[0]) + (m[1][1] * v[1]) + (m[1][2] * v[2]) + (m[1][3] * v[3]);
    out[2] = (m[2][0] * v[0]) + (m[2][1] * v[1]) + (m[2][2] * v[2]) + (m[2][3] * v[3]);
}

_AFXINL void AfxV4dMultiplyTransposedM4d(afxV4d const v, afxM4d const m, afxV4d out)
{
    // Computes the transpose of the product of two matrices.
    // WARNING: Qwadro uses row-major matrices. Using this function mean compute on colum-major matrix.
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(m);
    AfxAssert(v);
    AfxAssert(m != (void*)v);
    AfxAssert(v != (void*)out);

    out[0] = (m[0][0] * v[0]) + (m[0][1] * v[1]) + (m[0][2] * v[2]) + (m[0][3] * v[3]);
    out[1] = (m[1][0] * v[0]) + (m[1][1] * v[1]) + (m[1][2] * v[2]) + (m[1][3] * v[3]);
    out[2] = (m[2][0] * v[0]) + (m[2][1] * v[1]) + (m[2][2] * v[2]) + (m[2][3] * v[3]);
    out[3] = (m[3][0] * v[0]) + (m[3][1] * v[1]) + (m[3][2] * v[2]) + (m[3][3] * v[3]);
}

_AFXINL void AfxV3dRotateQuat(afxV3d const v, afxQuat const rot, afxV3d out)
{
    // Transform a vector using a rotation expressed as a unit quaternion
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxAssert(rot);
    AfxAssert(v != out);
    AfxAssert(rot != out);

    afxQuat a = { v[0], v[1], v[2], 0 }, b, c;
    AfxQuatConjugate(rot, b);
    AfxQuatMultiply(b, a, c);
    AfxQuatMultiply(c, rot, a);
    AfxV3dCopy(out, a);
}

// MultiplyArrayV2d

_AFXINL void AfxM2dMultiplyArrayV2d(afxM2d const m, afxNat cnt, afxV2d const v[], afxV2d out[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(m);
    AfxAssert(out);
    AfxAssert(cnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxAssert(out[i] != v[i]);
        AfxV2dMultiplyM2d(v[i], m, out[i]);
    }
}

// MultiplyArrayV3d

_AFXINL void AfxM3dMultiplyArrayV3d(afxM3d const m, afxNat cnt, afxV3d const v[], afxV3d out[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(m);
    AfxAssert(out);
    AfxAssert(cnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxAssert(out[i] != v[i]);
        AfxV3dMultiplyM3d(v[i], m, out[i]);
    }
}

_AFXINL void AfxM4dMultiplyArrayV3d(afxM4d const m, afxNat cnt, afxV3d const v[], afxV3d out[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(m);
    AfxAssert(out);
    AfxAssert(cnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxAssert(out[i] != v[i]);
        AfxV3dMultiplyM4d(v[i], m, out[i]);
    }
}

// MultiplyArrayV4d

_AFXINL void AfxM4dMultiplyArrayV4d(afxM4d const m, afxNat cnt, afxV4d const v[], afxV4d out[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(m);
    AfxAssert(out);
    AfxAssert(cnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxAssert(out[i] != v[i]);
        AfxV4dMultiplyM4d(v[i], m, out[i]);
    }
}

// Scale

_AFXINL void AfxM3dScaleV3d(afxM3d m, afxV3d const scale)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(scale);

    afxM3d tmp, tmp2;
    AfxM3dCopy(tmp, m);
    AfxV3dMakeScalingM3d(scale, tmp2);
    AfxM3dMultiply(m, tmp2, tmp);
}

_AFXINL void AfxM4dScaleV3d(afxM4d m, afxV3d const scale)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(scale);

    afxM4d tmp, tmp2;
    AfxM4dCopy(tmp, m);
    AfxV3dMakeScalingM4d(scale, tmp2);
    AfxM4dMultiply(m, tmp2, tmp);
}

_AFXINL void AfxM4dScaleV4d(afxM4d m, afxV4d const scale)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(scale);

    afxM4d tmp, tmp2;
    AfxM4dCopy(tmp, m);
    AfxV4dMakeScalingM4d(scale, tmp2);
    AfxM4dMultiply(m, tmp2, tmp);
}

// Translate

_AFXINL void AfxM4dTranslateV3d(afxM4d m, afxV3d const translation)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(translation);

    afxM4d tmp, tmp2;
    AfxM4dCopy(tmp, m);
    AfxV3dMakeTranslationM4d(translation, tmp2);
    AfxM4dMultiply(m, tmp2, tmp);
}

_AFXINL void AfxM4dTranslateV4d(afxM4d m, afxV4d const translation)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(translation);

    afxM4d tmp, tmp2;
    AfxM4dCopy(tmp, m);
    AfxV4dMakeTranslationM4d(translation, tmp2);
    AfxM4dMultiply(m, tmp2, tmp);
}

// Compose

_AFXINL void AfxM4dCompose(afxM4d m, afxV4d const scalOrigin, afxQuat const scalOrient, afxV3d const scaling, afxV4d const rotOrigin, afxQuat const rotQuat, afxV4d const translation)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(scalOrigin);
    AfxAssert(scalOrient);
    AfxAssert(scaling);
    AfxAssert(rotOrigin);
    AfxAssert(rotQuat);
    AfxAssert(translation);

    // M = Inverse(MScalingOrigin) * Transpose(MScalingOrientation) * MScaling * MScalingOrientation *
    //         MScalingOrigin * Inverse(MRotationOrigin) * MRotation * MRotationOrigin * MTranslation;

    afxV4d negScaOrigin;
    AfxV4dNegate(scalOrigin, negScaOrigin);

    afxM4d scalOriginI, scalOrientM, scalOrientT, scalM, rotM;
    AfxV4dMakeTranslationM4d(negScaOrigin, scalOriginI);
    AfxQuatMakeRotationM4d(scalOrient, scalOrientM);
    AfxM4dTranspose(scalOrientM, scalOrientT);
    AfxV3dMakeScalingM4d(scaling, scalM);
    AfxQuatMakeRotationM4d(rotQuat, rotM);

    afxM4d t;
    AfxM4dMultiply(scalOriginI, scalOrientT, t);
    AfxM4dMultiply(t, scalM, m);
    AfxM4dMultiply(m, scalOrientM, t);
    AfxV3dAdd(t[3], scalOrigin, t[3]);
    AfxV3dSub(t[3], rotOrigin, t[3]);
    AfxM4dMultiply(t, rotM, m);
    AfxV3dAdd(m[3], rotOrigin, m[3]);
    AfxV3dAdd(m[3], translation, m[3]);
}

_AFXINL void AfxM4dComposeAffine(afxM4d m, afxV3d const scale, afxV3d const rotAxis, afxQuat const rot, afxV4d const translation)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(scale);
    AfxAssert(rot);
    AfxAssert(translation);

    // M = MScaling * Inverse(MRotationOrigin) * MRotation * MRotationOrigin * MTranslation;

    afxM4d a, b, c;
    AfxV3dMakeScalingM4d(scale, a);
    AfxQuatMakeRotationM4d(rot, b);
    AfxV3dSub(a[3], rotAxis, a[3]);
    AfxM4dMultiply(a, b, m);
    AfxV3dAdd(m[3], rotAxis, m[3]);
    AfxV3dAdd(m[3], translation, m[3]);
}










// uncertain name

// MakeEulerRotation

_AFXINL void AfxM4dSetEulerRotation(afxM4d m, afxV3d const pitchYawRoll)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(pitchYawRoll);

    afxReal cp = AfxCos(pitchYawRoll[0]);
    afxReal sp = AfxSin(pitchYawRoll[0]);
    afxReal cy = AfxCos(pitchYawRoll[1]);
    afxReal sy = AfxSin(pitchYawRoll[1]);
    afxReal cr = AfxCos(pitchYawRoll[2]);
    afxReal sr = AfxSin(pitchYawRoll[2]);

    m[0][0] = cr * cy + sr * sp * sy;
    m[0][1] = sr * cp;
    m[0][2] = sr * sp * cy - cr * sy;
    m[0][3] = 0.f;

    m[1][0] = cr * sp * sy - sr * cy;
    m[1][1] = cr * cp;
    m[1][2] = sr * sy + cr * sp * cy;
    m[1][3] = 0.f;

    m[2][0] = cp * sy;
    m[2][1] = -sp;
    m[2][2] = cp * cy;
    m[2][3] = 0.f;

    m[3][0] = 0.f;
    m[3][1] = 0.f;
    m[3][2] = 0.f;
    m[3][3] = 1.f;
}

_AFXINL void AfxQuatSetEulerRotation(afxQuat q, afxV3d const pitchYawRoll)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(q);
    AfxAssert(pitchYawRoll);

    afxReal const halfpitch = pitchYawRoll[0] * 0.5f;
    afxReal cp = AfxCos(halfpitch);
    afxReal sp = AfxSin(halfpitch);
    afxReal const halfyaw   = pitchYawRoll[1] * 0.5f;
    afxReal cy = AfxCos(halfyaw);
    afxReal sy = AfxSin(halfyaw);
    afxReal const halfroll  = pitchYawRoll[2] * 0.5f;
    afxReal cr = AfxCos(halfroll);
    afxReal sr = AfxSin(halfroll);

    q[0] = cr * sp * cy + sr * cp * sy;
    q[1] = cr * cp * sy - sr * sp * cy;
    q[2] = sr * cp * cy - cr * sp * sy;
    q[3] = cr * cp * cy + sr * sp * sy;
}


// uncertain origin

// MakeAxialRotation
// Memory layout: looks like it is row-major

_AFXINL void AfxM3dMakeAxialRotation(afxM3d m, afxV3d const axis, afxReal radians)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(axis);

    afxReal sinTheta = AfxSin(radians), cosTheta = AfxCos(radians), cosValue = 1.0f - cosTheta;
    afxV3d axis2;
    AfxV3dNormalize(axis, axis2);

    m[0][0] = (axis2[0] * axis2[0] * cosValue) + cosTheta;
    m[0][1] = (axis2[0] * axis2[1] * cosValue) + (axis2[2] * sinTheta);
    m[0][2] = (axis2[0] * axis2[2] * cosValue) - (axis2[1] * sinTheta);

    m[1][0] = (axis2[1] * axis2[0] * cosValue) - (axis2[2] * sinTheta);
    m[1][1] = (axis2[1] * axis2[1] * cosValue) + cosTheta;
    m[1][2] = (axis2[1] * axis2[2] * cosValue) + (axis2[0] * sinTheta);

    m[2][0] = (axis2[2] * axis2[0] * cosValue) + (axis2[1] * sinTheta);
    m[2][1] = (axis2[2] * axis2[1] * cosValue) - (axis2[0] * sinTheta);
    m[2][2] = (axis2[2] * axis2[2] * cosValue) + cosTheta;
}

_AFXINL void AfxM4dMakeAxialRotation(afxM4d m, afxV3d const axis, afxReal radians)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(axis);

    afxReal sinTheta = AfxSin(radians), cosTheta = AfxCos(radians), cosValue = 1.0f - cosTheta;
    afxV3d axis2;
    AfxV3dNormalize(axis, axis2);

    m[0][0] = (axis2[0] * axis2[0] * cosValue) + cosTheta;
    m[0][1] = (axis2[0] * axis2[1] * cosValue) + (axis2[2] * sinTheta);
    m[0][2] = (axis2[0] * axis2[2] * cosValue) - (axis2[1] * sinTheta);
    m[0][3] = AfxScalar(0);

    m[1][0] = (axis2[1] * axis2[0] * cosValue) - (axis2[2] * sinTheta);
    m[1][1] = (axis2[1] * axis2[1] * cosValue) + cosTheta;
    m[1][2] = (axis2[1] * axis2[2] * cosValue) + (axis2[0] * sinTheta);
    m[1][3] = AfxScalar(0);

    m[2][0] = (axis2[2] * axis2[0] * cosValue) + (axis2[1] * sinTheta);
    m[2][1] = (axis2[2] * axis2[1] * cosValue) - (axis2[0] * sinTheta);
    m[2][2] = (axis2[2] * axis2[2] * cosValue) + cosTheta;
    m[2][3] = AfxScalar(0);

    m[3][0] = AfxScalar(0);
    m[3][1] = AfxScalar(0);
    m[3][2] = AfxScalar(0);
    m[3][3] = AfxScalar(1);
}

_AFXINL void AfxQuatMakeAxialRotation(afxQuat q, afxV3d const axis, afxReal radians)
{
    afxError err = AFX_ERR_NONE; AfxAssert(q);
    AfxAssert(axis);
    afxReal s = AfxSin(radians * AfxScalar(0.5));
    q[0] = axis[0] * s;
    q[1] = axis[1] * s;
    q[2] = axis[2] * s;
    q[3] = AfxCos(radians * AfxScalar(0.5));
    AfxQuatNormalize(q, q); // reduz erros causados por AfxSin() e AfxCos().
}
