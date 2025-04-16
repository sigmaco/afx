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
    Basic Arithmetic Functions
    These functions perform basic arithmetic operations.
    Addition: add, sum
    Subtraction: subtract
    Multiplication: multiply, product
    Division: divide, quotient
    Modulus: mod, remainder
    Exponentiation: pow, power, exponent
*/

#ifndef AFX_ARITHMETIC_H
#define AFX_ARITHMETIC_H

#include "qwadro/inc/math/afxVector.h"


////////////////////////////////////////////////////////////////////////////////
// Arithmetica                                                                //
////////////////////////////////////////////////////////////////////////////////

// v = a + b

AFXINL void     AfxV2dAdd(afxV2d v, afxV2d const a, afxV2d const b);
AFXINL void     AfxV3dAdd(afxV3d v, afxV3d const a, afxV3d const b);
AFXINL void     AfxV4dAdd(afxV4d v, afxV4d const a, afxV4d const b);

// v = a - b

AFXINL void     AfxV2dSub(afxV2d v, afxV2d const a, afxV2d const b); // v = a - b
AFXINL void     AfxV3dSub(afxV3d v, afxV3d const a, afxV3d const b); // v = a - b
AFXINL void     AfxV4dSub(afxV4d v, afxV4d const a, afxV4d const b); // v = a - b

// v = a / b

AFXINL void     AfxDivV2d(afxV2d v, afxV2d const a, afxV2d const b); // v = a / b
AFXINL void     AfxV3dDiv(afxV3d v, afxV3d const a, afxV3d const b); // v = a / b
AFXINL void     AfxDivV4d(afxV4d v, afxV4d const a, afxV4d const b); // v = a / b


// Divs

AFXINL void     AfxV2dDivs(afxV2d v, afxV2d const a, afxReal div);
AFXINL void     AfxV3dDivs(afxV3d v, afxV3d const a, afxReal div);
AFXINL void     AfxV4dDivs(afxV4d v, afxV4d const a, afxReal div);

// v = a % b

AFXINL void     AfxModV2d(afxV2d v, afxV2d const a, afxV2d const b); // v = a % b
AFXINL void     AfxModV3d(afxV3d v, afxV3d const a, afxV3d const b); // v = a % b
AFXINL void     AfxModV4d(afxV4d v, afxV4d const a, afxV4d const b); // v = a % b

// Scales (or multiplies) a vector or scalar by a constant factor.
// Resizing objects, scaling forces, enlarging or shrinking vectors.
// v = a * lambda

AFXINL void     AfxV2dScale(afxV2d v, afxV2d const a, afxReal lambda); // v = a * lambda
AFXINL void     AfxV3dScale(afxV3d v, afxV3d const a, afxReal lambda); // v = a * lambda
AFXINL void     AfxV4dScale(afxV4d v, afxV4d const a, afxReal lambda); // v = a * lambda

// v = a * b

AFXINL void     AfxV2dMultiply(afxV2d v, afxV2d const a, afxV2d const b); // v = a * b
AFXINL void     AfxV3dMultiply(afxV3d v, afxV3d const a, afxV3d const b); // v = a * b
AFXINL void     AfxV4dMultiply(afxV4d v, afxV4d const a, afxV4d const b); // v = a * b

// vector-multiply-add (accumulate)
// A common operation that performs (A * B) + C, combining multiplication and addition in a single step. 
// It is often used in shaders and numerical algorithms to reduce computation time.
// Efficient calculation in shaders, physics simulations, and financial models.
// v = a * b + c

AFXINL void     AfxV2dMad(afxV2d v, afxV2d const a, afxV2d const b, afxV2d const c); // v = a * b + c
AFXINL void     AfxV3dMad(afxV3d v, afxV3d const a, afxV3d const b, afxV3d const c); // v = a * b + c
AFXINL void     AfxV4dMad(afxV4d v, afxV4d const a, afxV4d const b, afxV4d const c); // v = a * b + c

// scalar-multiply-add (accumulate)
// A common operation that performs (A * B) + C, combining multiplication and addition in a single step. 
// It is often used in shaders and numerical algorithms to reduce computation time.
// Efficient calculation in shaders, physics simulations, and financial models.
// v = a + b * lambda

AFXINL void     AfxV2dMads(afxV2d v, afxReal lambda, afxV2d const b, afxV2d const c); // v = lambda * b + c
AFXINL void     AfxV3dMads(afxV3d v, afxReal lambda, afxV3d const b, afxV3d const c); // v = lambda * b + c
AFXINL void     AfxV4dMads(afxV4d v, afxReal lambda, afxV4d const b, afxV4d const c); // v = lambda * b + c

// reverse subtract
// v = c - (a * b)

AFXINL void     AfxResubV2d(afxV2d v, afxV2d const a, afxV2d const b, afxV2d const c); // v = c - (a * b)
AFXINL void     AfxResubV3d(afxV3d v, afxV3d const a, afxV3d const b, afxV3d const c); // v = c - (a * b)
AFXINL void     AfxResubV4d(afxV4d v, afxV4d const a, afxV4d const b, afxV4d const c); // v = c - (a * b)

// Mix
// The MIX function is conceptually similar to LERP but may involve additional functionality, such as more complex blending modes or different blending parameters. 
// This operation is very similar to lerp, but it often uses clamping or saturating the interpolation result. 
// It linearly blends two values (or vectors) based on a factor t.
// Mixing colors, blending two values in a weighted way.
// v = x * (1 - t) + y * t

AFXINL void     AfxV2dMix(afxV2d v, afxV2d const x, afxV2d const y, afxReal t);
AFXINL void     AfxV3dMix(afxV3d v, afxV3d const x, afxV3d const y, afxReal t);
AFXINL void     AfxV4dMix(afxV4d v, afxV4d const x, afxV4d const y, afxReal t);

// Lerp
// LERP is a method to interpolate linearly between two values. 
// In the context of 4D vectors, it calculates the intermediate vector between a start and end vector based on a factor t ranging from 0.0 to 1.0.
// Linearly interpolates between two values (or vectors) based on a parameter t (usually between 0 and 1). 
// It blends two values in a straight line.
// Animation, transition effects, smooth movement between two points.
// x + t * (y - x)

AFXINL void     AfxV2dLerp(afxV2d v, afxV2d const x, afxV2d const y, afxReal t);
AFXINL void     AfxV3dLerp(afxV3d v, afxV3d const x, afxV3d const y, afxReal t);
AFXINL void     AfxV4dLerp(afxV4d v, afxV4d const x, afxV4d const y, afxReal t);


#endif//AFX_ARITHMETIC_H
