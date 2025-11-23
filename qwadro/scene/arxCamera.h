/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *          Q W A D R O   4 D   R E N D E R I N G   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>
// This file is part of Advanced Renderware Extensions & Experiments for Qwadro.

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

#include "qwadro/render/arxRenderable.h"
#include "qwadro/draw/afxDrawDefs.h"
#include "qwadro/mem/afxArray.h"
#include "qwadro/coll/afxFrustum.h"
#include "qwadro/draw/avxMatrix.h"

typedef enum afxCameraFlag
{
    afxCameraFlag_P     = AFX_BITMASK(0),
    afxCameraFlag_V     = AFX_BITMASK(1),
    afxCameraFlag_PV    = (afxCameraFlag_P | afxCameraFlag_V)
} afxCameraFlags;

/// Reset a camera to its default configuration.
ARX void ArxResetCamera(arxCamera cam);

/*
    The ArxGetCameraMatrices() method retrieves the view matrix and its inverse from the given arxCamera.
    The view matrix (v) transforms coordinates from world space to camera/view space.
    The inverse view matrix (iv) transforms from view space back to world space.
*/

ARX void ArxGetCameraMatrices
(
    // The arxRamera object.
    arxCamera cam, 
    // A optional pointer to a 4x4 matrix that will be filled with the inverse view matrix; the camera/world matrix.
    afxM4d iv, 
    // A optional pointer to a 4x4 matrix that will be filled with the view matrix.
    afxM4d v
);

/*
    The ArxGetProjectionMatrices() method retrieves the projection matrix, its inverse, and combined matrix products for efficient rendering.
    Projection Matrix (P) transforms from view space to clip space (for example, applying perspective or orthographic projection).
    P * V is commonly used to go directly from world space to clip space.
    (P * V)-¹ lets you reconstruct world space positions from screen/clip space.
*/

ARX void ArxGetProjectionMatrices
(
    // The camera object.
    arxCamera cam, 
    // An optional pointer to the inverse projection matrix.
    afxM4d ip, 
    // An optional pointer to the inverse projection * view matrix.
    afxM4d ipv, 
    // An optional pointer to the projection * view matrix.
    afxM4d pv, 
    // An optional pointer to the projection matrix.
    afxM4d p
);

/*
    The ArxTranslateCamera() method moves a camera in 3D space, but relative to the camera's current orientation, not in world space.
    The motion vector is interpreted relative to the camera's current rotation/orientation. So, 
    calling ArxTranslateCamera(cam, {0, 0, -1}) moves the camera forward along where it's currently looking.
    It transforms the motion vector using the camera's basis vectors (right, up, forward) and adds that to the camera's position.

    Examples:
    
    // Move camera forward by 5 units:
    afxV3d forwardMove = { 0.0f, 0.0f, -5.0f };
    ArxTranslateCamera(myCam, forwardMove)

    // Strafe right by 2 units:
    afxV3d strafeRight = { 2.0f, 0.0f, 0.0f };
    ArxTranslateCamera(myCam, strafeRight);

    // Fly up (e.g. for free camera movement):
    afxV3d ascend = { 0.0f, 1.0f, 0.0f };
    ArxTranslateCamera(myCam, ascend);

    To build a full free-fly camera (like FPS or editor-style camera), you'd:
    Use ArxRotateCamera or similar to rotate it.
    Use ArxTranslateCamera for local movement (which adapts based on current orientation).
*/

ARX void ArxTranslateCamera
(
    // An handle to the camera you want to move.
    arxCamera cam, 
    // A 3D vector representing the motion offset.
    afxV3d const motion
);

/*
    The ArxGetCameraTranslation() method retrieves the final world-space position of the camera, 
    including any displacement offset set via ArxDisplaceCamera().
    This is not just the base position of the camera. It returns the effective origin point from which the camera sees the world, 
    for example, the eye point used in the view matrix. It combines the camera's world transform (position + orientation)
    and the displacement vector (set via ArxDisplaceCamera()).

    When the camera is displaced (for example, for orbit, offset views, or stereo rendering), the visual camera isn't physically moved, 
    but the rendering uses an adjusted eye position. This function gives you that adjusted position; the one the view matrix is based on.
    So use this if you need the camera's actual viewing position, not just its logical transform.
*/

ARX void ArxGetCameraTranslation
(
    // The arxCamera object.
    arxCamera cam, 
    // A pointer to the vector that will be set with the position.
    afxV3d origin
);

/*
    The ArxSetCameraOrigin() method sets the world-space position of the camera, excluding any displacement.
    Think of this as setting the camera's anchor point, not necessarily where its "eye" is (unless displacement is zero).
    This sets the base position of the camera in world space; the location from which the view matrix is constructed,
    orientation is applied, and the displacement is eventually added afterward when computing the final eye position.

    If you want the camera to orbit around a target, you might set the target's world position via ArxSetCameraOrigin(),
    set ArxOrbitCamera() to control azimuth/elevation, and set ArxDisplaceCamera() to control orbit distance.
*/

ARX void ArxSetCameraOrigin
(
    arxCamera cam, 
    afxV3d const origin
);

/// Get the world-space vector that points to the camera's left/right, down/up, near/far direction.
ARX void            ArxGetCameraDirectionX(arxCamera cam, afxV3d left, afxV3d right);
ARX void            ArxGetCameraDirectionY(arxCamera cam, afxV3d down, afxV3d up);
ARX void            ArxGetCameraDirectionZ(arxCamera cam, afxV3d near, afxV3d far);

/*
    The ArxGetCameraFov() method retrieves the current vertical field of view (FOV) of the specified camera, in radians.
    This value represents the vertical FOV angle, used in constructing the perspective projection matrix.
*/

ARX afxReal ArxGetCameraFov
(
    // The arxCamera object.
    arxCamera cam,
    // An optional pointer to afxReal which will hold the horizontal FOV.
    afxReal* fovX
);

/*
    The ArxAdjustCameraFov() method is used to adjust the field of view (FOV) of a camera, 
    and it returns the previous FOV that was set. This function can clamp the FOV to a valid range internally 
    (for example, between 0.01 and PI - 0.01 radians).

    Field of View is the vertical angle (in radians) of the camera's viewing cone.
    So, wider FOV is equal to more peripheral view, more distortion (used for fast-paced games or VR),
    and narrow FOV is equal to zoomed-in effect, useful for sniper scopes or cinematic shots.
*/

ARX afxReal ArxAdjustCameraFov
(
    // The arxCamera object.
    arxCamera cam, 
    // The desired vertical field of view, in radians.
    afxReal fov
);

/*
    The ArxSetCameraClipPlanes() method defines the near and far clipping planes of a camera,
    which control the depth range of the camera's view frustum.

    Anything closer than the near clip plane distance from the camera is not rendered.
    Anything further than the far clip plane distance is also not rendered.
    These planes define the depth of the view frustum.

    Changing the near/far clip planes alters the projection matrix, 
    which changes how depth is mapped to the screen, and affects depth precision in the Z-buffer.
    Setting near too small (like 0.001) with a very large far (like 10000) can cause Z-fighting due to floating-point precision loss.
    Ans objects outside the [near, far] range are considered outside the view frustum, so they'll be culled.

    EXAMPLE:
    ArxSetCameraClipPlanes(cam, 0.1f, 1000.0f);
    This sets a near clip at 0.1 units and far at 1000 units.
    Ideal for general 3D scenes.

    Good practices.
    For indoor scenes, you can use [ 0.1, 100 ] for better depth precision.
    For outdoor scenes, you can use [ 1.0, 10000 ] for wider range but worse precision.
    For shadow cascades, you can use [ tuned, tuned ] for adjust per cascade for quality.
*/

ARX void ArxSetCameraClipPlanes
(
    // The arxCamera object.
    arxCamera cam,
    // The distance from the camera to the near clipping plane (must be > 0).
    afxReal near, 
    // The distance from the camera to the far clipping plane (must be > near).
    afxReal far
);

/*
    The ArxGetCameraFrustum() method (potentially recomputes and) gets the camera's view frustum in world space,
    as well as the configured near and far clipping plane values for the frustum.
*/

ARX void ArxGetCameraFrustum
(
    // The arxCamera object.
    arxCamera cam, 
    // An optional pointer to a afxReal which will be set with the near clipping plane distance (from camera).
    afxReal* nearClipPlane, 
    // An optional pointer to a afxReal which will be set with the far clipping plane distance (from camera).
    afxReal* farClipPlane, 
    // An optional pointer to a afxFrustum which will be filled with the computed view frustum volume.
    afxFrustum* frustum
);

/*
    The ArxDisplaceCamera() method defines an offset from the camera origin.
    The offset vector has horizontal, vertical and distance from view as its components, respectively.
    The displacement is applied relative to the camera's orientation. So:
    
    // No offset; camera at its origin.
    displace = {0, 0, 0}
    
    // 1 unit right, 5 units forward from the camera’s local position.
    displace = {1, 0, -5}

    It may be used when computing ray origins, rendering focus points, or camera-relative objects.
    This differs from ArxTranslateCamera() in that TranslateCamera() moves the camera in the world.
    ArxDisplaceCamera() keeps the camera where it is but defines an offset "eye" position used internally for view calculations.
*/

ARX void ArxDisplaceCamera
(
    // The arxCamera object.
    arxCamera cam, 
    // The vector containing the displacement offset.
    afxV3d const displace
);

/*
    The ArxGetCameraDisplacement() method retrieves the current displacement vector from the camera.
    The offset vector has horizontal, vertical and distance from view as its components, respectively.

    EXAMPLE:
    afxV3d currentDisplace;
    ArxGetCameraDisplacement(cam, currentDisplace);
    // Log or modify it
    currentDisplace.z -= 2.0f;  // Zoom out a bit more
    ArxDisplaceCamera(cam, currentDisplace);
*/

ARX void ArxGetCameraDisplacement
(
    // The arxCamera object.
    arxCamera cam,
    // A pointer to the vector which will be filled with the displacement offset.
    afxV3d displace
);

// TODO: Remove ArxGetCameraDistance, ArxSetCameraDistance, ArxApplyCameraDistance?
/// These methods are useful to make a camera zoom in/out on a specific target. 
ARX afxReal         ArxGetCameraDistance(arxCamera cam);
ARX void            ArxSetCameraDistance(arxCamera cam, afxReal distance);

/// These methods are useful to make a camera orbit on a specific target. 
/// Note that elevation is measured as rotation about the X(right) axis (hence, it is negative if it is above the horizontal, positive if it is below), 
/// azimuth is measured as rotation about the Y(up) axis, and, and roll is measured as rotation about the Z(back) axis.
/// The EAR vector has the elevation as its first component, azimuth as its second, and roll as its third (all three are in radians). 

/*
    Elevation   X (Right)   Rotates the camera up/down. Negative means looking above the horizontal (tilt up), positive means below (tilt down).
    Azimuth     Y (Up)      Rotates the camera left/right horizontally. Commonly used to orbit around a target.
    Roll        Z (Back)    Rotates the camera around its forward axis, like tilting the head sideways.

    This uses an orientation model very common in orbit-style cameras, where the camera 
    looks at a target (often at the origin), rotates around it based on azimuth and elevation,
    and roll is typically zero unless explicitly use.

    These functions pair well with ArxDisplaceCamera() to create orbit-style cameras, 
    by setting orientation (azimuth/elevation), setting displacement (distance from target),
    and looking at target from that offset/orientation.

    This abstraction is designed for 3D editors, RTS cams, or 3rd-person views where orientation is more 
    intuitive to manage than raw matrices or quaternions.
*/
ARX void            ArxOrbitCamera(arxCamera cam, afxV3d const elevAzimRoll);
ARX void            ArxGetCameraOrbit(arxCamera cam, afxV3d elevAzimRoll);

/// Take the explicit control over what the Z projection range is,  and whether or not there is a far clipping plane.
ARX avxClipSpaceDepth ArxGetCameraDepthRange(arxCamera cam, afxReal* epsilon);

/// You must reset the camera's aspect ratios any time the display device, resolution, or window size changes.
ARX void            ArxAdjustCameraAspectRatio(arxCamera cam, afxReal physAspectRatio, afxV2d const screenExtent, afxV2d const windowExtent);

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

ARX afxError ArxAdjustCamera(arxCamera cam, avxViewport const* vp);

AFX_DEFINE_STRUCT(arxCameraConfig)
{
    avxClipSpaceDepth   csd;
    avxCanvasConfig     ccfg;
};

ARX afxError        ArxAcquireCameras
(
    arxScenario   scio, 
    afxUnit         cnt, 
    arxCamera       cameras[]
);

ARX afxReal64       ArxFindAllowedCameraLodError
(
    afxReal64 errInPixels, 
    afxInt vpHeightInPixels, 
    afxReal64 fovY, 
    afxReal64 distanceFromCam
);

#endif//ARX_CAMERA_H
