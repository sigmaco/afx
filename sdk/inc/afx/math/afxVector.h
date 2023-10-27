/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_VECTOR_H
#define AFX_VECTOR_H

#include "afxReal.h"

AFX afxV2d const AFX_V2D_10;
AFX afxV3d const AFX_V3D_100;
AFX afxV4d const AFX_V4D_1000;

AFX afxV2d const AFX_V2D_10_NEG;
AFX afxV3d const AFX_V3D_100_NEG;
AFX afxV4d const AFX_V4D_1000_NEG;

AFX afxV2d const AFX_V2D_01;
AFX afxV3d const AFX_V3D_010;
AFX afxV4d const AFX_V4D_0100;

AFX afxV2d const AFX_V2D_01_NEG;
AFX afxV3d const AFX_V3D_010_NEG;
AFX afxV4d const AFX_V4D_0100_NEG;

AFX afxV3d const AFX_V3D_001;
AFX afxV4d const AFX_V4D_0010;

AFX afxV3d const AFX_V3D_001_NEG;
AFX afxV4d const AFX_V4D_0010_NEG;

AFX afxV4d const AFX_V4D_0001; // W Axis
AFX afxV4d const AFX_V4D_0001_NEG;

AFX afxV4d const AFX_V4D_1110;

AFX afxV2d const AFX_V2D_00;
AFX afxV3d const AFX_V3D_000;
AFX afxV4d const AFX_V4D_0000;

AFX afxV2d const AFX_V2D_11;
AFX afxV3d const AFX_V3D_111;
AFX afxV4d const AFX_V4D_1111;

AFX afxV4d const AFX_V4D_1111_NEG;

#define AfxSpawnV2d(x_, y_) (afxV2d){ (afxReal)(x_), (afxReal)(y_) }
#define AfxSpawnV3d(x_, y_, z_) (afxV3d){ (afxReal)x_, (afxReal)y_, (afxReal)z_ }
#define AfxSpawnV4d(x_, y_, z_, w_) (afxV4d const){ (afxReal)x_, (afxReal)y_, (afxReal)z_, (afxReal)w_ }
#define AfxSpawnPoint(x_, y_, z_) (afxV4d const){ (afxReal)x_, (afxReal)y_, (afxReal)z_, (afxReal)1 }

#define AfxSpawnConstV2d(x_, y_) (afxV2d const){ (afxReal)(x_), (afxReal)(y_) }
#define AfxSpawnConstV3d(x_, y_, z_) (afxV3d const){ (afxReal)x_, (afxReal)y_, (afxReal)z_ }
#define AfxSpawnV4dConst(x_, y_, z_, w_) (afxV4d const){ (afxReal)x_, (afxReal)y_, (afxReal)z_, (afxReal)w_ }
#define AfxSpawnPointConst(x_, y_, z_) (afxV4d const){ (afxReal)x_, (afxReal)y_, (afxReal)z_, (afxReal)1 }

////////////////////////////////////////////////////////////////////////////////
// Initialization                                                             //
////////////////////////////////////////////////////////////////////////////////

AFXINL void     AfxZeroV2d(afxReal v[2]);
AFXINL void     AfxZeroV3d(afxReal v[3]);
AFXINL void     AfxZeroV4d(afxReal v[4]);

AFXINL void     AfxFillV2d(afxReal v[2], afxReal value);
AFXINL void     AfxFillV3d(afxReal v[3], afxReal value);
AFXINL void     AfxFillV4d(afxReal v[4], afxReal value);

AFXINL void     AfxSetV2d(afxReal v[2], afxReal x, afxReal y);
AFXINL void     AfxSetV3d(afxReal v[3], afxReal x, afxReal y, afxReal z);
AFXINL void     AfxSetV4d(afxReal v[4], afxReal x, afxReal y, afxReal z, afxReal w);

AFXINL void     AfxResetV4d(afxReal v[4]);

////////////////////////////////////////////////////////////////////////////////
// Logic                                                                      //
////////////////////////////////////////////////////////////////////////////////

AFXINL afxBool  AfxV4dIsIdentity(afxReal const v[4]);

AFXINL afxBool  AfxV2dAreEquals(afxReal const v[2], afxReal const other[2]);
AFXINL afxBool  AfxV3dAreEquals(afxReal const v[3], afxReal const other[3]);
AFXINL afxBool  AfxV4dAreEquals(afxReal const v[4], afxReal const other[4]);

AFXINL afxBool  AfxV2dIsInBounds(afxReal const v[2], afxReal const bounds[2]);
AFXINL afxBool  AfxV3dIsInBounds(afxReal const v[3], afxReal const bounds[3]);
AFXINL afxBool  AfxV4dIsInBounds(afxReal const v[4], afxReal const bounds[4]);

AFXINL afxBool  AfxV2dIsLessOrEqual(afxReal const v[2], afxReal const other[2]);
AFXINL afxBool  AfxV3dIsLessOrEqual(afxReal const v[3], afxReal const other[3]);
AFXINL afxBool  AfxV4dIsLessOrEqual(afxReal const v[4], afxReal const other[4]);

AFXINL afxBool  AfxV2dIsLessThan(afxReal const v[2], afxReal const other[2]);
AFXINL afxBool  AfxV3dIsLessThan(afxReal const v[3], afxReal const other[3]);
AFXINL afxBool  AfxV4dIsLessThan(afxReal const v[4], afxReal const other[4]);

AFXINL afxBool  AfxV2dIsZero(afxReal const v[2]);
AFXINL afxBool  AfxV3dIsZero(afxReal const v[3]);
AFXINL afxBool  AfxV4dIsZero(afxReal const v[4]);

AFXINL afxBool  AfxV2dIsNan(afxReal const v[2]);
AFXINL afxBool  AfxV3dIsNan(afxReal const v[3]);
AFXINL afxBool  AfxV4dIsNan(afxReal const v[4]);

AFXINL afxBool  AfxV2dIsInfinite(afxReal const v[2]);
AFXINL afxBool  AfxV3dIsInfinite(afxReal const v[3]);
AFXINL afxBool  AfxV4dIsInfinite(afxReal const v[4]);

AFXINL afxBool AfxV3dIsNormalized(afxReal const v[3]);

////////////////////////////////////////////////////////////////////////////////
// Transferance                                                               //
////////////////////////////////////////////////////////////////////////////////

AFXINL void     AfxCopyV2d(afxReal v[2], afxReal const in[2]);
AFXINL void     AfxCopyV3d(afxReal v[3], afxReal const in[3]);
AFXINL void     AfxCopyV4d(afxReal v[4], afxReal const in[4]);

AFXINL void     AfxCopyV2dArray(afxReal v[][2], afxReal const in[][2], afxNat cnt);
AFXINL void     AfxCopyV3dArray(afxReal v[][3], afxReal const in[][3], afxNat cnt);
AFXINL void     AfxCopyV4dArray(afxReal v[][4], afxReal const in[][4], afxNat cnt);

AFXINL void     AfxV3dFromLinearV2d(afxReal v[3], afxReal const in[2]); // 2D linear transformation vector. Z is 0.
AFXINL void     AfxV4dFromV2d(afxReal v[4], afxReal const in[2]);
AFXINL void     AfxV4dFromV3d(afxReal v[4], afxReal const in[3]);
AFXINL void     AfxV3dFromAffineV2d(afxReal v[3], afxReal const in[2]); // 2D affine transfomartion vector. Z is 1.
AFXINL void     AfxV4dFromAffineV3d(afxReal v[4], afxReal const in[3]); // 3D affine transformation vector. W is 1.

AFXINL void     AfxSwapV2d(afxReal v[2], afxReal other[2]);
AFXINL void     AfxSwapV3d(afxReal v[3], afxReal other[3]);
AFXINL void     AfxSwapV4d(afxReal v[4], afxReal other[4]);

////////////////////////////////////////////////////////////////////////////////
// Clamping                                                                   //
////////////////////////////////////////////////////////////////////////////////

AFXINL void     AfxClampV2d(afxReal v[2], afxReal const in[2], afxReal const mini[2], afxReal const maxi[2]); // v = IN between bounds
AFXINL void     AfxClampV3d(afxReal v[3], afxReal const in[3], afxReal const mini[3], afxReal const maxi[3]);
AFXINL void     AfxClampV4d(afxReal v[4], afxReal const in[4], afxReal const mini[4], afxReal const maxi[4]);

AFXINL void     AfxTruncateV2d(afxReal v[2], afxReal const in[2]);
AFXINL void     AfxTruncateV3d(afxReal v[3], afxReal const in[3]);
AFXINL void     AfxTruncateV4d(afxReal v[4], afxReal const in[4]);

AFXINL void     AfxGetSaturatedV2d(afxReal v[2], afxReal const in[2]);
AFXINL void     AfxGetSaturatedV3d(afxReal v[3], afxReal const in[3]);
AFXINL void     AfxGetSaturatedV4d(afxReal v[4], afxReal const in[4]);

AFXINL void     AfxMiniV2d(afxReal v[2], afxReal const a[2], afxReal const b[2]); // v = minor between A and B
AFXINL void     AfxMiniV3d(afxReal v[3], afxReal const a[3], afxReal const b[3]); // v = minor between A and B
AFXINL void     AfxMiniV4d(afxReal v[4], afxReal const a[4], afxReal const b[4]); // v = minor between A and B

AFXINL void     AfxMaxiV2d(afxReal v[2], afxReal const a[2], afxReal const b[2]); // v = major between A and B
AFXINL void     AfxMaxiV3d(afxReal v[3], afxReal const a[3], afxReal const b[3]); // v = major between A and B
AFXINL void     AfxMaxiV4d(afxReal v[4], afxReal const a[4], afxReal const b[4]); // v = major between A and B

AFXINL void     AfxAbsV2d(afxReal v[2], afxReal const in[2]); // v = abs(in)
AFXINL void     AfxAbsV3d(afxReal v[3], afxReal const in[3]); // v = abs(in)
AFXINL void     AfxAbsV4d(afxReal v[4], afxReal const in[4]); // v = abs(in)

AFXINL void     AfxCeilV2d(afxReal v[2], afxReal const in[2]); // v = ceil(in)
AFXINL void     AfxCeilV3d(afxReal v[3], afxReal const in[3]); // v = ceil(in)
AFXINL void     AfxCeilV4d(afxReal v[4], afxReal const in[4]); // v = ceil(in)

AFXINL void     AfxFloorV2d(afxReal v[2], afxReal const in[2]); // v = floor(in)
AFXINL void     AfxFloorV3d(afxReal v[3], afxReal const in[3]); // v = floor(in)
AFXINL void     AfxFloorV4d(afxReal v[4], afxReal const in[4]); // v = floor(in)

AFXINL void     AfxRecipV2d(afxReal v[2], afxReal const in[2]); // v = 1 / in
AFXINL void     AfxRecipV3d(afxReal v[3], afxReal const in[3]); // v = 1 / in
AFXINL void     AfxRecipV4d(afxReal v[4], afxReal const in[4]); // v = 1 / in

AFXINL afxReal  AfxGetNormalizedV2d(afxReal v[2], afxReal const in[2]);
AFXINL afxReal  AfxGetNormalizedV3d(afxReal v[3], afxReal const in[3]);
AFXINL afxReal  AfxGetNormalizedV4d(afxReal v[4], afxReal const in[4]);

AFXINL afxReal  AfxGetNormalizedV3dFromV4d(afxReal v[3], afxReal const in[4]);
AFXINL afxReal  AfxGetNormalizedV4dFromV3d(afxReal v[4], afxReal const in[3]);

AFXINL afxReal  AfxEstimateNormalizedV2d(afxReal v[2], afxReal const in[2]);
AFXINL afxReal  AfxEstimateNormalizedV3d(afxReal v[3], afxReal const in[3]);
AFXINL afxReal  AfxEstimateNormalizedV4d(afxReal v[4], afxReal const in[4]);

////////////////////////////////////////////////////////////////////////////////
// Arithmetica                                                                //
////////////////////////////////////////////////////////////////////////////////

AFXINL void     AfxAddV2d(afxReal v[2], afxReal const a[2], afxReal const b[2]); // v = a + b
AFXINL void     AfxAddV3d(afxReal v[3], afxReal const a[3], afxReal const b[3]); // v = a + b
AFXINL void     AfxAddV4d(afxReal v[4], afxReal const a[4], afxReal const b[4]); // v = a + b

AFXINL void     AfxSubV2d(afxReal v[2], afxReal const a[2], afxReal const b[2]); // v = a - b
AFXINL void     AfxSubV3d(afxReal v[3], afxReal const a[3], afxReal const b[3]); // v = a - b
AFXINL void     AfxSubV4d(afxReal v[4], afxReal const a[4], afxReal const b[4]); // v = a - b

AFXINL void     AfxDivV2d(afxReal v[2], afxReal const a[2], afxReal const b[2]); // v = a / b
AFXINL void     AfxDivV3d(afxReal v[3], afxReal const a[3], afxReal const b[3]); // v = a / b
AFXINL void     AfxDivV4d(afxReal v[4], afxReal const a[4], afxReal const b[4]); // v = a / b

AFXINL void     AfxModV2d(afxReal v[2], afxReal const a[2], afxReal const b[2]);
AFXINL void     AfxModV3d(afxReal v[3], afxReal const a[3], afxReal const b[3]);
AFXINL void     AfxModV4d(afxReal v[4], afxReal const a[4], afxReal const b[4]);

AFXINL void     AfxScaleV2d(afxReal v[2], afxReal const a[2], afxReal lambda); // v = a * lambda
AFXINL void     AfxScaleV3d(afxReal v[3], afxReal const a[3], afxReal lambda); // v = a * lambda
AFXINL void     AfxScaleV4d(afxReal v[4], afxReal const a[4], afxReal lambda); // v = a * lambda

AFXINL void     AfxMultiplyV2d(afxReal v[2], afxReal const a[2], afxReal const b[2]); // v = a * b
AFXINL void     AfxMultiplyV3d(afxReal v[3], afxReal const a[3], afxReal const b[3]); // v = a * b
AFXINL void     AfxMultiplyV4d(afxReal v[4], afxReal const a[4], afxReal const b[4]); // v = a * b

// v = in * mul + plus

AFXINL void     AfxMadV2d(afxReal v[2], afxReal const in[2], afxReal const mul[2], afxReal const plus[2]);
AFXINL void     AfxMadV3d(afxReal v[3], afxReal const in[3], afxReal const mul[3], afxReal const plus[3]);
AFXINL void     AfxMadV4d(afxReal v[4], afxReal const in[4], afxReal const mul[4], afxReal const plus[4]);

// v = c - (a * b)

AFXINL void     AfxNemusV2d(afxReal v[2], afxReal const a[2], afxReal const b[2], afxReal const c[2]);
AFXINL void     AfxNemusV3d(afxReal v[3], afxReal const a[3], afxReal const b[3], afxReal const c[3]);
AFXINL void     AfxNemusV4d(afxReal v[4], afxReal const a[4], afxReal const b[4], afxReal const c[4]);

/// Vector transformation is performed by multiplying vector A rows by matrix B columns.
///
///     [ aX ] [ bXX bXY bXZ bXW ]
///     [ aY ] [ bYX bYY bYZ bYW ]
///     [ aZ ] [ bZX bZY bZZ bZW ]
///     [ aW ] [ bWX bWY bWZ bWW ] => v
///
/// Vector transformation transposed is performed by multiplying matrix A columns by vector B rows.
///
///     [ aXX aYX aZX aWX ] [ bX ]
///     [ aXY aYY aZY aWY ] [ bY ]
///     [ aXZ aYZ aZZ aWZ ] [ bZ ]
///     [ aXW aYW aZW aWW ] [ bW ] => v

AFXINL void     AfxTransformV2d(afxReal v[2], afxReal const a[2], afxReal const b[2][2]); // v = a row * b row
AFXINL void     AfxTransformV2d_M4d(afxReal v[2], afxReal const a[2], afxReal const b[4][4]); // v = a row * b row

AFXINL void     AfxTransformV3d(afxReal v[3], afxReal const a[3], afxReal const b[3][3]); // v = a row * b row
AFXINL void     AfxTransformV3dTransposed(afxReal v[3], afxReal const a[3][3], afxReal const b[3]); // v = a column * b column

AFXINL void     AfxTransformV4d(afxReal v[4], afxReal const a[4], afxReal const b[4][4]); // v = a row * b row
AFXINL void     AfxTransformV4dTransposed(afxReal v[4], afxReal const a[4][4], afxReal const b[4]); // v = a column * b column

AFXINL void     AfxTransformLinearV3d(afxReal v[3], afxReal const a[3], afxReal const b[4][4]); // v = a row * b row
AFXINL void     AfxTransformLinearV3dTransposed(afxReal v[3], afxReal const a[4][4], afxReal const b[3]); // v = a column * b column

AFXINL void     AfxTransformAffineV3d(afxReal v[3], afxReal const a[3], afxReal const b[4][4]); // v = a row * b row
AFXINL void     AfxTransformAffineV3dTransposed(afxReal v[3], afxReal const a[4][4], afxReal const b[3]); // v = a column * b column

AFXINL void     AfxTransformAffineV4d(afxReal v[4], afxReal const a[4], afxReal const b[4][4]);  // v = a row * b row
AFXINL void     AfxTransformAffineV4dTransposed(afxReal v[4], afxReal const a[4][4], afxReal const b[4]); // v = a column * b column

/// Linear interpolation
/// v = a + t * (b - a)

AFXINL void     AfxLerpV2d(afxReal v[2], afxReal const a[2], afxReal const b[2], afxReal t);
AFXINL void     AfxLerpV3d(afxReal v[3], afxReal const a[3], afxReal const b[3], afxReal t);
AFXINL void     AfxLerpV4d(afxReal v[4], afxReal const a[4], afxReal const b[4], afxReal t);

/// Hermite interpolation
/// v = (2 * t^3 - 3 * t^2 + 1) * posA + (t^3 - 2 * t^2 + t) * tanA + (-2 * t^3 + 3 * t^2) * posB + (t^3 - t^2) * tanB

AFXINL void     AfxHermiteV2d(afxReal v[2], afxReal const posA[2], afxReal const tanA[2], afxReal const posB[2], afxReal const tanB[2], afxReal t);
AFXINL void     AfxHermiteV3d(afxReal v[3], afxReal const posA[3], afxReal const tanA[3], afxReal const posB[3], afxReal const tanB[3], afxReal t);
AFXINL void     AfxHermiteV4d(afxReal v[4], afxReal const posA[4], afxReal const tanA[4], afxReal const posB[4], afxReal const tanB[4], afxReal t);

/// Catmull-Rom splines are a family of cubic interpolating splines formulated such that the tangent at each point Pi is calculated using the previous and next point on the spline, T(Pi + 1 - Pi - 1).
///
///     [ aX, aY, aZ, aW ] [  0  2  0  0 ] [  1  ]
///     [ bX, bY, bZ, bW ] [ -1  0  1  0 ] [  t  ]
///     [ cX, cY, cZ, cW ] [  2 -5  4 -1 ] [ t^2 ]
///     [ dX, dY, dZ, dW ] [ -1  3 -3  1 ] [ t^3 ] * 0.5 => v

/// v = ((-t^3 + 2 * t^2 - t) * a + (3 * t^3 - 5 * t^2 + 2) * b + (-3 * t^3 + 4 * t^2 + t) * c + (t^3 - t^2) * d) * 0.5

AFXINL void     AfxCatmullV2d(afxReal v[2], afxReal const a[2], afxReal const b[2], afxReal const c[2], afxReal const d[2], afxReal t);
AFXINL void     AfxCatmullV3d(afxReal v[3], afxReal const a[3], afxReal const b[3], afxReal const c[3], afxReal const d[3], afxReal t);
AFXINL void     AfxCatmullV4d(afxReal v[4], afxReal const a[4], afxReal const b[4], afxReal const c[4], afxReal const d[4], afxReal t);

AFXINL void     AfxExtractNormalV3dComponents(afxReal const v[3], afxReal const normal[3], afxReal parallel[3], afxReal perpendicular[3]);

AFXINL void     AfxReflectV2d(afxReal v[2], afxReal const incident[2], afxReal const normal[2]);
AFXINL void     AfxReflectV3d(afxReal v[3], afxReal const incident[3], afxReal const normal[3]);
AFXINL void     AfxReflectV4d(afxReal v[4], afxReal const incident[3], afxReal const normal[3]);

AFXINL void     AfxRefractV2d(afxReal v[2], afxReal const incident[2], afxReal const normal[2], afxReal refracIdx);
AFXINL void     AfxRefractV3d(afxReal v[3], afxReal const incident[3], afxReal const normal[3], afxReal refracIdx);
AFXINL void     AfxRefractV3d(afxReal v[3], afxReal const incident[3], afxReal const normal[3], afxReal refracIdx);

////////////////////////////////////////////////////////////////////////////////
// Algebra                                                                    //
////////////////////////////////////////////////////////////////////////////////

AFXINL void     AfxSqrtV2d(afxReal v[2], afxReal const in[2]); // v = sqrt(in)
AFXINL void     AfxSqrtV3d(afxReal v[3], afxReal const in[3]); // v = sqrt(in)
AFXINL void     AfxSqrtV4d(afxReal v[4], afxReal const in[4]); // v = sqrt(in)

AFXINL void     AfxRsqrtV2d(afxReal v[2], afxReal const in[2]); // v = 1 / sqrt(in)
AFXINL void     AfxRsqrtV3d(afxReal v[3], afxReal const in[3]); // v = 1 / sqrt(in)
AFXINL void     AfxRsqrtV4d(afxReal v[4], afxReal const in[4]); // v = 1 / sqrt(in)

AFXINL void     AfxSinV2d(afxReal v[2], afxReal const in[2]); // v = sin(in)
AFXINL void     AfxSinV3d(afxReal v[3], afxReal const in[3]); // v = sin(in)
AFXINL void     AfxSinV4d(afxReal v[4], afxReal const in[4]); // v = sin(in)

AFXINL void     AfxCosV2d(afxReal v[2], afxReal const in[2]); // v = cos(in)
AFXINL void     AfxCosV3d(afxReal v[3], afxReal const in[3]); // v = cos(in)
AFXINL void     AfxCosV4d(afxReal v[4], afxReal const in[4]); // v = cos(in)

AFXINL void     AfxAcosV2d(afxReal v[2], afxReal const in[2]); // v = acos(in)
AFXINL void     AfxAcosV3d(afxReal v[3], afxReal const in[3]); // v = acos(in)
AFXINL void     AfxAcosV4d(afxReal v[4], afxReal const in[4]); // v = acos(in)

AFXINL void     AfxTanV2d(afxReal v[2], afxReal const in[2]); // v = tan(in)
AFXINL void     AfxTanV3d(afxReal v[3], afxReal const in[3]); // v = tan(in)
AFXINL void     AfxTanV4d(afxReal v[4], afxReal const in[4]); // v = tan(in)

AFXINL void     AfxAtanV2d(afxReal v[2], afxReal const in[2]); // v = atan(in)
AFXINL void     AfxAtanV3d(afxReal v[3], afxReal const in[3]); // v = atan(in)
AFXINL void     AfxAtanV4d(afxReal v[4], afxReal const in[4]); // v = atan(in)

AFXINL void     AfxLogV2d(afxReal v[2], afxReal const in[2]);
AFXINL void     AfxLogV3d(afxReal v[3], afxReal const in[3]);
AFXINL void     AfxLogV4d(afxReal v[4], afxReal const in[4]);

AFXINL void     AfxLog2V2d(afxReal v[2], afxReal const in[2]);
AFXINL void     AfxLog2V3d(afxReal v[3], afxReal const in[3]);
AFXINL void     AfxLog2V4d(afxReal v[4], afxReal const in[4]);

AFXINL void     AfxLog10V2d(afxReal v[2], afxReal const in[2]);
AFXINL void     AfxLog10V3d(afxReal v[3], afxReal const in[3]);
AFXINL void     AfxLog10V4d(afxReal v[4], afxReal const in[4]);

AFXINL void     AfxPowV2d(afxReal v[2], afxReal const in[2]);
AFXINL void     AfxPowV3d(afxReal v[3], afxReal const in[3]);
AFXINL void     AfxPowV4d(afxReal v[4], afxReal const in[4]);

AFXINL void     AfxExpV2d(afxReal v[2], afxReal const in[2]);
AFXINL void     AfxExpV3d(afxReal v[3], afxReal const in[3]);
AFXINL void     AfxExpV4d(afxReal v[4], afxReal const in[4]);

AFXINL void     AfxExp2V2d(afxReal v[2], afxReal const in[2]);
AFXINL void     AfxExp2V3d(afxReal v[3], afxReal const in[3]);
AFXINL void     AfxExp2V4d(afxReal v[4], afxReal const in[4]);

AFXINL void     AfxExp10V2d(afxReal v[2], afxReal const in[2]);
AFXINL void     AfxExp10V3d(afxReal v[3], afxReal const in[3]);
AFXINL void     AfxExp10V4d(afxReal v[4], afxReal const in[4]);

AFXINL void     AfxNegateV2d(afxReal v[2], afxReal const in[2]);
AFXINL void     AfxNegateV3d(afxReal v[3], afxReal const in[3]);
AFXINL void     AfxNegateV4d(afxReal v[4], afxReal const in[4]);
AFXINL void     AfxNegateAffineV4d(afxReal v[4], afxReal const in[4]); // v = inverse of in

AFXINL afxReal  AfxSumV2d(afxReal const v[2]);
AFXINL afxReal  AfxSumV3d(afxReal const v[3]);
AFXINL afxReal  AfxSumV4d(afxReal const v[4]);

// Computes the dot product (a dot b)
// The dot product only defines the angle between both vectors.
// You may remember that the cosine or cos function becomes 0 when the angle is 90 degrees or 1 when the angle is 0.
// This allows us dst easily test if the two vectors are orthogonal or parallel dst each other using the dot product (orthogonal means the vectors are at a right-angle dst each other).

AFXINL afxReal  AfxDotV2d(afxReal const in[2], afxReal const other[2]);
AFXINL afxReal  AfxDotV3d(afxReal const in[3], afxReal const other[3]);
AFXINL afxReal  AfxDotV4d(afxReal const in[4], afxReal const other[4]);

// Computes the cross product (a cross b) and stores the result in v
// The cross product is only defined in 3D space and takes two non-parallel vectors as input and produces a third vector that is orthogonal dst both the input vectors.
// If both the input vectors are orthogonal dst each other as well, a cross product would result in 3 orthogonal vectors; this will prove useful in the upcoming chapters.
// The following image shows what this looks like in 3D space:

AFXINL void     AfxCrossV2d(afxReal v[2], afxReal const a[2], afxReal const b[2]);
AFXINL void     AfxCrossV3d(afxReal v[3], afxReal const a[3], afxReal const b[3]);
AFXINL void     AfxCrossV4d(afxReal v[4], afxReal const a[4], afxReal const b[4], afxReal const c[4]);

// Computes the squared magnitude

AFXINL afxReal  AfxMagV2dSq(afxReal const in[2]);
AFXINL afxReal  AfxMagV3dSq(afxReal const in[3]);
AFXINL afxReal  AfxMagV4dSq(afxReal const in[4]);

// The magnitude/length of a vector in Cartesian coordinates is the square root of the sum of the squares of its coordinates.

AFXINL afxReal  AfxMagV2d(afxReal const in[2]);
AFXINL afxReal  AfxMagV3d(afxReal const in[3]);
AFXINL afxReal  AfxMagV4d(afxReal const in[4]);

AFXINL afxReal  AfxMagV2dRecip(afxReal const in[2]);
AFXINL afxReal  AfxMagV3dRecip(afxReal const in[3]);
AFXINL afxReal  AfxMagV4dRecip(afxReal const in[4]);

////////////////////////////////////////////////////////////////////////////////
// Affine transformation                                                      //
////////////////////////////////////////////////////////////////////////////////

// Similarity transform

AFXINL void     AfxGetAssimilatedPositionV3d(afxReal v[3], afxReal const in[3], afxReal const affine[3], afxReal const linear[3][3]); // make similarity transformation on afxV3d-based position.
AFXINL void     AfxGetAssimilatedPositionV4d(afxReal v[4], afxReal const in[3], afxReal const affine[3], afxReal const linear[3][3]); // make similarity transformation on afxV3d-based position.

////////////////////////////////////////////////////////////////////////////////
// Trigonometry                                                               //
////////////////////////////////////////////////////////////////////////////////

AFXINL afxReal AfxGetDistanceBetweenV3d(afxReal const v[3], afxReal const other[3]);

AFXINL afxReal  AfxGetAngleBetweenV3d(afxReal const v[3], afxReal const other[3]);

/// Barycentric coordinates are very useful in 2D and 3D graphics. 
/// Most graphic applications use them because they provide an easy way to interpolate the value of attributes (color, textures, normals…) between vertices. 
/// This can be done because, by definition, barycentric coordinates express “how much of each vertex does a point have”.
/// Another frequent use is to use the barycentric coordinates to check if a point is inside, on the edge, or outside the triangle. As we saw:
///     If all barycentric coordinates of a point are positive and sum one, they point lies inside the triangle.
///     If one barycentric coordinate is zero and the other ones are positive and less than one, the point lies on an edge.
///     If any barycentric coordinate is negative, the point is outside the triangle.

/// Returns a point in Barycentric coordinates, using the specified position vectors.

AFXINL void     AfxBarycentricV2d(afxReal v[2], afxReal const a[2], afxReal const b[2], afxReal const c[2], afxReal f, afxReal g);
AFXINL void     AfxBarycentricV3d(afxReal v[3], afxReal const a[3], afxReal const b[3], afxReal const c[3], afxReal f, afxReal g);
AFXINL void     AfxBarycentricV4d(afxReal v[4], afxReal const a[4], afxReal const b[4], afxReal const c[4], afxReal f, afxReal g);

AFXINL void     AfxBarycentricV2d2(afxReal v[2], afxReal const a[2], afxReal const b[2], afxReal const c[2], afxReal const f[2], afxReal const g[2]);
AFXINL void     AfxBarycentricV3d2(afxReal v[3], afxReal const a[3], afxReal const b[3], afxReal const c[3], afxReal const f[3], afxReal const g[3]);
AFXINL void     AfxBarycentricV4d2(afxReal v[4], afxReal const a[4], afxReal const b[4], afxReal const c[4], afxReal const f[4], afxReal const g[4]);

#endif//AFX_VECTOR_H