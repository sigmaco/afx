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
    The Qwadro quaternion functions use an 4D vector to represent quaternions, 
    where the X, Y, and Z components are the vector part and the W component is the scalar part.

    afxQuat: A 4-dimensional vector representing a rotation.
    The vector represents a 4 dimensional complex number where multiplication of the basis elements is not commutative(multiplying i with j gives a different result than multiplying j with i).
    Multiplying quaternions reproduces rotation sequences. However quaternions need to be often renormalized, or else they suffer from precision issues.
    It can be used to perform SLERP(spherical - linear interpolation) between two rotations.
*/

#ifndef AFX_QUATERNION_H
#define AFX_QUATERNION_H

#include "qwadro/math/afxVector.h"

typedef enum afxQuatBlend
{
    afxQuatBlend_DIRECT, // mixar como está.
    afxQuatBlend_INV, // mixar entrada como seu produto inverso.
    afxQuatBlend_ADJACENT, // usar adjacencia à rest pose.
    afxQuatBlend_ACCUM_ADJACENT, // usar brevidade
} afxQuatBlend;

AFX afxQuat const AFX_QUAT_ZERO;
AFX afxQuat const AFX_QUAT_IDENTITY;

#define AFX_QUAT(x, y, z, w) (afxQuat const){ x, y, z, w }
#define AFX_QUATERNION(x, y, z, w) (afxQuat const){ x, y, z, w }

////////////////////////////////////////////////////////////////////////////////
// Initialization                                                             //
////////////////////////////////////////////////////////////////////////////////

/// Redefine um quaternion ao estado de unit/identity.
/// Dado um quaternion (x, y, z, w), a função AfxQuatReset vai fazer o quaternion (0, 0, 0, 1).

AFXINL void AfxQuatReset(afxQuat q);

/// Preenche um quaternion com zeros.

AFXINL void AfxQuatZero(afxQuat q);

/// Define cada componente de um quaternion.

AFXINL void AfxQuatSet(afxQuat q, afxReal x, afxReal y, afxReal z, afxReal w);

/// Since rotation quaternions are normalized, there are only 3 degrees of freedom instead of 4. 
/// Thus you only really need to store 3 components of the quaternion, and the 4th can be calculated using the formula
/// We can compress a Quaternion down to three elements by making sure one of the them is greater than or equal to zero. We can then rebuild the missing element with this function.

AFXINL void AfxQuatFromV3d(afxQuat q, afxV3d const in);

/// It is useful to be able to move between the 3x3 matrix representation of a rotation and its quaternion counterpart. 
/// Qwadro provides functions for converting in both directions.

/// Computes a rotation quaternion from a rotation matrix.
/// This function only uses the upper 3x3 portion of the matrix. 
/// Note if the input matrix contains scales, shears, or other non-rotation transformations in the upper 3x3 matrix, then the output of this function is ill-defined.
/// q = Returns the rotation quaternion.
/// m = Rotation matrix.

AFXINL void AfxQuatRotationFromM3d(afxQuat q, afxM3d const m); // aka get matrix rotation
AFXINL void AfxQuatRotationFromM4d(afxQuat q, afxM4d const m);

/// Computes a rotation quaternion about an axis.
/// q = Returns the rotation quaternion.
/// axis = 3D vector describing the axis of rotation.
/// radians = Angle of rotation in radians. Angles are measured clockwise when looking along the rotation axis toward the origin.

AFXINL void AfxQuatRotationFromAxis(afxQuat q, afxV3d const axis, afxReal phi); // makes a quaternion rotation that will rotate around the given axis by the specified angle. The axis must be a normalized vector.

/// Computes a rotation quaternion based on the pitch, yaw, and roll (Euler angles).
/// Angles are measured clockwise when looking along the rotation axis toward the origin. This is a left-handed coordinate system. To use right-handed coordinates, negate all three angles.
/// The order of transformations is roll first, then pitch, then yaw.The rotations are all applied in the global coordinate frame.
/// This function takes x-axis, y-axis, z-axis angles as input parameters. 
/// The assignment of the labels pitch to the x-axis, yaw to the y-axis, and roll to the z-axis is a common one for computer graphics and games as it matches typical 'view' coordinate systems. 
/// There are of course other ways to assign those labels when using other coordinate systems (i.e. roll could be the x-axis, pitch the y-axis, and yaw the z-axis).
/// q = Returns the rotation quaternion.
/// pitchYawRoll = 3D vector containing the Euler angles in the order x-axis (pitch), then y-axis (yaw), and then z-axis (roll).

AFXINL void AfxQuatRotationFromEuler(afxQuat q, afxV3d const pitchYawRoll);

////////////////////////////////////////////////////////////////////////////////

/// Testa se um quaternion é um unit/identity quaternion.
/// q = Quaternion to test.

AFXINL afxBool  AfxQuatIsIdentity(afxQuat const q);

/// Em margem de erro escalar de Epsilon, testa se dois quaternions são equivalentes.
/// q = Quaternion to test.

AFXINL afxBool  AfxQuatIsEqual(afxQuat const q, afxQuat const other);

////////////////////////////////////////////////////////////////////////////////

/// Cambia (operação de troca) os dados entre dois quaternions.

AFXINL void AfxQuatSwap(afxQuat q, afxQuat other);

/// Copia os dados de um outro quaternion.

AFXINL void AfxQuatCopy(afxQuat q, afxQuat const in);

////////////////////////////////////////////////////////////////////////////////

/// Normalizes a quaternion.
/// q = Returns the normalized form of in.
/// in = Quaternion to normalize.

AFXINL afxReal  AfxQuatNormalize(afxQuat q, afxQuat const in);

/// Estimates the normalized version of a quaternion.
/// This estimation offer increased performance at the expense of reduced accuracy. 
/// So, it is appropriate for non-critical calculations where accuracy can be sacrificed for speed. The exact amount of lost accuracy and speed increase are platform dependent.
/// This function internally calls the AfxV4dNormalizeEstimated function.
/// q = An XMVECTOR union that is the estimate of the normalized version of a quaternion.
/// in = A quaternion for which to estimate the normalized version.

AFXINL afxReal  AfxQuatNormalizeEstimated(afxQuat q, afxQuat const in);

////////////////////////////////////////////////////////////////////////////////

AFXINL void AfxQuatAdd(afxQuat q, afxQuat const a, afxQuat const b); // q = a + b

AFXINL void AfxQuatSub(afxQuat q, afxQuat const a, afxQuat const b); // q = a - b

AFXINL void AfxQuatScale(afxQuat q, afxQuat const in, afxReal scalar);  // q = a * s

AFXINL void AfxQuatDiv(afxQuat q, afxQuat const in, afxReal dividend);

/// Computes the product of two quaternions.
/// This multiplication is a quaternion multiplication. The way that quaternion multiplication is defined is one of the things that makes quaternions good at representing rotations.
/// The result represents the rotation A followed by the rotation B to be consistent with AfxMulplityM4d concatenation since this function is typically used to concatenate quaternions that represent rotations (i.e. it returns B*A).
/// q = Returns the product of two quaternions as B*A (which is the concatenation of a rotation A followed by the rotation B)
/// a = First quaternion.
/// b = Second quaternion.

AFXINL void AfxQuatMultiply(afxQuat q, afxQuat const a, afxQuat const b); // q = a * b

// q = add + (mul * f)
AFXINL void AfxQuatMad(afxQuat q, afxQuat const add, afxQuat const mul, afxQuat const f);

// q = add + (mul * lambda)
AFXINL void AfxQuatMads(afxQuat q, afxQuat const add, afxQuat const mul, afxReal lambda);

////////////////////////////////////////////////////////////////////////////////

/// Computes the dot product of two quaternions.
/// q = First quaternion.
/// other = Second quaternion.

AFXINL afxReal  AfxQuatDot(afxQuat const q, afxQuat const other);

/// Computes the square of the magnitude of a quaternion.
/// q = Quaternion to measure.

AFXINL afxReal  AfxQuatSq(afxQuat const q);

/// Computes the magnitude of a quaternion.
/// q = Quaternion to measure.

AFXINL afxReal  AfxQuatMag(afxQuat const q);

/// Computes the reciprocal of the magnitude of a quaternion.
/// q = Returns the reciprocal of the magnitude of in.
/// in = Quaternion to measure.

AFXINL afxReal  AfxQuatMagRecip(afxQuat const q);

/// Negativa todos os componentes de um quaternion.

AFXINL void AfxQuatNeg(afxQuat q, afxQuat const in);

/// Computes the conjugate of a quaternion.
/// Given a quaternion ( x, y, z, w), the AfxQuatConj function returns the quaternion (-x, -y, -z, w).
/// Use the AfxQuatNormalize function for any quaternion input that is not already normalized.
/// q = Returns the conjugate of in.
/// in = The quaternion to conjugate.

AFXINL void AfxQuatConj(afxQuat q, afxQuat const in);

/// Computes the inverse of a quaternion.
/// q = Returns the inverse of in.
/// in = Quaternion to invert.

AFXINL void AfxQuatInv(afxQuat q, afxQuat const in); // q = inverse of in

/// Interpolating a quaternion is useful when smoothly varying between an initial and final rotation. 
/// Interpolation is good for finding arbitrary in-between values of rotation. This is employed especially in character animation systems. 
/// It is possible to interpolate rotation matrices, but the interpolated matrix may not be a size and shape preserving matrix. 
/// Needless to say, interpolating a quaternion is a bajillion times easier than interpolating rotation matrices.

/// There is one interesting property of quaternions that comes into play when dealing with interpolation. 
/// If a rotation can be represented by a quaternion q, then the quaterion - q also represents the same rotation. 
/// Why is that ? I'm not going to explain it right now, other than to say that it is connected to the very fabric of reality. 
/// (You think I'm joking but I'm not!) What you need to worry about is which one of these quaternions you are going to use.

/// To describe the difference between q and -q, consider that you turn a quarter turn to your left. 
/// Esentially this is the same as turning 3 / 4 turn to your right. 
/// One turn is the "short" turn and the other is the "long"one. 
/// When representing a static orientation it is irrelevant if a quaternion represents the short, or long path, because it just sits in the final position and you don't get to see the in-between values. 
/// However, when you are blending it surely does make a difference.

/// When blending between an initial and a final quaternion, there is some ambiguity as to if we are taking the "short" way or the "long" way. 
/// It seems like the right thing to always blend on the shortest path. Given the two input quaternions, it is possible to determine which way we are going to blend. 
/// You can check this by examining the sign of the 4D dot product of the inputs. If the sign is negative, then you know you are going to be blending the long way.

/// So, what do you do if you find out that you are blending the long way? Simply flip the sign on one of your input quaternions.remember q and -q represent the same rotation. 
/// Flipping the sign on one of your inputs will flip the sign of the 4D dot product.

/// Now that we have discussed that little tid-bit, let's move on to interpolation formulas.

/// There are a few different interpolation formulas, but two main ones:
///     NLerp is a linear interpolation of the components that is followed by a normalization of the interpolated quaternion, to ensure that it represents a rotation.
///     Slerp is a spherical interpolation which interpolates in a spherical space, rather than in the cartesian space of the coordinates. 
/// The interpolant of the slerp function moves at a constant motion, while the interpolant of the NLerp has some non-linear acceleration.

/// Heres the quick and dirty: Don't mess around with Slerp, even though you think it might be the more "correct" thing to do. It is too expensive, and has too many special cases that need to be considered. 
/// There are some complicated schemes that try to closely approximate the Slerp function, but it just isn't worth it. Just use NLerp. Especially for computationally strapped code.

AFXINL void AfxQuatLerp(afxQuat q, afxQuat const a, afxQuat const b, afxReal percent);

/// Interpolates between two unit quaternions, using spherical linear interpolation.
/// When t is 0.0f, the function returns A. When t is 1.0f, the function returns B.
/// q = Returns the interpolated quaternion. If A and B are not unit quaternions, the resulting interpolation is undefined.
/// a = Unit quaternion to interpolate from.
/// b = Unit quaternion to interpolate to.
/// t = Interpolation control factor.

AFXINL void AfxQuatSlerp(afxQuat q, afxQuat const a, afxQuat const b, afxReal t);

////////////////////////////////////////////////////////////////////////////////

/// Computes an axis and angle of rotation about that axis for a given quaternion.
/// q = Quaternion to measure.
/// axis = Address of a 3D vector describing the axis of rotation for the quaternion Q.
/// radians = Address of float describing the radian angle of rotation for the quaternion Q.

AFXINL void AfxQuatExtractAxialRotation(afxQuat const q, afxV3d axis, afxReal *radians); // extracts an axis/angle representation to this quaternion rotation.

AFXINL void AfxAssimilateQuat(afxM3d const ltm, afxM3d const iltm, afxUnit cnt, afxQuat const in[], afxQuat out[]); // make similarity transformation on afxQuat-based orientation.

/// Rotates a 3D vector using a quaternion.
/// v = Returns the rotated 3D vector.
/// in = 3D vector to rotate.
/// rot = Quaternion that describes the rotation to apply to the vector.

AFXINL void AfxQuatRotateV3dArray(afxQuat const q, afxUnit cnt, afxV3d const in[], afxV3d out[]);

////////////////////////////////////////////////////////////////////////////////

/// How to find a rotation which takes some initial vector into some final vector?
/// There are several variants of this problem, including "I am pointing my gun at Bob, and I would like to point my gun at Jim." 
/// I have seen this problem solved in many different ways, many times involving inverse trig functions. We are going to solve this problem with quaternions - and it's really easy!
/// Here is a function that will give you the rotation quaternion that will rotate some initial vector into some final vector

AFXINL void AfxQuatHead(afxQuat q, afxV3d const from, afxV3d const to);

/// Returns a point in barycentric coordinates, using the specified quaternions. Note that Barycentric coordinates work for 'flat' surfaces but not for 'curved' ones.
/// a = First quaternion in the triangle.
/// b = Second quaternion in the triangle.
/// c = Third quaternion in the triangle.
/// f = Weighting factor.
/// g = Weighting factor.

AFXINL void AfxQuatBarycentric(afxQuat q, afxQuat const a, afxQuat const b, afxQuat const c, afxReal f, afxReal g);

AFXINL void AfxQuatFromTangentM3d(afxQuat q, afxM3d const tbn);

AFXINL void AfxQuatFromTangentFrame(afxQuat q, afxV3d const normal, afxV3d const tangent, afxV3d const bitangent);

AFXINL void AfxQuatFromAngularVelocity(afxQuat q, afxV3d const vel);

/// Updating the dynamical state of a rigid body is referred to as integration. 
/// If you represent the orientation of this body with a quaternion, you will need to know how to update it. 
/// Here is a sample function for integrating a quaternion with a given angular velocity and time step.

AFXINL void AfxQuatIntegrate(afxQuat q, afxQuat const in, afxV3d const omega, afxReal dt);

/*
    The AfxQuatIntegrateEULER function for first-order (explicit Euler) quaternion integration.
*/

AFXINL void AfxQuatIntegrateEULER(afxQuat q, afxQuat const in, afxV3d const omega, afxReal dt);

////////////////////////////////////////////////////////////////////////////////

/// Qwadro uses quaternions to represent any decomposed rotational quantity. 
/// On occasion, if you are doing some very low-level animation work, 
/// you may find yourself working with an array of quaternions from Qwadro (such as those that come out of the b-spline buffer construction routines). 
/// For interpolation purposes, it is important that all neighboring quaternions be "as close as possible" to eachother. 
/// Since all single-spin rotations can be represented by one of two different quaternions, 
/// this amounts to running through the quaternion buffer and selecting the sequence of quaternions that minimizes the distance between each successive quaternion while keep the rotational meaning the same. 
/// Qwadro makes it simple to do this:

AFXINL void AfxEnsureQuaternionContinuity(afxUnit cnt, afxQuat q[]);

/// Suffice to say, you shouldn't normally be calling AfxEnsureQuaternionContinuity on anything unless you know you need to, 
/// because any time you're using Qwadro's higher-level animation functions, 
/// it makes sure that all quaternion operations happen correctly. 
/// AfxEnsureQuaternionContinuity is only for situations where you are manually performing some animation operation.

#endif//AFX_QUATERNION_H
