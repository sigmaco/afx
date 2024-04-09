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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA Foundation Math <https://sigmaco.org/math>

/// Qwadro performs all bone animation on decomposed transforms, where "decomposed" means that the position, orientation, and scale/shear components have been pulled out into separately animating quantities. 
/// This allows for fair, efficient animation and interpolation, whereas keeping everything as a tangled 4x4 matrix does not. 

/// The afxTransform is the primary structure used to store these decomposed transforms. Each structure stores a 4-element position vector, a 4-element quaternion orientation, and a 3x3 scale/shear matrix (note that it may also contain mirroring). 
/// Additionally, each transform stores a set of afxTransformFlags that indicates which, if any, of these values current has a non-identity value. These flags are solely used to speed up computation.

#ifndef AFX_TRANSFORM_H
#define AFX_TRANSFORM_H

#include "qwadro/math/afxVector.h"
#include "qwadro/math/afxMatrix.h"
#include "qwadro/math/afxQuaternion.h"

typedef enum afxTransformFlags
{
    afxTransformFlags_TRANSLATED    = AFX_BIT_OFFSET(0), // has non-identity position
    afxTransformFlags_ROTATED       = AFX_BIT_OFFSET(1), // has non-identity orientation
    afxTrasnformFlags_RIGID         = afxTransformFlags_TRANSLATED | afxTransformFlags_ROTATED,
    afxTransformFlags_DEFORMED      = AFX_BIT_OFFSET(2), // has non-identity scale/shear
    afxTransformFlags_ALL =         (afxTransformFlags_TRANSLATED | afxTransformFlags_ROTATED | afxTransformFlags_DEFORMED)
} afxTransformFlags;

AFX_DEFINE_STRUCT(afxTransform)
{
    afxTransformFlags   flags;
    afxV3d              position;
    afxQuat             orientation;
    afxM3d              scaleShear;
};

AFX afxTransform const AFX_TRANSFORM_ZERO;
AFX afxTransform const AFX_TRANSFORM_IDENTITY;

// You can initialize a afxTransform to the identity transform like this:

AFXINL void     AfxZeroTransform(afxTransform *t); // make zero
AFXINL void     AfxResetTransform(afxTransform *t); // make identity

AFXINL void     AfxCopyTransform(afxTransform *t, afxTransform const *in);

AFXINL void     AfxSetTransform(afxTransform *t, afxV3d const position, afxQuat const orientation, afxM3d const scaleShear);
AFXINL void     AfxSetTransformWithIdentityCheck(afxTransform *t, afxV3d const position, afxQuat const orientation, afxM3d const scaleShear);

AFXINL void     AfxClipTransformDofs(afxTransform *t, afxFlags allowedDOFs);

AFXINL afxReal  AfxDetTransform(afxTransform const *t);

AFXINL void     AfxLerpTransform(afxTransform *t, afxTransform const* a, afxTransform const* b, afxReal time);

AFXINL void     AfxInvertTransform(afxTransform const* in, afxTransform* t);

//AFXINL void     AfxLerpTransform(afxTransform const *t, afxTransform const *other, afxReal time, afxTransform *out); // aka linear blend

AFXINL void     AfxPreMultiplyTransform(afxTransform* t, afxTransform const* pre);
AFXINL void     AfxPostMultiplyTransform(afxTransform* t, afxTransform const* post);
AFXINL void     AfxMultiplyTransform(afxTransform* t, afxTransform const* a, afxTransform const* b);

AFXINL void     AfxTransformArrayedAtv3d(afxTransform const* t, afxNat cnt, afxV3d const in[], afxV3d out[]);
AFXINL void     AfxTransformArrayedLtv3d(afxTransform const* t, afxNat cnt, afxV3d const in[], afxV3d out[]);
AFXINL void     AfxTransformArrayedLtv3dTransposed(afxTransform const* t, afxNat cnt, afxV3d const in[], afxV3d out[]);

AFXINL void     AfxAssimilateTransforms(afxM3d const ltm, afxM3d const iltm, afxV4d const atv, afxNat cnt, afxTransform const in[], afxTransform out[]);

AFXINL void     AfxComputeTransformM4d(afxTransform const *t, afxM4d m); // build composite transform 4x4
AFXINL void     AfxComposeTransformCompactMatrix(afxTransform const *t, afxV3d m[4]); // build composite transform 4x3 (compact matrix)

#endif//AFX_TRANSFORM_H
