/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                      (c 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

#ifndef AFX_CAMERA_H
#define AFX_CAMERA_H

#include "afx/core/mem/afxArray.h"
#include "afx/math/volume/afxFrustum.h"
#include "../afxNode.h"

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

AFX_DEFINE_HANDLE(afxCamera);

typedef enum
{
    /// cam will render objects uniformly, with no sense of perspective.
    AFX_CAM_ORTHOGRAPHIC,
    /// cam will render objects with perspective intact.
    AFX_CAM_PERSPECTIVE
} afxCameraType;

typedef enum afxCameraDepthRange
{
    AFX_CAM_OUT_DEPTH_RANGE_ZERO2ONE = 0x0, // Direct3D
    AFX_CAM_OUT_DEPTH_RANGE_NEGONE2ONE = 0x1, // OpenGL (default in Qwadro)
    AFX_CAM_OUT_DEPTH_RANGE_NEGONE2ZERO = 0x2,
} afxCameraDepthRange;

AFX_OBJECT(afxCamera)
{
    AFX_OBJECT(afxNode) nod;
#ifdef _AFX_CAMERA_C
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
#endif
};

//static_assert(offsetof(afxCamera, focus % AFX_CAM_ALIGN == 0, "");

AFX afxNode     AfxCameraGetNode(afxCamera cam);
AFX afxFrustum* AfxCameraCopyFrustum(afxCamera cam, afxFrustum *frustum);
AFX afxResult   AfxCameraCaptureNodes(afxCamera cam, afxM4d p, afxNode root, afxArray(afxNode)*captures);
AFX afxResult   AfxCameraCaptureSpecificNodes(afxCamera cam, afxM4d p, afxNode root, afxArray(afxObject)*captures, afxNat typeCnt, afxFcc types[]);



AFX void    AfxCameraReset(afxCamera cam);

// From there, you can directly set many values to customize the camera:

AFX afxReal AfxCameraGetFov(afxCamera cam);
AFX void    AfxCameraSetFov(afxCamera cam, afxReal fov);

AFX void    AfxCameraSetClipPlanes(afxCamera cam, afxReal near, afxReal far);
AFX void    AfxCameraGetClipPlanes(afxCamera cam, afxReal *near, afxReal *far);

// After initialization, you need to set the camera's aspect ratios. This must be done any time the display device, resolution, 
// or window size changes (or, to make things simple,  you can simply set it every frame, since it is not terribly expensive):

AFX void    AfxCameraSetAspectRatios(afxCamera cam, afxReal physicalAspectRatio, afxV2d const screenExtent, afxV2d const windowExtent);

// Note that the choice of physical aspect ratio is important in making sure your game looks proper on all display devices.
// For consoles especially, it is important to give the user a choice of aspect ratios, since their home theatre setup may not always be NTSC.
// Qwadro allows you to use any physical aspect ratio (width over height) you like.

// If you don't know what the physical aspect ratio is of the device you're using (for example, if you're using a standard PC, there is no way to determine for sure what kind of monitor is attached), 
// you can either assume square pixels (pass the width of the screen divided by the height), or you can use Qwadro's "best guess": 

AFX afxReal AfxCameraGetMostLikelyPhysicalAspectRatio(afxInt screenWidth, afxInt screenHeight);

// It is often useful to have a camera that orbits and zooms in and out on a specific target. 
// You can turn a afxCamera into this type of camera by setting the Elevation/Azimuth/Roll (EAR) vector and the Offset vector in the afxCamera structure. 
// The EAR vector has the elevation as its first component, azimuth as its second, and roll as its third (all three are in radians). 
// The offset vector has horizontal and vertical offsets as its first two components (respectively), and the distance from camera as its third component:

AFX void    AfxCameraGetOffset(afxCamera cam, afxV3d offset);
AFX void    AfxCameraSetOffset(afxCamera cam, afxV3d const offset);
AFX void    AfxCameraAddOffset(afxCamera cam, afxV3d const offset);

AFX afxReal AfxCameraGetDistance(afxCamera cam);
AFX void    AfxCameraSetDistance(afxCamera cam, afxReal distance);
AFX void    AfxCameraAddDistance(afxCamera cam, afxReal distance);

AFX void    AfxCameraGetElevAzimRoll(afxCamera cam, afxV3d elevAzimRoll);
AFX void    AfxCameraSetElevAzimRoll(afxCamera cam, afxV3d const elevAzimRoll);
AFX void    AfxCameraAddElevAzimRoll(afxCamera cam, afxV3d const elevAzimRoll);

AFX afxReal AfxCameraGetElevation(afxCamera cam);
AFX void    AfxCameraSetElevation(afxCamera cam, afxReal elev);
AFX void    AfxCameraAddElevation(afxCamera cam, afxReal elev);

AFX afxReal AfxCameraGetAzimuth(afxCamera cam);
AFX void    AfxCameraSetAzimuth(afxCamera cam, afxReal azim);
AFX void    AfxCameraAddAzimuth(afxCamera cam, afxReal azim);

AFX afxReal AfxCameraGetRoll(afxCamera cam);
AFX void    AfxCameraSetRoll(afxCamera cam, afxReal roll);
AFX void    AfxCameraAddRoll(afxCamera cam, afxReal roll);

// Note that elevation is measured as rotation about the X(right) axis(hence, it is negative if it is above the horizontal, positive if it is below), 
// azimuth is measured as rotation about the Y(up) axis, and, and roll is measured as rotation about the Z(back) axis.

// For picking 3D objects with the mouse, it's necessary to be able to take a point on the screen and figure out where it would be in world-space. Qwadro can do this for you automatically like this:

AFX void    AfxCameraResolveScreenToWorldCoord(afxCamera cam, afxV2d const wh, afxV3d const screenPoint, afxV4d worldPoint);

// Since ray-based object picking is the most standard use of this function, Qwadro also provides a more hands-free version of the call that gives you all the picking ray information in one step:

AFX void    AfxCameraGetPickingRay(afxCamera cam, afxV2d const wh, afxV2d const cursor, afxV4d origin, afxV3d normal);

// Note that the MouseY must be "pre-flipped" to align with the standard 3D alignment of the Y-axis as going up.
// Most window systems have Y increasing downwards, whereas 3D graphics traditionally has Y increasing upwards, so you will often have to do a conversion like this before calling GetPickingRay:

// MouseY = (WindowHeight - 1) - MouseY;

// However, that conversion depends entirely on how the windowing system reports mouse cursor locations. 

// Both ScreenToWorld and GetPickingRay rely on the matrices of the camera, so changes made to the camera settings since the last RecomputeMatrices call will not be properly reflected.

// In addition to going from window space to world space, the camera routines also allow you to go in the other direction.
// For example, if you want to project a single point from world space to window space, you can use WorldToWindowSpace like this:

AFX void    AfxCameraResolveWorldToScreenCoord(afxCamera cam, afxV2d const wh, afxV4d const worldPoint, afxV3d screenPoint);

// Obviously, this routine is not optimized for large workloads, so it should only be used for occasional projections.

// Often times on-screen characters are controlled directly by pressing keyboard keys or by manipulating an analog stick on a controller. 
// In order to make this sort of control intuitive, a game must map the direction of the stick into the plane of movement for the character, 
// which requires transforming the 2D vector of the movement into the appropriate 3D vector in world space. 
// You can have Qwadro generate the necessary transform for you by using GetRelativePlanarBases:

AFX void    AfxCameraGetRelativePlanarBases(afxCamera cam, afxBool screenOrthogonal, afxV3d const planeNormal, afxV4d const pointOnPlane, afxV3d xBasis, afxV3d yBasis);

// The result from GetRelativePlanarBases is two vectors, one which is the world-space vector along which X motion should be interpreted(ie., horizontal movement via the keyboard or analog stick), 
// and the other is for Y. These vectors are unit length, but they are only orthogonal in world space if you pass false for the second parameter. 
// If you pass true, this instructs Qwadro to create non-orthgonal bases such that vertical motion(Y motion) will be mapped to a direction in world space which makes the character appear to move directly upwards on the screen, 
// which is actually not the direction orthogonal to perfect horizontal motion(unless the character is at the very center of the screen).

// When using a afxCamera with certain graphics APIs and/or certain algorithms, you may find that you need explicit control over what the Z projection range is, 
// and whether or not there is a far clipping plane. To accomodate these needs, the afxCamera has some expert settings that you can adjust:

AFX afxCameraDepthRange AfxCameraGetDepthRange(afxCamera cam);
AFX void                AfxCameraSetDepthRange(afxCamera cam, afxCameraDepthRange range);

AFX afxReal             AfxCameraGetDepthRangeEpsilon(afxCamera cam);
AFX void                AfxCameraSetDepthRangeEpsilon(afxCamera cam, afxReal epsilon);



AFX void    AfxCameraMoveRelative(afxCamera cam, afxV3d const motion);

AFX void    AfxCameraRecomputeMatrices(afxCamera cam);

AFX void    AfxCameraGetViewMatrices(afxCamera cam, afxM4d v, afxM4d iv);
AFX void    AfxCameraGetProjectionMatrices(afxCamera cam, afxM4d p, afxM4d ip);

AFX void    AfxCameraGetViewpoint(afxCamera cam, afxV4d origin);
AFX void    AfxCameraGetLeft(afxCamera cam, afxV3d left);
AFX void    AfxCameraGetRight(afxCamera cam, afxV3d right);
AFX void    AfxCameraGetUp(afxCamera cam, afxV3d up);
AFX void    AfxCameraGetDown(afxCamera cam, afxV3d down);
AFX void    AfxCameraGetForward(afxCamera cam, afxV3d fwd);
AFX void    AfxCameraGetBack(afxCamera cam, afxV3d back);

AFX afxReal AfxCameraFindAllowedLodError(afxReal errInPixels, afxInt vpHeightInPixels, afxReal fovY, afxReal distanceFromCam);

#endif//AFX_CAMERA_H