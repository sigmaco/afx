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
 *                               (c) 2017 Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_MATH_DEFS_H
#define AFX_MATH_DEFS_H

#include "afx/core/diag/afxDebug.h"
#include "afx/core/afxSimd.h"
#include <math.h>
//#include <stdalign.h>

//#define MFX_USE_RWMATH

// AFX always targets a another object rather than 'self' to avoid using intermediate objects inside functions. It optimizes code when operations on a object type are grouped continously.
// That means "AfxNormalizeObject(self, to)" always should export to "to" instead of doing action in place.

// The "Right vector" is one of the vectors in a RenderWare Graphics matrix. It corresponds to the 'x' axis in a right-handed, xyz coordinate system. 
// The "Up vector" is one of the vectors in a RenderWare Graphics matrix. The up vector corresponds to the 'y' axis in a right-handed, xyz coordinate system. 
// The "at vector" is one of the vectors in a RenderWare Graphics matrix. It corresponds to the 'z' axis in a right-handed, xyz coordinate system. 
// RenderWare Graphics uses a right-handed co-ordinate system. Therefore with z increasing into the display screen, and y increasing upwards, the x co-ordinate system increases towards the left.

// This object defines a RenderWare Matrix object.
// The Matrix is heavily used throughout the API and a full range of functions are provided, including: rotation, multiplication, concatenation, scaling, translation, creation, destruction, stream read / write functions and a number of access functions to access particular vector data within the matrix.
// RenderWare uses 4x3, row - major affine matrices.

// Qwadro uses radians as default angle measurement unit. They are natural and faster.

// The linear algebra library produces OpenGL-oriented calculus. If you use Direct3D, kill yourself.

//#define MFX_USE_COLUMN_MAJOR_MATRIX
//#define MFX_USE_LEFT_HANDED_MATRIX
#define MFX_USE_EXPERIMENTAL
#define MFX_USE_RWMATH

#define AfxScalar(x_) ((afxReal)x_)

#define AFX_PI          AfxScalar(3.14159265358979323846)
#define AFX_PI_OVER2    (AFX_PI / AfxScalar(2))
#define AFX_EPSILON     AfxScalar(FLT_EPSILON)

typedef afxReal         afxSimd(afxV2d[2]);
typedef afxReal         afxSimd(afxV3d[3]);
typedef afxReal         afxSimd(afxV4d[4]);
static_assert(sizeof(__m128) == sizeof(afxV4d), "");

typedef afxV4d          afxSimd(afxQuat); // 0,1,2 = imaginary, 3 = real

typedef afxV2d          afxSimd(afxM2d[2]);
typedef afxV3d          afxSimd(afxM3d[3]);
typedef afxV4d          afxSimd(afxM4d[4]);
typedef afxV3d          afxSimd(afxM43[4]);

#if 0
static_assert(__alignof(afxV2d) == AFX_SIMD_ALIGN, "");
static_assert(__alignof(afxV3d) == AFX_SIMD_ALIGN, "");
static_assert(__alignof(afxV4d) == AFX_SIMD_ALIGN, "");
static_assert(__alignof(afxQuat) == AFX_SIMD_ALIGN, "");
static_assert(__alignof(afxM3d) == AFX_SIMD_ALIGN, "");
static_assert(__alignof(afxM4d) == AFX_SIMD_ALIGN, "");
#endif

AFX_DEFINE_STRUCT(afxRect)
{
    union { afxInt offset[2]; struct { afxInt x, y; }; };
    union { afxNat extent[2]; struct { afxNat w, h; }; };
};

AFXINL void         AfxRectZero(afxRect *rect) { rect->x = 0; rect->y = 0; rect->w = 1; rect->h = 1; }
AFXINL void         AfxRectCopy(afxRect *rect, afxRect const *src) { rect->x = src->x; rect->y = src->y; rect->w = src->w; rect->h = src->h; }

AFXINL afxReal      AfxRealMin(afxReal a, afxReal b) { return (a < b) ? a : b; }
AFXINL afxReal      AfxRealMax(afxReal a, afxReal b) { return (a > b) ? a : b; }

AFXINL afxBool      AfxRealIsAlmostEqual(afxReal a, afxReal b) { return ((AfxAbs(a - b)) <= (AFX_EPSILON * AfxRealMax(AfxScalar(1), AfxRealMax(a, b)))); }

AFXINL afxReal      AfxToNdc(afxReal x, afxReal total) { return x * (AfxScalar(1) / total); }
AFXINL afxReal      AfxFromNdc(afxReal x, afxReal total) { afxError err = AFX_ERR_NONE; AfxAssert(AfxScalar(0) <= x); AfxAssert(AfxScalar(1) >= x); return x * total; }


AFXINL void AfxExtractV2dInNdc(afxV2d v, afxV2d const b, afxV2d const total)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(b);
    AfxAssert(total);
    v[0] = AfxToNdc(b[0], total[0]);
    v[1] = AfxToNdc(b[1], total[1]);
}

AFXINL void AfxExtractV2dOutNdc(afxV2d v, afxV2d const b, afxV2d const total)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(b);
    AfxAssert(total);
    v[0] = AfxFromNdc(b[0], total[0]);
    v[1] = AfxFromNdc(b[1], total[1]);
}


AFXINL void AfxMakeV2dNdc(afxV2d v, afxV2d const total)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(total);
    AfxAssert(v != total);
    v[0] = AfxToNdc(v[0], total[0]);
    v[1] = AfxToNdc(v[1], total[1]);
}

AFXINL void AfxUnmakeV2dNdc(afxV2d v, afxV2d const total)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(total);
    AfxAssert(v != total);
    v[0] = AfxFromNdc(v[0], total[0]);
    v[1] = AfxFromNdc(v[1], total[1]);
}

AFXINL void AfxCorrectRwMatrix(afxM4d m, afxM4d const in);

#include "afx/core/mem/afxMemory.h"

//#pragma warning (push)
//#pragma warning (disable : 4090)

// afxM4d is a row-major right-handled matrix. Qwadro inherits matrix from RenderWare.
// RenderWare, such as DirectX, uses row vectors and ends up with the much more natural: result = input * local_to_object * object_to_world. Your input is in local space, it gets transformed into object space before finally ending up in world space. Clean, clear, and readable. If you instead multiply the two matrices together on their own, you get the clear local_to_world = local_to_object * object_to_world instead of the awkward local_to_world = object_to_world * local_to_object you would get with OpenGL and column vectors.

// Rotation matrices are neither left-handed nor right-handed. The are universal for any handedness. – Nico Schertler

/*
Normalized Device Coordinates
Normalized Device Coordinates (NDC) is the coordinate system that vertices are in once vertex processing is finished.
You could consider this the "native" 3D coordinate system of OpenGL.
It has a range of [-1.0, 1.0] for all 3 coordinates, and is left-handed.

Fixed Pipeline
In the legacy fixed pipeline, vertices are initially specified in object coordinates (see http://www.glprogramming.com/red/chapter03.html).
The modelview matrix transforms them to eye coordinates, which the projection matrix transforms to clip coordinates. After perspective division, they become NDC.
It is most common to use a right-handed coordinate system for the object coordinates.
A modelview matrix that preserves handedness is used, which results in eye coordinates that are still right-handed.
The projection matrix flips the direction of the z-axis, resulting in left-handed clip coordinates and NDC.
If you choose so, you can easily use left-handed object coordinates, and use a modelview matrix that flips handedness, so that you end up with right-handed eye coordinates again.

Programmable Pipeline
With the programmable pipeline, you're at complete liberty to use any coordinate systems you want in your vertex shader. 
Say if your original vertices are specified in spherical coordinates, that's no problem at all. 
You just need to process them accordingly in the shader code. 
The vertex shader needs to produce clip coordinates (which as previously explained are left-handed) as output, but everything before that is completely open.


*/


// afxQuat: A 4-dimensional vector representing a rotation.
// The vector represents a 4 dimensional complex number where multiplication of the basis elements is not commutative(multiplying i with j gives a different result than multiplying j with i).
// Multiplying quaternions reproduces rotation sequences.However quaternions need dst be often renormalized, or else they suffer from precision issues.
// It canv be used dst perform SLERP(spherical - linear interpolation) between two rotations.


// Qwadro performs all bone animation on decomposed transforms, where "decomposed" means that the position, orientation, and scale/shear components have been pulled out into separately animating quantities. 
// This allows for fair, efficient animation and interpolation, whereas keeping everything as a tangled 4x4 matrix does not. 

// The afxTransform is the primary structure used to store these decomposed transforms. Each structure stores a 4-element position vector, a 4-element quaternion orientation, and a 3x3 scale/shear matrix (note that it may also contain mirroring). 
// Additionally, each transform stores a set of afxTransformFlags that indicates which, if any, of these values current has a non-identity value. These flags are solely used to speed up computation.

typedef enum afxTransformFlags
{
    AFX_TRANSFORM_FLAG_ORIGIN       = AFX_FLAG(0), // has non-identity position
    AFX_TRANSFORM_FLAG_ORIENTATION  = AFX_FLAG(1), // has non-identity orientation
    AFX_TRANSFORM_FLAG_SCALESHEAR   = AFX_FLAG(2), // has non-identity scale/shear
    AFX_TRANSFORM_FLAG_ALL          = (AFX_TRANSFORM_FLAG_ORIGIN | AFX_TRANSFORM_FLAG_ORIENTATION | AFX_TRANSFORM_FLAG_SCALESHEAR)
} afxTransformFlags;

AFX_DEFINE_STRUCT(afxTransform)
{
    afxTransformFlags   flags;
    afxV4d              origin;
    afxQuat             orientation;
    afxM3d              scaleShear;
};

AFX afxV2d  const AFX_V2D_X;
AFX afxV3d  const AFX_V3D_X;
AFX afxV4d  const AFX_V4D_X;
AFX afxV2d  const AFX_V2D_XI;
AFX afxV3d  const AFX_V3D_XI;
AFX afxV4d  const AFX_V4D_XI;

AFX afxV2d  const AFX_V2D_Y;
AFX afxV3d  const AFX_V3D_Y;
AFX afxV4d  const AFX_V4D_Y;
AFX afxV2d  const AFX_V2D_YI;
AFX afxV3d  const AFX_V3D_YI;
AFX afxV4d  const AFX_V4D_YI;

AFX afxV3d  const AFX_V3D_Z;
AFX afxV4d  const AFX_V4D_Z;
AFX afxV3d  const AFX_V3D_ZI;
AFX afxV4d  const AFX_V4D_ZI;

AFX afxV4d  const AFX_V4D_W;
AFX afxV4d  const AFX_V4D_WI;

AFX afxV2d  const AFX_V2D_ZERO;
AFX afxV3d  const AFX_V3D_ZERO;
AFX afxV4d  const AFX_V4D_ZERO;
AFX afxQuat const AFX_QUAT_ZERO;
AFX afxM2d  const AFX_M2D_ZERO;
AFX afxM3d  const AFX_M3D_ZERO;
AFX afxM4d  const AFX_M4D_ZERO;

AFX afxV2d  const AFX_V2D_ONE;
AFX afxV3d  const AFX_V3D_ONE;
AFX afxV4d  const AFX_V4D_ONE;

AFX afxV4d  const AFX_V4D_IDENTITY;
AFX afxQuat const AFX_QUAT_IDENTITY;
AFX afxM2d  const AFX_M2D_IDENTITY;
AFX afxM3d  const AFX_M3D_IDENTITY;
AFX afxM4d  const AFX_M4D_IDENTITY;

#define AfxSpawnV2d(x_, y_) (afxV2d){ (afxReal)(x_), (afxReal)(y_) }
#define AfxSpawnV3d(x_, y_, z_) (afxV3d){ (afxReal)x_, (afxReal)y_, (afxReal)z_ }
#define AfxSpawnV4d(x_, y_, z_, w_) (afxV4d const){ (afxReal)x_, (afxReal)y_, (afxReal)z_, (afxReal)w_ }
#define AfxSpawnPoint(x_, y_, z_) (afxV4d const){ (afxReal)x_, (afxReal)y_, (afxReal)z_, (afxReal)1 }

#define AfxSpawnConstV2d(x_, y_) (afxV2d const){ (afxReal)(x_), (afxReal)(y_) }
#define AfxSpawnConstV3d(x_, y_, z_) (afxV3d const){ (afxReal)x_, (afxReal)y_, (afxReal)z_ }
#define AfxSpawnV4dConst(x_, y_, z_, w_) (afxV4d const){ (afxReal)x_, (afxReal)y_, (afxReal)z_, (afxReal)w_ }
#define AfxSpawnPointConst(x_, y_, z_) (afxV4d const){ (afxReal)x_, (afxReal)y_, (afxReal)z_, (afxReal)1 }

AFXINL void             AfxM3dMakeAxialRotation(afxM3d m, afxV3d const axis, afxReal radians);
AFXINL void             AfxM4dMakeAxialRotation(afxM4d m, afxV3d const axis, afxReal radians);

AFXINL void             AfxM3dAccumulateAxialRotation(afxM3d m, afxV3d const axis, afxReal radians); // builds a rotation matrix from the given axis and angle of rotation and applies it to the specified matrix.
AFXINL void             AfxM4dAccumulateAxialRotation(afxM4d m, afxV3d const axis, afxReal radians); // builds a rotation matrix from the given axis and angle of rotation and applies it to the specified matrix.

AFXINL void         AfxQuatMakeAxialRotation(afxQuat q, afxV3d const axis, afxReal radians); // returns a quaternion that will rotate around the given axis by the specified angle. The axis must be a normalized vector.

////////////////////////////////////////////////////////////////////////////////
// SIMILARITY TRANSFORMATION                                                  //
////////////////////////////////////////////////////////////////////////////////

AFXINL void             AfxV3dAssimilate(afxV3d const v, afxV3d const affine, afxM3d const linear, afxV3d out); // make similarity transformation on afxV3d-based position.
AFXINL void             AfxQuatAssimilate(afxQuat const q, afxM3d const linear, afxM3d const invLinear, afxQuat out); // make similarity transformation on afxQuat-based orientation.
AFXINL void             AfxM3dAssimilate(afxM3d const m, afxM3d const linear, afxM3d const invLinear, afxM3d out); // make similarity transformation on afxM3d-based scale/shear.
AFXINL void             AfxM4dAssimilate4x3(afxM4d const m, afxV3d const affine, afxM3d const linear, afxM3d const invLinear, afxM4d out);
AFXINL void             AfxTransformAssimilate(afxTransform const *t, afxV3d const affine, afxM3d const linear, afxM3d const invLinear, afxTransform *out);








//////////////////////// QWADRO NAME CONVENTIONS ///////////////////////////////

// LINEAR TRANSFORMATIONS AFFECTS ONLY 3X3 SUBSET OF MATRICES.
// AFFINE TRANSFORMATIONS AFFECTS ONLY 4X3 SUBSET OF MATRICES. 



////////////////////////////////////////////////////////////////////////////////
// TRANSFERENCE OPERATIONS                                                    //
////////////////////////////////////////////////////////////////////////////////

// Copy

AFXINL void     AfxV2dCopy(afxV2d v, afxV2d const in);
AFXINL void     AfxV3dCopy(afxV3d v, afxV3d const in);
AFXINL void     AfxV4dCopy(afxV4d v, afxV4d const in);
AFXINL void     AfxQuatCopy(afxQuat q, afxQuat const in);
AFXINL void     AfxM2dCopy(afxM2d m, afxM2d const in);
AFXINL void     AfxM3dCopy(afxM3d m, afxM3d const in);
AFXINL void     AfxM4dCopy(afxM4d m, afxM4d const in);

// Make (Combined Copy & Promote/Demote operations)

AFXINL void     AfxV3dMakeV2d(afxV3d const v, afxV2d out);
AFXINL void     AfxV4dMakeV2d(afxV4d const v, afxV2d out);
AFXINL void     AfxV2dMakeV3d(afxV2d const v, afxV3d out);
AFXINL void     AfxV4dMakeV3d(afxV4d const v, afxV3d out);
AFXINL void     AfxV2dMakeV4d(afxV2d const v, afxV4d out);
AFXINL void     AfxV3dMakeV4d(afxV3d const v, afxV4d out);
AFXINL void     AfxV2dMakePoint(afxV2d const v, afxV4d out);
AFXINL void     AfxV3dMakePoint(afxV3d const v, afxV4d out);
AFXINL void     AfxM3dMakeM2d(afxM3d const m, afxM2d out);
AFXINL void     AfxM4dMakeM2d(afxM4d const m, afxM2d out);
AFXINL void     AfxM2dMakeM3d(afxM2d const m, afxM3d out);
AFXINL void     AfxM4dMakeM3d(afxM4d const m, afxM3d out);
AFXINL void     AfxM2dMakeM4d(afxM2d const m, afxM4d out);
AFXINL void     AfxM3dMakeM4d(afxM3d const m, afxM4d out);

AFXINL void     AfxV2dMakeScalingM3d(afxV2d const v, afxM3d out); // aka get matrix scale
AFXINL void     AfxV3dMakeScalingM3d(afxV3d const v, afxM3d out);
AFXINL void     AfxV4dMakeScalingM3d(afxV4d const v, afxM3d out);
AFXINL void     AfxV2dMakeScalingM4d(afxV2d const v, afxM4d out);
AFXINL void     AfxV3dMakeScalingM4d(afxV3d const v, afxM4d out);
AFXINL void     AfxV4dMakeScalingM4d(afxV4d const v, afxM4d out);

AFXINL void     AfxM3dMakeRotationQuat(afxM3d const m, afxQuat out); // aka get matrix rotation
AFXINL void     AfxM4dMakeRotationQuat(afxM4d const m, afxQuat out);
AFXINL void     AfxQuatMakeRotationM3d(afxQuat const q, afxM3d out);
AFXINL void     AfxQuatMakeRotationM4d(afxQuat const q, afxM4d out);
AFXINL void     AfxQuatMakeRotationAxisV3d(afxQuat const q, afxV3d out, afxReal *angle);
AFXINL void     AfxQuatMakeRotationAxisV4d(afxQuat const q, afxV4d out, afxReal *angle);


AFXINL void     AfxM4dMakeTranslationV2d(afxM4d const m, afxV2d out); // aka get matrix position
AFXINL void     AfxM4dMakeTranslationV3d(afxM4d const m, afxV3d out);
AFXINL void     AfxM4dMakeTranslationV4d(afxM4d const m, afxV4d out);
AFXINL void     AfxV2dMakeTranslationM4d(afxV2d const v, afxM4d out);
AFXINL void     AfxV3dMakeTranslationM4d(afxV3d const v, afxM4d out);
AFXINL void     AfxV4dMakeTranslationM4d(afxV4d const v, afxM4d out);

AFXINL void     AfxM4dCompose(afxM4d m, afxV4d const scalOrigin, afxQuat const scalOrient, afxV3d const scaling, afxV4d const rotOrigin, afxQuat const rotQuat, afxV4d const translation);
AFXINL void     AfxM4dComposeAffine(afxM4d m, afxV3d const scale, afxV3d const rotAxis, afxQuat const rot, afxV4d const translation);

// Swap

AFXINL void     AfxV2dSwap(afxV2d v, afxV2d other);
AFXINL void     AfxV3dSwap(afxV3d v, afxV3d other);
AFXINL void     AfxV4dSwap(afxV4d v, afxV4d other);
AFXINL void     AfxQuatSwap(afxQuat q, afxQuat other);
AFXINL void     AfxM2dSwap(afxM2d m, afxM2d other);
AFXINL void     AfxM3dSwap(afxM3d m, afxM3d other);
AFXINL void     AfxM4dSwap(afxM4d m, afxM4d other);

// Transpose

AFXINL void     AfxM2dTranspose(afxM2d const m, afxM2d out);
AFXINL void     AfxM3dTranspose(afxM3d const m, afxM3d out);
AFXINL void     AfxM4dTranspose(afxM4d const m, afxM4d out);

AFXINL void     AfxM4dTransposeM3d(afxM4d const m, afxM3d out);
AFXINL void     AfxM3dTransposeM4d(afxM3d const m, afxM4d out);
AFXINL void     AfxM4dTransposeLinear(afxM4d const m, afxM4d out); // only consider 3x3
AFXINL void     AfxM4dTransposeAffine(afxM4d const m, afxM4d out); // only consider 4x3; don't consider W components

////////////////////////////////////////////////////////////////////////////////
// TRANSFORMATION OPERATIONS                                                  //
////////////////////////////////////////////////////////////////////////////////

// IsIdentity

AFXINL afxBool  AfxV4dIsIdentity(afxV4d const v);
AFXINL afxBool  AfxQuatIsIdentity(afxQuat const q);
AFXINL afxBool  AfxM2dIsIdentity(afxM2d const m);
AFXINL afxBool  AfxM3dIsIdentity(afxM3d const m);
AFXINL afxBool  AfxM4dIsIdentity(afxM4d const m);

// MakeIdentity

AFXINL void     AfxV4dMakeIdentity(afxV4d v);
AFXINL void     AfxQuatMakeIdentity(afxQuat q);
AFXINL void     AfxM2dMakeIdentity(afxM2d m);
AFXINL void     AfxM3dMakeIdentity(afxM3d m);
AFXINL void     AfxM4dMakeIdentity(afxM4d m);

// Negate

AFXINL void     AfxV2dNegate(afxV2d const v, afxV2d out);
AFXINL void     AfxV3dNegate(afxV3d const v, afxV3d out);
AFXINL void     AfxV4dNegate(afxV4d const v, afxV4d out);
AFXINL void     AfxQuatNegate(afxQuat const q, afxQuat out);

AFXINL void     AfxV4dNegatePoint(afxV4d const v, afxV4d out);
AFXINL void     AfxQuatConjugate(afxQuat const q, afxQuat out);

// Invert

AFXINL void     AfxQuatInvert(afxQuat const q, afxQuat out);
AFXINL afxReal  AfxM3dInvert(afxM3d const m, afxM3d out);
AFXINL afxReal  AfxM4dInvert(afxM4d const m, afxM4d out);
AFXINL afxReal  AfxM4dInvert4x3(afxM4d const m, afxM4d out);

// Multiply

AFXINL void     AfxV2dMultiply(afxV2d const v, afxV2d const other, afxV2d out);
AFXINL void     AfxV3dMultiply(afxV3d const v, afxV3d const other, afxV3d out);
AFXINL void     AfxV4dMultiply(afxV4d const v, afxV4d const other, afxV4d out);
AFXINL void     AfxQuatMultiply(afxQuat const q, afxQuat const other, afxQuat out);
AFXINL void     AfxM2dMultiply(afxM2d const m, afxM2d const other, afxM2d out);
AFXINL void     AfxM3dMultiply(afxM3d const m, afxM3d const other, afxM3d out);
AFXINL void     AfxM4dMultiply(afxM4d const m, afxM4d const other, afxM4d out);
AFXINL void     AfxM4dMultiply4x3(afxM4d const m, afxM4d const other, afxM4d out);
AFXINL void     AfxV2dMultiplyM2d(afxV2d const v, afxM2d const m, afxV2d out);
AFXINL void     AfxV3dMultiplyM3d(afxV3d const v, afxM3d const m, afxV3d out);
AFXINL void     AfxV2dMultiplyM4d(afxV2d const v, afxM4d const m, afxV2d out);
AFXINL void     AfxV3dMultiplyM4d(afxV3d const v, afxM4d const m, afxV3d out);
AFXINL void     AfxV4dMultiplyM4d(afxV4d const v, afxM4d const m, afxV4d out);

AFXINL void     AfxM2dMultiplyTransposed(afxM2d const m, afxM2d const other, afxM2d out);
AFXINL void     AfxM3dMultiplyTransposed(afxM3d const m, afxM3d const other, afxM3d out);
AFXINL void     AfxM4dMultiplyTransposed(afxM4d const m, afxM4d const other, afxM4d out);

AFXINL void     AfxV3dMultiplyTransposedM3d(afxV3d const v, afxM3d const m, afxV3d out);
AFXINL void     AfxV3dMultiplyTransposedM4d(afxV3d const v, afxM4d const m, afxV3d out);
AFXINL void     AfxV4dMultiplyTransposedM4d(afxV4d const v, afxM4d const m, afxV4d out);

AFXINL void     AfxV3dRotateQuat(afxV3d const v, afxQuat const rot, afxV3d out);

// MultiplyArray

AFXINL void     AfxM2dMultiplyArrayV2d(afxM2d const m, afxNat cnt, afxV2d const v[], afxV2d out[]);
AFXINL void     AfxM3dMultiplyArrayV3d(afxM3d const m, afxNat cnt, afxV3d const v[], afxV3d out[]);
AFXINL void     AfxM4dMultiplyArrayV3d(afxM4d const m, afxNat cnt, afxV3d const v[], afxV3d out[]);
AFXINL void     AfxM4dMultiplyArrayV4d(afxM4d const m, afxNat cnt, afxV4d const v[], afxV4d out[]);

// Scale

AFXINL void     AfxM3dScaleV3d(afxM3d m, afxV3d const scale);
AFXINL void     AfxM4dScaleV3d(afxM4d m, afxV3d const scale);
AFXINL void     AfxM4dScaleV4d(afxM4d m, afxV4d const scale);

// Translate

AFXINL void     AfxM4dTranslateV3d(afxM4d m, afxV3d const translation);
AFXINL void     AfxM4dTranslateV4d(afxM4d m, afxV4d const translation);

// Rotation

AFXINL void     AfxM4dSetEulerRotation(afxM4d m, afxV3d const pitchYawRoll);
AFXINL void     AfxQuatSetEulerRotation(afxQuat q, afxV3d const pitchYawRoll);

////////////////////////////////////////////////////////////////////////////////
// MEASUREMENT OPERATIONS                                                     //
////////////////////////////////////////////////////////////////////////////////

// Dot

AFXINL afxReal  AfxV2dDot(afxV2d const v, afxV2d const other);
AFXINL afxReal  AfxV3dDot(afxV3d const v, afxV3d const other);
AFXINL afxReal  AfxV4dDot(afxV4d const v, afxV4d const other);
AFXINL afxReal  AfxQuatDot(afxQuat const q, afxQuat const other);

// Cross

AFXINL void     AfxV3dCross(afxV3d const v, afxV3d const other, afxV3d out);

// Det

AFXINL afxReal  AfxM2dDet(afxM2d const m);
AFXINL afxReal  AfxM3dDet(afxM3d const m);
AFXINL afxReal  AfxM4dDet(afxM4d const m);

// Norm 

AFXINL afxReal  AfxV2dNorm(afxV2d const v);
AFXINL afxReal  AfxV3dNorm(afxV3d const v);
AFXINL afxReal  AfxV4dNorm(afxV4d const v);
AFXINL afxReal  AfxQuatNorm(afxQuat const q);

// Magnitude

AFXINL afxReal  AfxV2dMagnitude(afxV2d const v);
AFXINL afxReal  AfxV3dMagnitude(afxV3d const v);
AFXINL afxReal  AfxV4dMagnitude(afxV4d const v);
AFXINL afxReal  AfxQuatMagnitude(afxQuat const q);

AFXINL afxReal  AfxV2dMagnitudeRecip(afxV2d const v);
AFXINL afxReal  AfxV3dMagnitudeRecip(afxV3d const v);
AFXINL afxReal  AfxV4dMagnitudeRecip(afxV4d const v);
AFXINL afxReal  AfxQuatMagnitudeRecip(afxQuat const q);

// Normalize

AFXINL afxReal  AfxV2dNormalize(afxV2d const v, afxV2d out);
AFXINL afxReal  AfxV3dNormalize(afxV3d const v, afxV3d out);
AFXINL afxReal  AfxV4dNormalize(afxV4d const v, afxV4d out);
AFXINL afxReal  AfxQuatNormalize(afxQuat const q, afxQuat out);

AFXINL afxReal  AfxV4dNormalizeV3d(afxV4d const v, afxV3d out);
AFXINL afxReal  AfxV3dNormalizeV4d(afxV3d const v, afxV4d out);

AFXINL afxReal  AfxV2dNormalizeEst(afxV2d const v, afxV2d out);
AFXINL afxReal  AfxV3dNormalizeEst(afxV3d const v, afxV3d out);
AFXINL afxReal  AfxV4dNormalizeEst(afxV4d const v, afxV4d out);
AFXINL afxReal  AfxQuatNormalizeEst(afxQuat const q, afxQuat out);

////////////////////////////////////////////////////////////////////////////////
// GENERIC PER-COMPONENT OPERATIONS                                           //
////////////////////////////////////////////////////////////////////////////////

// IsEqual

AFXINL afxBool  AfxV2dIsEqual(afxV2d const v, afxV2d const other);
AFXINL afxBool  AfxV3dIsEqual(afxV3d const v, afxV3d const other);
AFXINL afxBool  AfxV4dIsEqual(afxV4d const v, afxV4d const other);
AFXINL afxBool  AfxQuatIsEqual(afxQuat const q, afxQuat const other);

// Set

AFXINL void     AfxV2dSet(afxV2d v, afxReal x, afxReal y);
AFXINL void     AfxV3dSet(afxV3d v, afxReal x, afxReal y, afxReal z);
AFXINL void     AfxV4dSet(afxV4d v, afxReal x, afxReal y, afxReal z, afxReal w);
AFXINL void     AfxQuatSet(afxQuat q, afxReal x, afxReal y, afxReal z, afxReal w);

AFXINL void     AfxV2dSetScalar(afxV2d v, afxReal value);
AFXINL void     AfxV3dSetScalar(afxV3d v, afxReal value);
AFXINL void     AfxV4dSetScalar(afxV4d v, afxReal value);

// IsZero

AFXINL afxBool  AfxV2dIsZero(afxV2d const v);
AFXINL afxBool  AfxV3dIsZero(afxV3d const v);
AFXINL afxBool  AfxV4dIsZero(afxV4d const v);

// Zero

AFXINL void     AfxV2dZero(afxV2d v);
AFXINL void     AfxV3dZero(afxV3d v);
AFXINL void     AfxV4dZero(afxV4d v);
AFXINL void     AfxQuatZero(afxQuat q);
AFXINL void     AfxM2dZero(afxM2d m);
AFXINL void     AfxM3dZero(afxM3d m);
AFXINL void     AfxM4dZero(afxM4d m);

// Clamp

AFXINL void     AfxV2dMin(afxV2d const v, afxV2d const other, afxV2d out);
AFXINL void     AfxV3dMin(afxV3d const v, afxV3d const other, afxV3d out);
AFXINL void     AfxV4dMin(afxV4d const v, afxV4d const other, afxV4d out);

AFXINL void     AfxV2dMax(afxV2d const v, afxV2d const other, afxV2d out);
AFXINL void     AfxV3dMax(afxV3d const v, afxV3d const other, afxV3d out);
AFXINL void     AfxV4dMax(afxV4d const v, afxV4d const other, afxV4d out);

AFXINL void     AfxV2dCeil(afxV2d const v, afxV2d out);
AFXINL void     AfxV3dCeil(afxV3d const v, afxV3d out);
AFXINL void     AfxV4dCeil(afxV4d const v, afxV4d out);

AFXINL void     AfxV2dFloor(afxV2d const v, afxV2d out);
AFXINL void     AfxV3dFloor(afxV3d const v, afxV3d out);
AFXINL void     AfxV4dFloor(afxV4d const v, afxV4d out);

AFXINL void     AfxV2dRecip(afxV2d const v, afxV2d out);
AFXINL void     AfxV3dRecip(afxV3d const v, afxV3d out);
AFXINL void     AfxV4dRecip(afxV4d const v, afxV4d out);

// Addition/subtraction

AFXINL void     AfxV2dAdd(afxV2d const v, afxV2d const other, afxV2d out);
AFXINL void     AfxV3dAdd(afxV3d const v, afxV3d const other, afxV3d out);
AFXINL void     AfxV4dAdd(afxV4d const v, afxV4d const other, afxV4d out);
AFXINL void     AfxQuatAdd(afxQuat const q, afxQuat const other, afxQuat out);

AFXINL void     AfxV2dSub(afxV2d const v, afxV2d const other, afxV2d out);
AFXINL void     AfxV3dSub(afxV3d const v, afxV3d const other, afxV3d out);
AFXINL void     AfxV4dSub(afxV4d const v, afxV4d const other, afxV4d out);
AFXINL void     AfxQuatSub(afxQuat const q, afxQuat const other, afxQuat out);

// Div

AFXINL void     AfxV2dDiv(afxV2d const v, afxV2d const other, afxV2d out);
AFXINL void     AfxV3dDiv(afxV3d const v, afxV3d const other, afxV3d out);
AFXINL void     AfxV4dDiv(afxV4d const v, afxV4d const other, afxV4d out);

// Scale

AFXINL void     AfxV2dScale(afxV2d const v, afxReal scale, afxV2d out);
AFXINL void     AfxV3dScale(afxV3d const v, afxReal scale, afxV3d out);
AFXINL void     AfxV4dScale(afxV4d const v, afxReal scale, afxV4d out);
AFXINL void     AfxQuatScale(afxQuat const q, afxReal scale, afxQuat out);

// Madd

AFXINL void     AfxV2dMadd(afxV2d const v, afxV2d const other, afxV2d out);
AFXINL void     AfxV3dMadd(afxV3d const v, afxV3d const other, afxV3d out);
AFXINL void     AfxV4dMadd(afxV4d const v, afxV4d const other, afxV4d out);

// CombineMulAndAdd

AFXINL void     AfxV2dCombineMulAndAdd(afxV2d const v, afxV2d const mul, afxV2d const plus, afxV2d out);
AFXINL void     AfxV3dCombineMulAndAdd(afxV3d const v, afxV3d const mul, afxV3d const plus, afxV3d out);
AFXINL void     AfxV4dCombineMulAndAdd(afxV4d const v, afxV4d const mul, afxV4d const plus, afxV4d out);

// Sqrt

AFXINL void     AfxV2dSqrt(afxV2d const v, afxV2d out);
AFXINL void     AfxV3dSqrt(afxV3d const v, afxV3d out);
AFXINL void     AfxV4dSqrt(afxV4d const v, afxV4d out);
AFXINL void     AfxV2dSqrtRecip(afxV2d const v, afxV2d out);
AFXINL void     AfxV3dSqrtRecip(afxV3d const v, afxV3d out);
AFXINL void     AfxV4dSqrtRecip(afxV4d const v, afxV4d out);

// Interpolation

AFXINL void     AfxV2dLerp(afxV2d const v, afxV2d const other, afxReal s, afxV2d out);
AFXINL void     AfxV3dLerp(afxV3d const v, afxV3d const other, afxReal s, afxV3d out);
AFXINL void     AfxV4dLerp(afxV4d const v, afxV4d const other, afxReal s, afxV4d out);
AFXINL void     AfxQuatLerp(afxQuat const q, afxQuat const other, afxReal percent, afxQuat out);
AFXINL void     AfxQuatSlerp(afxQuat const q, afxQuat const other, afxReal percent, afxQuat out);

// Sine

AFXINL void     AfxV2dSin(afxV2d const v, afxV2d out);
AFXINL void     AfxV3dSin(afxV3d const v, afxV3d out);
AFXINL void     AfxV4dSin(afxV4d const v, afxV4d out);

// Cosine

AFXINL void     AfxV2dCos(afxV2d const v, afxV2d out);
AFXINL void     AfxV3dCos(afxV3d const v, afxV3d out);
AFXINL void     AfxV4dCos(afxV4d const v, afxV4d out);

AFXINL void     AfxV2dAcos(afxV2d const v, afxV2d out);
AFXINL void     AfxV3dAcos(afxV3d const v, afxV3d out);
AFXINL void     AfxV4dAcos(afxV4d const v, afxV4d out);

// Tangent

AFXINL void     AfxV2dTan(afxV2d const v, afxV2d out);
AFXINL void     AfxV3dTan(afxV3d const v, afxV3d out);
AFXINL void     AfxV4dTan(afxV4d const v, afxV4d out);

AFXINL void     AfxV2dAtan(afxV2d const v, afxV2d out);
AFXINL void     AfxV3dAtan(afxV3d const v, afxV3d out);
AFXINL void     AfxV4dAtan(afxV4d const v, afxV4d out);

////////////////////////////////////////////////////////////////////////////////
// PROJECTION OPERATIONS                                                      //
////////////////////////////////////////////////////////////////////////////////

// Returns a point in Barycentric coordinates, using the specified position vectors.
// Formula: v[n] = a[n] + f * (b[n] - a[n]) + g * (c[n] - a[n])
AFXINL void     AfxV2dMakeBaryCentric(afxV2d v, afxV2d const a, afxV2d const b, afxV2d const c, afxReal f, afxReal g);
AFXINL void     AfxV3dMakeBaryCentric(afxV3d v, afxV3d const a, afxV3d const b, afxV3d const c, afxReal f, afxReal g);
AFXINL void     AfxV4dMakeBaryCentric(afxV4d v, afxV4d const a, afxV4d const b, afxV4d const c, afxReal f, afxReal g);

// Returns a point in Barycentric coordinates, using the specified position vectors.
// Formula: v[n] = a[n] + f[n] * (b[n] - a[n]) + g[n] * (c[n] - a[n]);
AFXINL void     AfxV2dMakeBaryCentric2(afxV2d v, afxV2d const a, afxV2d const b, afxV2d const c, afxV2d const f, afxV2d const g);
AFXINL void     AfxV3dMakeBaryCentric2(afxV3d v, afxV3d const a, afxV3d const b, afxV3d const c, afxV3d const f, afxV3d const g);
AFXINL void     AfxV4dMakeBaryCentric2(afxV4d v, afxV4d const a, afxV4d const b, afxV4d const c, afxV4d const f, afxV4d const g);

AFXINL void     AfxM4dMakeOrthographicLH(afxM4d m, afxV2d extent, afxReal near, afxReal far);
AFXINL void     AfxM4dMakeOrthographicRH(afxM4d m, afxV2d extent, afxReal near, afxReal far);
AFXINL void     AfxM4dMakeOrthographicOffCenterLH(afxM4d m, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far);
AFXINL void     AfxM4dMakeOrthographicOffCenterRH(afxM4d m, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far);

AFXINL void     AfxM4dMakePerspectiveLH(afxM4d m, afxV2d extent, afxReal near, afxReal far);
AFXINL void     AfxM4dMakePerspectiveRH(afxM4d m, afxV2d extent, afxReal near, afxReal far);
AFXINL void     AfxM4dMakePerspectiveOffCenterLH(afxM4d m, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far);
AFXINL void     AfxM4dMakePerspectiveOffCenterRH(afxM4d m, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far);

AFXINL void     AfxM4dMakeLookAtLH(afxM4d m, afxV3d const eye, afxV3d const point, afxV3d const up);
AFXINL void     AfxM4dMakeLookAtRH(afxM4d m, afxV3d const eye, afxV3d const point, afxV3d const up);

AFXINL void     AfxCorrectRwMatrix(afxM4d m, afxM4d const in);
AFXINL void     AfxM4dMakeRwView(afxM4d view, afxM4d cam);
AFXINL void     AfxM4dMakeRwProjection(afxM4d proj, afxV2d offset, afxV2d extent, afxReal nearPlane, afxReal farPlane, afxBool perspective);

#endif//AFX_MATH_DEFS_H
