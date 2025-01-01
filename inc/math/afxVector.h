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

#include "qwadro/inc/math/afxScalar.h"

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

////////////////////////////////////////////////////////////////////////////////
// Clamping                                                                   //
////////////////////////////////////////////////////////////////////////////////

AFXINL void     AfxTruncateV2d(afxV2d v, afxV2d const in);
AFXINL void     AfxTruncateV3d(afxV3d v, afxV3d const in);
AFXINL void     AfxTruncateV4d(afxV4d v, afxV4d const in);

AFXINL void     AfxSaturateV2d(afxV2d v, afxV2d const in);
AFXINL void     AfxSaturateV3d(afxV3d v, afxV3d const in);
AFXINL void     AfxSaturateV4d(afxV4d v, afxV4d const in);

AFXINL afxReal  AfxV2dNormalize(afxV2d v, afxV2d const in);
AFXINL afxReal  AfxV3dNormalize(afxV3d v, afxV3d const in);
AFXINL afxReal  AfxV4dNormalize(afxV4d v, afxV4d const in);
AFXINL afxReal  AfxV4dNormalizeV3d(afxV4d v, afxV3d const in);
AFXINL afxReal  AfxV3dNormalizeV4d(afxV3d v, afxV4d const in);

AFXINL afxReal  AfxV2dNormalizeEstimated(afxV2d v, afxV2d const in);
AFXINL afxReal  AfxV3dNormalizeEstimated(afxV3d v, afxV3d const in);
AFXINL afxReal  AfxV4dNormalizeEstimated(afxV4d v, afxV4d const in);

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

// v = in * 0.5
AFXINL void     AfxHalfV2d(afxV2d v, afxV2d const in);
AFXINL void     AfxHalfV3d(afxV3d v, afxV3d const in);
AFXINL void     AfxHalfV4d(afxV4d v, afxV4d const in);

AFXINL void     AfxMinV2d(afxV2d v, afxV2d const a, afxV2d const b); // v = minor between A and B
AFXINL void     AfxMinV3d(afxV3d v, afxV3d const a, afxV3d const b); // v = minor between A and B
AFXINL void     AfxMinV4d(afxV4d v, afxV4d const a, afxV4d const b); // v = minor between A and B

AFXINL void     AfxMaxV2d(afxV2d v, afxV2d const a, afxV2d const b); // v = major between A and B
AFXINL void     AfxMaxV3d(afxV3d v, afxV3d const a, afxV3d const b); // v = major between A and B
AFXINL void     AfxMaxV4d(afxV4d v, afxV4d const a, afxV4d const b); // v = major between A and B

AFXINL void     AfxClampV2d(afxV2d v, afxV2d const in, afxV2d const min, afxV2d const max); // v = IN between bounds
AFXINL void     AfxClampV3d(afxV3d v, afxV3d const in, afxV3d const min, afxV3d const max);
AFXINL void     AfxClampV4d(afxV4d v, afxV4d const in, afxV4d const min, afxV4d const max);

AFXINL void     AfxNormalizeArrayedV2d(afxUnit cnt, afxV2d const in[], afxV2d out[]);
AFXINL void     AfxNormalizeArrayedV3d(afxUnit cnt, afxV3d const in[], afxV3d out[]);
AFXINL void     AfxNormalizeArrayedV4d(afxUnit cnt, afxV4d const in[], afxV4d out[]);

AFXINL void     AfxZeroOrNormalizeArrayedV2d(afxUnit cnt, afxV2d const in[], afxV2d out[]);
AFXINL void     AfxZeroOrNormalizeArrayedV3d(afxUnit cnt, afxV3d const in[], afxV3d out[]);
AFXINL void     AfxZeroOrNormalizeArrayedV4d(afxUnit cnt, afxV4d const in[], afxV4d out[]);

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

// v = a % b

AFXINL void     AfxModV2d(afxV2d v, afxV2d const a, afxV2d const b); // v = a % b
AFXINL void     AfxModV3d(afxV3d v, afxV3d const a, afxV3d const b); // v = a % b
AFXINL void     AfxModV4d(afxV4d v, afxV4d const a, afxV4d const b); // v = a % b

// v = a * lambda

AFXINL void     AfxV2dScale(afxV2d v, afxV2d const a, afxReal lambda); // v = a * lambda
AFXINL void     AfxV3dScale(afxV3d v, afxV3d const a, afxReal lambda); // v = a * lambda
AFXINL void     AfxV4dScale(afxV4d v, afxV4d const a, afxReal lambda); // v = a * lambda

// v = a * b

AFXINL void     AfxV2dMultiply(afxV2d v, afxV2d const a, afxV2d const b); // v = a * b
AFXINL void     AfxV3dMultiply(afxV3d v, afxV3d const a, afxV3d const b); // v = a * b
AFXINL void     AfxV4dMultiply(afxV4d v, afxV4d const a, afxV4d const b); // v = a * b

// vector-multiply-add (accumulate)
// v = a * b + c

AFXINL void     AfxV2dMad(afxV2d v, afxV2d const a, afxV2d const b, afxV2d const c); // v = a * b + c
AFXINL void     AfxV3dMad(afxV3d v, afxV3d const a, afxV3d const b, afxV3d const c); // v = a * b + c
AFXINL void     AfxV4dMad(afxV4d v, afxV4d const a, afxV4d const b, afxV4d const c); // v = a * b + c

// scalar-multiply-add (accumulate)
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
// It is often used in shader programming and graphics.
// v = x * (1 - t) + y * t

AFXINL void     AfxV2dMix(afxV2d v, afxV2d const x, afxV2d const y, afxReal t);
AFXINL void     AfxV3dMix(afxV3d v, afxV3d const x, afxV3d const y, afxReal t);
AFXINL void     AfxV4dMix(afxV4d v, afxV4d const x, afxV4d const y, afxReal t);

// Lerp
// LERP is a method to interpolate linearly between two values. 
// In the context of 4D vectors, it calculates the intermediate vector between a start and end vector based on a factor t ranging from 0.0 to 1.0.
// x + t * (y - x)

AFXINL void     AfxV2dLerp(afxV2d v, afxV2d const x, afxV2d const y, afxReal t);
AFXINL void     AfxV3dLerp(afxV3d v, afxV3d const x, afxV3d const y, afxReal t);
AFXINL void     AfxV4dLerp(afxV4d v, afxV4d const x, afxV4d const y, afxReal t);

// SLERP
// Function to perform Spherical Linear Interpolation (SLERP) between two quaternions
// v(t) = ((sin((1 - t) * theta)) / sin(theta)) * x + ((sin(t) * theta) / (sin(theta))) * y

AFXINL void     AfxV2dSlerp(afxV2d v, afxV2d x, afxV2d y, afxReal t);
AFXINL void     AfxV3dSlerp(afxV3d v, afxV3d x, afxV3d y, afxReal t);
AFXINL void     AfxV4dSlerp(afxV4d v, afxV4d x, afxV4d y, afxReal t);

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

AFXINL void     AfxV2dLog2(afxV2d v, afxV2d const in);
AFXINL void     AfxV3dLog2(afxV3d v, afxV3d const in);
AFXINL void     AfxV4dLog2(afxV4d v, afxV4d const in);

AFXINL void     AfxV2dLog10(afxV2d v, afxV2d const in);
AFXINL void     AfxV3dLog10(afxV3d v, afxV3d const in);
AFXINL void     AfxV4dLog10(afxV4d v, afxV4d const in);

AFXINL void     AfxV2dPow(afxV2d v, afxV2d const in);
AFXINL void     AfxV3dPow(afxV3d v, afxV3d const in);
AFXINL void     AfxV4dPow(afxV4d v, afxV4d const in);

AFXINL void     AfxV2dExp(afxV2d v, afxV2d const in);
AFXINL void     AfxV3dExp(afxV3d v, afxV3d const in);
AFXINL void     AfxV4dExp(afxV4d v, afxV4d const in);

AFXINL void     AfxV2dExp2(afxV2d v, afxV2d const in);
AFXINL void     AfxV3dExp2(afxV3d v, afxV3d const in);
AFXINL void     AfxV4dExp2(afxV4d v, afxV4d const in);

AFXINL void     AfxV2dExp10(afxV2d v, afxV2d const in);
AFXINL void     AfxV3dExp10(afxV3d v, afxV3d const in);
AFXINL void     AfxV4dExp10(afxV4d v, afxV4d const in);

AFXINL void     AfxV2dNeg(afxV2d v, afxV2d const in);
AFXINL void     AfxV3dNeg(afxV3d v, afxV3d const in);
AFXINL void     AfxV4dNeg(afxV4d v, afxV4d const in);
AFXINL void     AfxV4dNegAffine(afxV4d v, afxV4d const in); // v = inverse of in

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

AFXINL afxReal  AfxV2dDist(afxV2d const v, afxV2d const other);
AFXINL afxReal  AfxV3dDist(afxV3d const v, afxV3d const other);
AFXINL afxReal  AfxV4dDist(afxV4d const v, afxV4d const other);

// Computes the squared magnitude

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
