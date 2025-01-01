/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *        Q W A D R O   V I D E O   G R A P H I C S   I N F R A S T R U C T U R E
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

typedef enum avxClipSpaceDepth
// The depth mapping used by projection to map depth values into the clip space.
{
    avxClipSpaceDepth_ZERO_TO_ONE, // Qwadro/Vulkan/Direct3D
    avxClipSpaceDepth_NEG_ONE_TO_ONE, // OpenGL
    avxClipSpaceDepth_NEG_ONE_TO_ZERO // Experimental
} avxClipSpaceDepth;

// Shadow

AVXINL void     AfxComputeShadowM4d(afxM4d m, afxPlane const p, afxReal const lightPos[3]);

// Cubemap

AVXINL void     AfxCubemapMatrix_OpenGL(afxM4d m, afxUnit face);
AVXINL void     AfxCubemapMatrix_Direct3D(afxM4d m, afxUnit face);

// View space

AVXINL void     AfxComputeLookToMatrix(afxM4d m, afxV3d const eye, afxV3d const dir, afxV3d const up, afxBool nonRh);
AVXINL void     AfxComputeLookAtMatrix(afxM4d m, afxV3d const eye, afxV3d const target, afxV3d const up, afxBool nonRh);

// Orthographic projective space

AVXINL void     AfxComputeOrthographicMatrix(afxM4d m, afxV2d const extent, afxReal near, afxReal far, afxBool nonRh, avxClipSpaceDepth clip);
AVXINL void     AfxComputeOffcenterOrthographicMatrix(afxM4d m, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far, afxBool nonRh, avxClipSpaceDepth clip);

/// Computa uma afxM4d de projeção ortográfica desde uma afxBox.
AVXINL void     AfxComputeBoundingOrthographicMatrix(afxM4d m, afxBox const aabb, afxBool nonRh, avxClipSpaceDepth clip);

/// Computa uma afxM4d de projeção ortográfica genérica.
AVXINL void     AfxComputeBasicOrthographicMatrix(afxM4d m, afxReal aspectRatio, afxReal scale, afxReal range, afxBool nonRh, avxClipSpaceDepth clip);

// Perspective projective space

AVXINL void     AfxComputePerspectiveMatrix(afxM4d m, afxV2d const extent, afxReal near, afxReal far, afxBool nonRh, avxClipSpaceDepth clip);
AVXINL void     AfxComputeFovPerspectiveMatrix(afxM4d m, afxReal fovY, afxReal aspectRatio, afxReal near, afxReal far, afxBool nonRh, avxClipSpaceDepth clip);
AVXINL void     AfxComputeOffcenterPerspectiveMatrix(afxM4d m, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far, afxBool nonRh, avxClipSpaceDepth clip);

AVXINL void     AfxComputeBasicPerspectiveMatrix(afxM4d m, afxReal aspectRatio, afxReal range, afxBool nonRh, avxClipSpaceDepth clip);

// Decompose

AVXINL void     AfxDecomposePerspectiveM4d(afxM4d m, afxV2d const extent, afxReal* near, afxReal* far);
AVXINL void     AfxDecomposeFovPerspectiveM4d(afxM4d m, afxReal* fovY, afxReal* aspectRatio, afxReal* near, afxReal* far);
AVXINL void     AfxDecomposeOffcenterPerspectiveM4d(afxM4d m, afxReal* left, afxReal* right, afxReal* top, afxReal* bottom, afxReal* near, afxReal* far);

// RW

AVXINL void     AfxComputeRenderWareViewM4d(afxM4d m, afxM4d const cam);
AVXINL void     AfxComputeRenderWareProjectionM4d(afxM4d m, afxViewport const* vp, afxBool perspective);

#endif//AVX_MATRIX_H
