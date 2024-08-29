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

/// Qwadro performs all bone animation on decomposed transforms, where "decomposed" means that the position, orientation, and scale/shear components have been pulled out into separately animating quantities. 
/// This allows for fair, efficient animation and interpolation, whereas keeping everything as a tangled 4x4 matrix does not. 

/// The afxTransform is the primary structure used to store these decomposed transforms. Each structure stores a 4-element position vector, a 4-element quaternion orientation, and a 3x3 scale/shear matrix (note that it may also contain mirroring). 
/// Additionally, each transform stores a set of afxTransformFlags that indicates which, if any, of these values current has a non-identity value. These flags are solely used to speed up computation.

#ifndef AFX_TRANSFORM_H
#define AFX_TRANSFORM_H

#include "qwadro/inc/math/afxVector.h"
#include "qwadro/inc/math/afxMatrix.h"
#include "qwadro/inc/math/afxQuaternion.h"

typedef enum afxTransformFlag
{
    afxTransformFlag_TRANSLATED = AFX_BIT(0), // has non-identity position
    afxTransformFlag_ROTATED    = AFX_BIT(1), // has non-identity orientation
    afxTrasnformFlags_RIGID     = afxTransformFlag_TRANSLATED | afxTransformFlag_ROTATED,
    afxTransformFlag_DEFORMED   = AFX_BIT(2), // has non-identity scale/shear
    afxTransformFlag_ALL =      (afxTransformFlag_TRANSLATED | afxTransformFlag_ROTATED | afxTransformFlag_DEFORMED)
} afxTransformFlags;

AFX_DEFINE_STRUCT(afxTransform)
{
    afxQuat             orientation;
    afxV3d              position;
    afxTransformFlags   flags;
    afxM3d              scaleShear;
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

AFXINL void     AfxTransformArrayedAtv3d(afxTransform const* t, afxNat cnt, afxV3d const in[], afxV3d out[]);
AFXINL void     AfxTransformArrayedLtv3d(afxTransform const* t, afxNat cnt, afxV3d const in[], afxV3d out[]);
AFXINL void     AfxTransformArrayedLtv3dTransposed(afxTransform const* t, afxNat cnt, afxV3d const in[], afxV3d out[]);

AFXINL void     AfxAssimilateTransforms(afxM3d const ltm, afxM3d const iltm, afxV4d const atv, afxNat cnt, afxTransform const in[], afxTransform out[]);

AFXINL void     AfxComputeCompositeTransformM4d(afxTransform const* t, afxM4d m); // build composite transform 4x4
AFXINL void     AfxComputeCompositeTransformM4dc(afxTransform const* t, afxM4dc m); // build composite transform 4x3 (compact matrix)

AFXINL void BuildIdentityWorldPoseOnly_Generic(afxM4d const ParentMatrix, afxM4d ResultWorldMatrix);
AFXINL void BuildPositionWorldPoseOnly_Generic(afxV3d const Position, afxM4d const ParentMatrix, afxM4d ResultWorldMatrix);
AFXINL void BuildFullWorldPoseOnly_Generic(afxTransform const* t, afxM4d const ParentMatrix, afxM4d ResultWorldMatrix);
AFXINL void BuildSingleCompositeFromWorldPose_Generic(afxM4d const InverseWorld4x4, afxM4d const WorldMatrix, afxM4d ResultComposite);
AFXINL void BuildSingleCompositeFromWorldPoseTranspose_Generic(afxM4d const InverseWorld4x4, afxM4d const WorldMatrix, afxM4dc ResultComposite3x4);
AFXINL void BuildIdentityWorldPoseComposite_Generic(afxM4d const ParentMatrix, afxM4d const InverseWorld4x4, afxM4d ResultComposite, afxM4d ResultWorldMatrix);
AFXINL void BuildPositionWorldPoseComposite_Generic(afxV3d const Position, afxM4d const ParentMatrix, afxM4d const InverseWorld4x4, afxM4d ResultComposite, afxM4d ResultWorldMatrix);
AFXINL void BuildFullWorldPoseComposite_Generic(afxTransform const* t, afxM4d const ParentMatrix, afxM4d const InverseWorld4x4, afxM4d ResultComposite, afxM4d ResultWorldMatrix);
AFXINL void BuildPositionOrientationWorldPoseOnly_Generic(afxV3d const Position, afxQuat const Orientation, afxM4d const ParentMatrix, afxM4d ResultWorldMatrix);
AFXINL void BuildPositionOrientationWorldPoseComposite_Generic(afxV3d const Position, afxQuat const Orientation, afxM4d const ParentMatrix, afxM4d const InverseWorld4x4, afxM4d ResultComposite, afxM4d ResultWorldMatrix);
AFXINL void BWP_Dispatch(afxTransform* t, afxM4d const InverseWorld4x4, afxM4d Composite, afxM4d World, afxM4d const ParentWorld);
AFXINL void BWPNC_Dispatch(afxTransform* t, afxM4d const ParentWorld, afxM4d World);

#endif//AFX_TRANSFORM_H
