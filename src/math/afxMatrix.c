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

#include "qwadro/math/afxMatrix.h"
#include "qwadro/math/afxScalar.h"
#include "qwadro/math/afxVector.h"
#include "qwadro/coll/afxPlane.h"
#include "qwadro/io/afxStream.h"

// Memory layout: row-major

_AFX afxM2d const AFX_M2D_ZERO =
{
    { AFX_REAL(0), AFX_REAL(0) },
    { AFX_REAL(0), AFX_REAL(0) },
};

_AFX afxM3d const AFX_M3D_ZERO =
{
    { AFX_REAL(0), AFX_REAL(0), AFX_REAL(0) },
    { AFX_REAL(0), AFX_REAL(0), AFX_REAL(0) },
    { AFX_REAL(0), AFX_REAL(0), AFX_REAL(0) }
};

_AFX afxM4d const AFX_M4D_ZERO =
{
    { AFX_REAL(0), AFX_REAL(0), AFX_REAL(0), AFX_REAL(0) },
    { AFX_REAL(0), AFX_REAL(0), AFX_REAL(0), AFX_REAL(0) },
    { AFX_REAL(0), AFX_REAL(0), AFX_REAL(0), AFX_REAL(0) },
    { AFX_REAL(0), AFX_REAL(0), AFX_REAL(0), AFX_REAL(0) }
};

_AFX afxM2d const AFX_M2D_IDENTITY =
{
    { AFX_REAL(1), AFX_REAL(0) },
    { AFX_REAL(0), AFX_REAL(1) },
};

_AFX afxM3d const AFX_M3D_IDENTITY =
{
    { AFX_REAL(1), AFX_REAL(0), AFX_REAL(0) },
    { AFX_REAL(0), AFX_REAL(1), AFX_REAL(0) },
    { AFX_REAL(0), AFX_REAL(0), AFX_REAL(1) }
};

_AFX afxM4d const AFX_M4D_IDENTITY =
{
    { AFX_REAL(1), AFX_REAL(0), AFX_REAL(0), AFX_REAL(0) },
    { AFX_REAL(0), AFX_REAL(1), AFX_REAL(0), AFX_REAL(0) },
    { AFX_REAL(0), AFX_REAL(0), AFX_REAL(1), AFX_REAL(0) },
    { AFX_REAL(0), AFX_REAL(0), AFX_REAL(0), AFX_REAL(1) }
};

// IsIdentity

_AFXINL afxBool AfxM2dIsIdentity(afxM2d const m)
{
    afxBool flag = 1;

    for (afxUnit i = 0; flag && i < 2; i++)
        for (afxUnit j = 0; flag && j < 2; j++)
            if (m[i][j] != 1 && m[j][i] != 0)
                flag = 0;

    return flag;
}

_AFXINL afxBool AfxM3dIsIdentity(afxM3d const m)
{
    afxBool flag = 1;

    for (afxUnit i = 0; flag && i < 3; i++)
        for (afxUnit j = 0; flag && j < 3; j++)
            if (m[i][j] != 1 && m[j][i] != 0)
                flag = 0;

    return flag;
}

_AFXINL afxBool AfxM4dIsIdentity(afxM4d const m)
{
    afxBool flag = 1;

    for (afxUnit i = 0; flag && i < 4; i++)
        for (afxUnit j = 0; flag && j < 4; j++)
            if (m[i][j] != 1 && m[j][i] != 0)
                flag = 0;

    return flag;
}

////////////////////////////////////////////////////////////////////////////////
// Initialization methods                                                     //
////////////////////////////////////////////////////////////////////////////////

// Zero

_AFXINL void AfxM2dZero(afxM2d m)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AfxM2dCopy(m, AFX_M2D_ZERO);
}

_AFXINL void AfxM3dZero(afxM3d m)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AfxM3dCopy(m, AFX_M3D_ZERO);
}

_AFXINL void AfxM4dZero(afxM4d m)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AfxM4dCopy(m, AFX_M4D_ZERO);
}

// MakeIdentity

_AFXINL void AfxM2dReset(afxM2d m)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AfxM2dCopy(m, AFX_M2D_IDENTITY);
}

_AFXINL void AfxM3dReset(afxM3d m)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AfxM3dCopy(m, AFX_M3D_IDENTITY);
}

_AFXINL void AfxM4dReset(afxM4d m)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AfxM4dCopy(m, AFX_M4D_IDENTITY);
}

_AFXINL void AfxM4dEnsureLinear(afxM4d m)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    m[0][3] = 0.f;
    m[1][3] = 0.f;
    m[2][3] = 0.f;
    // in Qwadro, translation is transposed.
    AfxResetV4d(m[3]);
}

_AFXINL void AfxM4dEnsureAffine(afxM4d m)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    m[0][3] = 0.f;
    m[1][3] = 0.f;
    m[2][3] = 0.f;
    m[3][3] = 1.f;
}

// Set

_AFXINL void AfxM2dSet(afxM2d m, afxV2d const cx, afxV2d const cy)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cx);
    AFX_ASSERT(cy);

    m[0][0] = cx[0];
    m[1][0] = cx[1];

    m[0][1] = cy[0];
    m[1][1] = cy[1];
}

_AFXINL void AfxM3dSet(afxM3d m, afxV3d const cx, afxV3d const cy, afxV3d const cz)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cx);
    AFX_ASSERT(cy);
    AFX_ASSERT(cz);

    // compatible with MatrixColumns3x3(m, x, y, z)

    m[0][0] = cx[0];
    m[1][0] = cx[1];
    m[2][0] = cx[2];

    m[0][1] = cy[0];
    m[1][1] = cy[1];
    m[2][1] = cy[2];

    m[0][2] = cz[0];
    m[1][2] = cz[1];
    m[2][2] = cz[2];
}

_AFXINL void AfxM4dSet(afxM4d m, afxV4d const cx, afxV4d const cy, afxV4d const cz, afxV4d const cw)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cx);
    AFX_ASSERT(cy);
    AFX_ASSERT(cz);
    AFX_ASSERT(cw);

    m[0][0] = cx[0];
    m[1][0] = cx[1];
    m[2][0] = cx[2];
    m[3][0] = cx[3];

    m[0][1] = cy[0];
    m[1][1] = cy[1];
    m[2][1] = cy[2];
    m[3][1] = cy[3];

    m[0][2] = cz[0];
    m[1][2] = cz[1];
    m[2][2] = cz[2];
    m[3][2] = cz[3];

    // in Qwadro, translation is transposed.
    AfxV4dCopy(m[3], cw);
}

_AFXINL void AfxM4dSetLinear(afxM4d m, afxV3d const cx, afxV3d const cy, afxV3d const cz, afxV3d const cw)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cx);
    AFX_ASSERT(cy);
    AFX_ASSERT(cz);
    AFX_ASSERT(cw);

    m[0][0] = cx[0];
    m[1][0] = cx[1];
    m[2][0] = cx[2];
    m[3][0] = 0.0;

    m[0][1] = cy[0];
    m[1][1] = cy[1];
    m[2][1] = cy[2];
    m[3][1] = 0.0;

    m[0][2] = cz[0];
    m[1][2] = cz[1];
    m[2][2] = cz[2];
    m[3][2] = 0.0;

    // in Qwadro, translation is transposed.
    AfxResetV4d(m[3]);
}

_AFXINL void AfxM4dSetAffine(afxM4d m, afxV3d const cx, afxV3d const cy, afxV3d const cz, afxV3d const cw)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cx);
    AFX_ASSERT(cy);
    AFX_ASSERT(cz);
    AFX_ASSERT(cw);

    m[0][0] = cx[0];
    m[1][0] = cx[1];
    m[2][0] = cx[2];
    m[3][0] = 0.0;

    m[0][1] = cy[0];
    m[1][1] = cy[1];
    m[2][1] = cy[2];
    m[3][1] = 0.0;

    m[0][2] = cz[0];
    m[1][2] = cz[1];
    m[2][2] = cz[2];
    m[3][2] = 0.0;

    // in Qwadro, translation is transposed.
    AfxV4dCopyAtv3d(m[3], cw);
}

// SetTransposed

_AFXINL void AfxM2dSetTransposed(afxM2d m, afxV2d const rx, afxV2d const ry)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(rx);
    AFX_ASSERT(ry);
    AfxV2dCopy(m[0], rx);
    AfxV2dCopy(m[1], ry);
}

_AFXINL void AfxM3dSetTransposed(afxM3d m, afxV3d const rx, afxV3d const ry, afxV3d const rz)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(rx);
    AFX_ASSERT(ry);
    AFX_ASSERT(rz);
    AfxV3dCopy(m[0], rx);
    AfxV3dCopy(m[1], ry);
    AfxV3dCopy(m[2], rz);
}

_AFXINL void AfxM4dSetTransposed(afxM4d m, afxV4d const rx, afxV4d const ry, afxV4d const rz, afxV4d const rw)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(rx);
    AFX_ASSERT(ry);
    AFX_ASSERT(rz);
    AFX_ASSERT(rw);
    AfxV4dCopy(m[0], rx);
    AfxV4dCopy(m[1], ry);
    AfxV4dCopy(m[2], rz);
    AfxV4dCopy(m[3], rw);
}

_AFXINL void AfxM4dSetTransposedLinear(afxM4d m, afxV3d const rx, afxV3d const ry, afxV3d const rz, afxV3d const rw)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(rx);
    AFX_ASSERT(ry);
    AFX_ASSERT(rz);
    AFX_ASSERT(rw);

    AfxV4dCopyV3d(m[0], rx);
    AfxV4dCopyV3d(m[0], ry);
    AfxV4dCopyV3d(m[0], rz);
    AfxV4dCopyV3d(m[0], rw);
}

_AFXINL void AfxM4dSetTransposedAffine(afxM4d m, afxV3d const rx, afxV3d const ry, afxV3d const rz, afxV3d const rw)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(rx);
    AFX_ASSERT(ry);
    AFX_ASSERT(rz);
    AFX_ASSERT(rw);

    AfxV4dCopyAtv3d(m[0], rx);
    AfxV4dCopyAtv3d(m[0], ry);
    AfxV4dCopyAtv3d(m[0], rz);
    AfxV4dCopyAtv3d(m[0], rw);
}

// SetDiagonal

_AFXINL void AfxM2dSetDiagonal(afxM2d m, afxReal xx, afxReal yy)
{
    afxError err = { 0 };
    AFX_ASSERT(m);

    m[0][0] = xx;
    m[1][0] = 0.f;

    m[0][1] = 0.f;
    m[1][1] = yy;
}

_AFXINL void AfxM3dSetDiagonal(afxM3d m, afxReal xx, afxReal yy, afxReal zz)
{
    afxError err = { 0 };
    AFX_ASSERT(m);

    // compatible with MatrixColumns3x3(m, x, y, z)

    m[0][0] = xx;
    m[1][0] = 0.f;
    m[2][0] = 0.f;

    m[0][1] = 0.f;
    m[1][1] = yy;
    m[2][1] = 0.f;

    m[0][2] = 0.f;
    m[1][2] = 0.f;
    m[2][2] = zz;
}

_AFXINL void AfxM4dSetDiagonal(afxM4d m, afxReal xx, afxReal yy, afxReal zz, afxReal ww)
{
    afxError err = { 0 };
    AFX_ASSERT(m);

    m[0][0] = xx;
    m[1][0] = 0.f;
    m[2][0] = 0.f;
    m[3][0] = 0.f;

    m[0][1] = 0.f;
    m[1][1] = yy;
    m[2][1] = 0.f;
    m[3][1] = 0.f;

    m[0][2] = 0.f;
    m[1][2] = 0.f;
    m[2][2] = zz;
    m[3][2] = 0.f;

    // in Qwadro, translation is transposed.
    m[0][3] = 0.f;
    m[1][3] = 0.f;
    m[2][3] = 0.f;
    m[3][3] = ww;
}

////////////////////////////////////////////////////////////////////////////////
// Transferance methods                                                       //
////////////////////////////////////////////////////////////////////////////////

// Copy

_AFXINL void AfxM2dCopy(afxM2d m, afxM2d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(m);
    AFX_ASSERT_DIFF(m, in);

    m[0][0] = in[0][0];
    m[0][1] = in[0][1];

    m[1][0] = in[1][0];
    m[1][1] = in[1][1];
}

_AFXINL void AfxM3dCopy(afxM3d m, afxM3d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(m);
    AFX_ASSERT_DIFF(m, in);

    m[0][0] = in[0][0];
    m[0][1] = in[0][1];
    m[0][2] = in[0][2];

    m[1][0] = in[1][0];
    m[1][1] = in[1][1];
    m[1][2] = in[1][2];

    m[2][0] = in[2][0];
    m[2][1] = in[2][1];
    m[2][2] = in[2][2];
}

_AFXINL void AfxM4dCopy(afxM4d m, afxM4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(m);
    AFX_ASSERT_DIFF(m, in);

    m[0][0] = in[0][0];
    m[0][1] = in[0][1];
    m[0][2] = in[0][2];
    m[0][3] = in[0][3];

    m[1][0] = in[1][0];
    m[1][1] = in[1][1];
    m[1][2] = in[1][2];
    m[1][3] = in[1][3];

    m[2][0] = in[2][0];
    m[2][1] = in[2][1];
    m[2][2] = in[2][2];
    m[2][3] = in[2][3];

    m[3][0] = in[3][0];
    m[3][1] = in[3][1];
    m[3][2] = in[3][2];
    m[3][3] = in[3][3];
}

_AFXINL void AfxM4dCopyLtm(afxM4d m, afxM4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT2(in, m);

    m[0][0] = in[0][0];
    m[0][1] = in[0][1];
    m[0][2] = in[0][2];
    m[0][3] = 0.f;

    m[1][0] = in[1][0];
    m[1][1] = in[1][1];
    m[1][2] = in[1][2];
    m[1][3] = 0.f;

    m[2][0] = in[2][0];
    m[2][1] = in[2][1];
    m[2][2] = in[2][2];
    m[2][3] = 0.f;

    m[3][0] = 0.f;
    m[3][1] = 0.f;
    m[3][2] = 0.f;
    m[3][3] = 1.f;
}

_AFXINL void AfxM4dCopyAtm(afxM4d m, afxM4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT2(in, m);

    m[0][0] = in[0][0];
    m[0][1] = in[0][1];
    m[0][2] = in[0][2];
    m[0][3] = 0.f;

    m[1][0] = in[1][0];
    m[1][1] = in[1][1];
    m[1][2] = in[1][2];
    m[1][3] = 0.f;

    m[2][0] = in[2][0];
    m[2][1] = in[2][1];
    m[2][2] = in[2][2];
    m[2][3] = 0.f;

    m[3][0] = in[3][0];
    m[3][1] = in[3][1];
    m[3][2] = in[3][2];
    m[3][3] = 1.f;
}

_AFXINL void AfxM4dTransposeLtm(afxM4d m, afxM4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT3(in, m, m != in);

    m[0][0] = in[0][0];
    m[0][1] = in[1][0];
    m[0][2] = in[2][0];
    m[0][3] = 0.f;

    m[1][0] = in[0][1];
    m[1][1] = in[1][1];
    m[1][2] = in[2][1];
    m[1][3] = 0.f;

    m[2][0] = in[0][2];
    m[2][1] = in[1][2];
    m[2][2] = in[2][2];
    m[2][3] = 0.f;

    m[3][0] = 0.f;
    m[3][1] = 0.f;
    m[3][2] = 0.f;
    m[3][3] = 1.f;
}

_AFXINL void AfxM4dTransposeAtm(afxM4d m, afxM4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT3(in, m, m != in);

    m[0][0] = in[0][0];
    m[0][1] = in[1][0];
    m[0][2] = in[2][0];
    m[0][3] = 0.f;

    m[1][0] = in[0][1];
    m[1][1] = in[1][1];
    m[1][2] = in[2][1];
    m[1][3] = 0.f;

    m[2][0] = in[0][2];
    m[2][1] = in[1][2];
    m[2][2] = in[2][2];
    m[2][3] = 0.f;

    m[3][0] = in[0][3];
    m[3][1] = in[1][3];
    m[3][2] = in[2][3];
    m[3][3] = 1.f;
}

//

_AFXINL void AfxM2dCopyM3d(afxM2d m, afxM3d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(in);
    AFX_ASSERT_DIFF(in, m);

    m[0][0] = in[0][0];
    m[0][1] = in[0][1];

    m[1][0] = in[1][0];
    m[1][1] = in[1][1];
}

_AFXINL void AfxM3dCopyM4d(afxM3d m, afxM4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT3(m, in, (void*)m != (void*)in);

    m[0][0] = in[0][0];
    m[0][1] = in[0][1];
    m[0][2] = in[0][2];

    m[1][0] = in[1][0];
    m[1][1] = in[1][1];
    m[1][2] = in[1][2];

    m[2][0] = in[2][0];
    m[2][1] = in[2][1];
    m[2][2] = in[2][2];
}

_AFXINL void AfxM3dTransposeM4d(afxM3d m, afxM4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT3(m, in, (void*)m != (void*)in);

    m[0][0] = in[0][0];
    m[0][1] = in[1][0];
    m[0][2] = in[2][0];

    m[1][0] = in[0][1];
    m[1][1] = in[1][1];
    m[1][2] = in[2][1];

    m[2][0] = in[0][2];
    m[2][1] = in[1][2];
    m[2][2] = in[2][2];
}

_AFXINL void AfxM3dCopyM2d(afxM3d m, afxM2d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(in);
    AFX_ASSERT_DIFF(m, in);

    m[0][0] = in[0][0];
    m[0][1] = in[0][1];
    m[0][2] = AFX_REAL(0);

    m[1][0] = in[1][0];
    m[1][1] = in[1][1];
    m[1][2] = AFX_REAL(0);

    m[2][0] = AFX_REAL(0);
    m[2][1] = AFX_REAL(0);
    m[2][2] = AFX_REAL(1);
}

_AFXINL void AfxM4dCopyM2d(afxM4d m, afxM2d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(in);
    AFX_ASSERT_DIFF(m, in);

    m[0][0] = in[0][0];
    m[0][1] = in[0][1];
    m[0][2] = AFX_REAL(0);
    m[0][3] = AFX_REAL(0);

    m[1][0] = in[1][0];
    m[1][1] = in[1][1];
    m[1][2] = AFX_REAL(0);
    m[1][3] = AFX_REAL(0);

    m[2][0] = AFX_REAL(0);
    m[2][1] = AFX_REAL(0);
    m[2][2] = AFX_REAL(1);
    m[2][3] = AFX_REAL(0);

    m[3][0] = AFX_REAL(0);
    m[3][1] = AFX_REAL(0);
    m[3][2] = AFX_REAL(0);
    m[3][3] = AFX_REAL(1);
}

_AFXINL void AfxM4dCopyM3d(afxM4d m, afxM3d const ltm, afxV4d const atv)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(ltm);
    AFX_ASSERT(atv);
    AFX_ASSERT_DIFF(m, ltm);

    m[0][0] = ltm[0][0];
    m[0][1] = ltm[0][1];
    m[0][2] = ltm[0][2];
    m[0][3] = AFX_REAL(0);

    m[1][0] = ltm[1][0];
    m[1][1] = ltm[1][1];
    m[1][2] = ltm[1][2];
    m[1][3] = AFX_REAL(0);

    m[2][0] = ltm[2][0];
    m[2][1] = ltm[2][1];
    m[2][2] = ltm[2][2];
    m[2][3] = AFX_REAL(0);

    m[3][0] = atv[0];
    m[3][1] = atv[1];
    m[3][2] = atv[2];
    m[3][3] = 1.f;
}

_AFXINL void AfxM4rCopy(afxM4r m, afxM4r const in)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(in);
    AFX_ASSERT_DIFF(m, in);

    m[0][0] = in[0][0];
    m[0][1] = in[0][1];
    m[0][2] = in[0][2];

    m[1][0] = in[1][0];
    m[1][1] = in[1][1];
    m[1][2] = in[1][2];

    m[2][0] = in[2][0];
    m[2][1] = in[2][1];
    m[2][2] = in[2][2];

    m[3][0] = in[3][0];
    m[3][1] = in[3][1];
    m[3][2] = in[3][2];
}

_AFXINL void AfxM4rCopyM3d(afxM4r m, afxM3d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(in);
    AFX_ASSERT_DIFF(m, in);

    m[0][0] = in[0][0];
    m[0][1] = in[0][1];
    m[0][2] = in[0][2];

    m[1][0] = in[1][0];
    m[1][1] = in[1][1];
    m[1][2] = in[1][2];

    m[2][0] = in[2][0];
    m[2][1] = in[2][1];
    m[2][2] = in[2][2];

    m[3][0] = 0.f;
    m[3][1] = 0.f;
    m[3][2] = 0.f;
}

_AFXINL void AfxM4rCopyM4d(afxM4r m, afxM4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(in);
    AFX_ASSERT_DIFF(m, in);

    m[0][0] = in[0][0];
    m[0][1] = in[0][1];
    m[0][2] = in[0][2];

    m[1][0] = in[1][0];
    m[1][1] = in[1][1];
    m[1][2] = in[1][2];

    m[2][0] = in[2][0];
    m[2][1] = in[2][1];
    m[2][2] = in[2][2];

    m[3][0] = in[3][0];
    m[3][1] = in[3][1];
    m[3][2] = in[3][2];
}

_AFXINL void AfxM4dTransposeM3d(afxM4d m, afxM3d const ltm, afxV4d const atv)
{
    afxError err = { 0 };
    AFX_ASSERT(ltm);
    AFX_ASSERT(m);
    AFX_ASSERT(atv);

    m[0][0] = ltm[0][0];
    m[0][1] = ltm[1][0];
    m[0][2] = ltm[2][0];
    m[0][3] = 0.f;

    m[1][0] = ltm[0][1];
    m[1][1] = ltm[1][1];
    m[1][2] = ltm[2][1];
    m[1][3] = 0.f;

    m[2][0] = ltm[0][2];
    m[2][1] = ltm[1][2];
    m[2][2] = ltm[2][2];
    m[2][3] = 0.f;

    m[3][0] = atv[0];
    m[3][1] = atv[1];
    m[3][2] = atv[2];
    m[3][3] = 1.f;
}

// Swap

_AFXINL void AfxM2dSwap(afxM2d m, afxM2d other)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(other);
    AFX_ASSERT(m != other);
    afxM2d tmp;
    AfxM2dCopy(tmp, m);
    AfxM2dCopy(other, tmp);
    AfxM2dCopy(m, other);
}

_AFXINL void AfxM3dSwap(afxM3d m, afxM3d other)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(other);
    AFX_ASSERT(m != other);
    afxM3d tmp;
    AfxM3dCopy(tmp, m);
    AfxM3dCopy(other, tmp);
    AfxM3dCopy(m, other);
}

_AFXINL void AfxM4dSwap(afxM4d m, afxM4d other)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(other);
    AFX_ASSERT(m != other);
    afxM4d tmp;
    AfxM4dCopy(tmp, m);
    AfxM4dCopy(other, tmp);
    AfxM4dCopy(m, other);
}

// Transpose

_AFXINL void AfxM2dTranspose(afxM2d m, afxM2d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(m);
    AFX_ASSERT(m != in);

    m[0][0] = in[0][0];
    m[0][1] = in[1][0];

    m[1][0] = in[0][1];
    m[1][1] = in[1][1];
}

_AFXINL void AfxM3dTranspose(afxM3d m, afxM3d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(m);
    AFX_ASSERT(m != in);

    m[0][0] = in[0][0];
    m[0][1] = in[1][0];
    m[0][2] = in[2][0];

    m[1][0] = in[0][1];
    m[1][1] = in[1][1];
    m[1][2] = in[2][1];

    m[2][0] = in[0][2];
    m[2][1] = in[1][2];
    m[2][2] = in[2][2];
}

_AFXINL void AfxM4dTranspose(afxM4d m, afxM4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(m);
    AFX_ASSERT(m != in);

    m[0][0] = in[0][0];
    m[0][1] = in[1][0];
    m[0][2] = in[2][0];
    m[0][3] = in[3][0];

    m[1][0] = in[0][1];
    m[1][1] = in[1][1];
    m[1][2] = in[2][1];
    m[1][3] = in[3][1];

    m[2][0] = in[0][2];
    m[2][1] = in[1][2];
    m[2][2] = in[2][2];
    m[2][3] = in[3][2];

    m[3][0] = in[0][3];
    m[3][1] = in[1][3];
    m[3][2] = in[2][3];
    m[3][3] = in[3][3];
}

////////////////////////////////////////////////////////////////////////////////
// Arithmetic methods                                                         //
////////////////////////////////////////////////////////////////////////////////

// Add

_AFXINL void AfxM2dAdd(afxM2d m, afxM2d const a, afxM2d const b)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    AfxV2dAdd(m[0], a[0], b[0]);
    AfxV2dAdd(m[1], a[1], b[1]);
}

_AFXINL void AfxM3dAdd(afxM3d m, afxM3d const a, afxM3d const b)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    AfxV3dAdd(m[0], a[0], b[0]);
    AfxV3dAdd(m[1], a[1], b[1]);
    AfxV3dAdd(m[2], a[2], b[2]);
}

_AFXINL void AfxM4dAdd(afxM4d m, afxM4d const a, afxM4d const b)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    AfxV4dAdd(m[0], a[0], b[0]);
    AfxV4dAdd(m[1], a[1], b[1]);
    AfxV4dAdd(m[2], a[2], b[2]);
    AfxV4dAdd(m[3], a[3], b[3]);
}

// Sub

_AFXINL void AfxM2dSub(afxM2d m, afxM2d const a, afxM2d const b)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    AfxV2dSub(m[0], a[0], b[0]);
    AfxV2dSub(m[1], a[1], b[1]);
}

_AFXINL void AfxM3dSub(afxM3d m, afxM3d const a, afxM3d const b)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    AfxV3dSub(m[0], a[0], b[0]);
    AfxV3dSub(m[1], a[1], b[1]);
    AfxV3dSub(m[2], a[2], b[2]);
}

_AFXINL void AfxM4dSub(afxM4d m, afxM4d const a, afxM4d const b)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    AfxV4dSub(m[0], a[0], b[0]);
    AfxV4dSub(m[1], a[1], b[1]);
    AfxV4dSub(m[2], a[2], b[2]);
    AfxV4dSub(m[3], a[3], b[3]);
}

// Diff

_AFXINL void AfxM2dDiff(afxM2d m, afxM2d const a, afxM2d const b)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    AfxV2dSub(m[0], a[0], b[0]);
    AfxV2dSub(m[1], a[1], b[1]);
}

_AFXINL void AfxM3dDiff(afxM3d m, afxM3d const a, afxM3d const b)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    AfxV3dSub(m[0], a[0], b[0]);
    AfxV3dSub(m[1], a[1], b[1]);
    AfxV3dSub(m[2], a[2], b[2]);
}

_AFXINL void AfxM4dDiff(afxM4d m, afxM4d const a, afxM4d const b)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    AfxV4dSub(m[0], a[0], b[0]);
    AfxV4dSub(m[1], a[1], b[1]);
    AfxV4dSub(m[2], a[2], b[2]);
    AfxV4dSub(m[3], a[3], b[3]);
}

// Scale

_AFXINL void AfxM2dScale(afxM2d m, afxM2d const in, afxReal scale)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(in);
    AfxV2dScale(m[0], in[0], scale);
    AfxV2dScale(m[1], in[1], scale);
}

_AFXINL void AfxM3dScale(afxM3d m, afxM3d const in, afxReal scale)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(in);
    AfxV3dScale(m[0], in[0], scale);
    AfxV3dScale(m[1], in[1], scale);
    AfxV3dScale(m[2], in[2], scale);
}

_AFXINL void AfxM4dScale(afxM4d m, afxM4d const in, afxReal scale)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(in);
    AfxV4dScale(m[0], in[0], scale);
    AfxV4dScale(m[1], in[1], scale);
    AfxV4dScale(m[2], in[2], scale);
    AfxV4dScale(m[3], in[3], scale);
}

// AddScaled

_AFXINL void AfxM2dMads(afxM2d m, afxM2d const add, afxM2d const mul, afxReal scale)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(add);
    AFX_ASSERT(mul);
    AfxV2dMads(m[0], add[0], mul[0], scale);
    AfxV2dMads(m[1], add[1], mul[1], scale);
}

_AFXINL void AfxM3dMads(afxM3d m, afxM3d const add, afxM3d const mul, afxReal scale)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(add);
    AFX_ASSERT(mul);
    AfxV3dMads(m[0], add[0], mul[0], scale);
    AfxV3dMads(m[1], add[1], mul[1], scale);
    AfxV3dMads(m[2], add[2], mul[2], scale);
}

_AFXINL void AfxM4dMads(afxM4d m, afxM4d const add, afxM4d const mul, afxReal scale)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(add);
    AFX_ASSERT(mul);
    AfxV4dMads(m[0], add[0], mul[0], scale);
    AfxV4dMads(m[1], add[1], mul[1], scale);
    AfxV4dMads(m[2], add[2], mul[2], scale);
    AfxV4dMads(m[3], add[3], mul[3], scale);
}

// Mix
// The MIX function is conceptually similar to LERP but may involve additional functionality, such as more complex blending modes or different blending parameters. 
// It is often used in shader programming and graphics.
// v = x * (1 - t) + y * t

_AFXINL void AfxM2dMix(afxM2d m, afxM2d const a, afxM2d const b, afxReal t)
{
    afxError err = { 0 };
    AFX_ASSERT3(m, a, b);

    m[0][0] = a[0][0] * (1.f - t) + b[0][0] * t;
    m[0][1] = a[0][1] * (1.f - t) + b[0][1] * t;

    m[1][0] = a[1][0] * (1.f - t) + b[1][0] * t;
    m[1][1] = a[1][1] * (1.f - t) + b[1][1] * t;

    m[2][0] = a[2][0] * (1.f - t) + b[2][0] * t;
    m[2][1] = a[2][1] * (1.f - t) + b[2][1] * t;

    m[3][0] = a[3][0] * (1.f - t) + b[3][0] * t;
    m[3][1] = a[3][1] * (1.f - t) + b[3][1] * t;
}

_AFXINL void AfxM3dMix(afxM3d m, afxM3d const a, afxM3d const b, afxReal t)
{
    afxError err = { 0 };
    AFX_ASSERT3(m, a, b);

    m[0][0] = a[0][0] * (1.f - t) + b[0][0] * t;
    m[0][1] = a[0][1] * (1.f - t) + b[0][1] * t;
    m[0][2] = a[0][2] * (1.f - t) + b[0][2] * t;

    m[1][0] = a[1][0] * (1.f - t) + b[1][0] * t;
    m[1][1] = a[1][1] * (1.f - t) + b[1][1] * t;
    m[1][2] = a[1][2] * (1.f - t) + b[1][2] * t;

    m[2][0] = a[2][0] * (1.f - t) + b[2][0] * t;
    m[2][1] = a[2][1] * (1.f - t) + b[2][1] * t;
    m[2][2] = a[2][2] * (1.f - t) + b[2][2] * t;

    m[3][0] = a[3][0] * (1.f - t) + b[3][0] * t;
    m[3][1] = a[3][1] * (1.f - t) + b[3][1] * t;
    m[3][2] = a[3][2] * (1.f - t) + b[3][2] * t;
}

_AFXINL void AfxM4dMix(afxM4d m, afxM4d const a, afxM4d const b, afxReal t)
{
    afxError err = { 0 };
    AFX_ASSERT3(m, a, b);

    m[0][0] = a[0][0] * (1.f - t) + b[0][0] * t;
    m[0][1] = a[0][1] * (1.f - t) + b[0][1] * t;
    m[0][2] = a[0][2] * (1.f - t) + b[0][2] * t;
    m[0][3] = a[0][3] * (1.f - t) + b[0][3] * t;

    m[1][0] = a[1][0] * (1.f - t) + b[1][0] * t;
    m[1][1] = a[1][1] * (1.f - t) + b[1][1] * t;
    m[1][2] = a[1][2] * (1.f - t) + b[1][2] * t;
    m[1][3] = a[1][3] * (1.f - t) + b[1][3] * t;

    m[2][0] = a[2][0] * (1.f - t) + b[2][0] * t;
    m[2][1] = a[2][1] * (1.f - t) + b[2][1] * t;
    m[2][2] = a[2][2] * (1.f - t) + b[2][2] * t;
    m[2][3] = a[2][3] * (1.f - t) + b[2][3] * t;

    m[3][0] = a[3][0] * (1.f - t) + b[3][0] * t;
    m[3][1] = a[3][1] * (1.f - t) + b[3][1] * t;
    m[3][2] = a[3][2] * (1.f - t) + b[3][2] * t;
    m[3][3] = a[3][3] * (1.f - t) + b[3][3] * t;
}

_AFXINL void AfxM3dMixAffine(afxM3d m, afxM3d const a, afxM3d const b, afxReal t)
{
    afxError err = { 0 };
    AFX_ASSERT3(m, a, b);

    m[0][0] = a[0][0] * (1.f - t) + b[0][0] * t;
    m[0][1] = a[0][1] * (1.f - t) + b[0][1] * t;
    m[0][2] = 0.f;

    m[1][0] = a[1][0] * (1.f - t) + b[1][0] * t;
    m[1][1] = a[1][1] * (1.f - t) + b[1][1] * t;
    m[1][2] = 0.f;

    m[2][0] = a[2][0] * (1.f - t) + b[2][0] * t;
    m[2][1] = a[2][1] * (1.f - t) + b[2][1] * t;
    m[2][2] = 1.f;
}

_AFXINL void AfxM4dMixAffine(afxM4d m, afxM4d const a, afxM4d const b, afxReal t)
{
    afxError err = { 0 };
    AFX_ASSERT3(m, a, b);

    m[0][0] = a[0][0] * (1.f - t) + b[0][0] * t;
    m[0][1] = a[0][1] * (1.f - t) + b[0][1] * t;
    m[0][2] = a[0][2] * (1.f - t) + b[0][2] * t;
    m[0][3] = 0.f;

    m[1][0] = a[1][0] * (1.f - t) + b[1][0] * t;
    m[1][1] = a[1][1] * (1.f - t) + b[1][1] * t;
    m[1][2] = a[1][2] * (1.f - t) + b[1][2] * t;
    m[1][3] = 0.f;

    m[2][0] = a[2][0] * (1.f - t) + b[2][0] * t;
    m[2][1] = a[2][1] * (1.f - t) + b[2][1] * t;
    m[2][2] = a[2][2] * (1.f - t) + b[2][2] * t;
    m[2][3] = 0.f;

    m[3][0] = a[3][0] * (1.f - t) + b[3][0] * t;
    m[3][1] = a[3][1] * (1.f - t) + b[3][1] * t;
    m[3][2] = a[3][2] * (1.f - t) + b[3][2] * t;
    m[3][3] = 1.f;
}

_AFXINL void AfxM4dMixLinear(afxM4d m, afxM4d const a, afxM4d const b, afxReal t)
{
    afxError err = { 0 };
    AFX_ASSERT3(m, a, b);

    m[0][0] = a[0][0] * (1.f - t) + b[0][0] * t;
    m[0][1] = a[0][1] * (1.f - t) + b[0][1] * t;
    m[0][2] = a[0][2] * (1.f - t) + b[0][2] * t;
    m[0][3] = 0.f;

    m[1][0] = a[1][0] * (1.f - t) + b[1][0] * t;
    m[1][1] = a[1][1] * (1.f - t) + b[1][1] * t;
    m[1][2] = a[1][2] * (1.f - t) + b[1][2] * t;
    m[1][3] = 0.f;

    m[2][0] = a[2][0] * (1.f - t) + b[2][0] * t;
    m[2][1] = a[2][1] * (1.f - t) + b[2][1] * t;
    m[2][2] = a[2][2] * (1.f - t) + b[2][2] * t;
    m[2][3] = 0.f;

    m[3][0] = 0.f;
    m[3][1] = 0.f;
    m[3][2] = 0.f;
    m[3][3] = 1.f;
}

// Lerp
// LERP is a method to interpolate linearly between two values. 
// In the context of 4D vectors, it calculates the intermediate vector between a start and end vector based on a factor t ranging from 0.0 to 1.0.
// x + t * (y - x)

_AFXINL void AfxM2dLerp(afxM2d m, afxM2d const a, afxM2d const b, afxReal t)
{
    afxError err = { 0 };
    AFX_ASSERT3(m, a, b);

    m[0][0] = a[0][0] + t * (b[0][0] - a[0][0]);
    m[0][1] = a[0][1] + t * (b[0][1] - a[0][1]);

    m[1][0] = a[1][0] + t * (b[1][0] - a[1][0]);
    m[1][1] = a[1][1] + t * (b[1][1] - a[1][1]);

    m[2][0] = a[2][0] + t * (b[2][0] - a[2][0]);
    m[2][1] = a[2][1] + t * (b[2][1] - a[2][1]);

    m[3][0] = a[3][0] + t * (b[3][0] - a[3][0]);
    m[3][1] = a[3][1] + t * (b[3][1] - a[3][1]);
}

_AFXINL void AfxM3dLerp(afxM3d m, afxM3d const a, afxM3d const b, afxReal t)
{
    afxError err = { 0 };
    AFX_ASSERT3(m, a, b);

    m[0][0] = a[0][0] + t * (b[0][0] - a[0][0]);
    m[0][1] = a[0][1] + t * (b[0][1] - a[0][1]);
    m[0][2] = a[0][2] + t * (b[0][2] - a[0][2]);

    m[1][0] = a[1][0] + t * (b[1][0] - a[1][0]);
    m[1][1] = a[1][1] + t * (b[1][1] - a[1][1]);
    m[1][2] = a[1][2] + t * (b[1][2] - a[1][2]);

    m[2][0] = a[2][0] + t * (b[2][0] - a[2][0]);
    m[2][1] = a[2][1] + t * (b[2][1] - a[2][1]);
    m[2][2] = a[2][2] + t * (b[2][2] - a[2][2]);

    m[3][0] = a[3][0] + t * (b[3][0] - a[3][0]);
    m[3][1] = a[3][1] + t * (b[3][1] - a[3][1]);
    m[3][2] = a[3][2] + t * (b[3][2] - a[3][2]);
}

_AFXINL void AfxM4dLerp(afxM4d m, afxM4d const a, afxM4d const b, afxReal t)
{
    afxError err = { 0 };
    AFX_ASSERT3(m, a, b);

    m[0][0] = a[0][0] + t * (b[0][0] - a[0][0]);
    m[0][1] = a[0][1] + t * (b[0][1] - a[0][1]);
    m[0][2] = a[0][2] + t * (b[0][2] - a[0][2]);
    m[0][3] = a[0][3] + t * (b[0][3] - a[0][3]);

    m[1][0] = a[1][0] + t * (b[1][0] - a[1][0]);
    m[1][1] = a[1][1] + t * (b[1][1] - a[1][1]);
    m[1][2] = a[1][2] + t * (b[1][2] - a[1][2]);
    m[1][3] = a[1][3] + t * (b[1][3] - a[1][3]);

    m[2][0] = a[2][0] + t * (b[2][0] - a[2][0]);
    m[2][1] = a[2][1] + t * (b[2][1] - a[2][1]);
    m[2][2] = a[2][2] + t * (b[2][2] - a[2][2]);
    m[2][3] = a[2][3] + t * (b[2][3] - a[2][3]);

    m[3][0] = a[3][0] + t * (b[3][0] - a[3][0]);
    m[3][1] = a[3][1] + t * (b[3][1] - a[3][1]);
    m[3][2] = a[3][2] + t * (b[3][2] - a[3][2]);
    m[3][3] = a[3][3] + t * (b[3][3] - a[3][3]);
}

_AFXINL void AfxM3dLerpAffine(afxM3d m, afxM3d const a, afxM3d const b, afxReal t)
{
    afxError err = { 0 };
    AFX_ASSERT3(m, a, b);

    m[0][0] = a[0][0] + t * (b[0][0] - a[0][0]);
    m[0][1] = a[0][1] + t * (b[0][1] - a[0][1]);
    m[0][2] = 0.f;

    m[1][0] = a[1][0] + t * (b[1][0] - a[1][0]);
    m[1][1] = a[1][1] + t * (b[1][1] - a[1][1]);
    m[1][2] = 0.f;

    m[2][0] = a[2][0] + t * (b[2][0] - a[2][0]);
    m[2][1] = a[2][1] + t * (b[2][1] - a[2][1]);
    m[2][2] = 1.f;
}

_AFXINL void AfxM4dLerpAffine(afxM4d m, afxM4d const a, afxM4d const b, afxReal t)
{
    afxError err = { 0 };
    AFX_ASSERT3(m, a, b);

    m[0][0] = a[0][0] + t * (b[0][0] - a[0][0]);
    m[0][1] = a[0][1] + t * (b[0][1] - a[0][1]);
    m[0][2] = a[0][2] + t * (b[0][2] - a[0][2]);
    m[0][3] = 0.f;

    m[1][0] = a[1][0] + t * (b[1][0] - a[1][0]);
    m[1][1] = a[1][1] + t * (b[1][1] - a[1][1]);
    m[1][2] = a[1][2] + t * (b[1][2] - a[1][2]);
    m[1][3] = 0.f;

    m[2][0] = a[2][0] + t * (b[2][0] - a[2][0]);
    m[2][1] = a[2][1] + t * (b[2][1] - a[2][1]);
    m[2][2] = a[2][2] + t * (b[2][2] - a[2][2]);
    m[2][3] = 0.f;

    m[3][0] = a[3][0] + t * (b[3][0] - a[3][0]);
    m[3][1] = a[3][1] + t * (b[3][1] - a[3][1]);
    m[3][2] = a[3][2] + t * (b[3][2] - a[3][2]);
    m[3][3] = 1.f;
}

_AFXINL void AfxM4dLerpLinear(afxM4d m, afxM4d const a, afxM4d const b, afxReal t)
{
    afxError err = { 0 };
    AFX_ASSERT3(m, a, b);

    m[0][0] = a[0][0] + t * (b[0][0] - a[0][0]);
    m[0][1] = a[0][1] + t * (b[0][1] - a[0][1]);
    m[0][2] = a[0][2] + t * (b[0][2] - a[0][2]);
    m[0][3] = 0.f;

    m[1][0] = a[1][0] + t * (b[1][0] - a[1][0]);
    m[1][1] = a[1][1] + t * (b[1][1] - a[1][1]);
    m[1][2] = a[1][2] + t * (b[1][2] - a[1][2]);
    m[1][3] = 0.f;

    m[2][0] = a[2][0] + t * (b[2][0] - a[2][0]);
    m[2][1] = a[2][1] + t * (b[2][1] - a[2][1]);
    m[2][2] = a[2][2] + t * (b[2][2] - a[2][2]);
    m[2][3] = 0.f;

    m[3][0] = 0.f;
    m[3][1] = 0.f;
    m[3][2] = 0.f;
    m[3][3] = 1.f;
}

////////////////////////////////////////////////////////////////////////////////
// Algebra                                                                    //
////////////////////////////////////////////////////////////////////////////////

// Negate

_AFXINL afxReal AfxM3dInvert(afxM3d m, afxM3d const in)
{
    // Inspired on void MatrixInvert3x3(float *DestInit, const float *SourceInit)
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(m);
    AFX_ASSERT(in != m);

    afxReal det = (in[1][1] * in[2][2] - in[2][1] * in[1][2]) * in[0][0] - (in[2][2] * in[1][0] - in[2][0] * in[1][2]) * in[0][1] + (in[2][1] * in[1][0] - in[2][0] * in[1][1]) * in[0][2];

    if (det != 0.0)
    {
        det = 1.0 / det;
        m[0][0] =  ( in[1][1] * in[2][2] - in[2][1] * in[1][2]) * det;
        m[1][0] = -((in[2][2] * in[1][0] - in[2][0] * in[1][2]) * det);
        m[2][0] =  ( in[2][1] * in[1][0] - in[2][0] * in[1][1]) * det;

        m[0][1] = -((in[0][1] * in[2][2] - in[2][1] * in[0][2]) * det);
        m[1][1] =  ( in[2][2] * in[0][0] - in[2][0] * in[0][2]) * det;
        m[2][1] = -((in[2][1] * in[0][0] - in[2][0] * in[0][1]) * det);

        m[0][2] =  ( in[0][1] * in[1][2] - in[1][1] * in[0][2]) * det;
        m[1][2] = -((in[1][2] * in[0][0] - in[0][2] * in[1][0]) * det);
        m[2][2] =  ( in[1][1] * in[0][0] - in[0][1] * in[1][0]) * det;
    }
    else
    {
        AfxM3dCopy(m, in);
        AFX_ASSERT(det != 0.0);
    }
    return det;
}

// Invert
// Memory layout: hybrid

_AFXINL afxReal AfxM4dInvert(afxM4d m, afxM4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(m);
    AFX_ASSERT(in != m);

    // From StackOverflow, by wangzhe, at https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix

    m[0][0] =  in[1][1] * in[2][2] * in[3][3] - in[1][1] * in[2][3] * in[3][2] - in[2][1] * in[1][2] * in[3][3] + in[2][1] * in[1][3] * in[3][2] + in[3][1] * in[1][2] * in[2][3] - in[3][1] * in[1][3] * in[2][2];
    m[1][0] = -in[1][0] * in[2][2] * in[3][3] + in[1][0] * in[2][3] * in[3][2] + in[2][0] * in[1][2] * in[3][3] - in[2][0] * in[1][3] * in[3][2] - in[3][0] * in[1][2] * in[2][3] + in[3][0] * in[1][3] * in[2][2];
    m[2][0] =  in[1][0] * in[2][1] * in[3][3] - in[1][0] * in[2][3] * in[3][1] - in[2][0] * in[1][1] * in[3][3] + in[2][0] * in[1][3] * in[3][1] + in[3][0] * in[1][1] * in[2][3] - in[3][0] * in[1][3] * in[2][1];
    m[3][0] = -in[1][0] * in[2][1] * in[3][2] + in[1][0] * in[2][2] * in[3][1] + in[2][0] * in[1][1] * in[3][2] - in[2][0] * in[1][2] * in[3][1] - in[3][0] * in[1][1] * in[2][2] + in[3][0] * in[1][2] * in[2][1];
    
    m[0][1] = -in[0][1] * in[2][2] * in[3][3] + in[0][1] * in[2][3] * in[3][2] + in[2][1] * in[0][2] * in[3][3] - in[2][1] * in[0][3] * in[3][2] - in[3][1] * in[0][2] * in[2][3] + in[3][1] * in[0][3] * in[2][2];
    m[1][1] =  in[0][0] * in[2][2] * in[3][3] - in[0][0] * in[2][3] * in[3][2] - in[2][0] * in[0][2] * in[3][3] + in[2][0] * in[0][3] * in[3][2] + in[3][0] * in[0][2] * in[2][3] - in[3][0] * in[0][3] * in[2][2];
    m[2][1] = -in[0][0] * in[2][1] * in[3][3] + in[0][0] * in[2][3] * in[3][1] + in[2][0] * in[0][1] * in[3][3] - in[2][0] * in[0][3] * in[3][1] - in[3][0] * in[0][1] * in[2][3] + in[3][0] * in[0][3] * in[2][1];
    m[3][1] =  in[0][0] * in[2][1] * in[3][2] - in[0][0] * in[2][2] * in[3][1] - in[2][0] * in[0][1] * in[3][2] + in[2][0] * in[0][2] * in[3][1] + in[3][0] * in[0][1] * in[2][2] - in[3][0] * in[0][2] * in[2][1];
    
    m[0][2] =  in[0][1] * in[1][2] * in[3][3] - in[0][1] * in[1][3] * in[3][2] - in[1][1] * in[0][2] * in[3][3] + in[1][1] * in[0][3] * in[3][2] + in[3][1] * in[0][2] * in[1][3] - in[3][1] * in[0][3] * in[1][2];
    m[1][2] = -in[0][0] * in[1][2] * in[3][3] + in[0][0] * in[1][3] * in[3][2] + in[1][0] * in[0][2] * in[3][3] - in[1][0] * in[0][3] * in[3][2] - in[3][0] * in[0][2] * in[1][3] + in[3][0] * in[0][3] * in[1][2];
    m[2][2] =  in[0][0] * in[1][1] * in[3][3] - in[0][0] * in[1][3] * in[3][1] - in[1][0] * in[0][1] * in[3][3] + in[1][0] * in[0][3] * in[3][1] + in[3][0] * in[0][1] * in[1][3] - in[3][0] * in[0][3] * in[1][1];
    m[3][2] = -in[0][0] * in[1][1] * in[3][2] + in[0][0] * in[1][2] * in[3][1] + in[1][0] * in[0][1] * in[3][2] - in[1][0] * in[0][2] * in[3][1] - in[3][0] * in[0][1] * in[1][2] + in[3][0] * in[0][2] * in[1][1];
    
    m[0][3] = -in[0][1] * in[1][2] * in[2][3] + in[0][1] * in[1][3] * in[2][2] + in[1][1] * in[0][2] * in[2][3] - in[1][1] * in[0][3] * in[2][2] - in[2][1] * in[0][2] * in[1][3] + in[2][1] * in[0][3] * in[1][2];
    m[1][3] =  in[0][0] * in[1][2] * in[2][3] - in[0][0] * in[1][3] * in[2][2] - in[1][0] * in[0][2] * in[2][3] + in[1][0] * in[0][3] * in[2][2] + in[2][0] * in[0][2] * in[1][3] - in[2][0] * in[0][3] * in[1][2];
    m[2][3] = -in[0][0] * in[1][1] * in[2][3] + in[0][0] * in[1][3] * in[2][1] + in[1][0] * in[0][1] * in[2][3] - in[1][0] * in[0][3] * in[2][1] - in[2][0] * in[0][1] * in[1][3] + in[2][0] * in[0][3] * in[1][1];
    m[3][3] =  in[0][0] * in[1][1] * in[2][2] - in[0][0] * in[1][2] * in[2][1] - in[1][0] * in[0][1] * in[2][2] + in[1][0] * in[0][2] * in[2][1] + in[2][0] * in[0][1] * in[1][2] - in[2][0] * in[0][2] * in[1][1];

    afxReal det = in[0][0] * m[0][0] + in[0][1] * m[1][0] + in[0][2] * m[2][0] + in[0][3] * m[3][0];

    if (det)
    {
        det = 1.0 / det;

        for (afxUnit i = 0; i < 4; i++)
            for (afxUnit j = 0; j < 4; j++)
                m[i][j] = m[i][j] * det;
    }
    return det;
}

_AFXINL afxReal AfxM4dInvertAffine(afxM4d m, afxM4d const in)
{
    // Should be compatible with void MatrixInvert4x3(float *DestInit, const float *SourceInit)

    afxReal det = (in[1][1] * in[2][2] - in[2][1] * in[1][2]) * in[0][0] - (in[2][2] * in[0][1] - in[2][1] * in[0][2]) * in[1][0] + (in[1][2] * in[0][1] - in[1][1] * in[0][2]) * in[2][0];

    if (det == (afxReal)0) AfxM4dReset(m); // granny ignora LT se não for inversível mas nulifica o W.
    else
    {
        afxReal recip = 1.0 / det;
        m[0][0] =  ( in[1][1] * in[2][2] - in[2][1] * in[1][2]) * recip;
        m[0][1] = -((in[2][2] * in[0][1] - in[2][1] * in[0][2]) * recip);
        m[0][2] =  ( in[1][2] * in[0][1] - in[1][1] * in[0][2]) * recip;
        m[0][3] = 0.f;

        m[1][0] = -((in[1][0] * in[2][2] - in[2][0] * in[1][2]) * recip);
        m[1][1] =  ( in[2][2] * in[0][0] - in[2][0] * in[0][2]) * recip;
        m[1][2] = -((in[1][2] * in[0][0] - in[0][2] * in[1][0]) * recip);
        m[1][3] = 0.f;

        m[2][0] =  ( in[2][1] * in[1][0] - in[2][0] * in[1][1]) * recip;
        m[2][1] = -((in[2][1] * in[0][0] - in[2][0] * in[0][1]) * recip);
        m[2][2] =  ( in[1][1] * in[0][0] - in[1][0] * in[0][1]) * recip;
        m[2][3] = 0.f;
    }
    m[3][0] = -(m[2][0] * in[3][2] + m[1][0] * in[3][1] + m[0][0] * in[3][0]);
    m[3][1] = -(m[2][1] * in[3][2] + m[1][1] * in[3][1] + m[0][1] * in[3][0]);
    m[3][2] = -(m[2][2] * in[3][2] + m[1][2] * in[3][1] + m[0][2] * in[3][0]);
    m[3][3] = 1.f;
    return det;
}

// Det

_AFXINL afxReal AfxM2dDet(afxM2d const m)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    return  (m[0][0] * m[1][1]) - 
            (m[0][1] * m[1][0]);
}

_AFXINL afxReal AfxM3dDet(afxM3d const m)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    return  (m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1])) - 
            (m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0])) + 
            (m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]));
}

_AFXINL afxReal AfxM4dDet(afxM4d const m)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    return  (m[0][0] * m[1][1] * m[2][2] * m[3][3]) +
            (m[0][0] * m[1][2] * m[2][3] * m[3][1]) +
            (m[0][0] * m[1][3] * m[2][1] * m[3][2]) +
            (m[0][1] * m[1][0] * m[2][3] * m[3][2]) +
            (m[0][1] * m[1][2] * m[2][0] * m[3][3]) +
            (m[0][1] * m[1][3] * m[2][2] * m[3][0]) +
            (m[0][2] * m[1][0] * m[2][1] * m[3][3]) +
            (m[0][2] * m[1][1] * m[2][3] * m[3][0]) +
            (m[0][2] * m[1][3] * m[2][0] * m[3][1]) +
            (m[0][3] * m[1][0] * m[2][2] * m[3][1]) +
            (m[0][3] * m[1][1] * m[2][0] * m[3][2]) +
            (m[0][3] * m[1][2] * m[2][1] * m[3][0]) -
            (m[0][0] * m[1][1] * m[2][3] * m[3][2]) -
            (m[0][0] * m[1][2] * m[2][1] * m[3][3]) -
            (m[0][0] * m[1][3] * m[2][2] * m[3][1]) -
            (m[0][1] * m[1][0] * m[2][2] * m[3][3]) -
            (m[0][1] * m[1][2] * m[2][3] * m[3][0]) -
            (m[0][1] * m[1][3] * m[2][0] * m[3][2]) -
            (m[0][2] * m[1][0] * m[2][3] * m[3][1]) -
            (m[0][2] * m[1][1] * m[2][0] * m[3][3]) -
            (m[0][2] * m[1][3] * m[2][1] * m[3][0]) -
            (m[0][3] * m[1][0] * m[2][1] * m[3][2]) -
            (m[0][3] * m[1][1] * m[2][2] * m[3][0]) -
            (m[0][3] * m[1][2] * m[2][0] * m[3][1]);
}

////////////////////////////////////////////////////////////////////////////////
// Affine transformation matrix methods                                       //
////////////////////////////////////////////////////////////////////////////////

// MakeScaling

_AFXINL void AfxM3dScaling(afxM3d m, afxV3d const scale)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(scale);
    AFX_ASSERT_DIFF(m, scale);
    AfxM3dReset(m);
    m[0][0] = scale[0];
    m[1][1] = scale[1];
    m[2][2] = scale[2];
}

_AFXINL void AfxM4dScaling(afxM4d m, afxV3d const scale)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(scale);
    AFX_ASSERT_DIFF(m, scale);
    AfxM4dReset(m);
    m[0][0] = scale[0];
    m[1][1] = scale[1];
    m[2][2] = scale[2];
}

// MakeRotation

_AFXINL void AfxM3dRotationFromQuat(afxM3d m, afxQuat const q)
{
    // Inspired in AfxM4dRotationFromQuat

    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(q);
    AFX_ASSERT_DIFF(m, q);

    afxReal qx = q[0], qxx = qx * qx;
    afxReal qy = q[1], qyy = qy * qy;
    afxReal qz = q[2], qzz = qz * qz;
    afxReal qw = q[3];

    m[0][0] = 1.f - 2.f * qyy - 2.f * qzz;
    m[0][1] = 2.f * qx * qy + 2.f * qz * qw;
    m[0][2] = 2.f * qx * qz - 2.f * qy * qw;

    m[1][0] = 2.f * qx * qy - 2.f * qz * qw;
    m[1][1] = 1.f - 2.f * qxx - 2.f * qzz;
    m[1][2] = 2.f * qy * qz + 2.f * qx * qw;

    m[2][0] = 2.f * qx * qz + 2.f * qy * qw;
    m[2][1] = 2.f * qy * qz - 2.f * qx * qw;
    m[2][2] = 1.f - 2.f * qxx - 2.f * qyy;
}

_AFXINL void AfxM4dRotationFromQuat(afxM4d m, afxQuat const q)
{
    // Should be compatible with XMMATRIX XMMatrixRotationQuaternion(FXMVECTOR Quaternion)

    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(q);
    AFX_ASSERT_DIFF(m, q);

    afxReal qx = q[0], qxx = qx * qx;
    afxReal qy = q[1], qyy = qy * qy;
    afxReal qz = q[2], qzz = qz * qz;
    afxReal qw = q[3];

    m[0][0] = 1.f - 2.f * qyy - 2.f * qzz;
    m[0][1] = 2.f * qx * qy + 2.f * qz * qw;
    m[0][2] = 2.f * qx * qz - 2.f * qy * qw;
    m[0][3] = 0.f;

    m[1][0] = 2.f * qx * qy - 2.f * qz * qw;
    m[1][1] = 1.f - 2.f * qxx - 2.f * qzz;
    m[1][2] = 2.f * qy * qz + 2.f * qx * qw;
    m[1][3] = 0.f;

    m[2][0] = 2.f * qx * qz + 2.f * qy * qw;
    m[2][1] = 2.f * qy * qz - 2.f * qx * qw;
    m[2][2] = 1.f - 2.f * qxx - 2.f * qyy;
    m[2][3] = 0.f;

    m[3][0] = 0.f;
    m[3][1] = 0.f;
    m[3][2] = 0.f;
    m[3][3] = 1.0f;
}

// MakeRotation_Euler
//

_AFXINL void AfxM4dRotationFromEuler(afxM4d m, afxReal pitch, afxReal yaw, afxReal roll)
{
    // Should be compatible with XMMATRIX XMMatrixRotationRollPitchYaw(float Pitch, float Yaw, float Roll)
    // Should be compatible with XMMATRIX XMMatrixRotationRollPitchYawFromVector(FXMVECTOR Angles)

    afxError err = { 0 };
    AFX_ASSERT(m);

    /*
        Angles are measured clockwise when looking along the rotation axis toward the origin. 
        This is a left-handed coordinate system. To use right-handed coordinates, negate all three angles.
    */
    pitch = -pitch;
    yaw = -yaw;
    roll = -roll;

    afxReal pc, ps, yc, ys, rc, rs;
    AfxCosSinf(pitch, &pc, &ps);
    AfxCosSinf(yaw, &yc, &ys);
    AfxCosSinf(roll, &rc, &rs);

    m[0][0] = rc * yc + rs * ps * ys;
    m[0][1] = rs * pc;
    m[0][2] = rs * ps * yc - rc * ys;
    m[0][3] = 0.f;

    m[1][0] = rc * ps * ys - rs * yc;
    m[1][1] = rc * pc;
    m[1][2] = rs * ys + rc * ps * yc;
    m[1][3] = 0.f;

    m[2][0] = pc * ys;
    m[2][1] =-ps;
    m[2][2] = pc * yc;
    m[2][3] = 0.f;

    m[3][0] = 0.f;
    m[3][1] = 0.f;
    m[3][2] = 0.f;
    m[3][3] = 1.f;
}

// uncertain origin

// MakeRotation_Axial
// Memory layout: looks like it is row-major

_AFXINL void AfxM3dRotationFromAxis(afxM3d m, afxV3d const axis, afxReal /*theta*/radians)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(axis);
    AFX_ASSERT(!AfxV3dIsZero(axis));
    AFX_ASSERT(!AfxV3dIsInfinite(axis));

    afxReal cosTheta, sinTheta;
    AfxCosSinf(radians, &cosTheta, &sinTheta);
    afxReal cosValue = 1.0f - cosTheta;
    afxV3d axis2;
    AfxV3dNormalize(axis2, axis);

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

_AFXINL void AfxM4dRotationFromAxis(afxM4d m, afxV3d const axis, afxReal /*theta*/radians)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(axis);
    AFX_ASSERT(!AfxV3dIsZero(axis));
    AFX_ASSERT(!AfxV3dIsInfinite(axis));

    afxReal cosTheta, sinTheta;
    AfxCosSinf(radians, &cosTheta, &sinTheta);
    afxReal cosValue = 1.0f - cosTheta;
    afxV3d axis2;
    AfxV3dNormalize(axis2, axis);

    m[0][0] = (axis2[0] * axis2[0] * cosValue) + cosTheta;
    m[0][1] = (axis2[0] * axis2[1] * cosValue) + (axis2[2] * sinTheta);
    m[0][2] = (axis2[0] * axis2[2] * cosValue) - (axis2[1] * sinTheta);
    m[0][3] = AFX_REAL(0);

    m[1][0] = (axis2[1] * axis2[0] * cosValue) - (axis2[2] * sinTheta);
    m[1][1] = (axis2[1] * axis2[1] * cosValue) + cosTheta;
    m[1][2] = (axis2[1] * axis2[2] * cosValue) + (axis2[0] * sinTheta);
    m[1][3] = AFX_REAL(0);

    m[2][0] = (axis2[2] * axis2[0] * cosValue) + (axis2[1] * sinTheta);
    m[2][1] = (axis2[2] * axis2[1] * cosValue) - (axis2[0] * sinTheta);
    m[2][2] = (axis2[2] * axis2[2] * cosValue) + cosTheta;
    m[2][3] = AFX_REAL(0);

    m[3][0] = AFX_REAL(0);
    m[3][1] = AFX_REAL(0);
    m[3][2] = AFX_REAL(0);
    m[3][3] = AFX_REAL(1);
}

_AFXINL void AfxM4dRotationX(afxM4d m, afxReal angle)
{
    // Should be compatible with XMMATRIX XMMatrixRotationX(float Angle)

    afxReal co, si;
    AfxCosSinf(angle, &co, &si);
    AfxM4dReset(m);
    m[1][1] = co;
    m[1][2] = si;
    m[2][1] = -si;
    m[2][2] = co;
}

_AFXINL void AfxM4dRotationY(afxM4d m, afxReal angle)
{
    // Should be compatible with XMMATRIX XMMatrixRotationY(float Angle)

    afxReal co, si;
    AfxCosSinf(angle, &co, &si);
    AfxM4dReset(m);
    m[0][0] = co;
    m[0][2] = -si;
    m[2][0] = si;
    m[2][2] = co;
}

_AFXINL void AfxM4dRotationZ(afxM4d m, afxReal angle)
{
    // Should be compatible with XMMATRIX XMMatrixRotationZ(float Angle)

    afxReal co, si;
    AfxCosSinf(angle, &co, &si);
    AfxM4dReset(m);
    m[0][0] = co;
    m[0][1] = si;
    m[1][0] = -si;
    m[1][1] = co;
}

// Translate

_AFXINL void AfxM4dTranslation(afxM4d m, afxV3d const translation)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(translation);
    AfxM4dReset(m);
    AfxV3dCopy(m[3], translation);    
}

// Decompose

_AFXINL afxBool AfxPolarDecomposeM3d(afxM3d const m, afxReal tol, afxM3d rm, afxM3d ssm)
{
    // Should be compatible with char PolarDecompose(const float *Source3x3Init, float Tolerance, float *QInit, float *SInit)

    afxM3d ZMirror;
    AfxM3dReset(ZMirror);
    ZMirror[2][2] = -1.0;

    // 0 1 2
    // 3 4 5
    // 6 7 8

    if (AfxM3dDet(m) < 0.0)
    {
        AfxM3dMultiply(rm, m, ZMirror);
    }
    else
    {
        AfxM3dCopy(rm, m);
    }

    afxUnit i = 0;
    afxResult rslt = TRUE;

    while (1)
    {
        AfxM3dInvert(ssm, rm);
        AfxM3dAdd(ssm, ssm, rm);

        AfxV3dScale(ssm[0], ssm[0], 0.5);
        AfxV3dScale(ssm[1], ssm[1], 0.5);
        AfxV3dScale(ssm[2], ssm[2], 0.5);

        afxM3d ssMinR;
        AfxM3dSub(ssMinR, ssm, rm);

        AfxM3dCopy(rm, ssm);

        if (fabs(ssMinR[0][0]) + fabs(ssMinR[0][1]) + fabs(ssMinR[0][2]) + fabs(ssMinR[1][0]) + fabs(ssMinR[1][1]) + fabs(ssMinR[1][2]) + fabs(ssMinR[2][0]) + fabs(ssMinR[2][1]) + fabs(ssMinR[2][2]) < tol)
            break;

        if (++i >= 1000)
        {
            rslt = FALSE;
            break;
        }
    }
    TransposeMatrixMultiply3x3(rm, ssm, m);
    return rslt;
}

// Compose

_AFXINL void AfxM4dComposeTransformation(afxM4d m, afxV3d const scalPivot, afxQuat const scalOrient, afxV3d const scaling, afxV3d const rotPivot, afxQuat const rotQuat, afxV3d const translation)
{
    // Should be compatible with XMMATRIX XMMatrixTransformation(FXMVECTOR ScalingOrigin, FXMVECTOR ScalingOrientationQuaternion, FXMVECTOR Scaling, GXMVECTOR RotationOrigin, HXMVECTOR RotationQuaternion, HXMVECTOR Translation)

    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(scalPivot);
    AFX_ASSERT(scalOrient);
    AFX_ASSERT(scaling);
    AFX_ASSERT(rotPivot);
    AFX_ASSERT(rotQuat);
    AFX_ASSERT(translation);

    // M = Inverse(MScalingOrigin) * Transpose(MScalingOrientation) * MScaling * MScalingOrientation *
    //         MScalingOrigin * Inverse(MRotationOrigin) * MRotation * MRotationOrigin * MTranslation;

    afxV3d negScaOrigin;
    AfxV3dNeg(negScaOrigin, scalPivot);

    afxM4d scalOriginI, scalOrientM, scalOrientT, scalM, rotM;
    AfxM4dReset(scalOriginI);
    AfxV3dCopy(scalOriginI[3], negScaOrigin);
    AfxM4dRotationFromQuat(scalOrientM, scalOrient);
    AfxM4dTranspose(scalOrientT, scalOrientM);
    AfxM4dReset(scalM);
    AfxM4dScaling(scalM, scaling);
    AfxM4dRotationFromQuat(rotM, rotQuat);

    afxM4d t;
    AfxM4dMultiply(t, scalOriginI, scalOrientT);
    AfxM4dMultiply(m, t, scalM);
    AfxM4dMultiply(t, m, scalOrientM);
    AfxV3dAdd(t[3], t[3], scalPivot);
    AfxV3dSub(t[3], t[3], rotPivot);
    AfxM4dMultiply(m, t, rotM);
    AfxV3dAdd(m[3], m[3], rotPivot);
    AfxV3dAdd(m[3], m[3], translation);
}

_AFXINL void AfxM4dComposeAffineTransformation(afxM4d m, afxV3d const scale, afxV3d const rotPivot, afxQuat const rot, afxV3d const translation)
{
    // Should be compatible with XMMATRIX XMMatrixAffineTransformation(FXMVECTOR Scaling, FXMVECTOR RotationOrigin, FXMVECTOR RotationQuaternion, GXMVECTOR Translation)

    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(scale);
    AFX_ASSERT(rot);
    AFX_ASSERT(translation);

    // M = MScaling * Inverse(MRotationOrigin) * MRotation * MRotationOrigin * MTranslation;

    afxM4d sm, rm, c;
    AfxM4dReset(sm);
    AfxM4dScaling(sm, scale);
    AfxM4dRotationFromQuat(rm, rot);
    AfxV3dSub(sm[3], sm[3], rotPivot);
    AfxM4dMultiply(m, sm, rm);
    AfxV3dAdd(m[3], m[3], rotPivot);
    AfxV3dAdd(m[3], m[3], translation);
}

////////////////////////////////////////////////////////////////////////////////
// MATRIX TRANSFORMATION METHODS                                              //
////////////////////////////////////////////////////////////////////////////////

// In post-multiplication, each component of the result, at in given row R and column C, is computed by taking the dot product of the left matrix’s row R by the right matrix’s column C.

// MultiplyTransposed

_AFXINL void AfxM2dMultiply(afxM2d m, afxM2d const in, afxM2d const by)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(in);
    AFX_ASSERT(by);
    AFX_ASSERT(in != by);
    AFX_ASSERT(by != m);

    // Cut off in[][col] * by[row][] to visualize it in your mind
    // m[row<in>][col<by>] = in[row][] * by[][i]

    m[0][0] = (in[0][0] * by[0][0]) + (in[0][1] * by[1][0]);
    m[0][1] = (in[0][0] * by[0][1]) + (in[0][1] * by[1][1]);

    m[1][0] = (in[1][0] * by[0][0]) + (in[1][1] * by[1][0]);
    m[1][1] = (in[1][0] * by[0][1]) + (in[1][1] * by[1][1]);
}

_AFXINL void AfxM2dMultiplyTransposed(afxM2d m, afxM2d const in, afxM2d const by)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(in);
    AFX_ASSERT(by);
    AFX_ASSERT(in != by);
    AFX_ASSERT(by != m);

    // Cut off in[][col] * by[row][] to visualize it in your mind
    // m[col<by>][row<in>] = in[i][] * by[][col]

    m[0][0] = (in[0][0] * by[0][0]) + (in[0][1] * by[1][0]);
    m[0][1] = (in[1][0] * by[0][0]) + (in[1][1] * by[1][0]);

    m[1][0] = (in[0][0] * by[0][1]) + (in[0][1] * by[1][1]);
    m[1][1] = (in[1][0] * by[0][1]) + (in[1][1] * by[1][1]);
}

_AFXINL void AfxM3dMultiply(afxM3d m, afxM3d const in, afxM3d const by)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(in);
    AFX_ASSERT(by);
    AFX_ASSERT(in != by);
    AFX_ASSERT(by != m);

    // Compatible with MatrixMultiply3x3(m, in, by)

    // Cut off in[][col] * by[row][] to visualize it in your mind
    // m[row<in>][col<by>] = in[row][] * by[][i]

    m[0][0] = (in[0][0] * by[0][0]) + (in[0][1] * by[1][0]) + (in[0][2] * by[2][0]);
    m[0][1] = (in[0][0] * by[0][1]) + (in[0][1] * by[1][1]) + (in[0][2] * by[2][1]);
    m[0][2] = (in[0][0] * by[0][2]) + (in[0][1] * by[1][2]) + (in[0][2] * by[2][2]);

    m[1][0] = (in[1][0] * by[0][0]) + (in[1][1] * by[1][0]) + (in[1][2] * by[2][0]);
    m[1][1] = (in[1][0] * by[0][1]) + (in[1][1] * by[1][1]) + (in[1][2] * by[2][1]);
    m[1][2] = (in[1][0] * by[0][2]) + (in[1][1] * by[1][2]) + (in[1][2] * by[2][2]);

    m[2][0] = (in[2][0] * by[0][0]) + (in[2][1] * by[1][0]) + (in[2][2] * by[2][0]);
    m[2][1] = (in[2][0] * by[0][1]) + (in[2][1] * by[1][1]) + (in[2][2] * by[2][1]);
    m[2][2] = (in[2][0] * by[0][2]) + (in[2][1] * by[1][2]) + (in[2][2] * by[2][2]);
}

_AFXINL void AfxM3dMultiplyTransposed(afxM3d m, afxM3d const in, afxM3d const by)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(in);
    AFX_ASSERT(by);
    AFX_ASSERT(in != by);
    AFX_ASSERT(by != m);

    // Cut off in[][col] * by[row][] to visualize it in your mind
    // m[col<by>][row<in>] = in[i][] * by[][col]

    m[0][0] = (in[0][0] * by[0][0]) + (in[0][1] * by[1][0]) + (in[0][2] * by[2][0]);
    m[0][1] = (in[1][0] * by[0][0]) + (in[1][1] * by[1][0]) + (in[1][2] * by[2][0]);
    m[0][2] = (in[2][0] * by[0][0]) + (in[2][1] * by[1][0]) + (in[2][2] * by[2][0]);

    m[1][0] = (in[0][0] * by[0][1]) + (in[0][1] * by[1][1]) + (in[0][2] * by[2][1]);
    m[1][1] = (in[1][0] * by[0][1]) + (in[1][1] * by[1][1]) + (in[1][2] * by[2][1]);
    m[1][2] = (in[2][0] * by[0][1]) + (in[2][1] * by[1][1]) + (in[2][2] * by[2][1]);

    m[2][0] = (in[0][0] * by[0][2]) + (in[0][1] * by[1][2]) + (in[0][2] * by[2][2]);
    m[2][1] = (in[1][0] * by[0][2]) + (in[1][1] * by[1][2]) + (in[1][2] * by[2][2]);
    m[2][2] = (in[2][0] * by[0][2]) + (in[2][1] * by[1][2]) + (in[2][2] * by[2][2]);
}

_AFXINL void TransposeMatrixMultiply3x3(afxM3d IntoMatrix, afxM3d const TransposedMatrix, afxM3d const ByMatrix)
{
    // Compatible with TransposeMatrixMultiply3x3(out, in, by)

// 0 1 2, 3 4 5, 6 7 8

    // This function takes a already transposed matrix while AfxM3dMultiplyTransposed computes a new one. 

    IntoMatrix[0][0] = TransposedMatrix[0][0] * ByMatrix[0][0] + TransposedMatrix[1][0] * ByMatrix[1][0] + TransposedMatrix[2][0] * ByMatrix[2][0];
    IntoMatrix[0][1] = TransposedMatrix[0][0] * ByMatrix[0][1] + TransposedMatrix[1][0] * ByMatrix[1][1] + TransposedMatrix[2][0] * ByMatrix[2][1];
    IntoMatrix[0][2] = TransposedMatrix[0][0] * ByMatrix[0][2] + TransposedMatrix[1][0] * ByMatrix[1][2] + TransposedMatrix[2][0] * ByMatrix[2][2];

    IntoMatrix[1][0] = TransposedMatrix[0][1] * ByMatrix[0][0] + TransposedMatrix[1][1] * ByMatrix[1][0] + TransposedMatrix[2][1] * ByMatrix[2][0];
    IntoMatrix[1][1] = TransposedMatrix[0][1] * ByMatrix[0][1] + TransposedMatrix[1][1] * ByMatrix[1][1] + TransposedMatrix[2][1] * ByMatrix[2][1];
    IntoMatrix[1][2] = TransposedMatrix[0][1] * ByMatrix[0][2] + TransposedMatrix[1][1] * ByMatrix[1][2] + TransposedMatrix[2][1] * ByMatrix[2][2];
    
    IntoMatrix[2][0] = TransposedMatrix[0][2] * ByMatrix[0][0] + TransposedMatrix[1][2] * ByMatrix[1][0] + TransposedMatrix[2][2] * ByMatrix[2][0];
    IntoMatrix[2][1] = TransposedMatrix[0][2] * ByMatrix[0][1] + TransposedMatrix[1][2] * ByMatrix[1][1] + TransposedMatrix[2][2] * ByMatrix[2][1];
    IntoMatrix[2][2] = TransposedMatrix[0][2] * ByMatrix[0][2] + TransposedMatrix[1][2] * ByMatrix[1][2] + TransposedMatrix[2][2] * ByMatrix[2][2];
}

_AFXINL void AfxM4dMultiply(afxM4d m, afxM4d const in, afxM4d const by)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(in);
    AFX_ASSERT(by);
    AFX_ASSERT(in != by);
    AFX_ASSERT(by != m);

    // Compatible with m = XMMatrixMultiply(in, by)
    // Compatible with ColumnMatrixMultiply4x4(m, in, by)

    // Cut off in[][col] * by[row][] to visualize it in your mind
    // m[row<in>][col<by>] = in[row][] * by[][i]

    m[0][0] = (in[0][0] * by[0][0]) + (in[0][1] * by[1][0]) + (in[0][2] * by[2][0]) + (in[0][3] * by[3][0]);
    m[0][1] = (in[0][0] * by[0][1]) + (in[0][1] * by[1][1]) + (in[0][2] * by[2][1]) + (in[0][3] * by[3][1]);
    m[0][2] = (in[0][0] * by[0][2]) + (in[0][1] * by[1][2]) + (in[0][2] * by[2][2]) + (in[0][3] * by[3][2]);
    m[0][3] = (in[0][0] * by[0][3]) + (in[0][1] * by[1][3]) + (in[0][2] * by[2][3]) + (in[0][3] * by[3][3]);

    m[1][0] = (in[1][0] * by[0][0]) + (in[1][1] * by[1][0]) + (in[1][2] * by[2][0]) + (in[1][3] * by[3][0]);
    m[1][1] = (in[1][0] * by[0][1]) + (in[1][1] * by[1][1]) + (in[1][2] * by[2][1]) + (in[1][3] * by[3][1]);
    m[1][2] = (in[1][0] * by[0][2]) + (in[1][1] * by[1][2]) + (in[1][2] * by[2][2]) + (in[1][3] * by[3][2]);
    m[1][3] = (in[1][0] * by[0][3]) + (in[1][1] * by[1][3]) + (in[1][2] * by[2][3]) + (in[1][3] * by[3][3]);

    m[2][0] = (in[2][0] * by[0][0]) + (in[2][1] * by[1][0]) + (in[2][2] * by[2][0]) + (in[2][3] * by[3][0]);
    m[2][1] = (in[2][0] * by[0][1]) + (in[2][1] * by[1][1]) + (in[2][2] * by[2][1]) + (in[2][3] * by[3][1]);
    m[2][2] = (in[2][0] * by[0][2]) + (in[2][1] * by[1][2]) + (in[2][2] * by[2][2]) + (in[2][3] * by[3][2]);
    m[2][3] = (in[2][0] * by[0][3]) + (in[2][1] * by[1][3]) + (in[2][2] * by[2][3]) + (in[2][3] * by[3][3]);

    m[3][0] = (in[3][0] * by[0][0]) + (in[3][1] * by[1][0]) + (in[3][2] * by[2][0]) + (in[3][3] * by[3][0]);
    m[3][1] = (in[3][0] * by[0][1]) + (in[3][1] * by[1][1]) + (in[3][2] * by[2][1]) + (in[3][3] * by[3][1]);
    m[3][2] = (in[3][0] * by[0][2]) + (in[3][1] * by[1][2]) + (in[3][2] * by[2][2]) + (in[3][3] * by[3][2]);
    m[3][3] = (in[3][0] * by[0][3]) + (in[3][1] * by[1][3]) + (in[3][2] * by[2][3]) + (in[3][3] * by[3][3]);
}

_AFXINL void AfxM4dMultiplyTransposed(afxM4d m, afxM4d const in, afxM4d const by)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(in);
    AFX_ASSERT(by);
    AFX_ASSERT(in != by);
    AFX_ASSERT(by != m);

    // Compatible with m = XMMatrixMultiplyTranspose(in, by)

    // Cut off in[][col] * by[row][] to visualize it in your mind
    // m[col<by>][row<in>] = in[i][] * by[][col]

    m[0][0] = (in[0][0] * by[0][0]) + (in[0][1] * by[1][0]) + (in[0][2] * by[2][0]) + (in[0][3] * by[3][0]);
    m[0][1] = (in[1][0] * by[0][0]) + (in[1][1] * by[1][0]) + (in[1][2] * by[2][0]) + (in[1][3] * by[3][0]);
    m[0][2] = (in[2][0] * by[0][0]) + (in[2][1] * by[1][0]) + (in[2][2] * by[2][0]) + (in[2][3] * by[3][0]);
    m[0][3] = (in[3][0] * by[0][0]) + (in[3][1] * by[1][0]) + (in[3][2] * by[2][0]) + (in[3][3] * by[3][0]);

    m[1][0] = (in[0][0] * by[0][1]) + (in[0][1] * by[1][1]) + (in[0][2] * by[2][1]) + (in[0][3] * by[3][1]);
    m[1][1] = (in[1][0] * by[0][1]) + (in[1][1] * by[1][1]) + (in[1][2] * by[2][1]) + (in[1][3] * by[3][1]);
    m[1][2] = (in[2][0] * by[0][1]) + (in[2][1] * by[1][1]) + (in[2][2] * by[2][1]) + (in[2][3] * by[3][1]);
    m[1][3] = (in[3][0] * by[0][1]) + (in[3][1] * by[1][1]) + (in[3][2] * by[2][1]) + (in[3][3] * by[3][1]);

    m[2][0] = (in[0][0] * by[0][2]) + (in[0][1] * by[1][2]) + (in[0][2] * by[2][2]) + (in[0][3] * by[3][2]);
    m[2][1] = (in[1][0] * by[0][2]) + (in[1][1] * by[1][2]) + (in[1][2] * by[2][2]) + (in[1][3] * by[3][2]);
    m[2][2] = (in[2][0] * by[0][2]) + (in[2][1] * by[1][2]) + (in[2][2] * by[2][2]) + (in[2][3] * by[3][2]);
    m[2][3] = (in[3][0] * by[0][2]) + (in[3][1] * by[1][2]) + (in[3][2] * by[2][2]) + (in[3][3] * by[3][2]);

    m[3][0] = (in[0][0] * by[0][3]) + (in[0][1] * by[1][3]) + (in[0][2] * by[2][3]) + (in[0][3] * by[3][3]);
    m[3][1] = (in[1][0] * by[0][3]) + (in[1][1] * by[1][3]) + (in[1][2] * by[2][3]) + (in[1][3] * by[3][3]);
    m[3][2] = (in[2][0] * by[0][3]) + (in[2][1] * by[1][3]) + (in[2][2] * by[2][3]) + (in[2][3] * by[3][3]);
    m[3][3] = (in[3][0] * by[0][3]) + (in[3][1] * by[1][3]) + (in[3][2] * by[2][3]) + (in[3][3] * by[3][3]);
}

// MultiplyPlanar/Linear/Affine

_AFXINL void AfxM3dMultiplyAffine(afxM3d m, afxM3d const in, afxM3d const by)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(in);
    AFX_ASSERT(by);
    AFX_ASSERT(in != by);
    AFX_ASSERT(by != m);

    // Cut off in[][col] * by[row][] to visualize it in your mind
    // m[row<in>][col<by>] = in[row][] * by[][i]

    m[0][0] = (in[0][0] * by[0][0]) + (in[0][1] * by[1][0]) + (in[0][2] * by[2][0]);
    m[0][1] = (in[0][0] * by[0][1]) + (in[0][1] * by[1][1]) + (in[0][2] * by[2][1]);
    m[0][2] = 0.f;

    m[1][0] = (in[1][0] * by[0][0]) + (in[1][1] * by[1][0]) + (in[1][2] * by[2][0]);
    m[1][1] = (in[1][0] * by[0][1]) + (in[1][1] * by[1][1]) + (in[1][2] * by[2][1]);
    m[1][2] = 0.f;

    m[2][0] = (in[2][0] * by[0][0]) + (in[2][1] * by[1][0]) + (in[2][2] * by[2][0]);
    m[2][1] = (in[2][0] * by[0][1]) + (in[2][1] * by[1][1]) + (in[2][2] * by[2][1]);
    m[2][2] = 1.f;
}

_AFXINL void AfxM3dMultiplyTransposedAffine(afxM3d m, afxM3d const in, afxM3d const by)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(in);
    AFX_ASSERT(by);
    AFX_ASSERT(in != by);
    AFX_ASSERT(by != m);

    // Cut off in[][col] * by[row][] to visualize it in your mind
    // m[col<by>][row<in>] = in[i][] * by[][col]

    m[0][0] = (in[0][0] * by[0][0]) + (in[0][1] * by[1][0]) + (in[0][2] * by[2][0]);
    m[0][1] = (in[1][0] * by[0][0]) + (in[1][1] * by[1][0]) + (in[1][2] * by[2][0]);
    m[0][2] = 0.f;

    m[1][0] = (in[0][0] * by[0][1]) + (in[0][1] * by[1][1]) + (in[0][2] * by[2][1]);
    m[1][1] = (in[1][0] * by[0][1]) + (in[1][1] * by[1][1]) + (in[1][2] * by[2][1]);
    m[1][2] = 0.f;

    m[2][0] = (in[0][0] * by[0][2]) + (in[0][1] * by[1][2]) + (in[0][2] * by[2][2]);
    m[2][1] = (in[1][0] * by[0][2]) + (in[1][1] * by[1][2]) + (in[1][2] * by[2][2]);
    m[2][2] = 1.f;
}

_AFXINL void AfxM4dMultiplyLinear(afxM4d m, afxM4d const in, afxM4d const by)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(in);
    AFX_ASSERT(by);
    AFX_ASSERT(in != by);
    AFX_ASSERT(by != m);

    // Cut off in[][col] * by[row][] to visualize it in your mind
    // m[row<in>][col<by>] = in[row][] * by[][i]

    m[0][0] = (in[0][0] * by[0][0]) + (in[0][1] * by[1][0]) + (in[0][2] * by[2][0]);
    m[0][1] = (in[0][0] * by[0][1]) + (in[0][1] * by[1][1]) + (in[0][2] * by[2][1]);
    m[0][2] = (in[0][0] * by[0][2]) + (in[0][1] * by[1][2]) + (in[0][2] * by[2][2]);
    m[0][3] = 0.f;

    m[1][0] = (in[1][0] * by[0][0]) + (in[1][1] * by[1][0]) + (in[1][2] * by[2][0]);
    m[1][1] = (in[1][0] * by[0][1]) + (in[1][1] * by[1][1]) + (in[1][2] * by[2][1]);
    m[1][2] = (in[1][0] * by[0][2]) + (in[1][1] * by[1][2]) + (in[1][2] * by[2][2]);
    m[1][3] = 0.f;

    m[2][0] = (in[2][0] * by[0][0]) + (in[2][1] * by[1][0]) + (in[2][2] * by[2][0]);
    m[2][1] = (in[2][0] * by[0][1]) + (in[2][1] * by[1][1]) + (in[2][2] * by[2][1]);
    m[2][2] = (in[2][0] * by[0][2]) + (in[2][1] * by[1][2]) + (in[2][2] * by[2][2]);
    m[2][3] = 0.f;

    m[3][0] = 0.f;
    m[3][1] = 0.f;
    m[3][2] = 0.f;
    m[3][3] = 1.f;
}

_AFXINL void AfxM4dMultiplyTransposedLinear(afxM4d m, afxM4d const in, afxM4d const by)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(in);
    AFX_ASSERT(by);
    AFX_ASSERT(in != by);
    AFX_ASSERT(by != m);

    // Cut off in[][col] * by[row][] to visualize it in your mind
    // m[col<by>][row<in>] = in[i][] * by[][col]

    m[0][0] = (in[0][0] * by[0][0]) + (in[0][1] * by[1][0]) + (in[0][2] * by[2][0]);
    m[0][1] = (in[1][0] * by[0][0]) + (in[1][1] * by[1][0]) + (in[1][2] * by[2][0]);
    m[0][2] = (in[2][0] * by[0][0]) + (in[2][1] * by[1][0]) + (in[2][2] * by[2][0]);
    m[0][3] = 0.f;

    m[1][0] = (in[0][0] * by[0][1]) + (in[0][1] * by[1][1]) + (in[0][2] * by[2][1]);
    m[1][1] = (in[1][0] * by[0][1]) + (in[1][1] * by[1][1]) + (in[1][2] * by[2][1]);
    m[1][2] = (in[2][0] * by[0][1]) + (in[2][1] * by[1][1]) + (in[2][2] * by[2][1]);
    m[1][3] = 0.f;

    m[2][0] = (in[0][0] * by[0][2]) + (in[0][1] * by[1][2]) + (in[0][2] * by[2][2]);
    m[2][1] = (in[1][0] * by[0][2]) + (in[1][1] * by[1][2]) + (in[1][2] * by[2][2]);
    m[2][2] = (in[2][0] * by[0][2]) + (in[2][1] * by[1][2]) + (in[2][2] * by[2][2]);
    m[2][3] = 0.f;

    m[3][0] = 0.f;
    m[3][1] = 0.f;
    m[3][2] = 0.f;
    m[3][3] = 1.f;
}

_AFXINL void AfxM4dMultiplyAffine(afxM4d m, afxM4d const in, afxM4d const by)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(in);
    AFX_ASSERT(by);
    AFX_ASSERT(in != by);
    AFX_ASSERT(by != m);

    // Compatible with ColumnMatrixMultiply4x3(m, in, by)

    // Cut off in[][col] * by[row][] to visualize it in your mind
    // m[row<in>][col<by>] = in[row][] * by[][i]

    m[0][0] = (in[0][0] * by[0][0]) + (in[0][1] * by[1][0]) + (in[0][2] * by[2][0]);
    m[0][1] = (in[0][0] * by[0][1]) + (in[0][1] * by[1][1]) + (in[0][2] * by[2][1]);
    m[0][2] = (in[0][0] * by[0][2]) + (in[0][1] * by[1][2]) + (in[0][2] * by[2][2]);
    m[0][3] = 0.f;

    m[1][0] = (in[1][0] * by[0][0]) + (in[1][1] * by[1][0]) + (in[1][2] * by[2][0]);
    m[1][1] = (in[1][0] * by[0][1]) + (in[1][1] * by[1][1]) + (in[1][2] * by[2][1]);
    m[1][2] = (in[1][0] * by[0][2]) + (in[1][1] * by[1][2]) + (in[1][2] * by[2][2]);
    m[1][3] = 0.f;

    m[2][0] = (in[2][0] * by[0][0]) + (in[2][1] * by[1][0]) + (in[2][2] * by[2][0]);
    m[2][1] = (in[2][0] * by[0][1]) + (in[2][1] * by[1][1]) + (in[2][2] * by[2][1]);
    m[2][2] = (in[2][0] * by[0][2]) + (in[2][1] * by[1][2]) + (in[2][2] * by[2][2]);
    m[2][3] = 0.f;

    m[3][0] = (in[3][0] * by[0][0]) + (in[3][1] * by[1][0]) + (in[3][2] * by[2][0]) + by[3][0];
    m[3][1] = (in[3][0] * by[0][1]) + (in[3][1] * by[1][1]) + (in[3][2] * by[2][1]) + by[3][1];
    m[3][2] = (in[3][0] * by[0][2]) + (in[3][1] * by[1][2]) + (in[3][2] * by[2][2]) + by[3][2];
    m[3][3] = 1.f;
}

_AFXINL void AfxM4dMultiplyTransposedAffine(afxM4d m, afxM4d const in, afxM4d const by)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(in);
    AFX_ASSERT(by);
    AFX_ASSERT(in != by);
    AFX_ASSERT(by != m);

    // Cut off in[][col] * by[row][] to visualize it in your mind
    // m[col<by>][row<in>] = in[i][] * by[][col]

    m[0][0] = (in[0][0] * by[0][0]) + (in[0][1] * by[1][0]) + (in[0][2] * by[2][0]);
    m[0][1] = (in[1][0] * by[0][0]) + (in[1][1] * by[1][0]) + (in[1][2] * by[2][0]);
    m[0][2] = (in[2][0] * by[0][0]) + (in[2][1] * by[1][0]) + (in[2][2] * by[2][0]);
    m[0][3] = 0.f;

    m[1][0] = (in[0][0] * by[0][1]) + (in[0][1] * by[1][1]) + (in[0][2] * by[2][1]);
    m[1][1] = (in[1][0] * by[0][1]) + (in[1][1] * by[1][1]) + (in[1][2] * by[2][1]);
    m[1][2] = (in[2][0] * by[0][1]) + (in[2][1] * by[1][1]) + (in[2][2] * by[2][1]);
    m[1][3] = 0.f;

    m[2][0] = (in[0][0] * by[0][2]) + (in[0][1] * by[1][2]) + (in[0][2] * by[2][2]);
    m[2][1] = (in[1][0] * by[0][2]) + (in[1][1] * by[1][2]) + (in[1][2] * by[2][2]);
    m[2][2] = (in[2][0] * by[0][2]) + (in[2][1] * by[1][2]) + (in[2][2] * by[2][2]);
    m[2][3] = 0.f;

    m[3][0] = (in[0][0] * by[0][3]) + (in[0][1] * by[1][3]) + (in[0][2] * by[2][3]) + in[0][3];
    m[3][1] = (in[1][0] * by[0][3]) + (in[1][1] * by[1][3]) + (in[1][2] * by[2][3]) + in[1][3];
    m[3][2] = (in[2][0] * by[0][3]) + (in[2][1] * by[1][3]) + (in[2][2] * by[2][3]) + in[2][3];
    m[3][3] = 1.f;
}

// Multiply

// MultiplyArrayedV2d

_AFXINL void AfxM2dPostMultiplyV2d(afxM2d const m, afxUnit cnt, afxV2d const in[], afxV2d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
        AfxV2dPostMultiplyM2d(out[i], m, in[i]);
}

_AFXINL void AfxM3dPostMultiplyV3d(afxM3d const m, afxUnit cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
        AfxV3dPostMultiplyM3d(out[i], m, in[i]);
}

_AFXINL void AfxM4dPostMultiplyV4d(afxM4d const m, afxUnit cnt, afxV4d const in[], afxV4d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
        AfxV4dPostMultiplyM4d(out[i], m, in[i]);
}

_AFXINL void AfxM4dPostMultiplyAtv3d(afxM4d const m, afxUnit cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
        AfxV3dPostMultiplyAtm4d(out[i], m, in[i]);
}

_AFXINL void AfxM3dPostMultiplyLtv4d(afxM3d const m, afxUnit cnt, afxV4d const in[], afxV4d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
        AfxV4dPostMultiplyM3d(out[i], m, in[i]);
}

_AFXINL void AfxM4dPostMultiplyAtv4d(afxM4d const m, afxUnit cnt, afxV4d const in[], afxV4d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
        AfxV4dPostMultiplyAtm4d(out[i], m, in[i]);
}

// PreMultiply

_AFXINL void AfxM2dPreMultiplyV2d(afxM2d const m, afxUnit cnt, afxV2d const in[], afxV2d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
        AfxV2dPreMultiplyM2d(out[i], in[i], m);
}

_AFXINL void AfxM3dPreMultiplyV3d(afxM3d const m, afxUnit cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
        AfxV3dPreMultiplyM3d(out[i], in[i], m);
}

_AFXINL void AfxM4dPreMultiplyV4d(afxM4d const m, afxUnit cnt, afxV4d const in[], afxV4d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
        AfxV4dPreMultiplyM4d(out[i], in[i], m);
}

_AFXINL void AfxM4dPreMultiplyAtv3d(afxM4d const m, afxUnit cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
        AfxV3dPreMultiplyAtm4d(out[i], in[i], m);
}

_AFXINL void AfxM3dPreMultiplyLtv4d(afxM3d const m, afxUnit cnt, afxV4d const in[], afxV4d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
        AfxV4dPreMultiplyM3d(out[i], in[i], m);
}

_AFXINL void AfxM4dPreMultiplyAtv4d(afxM4d const m, afxUnit cnt, afxV4d const in[], afxV4d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
        AfxV4dPreMultiplyAtm4d(out[i], in[i], m);
}

// TransformSerialized

_AFXINL void AfxM2dPostMultiplyV2dSerialized(afxM2d const m, afxUnit inStride, afxUnit outStride, afxUnit cnt, afxV2d const in[], afxV2d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(inStride);
    AFX_ASSERT(out);
    AFX_ASSERT(outStride);

    afxByte* inPtr = (afxByte*)in;
    afxByte* outPtr = (afxByte*)out;

    for (afxUnit i = 0; i < cnt; i++)
    {
        AfxV2dPostMultiplyM2d((void*)outPtr, m, (void*)inPtr);

        inPtr += inStride;
        outPtr += outStride;
    }
}

_AFXINL void AfxM3dPostMultiplyV3dSerialized(afxM3d const m, afxUnit inStride, afxUnit outStride, afxUnit cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(inStride);
    AFX_ASSERT(out);
    AFX_ASSERT(outStride);

    afxByte* inPtr = (afxByte*)in;
    afxByte* outPtr = (afxByte*)out;

    for (afxUnit i = 0; i < cnt; i++)
    {
        AfxV3dPostMultiplyM3d((void*)outPtr, m, (void*)inPtr);

        inPtr += inStride;
        outPtr += outStride;
    }
}

_AFXINL void AfxM4dPostMultiplyV4dSerialized(afxM4d const m, afxUnit inStride, afxUnit outStride, afxUnit cnt, afxV4d const in[], afxV4d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(inStride);
    AFX_ASSERT(out);
    AFX_ASSERT(outStride);

    afxByte* inPtr = (afxByte*)in;
    afxByte* outPtr = (afxByte*)out;

    for (afxUnit i = 0; i < cnt; i++)
    {
        AfxV4dPostMultiplyM4d((void*)outPtr, m, (void*)inPtr);

        inPtr += inStride;
        outPtr += outStride;
    }
}

_AFXINL void AfxM4dPostMultiplyAtv3dSerialized(afxM4d const m, afxUnit inStride, afxUnit outStride, afxUnit cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(inStride);
    AFX_ASSERT(out);
    AFX_ASSERT(outStride);

    afxByte* inPtr = (afxByte*)in;
    afxByte* outPtr = (afxByte*)out;

    for (afxUnit i = 0; i < cnt; i++)
    {
        AfxV3dPostMultiplyAtm4d((void*)outPtr, m, (void*)inPtr);

        inPtr += inStride;
        outPtr += outStride;
    }
}

_AFXINL void AfxM3dPostMultiplyLtv4dSerialized(afxM3d const m, afxUnit inStride, afxUnit outStride, afxUnit cnt, afxV4d const in[], afxV4d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(inStride);
    AFX_ASSERT(out);
    AFX_ASSERT(outStride);

    afxByte* inPtr = (afxByte*)in;
    afxByte* outPtr = (afxByte*)out;

    for (afxUnit i = 0; i < cnt; i++)
    {
        AfxV4dPostMultiplyM3d((void*)outPtr, m, (void*)inPtr);

        inPtr += inStride;
        outPtr += outStride;
    }
}

_AFXINL void AfxM4dPostMultiplyAtv4dSerialized(afxM4d const m, afxUnit inStride, afxUnit outStride, afxUnit cnt, afxV4d const in[], afxV4d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(inStride);
    AFX_ASSERT(out);
    AFX_ASSERT(outStride);

    afxByte* inPtr = (afxByte*)in;
    afxByte* outPtr = (afxByte*)out;

    for (afxUnit i = 0; i < cnt; i++)
    {
        AfxV4dPostMultiplyAtm4d((void*)outPtr, m, (void*)inPtr);

        inPtr += inStride;
        outPtr += outStride;
    }
}

// Assimilate

_AFXINL void AfxAssimilateLtm3d(afxM3d const ltm, afxM3d const iltm, afxUnit cnt, afxM3d const in[], afxM3d out[])
{
    // Should be compatible with void InPlaceSimilarityTransformScaleShear
    afxError err = { 0 };
    AFX_ASSERT(ltm);
    AFX_ASSERT(iltm);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxM3d tmp;
        AfxM3dMultiply(tmp, ltm, in[i]);
        AfxM3dMultiply(out[i], tmp, iltm);
    }
}

_AFXINL void AfxAssimilateAtm4d(afxM3d const ltm, afxM3d const iltm, afxV4d const atv, afxUnit cnt, afxM4d const in[], afxM4d out[])
{
    // Should be compatible with void InPlaceSimilarityTransform4x3
    afxError err = { 0 };
    AFX_ASSERT(atv);
    AFX_ASSERT(ltm);
    AFX_ASSERT(iltm);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxM3d ss;
        AfxM3dCopyM4d(ss, in[i]);
        afxV4d pos;
        AfxAssimilateAtv4d(ltm, atv, 1, &in[i][3], &pos);
        AfxAssimilateLtm3d(ltm, iltm, 1, &ss, &ss);
        AfxM4dCopyM3d(out[i], ss, pos);
    }
}

_AFXINL void AfxM4dApplyRigidMotion(afxM4d m, afxM4d const mm, afxV3d const rotation, afxV3d const translation)
{
    // AfxApplyRootMotionVectorsToMatrix

    /*
        It composes and applies a transformation matrix based on root motion (translation and rotation), 
        adjusting the provided model matrix (mm) to produce a new transformed matrix (m). 
        This is essential in animation systems when root bone motion needs to be applied to a character's world transform.
    */

    afxError err = { 0 };
    AFX_ASSERT(translation);
    AFX_ASSERT(rotation);
    AFX_ASSERT(mm); // model matrix
    AFX_ASSERT(m);
    afxM4d upd, tmp;
    afxReal rl = AfxV3dMag(rotation);

    if (rl <= 0.0)
    {
        AfxM4dTranslation(upd, translation);
        AfxM4dCopy(tmp, mm);
        AfxM4dMultiplyAffine(m, upd, tmp);
    }
    else
    {
        afxV3d scaledRot;
        AfxV3dScale(scaledRot, rotation, 1.f / rl);
        AfxM4dRotationFromAxis(tmp, scaledRot, rl);
        AfxM4dTranspose(upd, tmp);
        AfxV3dCopy(upd[3], translation);
        AfxM4dCopy(tmp, mm);
        AfxM4dMultiplyAffine(m, upd, tmp);
    }
}
