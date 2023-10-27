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

#ifndef AFX_TRANSFORM_H
#define AFX_TRANSFORM_H

#include "afxMathDefs.h"

// Qwadro performs all bone animation on decomposed transforms, where "decomposed" means that the position, orientation, and scale/shear components have been pulled out into separately animating quantities. 
// This allows for fair, efficient animation and interpolation, whereas keeping everything as a tangled 4x4 matrix does not. 

// The afxTransform is the primary structure used to store these decomposed transforms. Each structure stores a 4-element position vector, a 4-element quaternion orientation, and a 3x3 scale/shear matrix (note that it may also contain mirroring). 
// Additionally, each transform stores a set of afxTransformFlags that indicates which, if any, of these values current has a non-identity value. These flags are solely used to speed up computation.

typedef enum afxTransformFlags
{
    afxTransformFlags_ORIGIN        = AFX_BIT_OFFSET(0), // has non-identity position
    afxTransformFlags_ORIENTATION   = AFX_BIT_OFFSET(1), // has non-identity orientation
    afxTransformFlags_SCALESHEAR    = AFX_BIT_OFFSET(2), // has non-identity scale/shear
    afxTransformFlags_ALL =         (afxTransformFlags_ORIGIN | afxTransformFlags_ORIENTATION | afxTransformFlags_SCALESHEAR)
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

AFXINL afxReal  AfxGetTransformDeterminant(afxTransform const *t);

AFXINL void     AfxLerpTransform(afxTransform *t, afxTransform const* a, afxTransform const* b, afxReal time);

AFXINL void     AfxGetInverseTransform(afxTransform* t, afxTransform const* in);

//AFXINL void     AfxLerpTransform(afxTransform const *t, afxTransform const *other, afxReal time, afxTransform *out); // aka linear blend

AFXINL void     AfxPremultiplyTransform(afxTransform* t, afxTransform const* pre);
AFXINL void     AfxPostmultiplyTransform(afxTransform* t, afxTransform const* post);
AFXINL void     AfxMultiplyTransform(afxTransform* t, afxTransform const* a, afxTransform const* b);

AFXINL void     AfxGetTransformMatrix(afxTransform const *t, afxReal m[4][4]); // build composite transform 4x4
AFXINL void     AfxGetTransformWorldMatrix(afxTransform const *t, afxReal const parent[4][4], afxReal w[4][4]); // compose transform world matrix 4x4 only
AFXINL void     AfxGetTransformWorldAndCompositeMatrix(afxTransform const *t, afxReal const parent[4][4], afxReal const iw[4][4], afxReal composite[4][4], afxReal w[4][4]);
AFXINL void     AfxGetTransformMatrixCompact(afxTransform const *t, afxReal m[4][3]); // build composite transform 4x3 (compact matrix)

AFXINL void     AfxAssimilateTransform(afxTransform* t, afxReal const affine[3], afxReal const linear[3][3], afxReal const invLinear[3][3]);

AFXINL void     AfxGetTransformedPointV3d(afxReal v[3], afxReal const in[3], afxTransform const* t);
AFXINL void     AfxGetTransformedNormalV3d(afxReal v[3], afxReal const in[3], afxTransform const* t);

#endif//AFX_TRANSFORM_H
