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

#ifndef AFX_TRANSFORM_H
#define AFX_TRANSFORM_H

#include "afxMathDefs.h"

// Qwadro performs all bone animation on decomposed transforms, where "decomposed" means that the position, orientation, and scale/shear components have been pulled out into separately animating quantities. 
// This allows for fair, efficient animation and interpolation, whereas keeping everything as a tangled 4x4 matrix does not. 

// The afxTransform is the primary structure used to store these decomposed transforms. Each structure stores a 4-element position vector, a 4-element quaternion orientation, and a 3x3 scale/shear matrix (note that it may also contain mirroring). 
// Additionally, each transform stores a set of afxTransformFlags that indicates which, if any, of these values current has a non-identity value. These flags are solely used to speed up computation.

typedef enum afxTransformFlags
{
    afxTransformFlags_ORIGIN        = AfxGetBitOffset(0), // has non-identity position
    afxTransformFlags_ORIENTATION   = AfxGetBitOffset(1), // has non-identity orientation
    afxTrasnformFlags_RIGID         = afxTransformFlags_ORIGIN | afxTransformFlags_ORIENTATION,
    afxTransformFlags_DEFORM        = AfxGetBitOffset(2), // has non-identity scale/shear
    afxTransformFlags_ALL =         (afxTransformFlags_ORIGIN | afxTransformFlags_ORIENTATION | afxTransformFlags_DEFORM)
} afxTransformFlags;

AFX_DEFINE_STRUCT(afxTransform)
{
    afxTransformFlags   flags;
    afxV3d              position;
    afxQuat             orientation;
    afxM3d              scaleShear;
};

// You can initialize a afxTransform to the identity transform like this:

AFXINL void     AfxZeroTransform(afxTransform *t); // make zero
AFXINL void     AfxResetTransform(afxTransform *t); // make identity

AFXINL void     AfxCopyTransform(afxTransform *t, afxTransform const *in);

AFXINL void     AfxSetTransform(afxTransform *t, afxV3d const position, afxQuat const orientation, afxM3d const scaleShear);
AFXINL void     AfxSetTransformWithIdentityCheck(afxTransform *t, afxV3d const position, afxQuat const orientation, afxM3d const scaleShear);

AFXINL void     AfxClipTransformDofs(afxTransform *t, afxFlags allowedDOFs);

AFXINL afxReal  AfxDetTransform(afxTransform const *t);

AFXINL void     AfxLerpTransform(afxTransform *t, afxTransform const* a, afxTransform const* b, afxReal time);

AFXINL void     AfxGetInverseTransform(afxTransform* t, afxTransform const* in);

//AFXINL void     AfxLerpTransform(afxTransform const *t, afxTransform const *other, afxReal time, afxTransform *out); // aka linear blend

AFXINL void     AfxPreMultiplyTransform(afxTransform* t, afxTransform const* pre);
AFXINL void     AfxPostMultiplyTransform(afxTransform* t, afxTransform const* post);
AFXINL void     AfxMultiplyTransform(afxTransform* t, afxTransform const* a, afxTransform const* b);

AFXINL void     AfxComposeTransformM4d(afxTransform const *t, afxReal m[4][4]); // build composite transform 4x4
AFXINL void     AfxComposeTransformWorldM4d(afxTransform const *t, afxReal const parent[4][4], afxReal w[4][4]); // compose transform world matrix 4x4 only
AFXINL void     AfxGetTransformWorldAndCompositeMatrix(afxTransform const *t, afxReal const parent[4][4], afxReal const iw[4][4], afxReal composite[4][4], afxReal w[4][4]);
AFXINL void     AfxComposeTransformCompactMatrix(afxTransform const *t, afxReal m[4][3]); // build composite transform 4x3 (compact matrix)

AFXINL void     AfxAssimilateTransforms(afxReal const ltm[3][3], afxReal const iltm[3][3], afxReal const atv[4], afxNat cnt, afxTransform const in[], afxTransform out[]);

AFXINL void     AfxTransformArrayedPointV3d(afxTransform const* t, afxNat cnt, afxReal const in[][3], afxReal out[][3]);
AFXINL void     AfxTransformArrayedNormalV3d(afxTransform const* t, afxNat cnt, afxReal const in[][3], afxReal out[][3]);

AFXINL afxError AfxReadTransforms(afxStream in, afxNat cnt, afxTransform dst[]);
AFXINL afxError AfxWriteTransforms(afxStream out, afxNat cnt, afxTransform const src[]);

#endif//AFX_TRANSFORM_H
