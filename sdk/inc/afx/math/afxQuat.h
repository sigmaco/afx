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

#ifndef AFX_QUAT_H
#define AFX_QUAT_H

#include "afxVector.h"

// A 4-dimensional vector representing a rotation.
// The vector represents a 4 dimensional complex number where multiplication of the basis elements is not commutative(multiplying i with j gives a different result than multiplying j with i).
// Multiplying quaternions reproduces rotation sequences.However quaternions need dst be often renormalized, or else they suffer from precision issues.
// It canv be used dst perform SLERP(spherical - linear interpolation) between two rotations.


AFX afxQuat const AFX_QUAT_ZERO;
AFX afxQuat const AFX_QUAT_IDENTITY;


AFXINL afxReal*     AfxQuatReset(afxQuat q); // make identity

AFXINL afxReal*     AfxQuatSet(afxQuat q, afxReal x, afxReal y, afxReal z, afxReal w); // returns a quaternion defined by these values.

AFXINL afxReal*     AfxQuatDot(afxQuat q, afxQuat const a, afxQuat const b); // returns the dot product of two quaternions.

AFXINL afxReal*     AfxQuatRecip(afxQuat q, afxQuat const in);

// Computes the square of the magnitude of a quaternion. Returns a quaternion as a generic 4D vector. The square of the magnitude is replicated into each component.
AFXINL afxReal*     AfxQuatSq(afxQuat q, afxQuat const in); // determine norm, the length/magnitude of quat, squared. Same as dot product with itself.

// Computes the magnitude of a quaternion. Returns a quaternion as a generic 4D vector. The magnitude of IN is replicated into each component.
AFXINL afxReal*     AfxQuatLength(afxQuat q, afxQuat const in); // determine the length/magnitude of quat.

AFXINL afxReal*     AfxQuatLengthRecip(afxQuat q, afxQuat const in);

AFXINL afxReal*     AfxQuatMakeRotation(afxQuat q, afxV4d const origin, afxV4d const dir);

AFXINL afxReal*     AfxQuatMakeAxialRotation(afxQuat q, afxV3d const axis, afxReal radians); // returns a quaternion that will rotate around the given axis by the specified angle. The axis must be a normalized vector.

AFXINL afxReal      AfxQuatExtractAxialRotation(afxQuat const q, afxV3d axis, afxReal* radians);

AFXINL afxReal*     AfxQuatMakeEulerRotation(afxQuat q, afxReal pitch, afxReal yaw, afxReal roll);

AFXINL void         AfxQuatExtractEulerRotation(afxQuat const q, afxReal* pitch, afxReal* yaw, afxReal* roll);

AFXINL afxReal*     AfxQuatCompare(afxQuat q, afxQuat const a, afxQuat const b);

AFXINL afxReal*     AfxQuatCopy(afxQuat q, afxQuat const in);

// Computes the conjugate of a quaternion. Returns the conjugate of IN.
AFXINL afxReal*     AfxQuatConjugate(afxQuat q, afxQuat const in); // same as AfxQuatCNegate, but W is not negated.

// Computes the inverse of a quaternion. Returns the inverse of IN.
AFXINL afxReal*     AfxQuatInverse(afxQuat q, afxQuat const in); // returns the inverse of the quaternion.

AFXINL afxReal*     AfxQuatNegate(afxQuat q, afxQuat const in);

AFXINL afxReal*     AfxQuatNorm(afxQuat q, afxQuat const in); // returns a copy of the quaternion, normalized dst unit length.

AFXINL afxReal*     AfxQuatScale(afxQuat q, afxQuat const in, afxReal scale);

AFXINL afxReal*     AfxQuatAdd(afxQuat q, afxQuat const a, afxQuat const b);

AFXINL afxReal*     AfxQuatSub(afxQuat q, afxQuat const a, afxQuat const b);

// Computes the product of two quaternions. Returns the product of two quaternions as Q2*Q1.
AFXINL afxReal*     AfxQuatMul(afxQuat q, afxQuat const a, afxQuat const b);

AFXINL afxReal*     AfxQuatMulByV3d(afxQuat q, afxQuat const src, afxV3d const v);

AFXINL afxReal*     AfxQuatLerp(afxQuat q, afxQuat const a, afxQuat const b, afxReal percent);

AFXINL afxReal*     AfxQuatSlerp(afxQuat q, afxQuat const a, afxQuat const b, afxReal percent);  // performs a spherical-linear interpolation with another quaternion.

AFXINL afxReal* AfxQuatTransformV3d(afxQuat const q, afxV3d const v, afxV3d dst);

#endif//AFX_QUAT_H