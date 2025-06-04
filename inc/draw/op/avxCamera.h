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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

  //////////////////////////////////////////////////////////////////////////////
 // QWADRO DEVICE-AWARE CAMERA                                               //
//////////////////////////////////////////////////////////////////////////////

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

#ifndef AVX_CAMERA_H
#define AVX_CAMERA_H

#include "qwadro/inc/draw/afxDrawDefs.h"
#include "qwadro/inc/mem/afxArray.h"
#include "qwadro/inc/math/bound/afxFrustum.h"
#include "qwadro/inc/draw/math/avxMatrix.h"

typedef enum afxCameraFlag
{
    afxCameraFlag_P     = AFX_BITMASK(0),
    afxCameraFlag_V     = AFX_BITMASK(1),
    afxCameraFlag_PV    = (afxCameraFlag_P | afxCameraFlag_V)
} afxCameraFlags;

/// Reset a camera to its default configuration.
AVX void            AvxResetCamera(avxCamera cam);

/// Before you use the Qwadro camera matrices, you need to make a call to AvxRecomputeCameraMatrices() to recompute them.
AVX void            AvxRecomputeCameraMatrices(avxCamera cam);

/// Once computed, you can use the Qwadro matrices of the camera to set up your graphics operations.
AVX void            AvxGetCameraMatrices(avxCamera cam, afxM4d p, afxM4d pv, afxM4d v, afxM4d ip, afxM4d ipv, afxM4d iv);

/// Call AvxTranslateCamera() to move a camera relative to its current orientation.
AVX void            AvxTranslateCamera(avxCamera cam, afxV3d const motion);

/// Call AvxGetCameraPosition() to get the world-space position of a camera (taking into account its displacement).
AVX void            AvxGetCameraPosition(avxCamera cam, afxV3d origin);
AVX void            AvxSetCameraPosition(avxCamera cam, afxV3d const origin);

/// Get the world-space vector that points to the camera's left/right, down/up, near/far direction.
AVX void            AvxGetCameraDirectionX(avxCamera cam, afxV3d left, afxV3d right);
AVX void            AvxGetCameraDirectionY(avxCamera cam, afxV3d down, afxV3d up);
AVX void            AvxGetCameraDirectionZ(avxCamera cam, afxV3d near, afxV3d far);

AVX afxReal         AvxGetCameraFov(avxCamera cam);
AVX afxReal         AvxAdjustCameraFov(avxCamera cam, afxReal fov);

/// Set the near and far clip planes.
AVX void            AvxSetCameraClipPlanes(avxCamera cam, afxReal near, afxReal far);
/// Once computed, you can use the frustum volume of the camera to apply culling techniques.
AVX void            AvxGetCameraFrustum(avxCamera cam, afxFrustum* frustum, afxReal* nearClipPlane, afxReal* farClipPlane);

/// Take the explicit control over what the Z projection range is,  and whether or not there is a far clipping plane.
AVX avxClipSpaceDepth AvxGetCameraDepthRange(avxCamera cam, afxReal* epsilon);
AVX void            AvxModifyCameraDepthRange(avxCamera cam, avxClipSpaceDepth range, afxReal epsilon);

/// You must reset the camera's aspect ratios any time the display device, resolution, or window size changes.
AVX void            AvxSetCameraAspectRatios(avxCamera cam, afxReal physAspectRatio, afxV2d const screenExtent, afxV2d const windowExtent);

/// The offset vector has horizontal and vertical offsets as its first two components (respectively), 
/// and the distance from camera as its third component.
AVX void            AvxResetCameraDisplacement(avxCamera cam, afxV3d const displacement);
AVX afxError        AvxConsultCameraDisplacement(avxCamera cam, afxV3d const displace, avxBlendOp op, afxV3d displacement);

/// These methods are useful to make a camera zoom in/out on a specific target. 
AVX afxReal         AvxGetCameraDistance(avxCamera cam);
AVX void            AvxSetCameraDistance(avxCamera cam, afxReal distance);
AVX void            AvxApplyCameraDistance(avxCamera cam, afxReal distance);

/// These methods are useful to make a camera orbit on a specific target. 
/// Note that elevation is measured as rotation about the X(right) axis (hence, it is negative if it is above the horizontal, positive if it is below), 
/// azimuth is measured as rotation about the Y(up) axis, and, and roll is measured as rotation about the Z(back) axis.
/// The EAR vector has the elevation as its first component, azimuth as its second, and roll as its third (all three are in radians). 
AVX void            AvxResetCameraOrientation(avxCamera cam, afxV3d const elevAzimRoll);
AVX afxError        AvxConsultCameraOrientation(avxCamera cam, afxV3d const rotation, avxBlendOp op, afxV3d elevAzimRoll);

/// For picking 3D objects with the mouse, it's necessary to be able to take a point on the screen and figure out where it would be in world-space. Qwadro can do this for you automatically like this:

AVX void            AvxFindWorldCoordinates(avxCamera cam, afxV2d const wh, afxV3d const screenPoint, afxV4d worldPoint);

/// Do a ray-based object picking returning the world position and the normal of a target.
/// Note that the MouseY must be "pre-flipped" to align with the standard 3D alignment of the Y-axis as going up.
/// curY = (wndHeight - 1) - curY;
AVX void            AvxGetCameraPickingRay(avxCamera cam, afxV2d const wh, afxV2d const cursor, afxV4d origin, afxV3d normal);

/// Use this method to project a single point from world space to window space.
AVX void            AvxFindScreenCoordinates(avxCamera cam, afxV2d const wh, afxV4d const worldPoint, afxV3d screenPoint);

/// Transforms a 2D vector of movement into the appropriate 3D vector in world space. 
AVX void            AvxComputeCameraRelativePlanarBases(avxCamera cam, afxBool screenOrthogonal, afxV3d const planeNormal, afxV4d const pointOnPlane, afxV3d xBasis, afxV3d yBasis);
/// The result from GetRelativePlanarBases is two vectors, one which is the world-space vector along which X motion should be interpreted (ex., horizontal movement via the keyboard or analog stick), 
/// and the other is for Y. These vectors are unit length, but they are only orthogonal in world space if you pass false for the second parameter. 
/// If you pass true, this instructs Qwadro to create non-orthgonal bases such that vertical motion (Y motion) will be mapped to a direction in world space which makes the character appear to move directly upwards on the screen, 
/// which is actually not the direction orthogonal to perfect horizontal motion (unless the character is at the very center of the screen).

AVX afxBool         AvxProcessCameraInteraction(avxCamera cam, afxUnit port, afxReal64 speed, afxReal64 dt);

AVX afxBool         AvxBeginSceneCapture(avxCamera cam);
AVX afxBool         AvxEndSceneCapture(avxCamera cam);

////////////////////////////////////////////////////////////////////////////////

AVX afxError        AvxAcquireCameras(afxDrawSystem dsys, afxUnit cnt, avxCamera cameras[]);

/// If you don't know what the physical aspect ratio is of the device you're using 
/// (for example, if you're using a standard PC, there is no way to determine for sure what kind of monitor is attached), 
/// you can either assume square pixels (pass the width of the screen divided by the height), or you can use Qwadro's "best guess": 

AVX afxReal64       AvxFindPhysicalAspectRatio(afxUnit screenWidth, afxUnit screenHeight);

AVX afxReal64       AvxFindAllowedCameraLodError(afxReal64 errInPixels, afxInt vpHeightInPixels, afxReal64 fovY, afxReal64 distanceFromCam);

#endif//AVX_CAMERA_H
