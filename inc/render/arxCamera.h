/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *             Q W A D R O   R E N D E R I N G   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

  //////////////////////////////////////////////////////////////////////////////
 // QWADRO DEVICE-AWARE CAMERA                                               //
//////////////////////////////////////////////////////////////////////////////

#ifndef ARX_CAMERA_H
#define ARX_CAMERA_H

/*
    The terms near, far, FOV, and frustum are all related to the camera's view and the projection of objects in the 3D world onto the 2D screen.

    NEAR AND FAR PLANES
    Near Plane: This defines how close an object can be to the camera before it is clipped (i.e., not rendered). 
    It's the closest distance from the camera at which objects are visible. Anything closer than this distance will not be rendered.

    Far Plane: This defines how far away an object can be from the camera before it is clipped. 
    It's the farthest distance from the camera at which objects are visible. Anything farther than this distance will not be rendered.

    These two planes together create a "viewing volume" in which the camera can see objects, 
    and this volume is typically defined in a view frustrum (explained below). 
    These planes are usually set based on the needs of your scene, and the closer the near plane, the more details you can capture, 
    but also the greater the risk of depth precision issues.

    FOV (FIELD OF VIEW)
    Field of View (FOV) is the extent of the observable world that is seen at any given moment. 
    It's measured in radians, and it controls how much of the 3D world is visible in the camera’s view.
    Vertical FOV (or Y-axis FOV) defines how much of the scene can be seen vertically.
    The horizontal FOV is derived from the vertical FOV, aspect ratio (screen width/height), and the camera's aspect ratio.

    FOV affects how "zoomed in" or "zoomed out" the scene looks. A wider FOV will show more of the scene, making objects look smaller and farther apart. A narrower FOV will zoom in on objects, making them appear larger and closer.

    A typical FOV for first-person games might be around 90 degrees.
    For a more cinematic look, a lower FOV (e.g., 60 degrees) might be used.
    FOV is set in radians for projection matrix calculations, but values are often specified in degrees in graphics engines.

    FRUSTUM
    The view frustum is a pyramid-like shape that defines the visible area of the 3D world. 
    It's the shape that represents the portion of space the camera can "see".
    The frustum has a near plane at the front, a far plane at the back, the sides (left, right, top, bottom) 
    are determined by the FOV and the aspect ratio of the screen.

    The frustum is what gets transformed by the camera's projection matrix. 
    Any geometry that falls inside the frustum is visible to the camera, while anything outside the frustum is culled and not rendered.
    A typical frustum consists of: near and far planes, the bounds of the frustum in terms of distance; and
    left, right, top, bottom planes, these are defined based on the FOV and aspect ratio of the camera.

    The near and far planes determine how far in front and behind the camera you can see objects.
    The FOV controls how wide or narrow the camera’s view is.
    The frustum is the actual 3D space (a pyramid shape) between the near and far planes that is visible to the camera.
    The projection matrix is used to calculate the transformation of objects in this frustum from 3D space to the 2D screen, 
    taking into account the near and far planes, the FOV, and the aspect ratio. 
    A perspective projection matrix (which is typically used for 3D scenes) will create this frustum, 
    and the projection will “shrink” objects as they get farther from the camera, which is what gives the scene a realistic appearance.
*/

#include "qwadro/inc/render/arxRenderable.h"
#include "qwadro/inc/draw/afxDrawDefs.h"
#include "qwadro/inc/mem/afxArray.h"
#include "qwadro/inc/math/afxFrustum.h"
#include "qwadro/inc/math/avxMatrix.h"

typedef enum afxCameraFlag
{
    afxCameraFlag_P     = AFX_BITMASK(0),
    afxCameraFlag_V     = AFX_BITMASK(1),
    afxCameraFlag_PV    = (afxCameraFlag_P | afxCameraFlag_V)
} afxCameraFlags;

/// Reset a camera to its default configuration.
ARX void            ArxResetCamera(arxCamera cam);

/// Before you use the Qwadro camera matrices, you need to make a call to ArxRecomputeCameraMatrices() to recompute them.
ARX void            ArxRecomputeCameraMatrices(arxCamera cam);

/// Once computed, you can use the Qwadro matrices of the camera to set up your graphics operations.
ARX void            ArxGetCameraMatrices(arxCamera cam, afxM4d p, afxM4d pv, afxM4d v, afxM4d ip, afxM4d ipv, afxM4d iv);

/// Call ArxTranslateCamera() to move a camera relative to its current orientation.
ARX void            ArxTranslateCamera(arxCamera cam, afxV3d const motion);

/// Call ArxGetCameraPosition() to get the world-space position of a camera (taking into account its displacement).
ARX void            ArxGetCameraPosition(arxCamera cam, afxV3d origin);
ARX void            ArxResetCameraPosition(arxCamera cam, afxV3d const origin);

/// Get the world-space vector that points to the camera's left/right, down/up, near/far direction.
ARX void            ArxGetCameraDirectionX(arxCamera cam, afxV3d left, afxV3d right);
ARX void            ArxGetCameraDirectionY(arxCamera cam, afxV3d down, afxV3d up);
ARX void            ArxGetCameraDirectionZ(arxCamera cam, afxV3d near, afxV3d far);

ARX afxReal         ArxGetCameraFov(arxCamera cam);
ARX afxReal         ArxAdjustCameraFov(arxCamera cam, afxReal fov);

/// Set the near and far clip planes.
ARX void            ArxSetCameraClipPlanes(arxCamera cam, afxReal near, afxReal far);
/// Once computed, you can use the frustum volume of the camera to apply culling techniques.
ARX void            ArxGetCameraFrustum(arxCamera cam, afxFrustum* frustum, afxReal* nearClipPlane, afxReal* farClipPlane);

/// Take the explicit control over what the Z projection range is,  and whether or not there is a far clipping plane.
ARX avxClipSpaceDepth ArxGetCameraDepthRange(arxCamera cam, afxReal* epsilon);
ARX void            ArxModifyCameraDepthRange(arxCamera cam, avxClipSpaceDepth range, afxReal epsilon);

/// You must reset the camera's aspect ratios any time the display device, resolution, or window size changes.
ARX void            ArxSetCameraAspectRatios(arxCamera cam, afxReal physAspectRatio, afxV2d const screenExtent, afxV2d const windowExtent);

/// The offset vector has horizontal and vertical offsets as its first two components (respectively), 
/// and the distance from camera as its third component.
ARX void            ArxResetCameraDisplacement(arxCamera cam, afxV3d const displacement);
ARX afxError        ArxConsultCameraDisplacement(arxCamera cam, afxV3d const displace, avxBlendOp op, afxV3d displacement);

/// These methods are useful to make a camera zoom in/out on a specific target. 
ARX afxReal         ArxGetCameraDistance(arxCamera cam);
ARX void            ArxSetCameraDistance(arxCamera cam, afxReal distance);
ARX void            ArxApplyCameraDistance(arxCamera cam, afxReal distance);

/// These methods are useful to make a camera orbit on a specific target. 
/// Note that elevation is measured as rotation about the X(right) axis (hence, it is negative if it is above the horizontal, positive if it is below), 
/// azimuth is measured as rotation about the Y(up) axis, and, and roll is measured as rotation about the Z(back) axis.
/// The EAR vector has the elevation as its first component, azimuth as its second, and roll as its third (all three are in radians). 
ARX void            ArxResetCameraOrientation(arxCamera cam, afxV3d const elevAzimRoll);
ARX afxError        ArxConsultCameraOrientation(arxCamera cam, afxV3d const rotation, avxBlendOp op, afxV3d elevAzimRoll);

/// For picking 3D objects with the mouse, it's necessary to be able to take a point on the screen and figure out where it would be in world-space. Qwadro can do this for you automatically like this:

ARX void            ArxFindWorldCoordinates(arxCamera cam, afxV2d const wh, afxV3d const screenPoint, afxV4d worldPoint);

/// Do a ray-based object picking returning the world position and the normal of a target.
/// Note that the MouseY must be "pre-flipped" to align with the standard 3D alignment of the Y-axis as going up.
/// curY = (wndHeight - 1) - curY;
ARX void            ArxGetCameraPickingRay(arxCamera cam, afxV2d const wh, afxV2d const cursor, afxV4d origin, afxV3d normal);

/// Use this method to project a single point from world space to window space.
ARX void            ArxFindScreenCoordinates(arxCamera cam, afxV2d const wh, afxV4d const worldPoint, afxV3d screenPoint);

/// Transforms a 2D vector of movement into the appropriate 3D vector in world space. 
ARX void            ArxComputeCameraRelativePlanarBases(arxCamera cam, afxBool screenOrthogonal, afxV3d const planeNormal, afxV4d const pointOnPlane, afxV3d xBasis, afxV3d yBasis);
/// The result from GetRelativePlanarBases is two vectors, one which is the world-space vector along which X motion should be interpreted (ex., horizontal movement via the keyboard or analog stick), 
/// and the other is for Y. These vectors are unit length, but they are only orthogonal in world space if you pass false for the second parameter. 
/// If you pass true, this instructs Qwadro to create non-orthgonal bases such that vertical motion (Y motion) will be mapped to a direction in world space which makes the character appear to move directly upwards on the screen, 
/// which is actually not the direction orthogonal to perfect horizontal motion (unless the character is at the very center of the screen).

ARX afxBool         ArxProcessCameraInteraction(arxCamera cam, afxUnit port, afxReal64 speed, afxReal64 dt);

////////////////////////////////////////////////////////////////////////////////

ARX afxError        ArxAcquireCameras(arxRenderware din, afxUnit cnt, arxCamera cameras[]);

ARX afxReal64       ArxFindAllowedCameraLodError(afxReal64 errInPixels, afxInt vpHeightInPixels, afxReal64 fovY, afxReal64 distanceFromCam);

#endif//ARX_CAMERA_H
