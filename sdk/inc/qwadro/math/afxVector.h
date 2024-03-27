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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

#ifndef AFX_VECTOR_H
#define AFX_VECTOR_H

#include "qwadro/math/afxMathDefs.h"

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

#define AfxSpawnV2d(x_, y_) (afxV2d){ (afxReal)(x_), (afxReal)(y_) }
#define AfxSpawnV3d(x_, y_, z_) (afxV3d){ (afxReal)x_, (afxReal)y_, (afxReal)z_ }
#define AfxSpawnV4d(x_, y_, z_, w_) (afxV4d const){ (afxReal)x_, (afxReal)y_, (afxReal)z_, (afxReal)w_ }
#define AfxSpawnPoint(x_, y_, z_) (afxV4d const){ (afxReal)x_, (afxReal)y_, (afxReal)z_, (afxReal)1 }

#define AfxSpawnConstV2d(x_, y_) (afxV2d const){ (afxReal)(x_), (afxReal)(y_) }
#define AfxSpawnConstV3d(x_, y_, z_) (afxV3d const){ (afxReal)x_, (afxReal)y_, (afxReal)z_ }
#define AfxSpawnV4dConst(x_, y_, z_, w_) (afxV4d const){ (afxReal)x_, (afxReal)y_, (afxReal)z_, (afxReal)w_ }
#define AfxSpawnPointConst(x_, y_, z_) (afxV4d const){ (afxReal)x_, (afxReal)y_, (afxReal)z_, (afxReal)1 }

#define AfxV2d(x_, y_) (afxV2d const){ (afxReal)(x_), (afxReal)(y_) }
#define AfxV3d(x_, y_, z_) (afxV3d const){ (afxReal)x_, (afxReal)y_, (afxReal)z_ }
#define AfxV4d(x_, y_, z_, w_) (afxV4d const){ (afxReal)x_, (afxReal)y_, (afxReal)z_, (afxReal)w_ }
#define AfxAtv4d(x_, y_, z_) (afxV4d const){ (afxReal)x_, (afxReal)y_, (afxReal)z_, (afxReal)1 }

////////////////////////////////////////////////////////////////////////////////
// Initialization                                                             //
////////////////////////////////////////////////////////////////////////////////

AFXINL void     AfxZeroV2d(afxV2d v);
AFXINL void     AfxZeroV3d(afxV3d v);
AFXINL void     AfxZeroV4d(afxV4d v);

AFXINL void     AfxFillV2d(afxV2d v, afxReal value);
AFXINL void     AfxFillV3d(afxV3d v, afxReal value);
AFXINL void     AfxFillV4d(afxV4d v, afxReal value);

AFXINL void     AfxSetV2d(afxV2d v, afxReal x, afxReal y);
AFXINL void     AfxSetV3d(afxV3d v, afxReal x, afxReal y, afxReal z);
AFXINL void     AfxSetV4d(afxV4d v, afxReal x, afxReal y, afxReal z, afxReal w);

AFXINL void     AfxResetV4d(afxV4d v);

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

AFXINL void     AfxCopyV2d(afxV2d v, afxV2d const in);
AFXINL void     AfxCopyV3d(afxV3d v, afxV3d const in);
AFXINL void     AfxCopyV4d(afxV4d v, afxV4d const in);

AFXINL void     AfxCopyArrayedV2d(afxNat cnt, afxV2d const in[], afxV2d out[]);
AFXINL void     AfxCopyArrayedV3d(afxNat cnt, afxV3d const in[], afxV3d out[]);
AFXINL void     AfxCopyArrayedV4d(afxNat cnt, afxV4d const in[], afxV4d out[]);

AFXINL void     AfxCopyV3dFromV2d(afxV3d v, afxV2d const in); // 2D linear transformation vector. Z is 0.
AFXINL void     AfxCopyLtv4dFromV2d(afxV4d v, afxV2d const in);
AFXINL void     AfxCopyLtv4d(afxV4d v, afxV3d const in);
AFXINL void     AfxCopyV3dFromV2d_(afxV3d v, afxV2d const in); // 2D affine transfomartion vector. Z is 1.
AFXINL void     AfxCopyAtv4d(afxV4d v, afxV3d const in); // 3D affine transformation vector. W is 1.

AFXINL void     AfxSwapV2d(afxV2d v, afxV2d other);
AFXINL void     AfxSwapV3d(afxV3d v, afxV3d other);
AFXINL void     AfxSwapV4d(afxV4d v, afxV4d other);

////////////////////////////////////////////////////////////////////////////////
// Clamping                                                                   //
////////////////////////////////////////////////////////////////////////////////

AFXINL void     AfxClampV2d(afxV2d v, afxV2d const in, afxV2d const min, afxV2d const max); // v = IN between bounds
AFXINL void     AfxClampV3d(afxV3d v, afxV3d const in, afxV3d const min, afxV3d const max);
AFXINL void     AfxClampV4d(afxV4d v, afxV4d const in, afxV4d const min, afxV4d const max);

AFXINL void     AfxTruncateV2d(afxV2d v, afxV2d const in);
AFXINL void     AfxTruncateV3d(afxV3d v, afxV3d const in);
AFXINL void     AfxTruncateV4d(afxV4d v, afxV4d const in);

AFXINL void     AfxSaturateV2d(afxV2d v, afxV2d const in);
AFXINL void     AfxSaturateV3d(afxV3d v, afxV3d const in);
AFXINL void     AfxSaturateV4d(afxV4d v, afxV4d const in);

AFXINL void     AfxMinV2d(afxV2d v, afxV2d const a, afxV2d const b); // v = minor between A and B
AFXINL void     AfxMinV3d(afxV3d v, afxV3d const a, afxV3d const b); // v = minor between A and B
AFXINL void     AfxMinV4d(afxV4d v, afxV4d const a, afxV4d const b); // v = minor between A and B

AFXINL void     AfxMaxV2d(afxV2d v, afxV2d const a, afxV2d const b); // v = major between A and B
AFXINL void     AfxMaxV3d(afxV3d v, afxV3d const a, afxV3d const b); // v = major between A and B
AFXINL void     AfxMaxV4d(afxV4d v, afxV4d const a, afxV4d const b); // v = major between A and B

AFXINL void     AfxAbsV2d(afxV2d v, afxV2d const in); // v = abs(in)
AFXINL void     AfxAbsV3d(afxV3d v, afxV3d const in); // v = abs(in)
AFXINL void     AfxAbsV4d(afxV4d v, afxV4d const in); // v = abs(in)

AFXINL void     AfxCeilV2d(afxV2d v, afxV2d const in); // v = ceil(in)
AFXINL void     AfxCeilV3d(afxV3d v, afxV3d const in); // v = ceil(in)
AFXINL void     AfxCeilV4d(afxV4d v, afxV4d const in); // v = ceil(in)

AFXINL void     AfxFloorV2d(afxV2d v, afxV2d const in); // v = floor(in)
AFXINL void     AfxFloorV3d(afxV3d v, afxV3d const in); // v = floor(in)
AFXINL void     AfxFloorV4d(afxV4d v, afxV4d const in); // v = floor(in)

AFXINL void     AfxRecipV2d(afxV2d v, afxV2d const in); // v = 1 / in
AFXINL void     AfxRecipV3d(afxV3d v, afxV3d const in); // v = 1 / in
AFXINL void     AfxRecipV4d(afxV4d v, afxV4d const in); // v = 1 / in

AFXINL afxReal  AfxNormalizeV2d(afxV2d v, afxV2d const in);
AFXINL afxReal  AfxNormalizeV3d(afxV3d v, afxV3d const in);
AFXINL afxReal  AfxNormalizeV4d(afxV4d v, afxV4d const in);

AFXINL afxReal  AfxNormalizeV3dFromV4d(afxV3d v, afxV4d const in);
AFXINL afxReal  AfxNormalizeV4dFromV3d(afxV4d v, afxV3d const in);

AFXINL afxReal  AfxNormalizeV2dEstimated(afxV2d v, afxV2d const in);
AFXINL afxReal  AfxNormalizeV3dEstimated(afxV3d v, afxV3d const in);
AFXINL afxReal  AfxNormalizeV4dEstimated(afxV4d v, afxV4d const in);

AFXINL void     AfxNormalizeArrayedV2d(afxNat cnt, afxV2d const in[], afxV2d out[]);
AFXINL void     AfxNormalizeArrayedV3d(afxNat cnt, afxV3d const in[], afxV3d out[]);
AFXINL void     AfxNormalizeArrayedV4d(afxNat cnt, afxV4d const in[], afxV4d out[]);

AFXINL void     AfxZeroOrNormalizeArrayedV2d(afxNat cnt, afxV2d const in[], afxV2d out[]);
AFXINL void     AfxZeroOrNormalizeArrayedV3d(afxNat cnt, afxV3d const in[], afxV3d out[]);
AFXINL void     AfxZeroOrNormalizeArrayedV4d(afxNat cnt, afxV4d const in[], afxV4d out[]);

////////////////////////////////////////////////////////////////////////////////
// Arithmetica                                                                //
////////////////////////////////////////////////////////////////////////////////

// v = a + b

AFXINL void     AfxAddV2d(afxV2d v, afxV2d const a, afxV2d const b);
AFXINL void     AfxAddV3d(afxV3d v, afxV3d const a, afxV3d const b);
AFXINL void     AfxAddV4d(afxV4d v, afxV4d const a, afxV4d const b);

// v = a + b * lambda

AFXINL void     AfxAddScaledV2d(afxV2d v, afxV2d const a, afxV2d const b, afxReal lambda);
AFXINL void     AfxAddScaledV3d(afxV3d v, afxV3d const a, afxV3d const b, afxReal lambda);
AFXINL void     AfxAddScaledV4d(afxV4d v, afxV4d const a, afxV4d const b, afxReal lambda);

// v = a - b

AFXINL void     AfxSubV2d(afxV2d v, afxV2d const a, afxV2d const b);
AFXINL void     AfxSubV3d(afxV3d v, afxV3d const a, afxV3d const b);
AFXINL void     AfxSubV4d(afxV4d v, afxV4d const a, afxV4d const b);

// v = a / b

AFXINL void     AfxDivV2d(afxV2d v, afxV2d const a, afxV2d const b);
AFXINL void     AfxDivV3d(afxV3d v, afxV3d const a, afxV3d const b);
AFXINL void     AfxDivV4d(afxV4d v, afxV4d const a, afxV4d const b);

// v = a % b

AFXINL void     AfxModV2d(afxV2d v, afxV2d const a, afxV2d const b);
AFXINL void     AfxModV3d(afxV3d v, afxV3d const a, afxV3d const b);
AFXINL void     AfxModV4d(afxV4d v, afxV4d const a, afxV4d const b);

// v = a * lambda

AFXINL void     AfxScaleV2d(afxV2d v, afxV2d const a, afxReal lambda);
AFXINL void     AfxScaleV3d(afxV3d v, afxV3d const a, afxReal lambda);
AFXINL void     AfxScaleV4d(afxV4d v, afxV4d const a, afxReal lambda);

// v = in * 0.5

AFXINL void     AfxHalfV2d(afxV2d v, afxV2d const in);
AFXINL void     AfxHalfV3d(afxV3d v, afxV3d const in);
AFXINL void     AfxHalfV4d(afxV4d v, afxV4d const in);

// v = a * b

AFXINL void     AfxMultiplyV2d(afxV2d v, afxV2d const a, afxV2d const b);
AFXINL void     AfxMultiplyV3d(afxV3d v, afxV3d const a, afxV3d const b);
AFXINL void     AfxMultiplyV4d(afxV4d v, afxV4d const a, afxV4d const b);

// v = in * mul + plus

AFXINL void     AfxMadV2d(afxV2d v, afxV2d const in, afxV2d const mul, afxV2d const plus);
AFXINL void     AfxMadV3d(afxV3d v, afxV3d const in, afxV3d const mul, afxV3d const plus);
AFXINL void     AfxMadV4d(afxV4d v, afxV4d const in, afxV4d const mul, afxV4d const plus);

// v = lambda1 * a + lambda2 * b

AFXINL void     AfxCombineV2d(afxV2d v, afxReal lambda1, afxV2d const a, afxReal lambda2, afxV2d const b);
AFXINL void     AfxCombineV3d(afxV3d v, afxReal lambda1, afxV3d const a, afxReal lambda2, afxV3d const b);
AFXINL void     AfxCombineV4d(afxV4d v, afxReal lambda1, afxV4d const a, afxReal lambda2, afxV4d const b);

// reverse subtract
// v = c - (a * b)

AFXINL void     AfxResubV2d(afxV2d v, afxV2d const a, afxV2d const b, afxV2d const c);
AFXINL void     AfxResubV3d(afxV3d v, afxV3d const a, afxV3d const b, afxV3d const c);
AFXINL void     AfxResubV4d(afxV4d v, afxV4d const a, afxV4d const b, afxV4d const c);

/// Linear interpolation
/// v = x * (1 - t) + y * t

AFXINL void     AfxLerpV2d(afxV2d v, afxV2d const x, afxV2d const y, afxReal t);
AFXINL void     AfxLerpV3d(afxV3d v, afxV3d const x, afxV3d const y, afxReal t);
AFXINL void     AfxLerpV4d(afxV4d v, afxV4d const x, afxV4d const y, afxReal t);

AFXINL void     AfxMixV2d(afxV2d v, afxV2d const x, afxV2d const y, afxReal t);
AFXINL void     AfxMixV3d(afxV3d v, afxV3d const x, afxV3d const y, afxReal t);
AFXINL void     AfxMixV4d(afxV4d v, afxV4d const x, afxV4d const y, afxReal t);

////////////////////////////////////////////////////////////////////////////////
// Algebra                                                                    //
////////////////////////////////////////////////////////////////////////////////

AFXINL void     AfxSqrtV2d(afxV2d v, afxV2d const in); // v = sqrt(in)
AFXINL void     AfxSqrtV3d(afxV3d v, afxV3d const in); // v = sqrt(in)
AFXINL void     AfxSqrtV4d(afxV4d v, afxV4d const in); // v = sqrt(in)

AFXINL void     AfxRsqrtV2d(afxV2d v, afxV2d const in); // v = 1 / sqrt(in)
AFXINL void     AfxRsqrtV3d(afxV3d v, afxV3d const in); // v = 1 / sqrt(in)
AFXINL void     AfxRsqrtV4d(afxV4d v, afxV4d const in); // v = 1 / sqrt(in)

AFXINL void     AfxSinV2d(afxV2d v, afxV2d const in); // v = sin(in)
AFXINL void     AfxSinV3d(afxV3d v, afxV3d const in); // v = sin(in)
AFXINL void     AfxSinV4d(afxV4d v, afxV4d const in); // v = sin(in)

AFXINL void     AfxCosV2d(afxV2d v, afxV2d const in); // v = cos(in)
AFXINL void     AfxCosV3d(afxV3d v, afxV3d const in); // v = cos(in)
AFXINL void     AfxCosV4d(afxV4d v, afxV4d const in); // v = cos(in)

AFXINL void     AfxAcosV2d(afxV2d v, afxV2d const in); // v = acos(in)
AFXINL void     AfxAcosV3d(afxV3d v, afxV3d const in); // v = acos(in)
AFXINL void     AfxAcosV4d(afxV4d v, afxV4d const in); // v = acos(in)

AFXINL void     AfxTanV2d(afxV2d v, afxV2d const in); // v = tan(in)
AFXINL void     AfxTanV3d(afxV3d v, afxV3d const in); // v = tan(in)
AFXINL void     AfxTanV4d(afxV4d v, afxV4d const in); // v = tan(in)

AFXINL void     AfxAtanV2d(afxV2d v, afxV2d const in); // v = atan(in)
AFXINL void     AfxAtanV3d(afxV3d v, afxV3d const in); // v = atan(in)
AFXINL void     AfxAtanV4d(afxV4d v, afxV4d const in); // v = atan(in)

AFXINL void     AfxLogV2d(afxV2d v, afxV2d const in);
AFXINL void     AfxLogV3d(afxV3d v, afxV3d const in);
AFXINL void     AfxLogV4d(afxV4d v, afxV4d const in);

AFXINL void     AfxLog2V2d(afxV2d v, afxV2d const in);
AFXINL void     AfxLog2V3d(afxV3d v, afxV3d const in);
AFXINL void     AfxLog2V4d(afxV4d v, afxV4d const in);

AFXINL void     AfxLog10V2d(afxV2d v, afxV2d const in);
AFXINL void     AfxLog10V3d(afxV3d v, afxV3d const in);
AFXINL void     AfxLog10V4d(afxV4d v, afxV4d const in);

AFXINL void     AfxPowV2d(afxV2d v, afxV2d const in);
AFXINL void     AfxPowV3d(afxV3d v, afxV3d const in);
AFXINL void     AfxPowV4d(afxV4d v, afxV4d const in);

AFXINL void     AfxExpV2d(afxV2d v, afxV2d const in);
AFXINL void     AfxExpV3d(afxV3d v, afxV3d const in);
AFXINL void     AfxExpV4d(afxV4d v, afxV4d const in);

AFXINL void     AfxExp2V2d(afxV2d v, afxV2d const in);
AFXINL void     AfxExp2V3d(afxV3d v, afxV3d const in);
AFXINL void     AfxExp2V4d(afxV4d v, afxV4d const in);

AFXINL void     AfxExp10V2d(afxV2d v, afxV2d const in);
AFXINL void     AfxExp10V3d(afxV3d v, afxV3d const in);
AFXINL void     AfxExp10V4d(afxV4d v, afxV4d const in);

AFXINL void     AfxNegV2d(afxV2d v, afxV2d const in);
AFXINL void     AfxNegV3d(afxV3d v, afxV3d const in);
AFXINL void     AfxNegV4d(afxV4d v, afxV4d const in);
AFXINL void     AfxNegAtv4d(afxV4d v, afxV4d const in); // v = inverse of in

AFXINL afxReal  AfxSumV2d(afxV2d const v);
AFXINL afxReal  AfxSumV3d(afxV3d const v);
AFXINL afxReal  AfxSumV4d(afxV4d const v);

// Computes the dot product (a dot b)
// The dot product only defines the angle between both vectors.
// You may remember that the cosine or cos function becomes 0 when the angle is 90 degrees or 1 when the angle is 0.
// This allows us dst easily test if the two vectors are orthogonal or parallel dst each other using the dot product (orthogonal means the vectors are at a right-angle dst each other).

AFXINL afxReal  AfxDotV2d(afxV2d const in, afxV2d const other);
AFXINL afxReal  AfxDotV3d(afxV3d const in, afxV3d const other);
AFXINL afxReal  AfxDotV4d(afxV4d const in, afxV4d const other);

// Computes the squared magnitude

AFXINL afxReal  AfxSqV2d(afxV2d const in);
AFXINL afxReal  AfxSqV3d(afxV3d const in);
AFXINL afxReal  AfxSqV4d(afxV4d const in);

// The magnitude/length of a vector in Cartesian coordinates is the square root of the sum of the squares of its coordinates.

AFXINL afxReal  AfxMagV2d(afxV2d const in);
AFXINL afxReal  AfxMagV3d(afxV3d const in);
AFXINL afxReal  AfxMagV4d(afxV4d const in);

AFXINL afxReal  AfxMagV2dRecip(afxV2d const in);
AFXINL afxReal  AfxMagV3dRecip(afxV3d const in);
AFXINL afxReal  AfxMagV4dRecip(afxV4d const in);

// Computes the cross product (a cross b) and stores the result in v
// The cross product is only defined in 3D space and takes two non-parallel vectors as input and produces a third vector that is orthogonal dst both the input vectors.
// If both the input vectors are orthogonal dst each other as well, a cross product would result in 3 orthogonal vectors; this will prove useful in the upcoming chapters.
// The following image shows what this looks like in 3D space:

AFXINL void     AfxCrossV2d(afxV2d v, afxV2d const a, afxV2d const b);
AFXINL void     AfxCrossV3d(afxV3d v, afxV3d const a, afxV3d const b);
AFXINL void     AfxCrossV4d(afxV4d v, afxV4d const a, afxV4d const b, afxV4d const c);

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

AFXINL void     AfxPostMultiplyV2d(afxM2d const m, afxV2d const in, afxV2d out); // m * v
AFXINL void     AfxPostMultiplyV3d(afxM3d const m, afxV3d const in, afxV3d out); // m * v
AFXINL void     AfxPostMultiplyV4d(afxM4d const m, afxV4d const in, afxV4d out); // m * v

AFXINL void     AfxPostMultiplyLtv3d(afxM4d const m, afxV3d const in, afxV3d out); // m * v
AFXINL void     AfxPostMultiplyLtv4d(afxM3d const m, afxV4d const in, afxV4d out); // m * v

AFXINL void     AfxPostMultiplyAtv2d(afxM4d const m, afxV2d const in, afxV2d out); // m * v
AFXINL void     AfxPostMultiplyAtv3d(afxM4d const m, afxV3d const in, afxV3d out); // m * v
AFXINL void     AfxPostMultiplyAtv4d(afxM4d const m, afxV4d const in, afxV4d out); // m * v

AFXINL void     TransposeVectorTransform4x3(afxV3d Dest, float D3, afxM4d const Transform);

AFXINL void     AfxPreMultiplyV2d(afxM2d const m, afxV2d const in, afxV2d out); // v * m
AFXINL void     AfxPreMultiplyV3d(afxM3d const m, afxV3d const in, afxV3d out); // v * m
AFXINL void     AfxPreMultiplyV4d(afxM4d const m, afxV4d const in, afxV4d out); // v * m

AFXINL void     AfxPreMultiplyLtv3d(afxM4d const m, afxV3d const in, afxV3d out); // v * m
AFXINL void     AfxPreMultiplyLtv4d(afxM3d const m, afxV4d const in, afxV4d out); // v * m

AFXINL void     AfxPreMultiplyAtv2d(afxM4d const m, afxV2d const in, afxV2d out); // v * m
AFXINL void     AfxPreMultiplyAtv3d(afxM4d const m, afxV3d const in, afxV3d out); // v * m
AFXINL void     AfxPreMultiplyAtv4d(afxM4d const m, afxV4d const in, afxV4d out); // v * m

// Similarity transform

AFXINL void     AfxAssimilateAtv3d(afxM3d const ltm, afxV4d const atv, afxNat cnt, afxV3d const in[], afxV3d out[]); // make similarity transformation on afxV3d-based position.
AFXINL void     AfxAssimilateAtv4d(afxM3d const ltm, afxV4d const atv, afxNat cnt, afxV4d const in[], afxV4d out[]); // make similarity transformation on afxV3d-based position.

#endif//AFX_VECTOR_H
