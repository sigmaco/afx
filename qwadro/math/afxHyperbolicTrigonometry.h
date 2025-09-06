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

// This code is part of SIGMA Advanced Math Extensions for Qwadro

/*
    Hyperbolic Functions.
    These functions relate to hyperbolic angles and are analogous to trigonometric functions.

    Sinh: sinh
    Cosh: cosh
    Tanh: tanh
    Coth: coth
    Sech: sech
    Csch: csch
    Inverse Hyperbolic: asinh, acosh, atanh, acoth, asech, acsch
*/

#ifndef AFX_HYPERBOLIC_TRIGONOMETRY_H
#define AFX_HYPERBOLIC_TRIGONOMETRY_H

#include "qwadro/math/afxVector.h"

// Returns the hyperbolic sine function; (e^x - e^(-x)) / 2.

AFXINL afxReal64    AfxSinh(afxReal64 x);
AFXINL afxReal      AfxSinhf(afxReal x);

// Returns the hyperbolic cosine function; (e^x + e^(-x)) / 2.

AFXINL afxReal64    AfxCosh(afxReal64 x);
AFXINL afxReal      AfxCoshf(afxReal x);

// Returns the hyperbolic tangent function; sinh(x) / cosh(x).

AFXINL afxReal64    AfxTanh(afxReal64 x);
AFXINL afxReal      AfxTanhf(afxReal x);

// Arc hyperbolic sine; returns the inverse of sinh.

AFXINL afxReal64    AfxAsinh(afxReal64 x);
AFXINL afxReal      AfxAsinhf(afxReal x);

// Arc hyperbolic cosine; returns the non-negative inverse of cosh.
// Results are undefined if x < 1.

AFXINL afxReal64    AfxAcosh(afxReal64 x);
AFXINL afxReal      AfxAcoshf(afxReal x);

// Arc hyperbolic tangent; returns the inverse of tanh.
// Results are undefined if x >= 1.

AFXINL afxReal64    AfxAtanh(afxReal64 y_over_x);
AFXINL afxReal      AfxAtanhf(afxReal y_over_x);

#endif//AFX_HYPERBOLIC_TRIGONOMETRY_H
