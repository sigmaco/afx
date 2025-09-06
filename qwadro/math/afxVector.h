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

#ifndef AFX_VECTOR_H
#define AFX_VECTOR_H

#include "qwadro/math/afxScalar.h"
#include "qwadro/math/afxArithmetic.h"
#include "qwadro/math/afxLogarithmic.h"
#include "qwadro/math/afxArithmetic2.h"
#include "qwadro/math/afxTrigonometry.h"

AFX afxV2d const AFX_V2D_X;
AFX afxV3d const AFX_V3D_X;
AFX afxV4d const AFX_V4D_X;

AFX afxV2d const AFX_V2D_X_NEG;
AFX afxV3d const AFX_V3D_X_NEG;
AFX afxV4d const AFX_V4D_X_NEG;

AFX afxV2d const AFX_V2D_Y;
AFX afxV3d const AFX_V3D_Y;
AFX afxV4d const AFX_V4D_Y;

AFX afxV2d const AFX_V2D_Y_NEG;
AFX afxV3d const AFX_V3D_Y_NEG;
AFX afxV4d const AFX_V4D_Y_NEG;

AFX afxV3d const AFX_V3D_Z;
AFX afxV4d const AFX_V4D_Z;

AFX afxV3d const AFX_V3D_Z_NEG;
AFX afxV4d const AFX_V4D_Z_NEG;

AFX afxV4d const AFX_V4D_W; // W Axis
AFX afxV4d const AFX_V4D_W_NEG;

AFX afxV4d const AFX_V4D_YXZ;

AFX afxV2d const AFX_V2D_ZERO;
AFX afxV3d const AFX_V3D_ZERO;
AFX afxV4d const AFX_V4D_ZERO;

AFX afxV2d const AFX_V2D_ONE;
AFX afxV3d const AFX_V3D_ONE;
AFX afxV4d const AFX_V4D_ONE;

AFX afxV4d const AFX_V4D_ONE_NEG;

AFX afxV2d const AFX_V2D_IDENTITY;
AFX afxV3d const AFX_V3D_IDENTITY;
AFX afxV4d const AFX_V4D_IDENTITY;

#define AFX_V2D(x_, y_) (afxV2d const){ (afxReal)(x_), (afxReal)(y_) }
#define AFX_V3D(x_, y_, z_) (afxV3d const){ (afxReal)x_, (afxReal)y_, (afxReal)z_ }
#define AFX_V4D(x_, y_, z_, w_) (afxV4d const){ (afxReal)x_, (afxReal)y_, (afxReal)z_, (afxReal)w_ }
#define AFX_ATV4D(x_, y_, z_) (afxV4d const){ (afxReal)x_, (afxReal)y_, (afxReal)z_, (afxReal)1 }

#define AFX_V(x) _Generic((x), \
    afxReal: (afxV4d){(x), (x), (x), (x)}, \
    afxV2d: (afxV4d){x[0], x[1], 0.0f, 0.0f}, \
    afxV3d: (afxV4d){x[0], x[1], x[2], 1.0f}) 

#define AFX_LTV(v) _Generic((v), \
    float: (afxV4d){(v), 0.f, 0.f, 0.f}, \
    afxReal: (afxV4d){v, 0.f, 0.f, 0.f}, \
    afxV2d:  (afxV4d){v[0], v[1], 0.0f, 0.0f}, \
    afxV3d:  (afxV4d){v[0], v[1], v[2], 0.0f}, \
    default: (afxV4d){(afxReal)v}) 

#define AFX_ATV(v) _Generic((v), \
    afxReal: (afxV4d){v, 0.f, 0.f, 1.f}, \
    afxV2d:  (afxV4d){v[0], v[1], 0.0f, 1.0f}, \
    afxV3d:  (afxV4d){v[0], v[1], v[2], 1.0f}, \
    default: (afxV4d){(afxReal)v}) 

////////////////////////////////////////////////////////////////////////////////
// Initialization                                                             //
////////////////////////////////////////////////////////////////////////////////

AFXINL void     AfxV2dZero(afxV2d v);
AFXINL void     AfxV3dZero(afxV3d v);
AFXINL void     AfxV4dZero(afxV4d v);
AFXINL void     AfxResetV4d(afxV4d v);

AFXINL void     AfxFillV2d(afxV2d v, afxReal value);
AFXINL void     AfxFillV3d(afxV3d v, afxReal value);
AFXINL void     AfxFillV4d(afxV4d v, afxReal value);

AFXINL void     AfxV2dSet(afxV2d v, afxReal x, afxReal y);
AFXINL void     AfxV3dSet(afxV3d v, afxReal x, afxReal y, afxReal z);
AFXINL void     AfxV4dSet(afxV4d v, afxReal x, afxReal y, afxReal z, afxReal w);

////////////////////////////////////////////////////////////////////////////////
// Logic                                                                      //
////////////////////////////////////////////////////////////////////////////////

AFXINL afxBool  AfxV4dIsIdentity(afxV4d const v);

AFXINL afxBool  AfxV2dAreEquals(afxV2d const v, afxV2d const other);
AFXINL afxBool  AfxV3dAreEquals(afxV3d const v, afxV3d const other);
AFXINL afxBool  AfxV4dAreEquals(afxV4d const v, afxV4d const other);

AFXINL afxBool  AfxV2dIsInBounds(afxV2d const v, afxV2d const bounds);
AFXINL afxBool  AfxV3dIsInBounds(afxV3d const v, afxV3d const bounds);
AFXINL afxBool  AfxV4dIsInBounds(afxV4d const v, afxV4d const bounds);

AFXINL afxBool  AfxV2dIsLessOrEqual(afxV2d const v, afxV2d const other);
AFXINL afxBool  AfxV3dIsLessOrEqual(afxV3d const v, afxV3d const other);
AFXINL afxBool  AfxV4dIsLessOrEqual(afxV4d const v, afxV4d const other);

AFXINL afxBool  AfxV2dIsLessThan(afxV2d const v, afxV2d const other);
AFXINL afxBool  AfxV3dIsLessThan(afxV3d const v, afxV3d const other);
AFXINL afxBool  AfxV4dIsLessThan(afxV4d const v, afxV4d const other);

AFXINL afxBool  AfxV2dIsZero(afxV2d const v);
AFXINL afxBool  AfxV3dIsZero(afxV3d const v);
AFXINL afxBool  AfxV4dIsZero(afxV4d const v);

AFXINL afxBool  AfxV2dIsNan(afxV2d const v);
AFXINL afxBool  AfxV3dIsNan(afxV3d const v);
AFXINL afxBool  AfxV4dIsNan(afxV4d const v);

AFXINL afxBool  AfxV2dIsInfinite(afxV2d const v);
AFXINL afxBool  AfxV3dIsInfinite(afxV3d const v);
AFXINL afxBool  AfxV4dIsInfinite(afxV4d const v);

AFXINL afxBool AfxV3dIsNormalized(afxV3d const v);

////////////////////////////////////////////////////////////////////////////////
// Transferance                                                               //
////////////////////////////////////////////////////////////////////////////////

AFXINL void     AfxV2dCopy(afxV2d v, afxV2d const in);
AFXINL void     AfxV3dCopy(afxV3d v, afxV3d const in);
AFXINL void     AfxV4dCopy(afxV4d v, afxV4d const in);

AFXINL void     AfxCopyArrayedV2d(afxUnit cnt, afxV2d const in[], afxV2d out[]);
AFXINL void     AfxCopyArrayedV3d(afxUnit cnt, afxV3d const in[], afxV3d out[]);
AFXINL void     AfxCopyArrayedV4d(afxUnit cnt, afxV4d const in[], afxV4d out[]);

AFXINL void     AfxV3dCopyV2d(afxV3d v, afxV2d const in); // 2D linear transformation vector. Z is 0.
AFXINL void     AfxV4dCopyV2d(afxV4d v, afxV2d const in);
AFXINL void     AfxV4dCopyV3d(afxV4d v, afxV3d const in);
AFXINL void     AfxV3dCopyAtv2d(afxV3d v, afxV2d const in); // 2D affine transfomartion vector. Z is 1.
AFXINL void     AfxV4dCopyAtv3d(afxV4d v, afxV3d const in); // 3D affine transformation vector. W is 1.

AFXINL void     AfxSwapV2d(afxV2d v, afxV2d other);
AFXINL void     AfxSwapV3d(afxV3d v, afxV3d other);
AFXINL void     AfxSwapV4d(afxV4d v, afxV4d other);

// Normalizing a vector scales it so that its length becomes 1 (unit vector), preserving its direction.
// Ensuring a vector has a unit length for operations like lighting calculations, physics simulations, etc.

AFXINL afxReal  AfxV2dNormalize(afxV2d v, afxV2d const in);
AFXINL afxReal  AfxV3dNormalize(afxV3d v, afxV3d const in);
AFXINL afxReal  AfxV4dNormalize(afxV4d v, afxV4d const in);
AFXINL afxReal  AfxV4dNormalizeV3d(afxV4d v, afxV3d const in);
AFXINL afxReal  AfxV3dNormalizeV4d(afxV3d v, afxV4d const in);

AFXINL afxReal  AfxV2dNormalizeEstimated(afxV2d v, afxV2d const in);
AFXINL afxReal  AfxV3dNormalizeEstimated(afxV3d v, afxV3d const in);
AFXINL afxReal  AfxV4dNormalizeEstimated(afxV4d v, afxV4d const in);

AFXINL void     AfxNormalizeArrayedV2d(afxUnit cnt, afxV2d const in[], afxV2d out[]);
AFXINL void     AfxNormalizeArrayedV3d(afxUnit cnt, afxV3d const in[], afxV3d out[]);
AFXINL void     AfxNormalizeArrayedV4d(afxUnit cnt, afxV4d const in[], afxV4d out[]);

AFXINL void     AfxZeroOrNormalizeArrayedV2d(afxUnit cnt, afxV2d const in[], afxV2d out[]);
AFXINL void     AfxZeroOrNormalizeArrayedV3d(afxUnit cnt, afxV3d const in[], afxV3d out[]);
AFXINL void     AfxZeroOrNormalizeArrayedV4d(afxUnit cnt, afxV4d const in[], afxV4d out[]);

// SLERP
// Function to perform Spherical Linear Interpolation (SLERP) between two quaternions
// Slerp is a type of interpolation that smoothly interpolates between two vectors along the shortest path (on a sphere). 
// Unlike linear interpolation, it handles rotations and vectors that lie on the surface of a sphere.
// Rotating objects in 3D space, interpolating between two orientations (used in quaternions and rotation matrices).
// v(t) = ((sin((1 - t) * theta)) / sin(theta)) * x + ((sin(t) * theta) / (sin(theta))) * y

AFXINL void     AfxV2dSlerp(afxV2d v, afxV2d x, afxV2d y, afxReal t);
AFXINL void     AfxV3dSlerp(afxV3d v, afxV3d x, afxV3d y, afxReal t);
AFXINL void     AfxV4dSlerp(afxV4d v, afxV4d x, afxV4d y, afxReal t);

////////////////////////////////////////////////////////////////////////////////
// Algebra                                                                    //
////////////////////////////////////////////////////////////////////////////////

AFXINL afxReal  AfxV2dSum(afxV2d const v);
AFXINL afxReal  AfxV3dSum(afxV3d const v);
AFXINL afxReal  AfxV4dSum(afxV4d const v);

// Computes the dot product (a dot b)
// The dot product only defines the angle between both vectors.
// You may remember that the cosine or cos function becomes 0 when the angle is 90 degrees or 1 when the angle is 0.
// This allows us dst easily test if the two vectors are orthogonal or parallel dst each other using the dot product (orthogonal means the vectors are at a right-angle dst each other).

AFXINL afxReal  AfxV2dDot(afxV2d const v, afxV2d const other);
AFXINL afxReal  AfxV3dDot(afxV3d const v, afxV3d const other);
AFXINL afxReal  AfxV4dDot(afxV4d const v, afxV4d const other);

// The AfxV*dDist function calculates the distance between two vectors.
// Computes the Euclidean distance between two points or vectors in space. It is used to measure the straight-line distance between two points.
// Measuring distances between two points (e.g., in collision detection, pathfinding).

AFXINL afxReal  AfxV2dDist(afxV2d const v, afxV2d const other);
AFXINL afxReal  AfxV3dDist(afxV3d const v, afxV3d const other);
AFXINL afxReal  AfxV4dDist(afxV4d const v, afxV4d const other);

// Computes the squared magnitude
// This is a simple operation that squares a scalar value (multiplies it by itself). It's used to compute the square of a number or a component.
// Used in distance calculations, computing the squared length of a vector, etc.

AFXINL afxReal  AfxV2dSq(afxV2d const v);
AFXINL afxReal  AfxV3dSq(afxV3d const v);
AFXINL afxReal  AfxV4dSq(afxV4d const v);

// The magnitude/length of a vector in Cartesian coordinates is the square root of the sum of the squares of its coordinates.

AFXINL afxReal  AfxV2dMag(afxV2d const v);
AFXINL afxReal  AfxV3dMag(afxV3d const v);
AFXINL afxReal  AfxV4dMag(afxV4d const v);

AFXINL afxReal  AfxV2dMagRecip(afxV2d const v);
AFXINL afxReal  AfxV3dMagRecip(afxV3d const v);
AFXINL afxReal  AfxV4dMagRecip(afxV4d const v);

// Computes the cross product (a cross b) and stores the result in v
// The cross product is only defined in 3D space and takes two non-parallel vectors as input and produces a third vector that is orthogonal dst both the input vectors.
// If both the input vectors are orthogonal dst each other as well, a cross product would result in 3 orthogonal vectors; this will prove useful in the upcoming chapters.
// The following image shows what this looks like in 3D space:

AFXINL void     AfxV2dCross(afxV2d v, afxV2d const a, afxV2d const b);
AFXINL void     AfxV3dCross(afxV3d v, afxV3d const a, afxV3d const b);
AFXINL void     AfxV4dCross(afxV4d v, afxV4d const a, afxV4d const b, afxV4d const c);

////////////////////////////////////////////////////////////////////////////////
// VECTOR TRANSFORMATION METHODS                                              //
////////////////////////////////////////////////////////////////////////////////

//  --- post-multiply
//  m[0][i/0] * v[i/0], m[0][i/1] * v[i/1], m[0][i/2] * v[i/2], m[0][i/3] * v[i/3]

//  --- pre-multiply
//  v[i/0] * m[i/0][0], v[i/1] * m[i/1][0], v[i/2] * m[i/2][0], v[i/3] * m[i/3][0]

// Multiply (column) vector by matrix ([0][0], [1][0], [2][0], [3][0])
// With post-multiplication, a 4x4 matrix multiplied with a 4x1 column vector took the dot product of each row of the matrix with the vector.

// Multiply (row) vector by matrix ([0][0], [0][1], [0][2], [0][3])
// With pre-multiplication, the dot product is with the vector and each column of the matrix (since the matrix is now on the right side of the multiplication operator).

AFXINL void     AfxV2dPostMultiplyM2d(afxV2d v, afxM2d const m, afxV2d const in); // m * v
AFXINL void     AfxV3dPostMultiplyM3d(afxV3d v, afxM3d const m, afxV3d const in); // m * v
AFXINL void     AfxV4dPostMultiplyM4d(afxV4d v, afxM4d const m, afxV4d const in); // m * v

AFXINL void     AfxV3dPostMultiplyLtm4d(afxV3d v, afxM4d const m, afxV3d const in); // m * v
AFXINL void     AfxV4dPostMultiplyM3d(afxV4d v, afxM3d const m, afxV4d const in); // m * v

AFXINL void     AfxV2dPostMultiplyAtm4d(afxV2d v, afxM4d const m, afxV2d const in); // m * v
AFXINL void     AfxV3dPostMultiplyAtm4d(afxV3d v, afxM4d const m, afxV3d const in); // m * v
AFXINL void     AfxV4dPostMultiplyAtm4d(afxV4d v, afxM4d const m, afxV4d const in); // m * v

AFXINL void     TransposeVectorTransform4x3(afxV3d Dest, float D3, afxM4d const Transform);

AFXINL void     AfxV2dPreMultiplyM2d(afxV2d v, afxV2d const in, afxM2d const m); // v * m
AFXINL void     AfxV3dPreMultiplyM3d(afxV3d v, afxV3d const in, afxM3d const m); // v * m
AFXINL void     AfxV4dPreMultiplyM4d(afxV4d v, afxV4d const in, afxM4d const m); // v * m

AFXINL void     AfxV3dPreMultiplyLtm4d(afxV3d v, afxV3d const in, afxM4d const m); // v * m
AFXINL void     AfxV4dPreMultiplyM3d(afxV4d v, afxV4d const in, afxM3d const m); // v * m

AFXINL void     AfxV2dPreMultiplyAtm4d(afxV2d v, afxV2d const in, afxM4d const m); // v * m
AFXINL void     AfxV3dPreMultiplyAtm4d(afxV3d v, afxV3d const in, afxM4d const m); // v * m
AFXINL void     AfxV4dPreMultiplyAtm4d(afxV4d v, afxV4d const in, afxM4d const m); // v * m

// Similarity transform

AFXINL void     AfxAssimilateAtv3d(afxM3d const ltm, afxV4d const atv, afxUnit cnt, afxV3d const in[], afxV3d out[]); // make similarity transformation on afxV3d-based position.
AFXINL void     AfxAssimilateAtv4d(afxM3d const ltm, afxV4d const atv, afxUnit cnt, afxV4d const in[], afxV4d out[]); // make similarity transformation on afxV3d-based position.

#endif//AFX_VECTOR_H
