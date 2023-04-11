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
    afxBool                 perspective; // is projection perspective or orthographic.

    afxV4d                  focus, up;

    afxReal                 fov; // in perspective projection only
    afxReal                 nearClip, farClip; // clipping planes

    afxBool                 leftHanded;
    afxInt                  cullingMask;

    afxM4d                  vMtx;
    afxFrustum              frustum;
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