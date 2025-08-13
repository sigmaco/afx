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

/**
    RenderWare uses 4x3, row-major affine matrices.
    
    This object defines a RenderWare Matrix object.
    
    The Matrix is heavily used throughout the API and a full range of functions are provided, including: rotation, multiplication, concatenation, scaling, translation, creation, destruction, stream read/write functions and a number of access functions to access particular vector data within the matrix.
    The "Right vector" is one of the vectors in a RenderWare Graphics matrix. It corresponds to the 'x' axis in a right-handed, xyz coordinate system. 
    The "Up vector" is one of the vectors in a RenderWare Graphics matrix. The up vector corresponds to the 'y' axis in a right-handed, xyz coordinate system. 
    The "at vector" is one of the vectors in a RenderWare Graphics matrix. It corresponds to the 'z' axis in a right-handed, xyz coordinate system. 
    RenderWare Graphics uses a right-handed co-ordinate system. Therefore with z increasing into the display screen, and y increasing upwards, the x co-ordinate system increases towards the left.
    RenderWare, such as DirectX, uses row vectors and ends up with the much more natural: result = input * local_to_object * object_to_world. Your input is in local space, it gets transformed into object space before finally ending up in world space. Clean, clear, and readable. If you instead multiply the two matrices together on their own, you get the clear local_to_world = local_to_object * object_to_world instead of the awkward local_to_world = object_to_world * local_to_object you would get with OpenGL shaders and column vectors.

    These translations are often called:

    Surge, translation along the longitudinal axis (forward or backwards)
    Sway, translation along the transverse axis (from side to side)
    Heave, translation along the vertical axis (to move up or down).

    The corresponding rotations are often called:

    roll, about the longitudinal axis
    pitch, about the transverse axis
    yaw, about the vertical axis.
*/

/// Affine matrix (RenderWare matrix) significa que somente o conjunto 4x3 é considerado. (aka. não é uma projective matrix). [0][3] = [1][3] = [2][3] = 0; [3][3] = 1;
/// Linear matrix significa que somente o conjunto 3x3 é considerado. (aka. não é uma translation matrix).
/// atm  = affine transformation matrix;
/// ltm  = linear transformation matrix;
/// iltm = inverse linear transformation matrix;
/// atv  = affine transformation vector; positions and points
/// ltv  = linear transformation vector; normals, directions and axis
/// iwm  = inverse world matrix;
/// wm   = world matrix;
/// m    = matrix;
/// mm   = model matrix;
/// ptm  = projective transformation matrix;
/// v    = vector

/// Entendendo um pouco da natureza das matrices.
/// Row-major and column major refere-se a forma como os valores de uma matrix são arranjados, onde:
///     Em row-major, os vetores são arranjos por cada row inteiro.      ( aX, aY, aZ, aW, bX, bY, bZ, bW, ...)
///     Em column-major, os valores são arranjos por cada column inteira ( aX, bX, cX, dX, aY, bY, cY, dY, ...).
/// "Transposição" refere-se a forma de interação com ao valores da matrix, isto é, se isto é pré-multiplicado para cada row ou pós-multiplicado para cada columna.
///     Em column-major, nós pós-multiplicamos um (column) vector (a direta) para cada columna da matrix (a esquerda), isto é, M[][c] x V[c].
///     Em row-major, nós pré-multiplicamos um (row) vector (a esquerda) para cada columna da matrix (a direita), isto é, V[r] x M[r][].
///     Uma pós-multiplicação de um (column) vector por uma column-major matrix deve ser igual de uma pré-multiplicação de (row) vector por uma row-major matrix.
/// "Handedness" refere-se a forma como um sistema de coordenada considera a orientação.
///     Em left-handed, um valor positivo ao eixo X representa o quão a "esquerda" algo está.
///     Em right-handed, um valor positivo ao eixo X representa o quão a "direita" algo está.

#ifndef AFX_MATRIX_H
#define AFX_MATRIX_H

#include "qwadro/inc/math/afxVector.h"
#include "qwadro/inc/math/afxQuaternion.h"
#include "qwadro/inc/math/coll/afxPlane.h"

AFX afxM2d const AFX_M2D_ZERO;
AFX afxM3d const AFX_M3D_ZERO;
AFX afxM4d const AFX_M4D_ZERO;

AFX afxM2d const AFX_M2D_IDENTITY;
AFX afxM3d const AFX_M3D_IDENTITY;
AFX afxM4d const AFX_M4D_IDENTITY;

AFXINL void     AfxM2dZero(afxM2d m);
AFXINL void     AfxM3dZero(afxM3d m);
AFXINL void     AfxM4dZero(afxM4d m);

AFXINL void     AfxM2dReset(afxM2d m);
AFXINL void     AfxM3dReset(afxM3d m);
AFXINL void     AfxM4dReset(afxM4d m);

AFXINL void     AfxM2dSet(afxM2d m, afxV2d const x, afxV2d const y);
AFXINL void     AfxM3dSet(afxM3d m, afxV3d const x, afxV3d const y, afxV3d const z);
AFXINL void     AfxM4dSet(afxM4d m, afxV4d const x, afxV4d const y, afxV4d const z, afxV4d const w);
AFXINL void     AfxM4dSetLinear(afxM4d m, afxV3d const x, afxV3d const y, afxV3d const z, afxV3d const w);
AFXINL void     AfxM4dSetAffine(afxM4d m, afxV3d const x, afxV3d const y, afxV3d const z, afxV3d const w);

AFXINL void     AfxM2dSetTransposed(afxM2d m, afxV2d const x, afxV2d const y); // Be carefull using it in Qwadro
AFXINL void     AfxM3dSetTransposed(afxM3d m, afxV3d const x, afxV3d const y, afxV3d const z); // Be carefull using it in Qwadro
AFXINL void     AfxM4dSetTransposed(afxM4d m, afxV4d const x, afxV4d const y, afxV4d const z, afxV4d const w);  // Be carefull using it in Qwadro

AFXINL void     AfxM4dEnsureLinear(afxM4d m); // make affine and zero translation.
AFXINL void     AfxM4dEnsureAffine(afxM4d m); // make affine.

AFXINL afxBool  AfxM2dIsIdentity(afxM2d const m);
AFXINL afxBool  AfxM3dIsIdentity(afxM3d const m);
AFXINL afxBool  AfxM4dIsIdentity(afxM4d const m);

AFXINL void     AfxM2dCopy(afxM2d m, afxM2d const in);
AFXINL void     AfxM3dCopy(afxM3d m, afxM3d const in);
AFXINL void     AfxM4dCopy(afxM4d m, afxM4d const in); // copy every row and column.
AFXINL void     AfxM3dCopyM2d(afxM3d m, afxM2d const in);
AFXINL void     AfxM4dCopyM2d(afxM4d m, afxM2d const in);
AFXINL void     AfxM2dCopyM3d(afxM2d m, afxM3d const in);
AFXINL void     AfxM4dCopyM3d(afxM4d m, afxM3d const ltm, afxV4d const atv);
AFXINL void     AfxM4dCopyLtm(afxM4d m, afxM4d const in); // copy only the 3x3
AFXINL void     AfxM4dCopyAtm(afxM4d m, afxM4d const in); // copy only the 4x3
AFXINL void     AfxM3dCopyM4d(afxM3d m, afxM4d const in);

AFXINL void     AfxM4rCopy(afxM4r m, afxM4r const in);
AFXINL void     AfxM4rCopyM3d(afxM4r m, afxM3d const in);
AFXINL void     AfxM4rCopyM4d(afxM4r m, afxM4d const in);

AFXINL void     AfxM2dTranspose(afxM2d m, afxM2d const in);
AFXINL void     AfxM3dTranspose(afxM3d m, afxM3d const in);
AFXINL void     AfxM4dTranspose(afxM4d m, afxM4d const in);
AFXINL void     AfxM4dTransposeAtm(afxM4d m, afxM4d const in); // only consider 4x3; ignore W components.
AFXINL void     AfxM3dTransposeM4d(afxM3d m, afxM4d const in); // ltm will be transposed
AFXINL void     AfxM4dTransposeLtm(afxM4d m, afxM4d const in); // only consider 3x3; ignore W components and the W row.
AFXINL void     AfxM4dTransposeM3d(afxM4d m, afxM3d const ltm, afxV4d const atv);

AFXINL afxReal  AfxM3dInvert(afxM3d m, afxM3d const in); // im = inverse of m
AFXINL afxReal  AfxM4dInvert(afxM4d m, afxM4d const in); // im = inverse of m
AFXINL afxReal  AfxM4dInvertAffine(afxM4d m, afxM4d const in); // im = inverse of m

/// Computes the determinant of a matrix.
AFXINL afxReal  AfxM2dDet(afxM2d const m);
AFXINL afxReal  AfxM3dDet(afxM3d const m);
AFXINL afxReal  AfxM4dDet(afxM4d const m);

AFXINL void     AfxM2dSwap(afxM2d m, afxM2d other);
AFXINL void     AfxM3dSwap(afxM3d m, afxM3d other);
AFXINL void     AfxM4dSwap(afxM4d m, afxM4d other);

AFXINL void     AfxM2dAdd(afxM2d m, afxM2d const a, afxM2d const b); // m = a + b
AFXINL void     AfxM3dAdd(afxM3d m, afxM3d const a, afxM3d const b); // m = a + b
AFXINL void     AfxM4dAdd(afxM4d m, afxM4d const a, afxM4d const b); // m = a + b

AFXINL void     AfxM2dSub(afxM2d m, afxM2d const a, afxM2d const b); // m = a - b
AFXINL void     AfxM3dSub(afxM3d m, afxM3d const a, afxM3d const b); // m = a - b
AFXINL void     AfxM4dSub(afxM4d m, afxM4d const a, afxM4d const b); // m = a - b

AFXINL void     AfxM2dScale(afxM2d m, afxM2d const in, afxReal scale);
AFXINL void     AfxM3dScale(afxM3d m, afxM3d const in, afxReal scale);
AFXINL void     AfxM4dScale(afxM4d m, afxM4d const in, afxReal scale);

AFXINL void     AfxM2dMads(afxM2d m, afxReal scale, afxM2d const b, afxM2d const c);
AFXINL void     AfxM3dMads(afxM3d m, afxReal scale, afxM3d const b, afxM3d const c);
AFXINL void     AfxM4dMads(afxM4d m, afxReal scale, afxM4d const b, afxM4d const c);

// Mix
// The MIX function is conceptually similar to LERP but may involve additional functionality, such as more complex blending modes or different blending parameters. 
// It is often used in shader programming and graphics.
// v = x * (1 - t) + y * t

AFXINL void     AfxM2dMix(afxM2d v, afxM2d const x, afxM2d const y, afxReal t);
AFXINL void     AfxM3dMix(afxM3d v, afxM3d const x, afxM3d const y, afxReal t);
AFXINL void     AfxM4dMix(afxM4d v, afxM4d const x, afxM4d const y, afxReal t);

AFXINL void     AfxM3dMixAffine(afxM3d m, afxM3d const a, afxM3d const b, afxReal t);
AFXINL void     AfxM4dMixAffine(afxM4d m, afxM4d const a, afxM4d const b, afxReal t);
AFXINL void     AfxM4dMixLinear(afxM4d m, afxM4d const a, afxM4d const b, afxReal t);

// Lerp
// LERP is a method to interpolate linearly between two values. 
// In the context of 4D vectors, it calculates the intermediate vector between a start and end vector based on a factor t ranging from 0.0 to 1.0.
// x + t * (y - x)

AFXINL void     AfxM2dLerp(afxM2d v, afxM2d const x, afxM2d const y, afxReal t);
AFXINL void     AfxM3dLerp(afxM3d v, afxM3d const x, afxM3d const y, afxReal t);
AFXINL void     AfxM4dLerp(afxM4d v, afxM4d const x, afxM4d const y, afxReal t);

AFXINL void     AfxM3dLerpAffine(afxM3d v, afxM3d const x, afxM3d const y, afxReal t);
AFXINL void     AfxM4dLerpAffine(afxM4d v, afxM4d const x, afxM4d const y, afxReal t);
AFXINL void     AfxM4dLerpLinear(afxM4d v, afxM4d const x, afxM4d const y, afxReal t);

////////////////////////////////////////////////////////////////////////////////
// Affine transformation matrix methods                                       //
////////////////////////////////////////////////////////////////////////////////

/// Builds a matrix that scales along the x-axis, y-axis, and z-axis.
AFXINL void         AfxM3dScaling(afxM3d m, afxV3d const scale);
AFXINL void         AfxM4dScaling(afxM4d m, afxV3d const scale);

/// Builds a rotation matrix from a quaternion.
AFXINL void         AfxM3dRotationFromQuat(afxM3d m, afxQuat const q);
AFXINL void         AfxM4dRotationFromQuat(afxM4d m, afxQuat const q);

/// Builds a matrix that rotates around an arbitrary axis.
AFXINL void         AfxM3dRotationFromAxis(afxM3d m, afxV3d const axis, afxReal /*theta*/radians);
AFXINL void         AfxM4dRotationFromAxis(afxM4d m, afxV3d const axis, afxReal /*theta*/radians);

/// Builds a rotation matrix based on a given pitch, yaw, and roll (Euler angles).
AFXINL void         AfxM4dRotationFromEuler(afxM4d m, afxReal pitch, afxReal yaw, afxReal roll);

/// Builds a matrix that rotates around the x-axis, y-axis or z-axis.
/// Angles are measured clockwise when looking along the rotation axis toward the origin.
AFXINL void         AfxM4dRotationX(afxM4d m, afxReal angle);
AFXINL void         AfxM4dRotationY(afxM4d m, afxReal angle);
AFXINL void         AfxM4dRotationZ(afxM4d m, afxReal angle);

AFXINL void         AfxM4dTranslation
/// Builds a translation matrix from the specified vector describing the translations along the x-axis, y-axis, and z-axis.
(
    afxM4d          m, 
    afxV3d const    translation // vector describing the translations along the x-axis, y-axis, and z-axis.
);

// PolarDecompose

/// Qwadro expects all animation tracks to be decomposed. 
/// Position, orientation, and scale/shear are stored separately. 
/// The exporters kick out data in this format, but if you're writing your own important or processing tool, you may only have matrices as input. 
/// Decomposing an arbitrary 3x3 matrix into its rotational and scale/shear components can be difficult, so Qwadro provides a function for doing this: 

AFXINL afxBool      AfxPolarDecomposeM3d(afxM3d const m, afxReal tol, afxM3d rm, afxM3d ssm);

// Compose

AFXINL void         AfxM4dComposeTransformation
/// Builds a transformation matrix.
(
    afxM4d          m, 
    afxV3d const    scalPivot, // Vector describing the center of the scaling.
    afxQuat const   scalOrient, // Quaternion describing the orientation of the scaling.
    afxV3d const    scaling, // Vector containing the scaling factors for the x-axis, y-axis, and z-axis.
    afxV3d const    rotPivot, // Vector describing the center of the rotation.
    afxQuat const   rotQuat, // Quaternion describing the rotation around the origin indicated by rotOrigin.
    afxV3d const    translation // Vector describing the translations along the x-axis, y-axis, and z-axis.
);

AFXINL void         AfxM4dComposeAffineTransformation
/// Builds an affine transformation matrix.
(
    afxM4d          m, 
    afxV3d const    scale, // Vector of scaling factors for each dimension.
    afxV3d const    rotPivot, // Point identifying the center of rotation.
    afxQuat const   rot, // Rotation factors.
    afxV3d const    translation // Translation offsets.
);

////////////////////////////////////////////////////////////////////////////////
// MATRIX TRANSFORMATION METHODS                                              //
////////////////////////////////////////////////////////////////////////////////

//  --- default layout
//  in[n][0] * by[0][0],  in[n][1] * by[1][0],  in[n][2] * by[2][0],  in[n][3] * by[3][0]
//  in[n][0] * by[0][1],  in[n][1] * by[1][1],  in[n][2] * by[2][1],  in[n][3] * by[3][1]
//  in[n][0] * by[0][2],  in[n][1] * by[1][2],  in[n][2] * by[2][2],  in[n][3] * by[3][2]
//  in[n][0] * by[0][3],  in[n][1] * by[1][3],  in[n][2] * by[2][3],  in[n][3] * by[3][3]

//  --- transposed layout
//  in[0][0] * by[0][n],  in[0][1] * by[1][n],  in[0][2] * by[2][n],  in[0][3] * by[3][n]
//  in[1][0] * by[0][n],  in[1][1] * by[1][n],  in[1][2] * by[2][n],  in[1][3] * by[3][n]
//  in[2][0] * by[0][n],  in[2][1] * by[1][n],  in[2][2] * by[2][n],  in[2][3] * by[3][n]
//  in[3][0] * by[0][n],  in[3][1] * by[1][n],  in[3][2] * by[2][n],  in[3][3] * by[3][n]

// Computes the product of two matrices.

AFXINL void     AfxM2dMultiply(afxM2d m, afxM2d const in, afxM2d const by);
AFXINL void     AfxM3dMultiply(afxM3d m, afxM3d const in, afxM3d const by);
AFXINL void     AfxM4dMultiply(afxM4d m, afxM4d const in, afxM4d const by);

AFXINL void     AfxM3dMultiplyAffine(afxM3d m, afxM3d const in, afxM3d const by); // 2x2 subset only
AFXINL void     AfxM4dMultiplyLinear(afxM4d m, afxM4d const in, afxM4d const by); // 3x3 subset only
AFXINL void     AfxM4dMultiplyAffine(afxM4d m, afxM4d const in, afxM4d const by); // 4x3 subset only (aka RenderWare (RwMatrix) matrix)

AFXINL void     TransposeMatrixMultiply3x3(afxM3d IntoMatrix, afxM3d const TransposedMatrix, afxM3d const ByMatrix);

// Computes the transpose of the product of two matrices.

AFXINL void     AfxM2dMultiplyTransposed(afxM2d m, afxM2d const in, afxM2d const by);
AFXINL void     AfxM3dMultiplyTransposed(afxM3d m, afxM3d const in, afxM3d const by);
AFXINL void     AfxM4dMultiplyTransposed(afxM4d m, afxM4d const in, afxM4d const by);

AFXINL void     AfxM3dMultiplyTransposedAffine(afxM3d m, afxM3d const in, afxM3d const by); // 2x2 subset only
AFXINL void     AfxM4dMultiplyTransposedLinear(afxM4d m, afxM4d const in, afxM4d const by); // 3x3 subset only of an affine transformation matrix
AFXINL void     AfxM4dMultiplyTransposedAffine(afxM4d m, afxM4d const in, afxM4d const by); // 4x3 subset only (aka RenderWare (RwMatrix) matrix)

// Matrix vs Vector

AFXINL void     AfxM2dPostMultiplyV2d(afxM2d const m, afxUnit cnt, afxV2d const in[], afxV2d out[]);
AFXINL void     AfxM3dPostMultiplyV3d(afxM3d const m, afxUnit cnt, afxV3d const in[], afxV3d out[]);
AFXINL void     AfxM4dPostMultiplyV4d(afxM4d const m, afxUnit cnt, afxV4d const in[], afxV4d out[]);

AFXINL void     AfxM3dPostMultiplyLtv4d(afxM3d const m, afxUnit cnt, afxV4d const in[], afxV4d out[]);
AFXINL void     AfxM4dPostMultiplyAtv3d(afxM4d const m, afxUnit cnt, afxV3d const in[], afxV3d out[]);
AFXINL void     AfxM4dPostMultiplyAtv4d(afxM4d const m, afxUnit cnt, afxV4d const in[], afxV4d out[]);

AFXINL void     AfxM2dPreMultiplyV2d(afxM2d const m, afxUnit cnt, afxV2d const in[], afxV2d out[]);
AFXINL void     AfxM3dPreMultiplyV3d(afxM3d const m, afxUnit cnt, afxV3d const in[], afxV3d out[]);
AFXINL void     AfxM4dPreMultiplyV4d(afxM4d const m, afxUnit cnt, afxV4d const in[], afxV4d out[]);

AFXINL void     AfxM3dPreMultiplyLtv4d(afxM3d const m, afxUnit cnt, afxV4d const in[], afxV4d out[]);
AFXINL void     AfxM4dPreMultiplyAtv3d(afxM4d const m, afxUnit cnt, afxV3d const in[], afxV3d out[]);
AFXINL void     AfxM4dPreMultiplyAtv4d(afxM4d const m, afxUnit cnt, afxV4d const in[], afxV4d out[]);


AFXINL void     AfxM2dPostMultiplyV2dSerialized(afxM2d const m, afxUnit inStride, afxUnit outStride, afxUnit cnt, afxV2d const in[], afxV2d out[]);
AFXINL void     AfxM3dPostMultiplyV3dSerialized(afxM3d const m, afxUnit inStride, afxUnit outStride, afxUnit cnt, afxV3d const in[], afxV3d out[]);
AFXINL void     AfxM4dPostMultiplyV4dSerialized(afxM4d const m, afxUnit inStride, afxUnit outStride, afxUnit cnt, afxV4d const in[], afxV4d out[]);

AFXINL void     AfxM4dPostMultiplyAtv3dSerialized(afxM4d const m, afxUnit inStride, afxUnit outStride, afxUnit cnt, afxV3d const in[], afxV3d out[]);
AFXINL void     AfxM3dPostMultiplyLtv4dSerialized(afxM3d const m, afxUnit inStride, afxUnit outStride, afxUnit cnt, afxV4d const in[], afxV4d out[]);
AFXINL void     AfxM4dPostMultiplyAtv4dSerialized(afxM4d const m, afxUnit inStride, afxUnit outStride, afxUnit cnt, afxV4d const in[], afxV4d out[]);

// Assimilate

AFXINL void     AfxAssimilateLtm3d(afxM3d const ltm, afxM3d const iltm, afxUnit cnt, afxM3d const in[], afxM3d out[]); // make similarity transformation on afxM3d-based scale/shear.
AFXINL void     AfxAssimilateAtm4d(afxM3d const ltm, afxM3d const iltm, afxV4d const atv, afxUnit cnt, afxM4d const in[], afxM4d out[]);

AFXINL void     AfxM4dApplyRigidMotion(afxM4d m, afxM4d const mm, afxV3d const rotation, afxV3d const translation);

#endif//AFX_MATRIX_H
