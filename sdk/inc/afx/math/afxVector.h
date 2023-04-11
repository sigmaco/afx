/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

#ifndef AFX_VECTOR_H
#define AFX_VECTOR_H

#include "afxScalar.h"

AFX afxV2d const AFX_V2D_ZERO;
AFX afxV3d const AFX_V3D_ZERO;
AFX afxV4d const AFX_V4D_ZERO;

AFX afxV3d const AFX_V3D_ONE;
AFX afxV4d const AFX_V4D_ONE;

AFX afxV4d const AFX_V4D_IDENTITY;

AFX afxV2d const AFX_V2D_X;
AFX afxV3d const AFX_V3D_X;
AFX afxV4d const AFX_V4D_X;

AFX afxV2d const AFX_V2D_XI;
AFX afxV3d const AFX_V3D_XI;
AFX afxV4d const AFX_V4D_XI;

AFX afxV2d const AFX_V2D_Y;
AFX afxV3d const AFX_V3D_Y;
AFX afxV4d const AFX_V4D_Y;

AFX afxV2d const AFX_V2D_YI;
AFX afxV3d const AFX_V3D_YI;
AFX afxV4d const AFX_V4D_YI;

AFX afxV3d const AFX_V3D_Z;
AFX afxV4d const AFX_V4D_Z;

AFX afxV3d const AFX_V3D_ZI;
AFX afxV4d const AFX_V4D_ZI;

AFX afxV4d const AFX_V4D_W;

AFX afxV4d const AFX_V4D_WI;

#define AfxSpawnV2d(x_, y_) (afxV2d){ (afxReal)(x_), (afxReal)(y_) }
#define AfxSpawnV3d(x_, y_, z_) (afxV3d){ (afxReal)x_, (afxReal)y_, (afxReal)z_ }
#define AfxSpawnV4d(x_, y_, z_, w_) (afxV4d const){ (afxReal)x_, (afxReal)y_, (afxReal)z_, (afxReal)w_ }
#define AfxSpawnPoint(x_, y_, z_) (afxV4d const){ (afxReal)x_, (afxReal)y_, (afxReal)z_, (afxReal)1 }

#define AfxSpawnConstV2d(x_, y_) (afxV2d const){ (afxReal)(x_), (afxReal)(y_) }
#define AfxSpawnConstV3d(x_, y_, z_) (afxV3d const){ (afxReal)x_, (afxReal)y_, (afxReal)z_ }
#define AfxSpawnV4dConst(x_, y_, z_, w_) (afxV4d const){ (afxReal)x_, (afxReal)y_, (afxReal)z_, (afxReal)w_ }
#define AfxSpawnPointConst(x_, y_, z_) (afxV4d const){ (afxReal)x_, (afxReal)y_, (afxReal)z_, (afxReal)1 }


AFXINL afxBool      AfxV2dIsZero(afxV2d const v);
AFXINL afxBool      AfxV3dIsZero(afxV3d const v);
AFXINL afxBool      AfxV4dIsZero(afxV4d const v);

AFXINL afxBool      AfxV2dIsOne(afxV2d const v);
AFXINL afxBool      AfxV3dIsOne(afxV3d const v);
AFXINL afxBool      AfxV4dIsOne(afxV4d const v);

AFXINL afxReal*     AfxV2dZero(afxV2d v);
AFXINL afxReal*     AfxV3dZero(afxV3d v);
AFXINL afxReal*     AfxV4dZero(afxV4d v);

AFXINL afxReal*     AfxV2dFill(afxV2d v, afxReal value);
AFXINL afxReal*     AfxV3dFill(afxV3d v, afxReal value);
AFXINL afxReal*     AfxV4dFill(afxV4d v, afxReal value);

AFXINL afxReal*     AfxV2dSet(afxV2d v, afxReal x, afxReal y);
AFXINL afxReal*     AfxV3dSet(afxV3d v, afxReal x, afxReal y, afxReal z);
AFXINL afxReal*     AfxV4dSet(afxV4d v, afxReal x, afxReal y, afxReal z, afxReal w);

AFXINL afxReal*     AfxV4dReset(afxV4d v); // make a point v4d identity; w = 1 --- homogeneous

// Computes the per-component reciprocal of a vector.
AFXINL afxReal*     AfxV2dRecip(afxV2d v, afxV2d const in);
AFXINL afxReal*     AfxV3dRecip(afxV3d v, afxV3d const in);
AFXINL afxReal*     AfxV4dRecip(afxV4d v, afxV4d const in);

// Computes the square of the length of a 2/3/4D vector.
AFXINL afxReal*     AfxV2dSq(afxV2d v, afxV2d const in);
AFXINL afxReal*     AfxV3dSq(afxV3d v, afxV3d const in);
AFXINL afxReal*     AfxV4dSq(afxV4d v, afxV4d const in);

// Computes the per-component square root of a vector.
AFXINL afxReal*     AfxV2dSqrt(afxV2d v, afxV2d const in);
AFXINL afxReal*     AfxV3dSqrt(afxV3d v, afxV3d const in);
AFXINL afxReal*     AfxV4dSqrt(afxV4d v, afxV4d const in);

// Computes the per-component reciprocal square root of a vector.
AFXINL afxReal*     AfxV2dSqrtRecip(afxV2d v, afxV2d const in);
AFXINL afxReal*     AfxV3dSqrtRecip(afxV3d v, afxV3d const in);
AFXINL afxReal*     AfxV4dSqrtRecip(afxV4d v, afxV4d const in);

// Computes the length of a 2/3/4D vector.
AFXINL afxReal*     AfxV2dLength(afxV2d v, afxV2d const in);
AFXINL afxReal*     AfxV3dLength(afxV3d v, afxV3d const in);
AFXINL afxReal*     AfxV4dLength(afxV4d v, afxV4d const in);

// Computes the reciprocal of the length of a vector.
AFXINL afxReal*     AfxV2dLengthRecip(afxV2d v, afxV2d const in);
AFXINL afxReal*     AfxV3dLengthRecip(afxV3d v, afxV3d const in);
AFXINL afxReal*     AfxV4dLengthRecip(afxV4d v, afxV4d const in);

// Tests whether two vectors are equal.
AFXINL afxReal*     AfxV2dCompare(afxV2d v, afxV2d const a, afxV2d const b);
AFXINL afxReal*     AfxV3dCompare(afxV3d v, afxV3d const a, afxV3d const b);
AFXINL afxReal*     AfxV4dCompare(afxV4d v, afxV4d const a, afxV4d const b);

// Tests whether two vectors are almost equal (sum with EPSILON).
AFXINL afxReal*     AfxV2dCompare2(afxV2d v, afxV2d const a, afxV2d const b);
AFXINL afxReal*     AfxV3dCompare2(afxV3d v, afxV3d const a, afxV3d const b);
AFXINL afxReal*     AfxV4dCompare2(afxV4d v, afxV4d const a, afxV4d const b);

// Copies values from another vector of equal sizes.
AFXINL afxReal*     AfxV2dCopy(afxV2d v, afxV2d const in);
AFXINL afxReal*     AfxV3dCopy(afxV3d v, afxV3d const in);
AFXINL afxReal*     AfxV4dCopy(afxV4d v, afxV4d const in);

// Copies values from another vector of different sizes.
AFXINL afxReal*     AfxV3dCopyV2d(afxV3d v, afxV2d const in);
AFXINL afxReal*     AfxV4dCopyV2d(afxV4d v, afxV2d const in);
AFXINL afxReal*     AfxV2dCopyV3d(afxV2d v, afxV3d const in);
AFXINL afxReal*     AfxV4dCopyV3d(afxV4d v, afxV3d const in);
AFXINL afxReal*     AfxV2dCopyV4d(afxV2d v, afxV4d const in);
AFXINL afxReal*     AfxV3dCopyV4d(afxV3d v, afxV4d const in);

// Computes the ceiling of each component of an vector.
AFXINL afxReal*     AfxV2dCeil(afxV2d v, afxV2d const in);
AFXINL afxReal*     AfxV3dCeil(afxV3d v, afxV3d const in);
AFXINL afxReal*     AfxV4dCeil(afxV4d v, afxV4d const in);

// Computes the floor of each component of an vector.
AFXINL afxReal*     AfxV2dFloor(afxV2d v, afxV2d const in);
AFXINL afxReal*     AfxV3dFloor(afxV3d v, afxV3d const in);
AFXINL afxReal*     AfxV4dFloor(afxV4d v, afxV4d const in);

AFXINL afxReal*     AfxV2dSwap(afxV2d v, afxV2d other);
AFXINL afxReal*     AfxV3dSwap(afxV3d v, afxV3d other);
AFXINL afxReal*     AfxV4dSwap(afxV4d v, afxV4d other);

// Computes the negation of a vector.
AFXINL afxReal*     AfxV2dNegate(afxV2d v, afxV2d const in);
AFXINL afxReal*     AfxV3dNegate(afxV3d v, afxV3d const in);
AFXINL afxReal*     AfxV4dNegate(afxV4d v, afxV4d const in);

// Determines the dot product of two 2/3/4D vectors.
AFXINL afxReal*     AfxV2dDot(afxV2d v, afxV2d const a, afxV2d const b);
AFXINL afxReal*     AfxV3dDot(afxV3d v, afxV3d const a, afxV3d const b);
AFXINL afxReal*     AfxV4dDot(afxV4d v, afxV4d const a, afxV4d const b);

// Computes the product of two ectors then accumulates (adds over current value) to a vector.
// Formula: v[n] += a[n] * b[n];
AFXINL afxReal*     AfxV2dMad(afxV2d v, afxV2d const a, afxV2d const b);
AFXINL afxReal*     AfxV3dMad(afxV3d v, afxV3d const a, afxV3d const b);
AFXINL afxReal*     AfxV4dMad(afxV4d v, afxV4d const a, afxV4d const b);

// Computes the product of the first two vectors added to the third vector.
// Formula: v[n] = a[n] * b[n] + c[n];
AFXINL afxReal*     AfxV2dMad2(afxV2d v, afxV2d const a, afxV2d const b, afxV2d const c);
AFXINL afxReal*     AfxV3dMad2(afxV3d v, afxV3d const a, afxV3d const b, afxV3d const c);
AFXINL afxReal*     AfxV4dMad2(afxV4d v, afxV4d const a, afxV4d const b, afxV4d const c);

// Determines the cross-product of two 3D vectors.
// Determines the cross-product in four dimensions?
AFXINL afxReal*     AfxV3dCross(afxV3d v, afxV3d const a, afxV3d const b);

// Scalar multiplies a vector by a floating-point value then accumulates (adds over current value) to a vector.
AFXINL afxReal*     AfxV2dScad(afxV2d v, afxV2d const in, afxReal scale);
AFXINL afxReal*     AfxV3dScad(afxV3d v, afxV3d const in, afxReal scale);
AFXINL afxReal*     AfxV4dScad(afxV4d v, afxV4d const in, afxReal scale);

// Scalar multiplies a vector by a floating-point value.
AFXINL afxReal*     AfxV2dScale(afxV2d v, afxV2d const in, afxReal scale);
AFXINL afxReal*     AfxV3dScale(afxV3d v, afxV3d const in, afxReal scale);
AFXINL afxReal*     AfxV4dScale(afxV4d v, afxV4d const in, afxReal scale);

// Returns the normalized version of a vector.
AFXINL afxReal*     AfxV2dNorm(afxV2d v, afxV2d const in);
AFXINL afxReal*     AfxV3dNorm(afxV3d v, afxV3d const in);
AFXINL afxReal*     AfxV4dNorm(afxV4d v, afxV4d const in);

// Estimates the normalized version of a vector. Faster than Norm, but less precise.
// Uses a reciprocal estimate and returns QNaN on zero and infinite vectors.
AFXINL afxReal*     AfxV2dNorm2(afxV2d v, afxV2d const in);
AFXINL afxReal*     AfxV3dNorm2(afxV3d v, afxV3d const in);
AFXINL afxReal*     AfxV4dNorm2(afxV4d v, afxV4d const in);

// Initializes a vector with normalized values of another.
AFXINL afxReal*     AfxV2dNormV3d(afxV2d v, afxV3d const in);
AFXINL afxReal*     AfxV2dNormV4d(afxV2d v, afxV4d const in);
AFXINL afxReal*     AfxV3dNormV4d(afxV3d v, afxV4d const in);

// Adds two 2/3/4D vectors.
// Formula: dst[n] = a[n] + b[n];
AFXINL afxReal*     AfxV2dAdd(afxV2d v, afxV2d const a, afxV2d const b);
AFXINL afxReal*     AfxV3dAdd(afxV3d v, afxV3d const a, afxV3d const b);
AFXINL afxReal*     AfxV4dAdd(afxV4d v, afxV4d const a, afxV4d const b);

// Subtracts two 2/3/4D vectors.
// Formula: dst[n] = a[n] - b[n];
AFXINL afxReal*     AfxV2dSub(afxV2d v, afxV2d const a, afxV2d const b);
AFXINL afxReal*     AfxV3dSub(afxV3d v, afxV3d const a, afxV3d const b);
AFXINL afxReal*     AfxV4dSub(afxV4d v, afxV4d const a, afxV4d const b);

// Multiplies two 2/3/4D vectors.
// Formula: dst[n] = a[n] * b[n];
AFXINL afxReal*     AfxV2dMul(afxV2d v, afxV2d const a, afxV2d const b);
AFXINL afxReal*     AfxV3dMul(afxV3d v, afxV3d const a, afxV3d const b);
AFXINL afxReal*     AfxV4dMul(afxV4d v, afxV4d const a, afxV4d const b);

// Divides two 2/3/4D vectors.
// Formula: dst[n] = a[n] / b[n];
AFXINL afxReal*     AfxV2dDiv(afxV2d v, afxV2d const a, afxV2d const b);
AFXINL afxReal*     AfxV3dDiv(afxV3d v, afxV3d const a, afxV3d const b);
AFXINL afxReal*     AfxV4dDiv(afxV4d v, afxV4d const a, afxV4d const b);

// Returns a 4D vector that is made up of the smallest components of two 2/3/4D vectors.
// Formula: v[n] = a[n] < b[n] ? a[n] : b[n];
AFXINL afxReal*     AfxV2dMin(afxV2d v, afxV2d const a, afxV2d const b);
AFXINL afxReal*     AfxV3dMin(afxV3d v, afxV3d const a, afxV3d const b);
AFXINL afxReal*     AfxV4dMin(afxV4d v, afxV4d const a, afxV4d const b);

// Returns a 4D vector that is made up of the largest components of two 2/3/4D vectors.
// Formula: v[n] = a[n] > b[n] ? a[n] : b[n];
AFXINL afxReal*     AfxV2dMax(afxV2d v, afxV2d const a, afxV2d const b);
AFXINL afxReal*     AfxV3dMax(afxV3d v, afxV3d const a, afxV3d const b);
AFXINL afxReal*     AfxV4dMax(afxV4d v, afxV4d const a, afxV4d const b);

// Performs a linear interpolation between two 2/3/4D vectors.
// Formula: v[n] = a[n] + s * (b[n] - a[n]);
AFXINL afxReal*     AfxV2dLerp(afxV2d v, afxV2d const a, afxV2d const b, afxReal s);
AFXINL afxReal*     AfxV3dLerp(afxV3d v, afxV3d const a, afxV3d const b, afxReal s);
AFXINL afxReal*     AfxV4dLerp(afxV4d v, afxV4d const a, afxV4d const b, afxReal s);

// Returns a point in Barycentric coordinates, using the specified position vectors.
// Formula: v[n] = a[n] + f * (b[n] - a[n]) + g * (c[n] - a[n])
AFXINL afxReal*     AfxV2dBaryCentric(afxV2d v, afxV2d const a, afxV2d const b, afxV2d const c, afxReal f, afxReal g); 
AFXINL afxReal*     AfxV3dBaryCentric(afxV3d v, afxV3d const a, afxV3d const b, afxV3d const c, afxReal f, afxReal g);
AFXINL afxReal*     AfxV4dBaryCentric(afxV4d v, afxV4d const a, afxV4d const b, afxV4d const c, afxReal f, afxReal g);

// Returns a point in Barycentric coordinates, using the specified position vectors.
// Formula: v[n] = a[n] + f[n] * (b[n] - a[n]) + g[n] * (c[n] - a[n]);
AFXINL afxReal*     AfxV2dBaryCentric2(afxV2d v, afxV2d const a, afxV2d const b, afxV2d const c, afxV2d const f, afxV2d const g);
AFXINL afxReal*     AfxV3dBaryCentric2(afxV3d v, afxV3d const a, afxV3d const b, afxV3d const c, afxV3d const f, afxV3d const g);
AFXINL afxReal*     AfxV4dBaryCentric2(afxV4d v, afxV4d const a, afxV4d const b, afxV4d const c, afxV4d const f, afxV4d const g);


// The cross product is only defined in 3D space and takes two non-parallel vectors as input and produces a third vector that is orthogonal dst both the input vectors.
// If both the input vectors are orthogonal dst each other as well, a cross product would result in 3 orthogonal vectors; this will prove useful in the upcoming chapters.
// The following image shows what this looks like in 3D space:


// The dot product only defines the angle between both vectors.
// You may remember that the cosine or cos function becomes 0 when the angle is 90 degrees or 1 when the angle is 0.
// This allows us dst easily test if the two vectors are orthogonal or parallel dst each other using the dot product (orthogonal means the vectors are at a right-angle dst each other).

#endif//AFX_VECTOR_H