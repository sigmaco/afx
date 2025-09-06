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

#include "qwadro/math/afxVector.h"


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

AFXINL void     AfxV2dDiv(afxV2d v, afxV2d const a, afxV2d const b); // v = a / b
AFXINL void     AfxV3dDiv(afxV3d v, afxV3d const a, afxV3d const b); // v = a / b
AFXINL void     AfxV4dDiv(afxV4d v, afxV4d const a, afxV4d const b); // v = a / b


// v = a / div

AFXINL void     AfxV2dDivs(afxV2d v, afxV2d const a, afxReal div);
AFXINL void     AfxV3dDivs(afxV3d v, afxV3d const a, afxReal div);
AFXINL void     AfxV4dDivs(afxV4d v, afxV4d const a, afxReal div);

// v = in * 0.5

AFXINL void     AfxV2dHalf(afxV2d v, afxV2d const in);
AFXINL void     AfxV3dHalf(afxV3d v, afxV3d const in);
AFXINL void     AfxV4dHalf(afxV4d v, afxV4d const in);

// v = a % b

AFXINL void     AfxV2dMod(afxV2d v, afxV2d const a, afxV2d const b); // v = a % b
AFXINL void     AfxV3dMod(afxV3d v, afxV3d const a, afxV3d const b); // v = a % b
AFXINL void     AfxV4dMod(afxV4d v, afxV4d const a, afxV4d const b); // v = a % b

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

// Scales (or multiplies) a vector or scalar by a constant factor.
// Resizing objects, scaling forces, enlarging or shrinking vectors.
// v = a * lambda

AFXINL void     AfxV2dScale(afxV2d v, afxV2d const a, afxReal lambda); // v = a * lambda
AFXINL void     AfxV3dScale(afxV3d v, afxV3d const a, afxReal lambda); // v = a * lambda
AFXINL void     AfxV4dScale(afxV4d v, afxV4d const a, afxReal lambda); // v = a * lambda

// reverse subtract
// v = c - (a * b)

AFXINL void     AfxV2dResub(afxV2d v, afxV2d const a, afxV2d const b, afxV2d const c); // v = c - (a * b)
AFXINL void     AfxV3dResub(afxV3d v, afxV3d const a, afxV3d const b, afxV3d const c); // v = c - (a * b)
AFXINL void     AfxV4dResub(afxV4d v, afxV4d const a, afxV4d const b, afxV4d const c); // v = c - (a * b)


AFXINL void     AfxV2dRecip(afxV2d v, afxV2d const in); // v = 1 / in
AFXINL void     AfxV3dRecip(afxV3d v, afxV3d const in); // v = 1 / in
AFXINL void     AfxV4dRecip(afxV4d v, afxV4d const in); // v = 1 / in

// Negates a vector or scalar, flipping its sign. For a vector, it inverts the direction of the vector.
// Used in physics simulations, reversing directions, negating forces, etc.

AFXINL void     AfxV2dNeg(afxV2d v, afxV2d const in);
AFXINL void     AfxV3dNeg(afxV3d v, afxV3d const in);
AFXINL void     AfxV4dNeg(afxV4d v, afxV4d const in);
AFXINL void     AfxV4dNegAffine(afxV4d v, afxV4d const in); // v = inverse of in


#endif//AFX_ARITHMETIC_H
