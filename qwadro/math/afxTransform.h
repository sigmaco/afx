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
    Qwadro performs all bone animation on decomposed transforms, where "decomposed" means that the position, 
    orientation, and scale/shear components have been pulled out into separately animating quantities. 
    This allows for fair, efficient animation and interpolation, whereas keeping everything as a tangled 4x4 matrix does not. 

    The afxTransform is the primary structure used to store these decomposed transforms. 
    Each structure stores a 4-element position vector, a 4-element quaternion orientation, and a 3x3 scale/shear matrix 
    (note that it may also contain mirroring). 
    Additionally, each transform stores a set of afxTransformFlags that indicates which, if any, of these values current 
    has a non-identity value. These flags are solely used to speed up computation.

    When using transformation matrices we usually use the terms "rotation" and "translation". 
    When using a pose we use the terms "orientation" and "position". 
    But that’s just a subtle difference – it pretty much means the same.
*/

#ifndef AFX_TRANSFORM_H
#define AFX_TRANSFORM_H

#include "qwadro/math/afxVector.h"
#include "qwadro/math/afxMatrix.h"
#include "qwadro/math/afxQuaternion.h"

typedef enum afxDof
{
    afxDof_POSITION_X   = AFX_BITMASK(0),
    afxDof_POSITION_Y   = AFX_BITMASK(1),
    afxDof_POSITION_Z   = AFX_BITMASK(2),
    afxDof_POSITION     = afxDof_POSITION_X | afxDof_POSITION_Y | afxDof_POSITION_Z,

    afxDof_ROTATION_X   = AFX_BITMASK(3),
    afxDof_ROTATION_Y   = AFX_BITMASK(4),
    afxDof_ROTATION_Z   = AFX_BITMASK(5),
    afxDof_ROTATION     = afxDof_ROTATION_X | afxDof_ROTATION_Y | afxDof_ROTATION_Z,

    // scale/shear
    afxDof_SCALE_X      = AFX_BITMASK(6),
    afxDof_SCALE_Y      = AFX_BITMASK(7),
    afxDof_SCALE_Z      = AFX_BITMASK(8),
    afxDof_SCALE        = afxDof_SCALE_X | afxDof_SCALE_Y | afxDof_SCALE_Z
} afxDof;

typedef enum afxTransformFlag
{
    afxTransformFlag_TRANSLATED = AFX_BITMASK(0), // has non-identity position
    afxTransformFlag_ROTATED    = AFX_BITMASK(1), // has non-identity orientation
    afxTransformFlag_RIGID      = afxTransformFlag_TRANSLATED | afxTransformFlag_ROTATED,
    afxTransformFlag_DEFORMED   = AFX_BITMASK(2), // has non-identity scale/shear
    afxTransformFlag_UNISCALED  = AFX_BITMASK(3), // has scalar scaling [ 1, ... ]
    afxTransformFlag_SCALED     = AFX_BITMASK(4), // has vector scaling [ 1, 0, 0, 0, 1, 0, 0, 0, 1 ]
    afxTransformFlag_ALL =      (afxTransformFlag_TRANSLATED | afxTransformFlag_ROTATED | afxTransformFlag_DEFORMED)
} afxTransformFlags;

AFX_DEFINE_STRUCT_ALIGNED(AFX_SIMD_ALIGNMENT, afxTransform)
{
    afxQuat             orientation;
    afxV3d              position; // @ 16
    afxTransformFlags   flags; // @ 28
    afxM3d              scaleShear; // @ 32
};

AFX afxTransform const AFX_TRANSFORM_ZERO;
AFX afxTransform const AFX_TRANSFORM_IDENTITY;

// You can initialize a afxTransform to the identity transform like this:

AFXINL void     AfxZeroTransform(afxTransform* t); // make zero
AFXINL void     AfxResetTransform(afxTransform* t); // make identity

AFXINL void     AfxCopyTransform(afxTransform* t, afxTransform const *in);

AFXINL void     AfxSetTransform(afxTransform* t, afxV3d const position, afxQuat const orientation, afxM3d const scaleShear, afxBool check);

AFXINL void     AfxClipTransformDofs(afxTransform* t, afxFlags allowedDOFs);

AFXINL afxReal  AfxDetTransform(afxTransform const* t);

AFXINL void     AfxMixTransform(afxTransform* t, afxTransform const* a, afxTransform const* b, afxReal time);

AFXINL void     AfxInvertTransform(afxTransform const* in, afxTransform* t);

AFXINL void     AfxPreMultiplyTransform(afxTransform* t, afxTransform const* pre);
AFXINL void     AfxPostMultiplyTransform(afxTransform* t, afxTransform const* post);
AFXINL void     AfxMultiplyTransform(afxTransform* t, afxTransform const* a, afxTransform const* b);

AFXINL void     AfxTransformArrayedAtv3d(afxTransform const* t, afxUnit cnt, afxV3d const in[], afxV3d out[]);
AFXINL void     AfxTransformArrayedLtv3d(afxTransform const* t, afxUnit cnt, afxV3d const in[], afxV3d out[]);
AFXINL void     AfxTransformArrayedLtv3dTransposed(afxTransform const* t, afxUnit cnt, afxV3d const in[], afxV3d out[]);

AFXINL void     AfxAssimilateTransforms(afxM3d const ltm, afxM3d const iltm, afxV4d const atv, afxUnit cnt, afxTransform const in[], afxTransform out[]);

AFXINL void     AfxComputeCompositeTransformM4d(afxTransform const* t, afxM4d m); // build composite transform 4x4
AFXINL void     AfxComputeCompositeTransformM4dc(afxTransform const* t, afxM4r m); // build composite transform 4x3 (compact matrix)

AFXINL void BuildIdentityWorldPoseOnly_Generic(afxM4d const ParentMatrix, afxM4d ResultWorldMatrix);
AFXINL void BuildPositionWorldPoseOnly_Generic(afxV3d const Position, afxM4d const ParentMatrix, afxM4d ResultWorldMatrix);
AFXINL void BuildFullWorldPoseOnly_Generic(afxTransform const* t, afxM4d const ParentMatrix, afxM4d ResultWorldMatrix);
AFXINL void BuildSingleCompositeFromWorldPose_Generic(afxM4d const InverseWorld4x4, afxM4d const WorldMatrix, afxM4d ResultComposite);
AFXINL void BuildSingleCompositeFromWorldPoseTranspose_Generic(afxM4d const InverseWorld4x4, afxM4d const WorldMatrix, afxM4r ResultComposite3x4);
AFXINL void BuildIdentityWorldPoseComposite_Generic(afxM4d const ParentMatrix, afxM4d const InverseWorld4x4, afxM4d ResultComposite, afxM4d ResultWorldMatrix);
AFXINL void BuildPositionWorldPoseComposite_Generic(afxV3d const Position, afxM4d const ParentMatrix, afxM4d const InverseWorld4x4, afxM4d ResultComposite, afxM4d ResultWorldMatrix);
AFXINL void BuildFullWorldPoseComposite_Generic(afxTransform const* t, afxM4d const ParentMatrix, afxM4d const InverseWorld4x4, afxM4d ResultComposite, afxM4d ResultWorldMatrix);
AFXINL void BuildPositionOrientationWorldPoseOnly_Generic(afxV3d const Position, afxQuat const Orientation, afxM4d const ParentMatrix, afxM4d ResultWorldMatrix);
AFXINL void BuildPositionOrientationWorldPoseComposite_Generic(afxV3d const Position, afxQuat const Orientation, afxM4d const ParentMatrix, afxM4d const InverseWorld4x4, afxM4d ResultComposite, afxM4d ResultWorldMatrix);
AFXINL void BWP_Dispatch(afxTransform const* t, afxM4d const InverseWorld4x4, afxM4d Composite, afxM4d World, afxM4d const ParentWorld);
AFXINL void BWPNC_Dispatch(afxTransform const* t, afxM4d const ParentWorld, afxM4d World);

#endif//AFX_TRANSFORM_H
