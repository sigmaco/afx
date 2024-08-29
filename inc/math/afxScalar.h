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

#ifndef AFX_SCALAR_H
#define AFX_SCALAR_H

#include "qwadro/inc/math/afxMathDefs.h"

AFXINL afxBool      AfxIsGreaterThanEpsilon(afxReal value);

AFXINL afxBool      AfxIsNan(afxReal value);

AFXINL afxBool      AfxIsInfinite(afxReal value);

AFXINL afxBool      AfxRealIsEquivalent(afxReal a, afxReal b);

AFXINL afxBool      AfxRealIsEqual(afxReal a, afxReal b, afxReal epsilon);

AFXINL afxReal64    AfxRecip(afxReal64 value);
AFXINL afxReal      AfxRecipf(afxReal value);

AFXINL afxReal64    AfxLerp(afxReal64 a, afxReal64 b, afxReal64 time);
AFXINL afxReal      AfxLerpf(afxReal a, afxReal b, afxReal time);

AFXINL afxReal64    AfxClampd(afxReal64 value, afxReal64 min, afxReal64 max);
AFXINL afxReal      AfxClampf(afxReal value, afxReal min, afxReal max);

AFXINL afxReal64    AfxMod(afxReal64 x, afxReal64 y);
AFXINL afxReal      AfxModf(afxReal x, afxReal y);

AFXINL afxReal64    AfxSq(afxReal64 s);
AFXINL afxReal      AfxSqf(afxReal s);

AFXINL afxReal64    AfxFloor(afxReal64 s);
AFXINL afxReal      AfxFloorf(afxReal s);

AFXINL afxReal64    AfxCeil(afxReal64 s);
AFXINL afxReal      AfxCeilf(afxReal s);

/*********************************************************************************
 * ANGLE AND TRIGONOMETRY FUNCTIONS                                              *
 *                                                                               *
 * Function parameters specified as angle are assumed to be in units of radians. *
 * In no case will any of these functions result in a divide by zero error.      *
 * If the divisor of a ratio is 0, then results will be undefined.               *
 *                                                                               *
 * These all operate component-wise. The description is per component.           *
 *********************************************************************************/

 // Converts degrees to radians.
 // The value returned by this function is (pi / 180) * degrees.

AFXINL afxReal64    AfxRad(afxReal64 degrees);
AFXINL afxReal      AfxRadf(afxReal degrees);

// Converts radians to degrees.
// The value returned by this function is (180 / pi) * radians.

AFXINL afxReal64    AfxDeg(afxReal64 radians);
AFXINL afxReal      AfxDegf(afxReal radians);

// The standard trigonometric sine function.

AFXINL afxReal64    AfxSin(afxReal64 radians);
AFXINL afxReal      AfxSinf(afxReal radians);

// The standard trigonometric cosine function.

AFXINL afxReal64    AfxCos(afxReal64 radians);
AFXINL afxReal      AfxCosf(afxReal radians);

// Combined cosine and sine

AFXINL void         AfxCosSin(afxReal64 radians, afxReal64* cos, afxReal64* sin);
AFXINL void         AfxCosSinf(afxReal radians, afxReal* cos, afxReal* sin);

// The standard trigonometric tangent function.

AFXINL afxReal64    AfxTan(afxReal64 radians);
AFXINL afxReal      AfxTanf(afxReal radians);

// Arc sine. Returns an angle whose sine is x.
// The range of values returned by this function is [-pi / 2, pi / 2].
// Results are undefined if |x| > 1.

AFXINL afxReal64    AfxAsin(afxReal64 x);
AFXINL afxReal      AfxAsinf(afxReal x);

// Arc cosine. Returns an angle whose cosine is x.
// The range of values returned by this function is [0, pi].
// Results are undefined if |x| > 1.

AFXINL afxReal64    AfxAcos(afxReal64 x);
AFXINL afxReal      AfxAcosf(afxReal x);

// Arc tangent. Returns an angle whose tangent is y_over_x.
// The range of values returned by this function is [-pi / 2, pi / 2].

AFXINL afxReal64    AfxAtan(afxReal64 y_over_x);
AFXINL afxReal      AfxAtanf(afxReal y_over_x);

// Arc tangent. Returns an angle whose tangent is y / x.
// The signs of x and y are used to determine what quadrant the angle is in.
// The range of values returned by this function is [-pi, pi].
// Results are undefined if xand y are both 0.

AFXINL afxReal64    AfxAtan2(afxReal64 y, afxReal64 x);
AFXINL afxReal      AfxAtan2f(afxReal y, afxReal x);

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


/***********************************************************************
 * EXPONENTIAL FUNCTIONS                                               *
 *                                                                     *
 * These all operate component-wise. The description is per component. *
 ***********************************************************************/

 // Returns the natural logarithm of x; returns the value y which satisfies the equation x = e^y.
 // Results are undefined if x <= 0.

AFXINL afxReal64    AfxLog(afxReal64 s);
AFXINL afxReal      AfxLogf(afxReal s);

// Returns the base 2 logarithm of x; returns the value y which satisfies the equation x = 2^y.
// Results are undefined if x <= 0.

AFXINL afxReal64    AfxLog2(afxReal64 s);
AFXINL afxReal      AfxLog2f(afxReal s);

AFXINL afxReal64    AfxLog10(afxReal64 s);
AFXINL afxReal      AfxLog10f(afxReal s);

// Returns the natural exponentiation of x; e^x.

AFXINL afxReal64    AfxExp(afxReal64 s);
AFXINL afxReal      AfxExpf(afxReal s);

// Returns 2 raised to the x power; 2^x.

AFXINL afxReal64    AfxExp2(afxReal64 s);
AFXINL afxReal      AfxExp2f(afxReal s);

// Returns x raised to the y power; x^y.
// Results are undefined if x < 0 or if x = 0 and y <= 0.

AFXINL afxReal64    AfxPow(afxReal64 base, afxReal64 exp);
AFXINL afxReal      AfxPowf(afxReal base, afxReal exp);

AFXINL afxReal64    AfxExp10(afxReal64 s);
AFXINL afxReal      AfxExp10f(afxReal s);

// Returns sqrt(x).
// Results are undefined if x < 0.

AFXINL afxReal64    AfxSqrt(afxReal64 s);
AFXINL afxReal      AfxSqrtf(afxReal s);

// Returns 1 / sqrt(x).
// Results are undefined if x <= 0.

AFXINL afxReal64    AfxRsqrt(afxReal64 s);
AFXINL afxReal      AfxRsqrtf(afxReal s);

AFXINL afxReal64    AfxNdc(afxReal64 x, afxReal64 total);
AFXINL afxReal      AfxNdcf(afxReal x, afxReal total);

AFXINL afxReal64    AfxUnndc(afxReal64 x, afxReal64 total);
AFXINL afxReal      AfxUnndcf(afxReal x, afxReal total);

#endif//AFX_SCALAR_H
