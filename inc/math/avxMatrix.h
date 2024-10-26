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

// This code is part of SIGMA Foundation Math <https://sigmaco.org/math>

// Projective transformation matrix.

#ifndef AVX_MATRIX_H
#define AVX_MATRIX_H

#include "qwadro/inc/math/afxMatrix.h"
#include "qwadro/inc/math/afxViewport.h"

typedef enum avxClipBoundary
{
    avxClipBoundary_ZERO_TO_ONE, // Qwadro/Vulkan/Direct3D
    avxClipBoundary_NEG_ONE_TO_ONE, // OpenGL
    avxClipBoundary_NEG_ONE_TO_ZERO // Experimental
} avxClipBoundary;

typedef enum avxHandedness
{
    avxHandedness_RH, // OpenGL/Vulkan/Qwadro
    avxHandedness_LH // Direct3D
} avxHandedness;

AFX_DEFINE_STRUCT(avxClipSpace)
{
    afxBool         nonRh;
    afxV3d          boundMin;
    afxV3d          boundMax;
    avxClipBoundary boundary;
};

AVX avxClipSpace const AVX_CLIP_SPACE_QWADRO;
AVX avxClipSpace const AVX_CLIP_SPACE_OPENGL;
AVX avxClipSpace const AVX_CLIP_SPACE_VULKAN;
AVX avxClipSpace const AVX_CLIP_SPACE_D3D;

// Shadow

AVXINL void     AfxComputeShadowM4d(afxM4d m, afxPlane const p, afxReal const lightPos[3]);

// Cubemap

AVXINL void     AfxCubemapMatrix_OpenGL(afxM4d m, afxUnit face);
AVXINL void     AfxCubemapMatrix_Direct3D(afxM4d m, afxUnit face);

// View space

AVXINL void     AfxComputeLookToMatrix(afxM4d m, afxV3d const eye, afxV3d const dir, afxV3d const up, avxClipSpace const* clip);
AVXINL void     AfxComputeLookAtMatrix(afxM4d m, afxV3d const eye, afxV3d const target, afxV3d const up, avxClipSpace const* clip);

// Orthographic projective space

AVXINL void     AfxComputeOrthographicMatrix(afxM4d m, afxV2d const extent, afxReal near, afxReal far, avxClipSpace const* clip);
AVXINL void     AfxComputeOffcenterOrthographicMatrix(afxM4d m, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far, avxClipSpace const* clip);

/// Computa uma afxM4d de projeção ortográfica desde uma afxBox.
AVXINL void     AfxComputeBoundingOrthographicMatrix(afxM4d m, afxBox const aabb, avxClipSpace const* clip);

/// Computa uma afxM4d de projeção ortográfica genérica.
AVXINL void     AfxComputeBasicOrthographicMatrix(afxM4d m, afxReal aspectRatio, afxReal scale, afxReal range, avxClipSpace const* clip);

// Perspective projective space

AVXINL void     AfxComputePerspectiveMatrix(afxM4d m, afxV2d const extent, afxReal near, afxReal far, avxClipSpace const* clip);
AVXINL void     AfxComputeFovPerspectiveMatrix(afxM4d m, afxReal fovY, afxReal aspectRatio, afxReal near, afxReal far, avxClipSpace const* clip);
AVXINL void     AfxComputeOffcenterPerspectiveMatrix(afxM4d m, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far, avxClipSpace const* clip);

AVXINL void     AfxComputeBasicPerspectiveMatrix(afxM4d m, afxReal aspectRatio, afxReal range, avxClipSpace const* clip);

// Decompose

AVXINL void     AfxDecomposePerspectiveM4d(afxM4d m, afxV2d const extent, afxReal* near, afxReal* far);
AVXINL void     AfxDecomposeFovPerspectiveM4d(afxM4d m, afxReal* fovY, afxReal* aspectRatio, afxReal* near, afxReal* far);
AVXINL void     AfxDecomposeOffcenterPerspectiveM4d(afxM4d m, afxReal* left, afxReal* right, afxReal* top, afxReal* bottom, afxReal* near, afxReal* far);

// RW

AVXINL void     AfxComputeRenderWareViewM4d(afxM4d m, afxM4d const cam);
AVXINL void     AfxComputeRenderWareProjectionM4d(afxM4d m, afxViewport const* vp, afxBool perspective);

#endif//AVX_MATRIX_H
