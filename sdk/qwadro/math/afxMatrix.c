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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

#include "qwadro/math/afxMatrix.h"
#include "qwadro/math/afxMathDefs.h"
#include "qwadro/math/afxReal.h"
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

_AFXINL void AfxZeroM2d(afxM2d m)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxCopyM2d(m, AFX_M2D_ZERO);
}

_AFXINL void AfxZeroM3d(afxM3d m)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxCopyM3d(m, AFX_M3D_ZERO);
}

_AFXINL void AfxZeroM4d(afxM4d m)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxCopyM4d(m, AFX_M4D_ZERO);
}

// MakeIdentity

_AFXINL void AfxResetM2d(afxM2d m)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxCopyM2d(m, AFX_M2D_IDENTITY);
}

_AFXINL void AfxResetM3d(afxM3d m)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxCopyM3d(m, AFX_M3D_IDENTITY);
}

_AFXINL void AfxResetM4d(afxM4d m)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxCopyM4d(m, AFX_M4D_IDENTITY);
}

_AFXINL void AfxEnsureLtm4d(afxM4d m)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    m[0][3] = 0.f;
    m[1][3] = 0.f;
    m[2][3] = 0.f;
    m[3][0] = 0.f;
    m[3][1] = 0.f;
    m[3][2] = 0.f;
    m[3][3] = 1.f;
}

_AFXINL void AfxEnsureAtm4d(afxM4d m)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    m[0][3] = 0.f;
    m[1][3] = 0.f;
    m[2][3] = 0.f;
    m[3][3] = 1.f;
}

// Set

_AFXINL void AfxSetM2d(afxM2d m, afxV2d const x, afxV2d const y)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(x);
    AfxAssert(y);
    AfxCopyV2d(m[0], x);
    AfxCopyV2d(m[1], y);
}

_AFXINL void AfxSetM3d(afxM3d m, afxV3d const x, afxV3d const y, afxV3d const z)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(x);
    AfxAssert(y);
    AfxAssert(z);
    AfxCopyV3d(m[0], x);
    AfxCopyV3d(m[1], y);
    AfxCopyV3d(m[2], z);
}

_AFXINL void AfxSetM4d(afxM4d m, afxV4d const x, afxV4d const y, afxV4d const z, afxV4d const w)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(x);
    AfxAssert(y);
    AfxAssert(z);
    AfxAssert(w);
    AfxCopyV4d(m[0], x);
    AfxCopyV4d(m[1], y);
    AfxCopyV4d(m[2], z);
    AfxCopyV4d(m[3], w);
}

// SetTransposed

_AFXINL void AfxSetM2dTransposed(afxM2d m, afxV2d const x, afxV2d const y)
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

_AFXINL void AfxSetM3dTransposed(afxM3d m, afxV3d const x, afxV3d const y, afxV3d const z)
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

_AFXINL void AfxSetM4dTransposed(afxM4d m, afxV4d const x, afxV4d const y, afxV4d const z, afxV4d const w)
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

    m[0][3] = w[0];
    m[1][3] = w[1];
    m[2][3] = w[2];
    m[3][3] = w[3];
}

////////////////////////////////////////////////////////////////////////////////
// Transferance methods                                                       //
////////////////////////////////////////////////////////////////////////////////

// Copy

_AFXINL void AfxCopyM2d(afxM2d m, afxM2d const in)
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

_AFXINL void AfxCopyM3d(afxM3d m, afxM3d const in)
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

_AFXINL void AfxCopyM4d(afxM4d m, afxM4d const in)
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

_AFXINL void AfxPickLtm4d(afxM4d const m, afxM4d ltm)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(ltm);
    AfxAssertDiffSoft(ltm, m);

    ltm[0][0] = m[0][0];
    ltm[0][1] = m[0][1];
    ltm[0][2] = m[0][2];
    ltm[0][3] = 0.f;

    ltm[1][0] = m[1][0];
    ltm[1][1] = m[1][1];
    ltm[1][2] = m[1][2];
    ltm[1][3] = 0.f;

    ltm[2][0] = m[2][0];
    ltm[2][1] = m[2][1];
    ltm[2][2] = m[2][2];
    ltm[2][3] = 0.f;

    ltm[3][0] = 0.f;
    ltm[3][1] = 0.f;
    ltm[3][2] = 0.f;
    ltm[3][3] = 1.f;
}

_AFXINL void AfxPickAtm4d(afxM4d const m, afxM4d atm)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(atm);
    AfxAssertDiffSoft(atm, m);

    atm[0][0] = m[0][0];
    atm[0][1] = m[0][1];
    atm[0][2] = m[0][2];
    atm[0][3] = 0.f;

    atm[1][0] = m[1][0];
    atm[1][1] = m[1][1];
    atm[1][2] = m[1][2];
    atm[1][3] = 0.f;

    atm[2][0] = m[2][0];
    atm[2][1] = m[2][1];
    atm[2][2] = m[2][2];
    atm[2][3] = 0.f;

    atm[3][0] = m[3][0];
    atm[3][1] = m[3][1];
    atm[3][2] = m[3][2];
    atm[3][3] = 1.f;
}

_AFXINL void AfxPickLtm4dTransposed(afxM4d const m, afxM4d t)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(t);

    t[0][0] = m[0][0];
    t[0][1] = m[1][0];
    t[0][2] = m[2][0];
    t[0][3] = 0.f;

    t[1][0] = m[0][1];
    t[1][1] = m[1][1];
    t[1][2] = m[2][1];
    t[1][3] = 0.f;

    t[2][0] = m[0][2];
    t[2][1] = m[1][2];
    t[2][2] = m[2][2];
    t[2][3] = 0.f;

    t[3][0] = 0.f;
    t[3][1] = 0.f;
    t[3][2] = 0.f;
    t[3][3] = 1.f;
}

_AFXINL void AfxPickAtm4dTransposed(afxM4d const m, afxM4d t)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(t);

    t[0][0] = m[0][0];
    t[0][1] = m[1][0];
    t[0][2] = m[2][0];
    t[0][3] = 0.f;

    t[1][0] = m[0][1];
    t[1][1] = m[1][1];
    t[1][2] = m[2][1];
    t[1][3] = 0.f;

    t[2][0] = m[0][2];
    t[2][1] = m[1][2];
    t[2][2] = m[2][2];
    t[2][3] = 0.f;

    t[3][0] = m[0][3];
    t[3][1] = m[1][3];
    t[3][2] = m[2][3];
    t[3][3] = 1.f;
}

//

_AFXINL void AfxPickM2d(afxM3d const m, afxM2d sub)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(sub);
    AfxAssert(m);
    AfxAssertDiff(m, sub);

    sub[0][0] = m[0][0];
    sub[0][1] = m[0][1];

    sub[1][0] = m[1][0];
    sub[1][1] = m[1][1];
}

_AFXINL void AfxPickM2d2(afxM4d const m, afxM2d sup, afxM2d inf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(sup);
    AfxAssert(inf);
    AfxAssert(m);
    AfxAssertDiff(sup, m);
    AfxAssertDiff(inf, m);

    sup[0][0] = m[0][0];
    sup[0][1] = m[0][1];

    sup[1][0] = m[1][0];
    sup[1][1] = m[1][1];

    inf[2][0] = m[2][0];
    inf[2][1] = m[2][1];

    inf[3][0] = m[3][0];
    inf[3][1] = m[3][1];
}

_AFXINL void AfxPickM3d(afxM4d const m, afxM3d sub)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(sub);
    AfxAssert(m);
    AfxAssertDiff(sub, m);

    sub[0][0] = m[0][0];
    sub[0][1] = m[0][1];
    sub[0][2] = m[0][2];

    sub[1][0] = m[1][0];
    sub[1][1] = m[1][1];
    sub[1][2] = m[1][2];

    sub[2][0] = m[2][0];
    sub[2][1] = m[2][1];
    sub[2][2] = m[2][2];
}

_AFXINL void AfxPickM3dTransposed(afxM4d const m, afxM3d t)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(t);
    AfxAssert(m);

    t[0][0] = m[0][0];
    t[0][1] = m[1][0];
    t[0][2] = m[2][0];

    t[1][0] = m[0][1];
    t[1][1] = m[1][1];
    t[1][2] = m[2][1];

    t[2][0] = m[0][2];
    t[2][1] = m[1][2];
    t[2][2] = m[2][2];
}

_AFXINL void AfxMakeM3dFromM2d(afxM3d m, afxM2d const in)
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

_AFXINL void AfxMakeM4dFromM2d(afxM4d m, afxM2d const in)
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

_AFXINL void AfxMakeM4dFromM3d(afxM4d m, afxM3d const in, afxV4d const translation)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(in);
    AfxAssert(translation);
    AfxAssertDiff(m, in);

    m[0][0] = in[0][0];
    m[0][1] = in[0][1];
    m[0][2] = in[0][2];
    m[0][3] = AfxScalar(0);

    m[1][0] = in[1][0];
    m[1][1] = in[1][1];
    m[1][2] = in[1][2];
    m[1][3] = AfxScalar(0);

    m[2][0] = in[2][0];
    m[2][1] = in[2][1];
    m[2][2] = in[2][2];
    m[2][3] = AfxScalar(0);

    m[3][0] = translation[0];
    m[3][1] = translation[1];
    m[3][2] = translation[2];
    m[3][3] = translation[3];
}

// Swap

_AFXINL void AfxSwapM2d(afxM2d m, afxM2d other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(other);
    AfxAssert(m != other);
    afxM2d tmp;
    AfxCopyM2d(tmp, m);
    AfxCopyM2d(other, tmp);
    AfxCopyM2d(m, other);
}

_AFXINL void AfxSwapM3d(afxM3d m, afxM3d other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(other);
    AfxAssert(m != other);
    afxM3d tmp;
    AfxCopyM3d(tmp, m);
    AfxCopyM3d(other, tmp);
    AfxCopyM3d(m, other);
}

_AFXINL void AfxSwapM4d(afxM4d m, afxM4d other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(other);
    AfxAssert(m != other);
    afxM4d tmp;
    AfxCopyM4d(tmp, m);
    AfxCopyM4d(other, tmp);
    AfxCopyM4d(m, other);
}

// Transpose

_AFXINL void AfxTransposeM2d(afxM2d const m, afxM2d t)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(t);
    AfxAssert(t != m);

    t[0][0] = m[0][0];
    t[0][1] = m[1][0];

    t[1][0] = m[0][1];
    t[1][1] = m[1][1];
}

_AFXINL void AfxTransposeM3d(afxM3d const m, afxM3d t)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(t);
    AfxAssert(t != m);

    t[0][0] = m[0][0];
    t[0][1] = m[1][0];
    t[0][2] = m[2][0];

    t[1][0] = m[0][1];
    t[1][1] = m[1][1];
    t[1][2] = m[2][1];

    t[2][0] = m[0][2];
    t[2][1] = m[1][2];
    t[2][2] = m[2][2];
}

_AFXINL void AfxTransposeM4d(afxM4d const m, afxM4d t)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(t);
    AfxAssert(t != m);

    t[0][0] = m[0][0];
    t[0][1] = m[1][0];
    t[0][2] = m[2][0];
    t[0][3] = m[3][0];

    t[1][0] = m[0][1];
    t[1][1] = m[1][1];
    t[1][2] = m[2][1];
    t[1][3] = m[3][1];

    t[2][0] = m[0][2];
    t[2][1] = m[1][2];
    t[2][2] = m[2][2];
    t[2][3] = m[3][2];

    t[3][0] = m[0][3];
    t[3][1] = m[1][3];
    t[3][2] = m[2][3];
    t[3][3] = m[3][3];
}

_AFXINL void AfxMakeM4dFromM3dTransposed(afxM4d m, afxM3d const in, afxV4d const translation)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(in);
    AfxAssert(m);
    AfxAssert(translation);

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

    m[3][0] = translation[0];
    m[3][1] = translation[1];
    m[3][2] = translation[2];
    m[3][3] = translation[3];
}

////////////////////////////////////////////////////////////////////////////////
// Arithmetic methods                                                         //
////////////////////////////////////////////////////////////////////////////////

// Add

_AFXINL void AfxAddM2d(afxM2d m, afxM2d const a, afxM2d const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(a);
    AfxAssert(b);
    AfxAddV2d(m[0], a[0], b[0]);
    AfxAddV2d(m[1], a[1], b[1]);
}

_AFXINL void AfxAddM3d(afxM3d m, afxM3d const a, afxM3d const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(a);
    AfxAssert(b);
    AfxAddV3d(m[0], a[0], b[0]);
    AfxAddV3d(m[1], a[1], b[1]);
    AfxAddV3d(m[2], a[2], b[2]);
}

_AFXINL void AfxAddM4d(afxM4d m, afxM4d const a, afxM4d const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(a);
    AfxAssert(b);
    AfxAddV4d(m[0], a[0], b[0]);
    AfxAddV4d(m[1], a[1], b[1]);
    AfxAddV4d(m[2], a[2], b[2]);
    AfxAddV4d(m[3], a[3], b[3]);
}

// Sub

_AFXINL void AfxSubM2d(afxM2d m, afxM2d const a, afxM2d const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(a);
    AfxAssert(b);
    AfxSubV2d(m[0], a[0], b[0]);
    AfxSubV2d(m[1], a[1], b[1]);
}

_AFXINL void AfxSubM3d(afxM3d m, afxM3d const a, afxM3d const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(a);
    AfxAssert(b);
    AfxSubV3d(m[0], a[0], b[0]);
    AfxSubV3d(m[1], a[1], b[1]);
    AfxSubV3d(m[2], a[2], b[2]);
}

_AFXINL void AfxSubM4d(afxM4d m, afxM4d const a, afxM4d const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(a);
    AfxAssert(b);
    AfxSubV4d(m[0], a[0], b[0]);
    AfxSubV4d(m[1], a[1], b[1]);
    AfxSubV4d(m[2], a[2], b[2]);
    AfxSubV4d(m[3], a[3], b[3]);
}

// Scale

_AFXINL void AfxScaleM2d(afxReal scale, afxM2d const in, afxM2d m)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(in);
    AfxScaleV2d(m[0], in[0], scale);
    AfxScaleV2d(m[1], in[1], scale);
}

_AFXINL void AfxScaleM3d(afxReal scale, afxM3d const in, afxM3d m)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(in);
    AfxScaleV3d(m[0], in[0], scale);
    AfxScaleV3d(m[1], in[1], scale);
    AfxScaleV3d(m[2], in[2], scale);
}

_AFXINL void AfxScaleM4d(afxReal scale, afxM4d const in, afxM4d m)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(in);
    AfxScaleV4d(m[0], in[0], scale);
    AfxScaleV4d(m[1], in[1], scale);
    AfxScaleV4d(m[2], in[2], scale);
    AfxScaleV4d(m[3], in[3], scale);
}

// Scad

_AFXINL void AfxScadM2d(afxM2d m, afxM2d const in, afxReal scale)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(in);
    AfxScadV2d(m[0], in[0], scale);
    AfxScadV2d(m[1], in[1], scale);
}

_AFXINL void AfxScadM3d(afxM3d m, afxM3d const in, afxReal scale)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(in);
    AfxScadV3d(m[0], in[0], scale);
    AfxScadV3d(m[1], in[1], scale);
    AfxScadV3d(m[2], in[2], scale);
}

_AFXINL void AfxScadM4d(afxM4d m, afxM4d const in, afxReal scale)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(in);
    AfxScadV4d(m[0], in[0], scale);
    AfxScadV4d(m[1], in[1], scale);
    AfxScadV4d(m[2], in[2], scale);
    AfxScadV4d(m[3], in[3], scale);
}

// Combine

_AFXINL void AfxCombineM2d(afxM2d m, afxReal lambda1, afxM2d const a, afxReal lambda2, afxM2d const b)
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

_AFXINL void AfxCombineM3d(afxM3d m, afxReal lambda1, afxM3d const a, afxReal lambda2, afxM3d const b)
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

_AFXINL void AfxCombineLtm4d(afxM4d m, afxReal lambda1, afxM4d const a, afxReal lambda2, afxM4d const b)
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

_AFXINL afxReal AfxInvertM3d(afxM3d const m, afxM3d im)
{
    // Inspired on void MatrixInvert3x3(float *DestInit, const float *SourceInit)
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(im);
    AfxAssert(m != im);

    afxReal det = (m[1][1] * m[2][2] - m[2][1] * m[1][2]) * m[0][0] - (m[2][2] * m[1][0] - m[2][0] * m[1][2]) * m[0][1] + (m[2][1] * m[1][0] - m[2][0] * m[1][1]) * m[0][2];

    if (det != 0.0)
    {
        det = 1.0 / det;
        im[0][0] =  ( m[1][1] * m[2][2] - m[2][1] * m[1][2]) * det;
        im[1][0] = -((m[2][2] * m[1][0] - m[2][0] * m[1][2]) * det);
        im[2][0] =  ( m[2][1] * m[1][0] - m[2][0] * m[1][1]) * det;

        im[0][1] = -((m[0][1] * m[2][2] - m[2][1] * m[0][2]) * det);
        im[1][1] =  ( m[2][2] * m[0][0] - m[2][0] * m[0][2]) * det;
        im[2][1] = -((m[2][1] * m[0][0] - m[2][0] * m[0][1]) * det);

        im[0][2] =  ( m[0][1] * m[1][2] - m[1][1] * m[0][2]) * det;
        im[1][2] = -((m[1][2] * m[0][0] - m[0][2] * m[1][0]) * det);
        im[2][2] =  ( m[1][1] * m[0][0] - m[0][1] * m[1][0]) * det;
    }
    else
    {
        AfxCopyM3d(im, m);
        AfxAssert(det != 0.0);
    }
    return det;
}

// Invert
// Memory layout: hybrid

_AFXINL afxReal AfxInvertM4d(afxM4d const m, afxM4d im)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(im);
    AfxAssert(m != im);

    // From StackOverflow, by wangzhe, at https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix

    im[0][0] =  m[1][1] * m[2][2] * m[3][3] - m[1][1] * m[2][3] * m[3][2] - m[2][1] * m[1][2] * m[3][3] + m[2][1] * m[1][3] * m[3][2] + m[3][1] * m[1][2] * m[2][3] - m[3][1] * m[1][3] * m[2][2];
    im[1][0] = -m[1][0] * m[2][2] * m[3][3] + m[1][0] * m[2][3] * m[3][2] + m[2][0] * m[1][2] * m[3][3] - m[2][0] * m[1][3] * m[3][2] - m[3][0] * m[1][2] * m[2][3] + m[3][0] * m[1][3] * m[2][2];
    im[2][0] =  m[1][0] * m[2][1] * m[3][3] - m[1][0] * m[2][3] * m[3][1] - m[2][0] * m[1][1] * m[3][3] + m[2][0] * m[1][3] * m[3][1] + m[3][0] * m[1][1] * m[2][3] - m[3][0] * m[1][3] * m[2][1];
    im[3][0] = -m[1][0] * m[2][1] * m[3][2] + m[1][0] * m[2][2] * m[3][1] + m[2][0] * m[1][1] * m[3][2] - m[2][0] * m[1][2] * m[3][1] - m[3][0] * m[1][1] * m[2][2] + m[3][0] * m[1][2] * m[2][1];
    
    im[0][1] = -m[0][1] * m[2][2] * m[3][3] + m[0][1] * m[2][3] * m[3][2] + m[2][1] * m[0][2] * m[3][3] - m[2][1] * m[0][3] * m[3][2] - m[3][1] * m[0][2] * m[2][3] + m[3][1] * m[0][3] * m[2][2];
    im[1][1] =  m[0][0] * m[2][2] * m[3][3] - m[0][0] * m[2][3] * m[3][2] - m[2][0] * m[0][2] * m[3][3] + m[2][0] * m[0][3] * m[3][2] + m[3][0] * m[0][2] * m[2][3] - m[3][0] * m[0][3] * m[2][2];
    im[2][1] = -m[0][0] * m[2][1] * m[3][3] + m[0][0] * m[2][3] * m[3][1] + m[2][0] * m[0][1] * m[3][3] - m[2][0] * m[0][3] * m[3][1] - m[3][0] * m[0][1] * m[2][3] + m[3][0] * m[0][3] * m[2][1];
    im[3][1] =  m[0][0] * m[2][1] * m[3][2] - m[0][0] * m[2][2] * m[3][1] - m[2][0] * m[0][1] * m[3][2] + m[2][0] * m[0][2] * m[3][1] + m[3][0] * m[0][1] * m[2][2] - m[3][0] * m[0][2] * m[2][1];
    
    im[0][2] =  m[0][1] * m[1][2] * m[3][3] - m[0][1] * m[1][3] * m[3][2] - m[1][1] * m[0][2] * m[3][3] + m[1][1] * m[0][3] * m[3][2] + m[3][1] * m[0][2] * m[1][3] - m[3][1] * m[0][3] * m[1][2];
    im[1][2] = -m[0][0] * m[1][2] * m[3][3] + m[0][0] * m[1][3] * m[3][2] + m[1][0] * m[0][2] * m[3][3] - m[1][0] * m[0][3] * m[3][2] - m[3][0] * m[0][2] * m[1][3] + m[3][0] * m[0][3] * m[1][2];
    im[2][2] =  m[0][0] * m[1][1] * m[3][3] - m[0][0] * m[1][3] * m[3][1] - m[1][0] * m[0][1] * m[3][3] + m[1][0] * m[0][3] * m[3][1] + m[3][0] * m[0][1] * m[1][3] - m[3][0] * m[0][3] * m[1][1];
    im[3][2] = -m[0][0] * m[1][1] * m[3][2] + m[0][0] * m[1][2] * m[3][1] + m[1][0] * m[0][1] * m[3][2] - m[1][0] * m[0][2] * m[3][1] - m[3][0] * m[0][1] * m[1][2] + m[3][0] * m[0][2] * m[1][1];
    
    im[0][3] = -m[0][1] * m[1][2] * m[2][3] + m[0][1] * m[1][3] * m[2][2] + m[1][1] * m[0][2] * m[2][3] - m[1][1] * m[0][3] * m[2][2] - m[2][1] * m[0][2] * m[1][3] + m[2][1] * m[0][3] * m[1][2];
    im[1][3] =  m[0][0] * m[1][2] * m[2][3] - m[0][0] * m[1][3] * m[2][2] - m[1][0] * m[0][2] * m[2][3] + m[1][0] * m[0][3] * m[2][2] + m[2][0] * m[0][2] * m[1][3] - m[2][0] * m[0][3] * m[1][2];
    im[2][3] = -m[0][0] * m[1][1] * m[2][3] + m[0][0] * m[1][3] * m[2][1] + m[1][0] * m[0][1] * m[2][3] - m[1][0] * m[0][3] * m[2][1] - m[2][0] * m[0][1] * m[1][3] + m[2][0] * m[0][3] * m[1][1];
    im[3][3] =  m[0][0] * m[1][1] * m[2][2] - m[0][0] * m[1][2] * m[2][1] - m[1][0] * m[0][1] * m[2][2] + m[1][0] * m[0][2] * m[2][1] + m[2][0] * m[0][1] * m[1][2] - m[2][0] * m[0][2] * m[1][1];

    afxReal det = m[0][0] * im[0][0] + m[0][1] * im[1][0] + m[0][2] * im[2][0] + m[0][3] * im[3][0];

    if (det)
    {
        det = 1.0 / det;

        for (afxNat i = 0; i < 4; i++)
            for (afxNat j = 0; j < 4; j++)
                im[i][j] = im[i][j] * det;
    }
    return det;
}

_AFXINL afxReal AfxInvertAtm4d(afxM4d const m, afxM4d im)
{
    // Should be compatible with void MatrixInvert4x3(float *DestInit, const float *SourceInit)

    afxReal det = (m[1][1] * m[2][2] - m[2][1] * m[1][2]) * m[0][0] - (m[2][2] * m[0][1] - m[2][1] * m[0][2]) * m[1][0] + (m[1][2] * m[0][1] - m[1][1] * m[0][2]) * m[2][0];

    if (det == (afxReal)0) AfxResetM4d(im); // granny ignora LT se não for inversível mas nulifica o W.
    else
    {
        afxReal recip = 1.0 / det;
        im[0][0] =  ( m[1][1] * m[2][2] - m[2][1] * m[1][2]) * recip;
        im[0][1] = -((m[2][2] * m[0][1] - m[2][1] * m[0][2]) * recip);
        im[0][2] =  ( m[1][2] * m[0][1] - m[1][1] * m[0][2]) * recip;
        im[0][3] = 0.f;

        im[1][0] = -((m[1][0] * m[2][2] - m[2][0] * m[1][2]) * recip);
        im[1][1] =  ( m[2][2] * m[0][0] - m[2][0] * m[0][2]) * recip;
        im[1][2] = -((m[1][2] * m[0][0] - m[0][2] * m[1][0]) * recip);
        im[1][3] = 0.f;

        im[2][0] =  ( m[2][1] * m[1][0] - m[2][0] * m[1][1]) * recip;
        im[2][1] = -((m[2][1] * m[0][0] - m[2][0] * m[0][1]) * recip);
        im[2][2] =  ( m[1][1] * m[0][0] - m[1][0] * m[0][1]) * recip;
        im[2][3] = 0.f;
    }
    im[3][0] = -(im[2][0] * m[3][2] + im[1][0] * m[3][1] + im[0][0] * m[3][0]);
    im[3][1] = -(im[2][1] * m[3][2] + im[1][1] * m[3][1] + im[0][1] * m[3][0]);
    im[3][2] = -(im[2][2] * m[3][2] + im[1][2] * m[3][1] + im[0][2] * m[3][0]);
    im[3][3] = 1.f;
    return det;
}

// Det

_AFXINL afxReal AfxDetM2d(afxM2d const m)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    return  (m[0][0] * m[1][1]) - 
            (m[0][1] * m[1][0]);
}

_AFXINL afxReal AfxDetM3d(afxM3d const m)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    return  (m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1])) - 
            (m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0])) + 
            (m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]));
}

_AFXINL afxReal AfxDetM4d(afxM4d const m)
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

_AFXINL void AfxScalingM3d(afxM3d m, afxV3d const scale)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(scale);
    AfxAssertDiff(m, scale);
    AfxResetM3d(m);
    m[0][0] = scale[0];
    m[1][1] = scale[1];
    m[2][2] = scale[2];
}

_AFXINL void AfxScalingM4d(afxM4d m, afxV3d const scale)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(scale);
    AfxAssertDiff(m, scale);
    AfxResetM4d(m);
    m[0][0] = scale[0];
    m[1][1] = scale[1];
    m[2][2] = scale[2];
}

// MakeRotation

_AFXINL void AfxRotationM3dFromQuat(afxM3d m, afxQuat const q)
{
    // Inspired in AfxRotationM4dFromQuat

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

_AFXINL void AfxRotationM4dFromQuat(afxM4d m, afxQuat const q)
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

_AFXINL void AfxRotationM4dFromEuler(afxM4d m, afxV3d const pitchYawRoll)
{
    // Should be compatible with XMMATRIX XMMatrixRotationRollPitchYaw(float Pitch, float Yaw, float Roll)
    // Should be compatible with XMMATRIX XMMatrixRotationRollPitchYawFromVector(FXMVECTOR Angles)

    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(pitchYawRoll);

    afxReal pc, ps, yc, ys, rc, rs;
    AfxCosSin(pitchYawRoll[0], &pc, &ps);
    AfxCosSin(pitchYawRoll[1], &yc, &ys);
    AfxCosSin(pitchYawRoll[2], &rc, &rs);

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

_AFXINL void AfxRotationM3dFromAxis(afxM3d m, afxV3d const axis, afxReal /*theta*/radians)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(axis);
    AfxAssert(!AfxV3dIsZero(axis));
    AfxAssert(!AfxV3dIsInfinite(axis));

    afxReal cosTheta, sinTheta;
    AfxCosSin(radians, &cosTheta, &sinTheta);
    afxReal cosValue = 1.0f - cosTheta;
    afxV3d axis2;
    AfxNormalizeV3d(axis2, axis);

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

_AFXINL void AfxRotationM4dFromAxis(afxM4d m, afxV3d const axis, afxReal /*theta*/radians)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(axis);
    AfxAssert(!AfxV3dIsZero(axis));
    AfxAssert(!AfxV3dIsInfinite(axis));

    afxReal cosTheta, sinTheta;
    AfxCosSin(radians, &cosTheta, &sinTheta);
    afxReal cosValue = 1.0f - cosTheta;
    afxV3d axis2;
    AfxNormalizeV3d(axis2, axis);

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

_AFXINL void AfxRotationM4dFromX(afxM4d m, afxReal angle)
{
    // Should be compatible with XMMATRIX XMMatrixRotationX(float Angle)

    afxReal co, si;
    AfxCosSin(angle, &co, &si);

    AfxResetM4d(m);
    m[1][1] = co;
    m[1][2] = si;
    m[2][1] = -si;
    m[2][2] = co;
}

_AFXINL void AfxRotationM4dFromY(afxM4d m, afxReal angle)
{
    // Should be compatible with XMMATRIX XMMatrixRotationY(float Angle)

    afxReal co, si;
    AfxCosSin(angle, &co, &si);

    AfxResetM4d(m);

    m[0][0] = co;
    m[0][2] = -si;
    m[2][0] = si;
    m[2][2] = co;
}

_AFXINL void AfxRotationM4dFromZ(afxM4d m, afxReal angle)
{
    // Should be compatible with XMMATRIX XMMatrixRotationZ(float Angle)

    afxReal co, si;
    AfxCosSin(angle, &co, &si);

    AfxResetM4d(m);

    m[0][0] = co;
    m[0][1] = si;
    m[1][0] = -si;
    m[1][1] = co;
}

// Translate

_AFXINL void AfxTranslationM4d(afxM4d m, afxV3d const translation)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(translation);
    AfxResetM4d(m);
    AfxCopyV3d(m[3], translation);    
}

// Decompose

_AFXINL afxBool AfxPolarDecomposeM3d(afxM3d const m, afxReal tol, afxM3d rm, afxM3d ssm)
{
    // Should be compatible with char PolarDecompose(const float *Source3x3Init, float Tolerance, float *QInit, float *SInit)

    afxM3d ZMirror;
    AfxResetM3d(ZMirror);
    ZMirror[2][2] = -1.0;

    // 0 1 2
    // 3 4 5
    // 6 7 8

    if (AfxDetM3d(m) < 0.0)
    {
        AfxMultiplyM3d(rm, m, ZMirror);
    }
    else
    {
        AfxCopyM3d(rm, m);
    }

    afxNat i = 0;
    afxResult rslt = TRUE;

    while (1)
    {
        AfxInvertM3d(rm, ssm);
        AfxAddM3d(ssm, ssm, rm);

        AfxScaleV3d(ssm[0], ssm[0], 0.5);
        AfxScaleV3d(ssm[1], ssm[1], 0.5);
        AfxScaleV3d(ssm[2], ssm[2], 0.5);

        afxM3d ssMinR;
        AfxSubM3d(ssMinR, ssm, rm);

        AfxCopyM3d(rm, ssm);

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

_AFXINL void AfxComposeM4d(afxM4d m, afxV3d const scalOrigin, afxQuat const scalOrient, afxV3d const scaling, afxV3d const rotOrigin, afxQuat const rotQuat, afxV3d const translation)
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
    AfxNegV3d(negScaOrigin, scalOrigin);

    afxM4d scalOriginI, scalOrientM, scalOrientT, scalM, rotM;
    AfxResetM4d(scalOriginI);
    AfxCopyV3d(scalOriginI[3], negScaOrigin);
    AfxRotationM4dFromQuat(scalOrientM, scalOrient);
    AfxTransposeM4d(scalOrientM, scalOrientT);
    AfxResetM4d(scalM);
    AfxScalingM4d(scalM, scaling);
    AfxRotationM4dFromQuat(rotM, rotQuat);

    afxM4d t;
    AfxMultiplyM4d(t, scalOriginI, scalOrientT);
    AfxMultiplyM4d(m, t, scalM);
    AfxMultiplyM4d(t, m, scalOrientM);
    AfxAddV3d(t[3], t[3], scalOrigin);
    AfxSubV3d(t[3], t[3], rotOrigin);
    AfxMultiplyM4d(m, t, rotM);
    AfxAddV3d(m[3], m[3], rotOrigin);
    AfxAddV3d(m[3], m[3], translation);
}

_AFXINL void AfxComposeAtm4d(afxM4d m, afxV3d const scale, afxV3d const rotAxis, afxQuat const rot, afxV3d const translation)
{
    // Should be compatible with XMMATRIX XMMatrixAffineTransformation(FXMVECTOR Scaling, FXMVECTOR RotationOrigin, FXMVECTOR RotationQuaternion, GXMVECTOR Translation)

    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(scale);
    AfxAssert(rot);
    AfxAssert(translation);

    // M = MScaling * Inverse(MRotationOrigin) * MRotation * MRotationOrigin * MTranslation;

    afxM4d sm, rm, c;
    AfxResetM4d(sm);
    AfxScalingM4d(sm, scale);
    AfxRotationM4dFromQuat(rm, rot);
    AfxSubV3d(sm[3], sm[3], rotAxis);
    AfxMultiplyM4d(m, sm, rm);
    AfxAddV3d(m[3], m[3], rotAxis);
    AfxAddV3d(m[3], m[3], translation);
}

////////////////////////////////////////////////////////////////////////////////
// MATRIX TRANSFORMATION METHODS                                              //
////////////////////////////////////////////////////////////////////////////////

// In post-multiplication, each component of the result, at in given row R and column C, is computed by taking the dot product of the left matrix’s row R by the right matrix’s column C.

// MultiplyTransposed

_AFXINL void AfxMultiplyM2dTransposed(afxM2d m, afxM2d const in, afxM2d const by)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(in);
    AfxAssert(by);
    AfxAssert(in != by);
    AfxAssert(by != m);

    m[0][0] = (in[0][0] * by[0][0]) + (in[0][1] * by[1][0]);
    m[0][1] = (in[1][0] * by[0][0]) + (in[1][1] * by[1][0]);

    m[1][0] = (in[0][0] * by[0][1]) + (in[0][1] * by[1][1]);
    m[1][1] = (in[1][0] * by[0][1]) + (in[1][1] * by[1][1]);
}

_AFXINL void AfxMultiplyM3dTransposed(afxM3d m, afxM3d const in, afxM3d const by)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(in);
    AfxAssert(by);
    AfxAssert(in != by);
    AfxAssert(by != m);

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

// 0 1 2, 3 4 5, 6 7 8

_AFXINL void TransposeMatrixMultiply3x3(afxM3d IntoMatrix, afxM3d const TransposedMatrix, afxM3d const ByMatrix)
{
    // Compatible with TransposeMatrixMultiply3x3(out, in, by)

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

_AFXINL void AfxMultiplyM4dTransposed(afxM4d m, afxM4d const in, afxM4d const by)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(in);
    AfxAssert(by);
    AfxAssert(in != by);
    AfxAssert(by != m);

    // Compatible with m = XMMatrixMultiplyTranspose(in, by)

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

_AFXINL void AfxMultiplyPlanarM3dTransposed(afxM3d m, afxM3d const in, afxM3d const by)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(in);
    AfxAssert(by);
    AfxAssert(in != by);
    AfxAssert(by != m);

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

_AFXINL void AfxMultiplyLtm4dTransposed(afxM4d m, afxM4d const in, afxM4d const by)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(in);
    AfxAssert(by);
    AfxAssert(in != by);
    AfxAssert(by != m);

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

_AFXINL void AfxMultiplyAtm4dTransposed(afxM4d m, afxM4d const in, afxM4d const by)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(in);
    AfxAssert(by);
    AfxAssert(in != by);
    AfxAssert(by != m);

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

_AFXINL void AfxMultiplyM2d(afxM2d m, afxM2d const in, afxM2d const by)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(in);
    AfxAssert(by);
    AfxAssert(in != by);
    AfxAssert(by != m);

    m[0][0] = (in[0][0] * by[0][0]) + (in[0][1] * by[1][0]);
    m[0][1] = (in[0][0] * by[0][1]) + (in[0][1] * by[1][1]);

    m[1][0] = (in[1][0] * by[0][0]) + (in[1][1] * by[1][0]);
    m[1][1] = (in[1][0] * by[0][1]) + (in[1][1] * by[1][1]);
}

_AFXINL void AfxMultiplyM3d(afxM3d m, afxM3d const in, afxM3d const by)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(in);
    AfxAssert(by);
    AfxAssert(in != by);
    AfxAssert(by != m);

    // Compatible with MatrixMultiply3x3(m, in, by)

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

_AFXINL void AfxMultiplyM4d(afxM4d m, afxM4d const in, afxM4d const by)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(in);
    AfxAssert(by);
    AfxAssert(in != by);
    AfxAssert(by != m);

    // Compatible with m = XMMatrixMultiply(in, by)
    // Compatible with ColumnMatrixMultiply4x4(m, in, by)

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

_AFXINL void AfxMultiplyPlanarM3d(afxM3d m, afxM3d const in, afxM3d const by)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(in);
    AfxAssert(by);
    AfxAssert(in != by);
    AfxAssert(by != m);

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

_AFXINL void AfxMultiplyLtm4d(afxM4d m, afxM4d const in, afxM4d const by)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(in);
    AfxAssert(by);
    AfxAssert(in != by);
    AfxAssert(by != m);

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

_AFXINL void AfxMultiplyAtm4d(afxM4d m, afxM4d const in, afxM4d const by)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(in);
    AfxAssert(by);
    AfxAssert(in != by);
    AfxAssert(by != m);

    // Compatible with ColumnMatrixMultiply4x3(m, in, by)
    
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

// MultiplyArrayedV2d

_AFXINL void AfxPostMultiplyArrayedV2d(afxM2d const m, afxNat cnt, afxV2d const in[], afxV2d out[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
        AfxPostMultiplyV2d(m, in[i], out[i]);
}

_AFXINL void AfxPostMultiplyArrayedV3d(afxM3d const m, afxNat cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
        AfxPostMultiplyV3d(m, in[i], out[i]);
}

_AFXINL void AfxPostMultiplyArrayedV4d(afxM4d const m, afxNat cnt, afxV4d const in[], afxV4d out[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
        AfxPostMultiplyV4d(m, in[i], out[i]);
}

_AFXINL void AfxPostMultiplyArrayedAtv3d(afxM4d const m, afxNat cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
        AfxPostMultiplyAtv3d(m, in[i], out[i]);
}

_AFXINL void AfxPostMultiplyArrayedLtv4d(afxM3d const m, afxNat cnt, afxV4d const in[], afxV4d out[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
        AfxPostMultiplyLtv4d(m, in[i], out[i]);
}

_AFXINL void AfxPostMultiplyArrayedAtv4d(afxM4d const m, afxNat cnt, afxV4d const in[], afxV4d out[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
        AfxPostMultiplyAtv4d(m, in[i], out[i]);
}

// PreMultiply

_AFXINL void AfxPreMultiplyArrayedV2d(afxM2d const m, afxNat cnt, afxV2d const in[], afxV2d out[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
        AfxPreMultiplyV2d(m, in[i], out[i]);
}

_AFXINL void AfxPreMultiplyArrayedV3d(afxM3d const m, afxNat cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
        AfxPreMultiplyV3d(m, in[i], out[i]);
}

_AFXINL void AfxPreMultiplyArrayedV4d(afxM4d const m, afxNat cnt, afxV4d const in[], afxV4d out[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
        AfxPreMultiplyV4d(m, in[i], out[i]);
}

_AFXINL void AfxPreMultiplyArrayedAtv3d(afxM4d const m, afxNat cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
        AfxPreMultiplyAtv3d(m, in[i], out[i]);
}

_AFXINL void AfxPreMultiplyArrayedLtv4d(afxM3d const m, afxNat cnt, afxV4d const in[], afxV4d out[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
        AfxPreMultiplyLtv4d(m, in[i], out[i]);
}

_AFXINL void AfxPreMultiplyArrayedAtv4d(afxM4d const m, afxNat cnt, afxV4d const in[], afxV4d out[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
        AfxPreMultiplyAtv4d(m, in[i], out[i]);
}

// TransformSerialized

_AFXINL void AfxPostMultiplySerializedV2d(afxM2d const m, afxNat inStride, afxNat outStride, afxNat cnt, afxV2d const in[], afxV2d out[])
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
        AfxPostMultiplyV2d(m, (void*)inPtr, (void*)outPtr);

        inPtr += inStride;
        outPtr += outStride;
    }
}

_AFXINL void AfxPostMultiplySerializedV3d(afxM3d const m, afxNat inStride, afxNat outStride, afxNat cnt, afxV3d const in[], afxV3d out[])
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
        AfxPostMultiplyV3d(m, (void*)inPtr, (void*)outPtr);

        inPtr += inStride;
        outPtr += outStride;
    }
}

_AFXINL void AfxPostMultiplySerializedV4d(afxM4d const m, afxNat inStride, afxNat outStride, afxNat cnt, afxV4d const in[], afxV4d out[])
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
        AfxPostMultiplyV4d(m, (void*)inPtr, (void*)outPtr);

        inPtr += inStride;
        outPtr += outStride;
    }
}

_AFXINL void AfxPostMultiplySerializedAtv3d(afxM4d const m, afxNat inStride, afxNat outStride, afxNat cnt, afxV3d const in[], afxV3d out[])
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
        AfxPostMultiplyAtv3d(m, (void*)inPtr, (void*)outPtr);

        inPtr += inStride;
        outPtr += outStride;
    }
}

_AFXINL void AfxPostMultiplySerializedLtv4d(afxM3d const m, afxNat inStride, afxNat outStride, afxNat cnt, afxV4d const in[], afxV4d out[])
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
        AfxPostMultiplyLtv4d(m, (void*)inPtr, (void*)outPtr);

        inPtr += inStride;
        outPtr += outStride;
    }
}

_AFXINL void AfxPostMultiplySerializedAtv4d(afxM4d const m, afxNat inStride, afxNat outStride, afxNat cnt, afxV4d const in[], afxV4d out[])
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
        AfxPostMultiplyAtv4d(m, (void*)inPtr, (void*)outPtr);

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
        AfxMultiplyM3d(tmp, ltm, in[i]);
        AfxMultiplyM3d(out[i], tmp, iltm);
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
        AfxPickM3d(in[i], ss);
        afxV4d pos;
        AfxAssimilateAtv4d(ltm, atv, 1, &in[i][3], &pos);
        AfxAssimilateLtm3d(ltm, iltm, 1, &ss, &ss);
        AfxMakeM4dFromM3d(out[i], ss, pos);
    }
}

_AFX void AfxApplyRootMotionVectorsToMatrix(afxV3d const translation, afxV3d const rotation, afxM4d const mm, afxM4d m)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(translation);
    AfxAssert(rotation);
    AfxAssert(mm); // model matrix
    AfxAssert(m);
    afxM4d upd;
    afxReal rl = AfxMagV3d(rotation);

    if (rl <= 0.0) AfxTranslationM4d(upd, translation);
    else
    {
        afxV3d scaledRot;
        AfxScaleV3d(scaledRot, rotation, 1.f / rl);
        AfxRotationM4dFromAxis(upd, scaledRot, rl);
        upd[3][0] = translation[0];
        upd[3][1] = translation[1];
        upd[3][2] = translation[2];
    }

    afxM4d tmp;
    AfxMultiplyAtm4d(tmp, upd, mm);
    AfxCopyM4d(m, tmp);
}
