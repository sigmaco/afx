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

#ifndef AFX_VECTOR_H
#define AFX_VECTOR_H

#include "afxReal.h"

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

AFXINL void     AfxCopyArrayedV2d(afxNat cnt, afxReal const in[][2], afxReal out[][2]);
AFXINL void     AfxCopyArrayedV3d(afxNat cnt, afxReal const in[][3], afxReal out[][3]);
AFXINL void     AfxCopyArrayedV4d(afxNat cnt, afxReal const in[][4], afxReal out[][4]);

AFXINL void     AfxLtv3FromLtv2(afxReal v[3], afxReal const in[2]); // 2D linear transformation vector. Z is 0.
AFXINL void     AfxV4dFromV2d(afxReal v[4], afxReal const in[2]);
AFXINL void     AfxV4dFromV3d(afxReal v[4], afxReal const in[3]);
AFXINL void     AfxAtv3FromAtv2(afxReal v[3], afxReal const in[2]); // 2D affine transfomartion vector. Z is 1.
AFXINL void     AfxAtv4FromAtv3(afxReal v[4], afxReal const in[3]); // 3D affine transformation vector. W is 1.

AFXINL void     AfxSwapV2d(afxReal v[2], afxReal other[2]);
AFXINL void     AfxSwapV3d(afxReal v[3], afxReal other[3]);
AFXINL void     AfxSwapV4d(afxReal v[4], afxReal other[4]);

////////////////////////////////////////////////////////////////////////////////
// Clamping                                                                   //
////////////////////////////////////////////////////////////////////////////////

AFXINL void     AfxClampV2d(afxReal v[2], afxReal const in[2], afxReal const min[2], afxReal const max[2]); // v = IN between bounds
AFXINL void     AfxClampV3d(afxReal v[3], afxReal const in[3], afxReal const min[3], afxReal const max[3]);
AFXINL void     AfxClampV4d(afxReal v[4], afxReal const in[4], afxReal const min[4], afxReal const max[4]);

AFXINL void     AfxTruncateV2d(afxReal v[2], afxReal const in[2]);
AFXINL void     AfxTruncateV3d(afxReal v[3], afxReal const in[3]);
AFXINL void     AfxTruncateV4d(afxReal v[4], afxReal const in[4]);

AFXINL void     AfxSaturateV2d(afxReal v[2], afxReal const in[2]);
AFXINL void     AfxSaturateV3d(afxReal v[3], afxReal const in[3]);
AFXINL void     AfxSaturateV4d(afxReal v[4], afxReal const in[4]);

AFXINL void     AfxMinV2d(afxReal v[2], afxReal const a[2], afxReal const b[2]); // v = minor between A and B
AFXINL void     AfxMinV3d(afxReal v[3], afxReal const a[3], afxReal const b[3]); // v = minor between A and B
AFXINL void     AfxMinV4d(afxReal v[4], afxReal const a[4], afxReal const b[4]); // v = minor between A and B

AFXINL void     AfxMaxV2d(afxReal v[2], afxReal const a[2], afxReal const b[2]); // v = major between A and B
AFXINL void     AfxMaxV3d(afxReal v[3], afxReal const a[3], afxReal const b[3]); // v = major between A and B
AFXINL void     AfxMaxV4d(afxReal v[4], afxReal const a[4], afxReal const b[4]); // v = major between A and B

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

AFXINL afxReal  AfxNormalV2d(afxReal v[2], afxReal const in[2]);
AFXINL afxReal  AfxNormalV3d(afxReal v[3], afxReal const in[3]);
AFXINL afxReal  AfxNormalV4d(afxReal v[4], afxReal const in[4]);

AFXINL afxReal  AfxNormalV3dFromV4d(afxReal v[3], afxReal const in[4]);
AFXINL afxReal  AfxNormalV4dFromV3d(afxReal v[4], afxReal const in[3]);

AFXINL afxReal  AfxEstimateNormalizedV2d(afxReal v[2], afxReal const in[2]);
AFXINL afxReal  AfxEstimateNormalizedV3d(afxReal v[3], afxReal const in[3]);
AFXINL afxReal  AfxEstimateNormalizedV4d(afxReal v[4], afxReal const in[4]);

AFXINL void     AfxNormalizeArrayedV2d(afxNat cnt, afxReal const in[][2], afxReal out[][2]);
AFXINL void     AfxNormalizeArrayedV3d(afxNat cnt, afxReal const in[][3], afxReal out[][3]);
AFXINL void     AfxNormalizeArrayedV4d(afxNat cnt, afxReal const in[][4], afxReal out[][4]);

AFXINL void     AfxNormalizeOrZeroArrayedV2d(afxNat cnt, afxReal const in[][2], afxReal out[][2]);
AFXINL void     AfxNormalizeOrZeroArrayedV3d(afxNat cnt, afxReal const in[][3], afxReal out[][3]);
AFXINL void     AfxNormalizeOrZeroArrayedV4d(afxNat cnt, afxReal const in[][4], afxReal out[][4]);

////////////////////////////////////////////////////////////////////////////////
// Arithmetica                                                                //
////////////////////////////////////////////////////////////////////////////////

// v = a + b

AFXINL void     AfxAddV2d(afxReal v[2], afxReal const a[2], afxReal const b[2]);
AFXINL void     AfxAddV3d(afxReal v[3], afxReal const a[3], afxReal const b[3]);
AFXINL void     AfxAddV4d(afxReal v[4], afxReal const a[4], afxReal const b[4]);

// v = a - b

AFXINL void     AfxSubV2d(afxReal v[2], afxReal const a[2], afxReal const b[2]);
AFXINL void     AfxSubV3d(afxReal v[3], afxReal const a[3], afxReal const b[3]);
AFXINL void     AfxSubV4d(afxReal v[4], afxReal const a[4], afxReal const b[4]);

// v = a / b

AFXINL void     AfxDivV2d(afxReal v[2], afxReal const a[2], afxReal const b[2]);
AFXINL void     AfxDivV3d(afxReal v[3], afxReal const a[3], afxReal const b[3]);
AFXINL void     AfxDivV4d(afxReal v[4], afxReal const a[4], afxReal const b[4]);

AFXINL void     AfxModV2d(afxReal v[2], afxReal const a[2], afxReal const b[2]);
AFXINL void     AfxModV3d(afxReal v[3], afxReal const a[3], afxReal const b[3]);
AFXINL void     AfxModV4d(afxReal v[4], afxReal const a[4], afxReal const b[4]);

// v = a * lambda

AFXINL void     AfxScaleV2d(afxReal v[2], afxReal const a[2], afxReal lambda);
AFXINL void     AfxScaleV3d(afxReal v[3], afxReal const a[3], afxReal lambda);
AFXINL void     AfxScaleV4d(afxReal v[4], afxReal const a[4], afxReal lambda);

// v = in * 0.5

AFXINL void     AfxHalfV2d(afxReal v[2], afxReal const in[2]);
AFXINL void     AfxHalfV3d(afxReal v[3], afxReal const in[3]);
AFXINL void     AfxHalfV4d(afxReal v[4], afxReal const in[4]);

// v = a * b

AFXINL void     AfxMultiplyV2d(afxReal v[2], afxReal const a[2], afxReal const b[2]);
AFXINL void     AfxMultiplyV3d(afxReal v[3], afxReal const a[3], afxReal const b[3]);
AFXINL void     AfxMultiplyV4d(afxReal v[4], afxReal const a[4], afxReal const b[4]);

// v = in * mul + plus

AFXINL void     AfxMadV2d(afxReal v[2], afxReal const in[2], afxReal const mul[2], afxReal const plus[2]);
AFXINL void     AfxMadV3d(afxReal v[3], afxReal const in[3], afxReal const mul[3], afxReal const plus[3]);
AFXINL void     AfxMadV4d(afxReal v[4], afxReal const in[4], afxReal const mul[4], afxReal const plus[4]);

// v = lambda1 * a + lambda2 * b

AFXINL void     AfxCombineV2d(afxReal v[2], afxReal lambda1, afxReal const a[2], afxReal lambda2, afxReal const b[2]);
AFXINL void     AfxCombineV3d(afxReal v[3], afxReal lambda1, afxReal const a[3], afxReal lambda2, afxReal const b[3]);
AFXINL void     AfxCombineV4d(afxReal v[4], afxReal lambda1, afxReal const a[4], afxReal lambda2, afxReal const b[4]);

// reverse subtract
// v = c - (a * b)

AFXINL void     AfxResubV2d(afxReal v[2], afxReal const a[2], afxReal const b[2], afxReal const c[2]);
AFXINL void     AfxResubV3d(afxReal v[3], afxReal const a[3], afxReal const b[3], afxReal const c[3]);
AFXINL void     AfxResubV4d(afxReal v[4], afxReal const a[4], afxReal const b[4], afxReal const c[4]);

/// Linear interpolation
/// v = x * (1 - t) + y * t

AFXINL void     AfxLerpV2d(afxReal v[2], afxReal const x[2], afxReal const y[2], afxReal t);
AFXINL void     AfxLerpV3d(afxReal v[3], afxReal const x[3], afxReal const y[3], afxReal t);
AFXINL void     AfxLerpV4d(afxReal v[4], afxReal const x[4], afxReal const y[4], afxReal t);

AFXINL void     AfxMixV2d(afxReal v[2], afxReal const x[2], afxReal const y[2], afxReal t);
AFXINL void     AfxMixV3d(afxReal v[3], afxReal const x[3], afxReal const y[3], afxReal t);
AFXINL void     AfxMixV4d(afxReal v[4], afxReal const x[4], afxReal const y[4], afxReal t);

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

AFXINL void     AfxNegV2d(afxReal v[2], afxReal const in[2]);
AFXINL void     AfxNegV3d(afxReal v[3], afxReal const in[3]);
AFXINL void     AfxNegV4d(afxReal v[4], afxReal const in[4]);
AFXINL void     AfxNegAtv4(afxReal v[4], afxReal const in[4]); // v = inverse of in

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
// VECTOR TRANSFORMATION METHODS                                              //
////////////////////////////////////////////////////////////////////////////////

//  --- Normal layout
//  m[0][0] * v[0], m[0][1] * v[1], m[0][2] * v[2], m[0][3] * v[3]

//  --- Transposed layout
//  v[0] * m[0][0], v[1] * m[1][0], v[2] * m[2][0], v[3] * m[3][0]


// Multiply (row) vector by matrix ([0][0], [0][1], [0][2], [0][3])

// With post-multiplication, a 4x4 matrix multiplied with a 4x1 column vector took the dot product of each row of the matrix with the vector.

AFXINL void     AfxPostMultiplyV2d(afxReal const m[2][2], afxReal const in[2], afxReal out[2]);
AFXINL void     AfxPostMultiplyV3d(afxReal const m[3][3], afxReal const in[3], afxReal out[3]);
AFXINL void     AfxPostMultiplyV4d(afxReal const m[4][4], afxReal const in[4], afxReal out[4]);

AFXINL void     AfxPostMultiplyLtv3(afxReal const m[4][4], afxReal const in[3], afxReal out[3]);
AFXINL void     AfxPostMultiplyLtv4(afxReal const m[3][3], afxReal const in[4], afxReal out[4]);

AFXINL void     AfxPostMultiplyAtv2(afxReal const m[4][4], afxReal const in[2], afxReal out[2]);
AFXINL void     AfxPostMultiplyAtv3(afxReal const m[4][4], afxReal const in[3], afxReal out[3]);
AFXINL void     AfxPostMultiplyAtv4(afxReal const m[4][4], afxReal const in[4], afxReal out[4]);

// With pre-multiplication, the dot product is with the vector and each column of the matrix (since the matrix is now on the right side of the multiplication operator).

// Multiply column vector by matrix ([0][0], [1][0], [2][0], [3][0])

AFXINL void     AfxPreMultiplyV2d(afxReal const m[2][2], afxReal const in[2], afxReal out[2]);
AFXINL void     AfxPreMultiplyV3d(afxReal const m[3][3], afxReal const in[3], afxReal out[3]);
AFXINL void     AfxPreMultiplyV4d(afxReal const m[4][4], afxReal const in[4], afxReal out[4]);

AFXINL void     AfxPreMultiplyLtv3(afxReal const m[4][4], afxReal const in[3], afxReal out[3]);
AFXINL void     AfxPreMultiplyLtv4(afxReal const m[3][3], afxReal const in[4], afxReal out[4]);

AFXINL void     AfxPreMultiplyAtv2(afxReal const m[4][4], afxReal const in[2], afxReal out[2]);
AFXINL void     AfxPreMultiplyAtv3(afxReal const m[4][4], afxReal const in[3], afxReal out[3]);
AFXINL void     AfxPreMultiplyAtv4(afxReal const m[4][4], afxReal const in[4], afxReal out[4]);

// Similarity transform

AFXINL void     AfxAssimilateAtv3(afxReal const ltm[3][3], afxReal const atv[4], afxNat cnt, afxReal const in[][3], afxReal out[][3]); // make similarity transformation on afxV3d-based position.
AFXINL void     AfxAssimilateAtv4(afxReal const ltm[3][3], afxReal const atv[4], afxNat cnt, afxReal const in[][4], afxReal out[][4]); // make similarity transformation on afxV3d-based position.

#endif//AFX_VECTOR_H
