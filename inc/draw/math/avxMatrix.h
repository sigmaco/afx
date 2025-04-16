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
#include "qwadro/inc/draw/math/avxViewport.h"

/*
    Reversed Z (Inverted Z) Depth Range:
    In standard depth mapping (ZERO_TO_ONE), the near plane typically corresponds to a depth of 0 and the far plane corresponds to a depth of 1. 
    
    In reversed Z, the mapping is flipped, meaning:
        The near plane maps to 1.0 in depth space.
        The far plane maps to 0.0 in depth space.
    
    This "reversed" range helps with depth precision because in traditional depth ranges, the depth buffer tends to have more precision for objects near the near plane and less precision for objects far away. 
    This is due to the way depth is typically stored in a linear space.
    
    With reversed Z:
        Objects near the camera (near plane) are represented by smaller depth values (closer to 1.0).
        Objects farther from the camera (far plane) are represented by larger depth values (closer to 0.0).

    This allows for better precision in the depth buffer because the depth values are now distributed more evenly across the entire depth range, 
    which is particularly useful when rendering large scenes with objects at a wide range of depths.
*/

typedef enum avxClipSpaceDepth
// The depth mapping used by projection to map depth values into the clip space.
{
    // Direct3D and Vulkan uses a normalized depth range from 0 (near plane) to 1 (far plane). 
    // This is a more intuitive depth range, as it maps more directly to values that are easy to work with in shaders 
    // and computations, and it's compatible with the way hardware handles depth buffers.
    avxClipSpaceDepth_ZERO_TO_ONE, // Qwadro, Vulkan, Direct3D

    // This is the initial and most common depth range used in OpenGL, where the depth is normalized to the range from -1 (near plane) to 1 (far plane).
    // This is useful for depth buffering in rendering, with the center of the near plane at -1 and the center of the far plane at 1.
    avxClipSpaceDepth_NEG_ONE_TO_ONE, // OpenGL, Vulkan

    // QWADRO EXPERIMENTAL

    // In the NEG_ONE_TO_ZERO range, the near plane maps to -1, and the far plane maps to 0.
    // This is conceptually similar to reversed Z, with a slight difference in the specific range of values.
    // But, it still follows the principle of putting closer objects at higher values and farther objects at lower values.
    // This approach would indeed help with depth precision because:
    //      Depth values near the near plane (which will now be closer to -1) have higher precision.
    //      Depth values near the far plane (which will now be closer to 0) also get more evenly distributed.

    // Benefits of Reversed Z (and NEG_ONE_TO_ZERO):
    // Improved precision. The most significant benefit is improved depth buffer precision.
    // Depth values are distributed more uniformly across the depth range, which helps minimize depth fighting and improves the quality of depth testing, 
    // especially for large scenes with objects that span a wide range of distances.
    // Avoiding precision loss. With the traditional ZERO_TO_ONE depth range, you get higher precision for objects closer to the near plane, 
    // but this leaves far objects with much less precision. Reversed Z distributes precision more evenly across the depth range, 
    // which is helpful for distant objects.
    // Better handling of depth-fighting. Depth-fighting (or z-fighting) is a common artifact where two surfaces in the scene are very close to each other in depth, 
    // causing them to "flicker" in and out of view due to limited depth buffer precision. By using reversed Z or NEG_ONE_TO_ZERO, 
    // this issue can be alleviated for large-scale scenes where far objects are being rendered.

    // Practical use cases:
    // Shadow mapping. Reversed Z is particularly useful in shadow mapping techniques.
    // The precision improvements allow shadows to be rendered more accurately, especially for distant objects.
    // Large-scale environments. When dealing with large-scale environments like open-world games or simulations with vast 
    // landscapes, reversed Z ensures better depth precision across the entire scene.
    // Rendering with large depth ranges. In situations where objects are spread across a large range of depths (e.g., space 
    // scenes, large architectural scenes), reversed Z can help maintain consistent depth precision.

    // Key Takeaways for NEG_ONE_TO_ZERO:
    // NEG_ONE_TO_ZERO is indeed a form of reversed Z.
    // The principle behind it is to allocate more precision to closer objects (which are mapped to values closer to -1) and ensure better depth distribution across the entire scene.
    // It's less common but can be useful in scenarios where depth precision is critical, especially for large-scale scenes and certain rendering techniques like shadow mapping or deferred rendering.
    // If your goal is improving depth buffer precision, especially for scenes with large depth ranges, using NEG_ONE_TO_ZERO or reversed Z would be a viable option to consider, and the behavior of NEG_ONE_TO_ZERO in your matrix calculations fits well into that framework.

    // This depth range isn't as common, and it's often seen in specialized use cases. 
    // Some systems or engines might use this range for custom depth buffer or projection space setups. 
    // It’s not widely supported or documented in major graphics APIs, but it might appear in some niche applications or custom solutions.
    // From a theoretical standpoint, this could be useful in certain specialized rendering techniques or custom depth buffer handling, but its real-world applicability is unclear.
    // One possible application for this depth range could be in shadow mapping or custom depth projections for specific rendering techniques where the depth needs to go in reverse order (with 0 being closer and -1 being farthest).
    avxClipSpaceDepth_NEG_ONE_TO_ZERO // Qwadro Reversed Z (experimental stuff)
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
AVXINL void     AfxComputeRenderWareProjectionM4d(afxM4d m, avxViewport const* vp, afxBool perspective);

#endif//AVX_MATRIX_H
