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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#include "afx/math/afxMatrix.h"
#include "afx/math/afxMathDefs.h"
#include "afx/math/afxReal.h"
#include "afx/math/afxVector.h"
#include "afx/math/afxPlane.h"

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

_AFXINL afxBool AfxM2dIsIdentity(afxReal const m[2][2])
{
    afxBool flag = 1;

    for (afxNat i = 0; flag && i < 2; i++)
        for (afxNat j = 0; flag && j < 2; j++)
            if (m[i][j] != 1 && m[j][i] != 0)
                flag = 0;

    return flag;
}

_AFXINL afxBool AfxM3dIsIdentity(afxReal const m[3][3])
{
    afxBool flag = 1;

    for (afxNat i = 0; flag && i < 3; i++)
        for (afxNat j = 0; flag && j < 3; j++)
            if (m[i][j] != 1 && m[j][i] != 0)
                flag = 0;

    return flag;
}

_AFXINL afxBool AfxM4dIsIdentity(afxReal const m[4][4])
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

_AFXINL void AfxZeroM2d(afxReal m[2][2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxCopyM2d(m, AFX_M2D_ZERO);
}

_AFXINL void AfxZeroM3d(afxReal m[3][3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxCopyM3d(m, AFX_M3D_ZERO);
}

_AFXINL void AfxZeroM4d(afxReal m[4][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxCopyM4d(m, AFX_M4D_ZERO);
}

// MakeIdentity

_AFXINL void AfxResetM2d(afxReal m[2][2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxCopyM2d(m, AFX_M2D_IDENTITY);
}

_AFXINL void AfxResetM3d(afxReal m[3][3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxCopyM3d(m, AFX_M3D_IDENTITY);
}

_AFXINL void AfxResetM4d(afxReal m[4][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxCopyM4d(m, AFX_M4D_IDENTITY);
}

_AFXINL void AfxEnsureLinearM4d(afxReal m[4][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    m[0][3] = m[1][3] = m[2][3] = (afxReal)0;
    AfxResetV4d(m[3]);
}

_AFXINL void AfxEnsureAffineM4d(afxReal m[4][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    m[0][3] = m[1][3] = m[2][3] = (afxReal)0;
    m[3][3] = (afxReal)1;
}

// Set

_AFXINL void AfxSetM2d(afxReal m[2][2], afxReal const x[2], afxReal const y[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(x);
    AfxAssert(y);
    AfxCopyV2d(m[0], x);
    AfxCopyV2d(m[1], y);
}

_AFXINL void AfxSetM3d(afxReal m[3][3], afxReal const x[3], afxReal const y[3], afxReal const z[3])
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

_AFXINL void AfxSetM4d(afxReal m[4][4], afxReal const x[4], afxReal const y[4], afxReal const z[4], afxReal const w[4])
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

_AFXINL void AfxSetTransposedM2d(afxReal m[2][2], afxReal const x[2], afxReal const y[2])
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

_AFXINL void AfxSetTransposedM3d(afxReal m[3][3], afxReal const x[3], afxReal const y[3], afxReal const z[3])
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

_AFXINL void AfxSetTransposedM4d(afxReal m[4][4], afxReal const x[4], afxReal const y[4], afxReal const z[4], afxReal const w[4])
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

_AFXINL void AfxCopyM2d(afxReal m[2][2], afxReal const in[2][2])
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

_AFXINL void AfxCopyM3d(afxReal m[3][3], afxReal const in[3][3])
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

_AFXINL void AfxCopyM4d(afxReal m[4][4], afxReal const in[4][4])
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

_AFXINL void AfxCopyLinearM4d(afxReal m[4][4], afxReal const in[4][4])
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

_AFXINL void AfxCopyAffineM4d(afxReal m[4][4], afxReal const in[4][4])
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

    m[3][0] = in[3][0];
    m[3][1] = in[3][1];
    m[3][2] = in[3][2];
}

_AFXINL void AfxCopyTransposedLinearM4d(afxReal m[4][4], afxReal const in[4][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(in);
    AfxAssert(m);

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

_AFXINL void AfxCopyTransposedAffineM4d(afxReal m[4][4], afxReal const in[4][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(in);
    AfxAssert(m);

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

_AFXINL void AfxM2dFromM3d(afxReal m[2][2], afxReal const in[3][3])
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

_AFXINL void AfxM2dFromM4d(afxReal m[2][2], afxReal const in[4][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(in);
    AfxAssertDiff(m, in);

    m[0][0] = in[0][0];
    m[0][1] = in[0][1];

    m[1][0] = in[1][0];
    m[1][1] = in[1][1];
}

_AFXINL void AfxM3dFromM4d(afxReal m[3][3], afxReal const in[4][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(in);
    AfxAssertDiff(m, in);

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

_AFXINL void AfxM3dFromM4dTransposed(afxReal m[3][3], afxReal const in[4][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(in);

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

_AFXINL void AfxM3dFromM2d(afxReal m[3][3], afxReal const in[2][2])
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

_AFXINL void AfxM4dFromM2d(afxReal m[4][4], afxReal const in[2][2])
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

_AFXINL void AfxM4dFromM3d(afxReal m[4][4], afxReal const in[3][3], afxReal const translation[4])
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

_AFXINL void AfxSwapM2d(afxReal m[2][2], afxReal other[2][2])
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

_AFXINL void AfxSwapM3d(afxReal m[3][3], afxReal other[3][3])
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

_AFXINL void AfxSwapM4d(afxReal m[4][4], afxReal other[4][4])
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

_AFXINL void AfxTransposeM2d(afxReal m[2][2], afxReal const in[2][2])
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

_AFXINL void AfxTransposeM3d(afxReal m[3][3], afxReal const in[3][3])
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

_AFXINL void AfxTransposeM4d(afxReal m[4][4], afxReal const in[4][4])
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

_AFXINL void AfxM4dFromM3dTransposed(afxReal m[4][4], afxReal const in[3][3], afxReal const translation[4])
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

_AFXINL void AfxAddM2d(afxReal m[2][2], afxReal const a[2][2], afxReal const b[2][2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(a);
    AfxAssert(b);
    AfxAddV2d(m[0], a[0], b[0]);
    AfxAddV2d(m[1], a[1], b[1]);
}

_AFXINL void AfxAddM3d(afxReal m[3][3], afxReal const a[3][3], afxReal const b[3][3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(a);
    AfxAssert(b);
    AfxAddV3d(m[0], a[0], b[0]);
    AfxAddV3d(m[1], a[1], b[1]);
    AfxAddV3d(m[2], a[2], b[2]);
}

_AFXINL void AfxAddM4d(afxReal m[4][4], afxReal const a[4][4], afxReal const b[4][4])
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

_AFXINL void AfxSubM2d(afxReal m[2][2], afxReal const a[2][2], afxReal const b[2][2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(a);
    AfxAssert(b);
    AfxSubV2d(m[0], a[0], b[0]);
    AfxSubV2d(m[1], a[1], b[1]);
}

_AFXINL void AfxSubM3d(afxReal m[3][3], afxReal const a[3][3], afxReal const b[3][3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(a);
    AfxAssert(b);
    AfxSubV3d(m[0], a[0], b[0]);
    AfxSubV3d(m[1], a[1], b[1]);
    AfxSubV3d(m[2], a[2], b[2]);
}

_AFXINL void AfxSubM4d(afxReal m[4][4], afxReal const a[4][4], afxReal const b[4][4])
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

// Combine

_AFXINL void AfxCombineM2d(afxReal m[2][2], afxReal lambda1, afxReal const a[2][2], afxReal lambda2, afxReal const b[2][2])
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

_AFXINL void AfxCombineM3d(afxReal m[3][3], afxReal lambda1, afxReal const a[3][3], afxReal lambda2, afxReal const b[3][3])
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

_AFXINL void AfxCombineLinearM4d(afxReal m[4][4], afxReal lambda1, afxReal const a[4][4], afxReal lambda2, afxReal const b[4][4])
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

_AFXINL afxReal AfxInvertM3d(afxReal m[3][3], afxReal const in[3][3])
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
        AfxCopyM3d(m, in);
        AfxAssert(det != 0.0);
    }
    return det;
}

// Invert
// Memory layout: hybrid

_AFXINL afxReal AfxInvertM4d(afxReal m[4][4], afxReal const in[4][4])
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

_AFXINL afxReal AfxInvertAffineM4d(afxReal m[4][4], afxReal const in[4][4])
{
    // Should be compatible with void MatrixInvert4x3(float *DestInit, const float *SourceInit)

    afxReal det = (in[1][1] * in[2][2] - in[2][1] * in[1][2]) * in[0][0] - (in[2][2] * in[0][1] - in[2][1] * in[0][2]) * in[1][0] + (in[1][2] * in[0][1] - in[1][1] * in[0][2]) * in[2][0];

    if (det == (afxReal)0) AfxResetM4d(m); // granny ignora LT se não for inversível mas nulifica o W.
    else
    {
        afxReal recip = 1.0 / det;
        m[0][0] =  ( in[1][1] * in[2][2] - in[2][1] * in[1][2]) * recip;
        m[0][1] = -((in[2][2] * in[0][1] - in[2][1] * in[0][2]) * recip);
        m[0][2] =  ( in[1][2] * in[0][1] - in[1][1] * in[0][2]) * recip;
        m[1][0] = -((in[1][0] * in[2][2] - in[2][0] * in[1][2]) * recip);
        m[1][1] =  ( in[2][2] * in[0][0] - in[2][0] * in[0][2]) * recip;
        m[1][2] = -((in[1][2] * in[0][0] - in[0][2] * in[1][0]) * recip);
        m[2][0] =  ( in[2][1] * in[1][0] - in[2][0] * in[1][1]) * recip;
        m[2][1] = -((in[2][1] * in[0][0] - in[2][0] * in[0][1]) * recip);
        m[2][2] =  ( in[1][1] * in[0][0] - in[1][0] * in[0][1]) * recip;
        m[0][3] = 0.f;
        m[1][3] = 0.f;
        m[2][3] = 0.f;
    }
    m[3][0] = -(m[2][0] * in[3][2] + m[1][0] * in[3][1] + m[0][0] * in[3][0]);
    m[3][1] = -(m[2][1] * in[3][2] + m[1][1] * in[3][1] + m[0][1] * in[3][0]);
    m[3][2] = -(m[2][2] * in[3][2] + m[1][2] * in[3][1] + m[0][2] * in[3][0]);
    m[3][3] = 1.f;
    return det;
}

// Det

_AFXINL afxReal AfxDetM2d(afxReal const m[2][2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    return  (m[0][0] * m[1][1]) - 
            (m[0][1] * m[1][0]);
}

_AFXINL afxReal AfxDetM3d(afxReal const m[3][3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    return  (m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1])) - 
            (m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0])) + 
            (m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]));
}

_AFXINL afxReal AfxDetM4d(afxReal const m[4][4])
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

_AFXINL void AfxSetM3dScaleV2d(afxReal m[3][3], afxReal const v[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(v);
    AfxAssertDiff(m, v);
    //AfxResetM4d(m);
    m[0][0] = v[0];
    m[1][1] = v[1];
}

_AFXINL void AfxSetM3dScale(afxReal m[3][3], afxReal const v[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(v);
    AfxAssertDiff(m, v);
    //AfxResetM4d(m);
    m[0][0] = v[0];
    m[1][1] = v[1];
    m[2][2] = v[2];
}

_AFXINL void AfxSetM4dScaleV2d(afxReal m[4][4], afxReal const v[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(v);
    AfxAssertDiff(m, v);
    //AfxResetM4d(m);
    m[0][0] = v[0];
    m[1][1] = v[1];
}

_AFXINL void AfxSetM4dScale(afxReal m[4][4], afxReal const v[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(v);
    AfxAssertDiff(m, v);
    //AfxResetM4d(m);
    m[0][0] = v[0];
    m[1][1] = v[1];
    m[2][2] = v[2];
}

// Scale

_AFXINL void AfxApplyM3dScale(afxReal m[3][3], afxReal const scale[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(scale);

    afxM3d tmp, tmp2;
    AfxCopyM3d(tmp, m);
    AfxResetM3d(tmp2);
    AfxSetM3dScale(tmp2, scale);
    AfxMultiplyM3d(m, tmp2, tmp);
}

_AFXINL void AfxApplyPlanarM3dScale(afxReal m[3][3], afxReal const scale[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(scale);

    afxM3d tmp, tmp2;
    AfxCopyM3d(tmp, m);
    AfxResetM3d(tmp2);
    AfxSetM3dScale(tmp2, scale);
    AfxMultiplyPlanarM3d(m, tmp2, tmp);
}

_AFXINL void AfxApplyAffineM4dScale(afxReal m[4][4], afxReal const scale[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(scale);

    afxM4d tmp, tmp2;
    AfxCopyM4d(tmp, m);
    AfxResetM4d(tmp2);
    AfxSetM4dScale(tmp2, scale);
    AfxMultiplyAffineM4d(m, tmp2, tmp);
}

_AFXINL void AfxApplyM4dScale(afxReal m[4][4], afxReal const scale[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(scale);

    afxM4d tmp, tmp2;
    AfxCopyM4d(tmp, m);
    AfxResetM4d(tmp2);
    AfxSetM4dScale(tmp2, scale);
    AfxMultiplyM4d(m, tmp, tmp2);
}

// MakeRotation

_AFXINL void AfxM3dFromQuat(afxReal m[3][3], afxQuat const q)
{
    // Inspired in AfxM4dFromQuat

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

_AFXINL void AfxM4dFromQuat(afxReal m[4][4], afxQuat const q)
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

// uncertain name

// MakeRotation_Euler
//

_AFXINL void AfxM4dFromEuler(afxReal m[4][4], afxReal const pitchYawRoll[3])
{
    // Should be compatible with XMMATRIX XMMatrixRotationRollPitchYaw(float Pitch, float Yaw, float Roll)
    // Should be compatible with XMMATRIX XMMatrixRotationRollPitchYawFromVector(FXMVECTOR Angles)

    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(pitchYawRoll);

    afxV2d p, y, r;
    AfxCosSin(pitchYawRoll[0], p);
    AfxCosSin(pitchYawRoll[1], y);
    AfxCosSin(pitchYawRoll[2], r);

    m[0][0] = r[0] * y[0] + r[1] * p[1] * y[1];
    m[0][1] = r[1] * p[0];
    m[0][2] = r[1] * p[1] * y[0] - r[0] * y[1];
    m[0][3] = 0.f;

    m[1][0] = r[0] * p[1] * y[1] - r[1] * y[0];
    m[1][1] = r[0] * p[0];
    m[1][2] = r[1] * y[1] + r[0] * p[1] * y[0];
    m[1][3] = 0.f;

    m[2][0] = p[0] * y[1];
    m[2][1] =-p[1];
    m[2][2] = p[0] * y[0];
    m[2][3] = 0.f;

    m[3][0] = 0.f;
    m[3][1] = 0.f;
    m[3][2] = 0.f;
    m[3][3] = 1.f;
}

// uncertain origin

// MakeRotation_Axial
// Memory layout: looks like it is row-major

_AFXINL void AfxM3dFromAxisAngle(afxReal m[3][3], afxReal const axis[3], afxReal /*theta*/radians)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(axis);
    AfxAssert(!AfxV3dIsZero(axis));
    AfxAssert(!AfxV3dIsInfinite(axis));

    afxV2d cosSinTheta;
    AfxCosSin(radians, cosSinTheta);
    afxReal cosValue = 1.0f - cosSinTheta[0];
    afxV3d axis2;
    AfxGetNormalizedV3d(axis2, axis);

    m[0][0] = (axis2[0] * axis2[0] * cosValue) + cosSinTheta[0];
    m[0][1] = (axis2[0] * axis2[1] * cosValue) + (axis2[2] * cosSinTheta[1]);
    m[0][2] = (axis2[0] * axis2[2] * cosValue) - (axis2[1] * cosSinTheta[1]);

    m[1][0] = (axis2[1] * axis2[0] * cosValue) - (axis2[2] * cosSinTheta[1]);
    m[1][1] = (axis2[1] * axis2[1] * cosValue) + cosSinTheta[0];
    m[1][2] = (axis2[1] * axis2[2] * cosValue) + (axis2[0] * cosSinTheta[1]);

    m[2][0] = (axis2[2] * axis2[0] * cosValue) + (axis2[1] * cosSinTheta[1]);
    m[2][1] = (axis2[2] * axis2[1] * cosValue) - (axis2[0] * cosSinTheta[1]);
    m[2][2] = (axis2[2] * axis2[2] * cosValue) + cosSinTheta[0];
}

_AFXINL void AfxM4dFromAxisAngle(afxReal m[4][4], afxReal const axis[3], afxReal /*theta*/radians)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(axis);
    AfxAssert(!AfxV3dIsZero(axis));
    AfxAssert(!AfxV3dIsInfinite(axis));

    afxV2d cosSinTheta;
    AfxCosSin(radians, cosSinTheta);
    afxReal cosValue = 1.0f - cosSinTheta[0];
    afxV3d axis2;
    AfxGetNormalizedV3d(axis2, axis);

    m[0][0] = (axis2[0] * axis2[0] * cosValue) + cosSinTheta[0];
    m[0][1] = (axis2[0] * axis2[1] * cosValue) + (axis2[2] * cosSinTheta[1]);
    m[0][2] = (axis2[0] * axis2[2] * cosValue) - (axis2[1] * cosSinTheta[1]);
    m[0][3] = AfxScalar(0);

    m[1][0] = (axis2[1] * axis2[0] * cosValue) - (axis2[2] * cosSinTheta[1]);
    m[1][1] = (axis2[1] * axis2[1] * cosValue) + cosSinTheta[0];
    m[1][2] = (axis2[1] * axis2[2] * cosValue) + (axis2[0] * cosSinTheta[1]);
    m[1][3] = AfxScalar(0);

    m[2][0] = (axis2[2] * axis2[0] * cosValue) + (axis2[1] * cosSinTheta[1]);
    m[2][1] = (axis2[2] * axis2[1] * cosValue) - (axis2[0] * cosSinTheta[1]);
    m[2][2] = (axis2[2] * axis2[2] * cosValue) + cosSinTheta[0];
    m[2][3] = AfxScalar(0);

    m[3][0] = AfxScalar(0);
    m[3][1] = AfxScalar(0);
    m[3][2] = AfxScalar(0);
    m[3][3] = AfxScalar(1);
}

_AFXINL void AfxM4dFromRotationX(afxReal m[4][4], afxReal angle)
{
    // Should be compatible with XMMATRIX XMMatrixRotationX(float Angle)

    afxV2d cosSin;
    AfxCosSin(angle, cosSin);

    AfxResetM4d(m);
    m[1][1] = cosSin[0];
    m[1][2] = cosSin[1];
    m[2][1] = -cosSin[1];
    m[2][2] = cosSin[0];
}

_AFXINL void AfxM4dFromRotationY(afxReal m[4][4], afxReal angle)
{
    // Should be compatible with XMMATRIX XMMatrixRotationY(float Angle)

    afxV2d cosSin;
    AfxCosSin(angle, cosSin);

    AfxResetM4d(m);

    m[0][0] = cosSin[0];
    m[0][2] = -cosSin[1];
    m[2][0] = cosSin[1];
    m[2][2] = cosSin[0];
}

_AFXINL void AfxM4dFromRotationZ(afxReal m[4][4], afxReal angle)
{
    // Should be compatible with XMMATRIX XMMatrixRotationZ(float Angle)

    afxV2d cosSin;
    AfxCosSin(angle, cosSin);

    AfxResetM4d(m);

    m[0][0] = cosSin[0];
    m[0][1] = cosSin[1];
    m[1][0] = -cosSin[1];
    m[1][1] = cosSin[0];
}

// Rotate
// Memory layout: looks like it is row-major

_AFXINL void AfxRotateM3d_Axial(afxReal m[3][3], afxReal const axis[3], afxReal radians)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(axis);

    afxM3d tmp, tmp2;
    AfxCopyM3d(tmp, m);
    AfxM3dFromAxisAngle(tmp2, axis, radians);
    AfxMultiplyM3d(m, tmp2, tmp);
}

_AFXINL void AfxRotateM4d_Axial(afxReal m[4][4], afxReal const axis[3], afxReal radians)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(axis);

    afxM4d tmp, tmp2;
    AfxCopyM4d(tmp, m);
    AfxM4dFromAxisAngle(tmp2, axis, radians);
    AfxMultiplyAffineM4d(m, tmp2, tmp);
}

// Translate

_AFXINL void AfxTranslateM4d(afxReal m[4][4], afxReal const translation[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(translation);

    afxM4d tmp, tmp2;
    AfxCopyM4d(tmp, m);
    AfxResetM4d(tmp2);
    AfxCopyV4d(tmp2[3], translation);
    AfxMultiplyM4d(m, tmp2, tmp);
}

_AFXINL void AfxTranslateAffineM4d(afxReal m[4][4], afxReal const translation[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(translation);

    afxM4d tmp, tmp2;
    AfxCopyM4d(tmp, m);
    AfxResetM4d(tmp2);
    AfxCopyV3d(tmp2[3], translation);
    AfxMultiplyAffineM4d(m, tmp2, tmp);
}

_AFXINL void AfxMakeTranslationM4d(afxReal m[4][4], afxReal const translation[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(translation);
    AfxResetM4d(m);
    AfxCopyV3d(m[3], translation);    
}

// Decompose

_AFXINL afxBool AfxPolarDecomposeM3d(afxReal const m[3][3], afxReal tol, afxReal rm[3][3], afxReal ssm[3][3])
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

    afxNat v13 = 0;

    while (1)
    {
        AfxInvertM3d(ssm, rm);
        AfxAddM3d(ssm, ssm, rm);

        AfxScaleV3d(ssm[0], ssm[0], 0.5);
        AfxScaleV3d(ssm[1], ssm[1], 0.5);
        AfxScaleV3d(ssm[2], ssm[2], 0.5);

        afxM3d ssMinR;
        AfxSubM3d(ssMinR, ssm, rm);

        AfxCopyM3d(rm, ssm);

        if (fabs(ssMinR[0][0]) + fabs(ssMinR[0][1]) + fabs(ssMinR[0][2]) + fabs(ssMinR[1][0]) + fabs(ssMinR[1][1]) + fabs(ssMinR[1][2]) + fabs(ssMinR[2][0]) + fabs(ssMinR[2][1]) + fabs(ssMinR[2][2]) < tol)
            break;

        if (++v13 >= 1000)
        {
            return FALSE;
        }
    }
    return TRUE;
}

// Compose

_AFXINL void AfxComposeM4d(afxReal m[4][4], afxReal const scalOrigin[3], afxQuat const scalOrient, afxReal const scaling[3], afxReal const rotOrigin[3], afxQuat const rotQuat, afxReal const translation[3])
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
    AfxNegateV3d(negScaOrigin, scalOrigin);

    afxM4d scalOriginI, scalOrientM, scalOrientT, scalM, rotM;
    AfxResetM4d(scalOriginI);
    AfxCopyV3d(scalOriginI[3], negScaOrigin);
    AfxM4dFromQuat(scalOrientM, scalOrient);
    AfxTransposeM4d(scalOrientT, scalOrientM);
    AfxResetM4d(scalM);
    AfxSetM4dScale(scalM, scaling);
    AfxM4dFromQuat(rotM, rotQuat);

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

_AFXINL void AfxComposeAffineM4d(afxReal m[4][4], afxReal const scale[3], afxReal const rotAxis[3], afxQuat const rot, afxReal const translation[3])
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
    AfxSetM4dScale(sm, scale);
    AfxM4dFromQuat(rm, rot);
    AfxSubV3d(sm[3], sm[3], rotAxis);
    AfxMultiplyM4d(m, sm, rm);
    AfxAddV3d(m[3], m[3], rotAxis);
    AfxAddV3d(m[3], m[3], translation);
}

////////////////////////////////////////////////////////////////////////////////
// MATRIX TRANSFORMATION METHODS                                              //
////////////////////////////////////////////////////////////////////////////////

// Multiply

_AFXINL void AfxMultiplyM2d(afxReal m[2][2], afxReal const a[2][2], afxReal const b[2][2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(a);
    AfxAssert(b);
    AfxAssert(a != b);
    AfxAssert(b != m);

    // In post-multiplication, each component of the result, at a given row R and column C, is computed by taking the dot product of the left matrix’s row R by the right matrix’s column C.

    m[0][0] = (a[0][0] * b[0][0]) + (a[0][1] * b[1][0]);
    m[0][1] = (a[1][0] * b[0][0]) + (a[1][1] * b[1][0]);

    m[1][0] = (a[0][0] * b[0][1]) + (a[0][1] * b[1][1]);
    m[1][1] = (a[1][0] * b[0][1]) + (a[1][1] * b[1][1]);
}

_AFXINL void AfxMultiplyM3d(afxReal m[3][3], afxReal const a[3][3], afxReal const b[3][3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(a);
    AfxAssert(b);
    AfxAssert(a != b);
    AfxAssert(b != m);

    // In post-multiplication, each component of the result, at a given row R and column C, is computed by taking the dot product of the left matrix’s row R by the right matrix’s column C.

    // Compatible with MatrixMultiply3x3(m, a, b)

    m[0][0] = (a[0][0] * b[0][0]) + (a[0][1] * b[1][0]) + (a[0][2] * b[2][0]);
    m[0][1] = (a[1][0] * b[0][0]) + (a[1][1] * b[1][0]) + (a[1][2] * b[2][0]);
    m[0][2] = (a[2][0] * b[0][0]) + (a[2][1] * b[1][0]) + (a[2][2] * b[2][0]);

    m[1][0] = (a[0][0] * b[0][1]) + (a[0][1] * b[1][1]) + (a[0][2] * b[2][1]);
    m[1][1] = (a[1][0] * b[0][1]) + (a[1][1] * b[1][1]) + (a[1][2] * b[2][1]);
    m[1][2] = (a[2][0] * b[0][1]) + (a[2][1] * b[1][1]) + (a[2][2] * b[2][1]);

    m[2][0] = (a[0][0] * b[0][2]) + (a[0][1] * b[1][2]) + (a[0][2] * b[2][2]);
    m[2][1] = (a[1][0] * b[0][2]) + (a[1][1] * b[1][2]) + (a[1][2] * b[2][2]);
    m[2][2] = (a[2][0] * b[0][2]) + (a[2][1] * b[1][2]) + (a[2][2] * b[2][2]);
}

_AFXINL void AfxMultiplyM4d(afxReal m[4][4], afxReal const a[4][4], afxReal const b[4][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(a);
    AfxAssert(b);
    AfxAssert(a != b);
    AfxAssert(b != m);

    // In post-multiplication, each component of the result, at a given row R and column C, is computed by taking the dot product of the left matrix’s row R by the right matrix’s column C.

    m[0][0] = (a[0][0] * b[0][0]) + (a[0][1] * b[1][0]) + (a[0][2] * b[2][0]) + (a[0][3] * b[3][0]);
    m[0][1] = (a[1][0] * b[0][0]) + (a[1][1] * b[1][0]) + (a[1][2] * b[2][0]) + (a[1][3] * b[3][0]);
    m[0][2] = (a[2][0] * b[0][0]) + (a[2][1] * b[1][0]) + (a[2][2] * b[2][0]) + (a[2][3] * b[3][0]);
    m[0][3] = (a[3][0] * b[0][0]) + (a[3][1] * b[1][0]) + (a[3][2] * b[2][0]) + (a[3][3] * b[3][0]);

    m[1][0] = (a[0][0] * b[0][1]) + (a[0][1] * b[1][1]) + (a[0][2] * b[2][1]) + (a[0][3] * b[3][1]);
    m[1][1] = (a[1][0] * b[0][1]) + (a[1][1] * b[1][1]) + (a[1][2] * b[2][1]) + (a[1][3] * b[3][1]);
    m[1][2] = (a[2][0] * b[0][1]) + (a[2][1] * b[1][1]) + (a[2][2] * b[2][1]) + (a[2][3] * b[3][1]);
    m[1][3] = (a[3][0] * b[0][1]) + (a[3][1] * b[1][1]) + (a[3][2] * b[2][1]) + (a[3][3] * b[3][1]);

    m[2][0] = (a[0][0] * b[0][2]) + (a[0][1] * b[1][2]) + (a[0][2] * b[2][2]) + (a[0][3] * b[3][2]);
    m[2][1] = (a[1][0] * b[0][2]) + (a[1][1] * b[1][2]) + (a[1][2] * b[2][2]) + (a[1][3] * b[3][2]);
    m[2][2] = (a[2][0] * b[0][2]) + (a[2][1] * b[1][2]) + (a[2][2] * b[2][2]) + (a[2][3] * b[3][2]);
    m[2][3] = (a[3][0] * b[0][2]) + (a[3][1] * b[1][2]) + (a[3][2] * b[2][2]) + (a[3][3] * b[3][2]);

    m[3][0] = (a[0][0] * b[0][3]) + (a[0][1] * b[1][3]) + (a[0][2] * b[2][3]) + (a[0][3] * b[3][3]);
    m[3][1] = (a[1][0] * b[0][3]) + (a[1][1] * b[1][3]) + (a[1][2] * b[2][3]) + (a[1][3] * b[3][3]);
    m[3][2] = (a[2][0] * b[0][3]) + (a[2][1] * b[1][3]) + (a[2][2] * b[2][3]) + (a[2][3] * b[3][3]);
    m[3][3] = (a[3][0] * b[0][3]) + (a[3][1] * b[1][3]) + (a[3][2] * b[2][3]) + (a[3][3] * b[3][3]);
}

// MultiplyPlanar/Linear/Affine

_AFXINL void AfxMultiplyPlanarM3d(afxReal m[3][3], afxReal const a[3][3], afxReal const b[3][3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(a);
    AfxAssert(b);
    AfxAssert(a != b);
    AfxAssert(b != m);

    // In post-multiplication, each component of the result, at a given row R and column C, is computed by taking the dot product of the left matrix’s row R by the right matrix’s column C.

    m[0][0] = (a[0][0] * b[0][0]) + (a[0][1] * b[1][0]) + (a[0][2] * b[2][0]);
    m[0][1] = (a[1][0] * b[0][0]) + (a[1][1] * b[1][0]) + (a[1][2] * b[2][0]);
    m[0][2] = 0.f;
    m[1][0] = (a[0][0] * b[0][1]) + (a[0][1] * b[1][1]) + (a[0][2] * b[2][1]);
    m[1][1] = (a[1][0] * b[0][1]) + (a[1][1] * b[1][1]) + (a[1][2] * b[2][1]);
    m[1][2] = 0.f;
    m[2][0] = (a[0][0] * b[0][2]) + (a[0][1] * b[1][2]) + (a[0][2] * b[2][2]);
    m[2][1] = (a[1][0] * b[0][2]) + (a[1][1] * b[1][2]) + (a[1][2] * b[2][2]);
    m[2][2] = 1.f;
}

_AFXINL void AfxMultiplyLinearM4d(afxReal m[4][4], afxReal const a[4][4], afxReal const b[4][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(a);
    AfxAssert(b);
    AfxAssert(a != b);
    AfxAssert(b != m);

    // In post-multiplication, each component of the result, at a given row R and column C, is computed by taking the dot product of the left matrix’s row R by the right matrix’s column C.

    m[0][0] = (a[0][0] * b[0][0]) + (a[0][1] * b[1][0]) + (a[0][2] * b[2][0]);
    m[0][1] = (a[1][0] * b[0][0]) + (a[1][1] * b[1][0]) + (a[1][2] * b[2][0]);
    m[0][2] = (a[2][0] * b[0][0]) + (a[2][1] * b[1][0]) + (a[2][2] * b[2][0]);
    m[0][3] = 0.f;
    m[1][0] = (a[0][0] * b[0][1]) + (a[0][1] * b[1][1]) + (a[0][2] * b[2][1]);
    m[1][1] = (a[1][0] * b[0][1]) + (a[1][1] * b[1][1]) + (a[1][2] * b[2][1]);
    m[1][2] = (a[2][0] * b[0][1]) + (a[2][1] * b[1][1]) + (a[2][2] * b[2][1]);
    m[1][3] = 0.f;
    m[2][0] = (a[0][0] * b[0][2]) + (a[0][1] * b[1][2]) + (a[0][2] * b[2][2]);
    m[2][1] = (a[1][0] * b[0][2]) + (a[1][1] * b[1][2]) + (a[1][2] * b[2][2]);
    m[2][2] = (a[2][0] * b[0][2]) + (a[2][1] * b[1][2]) + (a[2][2] * b[2][2]);
    m[2][3] = 0.f;
    m[3][0] = 0.f;
    m[3][1] = 0.f;
    m[3][2] = 0.f;
    m[3][3] = 1.f;
}

_AFXINL void AfxMultiplyAffineM4d(afxReal m[4][4], afxReal const a[4][4], afxReal const b[4][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(a);
    AfxAssert(b);
    AfxAssert(a != b);
    AfxAssert(b != m);

    // In post-multiplication, each component of the result, at a given row R and column C, is computed by taking the dot product of the left matrix’s row R by the right matrix’s column C.

    m[0][0] = (a[0][0] * b[0][0]) + (a[0][1] * b[1][0]) + (a[0][2] * b[2][0]);
    m[0][1] = (a[1][0] * b[0][0]) + (a[1][1] * b[1][0]) + (a[1][2] * b[2][0]);
    m[0][2] = (a[2][0] * b[0][0]) + (a[2][1] * b[1][0]) + (a[2][2] * b[2][0]);
    m[0][3] = 0.f;
    m[1][0] = (a[0][0] * b[0][1]) + (a[0][1] * b[1][1]) + (a[0][2] * b[2][1]);
    m[1][1] = (a[1][0] * b[0][1]) + (a[1][1] * b[1][1]) + (a[1][2] * b[2][1]);
    m[1][2] = (a[2][0] * b[0][1]) + (a[2][1] * b[1][1]) + (a[2][2] * b[2][1]);
    m[1][3] = 0.f;
    m[2][0] = (a[0][0] * b[0][2]) + (a[0][1] * b[1][2]) + (a[0][2] * b[2][2]);
    m[2][1] = (a[1][0] * b[0][2]) + (a[1][1] * b[1][2]) + (a[1][2] * b[2][2]);
    m[2][2] = (a[2][0] * b[0][2]) + (a[2][1] * b[1][2]) + (a[2][2] * b[2][2]);
    m[2][3] = 0.f;
    m[3][0] = (a[0][0] * b[0][3]) + (a[0][1] * b[1][3]) + (a[0][2] * b[2][3]) + a[0][3];
    m[3][1] = (a[1][0] * b[0][3]) + (a[1][1] * b[1][3]) + (a[1][2] * b[2][3]) + a[1][3];
    m[3][2] = (a[2][0] * b[0][3]) + (a[2][1] * b[1][3]) + (a[2][2] * b[2][3]) + a[2][3];
    m[3][3] = 1.f;
}

// MultiplyTransposed

_AFXINL void AfxMultiplyTransposedM2d(afxReal m[2][2], afxReal const a[2][2], afxReal const b[2][2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(a);
    AfxAssert(b);
    AfxAssert(a != b);
    AfxAssert(b != m);

    m[0][0] = (a[0][0] * b[0][0]) + (a[0][1] * b[1][0]);
    m[0][1] = (a[0][0] * b[0][1]) + (a[0][1] * b[1][1]);

    m[1][0] = (a[1][0] * b[0][0]) + (a[1][1] * b[1][0]);
    m[1][1] = (a[1][0] * b[0][1]) + (a[1][1] * b[1][1]);
}

_AFXINL void AfxMultiplyTransposedM3d(afxReal m[3][3], afxReal const a[3][3], afxReal const b[3][3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(a);
    AfxAssert(b);
    AfxAssert(a != b);
    AfxAssert(b != m);

    // Compatible with TransposeMatrixMultiply3x3(m, a, b)

    m[0][0] = (a[0][0] * b[0][0]) + (a[0][1] * b[1][0]) + (a[0][2] * b[2][0]);
    m[0][1] = (a[0][0] * b[0][1]) + (a[0][1] * b[1][1]) + (a[0][2] * b[2][1]);
    m[0][2] = (a[0][0] * b[0][2]) + (a[0][1] * b[1][2]) + (a[0][2] * b[2][2]);

    m[1][0] = (a[1][0] * b[0][0]) + (a[1][1] * b[1][0]) + (a[1][2] * b[2][0]);
    m[1][1] = (a[1][0] * b[0][1]) + (a[1][1] * b[1][1]) + (a[1][2] * b[2][1]);
    m[1][2] = (a[1][0] * b[0][2]) + (a[1][1] * b[1][2]) + (a[1][2] * b[2][2]);

    m[2][0] = (a[2][0] * b[0][0]) + (a[2][1] * b[1][0]) + (a[2][2] * b[2][0]);
    m[2][1] = (a[2][0] * b[0][1]) + (a[2][1] * b[1][1]) + (a[2][2] * b[2][1]);
    m[2][2] = (a[2][0] * b[0][2]) + (a[2][1] * b[1][2]) + (a[2][2] * b[2][2]);
}

_AFXINL void AfxMultiplyTransposedM4d(afxReal m[4][4], afxReal const a[4][4], afxReal const b[4][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(a);
    AfxAssert(b);
    AfxAssert(a != b);
    AfxAssert(b != m);

    m[0][0] = (a[0][0] * b[0][0]) + (a[0][1] * b[1][0]) + (a[0][2] * b[2][0]) + (a[0][3] * b[3][0]);
    m[0][1] = (a[0][0] * b[0][1]) + (a[0][1] * b[1][1]) + (a[0][2] * b[2][1]) + (a[0][3] * b[3][1]);
    m[0][2] = (a[0][0] * b[0][2]) + (a[0][1] * b[1][2]) + (a[0][2] * b[2][2]) + (a[0][3] * b[3][2]);
    m[0][3] = (a[0][0] * b[0][3]) + (a[0][1] * b[1][3]) + (a[0][2] * b[2][3]) + (a[0][3] * b[3][3]);

    m[1][0] = (a[1][0] * b[0][0]) + (a[1][1] * b[1][0]) + (a[1][2] * b[2][0]) + (a[1][3] * b[3][0]);
    m[1][1] = (a[1][0] * b[0][1]) + (a[1][1] * b[1][1]) + (a[1][2] * b[2][1]) + (a[1][3] * b[3][1]);
    m[1][2] = (a[1][0] * b[0][2]) + (a[1][1] * b[1][2]) + (a[1][2] * b[2][2]) + (a[1][3] * b[3][2]);
    m[1][3] = (a[1][0] * b[0][3]) + (a[1][1] * b[1][3]) + (a[1][2] * b[2][3]) + (a[1][3] * b[3][3]);

    m[2][0] = (a[2][0] * b[0][0]) + (a[2][1] * b[1][0]) + (a[2][2] * b[2][0]) + (a[2][3] * b[3][0]);
    m[2][1] = (a[2][0] * b[0][1]) + (a[2][1] * b[1][1]) + (a[2][2] * b[2][1]) + (a[2][3] * b[3][1]);
    m[2][2] = (a[2][0] * b[0][2]) + (a[2][1] * b[1][2]) + (a[2][2] * b[2][2]) + (a[2][3] * b[3][2]);
    m[2][3] = (a[2][0] * b[0][3]) + (a[2][1] * b[1][3]) + (a[2][2] * b[2][3]) + (a[2][3] * b[3][3]);

    m[3][0] = (a[3][0] * b[0][0]) + (a[3][1] * b[1][0]) + (a[3][2] * b[2][0]) + (a[3][3] * b[3][0]);
    m[3][1] = (a[3][0] * b[0][1]) + (a[3][1] * b[1][1]) + (a[3][2] * b[2][1]) + (a[3][3] * b[3][1]);
    m[3][2] = (a[3][0] * b[0][2]) + (a[3][1] * b[1][2]) + (a[3][2] * b[2][2]) + (a[3][3] * b[3][2]);
    m[3][3] = (a[3][0] * b[0][3]) + (a[3][1] * b[1][3]) + (a[3][2] * b[2][3]) + (a[3][3] * b[3][3]);
}

_AFXINL void AfxMultiplyTransposedPlanarM3d(afxReal m[3][3], afxReal const a[3][3], afxReal const b[3][3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(a);
    AfxAssert(b);
    AfxAssert(a != b);
    AfxAssert(b != m);

    m[0][0] = (a[0][0] * b[0][0]) + (a[0][1] * b[1][0]) + (a[0][2] * b[2][0]);
    m[0][1] = (a[0][0] * b[0][1]) + (a[0][1] * b[1][1]) + (a[0][2] * b[2][1]);
    m[0][2] = 0.f;
    m[1][0] = (a[1][0] * b[0][0]) + (a[1][1] * b[1][0]) + (a[1][2] * b[2][0]);
    m[1][1] = (a[1][0] * b[0][1]) + (a[1][1] * b[1][1]) + (a[1][2] * b[2][1]);
    m[1][2] = 0.f;
    m[2][0] = (a[2][0] * b[0][0]) + (a[2][1] * b[1][0]) + (a[2][2] * b[2][0]);
    m[2][1] = (a[2][0] * b[0][1]) + (a[2][1] * b[1][1]) + (a[2][2] * b[2][1]);
    m[2][2] = 1.f;
}

_AFXINL void AfxMultiplyTransposedLinearM4d(afxReal m[4][4], afxReal const a[4][4], afxReal const b[4][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(a);
    AfxAssert(b);
    AfxAssert(a != b);
    AfxAssert(b != m);

    m[0][0] = (a[0][0] * b[0][0]) + (a[0][1] * b[1][0]) + (a[0][2] * b[2][0]);
    m[0][1] = (a[0][0] * b[0][1]) + (a[0][1] * b[1][1]) + (a[0][2] * b[2][1]);
    m[0][2] = (a[0][0] * b[0][2]) + (a[0][1] * b[1][2]) + (a[0][2] * b[2][2]);
    m[0][3] = 0.f;
    m[1][0] = (a[1][0] * b[0][0]) + (a[1][1] * b[1][0]) + (a[1][2] * b[2][0]);
    m[1][1] = (a[1][0] * b[0][1]) + (a[1][1] * b[1][1]) + (a[1][2] * b[2][1]);
    m[1][2] = (a[1][0] * b[0][2]) + (a[1][1] * b[1][2]) + (a[1][2] * b[2][2]);
    m[1][3] = 0.f;
    m[2][0] = (a[2][0] * b[0][0]) + (a[2][1] * b[1][0]) + (a[2][2] * b[2][0]);
    m[2][1] = (a[2][0] * b[0][1]) + (a[2][1] * b[1][1]) + (a[2][2] * b[2][1]);
    m[2][2] = (a[2][0] * b[0][2]) + (a[2][1] * b[1][2]) + (a[2][2] * b[2][2]);
    m[2][3] = 0.f;
    m[3][0] = 0.f;
    m[3][1] = 0.f;
    m[3][2] = 0.f;
    m[3][3] = 1.f;
}

_AFXINL void AfxMultiplyTransposedAffineM4d(afxReal m[4][4], afxReal const a[4][4], afxReal const b[4][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(a);
    AfxAssert(b);
    AfxAssert(a != b);
    AfxAssert(b != m);

    // Compatible with ColumnMatrixMultiply4x3(m, a, b)

    m[0][0] = (a[0][0] * b[0][0]) + (a[0][1] * b[1][0]) + (a[0][2] * b[2][0]);
    m[0][1] = (a[0][0] * b[0][1]) + (a[0][1] * b[1][1]) + (a[0][2] * b[2][1]);
    m[0][2] = (a[0][0] * b[0][2]) + (a[0][1] * b[1][2]) + (a[0][2] * b[2][2]);
    m[0][3] = 0.f;
    m[1][0] = (a[1][0] * b[0][0]) + (a[1][1] * b[1][0]) + (a[1][2] * b[2][0]);
    m[1][1] = (a[1][0] * b[0][1]) + (a[1][1] * b[1][1]) + (a[1][2] * b[2][1]);
    m[1][2] = (a[1][0] * b[0][2]) + (a[1][1] * b[1][2]) + (a[1][2] * b[2][2]);
    m[1][3] = 0.f;
    m[2][0] = (a[2][0] * b[0][0]) + (a[2][1] * b[1][0]) + (a[2][2] * b[2][0]);
    m[2][1] = (a[2][0] * b[0][1]) + (a[2][1] * b[1][1]) + (a[2][2] * b[2][1]);
    m[2][2] = (a[2][0] * b[0][2]) + (a[2][1] * b[1][2]) + (a[2][2] * b[2][2]);
    m[2][3] = 0.f;
    m[3][0] = (a[3][0] * b[0][0]) + (a[3][1] * b[1][0]) + (a[3][2] * b[2][0]) + b[3][0];
    m[3][1] = (a[3][0] * b[0][1]) + (a[3][1] * b[1][1]) + (a[3][2] * b[2][1]) + b[3][1];
    m[3][2] = (a[3][0] * b[0][2]) + (a[3][1] * b[1][2]) + (a[3][2] * b[2][2]) + b[3][2];
    m[3][3] = 1.f;
}

// MultiplyArrayedV2d

_AFXINL void AfxPostMultiplyArrayedV2d(afxReal const m[2][2], afxNat cnt, afxReal const in[][2], afxReal out[][2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
        AfxPostMultiplyV2d(out[i], m, in[i]);
}

_AFXINL void AfxPostMultiplyArrayedV3d(afxReal const m[3][3], afxNat cnt, afxReal const in[][3], afxReal out[][3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
        AfxPostMultiplyV3d(out[i], m, in[i]);
}

_AFXINL void AfxPostMultiplyArrayedV4d(afxReal const m[4][4], afxNat cnt, afxReal const in[][4], afxReal out[][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
        AfxPostMultiplyV4d(out[i], m, in[i]);
}

_AFXINL void AfxPostMultiplyArrayedPointV3d(afxReal const m[4][4], afxNat cnt, afxReal const in[][3], afxReal out[][3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
        AfxPostMultiplyPointV3d(out[i], m, in[i]);
}

_AFXINL void AfxPostMultiplyArrayedNormalV4d(afxReal const m[3][3], afxNat cnt, afxReal const in[][4], afxReal out[][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
        AfxPostMultiplyNormalV4d(out[i], m, in[i]);
}

_AFXINL void AfxPostMultiplyArrayedAffineV4d(afxReal const m[4][4], afxNat cnt, afxReal const in[][4], afxReal out[][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
        AfxPostMultiplyAffineV4d(out[i], m, in[i]);
}

// PreMultiply

_AFXINL void AfxPreMultiplyArrayedV2d(afxReal const m[2][2], afxNat cnt, afxReal const in[][2], afxReal out[][2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
        AfxPreMultiplyV2d(out[i], in[i], m);
}

_AFXINL void AfxPreMultiplyArrayedV3d(afxReal const m[3][3], afxNat cnt, afxReal const in[][3], afxReal out[][3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
        AfxPreMultiplyV3d(out[i], in[i], m);
}

_AFXINL void AfxPreMultiplyArrayedV4d(afxReal const m[4][4], afxNat cnt, afxReal const in[][4], afxReal out[][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
        AfxPreMultiplyV4d(out[i], in[i], m);
}

_AFXINL void AfxPreMultiplyArrayedPointV3d(afxReal const m[4][4], afxNat cnt, afxReal const in[][3], afxReal out[][3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
        AfxPreMultiplyPointV3d(out[i], in[i], m);
}

_AFXINL void AfxPreMultiplyArrayedNormalV4d(afxReal const m[3][3], afxNat cnt, afxReal const in[][4], afxReal out[][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
        AfxPreMultiplyNormalV4d(out[i], in[i], m);
}

_AFXINL void AfxPreMultiplyArrayedAffineV4d(afxReal const m[4][4], afxNat cnt, afxReal const in[][4], afxReal out[][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
        AfxPreMultiplyAffineV4d(out[i], in[i], m);
}

// TransformSerialized

_AFXINL void AfxPostMultiplySerializedV2d(afxReal const m[2][2], afxNat inStride, afxNat outStride, afxNat cnt, afxReal const in[][2], afxReal out[][2])
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
        AfxPostMultiplyV2d((void*)outPtr, m, (void*)inPtr);

        inPtr += inStride;
        outPtr += outStride;
    }
}

_AFXINL void AfxPostMultiplySerializedV3d(afxReal const m[3][3], afxNat inStride, afxNat outStride, afxNat cnt, afxReal const in[][3], afxReal out[][3])
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
        AfxPostMultiplyV3d((void*)outPtr, m, (void*)inPtr);

        inPtr += inStride;
        outPtr += outStride;
    }
}

_AFXINL void AfxPostMultiplySerializedV4d(afxReal const m[4][4], afxNat inStride, afxNat outStride, afxNat cnt, afxReal const in[][4], afxReal out[][4])
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
        AfxPostMultiplyV4d((void*)outPtr, m, (void*)inPtr);

        inPtr += inStride;
        outPtr += outStride;
    }
}

_AFXINL void AfxPostMultiplySerializedPointV3d(afxReal const m[4][4], afxNat inStride, afxNat outStride, afxNat cnt, afxReal const in[][3], afxReal out[][3])
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
        AfxPostMultiplyPointV3d((void*)outPtr, m, (void*)inPtr);

        inPtr += inStride;
        outPtr += outStride;
    }
}

_AFXINL void AfxPostMultiplySerializedNormalV4d(afxReal const m[3][3], afxNat inStride, afxNat outStride, afxNat cnt, afxReal const in[][4], afxReal out[][4])
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
        AfxPostMultiplyNormalV4d((void*)outPtr, m, (void*)inPtr);

        inPtr += inStride;
        outPtr += outStride;
    }
}

_AFXINL void AfxPostMultiplySerializedAffineV4d(afxReal const m[4][4], afxNat inStride, afxNat outStride, afxNat cnt, afxReal const in[][4], afxReal out[][4])
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
        AfxPostMultiplyAffineV4d((void*)outPtr, m, (void*)inPtr);

        inPtr += inStride;
        outPtr += outStride;
    }
}

// Assimilate

_AFXINL void AfxAssimilateM3d(afxReal const lt[3][3], afxReal const ilt[3][3], afxNat cnt, afxReal const in[][3][3], afxReal out[][3][3])
{
    // Should be compatible with void InPlaceSimilarityTransformScaleShear(const float *Linear3x3, const float *InverseLinear3x3, float *ScaleShear3x3)
    afxError err = NIL;
    AfxAssert(lt);
    AfxAssert(ilt);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxM3d tmp;
        AfxMultiplyM3d(tmp, lt, in[i]);
        AfxMultiplyM3d(out[i], tmp, ilt);
    }
}

_AFXINL void AfxAssimilateAffineM4d(afxReal const lt[3][3], afxReal const ilt[3][3], afxReal const at[3], afxNat cnt, afxReal const in[][4][4], afxReal out[][4][4])
{
    // Should be compatible with void InPlaceSimilarityTransform4x3(const float *Affine3, const float *Linear3x3, const float *InverseLinear3x3, float *ResultInit)
    afxError err = NIL;
    AfxAssert(at);
    AfxAssert(lt);
    AfxAssert(ilt);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxM3d ss;
        AfxM3dFromM4d(ss, in[i]);
        AfxAssimilateM3d(lt, ilt, 1, &ss, &ss);
        afxV4d pos;
        AfxAssimilatePointV4d(lt, at, 1, &in[i][3], &pos);
        AfxM4dFromM3d(out[i], ss, pos);
    }
}

_AFX void AfxApplyRootMotionVectorsToMatrix(afxReal const translation[3], afxReal const rotation[3], afxReal const mm[4][4], afxReal m[4][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(translation);
    AfxAssert(rotation);
    AfxAssert(mm); // model matrix
    AfxAssert(m);
    afxM4d upd;
    afxReal rl = AfxMagV3d(rotation);

    if (rl <= 0.0) AfxMakeTranslationM4d(upd, translation);
    else
    {
        afxV3d scaledRot;
        AfxScaleV3d(scaledRot, rotation, 1.f / rl);
        AfxM4dFromAxisAngle(upd, scaledRot, rl);
        upd[3][0] = translation[0];
        upd[3][1] = translation[1];
        upd[3][2] = translation[2];
    }

    afxM4d tmp;
    AfxMultiplyTransposedAffineM4d(tmp, upd, mm);
    AfxCopyM4d(m, tmp);
}
