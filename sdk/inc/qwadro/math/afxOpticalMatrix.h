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

// Projective transformation matrix.

#ifndef AFX_PROJECTIVE_MATRIX_H
#define AFX_PROJECTIVE_MATRIX_H

#include "afxMatrix.h"

typedef enum afxClipBoundary
{
    afxClipBoundary_ZERO_TO_ONE, // Vulkan/Direct3D
    afxClipBoundary_NEG_ONE_TO_ONE, // OpenGL
    afxClipBoundary_NEG_ONE_TO_ZERO // Tchola
} afxClipBoundary;

typedef enum afxHandedness
{
    afxHandedness_RH, // OpenGL/Vulkan/Qwadro
    afxHandedness_LH // Direct3D
} afxHandedness;

AFX_DEFINE_STRUCT(afxClipSpace)
{
    afxBool         nonRh;
    afxV3d          boundMin;
    afxV3d          boundMax;
    afxClipBoundary boundary;
};

// Shadow

AFXINL void     AfxComputeShadowM4d(afxReal m[4][4], afxPlane const p, afxReal const lightPos[3]);

// Cubemap

AFXINL void     AfxCubemapMatrix_OpenGL(afxReal m[4][4], afxNat face);
AFXINL void     AfxCubemapMatrix_Direct3D(afxReal m[4][4], afxNat face);

// View space

AFXINL void     AfxComputeLookToMatrix(afxReal v[4][4], afxReal const eye[3], afxReal const dir[3], afxReal const up[3], afxClipSpace const* clip);
AFXINL void     AfxComputeLookAtMatrix(afxReal v[4][4], afxReal const eye[3], afxReal const target[3], afxReal const up[3], afxClipSpace const* clip);

// Orthographic projective space

AFXINL void     AfxComputeOrthographicMatrix(afxReal p[4][4], afxReal const extent[2], afxReal near, afxReal far, afxClipSpace const* clip);
AFXINL void     AfxComputeOffcenterOrthographicMatrix(afxReal p[4][4], afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far, afxClipSpace const* clip);

/// Computa uma afxM4d de projeção ortográfica desde uma afxAabb.
AFXINL void     AfxComputeBoundingOrthographicMatrix(afxReal m[4][4], afxAabb const aabb, afxClipSpace const* clip);

/// Computa uma afxM4d de projeção ortográfica genérica.
AFXINL void     AfxComputeBasicOrthographicMatrix(afxReal m[4][4], afxReal aspectRatio, afxReal scale, afxReal range, afxClipSpace const* clip);

// Perspective projective space

AFXINL void     AfxComputePerspectiveMatrix(afxReal p[4][4], afxReal const extent[2], afxReal near, afxReal far, afxClipSpace const* clip);
AFXINL void     AfxComputeFovPerspectiveMatrix(afxReal p[4][4], afxReal fovY, afxReal aspectRatio, afxReal near, afxReal far, afxClipSpace const* clip);
AFXINL void     AfxComputeOffcenterPerspectiveMatrix(afxReal p[4][4], afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far, afxClipSpace const* clip);

AFXINL void     AfxComputeBasicPerspectiveMatrix(afxReal m[4][4], afxReal aspectRatio, afxReal range, afxClipSpace const* clip);

// Decompose

AFXINL void     AfxDecomposePerspectiveM4d(afxReal m[4][4], afxReal extent[2], afxReal* near, afxReal* far);
AFXINL void     AfxDecomposeFovPerspectiveM4d(afxReal m[4][4], afxReal* fovY, afxReal* aspectRatio, afxReal* near, afxReal* far);
AFXINL void     AfxDecomposeOffcenterPerspectiveM4d(afxReal m[4][4], afxReal* left, afxReal* right, afxReal* top, afxReal* bottom, afxReal* near, afxReal* far);

// RW

AFXINL void     AfxComputeRenderWareViewM4d(afxReal v[4][4], afxReal const cam[4][4]);
AFXINL void     AfxComputeRenderWareProjectionM4d(afxReal p[4][4], afxViewport const* vp, afxBool perspective);

AFX afxClipSpace const  AFX_CLIP_SPACE_OPENGL;
AFX afxClipSpace const  AFX_CLIP_SPACE_VULKAN;
AFX afxClipSpace const  AFX_CLIP_SPACE_D3D;

#endif//AFX_PROJECTIVE_MATRIX_H
