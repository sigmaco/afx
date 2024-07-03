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

// This code is part of SIGMA Foundation Math <https://sigmaco.org/math>

/// RenderWare uses 4x3, row-major affine matrices.
/// This object defines a RenderWare Matrix object.
/// The Matrix is heavily used throughout the API and a full range of functions are provided, including: rotation, multiplication, concatenation, scaling, translation, creation, destruction, stream read/write functions and a number of access functions to access particular vector data within the matrix.
/// The "Right vector" is one of the vectors in a RenderWare Graphics matrix. It corresponds to the 'x' axis in a right-handed, xyz coordinate system. 
/// The "Up vector" is one of the vectors in a RenderWare Graphics matrix. The up vector corresponds to the 'y' axis in a right-handed, xyz coordinate system. 
/// The "at vector" is one of the vectors in a RenderWare Graphics matrix. It corresponds to the 'z' axis in a right-handed, xyz coordinate system. 
/// RenderWare Graphics uses a right-handed co-ordinate system. Therefore with z increasing into the display screen, and y increasing upwards, the x co-ordinate system increases towards the left.
/// RenderWare, such as DirectX, uses row vectors and ends up with the much more natural: result = input * local_to_object * object_to_world. Your input is in local space, it gets transformed into object space before finally ending up in world space. Clean, clear, and readable. If you instead multiply the two matrices together on their own, you get the clear local_to_world = local_to_object * object_to_world instead of the awkward local_to_world = object_to_world * local_to_object you would get with OpenGL shaders and column vectors.

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

#include "qwadro/math/afxVector.h"
#include "qwadro/math/afxQuaternion.h"
#include "qwadro/math/afxPlane.h"

AFX afxM2d const AFX_M2D_ZERO;
AFX afxM3d const AFX_M3D_ZERO;
AFX afxM4d const AFX_M4D_ZERO;

AFX afxM2d const AFX_M2D_IDENTITY;
AFX afxM3d const AFX_M3D_IDENTITY;
AFX afxM4d const AFX_M4D_IDENTITY;

AFXINL void     AfxZeroM2d(afxM2d m);
AFXINL void     AfxZeroM3d(afxM3d m);
AFXINL void     AfxZeroM4d(afxM4d m);

AFXINL void     AfxResetM2d(afxM2d m);
AFXINL void     AfxResetM3d(afxM3d m);
AFXINL void     AfxResetM4d(afxM4d m);

AFXINL void     AfxSetM2d(afxM2d m, afxV2d const x, afxV2d const y);
AFXINL void     AfxSetM3d(afxM3d m, afxV3d const x, afxV3d const y, afxV3d const z);
AFXINL void     AfxSetM4d(afxM4d m, afxV4d const x, afxV4d const y, afxV4d const z, afxV4d const w);
AFXINL void     AfxSetLtm4d(afxM4d m, afxV3d const x, afxV3d const y, afxV3d const z, afxV3d const w);
AFXINL void     AfxSetAtm4d(afxM4d m, afxV3d const x, afxV3d const y, afxV3d const z, afxV3d const w);

AFXINL void     AfxSetM2dTransposed(afxM2d m, afxV2d const x, afxV2d const y); // Be carefull using it in Qwadro
AFXINL void     AfxSetM3dTransposed(afxM3d m, afxV3d const x, afxV3d const y, afxV3d const z); // Be carefull using it in Qwadro
AFXINL void     AfxSetM4dTransposed(afxM4d m, afxV4d const x, afxV4d const y, afxV4d const z, afxV4d const w);  // Be carefull using it in Qwadro

AFXINL void     AfxEnsureLtm4d(afxM4d m); // make affine and zero translation.
AFXINL void     AfxEnsureAtm4d(afxM4d m); // make affine.

AFXINL afxBool  AfxM2dIsIdentity(afxM2d const m);
AFXINL afxBool  AfxM3dIsIdentity(afxM3d const m);
AFXINL afxBool  AfxM4dIsIdentity(afxM4d const m);

AFXINL void     AfxCopyM2d(afxM2d m, afxM2d const in);
AFXINL void     AfxCopyM3d(afxM3d m, afxM3d const in);
AFXINL void     AfxCopyM4d(afxM4d m, afxM4d const in); // copy every row and column.

AFXINL void     AfxPickM2d(afxM3d const m, afxM2d sub);
AFXINL void     AfxPickM2d2(afxM4d const m, afxM2d sup, afxM2d inf);

AFXINL void     AfxClipM3d(afxM3d ltm, afxM4d const m);
AFXINL void     AfxClipM3dTransposed(afxM3d lttm, afxM4d const m); // ltm will be transposed

AFXINL void     AfxClipLtm4d(afxM4d ltm, afxM4d const m); // copy only the 3x3
AFXINL void     AfxClipLtm4dTransposed(afxM4d lttm, afxM4d const m); // only consider 3x3; ignore W components and the W row.

AFXINL void     AfxClipAtm4d(afxM4d atm, afxM4d const m); // copy only the 4x3
AFXINL void     AfxClipAtm4dTransposed(afxM4d attm, afxM4d const m); // only consider 4x3; ignore W components.

AFXINL void     AfxMakeM3dFromM2d(afxM3d m, afxM2d const in);
AFXINL void     AfxMakeM4dFromM2d(afxM4d m, afxM2d const in);
AFXINL void     AfxMakeM4dFromM3d(afxM4d m, afxM3d const in, afxV4d const translation);
AFXINL void     AfxMakeM4dFromM3dTransposed(afxM4d m, afxM3d const in, afxV4d const translation);

AFXINL void     AfxSwapM2d(afxM2d m, afxM2d other);
AFXINL void     AfxSwapM3d(afxM3d m, afxM3d other);
AFXINL void     AfxSwapM4d(afxM4d m, afxM4d other);

AFXINL void     AfxTransposeM2d(afxM2d const m, afxM2d t);
AFXINL void     AfxTransposeM3d(afxM3d const m, afxM3d t);
AFXINL void     AfxTransposeM4d(afxM4d const m, afxM4d t);

AFXINL void     AfxAddM2d(afxM2d m, afxM2d const a, afxM2d const b); // m = a + b
AFXINL void     AfxAddM3d(afxM3d m, afxM3d const a, afxM3d const b); // m = a + b
AFXINL void     AfxAddM4d(afxM4d m, afxM4d const a, afxM4d const b); // m = a + b

AFXINL void     AfxSubM2d(afxM2d m, afxM2d const a, afxM2d const b); // m = a - b
AFXINL void     AfxSubM3d(afxM3d m, afxM3d const a, afxM3d const b); // m = a - b
AFXINL void     AfxSubM4d(afxM4d m, afxM4d const a, afxM4d const b); // m = a - b

AFXINL void     AfxScaleM2d(afxReal scale, afxM2d const in, afxM2d m);
AFXINL void     AfxScaleM3d(afxReal scale, afxM3d const in, afxM3d m);
AFXINL void     AfxScaleM4d(afxReal scale, afxM4d const in, afxM4d m);

AFXINL void     AfxAddScaledM2d(afxM2d m, afxM2d const a, afxM2d const b, afxReal scale);
AFXINL void     AfxAddScaledM3d(afxM3d m, afxM3d const a, afxM3d const b, afxReal scale);
AFXINL void     AfxAddScaledM4d(afxM4d m, afxM4d const a, afxM4d const b, afxReal scale);

AFXINL void     AfxCombineM2d(afxM2d m, afxReal lambda1, afxM2d const a, afxReal lambda2, afxM2d const b);
AFXINL void     AfxCombineM3d(afxM3d m, afxReal lambda1, afxM3d const a, afxReal lambda2, afxM3d const b);
AFXINL void     AfxCombineLtm4d(afxM4d m, afxReal lambda1, afxM4d const a, afxReal lambda2, afxM4d const b);

AFXINL afxReal  AfxInvertM3d(afxM3d const m, afxM3d im); // im = inverse of m
AFXINL afxReal  AfxInvertM4d(afxM4d const m, afxM4d im); // im = inverse of m
AFXINL afxReal  AfxInvertAtm4d(afxM4d const m, afxM4d im); // im = inverse of m

AFXINL afxReal  AfxDetM2d(afxM2d const m);
AFXINL afxReal  AfxDetM3d(afxM3d const m);
AFXINL afxReal  AfxDetM4d(afxM4d const m);

////////////////////////////////////////////////////////////////////////////////
// Affine transformation matrix methods                                       //
////////////////////////////////////////////////////////////////////////////////

// Scale

AFXINL void     AfxScalingM3d(afxM3d m, afxV3d const scale);
AFXINL void     AfxScalingM4d(afxM4d m, afxV3d const scale);

// Rotate and Orientation

/// Rotation matrices are neither left-handed nor right-handed. The are universal for any handedness. – Nico Schertler

/// It is useful to be able to move between the 3x3 matrix representation of a rotation and its quaternion counterpart. 
/// Qwadro provides functions for converting in both directions: 

AFXINL void     AfxRotationM3dFromQuat(afxM3d m, afxQuat const q);
AFXINL void     AfxRotationM4dFromQuat(afxM4d m, afxQuat const q);

AFXINL void     AfxRotationM3dFromAxis(afxM3d m, afxV3d const axis, afxReal /*theta*/radians);
AFXINL void     AfxRotationM4dFromAxis(afxM4d m, afxV3d const axis, afxReal /*theta*/radians);
AFXINL void     AfxRotationM4dFromEuler(afxM4d m, afxV3d const pitchYawRoll);

AFXINL void     AfxRotationM4dFromX(afxM4d m, afxReal angle);
AFXINL void     AfxRotationM4dFromY(afxM4d m, afxReal angle);
AFXINL void     AfxRotationM4dFromZ(afxM4d m, afxReal angle);

// Translate and Position

AFXINL void     AfxTranslationM4d(afxM4d m, afxV3d const translation);

// PolarDecompose

/// Qwadro expects all animation tracks to be decomposed. 
/// Position, orientation, and scale/shear are stored separately. 
/// The exporters kick out data in this format, but if you're writing your own important or processing tool, you may only have matrices as input. 
/// Decomposing an arbitrary 3x3 matrix into its rotational and scale/shear components can be difficult, so Qwadro provides a function for doing this: 

AFXINL afxBool  AfxPolarDecomposeM3d(afxM3d const m, afxReal tol, afxM3d rm, afxM3d ssm);

// Compose

AFXINL void     AfxComposeM4d(afxM4d m, afxV3d const scalOrigin, afxQuat const scalOrient, afxV3d const scaling, afxV3d const rotOrigin, afxQuat const rotQuat, afxV3d const translation);
AFXINL void     AfxComposeAtm4d(afxM4d m, afxV3d const scale, afxV3d const rotAxis, afxQuat const rot, afxV3d const translation);

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

AFXINL void     AfxMultiplyM2d(afxM2d m, afxM2d const in, afxM2d const by);
AFXINL void     AfxMultiplyM3d(afxM3d m, afxM3d const in, afxM3d const by);
AFXINL void     AfxMultiplyM4d(afxM4d m, afxM4d const in, afxM4d const by);

AFXINL void     AfxMultiplyAtm3d(afxM3d m, afxM3d const in, afxM3d const by); // 2x2 subset only
AFXINL void     AfxMultiplyLtm4d(afxM4d m, afxM4d const in, afxM4d const by); // 3x3 subset only
AFXINL void     AfxMultiplyAtm4d(afxM4d m, afxM4d const in, afxM4d const by); // 4x3 subset only (aka RenderWare (RwMatrix) matrix)

AFXINL void     TransposeMatrixMultiply3x3(afxM3d IntoMatrix, afxM3d const TransposedMatrix, afxM3d const ByMatrix);

// Computes the transpose of the product of two matrices.

AFXINL void     AfxMultiplyM2dTransposed(afxM2d m, afxM2d const in, afxM2d const by);
AFXINL void     AfxMultiplyM3dTransposed(afxM3d m, afxM3d const in, afxM3d const by);
AFXINL void     AfxMultiplyM4dTransposed(afxM4d m, afxM4d const in, afxM4d const by);

AFXINL void     AfxMultiplyAtm3dTransposed(afxM3d m, afxM3d const in, afxM3d const by); // 2x2 subset only
AFXINL void     AfxMultiplyLtm4dTransposed(afxM4d m, afxM4d const in, afxM4d const by); // 3x3 subset only of an affine transformation matrix
AFXINL void     AfxMultiplyAtm4dTransposed(afxM4d m, afxM4d const in, afxM4d const by); // 4x3 subset only (aka RenderWare (RwMatrix) matrix)

// Matrix vs Vector

AFXINL void     AfxPostMultiplyArrayedV2d(afxM2d const m, afxNat cnt, afxV2d const in[], afxV2d out[]);
AFXINL void     AfxPostMultiplyArrayedV3d(afxM3d const m, afxNat cnt, afxV3d const in[], afxV3d out[]);
AFXINL void     AfxPostMultiplyArrayedV4d(afxM4d const m, afxNat cnt, afxV4d const in[], afxV4d out[]);

AFXINL void     AfxPostMultiplyArrayedAtv3d(afxM4d const m, afxNat cnt, afxV3d const in[], afxV3d out[]);
AFXINL void     AfxPostMultiplyArrayedLtv4d(afxM3d const m, afxNat cnt, afxV4d const in[], afxV4d out[]);
AFXINL void     AfxPostMultiplyArrayedAtv4d(afxM4d const m, afxNat cnt, afxV4d const in[], afxV4d out[]);

AFXINL void     AfxPreMultiplyArrayedV2d(afxM2d const m, afxNat cnt, afxV2d const in[], afxV2d out[]);
AFXINL void     AfxPreMultiplyArrayedV3d(afxM3d const m, afxNat cnt, afxV3d const in[], afxV3d out[]);
AFXINL void     AfxPreMultiplyArrayedV4d(afxM4d const m, afxNat cnt, afxV4d const in[], afxV4d out[]);

AFXINL void     AfxPreMultiplyArrayedAtv3d(afxM4d const m, afxNat cnt, afxV3d const in[], afxV3d out[]);
AFXINL void     AfxPreMultiplyArrayedLtv4d(afxM3d const m, afxNat cnt, afxV4d const in[], afxV4d out[]);
AFXINL void     AfxPreMultiplyArrayedAtv4d(afxM4d const m, afxNat cnt, afxV4d const in[], afxV4d out[]);

AFXINL void     AfxPostMultiplySerializedV2d(afxM2d const m, afxNat inStride, afxNat outStride, afxNat cnt, afxV2d const in[], afxV2d out[]);
AFXINL void     AfxPostMultiplySerializedV3d(afxM3d const m, afxNat inStride, afxNat outStride, afxNat cnt, afxV3d const in[], afxV3d out[]);
AFXINL void     AfxPostMultiplySerializedV4d(afxM4d const m, afxNat inStride, afxNat outStride, afxNat cnt, afxV4d const in[], afxV4d out[]);

AFXINL void     AfxPostMultiplySerializedAtv3d(afxM4d const m, afxNat inStride, afxNat outStride, afxNat cnt, afxV3d const in[], afxV3d out[]);
AFXINL void     AfxPostMultiplySerializedLtv4d(afxM3d const m, afxNat inStride, afxNat outStride, afxNat cnt, afxV4d const in[], afxV4d out[]);
AFXINL void     AfxPostMultiplySerializedAtv4d(afxM4d const m, afxNat inStride, afxNat outStride, afxNat cnt, afxV4d const in[], afxV4d out[]);

// Assimilate

AFXINL void     AfxAssimilateLtm3d(afxM3d const ltm, afxM3d const iltm, afxNat cnt, afxM3d const in[], afxM3d out[]); // make similarity transformation on afxM3d-based scale/shear.
AFXINL void     AfxAssimilateAtm4d(afxM3d const ltm, afxM3d const iltm, afxV4d const atv, afxNat cnt, afxM4d const in[], afxM4d out[]);

AFX void        AfxApplyRootMotionVectorsToMatrix(afxV3d const translation, afxV3d const rotation, afxM4d const mm, afxM4d m);

#endif//AFX_MATRIX_H
