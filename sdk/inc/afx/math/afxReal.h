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

#ifndef AFX_SCALAR_H
#define AFX_SCALAR_H

#include "afxMathDefs.h"

AFXINL afxReal16 AfxReal32To16(afxReal f);
AFXINL afxReal32 AfxReal16To32(afxReal16 hf);

AFXINL afxInt16 AfxReal32ToSnorm(afxReal v);
AFXINL afxReal AfxSnorm16ToReal32(afxInt16 v);

AFXINL afxBool AfxIsGreaterThanEpsilon(afxReal value);

AFXINL afxBool AfxIsNan(afxReal value);

AFXINL afxBool AfxIsInfinite(afxReal value);

AFXINL afxBool AfxRealIsEquivalent(afxReal a, afxReal b);

AFXINL afxBool AfxRealIsEqual(afxReal a, afxReal b, afxReal epsilon);

AFXINL afxReal AfxRecip(afxReal value);

AFXINL afxReal AfxLerp(afxReal a, afxReal b, afxReal time);

AFXINL afxReal AfxClamp(afxReal value, afxReal min, afxReal max);

AFXINL afxReal AfxSq(afxReal s);

AFXINL afxReal AfxFloor(afxReal s);

AFXINL afxReal AfxCeil(afxReal s);

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

AFXINL afxReal AfxRad(afxReal degrees);

// Converts radians to degrees.
// The value returned by this function is (180 / pi) * radians.

AFXINL afxReal AfxDeg(afxReal radians);

// The standard trigonometric sine function.

AFXINL afxReal AfxSin(afxReal radians);

// The standard trigonometric cosine function.

AFXINL afxReal AfxCos(afxReal radians);

// Combined cosine and sine

AFXINL void AfxCosSin(afxReal radians, afxReal cosSin[2]);

// The standard trigonometric tangent function.

AFXINL afxReal AfxTan(afxReal radians);

// Arc sine. Returns an angle whose sine is x.
// The range of values returned by this function is [-pi / 2, pi / 2].
// Results are undefined if |x| > 1.

AFXINL afxReal AfxAsin(afxReal x);

// Arc cosine. Returns an angle whose cosine is x.
// The range of values returned by this function is [0, pi].
// Results are undefined if |x| > 1.

AFXINL afxReal AfxAcos(afxReal x);

// Arc tangent. Returns an angle whose tangent is y_over_x.
// The range of values returned by this function is [-pi / 2, pi / 2].

AFXINL afxReal AfxAtan(afxReal y_over_x);

// Arc tangent. Returns an angle whose tangent is y / x.
// The signs of x and y are used to determine what quadrant the angle is in.
// The range of values returned by this function is [-pi, pi].
// Results are undefined if xand y are both 0.

AFXINL afxReal AfxAtan2(afxReal y, afxReal x);

// Returns the hyperbolic sine function; (e^x - e^(-x)) / 2.

AFXINL afxReal AfxSinh(afxReal x);

// Returns the hyperbolic cosine function; (e^x + e^(-x)) / 2.

AFXINL afxReal AfxCosh(afxReal x);

// Returns the hyperbolic tangent function; sinh(x) / cosh(x).

AFXINL afxReal AfxTanh(afxReal x);

// Arc hyperbolic sine; returns the inverse of sinh.

AFXINL afxReal AfxAsinh(afxReal x);

// Arc hyperbolic cosine; returns the non-negative inverse of cosh.
// Results are undefined if x < 1.

AFXINL afxReal AfxAcosh(afxReal x);

// Arc hyperbolic tangent; returns the inverse of tanh.
// Results are undefined if x >= 1.

AFXINL afxReal AfxAtanh(afxReal y_over_x);


/***********************************************************************
 * EXPONENTIAL FUNCTIONS                                               *
 *                                                                     *
 * These all operate component-wise. The description is per component. *
 ***********************************************************************/

// Returns the natural logarithm of x; returns the value y which satisfies the equation x = e^y.
// Results are undefined if x <= 0.

AFXINL afxReal AfxLog(afxReal s);

// Returns the base 2 logarithm of x; returns the value y which satisfies the equation x = 2^y.
// Results are undefined if x <= 0.

AFXINL afxReal AfxLog2(afxReal s);

AFXINL afxReal AfxLog10(afxReal s);

// Returns the natural exponentiation of x; e^x.

AFXINL afxReal AfxExp(afxReal s);

// Returns 2 raised to the x power; 2^x.

AFXINL afxReal AfxExp2(afxReal s);

// Returns x raised to the y power; x^y.
// Results are undefined if x < 0 or if x = 0 and y <= 0.

AFXINL afxReal AfxPow(afxReal base, afxReal exp);

AFXINL afxReal AfxExp10(afxReal s);

// Returns sqrt(x).
// Results are undefined if x < 0.

AFXINL afxReal AfxSqrt(afxReal s);

// Returns 1 / sqrt(x).
// Results are undefined if x <= 0.

AFXINL afxReal AfxRsqrt(afxReal s);

#endif//AFX_SCALAR_H