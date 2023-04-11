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

#ifndef AFX_M4D_H
#define AFX_M4D_H

#include "afxQuat.h"
#include "afxVector.h"
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


AFX afxM2d const AFX_M2D_ZERO;
AFX afxM3d const AFX_M3D_ZERO;
AFX afxM4d const AFX_M4D_ZERO;

AFX afxM2d const AFX_M2D_IDENTITY;
AFX afxM3d const AFX_M3D_IDENTITY;
AFX afxM4d const AFX_M4D_IDENTITY;

/* Funções sem parâmetros
 * Funções com parâmetros scalar
 * Funções com parâmetros vector
 * Funções com parâmetros matrix
 */

// Zeroes all elements of a matrix.
AFXINL afxV2d*          AfxM2dZero(afxM2d m);
AFXINL afxV3d*          AfxM3dZero(afxM3d m);
AFXINL afxV4d*          AfxM4dZero(afxM4d m);

// Resets elements of a matrix to identity form.
AFXINL afxV2d*          AfxM2dReset(afxM2d m);
AFXINL afxV3d*          AfxM3dReset(afxM3d m);
AFXINL afxV4d*          AfxM4dReset(afxM4d m);

// Produces the sum of all diagonal elements of a matrix.
AFXINL afxReal          AfxM2dGetTrace(afxM2d const m);
AFXINL afxReal          AfxM3dGetTrace(afxM3d const m);
AFXINL afxReal          AfxM4dGetTrace(afxM4d const m);

// Calculates the determinant value of a matrix.
AFXINL afxReal          AfxM3dGetDeterminant(afxM3d const m);
AFXINL afxReal          AfxM4dGetDeterminant(afxM4d const m); // used to find the determinant value to the specified transformation matrix.

// Sets all diagonal elements of a matrix with a single scalar value.
AFXINL afxV2d*          AfxM2dSetTrace(afxM2d m, afxReal diag);
AFXINL afxV3d*          AfxM3dSetTrace(afxM3d m, afxReal diag);
AFXINL afxV4d*          AfxM4dSetTrace(afxM4d m, afxReal diag); // used to set a value diagonally to [0][0], [1][1], [2][2], [3][3].

// Copies out the right/side (X) axis of a matrix.
AFXINL afxReal*         AfxM2dGetLookRight(afxM2d const m, afxV2d right);
AFXINL afxReal*         AfxM3dGetLookRight(afxM3d const m, afxV3d right);
AFXINL afxReal*         AfxM4dGetLookRight(afxM4d const m, afxV4d right); // used to retrieve the 'look-right' vector of the specified transformation matrix.

// Copies out the up (Y) axis of a matrix.
AFXINL afxReal*         AfxM2dGetLookUp(afxM2d const m, afxV2d up);
AFXINL afxReal*         AfxM3dGetLookUp(afxM3d const m, afxV3d up);
AFXINL afxReal*         AfxM4dGetLookUp(afxM4d const m, afxV4d up); // used to retrieve the 'look-up' vector of the specified transformation matrix.

// Copies out the at/forward (Z) axis of a matrix.
AFXINL afxReal*         AfxM3dGetLookAt(afxM3d const m, afxV3d fwd);
AFXINL afxReal*         AfxM4dGetLookAt(afxM4d const m, afxV4d fwd); // used to retrieve the 'look-at' vector of the specified transformation matrix.

// Copies out the origin of a matrix.
AFXINL afxReal*         AfxM4dGetTranslation(afxM4d const m, afxV4d pos); // used to retrieve the 'position' vector from the specified transformation matrix.

// Copies the data from a matrix into another.
AFXINL afxV2d*          AfxM2dCopy(afxM2d m, afxM2d const in);
AFXINL afxV3d*          AfxM3dCopy(afxM3d m, afxM3d const in);
AFXINL afxV4d*          AfxM4dCopy(afxM4d m, afxM4d const in); // used to copy the contents of one matrix to another.

// Initializes a identity matrix reusing data from another matrix of different size.
AFXINL afxV3d*          AfxM3dSetM2d(afxM3d m, afxM2d const in); // make a afxM3d using [2][2] data from a afxM2d
AFXINL afxV4d*          AfxM4dSetM2d(afxM4d m, afxM2d const in); // make a afxM4d from a afxM2d

AFXINL afxV2d*          AfxM2dSetM3d(afxM2d m, afxM3d const in);
AFXINL afxV4d*          AfxM4dSetM3d(afxM4d m, afxM3d const in); // make a afxM4d from a afxM3d

AFXINL afxV2d*          AfxM2dSetM4d(afxM2d m, afxM4d const in); // extracts [2][2] data from afxM4d to a afxM2d
AFXINL afxV3d*          AfxM3dSetM4d(afxM3d m, afxM4d const in); // extracts [3][3] data from afxM4d to a afxM3d

// Just copies data of elements existing in source matrix, without touching on other elements.
AFXINL afxV3d*          AfxM3dCopyM2d(afxM3d m, afxM2d const in);
AFXINL afxV4d*          AfxM4dCopyM2d(afxM4d m, afxM2d const in);

AFXINL afxV4d*          AfxM4dCopyM3d(afxM4d m, afxM3d const in);

// Make a inverted copy from a matrix.
AFXINL afxV3d*          AfxM3dMakeInverse(afxM3d m, afxM3d const in);
AFXINL afxReal          AfxM4dMakeInverse(afxM4d m, afxM4d const in); // performs a matrix inversion calculation on the input matrix. Returns the determinant value.

// Make a transposed copy from a matrix.
AFXINL afxV2d*          AfxM2dMakeTranspose(afxM2d m, afxM2d const in);
AFXINL afxV3d*          AfxM3dMakeTranspose(afxM3d m, afxM3d const in);
AFXINL afxV4d*          AfxM4dMakeTranspose(afxM4d m, afxM4d const in); // used to make a inversion matrix from input and copy out.

AFXINL afxV3d*          AfxM3dMakeAdjugate(afxM3d m, afxM3d const in);

AFXINL afxV3d*          AfxM3dMakeRotation(afxM3d m, afxQuat const q);
AFXINL afxV4d*          AfxM4dMakeRotation(afxM4d m, afxQuat const q);

AFXINL afxV4d*          AfxM4dMakeAxialRotation(afxM4d m, afxV3d const axis, afxReal radians);

AFXINL afxV4d*          AfxM4dAddAxialRotation(afxM4d m, afxV3d const axis, afxReal radians); // builds a rotation matrix from the given axis and angle of rotation and applies it to the specified matrix.

AFXINL afxReal*         AfxM4dGetRotation(afxM4d const m, afxQuat rot); // used to retrieve the rotation quaternion from the specified transformation matrix.

AFXINL afxV3d*          AfxM3dMakeScaling(afxM3d m, afxV3d const scale);
AFXINL afxV4d*          AfxM4dMakeScaling(afxM4d m, afxV3d const scale);

AFXINL afxV3d*          AfxM3dAddScale(afxM3d m, afxV3d const scale); // builds a scaling matrix from the given scale factors and applies it to the specified matrix in place.
AFXINL afxV4d*          AfxM4dAddScale(afxM4d m, afxV3d const scale); // builds a scaling matrix from the given scale factors and applies it to the specified matrix in place.

AFXINL afxV4d*          AfxM4dMakeTranslation(afxM4d m, afxV4d const pos);

AFXINL afxV4d*          AfxM4dAddTranslation(afxM4d m, afxV4d const pos); // builds a translation matrix from the given vector and applies it to the specified matrix.

AFXINL afxV4d*          AfxM4dCompose(afxM4d m, afxV4d const scale, afxQuat const rotation, afxV4d const translation);

AFXINL void             AfxM4dDecompose(afxM4d const m, afxV4d scale, afxQuat rotation, afxV4d translation);

// Make a combination of two matrix into another.
AFXINL afxV2d*          AfxM2dMultiply(afxM2d m, afxM2d const a, afxM2d const b);
AFXINL afxV3d*          AfxM3dMultiply(afxM3d m, afxM3d const a, afxM3d const b);
AFXINL afxV4d*          AfxM4dMultiply(afxM4d m, afxM4d const a, afxM4d const b); // multiplies two matrices in the order given. The matrix used for the result must be different from the input matrices.
// m = dst, a = new/last, b = base/prev; exemplo: AfxM4dMultiply(w, m, parent->w);

// VECTOR TRANSFORMATIONS

AFXINL afxReal*         AfxM2dTransformV2d(afxM2d const m, afxV2d const v, afxV2d dst);
AFXINL afxReal*         AfxM4dTransformV2d(afxM4d const m, afxV2d const v, afxV2d dst);

AFXINL afxReal*         AfxM3dTransformV3d(afxM3d const m, afxV3d const v, afxV3d dst);
AFXINL afxReal*         AfxM4dTransformV3d(afxM4d const m, afxV3d const v, afxV3d dst);

AFXINL afxReal*         AfxM4dTransformV4d(afxM4d const m, afxV4d const v, afxV4d dst);

AFXINL afxV2d*          AfxM2dTransformV2dArray(afxM2d const m, afxNat cnt, afxV2d const v[], afxV2d dst[]);

AFXINL afxV3d*          AfxM3dTransformV3dArray(afxM3d const m, afxNat cnt, afxV3d const v[], afxV3d dst[]);
AFXINL afxV3d*          AfxM4dTransformV3dArray(afxM4d const m, afxNat cnt, afxV3d const v[], afxV3d dst[]);

AFXINL afxV4d*          AfxM4dTransformV4dArray(afxM4d const m, afxNat cnt, afxV4d const v[], afxV4d dst[]);

// PROJECTIVE TRANSFORMATIONS
// Z is forward, X is side and Y is up.

AFXINL afxV4d*          AfxM4dMakeOrthographic(afxM4d m, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far);

// Builds a left/right-handed orthographic projection matrix.
AFXINL afxV4d*          AfxM4dMakeOrthographic2LH(afxM4d m, afxV2d extent, afxReal near, afxReal far);
AFXINL afxV4d*          AfxM4dMakeOrthographic2RH(afxM4d m, afxV2d extent, afxReal near, afxReal far);

// Builds a customized, left/right-handed orthographic projection matrix.
AFXINL afxV4d*          AfxM4dMakeOrthographicOffCenterLH(afxM4d m, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far);
AFXINL afxV4d*          AfxM4dMakeOrthographicOffCenterRH(afxM4d m, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far);

AFXINL afxV4d*          AfxM4dMakeFrustum(afxM4d m, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far);
AFXINL afxV4d*          AfxM4dMakePerspective(afxM4d m, afxReal fovy, afxReal aspectRatio, afxReal near, afxReal far);

// Builds a left/right-handed perspective projection matrix.
AFXINL afxV4d*          AfxM4dMakePerspective2LH(afxM4d m, afxV2d extent, afxReal near, afxReal far);
AFXINL afxV4d*          AfxM4dMakePerspective2LH(afxM4d m, afxV2d extent, afxReal near, afxReal far);

AFXINL afxV4d*          AfxM4dMakePerspectiveOffCenterLH(afxM4d m, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far);
AFXINL afxV4d*          AfxM4dMakePerspectiveOffCenterRH(afxM4d m, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far);

AFXINL afxV4d*          AfxM4dMakeRwProjection(afxM4d proj, afxV2d offset, afxV2d extent, afxReal nearPlane, afxReal farPlane, afxBool perspective); // viewport offset and extent
AFXINL afxV4d*          AfxM4dMakeRwView(afxM4d view, afxM4d cam);

AFXINL afxV4d*          AfxM4dMakeLookAt(afxM4d m, afxV3d const eye, afxV3d const point, afxV3d const up);

// Builds a left/right-handed, look-at matrix.
AFXINL afxV4d*          AfxM4dMakeLookAt2LH(afxM4d m, afxV3d const eye, afxV3d const point, afxV3d const up);
AFXINL afxV4d*          AfxM4dMakeLookAt2RH(afxM4d m, afxV3d const eye, afxV3d const point, afxV3d const up);

AFXINL afxV4d*          AfxM4dMakeLookTo(afxM4d m, afxV3d const eye, afxV3d const dir, afxV3d const up);

#endif//AFX_M4D_H