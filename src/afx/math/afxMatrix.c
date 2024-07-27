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

#include "qwadro/math/afxMatrix.h"
#include "qwadro/math/afxMathDefs.h"
#include "qwadro/math/afxVector.h"
#include "qwadro/math/afxPlane.h"
#include "qwadro/io/afxStream.h"

// Memory layout: row-major

_AFX afxM2d const AFX_M2D_ZERO =
{
    { AfxScalar(0), AfxScalar(0) },
    { AfxScalar(0), AfxScalar(0) },
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

////////////////////////////////////////////////////////////////////////////////
// Initialization methods                                                     //
////////////////////////////////////////////////////////////////////////////////

// Zero

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

// MakeIdentity

_AFXINL void AfxM2dReset(afxM2d m)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxM2dCopy(m, AFX_M2D_IDENTITY);
}

_AFXINL void AfxM3dReset(afxM3d m)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxM3dCopy(m, AFX_M3D_IDENTITY);
}

_AFXINL void AfxM4dReset(afxM4d m)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxM4dCopy(m, AFX_M4D_IDENTITY);
}

_AFXINL void AfxM4dEnsureLinear(afxM4d m)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    m[0][3] = 0.f;
    m[1][3] = 0.f;
    m[2][3] = 0.f;
    // in Qwadro, translation is transposed.
    AfxResetV4d(m[3]);
}

_AFXINL void AfxM4dEnsureAffine(afxM4d m)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    m[0][3] = 0.f;
    m[1][3] = 0.f;
    m[2][3] = 0.f;
    m[3][3] = 1.f;
}

// Set

_AFXINL void AfxM2dSet(afxM2d m, afxV2d const x, afxV2d const y)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(x);
    AfxAssert(y);

    m[0][0] = x[0];
    m[1][0] = x[1];

    m[0][1] = y[0];
    m[1][1] = y[1];
}

_AFXINL void AfxM3dSet(afxM3d m, afxV3d const x, afxV3d const y, afxV3d const z)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(x);
    AfxAssert(y);
    AfxAssert(z);

    // compatible with MatrixColumns3x3(m, x, y, z)

    m[0][0] = x[0];
    m[1][0] = x[1];
    m[2][0] = x[2];

    m[0][1] = y[0];
    m[1][1] = y[1];
    m[2][1] = y[2];

    m[0][2] = z[0];
    m[1][2] = z[1];
    m[2][2] = z[2];
}

_AFXINL void AfxM4dSet(afxM4d m, afxV4d const x, afxV4d const y, afxV4d const z, afxV4d const w)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(x);
    AfxAssert(y);
    AfxAssert(z);
    AfxAssert(w);

    m[0][0] = x[0];
    m[1][0] = x[1];
    m[2][0] = x[2];
    m[3][0] = x[3];

    m[0][1] = y[0];
    m[1][1] = y[1];
    m[2][1] = y[2];
    m[3][1] = y[3];

    m[0][2] = z[0];
    m[1][2] = z[1];
    m[2][2] = z[2];
    m[3][2] = z[3];

    // in Qwadro, translation is transposed.
    AfxV4dCopy(m[3], w);
}

_AFXINL void AfxM4dSetLinear(afxM4d m, afxV3d const x, afxV3d const y, afxV3d const z, afxV3d const w)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(x);
    AfxAssert(y);
    AfxAssert(z);
    AfxAssert(w);

    m[0][0] = x[0];
    m[1][0] = x[1];
    m[2][0] = x[2];
    m[3][0] = 0.0;

    m[0][1] = y[0];
    m[1][1] = y[1];
    m[2][1] = y[2];
    m[3][1] = 0.0;

    m[0][2] = z[0];
    m[1][2] = z[1];
    m[2][2] = z[2];
    m[3][2] = 0.0;

    // in Qwadro, translation is transposed.
    AfxResetV4d(m[3]);
}

_AFXINL void AfxM4dSetAffine(afxM4d m, afxV3d const x, afxV3d const y, afxV3d const z, afxV3d const w)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(x);
    AfxAssert(y);
    AfxAssert(z);
    AfxAssert(w);

    m[0][0] = x[0];
    m[1][0] = x[1];
    m[2][0] = x[2];
    m[3][0] = 0.0;

    m[0][1] = y[0];
    m[1][1] = y[1];
    m[2][1] = y[2];
    m[3][1] = 0.0;

    m[0][2] = z[0];
    m[1][2] = z[1];
    m[2][2] = z[2];
    m[3][2] = 0.0;

    // in Qwadro, translation is transposed.
    AfxV4dCopyAtv3d(m[3], w);
}

// SetTransposed

_AFXINL void AfxM2dSetTransposed(afxM2d m, afxV2d const x, afxV2d const y)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(x);
    AfxAssert(y);
    AfxV2dCopy(m[0], x);
    AfxV2dCopy(m[1], y);
}

_AFXINL void AfxM3dSetTransposed(afxM3d m, afxV3d const x, afxV3d const y, afxV3d const z)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(x);
    AfxAssert(y);
    AfxAssert(z);
    AfxV3dCopy(m[0], x);
    AfxV3dCopy(m[1], y);
    AfxV3dCopy(m[2], z);
}

_AFXINL void AfxM4dSetTransposed(afxM4d m, afxV4d const x, afxV4d const y, afxV4d const z, afxV4d const w)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(x);
    AfxAssert(y);
    AfxAssert(z);
    AfxAssert(w);
    AfxV4dCopy(m[0], x);
    AfxV4dCopy(m[1], y);
    AfxV4dCopy(m[2], z);
    AfxV4dCopy(m[3], w);
}

////////////////////////////////////////////////////////////////////////////////
// Transferance methods                                                       //
////////////////////////////////////////////////////////////////////////////////

// Copy

_AFXINL void AfxM2dCopy(afxM2d m, afxM2d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(in);
    AfxAssert(m);
    AfxAssertDiffSoft(m, in);

    m[0][0] = in[0][0];
    m[0][1] = in[0][1];

    m[1][0] = in[1][0];
    m[1][1] = in[1][1];
}

_AFXINL void AfxM3dCopy(afxM3d m, afxM3d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(in);
    AfxAssert(m);
    AfxAssertDiffSoft(m, in);

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
    afxError err = AFX_ERR_NONE;
    AfxAssert(in);
    AfxAssert(m);
    AfxAssertDiffSoft(m, in);

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
    afxError err = AFX_ERR_NONE;
    AfxAssert2(in, m);

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
    afxError err = AFX_ERR_NONE;
    AfxAssert2(in, m);

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
    afxError err = AFX_ERR_NONE;
    AfxAssert3(in, m, m != in);

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
    afxError err = AFX_ERR_NONE;
    AfxAssert3(in, m, m != in);

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
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(in);
    AfxAssertDiff(in, m);

    m[0][0] = in[0][0];
    m[0][1] = in[0][1];

    m[1][0] = in[1][0];
    m[1][1] = in[1][1];
}

_AFXINL void AfxM3dCopyM4d(afxM3d m, afxM4d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert3(m, in, (void*)m != (void*)in);

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
    afxError err = AFX_ERR_NONE;
    AfxAssert3(m, in, (void*)m != (void*)in);

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
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(in);
    AfxAssertDiff(m, in);

    m[0][0] = in[0][0];
    m[0][1] = in[0][1];
    m[0][2] = AfxScalar(0);

    m[1][0] = in[1][0];
    m[1][1] = in[1][1];
    m[1][2] = AfxScalar(0);

    m[2][0] = AfxScalar(0);
    m[2][1] = AfxScalar(0);
    m[2][2] = AfxScalar(1);
}

_AFXINL void AfxM4dCopyM2d(afxM4d m, afxM2d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(in);
    AfxAssertDiff(m, in);

    m[0][0] = in[0][0];
    m[0][1] = in[0][1];
    m[0][2] = AfxScalar(0);
    m[0][3] = AfxScalar(0);

    m[1][0] = in[1][0];
    m[1][1] = in[1][1];
    m[1][2] = AfxScalar(0);
    m[1][3] = AfxScalar(0);

    m[2][0] = AfxScalar(0);
    m[2][1] = AfxScalar(0);
    m[2][2] = AfxScalar(1);
    m[2][3] = AfxScalar(0);

    m[3][0] = AfxScalar(0);
    m[3][1] = AfxScalar(0);
    m[3][2] = AfxScalar(0);
    m[3][3] = AfxScalar(1);
}

_AFXINL void AfxM4dCopyM3d(afxM4d m, afxM3d const ltm, afxV4d const atv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(ltm);
    AfxAssert(atv);
    AfxAssertDiff(m, ltm);

    m[0][0] = ltm[0][0];
    m[0][1] = ltm[0][1];
    m[0][2] = ltm[0][2];
    m[0][3] = AfxScalar(0);

    m[1][0] = ltm[1][0];
    m[1][1] = ltm[1][1];
    m[1][2] = ltm[1][2];
    m[1][3] = AfxScalar(0);

    m[2][0] = ltm[2][0];
    m[2][1] = ltm[2][1];
    m[2][2] = ltm[2][2];
    m[2][3] = AfxScalar(0);

    m[3][0] = atv[0];
    m[3][1] = atv[1];
    m[3][2] = atv[2];
    m[3][3] = 1.f;
}

_AFXINL void AfxM4dTransposeM3d(afxM4d m, afxM3d const ltm, afxV4d const atv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(ltm);
    AfxAssert(m);
    AfxAssert(atv);

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
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(other);
    AfxAssert(m != other);
    afxM2d tmp;
    AfxM2dCopy(tmp, m);
    AfxM2dCopy(other, tmp);
    AfxM2dCopy(m, other);
}

_AFXINL void AfxM3dSwap(afxM3d m, afxM3d other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(other);
    AfxAssert(m != other);
    afxM3d tmp;
    AfxM3dCopy(tmp, m);
    AfxM3dCopy(other, tmp);
    AfxM3dCopy(m, other);
}

_AFXINL void AfxM4dSwap(afxM4d m, afxM4d other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(other);
    AfxAssert(m != other);
    afxM4d tmp;
    AfxM4dCopy(tmp, m);
    AfxM4dCopy(other, tmp);
    AfxM4dCopy(m, other);
}

// Transpose

_AFXINL void AfxM2dTranspose(afxM2d m, afxM2d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(in);
    AfxAssert(m);
    AfxAssert(m != in);

    m[0][0] = in[0][0];
    m[0][1] = in[1][0];

    m[1][0] = in[0][1];
    m[1][1] = in[1][1];
}

_AFXINL void AfxM3dTranspose(afxM3d m, afxM3d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(in);
    AfxAssert(m);
    AfxAssert(m != in);

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
    afxError err = AFX_ERR_NONE;
    AfxAssert(in);
    AfxAssert(m);
    AfxAssert(m != in);

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
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(a);
    AfxAssert(b);
    AfxV2dAdd(m[0], a[0], b[0]);
    AfxV2dAdd(m[1], a[1], b[1]);
}

_AFXINL void AfxM3dAdd(afxM3d m, afxM3d const a, afxM3d const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(a);
    AfxAssert(b);
    AfxV3dAdd(m[0], a[0], b[0]);
    AfxV3dAdd(m[1], a[1], b[1]);
    AfxV3dAdd(m[2], a[2], b[2]);
}

_AFXINL void AfxM4dAdd(afxM4d m, afxM4d const a, afxM4d const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(a);
    AfxAssert(b);
    AfxV4dAdd(m[0], a[0], b[0]);
    AfxV4dAdd(m[1], a[1], b[1]);
    AfxV4dAdd(m[2], a[2], b[2]);
    AfxV4dAdd(m[3], a[3], b[3]);
}

// Sub

_AFXINL void AfxM2dSub(afxM2d m, afxM2d const a, afxM2d const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(a);
    AfxAssert(b);
    AfxV2dSub(m[0], a[0], b[0]);
    AfxV2dSub(m[1], a[1], b[1]);
}

_AFXINL void AfxM3dSub(afxM3d m, afxM3d const a, afxM3d const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(a);
    AfxAssert(b);
    AfxV3dSub(m[0], a[0], b[0]);
    AfxV3dSub(m[1], a[1], b[1]);
    AfxV3dSub(m[2], a[2], b[2]);
}

_AFXINL void AfxM4dSub(afxM4d m, afxM4d const a, afxM4d const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(a);
    AfxAssert(b);
    AfxV4dSub(m[0], a[0], b[0]);
    AfxV4dSub(m[1], a[1], b[1]);
    AfxV4dSub(m[2], a[2], b[2]);
    AfxV4dSub(m[3], a[3], b[3]);
}

// Scale

_AFXINL void AfxM2dScale(afxM2d m, afxM2d const in, afxReal scale)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(in);
    AfxV2dScale(m[0], in[0], scale);
    AfxV2dScale(m[1], in[1], scale);
}

_AFXINL void AfxM3dScale(afxM3d m, afxM3d const in, afxReal scale)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(in);
    AfxV3dScale(m[0], in[0], scale);
    AfxV3dScale(m[1], in[1], scale);
    AfxV3dScale(m[2], in[2], scale);
}

_AFXINL void AfxM4dScale(afxM4d m, afxM4d const in, afxReal scale)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(in);
    AfxV4dScale(m[0], in[0], scale);
    AfxV4dScale(m[1], in[1], scale);
    AfxV4dScale(m[2], in[2], scale);
    AfxV4dScale(m[3], in[3], scale);
}

// AddScaled

_AFXINL void AfxM2dAddScaled(afxM2d m, afxM2d const a, afxM2d const b, afxReal scale)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(a);
    AfxAssert(b);
    AfxAddScaledV2d(m[0], a[0], b[0], scale);
    AfxAddScaledV2d(m[1], a[1], b[1], scale);
}

_AFXINL void AfxM3dAddScaled(afxM3d m, afxM3d const a, afxM3d const b, afxReal scale)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(a);
    AfxAssert(b);
    AfxAddScaledV3d(m[0], a[0], b[0], scale);
    AfxAddScaledV3d(m[1], a[1], b[1], scale);
    AfxAddScaledV3d(m[2], a[2], b[2], scale);
}

_AFXINL void AfxM4dAddScaled(afxM4d m, afxM4d const a, afxM4d const b, afxReal scale)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(a);
    AfxAssert(b);
    AfxAddScaledV4d(m[0], a[0], b[0], scale);
    AfxAddScaledV4d(m[1], a[1], b[1], scale);
    AfxAddScaledV4d(m[2], a[2], b[2], scale);
    AfxAddScaledV4d(m[3], a[3], b[3], scale);
}

// Combine

_AFXINL void AfxM2dCombine(afxM2d m, afxReal lambda1, afxM2d const a, afxReal lambda2, afxM2d const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(a);
    AfxAssert(b);

    m[0][0] = lambda1 * a[0][0] + lambda2 * b[0][0];
    m[0][1] = lambda1 * a[0][1] + lambda2 * b[0][1];

    m[1][0] = lambda1 * a[1][0] + lambda2 * b[1][0];
    m[1][1] = lambda1 * a[1][1] + lambda2 * b[1][1];    
}

_AFXINL void AfxM3dCombine(afxM3d m, afxReal lambda1, afxM3d const a, afxReal lambda2, afxM3d const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(a);
    AfxAssert(b);

    m[0][0] = lambda1 * a[0][0] + lambda2 * b[0][0];
    m[0][1] = lambda1 * a[0][1] + lambda2 * b[0][1];
    m[0][2] = lambda1 * a[0][2] + lambda2 * b[0][2];

    m[1][0] = lambda1 * a[1][0] + lambda2 * b[1][0];
    m[1][1] = lambda1 * a[1][1] + lambda2 * b[1][1];
    m[1][2] = lambda1 * a[1][2] + lambda2 * b[1][2];

    m[2][0] = lambda1 * a[2][0] + lambda2 * b[2][0];
    m[2][1] = lambda1 * a[2][1] + lambda2 * b[2][1];
    m[2][2] = lambda1 * a[2][2] + lambda2 * b[2][2];
}

_AFXINL void AfxM4dCombineLinear(afxM4d m, afxReal lambda1, afxM4d const a, afxReal lambda2, afxM4d const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(a);
    AfxAssert(b);

    m[0][0] = lambda1 * a[0][0] + lambda2 * b[0][0];
    m[0][1] = lambda1 * a[0][1] + lambda2 * b[0][1];
    m[0][2] = lambda1 * a[0][2] + lambda2 * b[0][2];
    m[0][3] = 0.f;

    m[1][0] = lambda1 * a[1][0] + lambda2 * b[1][0];
    m[1][1] = lambda1 * a[1][1] + lambda2 * b[1][1];
    m[1][2] = lambda1 * a[1][2] + lambda2 * b[1][2];
    m[1][3] = 0.f;

    m[2][0] = lambda1 * a[2][0] + lambda2 * b[2][0];
    m[2][1] = lambda1 * a[2][1] + lambda2 * b[2][1];
    m[2][2] = lambda1 * a[2][2] + lambda2 * b[2][2];
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
    afxError err = AFX_ERR_NONE;
    AfxAssert(in);
    AfxAssert(m);
    AfxAssert(in != m);

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
        AfxAssert(det != 0.0);
    }
    return det;
}

// Invert
// Memory layout: hybrid

_AFXINL afxReal AfxM4dInvert(afxM4d m, afxM4d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(in);
    AfxAssert(m);
    AfxAssert(in != m);

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

        for (afxNat i = 0; i < 4; i++)
            for (afxNat j = 0; j < 4; j++)
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
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    return  (m[0][0] * m[1][1]) - 
            (m[0][1] * m[1][0]);
}

_AFXINL afxReal AfxM3dDet(afxM3d const m)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    return  (m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1])) - 
            (m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0])) + 
            (m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]));
}

_AFXINL afxReal AfxM4dDet(afxM4d const m)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
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
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(scale);
    AfxAssertDiff(m, scale);
    AfxM3dReset(m);
    m[0][0] = scale[0];
    m[1][1] = scale[1];
    m[2][2] = scale[2];
}

_AFXINL void AfxM4dScaling(afxM4d m, afxV3d const scale)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(scale);
    AfxAssertDiff(m, scale);
    AfxM4dReset(m);
    m[0][0] = scale[0];
    m[1][1] = scale[1];
    m[2][2] = scale[2];
}

// MakeRotation

_AFXINL void AfxM3dRotationFromQuat(afxM3d m, afxQuat const q)
{
    // Inspired in AfxM4dRotationFromQuat

    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(q);
    AfxAssertDiff(m, q);

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

    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(q);
    AfxAssertDiff(m, q);

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

    afxError err = AFX_ERR_NONE;
    AfxAssert(m);

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
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(axis);
    AfxAssert(!AfxV3dIsZero(axis));
    AfxAssert(!AfxV3dIsInfinite(axis));

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
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(axis);
    AfxAssert(!AfxV3dIsZero(axis));
    AfxAssert(!AfxV3dIsInfinite(axis));

    afxReal cosTheta, sinTheta;
    AfxCosSinf(radians, &cosTheta, &sinTheta);
    afxReal cosValue = 1.0f - cosTheta;
    afxV3d axis2;
    AfxV3dNormalize(axis2, axis);

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
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(translation);
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

    afxNat i = 0;
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

_AFXINL void AfxM4dComposeTransformation(afxM4d m, afxV3d const scalOrigin, afxQuat const scalOrient, afxV3d const scaling, afxV3d const rotOrigin, afxQuat const rotQuat, afxV3d const translation)
{
    // Should be compatible with XMMATRIX XMMatrixTransformation(FXMVECTOR ScalingOrigin, FXMVECTOR ScalingOrientationQuaternion, FXMVECTOR Scaling, GXMVECTOR RotationOrigin, HXMVECTOR RotationQuaternion, HXMVECTOR Translation)

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

    afxV3d negScaOrigin;
    AfxV3dNeg(negScaOrigin, scalOrigin);

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
    AfxV3dAdd(t[3], t[3], scalOrigin);
    AfxV3dSub(t[3], t[3], rotOrigin);
    AfxM4dMultiply(m, t, rotM);
    AfxV3dAdd(m[3], m[3], rotOrigin);
    AfxV3dAdd(m[3], m[3], translation);
}

_AFXINL void AfxM4dComposeAffineTransformation(afxM4d m, afxV3d const scale, afxV3d const rotAxis, afxQuat const rot, afxV3d const translation)
{
    // Should be compatible with XMMATRIX XMMatrixAffineTransformation(FXMVECTOR Scaling, FXMVECTOR RotationOrigin, FXMVECTOR RotationQuaternion, GXMVECTOR Translation)

    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(scale);
    AfxAssert(rot);
    AfxAssert(translation);

    // M = MScaling * Inverse(MRotationOrigin) * MRotation * MRotationOrigin * MTranslation;

    afxM4d sm, rm, c;
    AfxM4dReset(sm);
    AfxM4dScaling(sm, scale);
    AfxM4dRotationFromQuat(rm, rot);
    AfxV3dSub(sm[3], sm[3], rotAxis);
    AfxM4dMultiply(m, sm, rm);
    AfxV3dAdd(m[3], m[3], rotAxis);
    AfxV3dAdd(m[3], m[3], translation);
}

////////////////////////////////////////////////////////////////////////////////
// MATRIX TRANSFORMATION METHODS                                              //
////////////////////////////////////////////////////////////////////////////////

// In post-multiplication, each component of the result, at in given row R and column C, is computed by taking the dot product of the left matrix’s row R by the right matrix’s column C.

// MultiplyTransposed

_AFXINL void AfxM2dMultiply(afxM2d m, afxM2d const in, afxM2d const by)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(in);
    AfxAssert(by);
    AfxAssert(in != by);
    AfxAssert(by != m);

    // Cut off in[][col] * by[row][] to visualize it in your mind
    // m[row<in>][col<by>] = in[row][] * by[][i]

    m[0][0] = (in[0][0] * by[0][0]) + (in[0][1] * by[1][0]);
    m[0][1] = (in[0][0] * by[0][1]) + (in[0][1] * by[1][1]);

    m[1][0] = (in[1][0] * by[0][0]) + (in[1][1] * by[1][0]);
    m[1][1] = (in[1][0] * by[0][1]) + (in[1][1] * by[1][1]);
}

_AFXINL void AfxM2dMultiplyTransposed(afxM2d m, afxM2d const in, afxM2d const by)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(in);
    AfxAssert(by);
    AfxAssert(in != by);
    AfxAssert(by != m);

    // Cut off in[][col] * by[row][] to visualize it in your mind
    // m[col<by>][row<in>] = in[i][] * by[][col]

    m[0][0] = (in[0][0] * by[0][0]) + (in[0][1] * by[1][0]);
    m[0][1] = (in[1][0] * by[0][0]) + (in[1][1] * by[1][0]);

    m[1][0] = (in[0][0] * by[0][1]) + (in[0][1] * by[1][1]);
    m[1][1] = (in[1][0] * by[0][1]) + (in[1][1] * by[1][1]);
}

_AFXINL void AfxM3dMultiply(afxM3d m, afxM3d const in, afxM3d const by)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(in);
    AfxAssert(by);
    AfxAssert(in != by);
    AfxAssert(by != m);

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
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(in);
    AfxAssert(by);
    AfxAssert(in != by);
    AfxAssert(by != m);

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
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(in);
    AfxAssert(by);
    AfxAssert(in != by);
    AfxAssert(by != m);

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
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(in);
    AfxAssert(by);
    AfxAssert(in != by);
    AfxAssert(by != m);

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
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(in);
    AfxAssert(by);
    AfxAssert(in != by);
    AfxAssert(by != m);

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
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(in);
    AfxAssert(by);
    AfxAssert(in != by);
    AfxAssert(by != m);

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
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(in);
    AfxAssert(by);
    AfxAssert(in != by);
    AfxAssert(by != m);

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
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(in);
    AfxAssert(by);
    AfxAssert(in != by);
    AfxAssert(by != m);

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
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(in);
    AfxAssert(by);
    AfxAssert(in != by);
    AfxAssert(by != m);

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
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(in);
    AfxAssert(by);
    AfxAssert(in != by);
    AfxAssert(by != m);

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

_AFXINL void AfxM2dPostMultiplyV2d(afxM2d const m, afxNat cnt, afxV2d const in[], afxV2d out[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
        AfxV2dPostMultiplyM2d(out[i], m, in[i]);
}

_AFXINL void AfxM3dPostMultiplyV3d(afxM3d const m, afxNat cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
        AfxV3dPostMultiplyM3d(out[i], m, in[i]);
}

_AFXINL void AfxM4dPostMultiplyV4d(afxM4d const m, afxNat cnt, afxV4d const in[], afxV4d out[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
        AfxV4dPostMultiplyM4d(out[i], m, in[i]);
}

_AFXINL void AfxM4dPostMultiplyAtv3d(afxM4d const m, afxNat cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
        AfxV3dPostMultiplyAtm4d(out[i], m, in[i]);
}

_AFXINL void AfxM3dPostMultiplyLtv4d(afxM3d const m, afxNat cnt, afxV4d const in[], afxV4d out[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
        AfxV4dPostMultiplyM3d(out[i], m, in[i]);
}

_AFXINL void AfxM4dPostMultiplyAtv4d(afxM4d const m, afxNat cnt, afxV4d const in[], afxV4d out[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
        AfxV4dPostMultiplyAtm4d(out[i], m, in[i]);
}

// PreMultiply

_AFXINL void AfxM2dPreMultiplyV2d(afxM2d const m, afxNat cnt, afxV2d const in[], afxV2d out[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
        AfxV2dPreMultiplyM2d(out[i], in[i], m);
}

_AFXINL void AfxM3dPreMultiplyV3d(afxM3d const m, afxNat cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
        AfxV3dPreMultiplyM3d(out[i], in[i], m);
}

_AFXINL void AfxM4dPreMultiplyV4d(afxM4d const m, afxNat cnt, afxV4d const in[], afxV4d out[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
        AfxV4dPreMultiplyM4d(out[i], in[i], m);
}

_AFXINL void AfxM4dPreMultiplyAtv3d(afxM4d const m, afxNat cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
        AfxV3dPreMultiplyAtm4d(out[i], in[i], m);
}

_AFXINL void AfxM3dPreMultiplyLtv4d(afxM3d const m, afxNat cnt, afxV4d const in[], afxV4d out[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
        AfxV4dPreMultiplyM3d(out[i], in[i], m);
}

_AFXINL void AfxM4dPreMultiplyAtv4d(afxM4d const m, afxNat cnt, afxV4d const in[], afxV4d out[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
        AfxV4dPreMultiplyAtm4d(out[i], in[i], m);
}

// TransformSerialized

_AFXINL void AfxM2dPostMultiplyV2dSerialized(afxM2d const m, afxNat inStride, afxNat outStride, afxNat cnt, afxV2d const in[], afxV2d out[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(inStride);
    AfxAssert(out);
    AfxAssert(outStride);

    afxByte* inPtr = (afxByte*)in;
    afxByte* outPtr = (afxByte*)out;

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxV2dPostMultiplyM2d((void*)outPtr, m, (void*)inPtr);

        inPtr += inStride;
        outPtr += outStride;
    }
}

_AFXINL void AfxM3dPostMultiplyV3dSerialized(afxM3d const m, afxNat inStride, afxNat outStride, afxNat cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(inStride);
    AfxAssert(out);
    AfxAssert(outStride);

    afxByte* inPtr = (afxByte*)in;
    afxByte* outPtr = (afxByte*)out;

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxV3dPostMultiplyM3d((void*)outPtr, m, (void*)inPtr);

        inPtr += inStride;
        outPtr += outStride;
    }
}

_AFXINL void AfxM4dPostMultiplyV4dSerialized(afxM4d const m, afxNat inStride, afxNat outStride, afxNat cnt, afxV4d const in[], afxV4d out[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(inStride);
    AfxAssert(out);
    AfxAssert(outStride);

    afxByte* inPtr = (afxByte*)in;
    afxByte* outPtr = (afxByte*)out;

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxV4dPostMultiplyM4d((void*)outPtr, m, (void*)inPtr);

        inPtr += inStride;
        outPtr += outStride;
    }
}

_AFXINL void AfxM4dPostMultiplyAtv3dSerialized(afxM4d const m, afxNat inStride, afxNat outStride, afxNat cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(inStride);
    AfxAssert(out);
    AfxAssert(outStride);

    afxByte* inPtr = (afxByte*)in;
    afxByte* outPtr = (afxByte*)out;

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxV3dPostMultiplyAtm4d((void*)outPtr, m, (void*)inPtr);

        inPtr += inStride;
        outPtr += outStride;
    }
}

_AFXINL void AfxM3dPostMultiplyLtv4dSerialized(afxM3d const m, afxNat inStride, afxNat outStride, afxNat cnt, afxV4d const in[], afxV4d out[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(inStride);
    AfxAssert(out);
    AfxAssert(outStride);

    afxByte* inPtr = (afxByte*)in;
    afxByte* outPtr = (afxByte*)out;

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxV4dPostMultiplyM3d((void*)outPtr, m, (void*)inPtr);

        inPtr += inStride;
        outPtr += outStride;
    }
}

_AFXINL void AfxM4dPostMultiplyAtv4dSerialized(afxM4d const m, afxNat inStride, afxNat outStride, afxNat cnt, afxV4d const in[], afxV4d out[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(inStride);
    AfxAssert(out);
    AfxAssert(outStride);

    afxByte* inPtr = (afxByte*)in;
    afxByte* outPtr = (afxByte*)out;

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxV4dPostMultiplyAtm4d((void*)outPtr, m, (void*)inPtr);

        inPtr += inStride;
        outPtr += outStride;
    }
}

// Assimilate

_AFXINL void AfxAssimilateLtm3d(afxM3d const ltm, afxM3d const iltm, afxNat cnt, afxM3d const in[], afxM3d out[])
{
    // Should be compatible with void InPlaceSimilarityTransformScaleShear(const float *Linear3x3, const float *InverseLinear3x3, float *ScaleShear3x3)
    afxError err = NIL;
    AfxAssert(ltm);
    AfxAssert(iltm);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxM3d tmp;
        AfxM3dMultiply(tmp, ltm, in[i]);
        AfxM3dMultiply(out[i], tmp, iltm);
    }
}

_AFXINL void AfxAssimilateAtm4d(afxM3d const ltm, afxM3d const iltm, afxV4d const atv, afxNat cnt, afxM4d const in[], afxM4d out[])
{
    // Should be compatible with void InPlaceSimilarityTransform4x3(const float *Affine3, const float *Linear3x3, const float *InverseLinear3x3, float *ResultInit)
    afxError err = NIL;
    AfxAssert(atv);
    AfxAssert(ltm);
    AfxAssert(iltm);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxM3d ss;
        AfxM3dCopyM4d(ss, in[i]);
        afxV4d pos;
        AfxAssimilateAtv4d(ltm, atv, 1, &in[i][3], &pos);
        AfxAssimilateLtm3d(ltm, iltm, 1, &ss, &ss);
        AfxM4dCopyM3d(out[i], ss, pos);
    }
}

_AFX void AfxApplyRootMotionVectorsToMatrix(afxV3d const translation, afxV3d const rotation, afxM4d const mm, afxM4d m)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(translation);
    AfxAssert(rotation);
    AfxAssert(mm); // model matrix
    AfxAssert(m);
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
