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
// This file is part of Acceleration for RenderWare on Qwadro.

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

/*
    The ArxGetCameraDirectionX() retrieves the camera's X-axis direction vectors in world space.
*/

ARX void ArxGetCameraDirectionX
(
    // The camera whose orientation is queried.
    arxCamera cam, 
    // Output vector pointing to the camera's left in world space.
    afxV3d left, 
    // Output vector pointing to the camera's right in world space.
    afxV3d right
);

/*
    The ArxGetCameraDirectionY() retrieves the camera's Y-axis direction vectors in world space.
*/

ARX void ArxGetCameraDirectionY
(
    // The camera whose orientation is queried.
    arxCamera cam, 
    // Output vector pointing downward relative to the camera.
    afxV3d down, 
    // Output vector pointing upward relative to the camera.
    afxV3d up
);

/*
    The ArxGetCameraDirectionZ() retrieves the camera's Z-axis direction vectors in world space.
*/

ARX void ArxGetCameraDirectionZ
(
    // The camera whose position/orientation is queried.
    arxCamera cam, 
    // Output vector pointing toward the "near" (forward) direction.
    afxV3d near, 
    // Output vector pointing toward the "far" (backward) direction.
    afxV3d far
);

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

/*
    The ArxOrbitCamera() method makes a camera orbit on a specific target. 
    Note that elevation is measured as rotation about the X (right) axis (hence, it is negative if it is above the horizontal, positive if it is below), 
    azimuth is measured as rotation about the Y (up) axis, and, and roll is measured as rotation about the Z (back) axis.
    The EAR vector has the elevation as its first component, azimuth as its second, and roll as its third (all three are in radians). 

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

ARX void ArxOrbitCamera
(
    arxCamera cam, 
    afxV3d const elevAzimRoll
);

ARX void ArxGetCameraOrbit
(
    arxCamera cam, 
    afxV3d elevAzimRoll
);

/*
    The ArxGetCameraDepthRange() method retrieves the camera's clip-space depth configuration.

    This function returns the camera's current clip-space depth mode
    (e.g., [0,1], [-1,1], reversed-Z, infinite-far, etc.) and allows you
    to query the Z-range epsilon; a small value used internally to avoid
    precision artifacts when a far clipping plane is removed.
*/

ARX avxClipSpaceDepth ArxGetCameraDepthRange
(
    // The camera whose depth configuration is being queried.
    arxCamera cam, 
    // Optional output pointer that receives the epsilon value used for depth range calculations. 
    // May be NULL if not needed.
    afxReal* epsilon
);

/*
    The ArxAdjustCameraAspectRatio() updates the camera's aspect ratio to match the current display configuration.

    You must call this whenever the display device, window size, resolution,
    or rendering surface changes. This ensures that the camera's projection
    matrix maintains correct proportions and avoids stretching or distortion.
*/

ARX void ArxAdjustCameraAspectRatio
(
    // The camera whose aspect ratio should be updated.
    arxCamera cam, 
    // The physical aspect ratio of the display device.
    afxReal physAspectRatio, 
    // The pixel dimensions of the active framebuffer or screen.
    afxV2d const screenExtent, 
    // The pixel dimensions of the application window.
    afxV2d const windowExtent
);

/*
    The ArxFindWorldCoordinates() method converts a screen-space position to its corresponding world-space location.

    This function is useful for tasks such as object picking or determining
    where a pixel on the screen lies in the 3D scene. Qwadro automatically
    performs the required unprojection from clip space to world space.
*/

ARX void ArxFindWorldCoordinates
(
    // The camera used for the transformation.
    arxCamera cam, 
    // The window or framebuffer dimensions (width, height).
    afxV2d const wh, 
    // The input point in screen space. Typically:
    // screenPoint.x = cursor X, screenPoint.y = cursor Y, screenPoint.z = depth (0=near, 1=far)
    afxV3d const screenPoint, 
    // Output world-space position (homogeneous vec4).
    afxV4d worldPoint
);

/*
    The ArxGetCameraPickingRay() method produces a world-space ray suitable for object picking or intersection tests.

    This converts a 2D cursor coordinate into:
        origin --- the camera-space (or world-space) point where the ray starts  
        normal --- a normalized direction vector pointing forward into the scene

    IMPORTANT: The input cursor Y value *must be pre-flipped* to match
    standard 3D coordinates where +Y goes up:
    flippedY = (wndHeight - 1) - cursorY;
*/

ARX void ArxGetCameraPickingRay
(
    // The camera issuing the ray.
    arxCamera cam, 
    // The window or framebuffer dimensions (width, height).
    afxV2d const wh, 
    // The cursor position (X, pre-flipped Y).
    afxV2d const cursor, 
    // Output: world-space origin of the ray (vec4).
    afxV4d origin, 
    // Output: normalized world-space direction of the ray (vec3).
    afxV3d normal
);

/*
    The ArxFindScreenCoordinates() method projects a world-space point into screen coordinates.
    
    This is the reverse of ArxFindWorldCoordinates() and is commonly used 
    for UI hit-tests, rendering labels, HUD elements, or debugging aids.
*/

ARX void ArxFindScreenCoordinates
(
    // The camera performing the projection.
    arxCamera cam, 
    // The window or framebuffer dimensions (width, height).
    afxV2d const wh, 
    // The world-space location (vec4).
    afxV4d const worldPoint, 
    // Output point in screen space (x, y, depth).
    afxV3d screenPoint
);

/*
    The ArxComputeCameraRelativePlanarBases() computes camera-relative 3D basis vectors for interpreting 2D movement input.

    This function converts a 2D movement vector (e.g., WASD input, analog stick,
    virtual joystick, or drag vector) into two world-space directions lying on
    (or projected onto) a specified plane. These directions can then be multiplied
    by user input to produce correct world-space motion.

    ArxComputeCameraRelativePlanarBases produces two normalized world-space vectors:
        xBasis --- the 3D direction corresponding to horizontal input (X-axis input).
        yBasis --- the 3D direction corresponding to vertical input (Y-axis input).

    These are always unit length, but their orthogonality depends on the screenOrthogonal flag:
        When screenOrthogonal == false (standard planar movement):
            The resulting xBasis and yBasis are orthogonal in world space.
            The bases represent the true camera-relative directions constrained to the plane.
            Useful for physics, consistent traversal, character movement, or navigation on a surface.
        When screenOrthogonal == true (screen-aligned motion):
            The Y-movement basis is adjusted so the character appears to move straight up the screen,
            even if this is not the world-space direction orthogonal to horizontal movement.
            As a result, xBasis and yBasis are no longer orthogonal in world space.
            This is typically used for:
                Top-down or isometric control feel
                Twin-stick shooters
                Pointer-driven movement where screen direction is prioritized over world-space geometry

    This behavior compensates for perspective distortion:
    Unless the player is exactly at the center of the screen, "up on the screen" is not perfectly perpendicular 
    to "right on the screen" in world space. Qwadro resolves this by intentionally breaking world-space 
    orthogonality to preserve screen-space intuitiveness.
*/

ARX void ArxComputeCameraRelativePlanarBases
(
    // The camera whose orientation is used to derive the bases.
    arxCamera cam, 
    // If true, generate screen-orthogonal bases (see notes below).
    afxBool screenOrthogonal, 
    // The normal vector of the plane onto which movement will be constrained.
    afxV3d const planeNormal, 
    // A point lying on the constraint plane.
    afxV4d const pointOnPlane, 
    // Output: world-space basis vector corresponding to +X motion.
    afxV3d xBasis, 
    // Output: world-space basis vector corresponding to +Y motion.
    afxV3d yBasis
);

ARX afxError ArxAdjustCamera(arxCamera cam, avxViewport const* vp);

////////////////////////////////////////////////////////////////////////////////

AFX_DEFINE_STRUCT(arxCameraConfig)
{
    avxClipSpaceDepth   csd;
    avxCanvasConfig     ccfg;
};

ARX afxError        ArxAcquireCameras
(
    arxScenario     scio, 
    afxUnit         cnt, 
    arxCamera       cameras[]
);

/*
    The ArxFindAllowedCameraLodError() function converts an acceptable LOD error measured in screen 
    pixels into an equivalent world-space geometric error allowed at a given distance from the camera.
    This is used for:
        mesh LOD selection
        terrain tessellation
        impostor switching
        error-metric based simplification
    It answers the question: "If I want at most errInPixels pixels of screen-space deviation, 
    how much geometric deviation (in world units) is allowed at distance d?"

    It converts a maximum screen-space error (in pixels) into the corresponding
    world-space geometric error allowed at a given camera distance.

    This is typically used for LOD systems: if a mesh or surface deviates from
    the ideal by less than the returned error value, that deviation will appear
    smaller than 'errInPixels' from the camera's viewpoint.

    If the allowed world-space deviation is 0.08 units, any LOD or tessellation 
    error <= 0.08 will be visually indistinguishable within the desired pixel tolerance.
*/

ARX afxReal64 ArxFindAllowedCameraLodError
// Returns the allowed world-space geometric error at 'distanceFromCam'.
(
    // Maximum pixel deviation allowed on screen.
    afxReal64 errInPixels, 
    // Height of the viewport in pixels.
    afxInt vpHeightInPixels, 
    // Vertical field of view (radians).
    afxReal64 fovY, 
    // Distance from the camera to the object/surface.
    afxReal64 distanceFromCam
);

#endif//ARX_CAMERA_H
