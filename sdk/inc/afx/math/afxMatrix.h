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

#ifndef AFX_MATRIX_H
#define AFX_MATRIX_H

#include "afxPlane.h"
#include "afxViewport.h"

 // The "Right vector" is one of the vectors in a RenderWare Graphics matrix. It corresponds to the 'x' axis in a right-handed, xyz coordinate system. 
 // The "Up vector" is one of the vectors in a RenderWare Graphics matrix. The up vector corresponds to the 'y' axis in a right-handed, xyz coordinate system. 
 // The "at vector" is one of the vectors in a RenderWare Graphics matrix. It corresponds to the 'z' axis in a right-handed, xyz coordinate system. 
 // RenderWare Graphics uses a right-handed co-ordinate system. Therefore with z increasing into the display screen, and y increasing upwards, the x co-ordinate system increases towards the left.

 // This object defines a RenderWare Matrix object.
 // The Matrix is heavily used throughout the API and a full range of functions are provided, including: rotation, multiplication, concatenation, scaling, translation, creation, destruction, stream read/write functions and a number of access functions to access particular vector data within the matrix.
 // RenderWare uses 4x3, row-major affine matrices.

// afxM4d is a row-major right-handled matrix. Qwadro inherits matrix from RenderWare.
// RenderWare, such as DirectX, uses row vectors and ends up with the much more natural: result = input * local_to_object * object_to_world. Your input is in local space, it gets transformed into object space before finally ending up in world space. Clean, clear, and readable. If you instead multiply the two matrices together on their own, you get the clear local_to_world = local_to_object * object_to_world instead of the awkward local_to_world = object_to_world * local_to_object you would get with OpenGL shaders and column vectors.

/*
These translations are often called:

Surge, translation along the longitudinal axis (forward or backwards)
Sway, translation along the transverse axis (from side to side)
Heave, translation along the vertical axis (to move up or down).

The corresponding rotations are often called:

roll, about the longitudinal axis
pitch, about the transverse axis
yaw, about the vertical axis.

*/

//#define MFX_USE_COLUMN_MAJOR_MATRIX
//#define MFX_USE_LEFT_HANDED_MATRIX



// Affine matrix significa que somente o conjunto 4x3 é considerado. (aka. não é uma projective matrix). [0][3] = [1][3] = [2][3] = 0; [3][3] = 1;
// Linear matrix significa que somente o conjunto 3x3 é considerado. (aka. não é uma 3D translation matrix).
// atm  = affine transformation matrix;
// ltm  = linear transformation matrix;
// iltm = inverse linear transformation matrix;
// atv  = affine transformation vector; positions and points
// ltv  = linear transformation vector; normals, directions and axis
// iwm  = inverse world matrix;
// wm   = world matrix;
// m    = matrix;
// mm   = model matrix;
// ptm  = projective transformation matrix;
// v    = vector

/// Entendendo um pouco da natureza das matrices.
/// Row-major and column major refere-se a forma como os valores de uma matrix são arranjados, onde:
///     Em row-major, os vetores são arranjos por cada row inteiro.      ( aX, aY, aZ, aW, bX, bY, bZ, bW, ...)
///     Em column-major, os valores são arranjos por cada column inteira ( aX, bX, cX, dX, aY, bY, cY, dY, ...).
/// "Transposição" refere-se a forma de interação com ao valores da matrix, isto é, se isto é pré-multiplicado para cada row ou pós-multiplicado para cada columna.
///     Em column-major, nós pós-multiplicamos um (column) vector (a direta) para cada columna da matrix (a esquerda), isto é, M[][c] x V[c].
///     Em row-major, nós pré-multiplicamos um (row) vector (a esquerda) para cada columna da matrix (a direita), isto é, V[r] x M[r][].
///     Uma pós-multiplicação de um (column) vector por uma column-major matrix deve ser igual de uma pré-multiplicação de (row) vector por uma row-major matrix.
/// "Handedness" refere-se a forma como um sistema de coordenada considera o eixo X.
///     Em left-handed, um valor positivo ao eixo X representa o quão a "esquerda" algo está.
///     Em right-handed, um valor positivo ao eixo X representa o quão a "direita" algo está.

AFX afxM2d  const AFX_M2D_ZERO;
AFX afxM3d  const AFX_M3D_ZERO;
AFX afxM4d  const AFX_M4D_ZERO;

AFX afxM2d  const AFX_M2D_IDENTITY;
AFX afxM3d  const AFX_M3D_IDENTITY;
AFX afxM4d  const AFX_M4D_IDENTITY;

////////////////////////////////////////////////////////////////////////////////
// Initialization methods                                                     //
////////////////////////////////////////////////////////////////////////////////

AFXINL void     AfxZeroM2d(afxReal m[2][2]);
AFXINL void     AfxZeroM3d(afxReal m[3][3]);
AFXINL void     AfxZeroM4d(afxReal m[4][4]);

AFXINL void     AfxResetM2d(afxReal m[2][2]);
AFXINL void     AfxResetM3d(afxReal m[3][3]);
AFXINL void     AfxResetM4d(afxReal m[4][4]);

AFXINL void     AfxSetM2d(afxReal m[2][2], afxReal const x[2], afxReal const y[2]);
AFXINL void     AfxSetM3d(afxReal m[3][3], afxReal const x[3], afxReal const y[3], afxReal const z[3]);
AFXINL void     AfxSetM4d(afxReal m[4][4], afxReal const x[4], afxReal const y[4], afxReal const z[4], afxReal const w[4]);

// SetMatrixTransposed initializes with column vectors

AFXINL void     AfxSetM2dTransposed(afxReal m[2][2], afxReal const x[2], afxReal const y[2]);
AFXINL void     AfxSetM3dTransposed(afxReal m[3][3], afxReal const x[3], afxReal const y[3], afxReal const z[3]);
AFXINL void     AfxSetM4dTransposed(afxReal m[4][4], afxReal const x[4], afxReal const y[4], afxReal const z[4], afxReal const w[4]);

AFXINL void     AfxEnsureLinearM4d(afxReal m[4][4]); // make affine and zero translation.
AFXINL void     AfxEnsureAffineM4d(afxReal m[4][4]); // make affine; zero every W column.

////////////////////////////////////////////////////////////////////////////////
// Logic methods                                                              //
////////////////////////////////////////////////////////////////////////////////

AFXINL afxBool  AfxM2dIsIdentity(afxReal const m[2][2]);
AFXINL afxBool  AfxM3dIsIdentity(afxReal const m[3][3]);
AFXINL afxBool  AfxM4dIsIdentity(afxReal const m[4][4]);

////////////////////////////////////////////////////////////////////////////////
// Transferance methods                                                       //
////////////////////////////////////////////////////////////////////////////////

AFXINL void     AfxCopyM2d(afxReal m[2][2], afxReal const in[2][2]);
AFXINL void     AfxM2dFromM3d(afxReal m[2][2], afxReal const in[3][3]);
AFXINL void     AfxM2dFromM4d(afxReal m[2][2], afxReal const in[4][4]);

AFXINL void     AfxCopyM3d(afxReal m[3][3], afxReal const in[3][3]);
AFXINL void     AfxM3dFromM4d(afxReal m[3][3], afxReal const in[4][4]);
AFXINL void     AfxM3dFromM4dTransposed(afxReal m[3][3], afxReal const in[4][4]);

AFXINL void     AfxCopyM4d(afxReal m[4][4], afxReal const in[4][4]); // copy every row and column.
AFXINL void     AfxCopyLinearM4d(afxReal m[4][4], afxReal const in[4][4]); // copy only the 3x3
AFXINL void     AfxCopyAffineM4d(afxReal m[4][4], afxReal const in[4][4]); // copy only the 4x3

AFXINL void     AfxM3dFromM2d(afxReal m[3][3], afxReal const in[2][2]);
AFXINL void     AfxM4dFromM2d(afxReal m[4][4], afxReal const in[2][2]);
AFXINL void     AfxM4dFromM3d(afxReal m[4][4], afxReal const in[3][3], afxReal const translation[4]);

AFXINL void     AfxSwapM2d(afxReal m[2][2], afxReal other[2][2]);
AFXINL void     AfxSwapM3d(afxReal m[3][3], afxReal other[3][3]);
AFXINL void     AfxSwapM4d(afxReal m[4][4], afxReal other[4][4]);

AFXINL void     AfxTransposeM2d(afxReal m[2][2], afxReal const in[2][2]);
AFXINL void     AfxTransposeM3d(afxReal m[3][3], afxReal const in[3][3]);
AFXINL void     AfxTransposeM4d(afxReal m[4][4], afxReal const in[4][4]);

AFXINL void     AfxM4dFromM3dTransposed(afxReal m[4][4], afxReal const in[3][3], afxReal const translation[4]);
AFXINL void     AfxM4dFromLinearTransposed(afxReal m[4][4], afxReal const in[4][4]); // only consider 3x3; ignore W components and the W row.
AFXINL void     AfxM4dFromAffineTransposed(afxReal m[4][4], afxReal const in[4][4]); // only consider 4x3; ignore W components.

////////////////////////////////////////////////////////////////////////////////
// Clamping methods                                                           //
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
// Arithmetic methods                                                         //
////////////////////////////////////////////////////////////////////////////////

AFXINL void     AfxAddM2d(afxReal m[2][2], afxReal const a[2][2], afxReal const b[2][2]); // m = a + b
AFXINL void     AfxAddM3d(afxReal m[3][3], afxReal const a[3][3], afxReal const b[3][3]); // m = a + b
AFXINL void     AfxAddM4d(afxReal m[4][4], afxReal const a[4][4], afxReal const b[4][4]); // m = a + b

AFXINL void     AfxSubM2d(afxReal m[2][2], afxReal const a[2][2], afxReal const b[2][2]); // m = a - b
AFXINL void     AfxSubM3d(afxReal m[3][3], afxReal const a[3][3], afxReal const b[3][3]); // m = a - b
AFXINL void     AfxSubM4d(afxReal m[4][4], afxReal const a[4][4], afxReal const b[4][4]); // m = a - b

////////////////////////////////////////////////////////////////////////////////
// Algebra                                                                    //
////////////////////////////////////////////////////////////////////////////////

AFXINL afxReal  AfxInvertM3d(afxReal m[3][3], afxReal const in[3][3]); // m = inverse of in
AFXINL afxReal  AfxInvertM4d(afxReal m[4][4], afxReal const in[4][4]); // m = inverse of in
AFXINL afxReal  AfxInvertAffineM4d(afxReal m[4][4], afxReal const in[4][4]); // m = inverse of in

AFXINL afxReal  AfxDetM2d(afxReal const m[2][2]);
AFXINL afxReal  AfxDetM3d(afxReal const m[3][3]);
AFXINL afxReal  AfxDetM4d(afxReal const m[4][4]);

////////////////////////////////////////////////////////////////////////////////
// Affine transformation matrix methods                                       //
////////////////////////////////////////////////////////////////////////////////

// Scale

AFXINL void     AfxSetM3dScaleV2d(afxReal m[3][3], afxReal const v[2]); // aka get matrix scale
AFXINL void     AfxSetM3dScale(afxReal m[3][3], afxReal const v[3]);
AFXINL void     AfxSetM4dScaleV2d(afxReal m[4][4], afxReal const v[2]);
AFXINL void     AfxSetM4dScale(afxReal m[4][4], afxReal const v[3]);

AFXINL void     AfxScaleM3d(afxReal m[3][3], afxReal const scale[3]);
AFXINL void     AfxScalePlanarM3d(afxReal m[3][3], afxReal const scale[3]); // scales 2D affine transformation (represented by afxM3d)
AFXINL void     AfxScaleM4d(afxReal m[4][4], afxReal const scale[4]);
AFXINL void     AfxScaleAffineM4d(afxReal m[4][4], afxReal const scale[3]); // scales 3D affine transformation (represented by afxM4d)

// Rotate and Orientation

/// Rotation matrices are neither left-handed nor right-handed. The are universal for any handedness. – Nico Schertler

/// It is useful to be able to move between the 3x3 matrix representation of a rotation and its quaternion counterpart. 
/// Qwadro provides functions for converting in both directions: 

AFXINL void     AfxM3dFromQuat(afxReal m[3][3], afxQuat const q);
AFXINL void     AfxM4dFromQuat(afxReal m[4][4], afxQuat const q);

AFXINL void     AfxM3dFromAxisAngle(afxReal m[3][3], afxReal const axis[3], afxReal /*theta*/radians);
AFXINL void     AfxM4dFromAxisAngle(afxReal m[4][4], afxReal const axis[3], afxReal /*theta*/radians);
AFXINL void     AfxM4dFromEuler(afxReal m[4][4], afxReal const pitchYawRoll[3]);

AFXINL void     AfxM4dFromRotationX(afxReal m[4][4], afxReal angle);
AFXINL void     AfxM4dFromRotationY(afxReal m[4][4], afxReal angle);
AFXINL void     AfxM4dFromRotationZ(afxReal m[4][4], afxReal angle);

AFXINL void     AfxRotateM3d_Axial(afxReal m[3][3], afxReal const axis[3], afxReal radians); // accumulates a rotation matrix from the given axis and angle of rotation and applies it to the specified matrix.
AFXINL void     AfxRotateM4d_Axial(afxReal m[4][4], afxReal const axis[3], afxReal radians); // accumulates a rotation matrix from the given axis and angle of rotation and applies it to the specified matrix.

// Translate and Position

AFXINL void     AfxTranslateM4d(afxReal m[4][4], afxReal const translation[4]);
AFXINL void     AfxTranslateAffineM4d(afxReal m[4][4], afxReal const translation[3]);

// PolarDecompose

/// Qwadro expects all animation tracks to be decomposed. 
/// Position, orientation, and scale/shear are stored separately. 
/// The exporters kick out data in this format, but if you're writing your own important or processing tool, you may only have matrices as input. 
/// Decomposing an arbitrary 3x3 matrix into its rotational and scale/shear components can be difficult, so Qwadro provides a function for doing this: 

AFXINL afxBool  AfxPolarDecomposeM3d(afxReal const m[3][3], afxReal tol, afxReal rm[3][3], afxReal ssm[3][3]);

// Compose

AFXINL void     AfxComposeM4d(afxReal m[4][4], afxReal const scalOrigin[3], afxQuat const scalOrient, afxReal const scaling[3], afxReal const rotOrigin[3], afxQuat const rotQuat, afxReal const translation[3]);
AFXINL void     AfxComposeAffineM4d(afxReal m[4][4], afxReal const scale[3], afxReal const rotAxis[3], afxQuat const rot, afxReal const translation[3]);

////////////////////////////////////////////////////////////////////////////////
// MATRIX TRANSFORMATION METHODS                                              //
////////////////////////////////////////////////////////////////////////////////

//  --- Normal layout
//  r[0][0] * c[0][n],  r[0][1] * c[1][n],  r[0][2] * c[2][n],  r[0][3] * c[3][n]
//  r[1][0] * c[0][n],  r[1][1] * c[1][n],  r[1][2] * c[2][n],  r[1][3] * c[3][n]
//  r[2][0] * c[0][n],  r[2][1] * c[1][n],  r[2][2] * c[2][n],  r[2][3] * c[3][n]
//  r[3][0] * c[0][n],  r[3][1] * c[1][n],  r[3][2] * c[2][n],  r[3][3] * c[3][n]

//  --- Transposed layout
//  r[n][0] * c[0][0],  r[n][1] * c[1][0],  r[n][2] * c[2][0],  r[n][3] * c[3][0]
//  r[n][0] * c[0][1],  r[n][1] * c[1][1],  r[n][2] * c[2][1],  r[n][3] * c[3][1]
//  r[n][0] * c[0][2],  r[n][1] * c[1][2],  r[n][2] * c[2][2],  r[n][3] * c[3][2]
//  r[n][0] * c[0][3],  r[n][1] * c[1][3],  r[n][2] * c[2][3],  r[n][3] * c[3][3]

AFXINL void     AfxMultiplyM2d(afxReal m[2][2], afxReal const r[2][2], afxReal const c[2][2]);
AFXINL void     AfxMultiplyM3d(afxReal m[3][3], afxReal const r[3][3], afxReal const c[3][3]);
AFXINL void     AfxMultiplyM4d(afxReal m[4][4], afxReal const r[4][4], afxReal const c[4][4]);

AFXINL void     AfxMultiplyPlanarM3d(afxReal m[3][3], afxReal const r[3][3], afxReal const c[3][3]);
AFXINL void     AfxMultiplyLinearM4d(afxReal m[4][4], afxReal const r[4][4], afxReal const c[4][4]);
AFXINL void     AfxMultiplyAffineM4d(afxReal m[4][4], afxReal const r[4][4], afxReal const c[4][4]);

AFXINL void     AfxMultiplyTransposedM2d(afxReal m[2][2], afxReal const c[2][2], afxReal const r[2][2]);
AFXINL void     AfxMultiplyTransposedM3d(afxReal m[3][3], afxReal const c[3][3], afxReal const r[3][3]);
AFXINL void     AfxMultiplyTransposedM4d(afxReal m[4][4], afxReal const c[4][4], afxReal const r[4][4]);

AFXINL void     AfxMultiplyTransposedPlanarM3d(afxReal m[3][3], afxReal const c[3][3], afxReal const r[3][3]);
AFXINL void     AfxMultiplyTransposedLinearM4d(afxReal m[4][4], afxReal const c[4][4], afxReal const r[4][4]);
AFXINL void     AfxMultiplyTransposedAffineM4d(afxReal m[4][4], afxReal const c[4][4], afxReal const r[4][4]);

AFXINL void     AfxTransformArrayedV2d(afxReal const m[2][2], afxNat cnt, afxReal const in[][2], afxReal out[][2]);
AFXINL void     AfxTransformArrayedV3d(afxReal const m[3][3], afxNat cnt, afxReal const in[][3], afxReal out[][3]);
AFXINL void     AfxTransformArrayedV4d(afxReal const m[4][4], afxNat cnt, afxReal const in[][4], afxReal out[][4]);

AFXINL void     AfxTransformArrayedPointV3d(afxReal const m[4][4], afxNat cnt, afxReal const in[][3], afxReal out[][3]);
AFXINL void     AfxTransformArrayedNormalV4d(afxReal const m[3][3], afxNat cnt, afxReal const in[][4], afxReal out[][4]);
AFXINL void     AfxTransformArrayedAffineV4d(afxReal const m[4][4], afxNat cnt, afxReal const in[][4], afxReal out[][4]);

AFXINL void     AfxTransformSerializedV2d(afxReal const m[2][2], afxNat inStride, afxNat outStride, afxNat cnt, afxReal const in[][2], afxReal out[][2]);
AFXINL void     AfxTransformSerializedV3d(afxReal const m[3][3], afxNat inStride, afxNat outStride, afxNat cnt, afxReal const in[][3], afxReal out[][3]);
AFXINL void     AfxTransformSerializedV4d(afxReal const m[4][4], afxNat inStride, afxNat outStride, afxNat cnt, afxReal const in[][4], afxReal out[][4]);

AFXINL void     AfxTransformSerializedPointV3d(afxReal const m[4][4], afxNat inStride, afxNat outStride, afxNat cnt, afxReal const in[][3], afxReal out[][3]);
AFXINL void     AfxTransformSerializedNormalV4d(afxReal const m[3][3], afxNat inStride, afxNat outStride, afxNat cnt, afxReal const in[][4], afxReal out[][4]);
AFXINL void     AfxTransformSerializedAffineV4d(afxReal const m[4][4], afxNat inStride, afxNat outStride, afxNat cnt, afxReal const in[][4], afxReal out[][4]);

// Assimilate

AFXINL void     AfxAssimilateM3d(afxReal const linear[3][3], afxReal const invLinear[3][3], afxNat cnt, afxReal const in[][3][3], afxReal out[][3][3]); // make similarity transformation on afxM3d-based scale/shear.
AFXINL void     AfxAssimilateAffineM4d(afxReal const linear[3][3], afxReal const invLinear[3][3], afxReal const affine[3], afxNat cnt, afxReal const in[][4][4], afxReal out[][4][4]);

#endif//AFX_MATRIX_H