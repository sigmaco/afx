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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

// Projective transformation matrix.

#ifndef AFX_PROJECTIVE_MATRIX_H
#define AFX_PROJECTIVE_MATRIX_H

#include "afxMatrix.h"

typedef enum afxClipDepthRange
{
    afxClipDepthRange_ZERO_TO_ONE,
    afxClipDepthRange_NEG_ONE_TO_ONE,
    afxClipDepthRange_NEG_ONE_TO_ZERO
} afxClipDepthRange;

typedef enum afxHandedness
{
    afxHandedness_RH,
    afxHandedness_LH
} afxHandedness;

// Shadow

AFXINL void     AfxComputeShadowM4d(afxReal m[4][4], afxPlane const p, afxReal const lightPos[3]);

// Cubemap

AFXINL void     AfxCubemapMatrix_OpenGL(afxReal m[4][4], afxNat face);
AFXINL void     AfxCubemapMatrix_Direct3D(afxReal m[4][4], afxNat face);

// View space

AFXINL void     AfxComputeLookToMatrix(afxReal v[4][4], afxReal const eye[3], afxReal const dir[3], afxReal const up[3], afxBool lh);
AFXINL void     AfxComputeLookAtMatrix(afxReal v[4][4], afxReal const eye[3], afxReal const target[3], afxReal const up[3], afxBool lh);

// Orthographic projective space

AFXINL void     AfxComputeOrthographicMatrix(afxReal p[4][4], afxReal const extent[2], afxReal near, afxReal far, afxBool lh, afxClipDepthRange cdr);
AFXINL void     AfxComputeOffcenterOrthographicMatrix(afxReal p[4][4], afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far, afxBool lh, afxClipDepthRange cdr);

/// Computa uma afxM4d de projeção ortográfica desde uma afxAabb.
AFXINL void     AfxComputeOrthographicMatrixFromAabb(afxReal m[4][4], afxAabb const aabb, afxBool lh, afxClipDepthRange cdr);

/// Computa uma afxM4d de projeção ortográfica genérica.
AFXINL void     AfxResetOrthographicMatrix(afxReal m[4][4], afxReal aspectRatio, afxReal scale, afxBool lh, afxClipDepthRange cdr);

// Perspective projective space

AFXINL void     AfxComputePerspectiveMatrix(afxReal p[4][4], afxReal const extent[2], afxReal near, afxReal far, afxBool lh, afxClipDepthRange cdr);
AFXINL void     AfxComputeFovPerspectiveMatrix(afxReal p[4][4], afxReal fovY, afxReal aspectRatio, afxReal near, afxReal far, afxBool lh, afxClipDepthRange cdr);
AFXINL void     AfxComputeOffcenterPerspectiveMatrix(afxReal p[4][4], afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far, afxBool lh, afxClipDepthRange cdr);

AFXINL void     AfxResetPerspectiveMatrix(afxReal m[4][4], afxReal aspectRatio, afxBool lh, afxClipDepthRange cdr);

// Decompose

AFXINL void     AfxDecomposePerspectiveM4d(afxReal m[4][4], afxReal extent[2], afxReal* near, afxReal* far);
AFXINL void     AfxDecomposeFovPerspectiveM4d(afxReal m[4][4], afxReal* fovY, afxReal* aspectRatio, afxReal* near, afxReal* far);
AFXINL void     AfxDecomposeOffcenterPerspectiveM4d(afxReal m[4][4], afxReal* left, afxReal* right, afxReal* top, afxReal* bottom, afxReal* near, afxReal* far);

// RW

AFXINL void     AfxComputeRenderWareViewM4d(afxReal v[4][4], afxReal const cam[4][4]);
AFXINL void     AfxComputeRenderWareProjectionM4d(afxReal p[4][4], afxViewport const* vp, afxBool perspective);

#endif//AFX_PROJECTIVE_MATRIX_H
