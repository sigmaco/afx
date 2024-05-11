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
 *                       (c) 2017 SIGMA, Engitech, Scitech, Serpro
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA Foundation Math <https://sigmaco.org/math>

// Projective transformation matrix.

#ifndef AFX_PROJECTIVE_MATRIX_H
#define AFX_PROJECTIVE_MATRIX_H

#include "afxMatrix.h"

typedef enum afxClipBoundary
{
    afxClipBoundary_ZERO_TO_ONE, // Qwadro/Vulkan/Direct3D
    afxClipBoundary_NEG_ONE_TO_ONE, // OpenGL
    afxClipBoundary_NEG_ONE_TO_ZERO // Experimental
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

AFX afxClipSpace const AFX_CLIP_SPACE_QWADRO;
AFX afxClipSpace const AFX_CLIP_SPACE_OPENGL;
AFX afxClipSpace const AFX_CLIP_SPACE_VULKAN;
AFX afxClipSpace const AFX_CLIP_SPACE_D3D;

// Shadow

AFXINL void     AfxComputeShadowM4d(afxM4d m, afxPlane const p, afxReal const lightPos[3]);

// Cubemap

AFXINL void     AfxCubemapMatrix_OpenGL(afxM4d m, afxNat face);
AFXINL void     AfxCubemapMatrix_Direct3D(afxM4d m, afxNat face);

// View space

AFXINL void     AfxComputeLookToMatrix(afxM4d m, afxV3d const eye, afxV3d const dir, afxV3d const up, afxClipSpace const* clip);
AFXINL void     AfxComputeLookAtMatrix(afxM4d m, afxV3d const eye, afxV3d const target, afxV3d const up, afxClipSpace const* clip);

// Orthographic projective space

AFXINL void     AfxComputeOrthographicMatrix(afxM4d m, afxV2d const extent, afxReal near, afxReal far, afxClipSpace const* clip);
AFXINL void     AfxComputeOffcenterOrthographicMatrix(afxM4d m, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far, afxClipSpace const* clip);

/// Computa uma afxM4d de projeção ortográfica desde uma afxAabb.
AFXINL void     AfxComputeBoundingOrthographicMatrix(afxM4d m, afxAabb const aabb, afxClipSpace const* clip);

/// Computa uma afxM4d de projeção ortográfica genérica.
AFXINL void     AfxComputeBasicOrthographicMatrix(afxM4d m, afxReal aspectRatio, afxReal scale, afxReal range, afxClipSpace const* clip);

// Perspective projective space

AFXINL void     AfxComputePerspectiveMatrix(afxM4d m, afxV2d const extent, afxReal near, afxReal far, afxClipSpace const* clip);
AFXINL void     AfxComputeFovPerspectiveMatrix(afxM4d m, afxReal fovY, afxReal aspectRatio, afxReal near, afxReal far, afxClipSpace const* clip);
AFXINL void     AfxComputeOffcenterPerspectiveMatrix(afxM4d m, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far, afxClipSpace const* clip);

AFXINL void     AfxComputeBasicPerspectiveMatrix(afxM4d m, afxReal aspectRatio, afxReal range, afxClipSpace const* clip);

// Decompose

AFXINL void     AfxDecomposePerspectiveM4d(afxM4d m, afxV2d const extent, afxReal* near, afxReal* far);
AFXINL void     AfxDecomposeFovPerspectiveM4d(afxM4d m, afxReal* fovY, afxReal* aspectRatio, afxReal* near, afxReal* far);
AFXINL void     AfxDecomposeOffcenterPerspectiveM4d(afxM4d m, afxReal* left, afxReal* right, afxReal* top, afxReal* bottom, afxReal* near, afxReal* far);

// RW

AFXINL void     AfxComputeRenderWareViewM4d(afxM4d m, afxM4d const cam);
AFXINL void     AfxComputeRenderWareProjectionM4d(afxM4d m, afxViewport const* vp, afxBool perspective);

#endif//AFX_PROJECTIVE_MATRIX_H
