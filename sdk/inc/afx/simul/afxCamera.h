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
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

#ifndef AFX_CAMERA_H
#define AFX_CAMERA_H

#include "afx/core/mem/afxArray.h"
#include "../math/volume/afxFrustum.h"
#include "dag/afxNode.h"

#define AFX_CAM_ALIGN AFX_SIMD_ALIGN

/*
    There are a number of annoying details involved in 3D camera specification.
    It is generally desirable to have target-based camera control, with parameteric elevation, azimuth, and roll, and both absolute and camera-relative movement.
    It is also a good idea to correct for physical display device aspect ratios (ie., for compensating between a regular TV and an HDTV, for example).
    Furthermore, there are a number of useful queries you might want to make, such as where the camera is, what its axes are, and where the mouse cursor on the screen would be in world-space (for object picking and such).

    To eliminate these annoyances, and make camera manipulation quick and easy, Qwadro supplies the afxCamera object.
    It helps out with all of the common camera tasks listed above, and quite a few not-so-common ones as well.

    It is often useful to have a camera that orbits and zooms in and out on a specific target.
    You can turn a afxCamera into this type of camera by setting the Elevation/Azimuth/Roll (EAR) vector and the Offset vector in the afxCamera structure.
    The EAR vector has the elevation as its first component, azimuth as its second, and roll as its third (all three are in radians).
    The offset vector has horizontal and vertical offsets as its first two components (respectively), and the distance from camera as its third component.

    Note that elevation is measured as rotation about the X (right) axis (hence, it is negative if it is above the horizontal, positive if it is below), azimuth is measured as rotation about the Y (up) axis, and, and roll is measured as rotation about the Z (back) axis.


*/

AFX_DEFINE_HANDLE(afxCamera);

typedef enum
{
    /// Camera will render objects uniformly, with no sense of perspective.
    AFX_CAM_ORTHOGRAPHIC,
    /// Camera will render objects with perspective intact.
    AFX_CAM_PERSPECTIVE
} afxCameraType;

AFX_OBJECT(afxCamera)
{
    AFX_OBJECT(afxNode) nod;
    afxBool             perspective; // is projection perspective or orthographic.

    afxV4d              focus, up;

    afxReal             fov; // in perspective projection only
    afxReal             nearClip, farClip; // clipping planes

    afxBool             leftHanded;
    afxInt              cullingMask;

    afxM4d              vMtx;
    afxFrustum          frustum;
};

//static_assert(offsetof(afxCamera, focus) % AFX_CAM_ALIGN == 0, "");

AFX afxNode     AfxCameraGetNode(afxCamera cam);

AFX afxReal*    AfxCameraGetFocus(afxCamera cam, afxV4d focus);
AFX afxReal*    AfxCameraGetUp(afxCamera cam, afxV4d up);

AFX afxReal     AfxCameraGetFov(afxCamera cam);
AFX void        AfxCameraGetClipPlanes(afxCamera cam, afxReal nearFarClips[2]);

AFX afxInt      AfxCameraGetCullingMask(afxCamera cam);
    
AFX afxV4d*     AfxCameraCopyViewMatrix(afxCamera cam, afxM4d v);
AFX afxFrustum* AfxCameraCopyFrustum(afxCamera cam, afxFrustum *frustum);

AFX afxResult   AfxCameraCaptureNodes(afxCamera cam, afxM4d p, afxNode root, afxArray(afxNode) *captures);
AFX afxResult   AfxCameraCaptureSpecificNodes(afxCamera cam, afxM4d p, afxNode root, afxArray(afxObject) *captures, afxNat typeCnt, afxFcc types[]);

#endif//AFX_CAMERA_H