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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AVX_CAMERA_H
#define AVX_CAMERA_H

#include "qwadro/draw/afxDrawDefs.h"
#include "qwadro/mem/afxArray.h"
#include "qwadro/math/afxFrustum.h"
#include "qwadro/draw/math/avxMatrix.h"

/// Before you use the Qwadro camera matrices, you need to make a call to AfxRecomputeCameraMatrices() to recompute them.
AVX void            AfxRecomputeCameraMatrices(avxCamera cam);

/// Once computed, you can use the Qwadro matrices of the camera to set up your graphics operations.
AVX void            AfxGetCameraMatrices(avxCamera cam, afxM4d v, afxM4d iv);
AVX void            AfxGetCameraProjectiveMatrices(avxCamera cam, afxM4d p, afxM4d ip);

/// Call AfxTranslateCamera() to move a camera relative to its current orientation.
AVX void            AfxTranslateCamera(avxCamera cam, afxV3d const motion);

/// Call AfxGetCameraPosition() to get the world-space position of a camera (taking into account its displacement).
AVX void            AfxGetCameraPosition(avxCamera cam, afxV4d origin);

/// Get the world-space vector that points to the camera's left/right, down/up, near/far direction.
AVX void            AfxGetCameraDirectionX(avxCamera cam, afxV3d left, afxV3d right);
AVX void            AfxGetCameraDirectionY(avxCamera cam, afxV3d down, afxV3d up);
AVX void            AfxGetCameraDirectionZ(avxCamera cam, afxV3d near, afxV3d far);

/// Once computed, you can use the frustum volume of the camera to apply culling techniques.
AVX void            AfxGetCameraFrustum(avxCamera cam, afxFrustum *frustum);

/// Reset a camera to its default configuration.
AVX void            AfxResetCamera(avxCamera cam);

AVX afxReal         AfxGetCameraFov(avxCamera cam);
AVX void            AfxSetCameraFov(avxCamera cam, afxReal fov);

/// Set the near and far clip planes.
AVX void            AfxSetCameraClipPlanes(avxCamera cam, afxReal near, afxReal far);
AVX void            AfxGetCameraClipPlanes(avxCamera cam, afxReal *near, afxReal *far);

/// Take the explicit control over what the Z projection range is,  and whether or not there is a far clipping plane.
AVX avxClipBoundary AfxGetCameraDepthRange(avxCamera cam);
AVX void            AfxSetCameraDepthRange(avxCamera cam, avxClipBoundary range);

AVX afxReal         AfxGetCameraDepthRangeEpsilon(avxCamera cam);
AVX void            AfxSetCameraDepthRangeEpsilon(avxCamera cam, afxReal epsilon);

/// You must reset the camera's aspect ratios any time the display device, resolution, or window size changes.
AVX void            AfxSetCameraAspectRatios(avxCamera cam, afxReal physAspectRatio, afxV2d const screenExtent, afxV2d const windowExtent);

/// The offset vector has horizontal and vertical offsets as its first two components (respectively), 
/// and the distance from camera as its third component.
AVX void            AfxGetCameraDisplacement(avxCamera cam, afxV3d displacement);
AVX void            AfxResetCameraDisplacement(avxCamera cam, afxV3d const displacement);
AVX void            AfxApplyCameraDisplacement(avxCamera cam, afxV3d const displacement);

/// These methods are useful to make a camera zoom in/out on a specific target. 
AVX afxReal         AfxGetCameraDistance(avxCamera cam);
AVX void            AfxSetCameraDistance(avxCamera cam, afxReal distance);
AVX void            AfxApplyCameraDistance(avxCamera cam, afxReal distance);

/// These methods are useful to make a camera orbit on a specific target. 
/// Note that elevation is measured as rotation about the X(right) axis (hence, it is negative if it is above the horizontal, positive if it is below), 
/// azimuth is measured as rotation about the Y(up) axis, and, and roll is measured as rotation about the Z(back) axis.
AVX void            AfxGetCameraOrientation2(avxCamera cam, afxReal* elevation, afxReal* azimuth, afxReal* roll);
AVX void            AfxApplyCameraOrientation2(avxCamera cam, afxReal elevation, afxReal azimuth, afxReal roll);

/// The EAR vector has the elevation as its first component, azimuth as its second, and roll as its third (all three are in radians). 
AVX void            AfxGetCameraOrientation(avxCamera cam, afxV3d elevAzimRoll);
AVX void            AfxResetCameraOrientation(avxCamera cam, afxV3d const elevAzimRoll);
AVX void            AfxApplyCameraOrientation(avxCamera cam, afxV3d const elevAzimRoll);

/// For picking 3D objects with the mouse, it's necessary to be able to take a point on the screen and figure out where it would be in world-space. Qwadro can do this for you automatically like this:

AVX void            AfxFindWorldCoordinates(avxCamera cam, afxV2d const wh, afxV3d const screenPoint, afxV4d worldPoint);

/// Do a ray-based object picking returning the world position and the normal of a target.
/// Note that the MouseY must be "pre-flipped" to align with the standard 3D alignment of the Y-axis as going up.
/// curY = (wndHeight - 1) - curY;
AVX void            AfxGetCameraPickingRay(avxCamera cam, afxV2d const wh, afxV2d const cursor, afxV4d origin, afxV3d normal);

/// Use this method to project a single point from world space to window space.
AVX void            AfxFindScreenCoordinates(avxCamera cam, afxV2d const wh, afxV4d const worldPoint, afxV3d screenPoint);

/// Transforms a 2D vector of movement into the appropriate 3D vector in world space. 
AVX void            AfxComputeCameraRelativePlanarBases(avxCamera cam, afxBool screenOrthogonal, afxV3d const planeNormal, afxV4d const pointOnPlane, afxV3d xBasis, afxV3d yBasis);
/// The result from GetRelativePlanarBases is two vectors, one which is the world-space vector along which X motion should be interpreted (ex., horizontal movement via the keyboard or analog stick), 
/// and the other is for Y. These vectors are unit length, but they are only orthogonal in world space if you pass false for the second parameter. 
/// If you pass true, this instructs Qwadro to create non-orthgonal bases such that vertical motion (Y motion) will be mapped to a direction in world space which makes the character appear to move directly upwards on the screen, 
/// which is actually not the direction orthogonal to perfect horizontal motion (unless the character is at the very center of the screen).

AVX afxBool         AfxProcessCameraInteraction(avxCamera cam, afxNat port, afxReal64 speed, afxReal64 dt);

AVX afxBool         AvxBeginSceneCapture(avxCamera cam);
AVX afxBool         AvxEndSceneCapture(avxCamera cam);

////////////////////////////////////////////////////////////////////////////////

AVX afxError        AfxAcquireCameras(afxDrawInput din, afxNat cnt, avxCamera cam[]);

#endif//AVX_CAMERA_H
