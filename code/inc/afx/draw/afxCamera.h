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
 *                      (c 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

#ifndef AFX_CAMERA_H
#define AFX_CAMERA_H

#include "afx/draw/afxDrawDefs.h"
#include "afx/core/afxArray.h"
#include "afx/math/afxFrustum.h"

#define AFX_CAM_ALIGN AFX_SIMD_ALIGN

/*
    There are a number of annoying details involved in 3D camera specification.
    It is generally desirable to have target-based camera control, with parameteric elevation, azimuth, and roll, and both absolute and camera-relative movement.
    It is also a good idea to correct for physical display device aspect ratios (ie., for compensating between a regular TV and an HDTV, for example).
    Furthermore, there are a number of useful queries you might want to make, such as where the camera is, what its axes are, and where the mouse cursor on the screen would be in world-space (for object picking and such).

    To eliminate these annoyances, and make camera manipulation quick and easy, Qwadro supplies the afxCamera object.
    It helps out with all of the common camera tasks listed above, and quite a few not-so-common ones as well.

    It is often useful to have a camera that orbits and zooms in and out on a specific target.
    You can turn a afxCamera into this type of camera by setting the Elevation/Azimuth/Roll (EAR vector and the Offset vector in the afxCamera structure.
    The EAR vector has the elevation as its first component, azimuth as its second, and roll as its third (all three are in radians).
    The offset vector has horizontal and vertical offsets as its first two components (respectively), and the distance from camera as its third component.

    Note that elevation is measured as rotation about the X (right axis (hence, it is negative if it is above the horizontal, positive if it is below), azimuth is measured as rotation about the Y (up axis, and, and roll is measured as rotation about the Z (back axis.
*/

typedef enum
{
    /// cam will render objects uniformly, with no sense of perspective.
    afxCameraType_ORTHOGRAPHIC,
    /// cam will render objects with perspective intact.
    afxCameraType_PERSPECTIVE
} afxCameraType;

typedef enum afxCameraDepthRange
{
    afxCameraDepthRange_ZERO2ONE, // Direct3D
    afxCameraDepthRange_NEGONE2ONE, // OpenGL (default in Qwadro)
    afxCameraDepthRange_NEGONE2ZERO,
} afxCameraDepthRange;

#ifdef _AFX_DRAW_C
#ifdef _AFX_CAMERA_C
AFX_OBJECT(afxCamera)
{
    afxReal             wpOverHp; // physical w/h
    afxReal             wrOverHr; // resolution w/h
    afxReal             wwOverHw; // window w/h
    afxReal             fovY;
    afxReal             nearClipPlane;
    afxReal             farClipPlane;
    afxReal             depthRangeEpsilon;
    afxCameraDepthRange depthRange;
    afxBool             useQuatOrient;
    afxQuat             orient;
    afxM3d              orientM3d;
    afxV3d              pos;
    afxV3d              elevAzimRoll;
    afxV3d              offset;
    afxM4d              v;
    afxM4d              iv; // inverse view
    afxM4d              p;
    afxM4d              ip; // inverse projection

    afxBool     perspective; // is projection perspective or orthographic.
    afxFrustum  frustum;
};
#endif//_AFX_CAMERA_C
#endif//_AFX_DRAW_C

//static_assert(offsetof(afxCamera, focus % AFX_CAM_ALIGN == 0, "");

AFX void AfxGetCameraFrustum(afxCamera cam, afxFrustum *frustum);

AFX void AfxResetCamera(afxCamera cam);

// From there, you can directly set many values to customize the camera:

AFX afxReal AfxGetCameraFov(afxCamera cam);
AFX void AfxSetCameraFov(afxCamera cam, afxReal fov);

AFX void AfxSetCameraClipPlanes(afxCamera cam, afxReal near, afxReal far);
AFX void AfxGetCameraClipPlanes(afxCamera cam, afxReal *near, afxReal *far);

// After initialization, you need to set the camera's aspect ratios. This must be done any time the display device, resolution, 
// or window size changes (or, to make things simple,  you can simply set it every frame, since it is not terribly expensive):

AFX void AfxSetCameraAspectRatios(afxCamera cam, afxReal physAspectRatio, afxReal const screenExtent[2], afxReal const windowExtent[2]);

// Note that the choice of physical aspect ratio is important in making sure your game looks proper on all display devices.
// For consoles especially, it is important to give the user a choice of aspect ratios, since their home theatre setup may not always be NTSC.
// Qwadro allows you to use any physical aspect ratio (width over height) you like.

// It is often useful to have a camera that orbits and zooms in and out on a specific target. 
// You can turn a afxCamera into this type of camera by setting the Elevation/Azimuth/Roll (EAR) vector and the Offset vector in the afxCamera structure. 
// The EAR vector has the elevation as its first component, azimuth as its second, and roll as its third (all three are in radians). 
// The offset vector has horizontal and vertical offsets as its first two components (respectively), and the distance from camera as its third component:

AFX void    AfxGetCameraOffset(afxCamera cam, afxReal offset[3]);
AFX void    AfxSetCameraOffset(afxCamera cam, afxReal const offset[3]);
AFX void    AfxApplyCameraOffset(afxCamera cam, afxReal const offset[3]);

AFX afxReal AfxGetCameraDistance(afxCamera cam);
AFX void    AfxSetCameraDistance(afxCamera cam, afxReal distance);
AFX void    AfxApplyCameraDistance(afxCamera cam, afxReal distance);

AFX void    AfxGetCameraElevAzimRoll(afxCamera cam, afxReal elevAzimRoll[3]);
AFX void    AfxSetCameraElevAzimRoll(afxCamera cam, afxReal const elevAzimRoll[3]);
AFX void    AfxApplyCameraElevAzimRoll(afxCamera cam, afxReal const elevAzimRoll[3]);

AFX afxReal AfxGetCameraElevation(afxCamera cam);
AFX void    AfxSetCameraElevation(afxCamera cam, afxReal elev);
AFX void    AfxApplyCameraElevation(afxCamera cam, afxReal elev);

AFX afxReal AfxGetCameraAzimuth(afxCamera cam);
AFX void    AfxSetCameraAzimuth(afxCamera cam, afxReal azim);
AFX void    AfxApplyCameraAzimuth(afxCamera cam, afxReal azim);

AFX afxReal AfxGetCameraRoll(afxCamera cam);
AFX void    AfxSetCameraRoll(afxCamera cam, afxReal roll);
AFX void    AfxApplyCameraRoll(afxCamera cam, afxReal roll);

// Note that elevation is measured as rotation about the X(right) axis(hence, it is negative if it is above the horizontal, positive if it is below), 
// azimuth is measured as rotation about the Y(up) axis, and, and roll is measured as rotation about the Z(back) axis.

// For picking 3D objects with the mouse, it's necessary to be able to take a point on the screen and figure out where it would be in world-space. Qwadro can do this for you automatically like this:

AFX void AfxFindWorldCoordinates(afxCamera cam, afxReal const wh[2], afxReal const screenPoint[3], afxReal worldPoint[4]);

// Since ray-based object picking is the most standard use of this function, Qwadro also provides a more hands-free version of the call that gives you all the picking ray information in one step:

AFX void AfxGetCameraPickingRay(afxCamera cam, afxReal const wh[2], afxReal const cursor[2], afxReal origin[4], afxReal normal[3]);

// Note that the MouseY must be "pre-flipped" to align with the standard 3D alignment of the Y-axis as going up.
// Most window systems have Y increasing downwards, whereas 3D graphics traditionally has Y increasing upwards, so you will often have to do a conversion like this before calling GetPickingRay:

// MouseY = (WindowHeight - 1) - MouseY;

// However, that conversion depends entirely on how the windowing system reports mouse cursor locations. 

// Both ScreenToWorld and GetPickingRay rely on the matrices of the camera, so changes made to the camera settings since the last RecomputeMatrices call will not be properly reflected.

// In addition to going from window space to world space, the camera routines also allow you to go in the other direction.
// For example, if you want to project a single point from world space to window space, you can use WorldToWindowSpace like this:

AFX void AfxFindScreenCoordinates(afxCamera cam, afxReal const wh[2], afxReal const worldPoint[4], afxReal screenPoint[3]);

// Obviously, this routine is not optimized for large workloads, so it should only be used for occasional projections.

// Often times on-screen characters are controlled directly by pressing keyboard keys or by manipulating an analog stick on a controller. 
// In order to make this sort of control intuitive, a game must map the direction of the stick into the plane of movement for the character, 
// which requires transforming the 2D vector of the movement into the appropriate 3D vector in world space. 
// You can have Qwadro generate the necessary transform for you by using GetRelativePlanarBases:

AFX void AfxComputeCameraRelativePlanarBases(afxCamera cam, afxBool screenOrthogonal, afxReal const planeNormal[3], afxReal const pointOnPlane[4], afxReal xBasis[3], afxReal yBasis[3]);

// The result from GetRelativePlanarBases is two vectors, one which is the world-space vector along which X motion should be interpreted(ie., horizontal movement via the keyboard or analog stick), 
// and the other is for Y. These vectors are unit length, but they are only orthogonal in world space if you pass false for the second parameter. 
// If you pass true, this instructs Qwadro to create non-orthgonal bases such that vertical motion(Y motion) will be mapped to a direction in world space which makes the character appear to move directly upwards on the screen, 
// which is actually not the direction orthogonal to perfect horizontal motion(unless the character is at the very center of the screen).

// When using a afxCamera with certain graphics APIs and/or certain algorithms, you may find that you need explicit control over what the Z projection range is, 
// and whether or not there is a far clipping plane. To accomodate these needs, the afxCamera has some expert settings that you can adjust:

AFX afxCameraDepthRange AfxGetCameraDepthRange(afxCamera cam);
AFX void                AfxSetCameraDepthRange(afxCamera cam, afxCameraDepthRange range);

AFX afxReal             AfxGetCameraDepthRangeEpsilon(afxCamera cam);
AFX void                AfxSetCameraDepthRangeEpsilon(afxCamera cam, afxReal epsilon);



AFX void    AfxApplyCameraMotion(afxCamera cam, afxReal const motion[3]);

AFX void    AfxRecomputeCameraMatrices(afxCamera cam);

AFX void    AfxGetCameraViewMatrices(afxCamera cam, afxReal v[4][4], afxReal iv[4][4]);
AFX void    AfxGetCameraProjectionMatrices(afxCamera cam, afxReal p[4][4], afxReal ip[4][4]);

AFX void    AfxGetCameraPosition(afxCamera cam, afxReal origin[4]);
AFX void    AfxGetCameraLeft(afxCamera cam, afxReal left[3]);
AFX void    AfxGetCameraRight(afxCamera cam, afxReal right[3]);
AFX void    AfxGetCameraUp(afxCamera cam, afxReal up[3]);
AFX void    AfxGetCameraDown(afxCamera cam, afxReal down[3]);
AFX void    AfxGetCameraForward(afxCamera cam, afxReal fwd[3]);
AFX void    AfxGetCameraBack(afxCamera cam, afxReal back[3]);

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

AFX afxError    AfxAcquireCameras(afxDrawSystem dsys, afxNat cnt, afxCamera cam[]);

AFX afxReal     AfxFindAllowedCameraLodError(afxReal errInPixels, afxInt vpHeightInPixels, afxReal fovY, afxReal distanceFromCam);

// If you don't know what the physical aspect ratio is of the device you're using (for example, if you're using a standard PC, there is no way to determine for sure what kind of monitor is attached), 
// you can either assume square pixels (pass the width of the screen divided by the height), or you can use Qwadro's "best guess": 

AFX afxReal     AfxTryFindPhysicalAspectRatio(afxNat screenWidth, afxNat screenHeight);

#endif//AFX_CAMERA_H