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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

#define _AFX_CAMERA_C
#define _AFX_DRAW_C
#include "qwadro/draw/afxDrawInput.h"
#include "qwadro/math/afxOpticalMatrix.h"
#include "qwadro/math/afxVector.h"
#include "qwadro/math/afxFrustum.h"
#include "qwadro/core/afxManager.h"
#include "qwadro/draw/afxCamera.h"
#include "qwadro/core/afxSystem.h"
#include "qwadro/math/afxQuaternion.h"

_AVX void AfxGetCameraMatrices(afxCamera cam, afxReal v[4][4], afxReal iv[4][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    if (v)
        AfxCopyM4d(v, cam->v);

    if (iv)
        AfxCopyM4d(iv, cam->iv);
}

_AVX void AfxGetCameraProjectiveMatrices(afxCamera cam, afxReal p[4][4], afxReal ip[4][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    if (p)
        AfxCopyM4d(p, cam->p);

    if (ip)
        AfxCopyM4d(ip, cam->ip);
}

_AVX afxReal AfxGetCameraFov(afxCamera cam)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    return cam->fovY;
}

_AVX void AfxSetCameraFov(afxCamera cam, afxReal fov)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    cam->fovY = fov;
}

_AVX void AfxSetCameraClipPlanes(afxCamera cam, afxReal near, afxReal far)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    cam->nearClipPlane = near;
    cam->farClipPlane = far;
}

_AVX void AfxGetCameraClipPlanes(afxCamera cam, afxReal *near, afxReal *far)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    near[0] = cam->nearClipPlane;
    far[1] = cam->farClipPlane;
}

_AVX afxClipBoundary AfxGetCameraDepthRange(afxCamera cam)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    return cam->depthRange;
}

_AVX void AfxSetCameraDepthRange(afxCamera cam, afxClipBoundary range)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    cam->depthRange = range;
}

_AVX afxReal AfxGetCameraDepthRangeEpsilon(afxCamera cam)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    return cam->depthRangeEpsilon;
}

_AVX void AfxSetCameraDepthRangeEpsilon(afxCamera cam, afxReal epsilon)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    cam->depthRangeEpsilon = epsilon;
}

_AVX void AfxGetCameraOffset(afxCamera cam, afxReal offset[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    AfxCopyV3d(offset, cam->offset);
}

_AVX void AfxSetCameraOffset(afxCamera cam, afxReal const offset[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    AfxCopyV3d(cam->offset, offset);
}

_AVX void AfxApplyCameraOffset(afxCamera cam, afxReal const offset[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    AfxAddV3d(cam->offset, cam->offset, offset);
}

_AVX afxReal AfxGetCameraDistance(afxCamera cam)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    return cam->offset[2];
}

_AVX void AfxSetCameraDistance(afxCamera cam, afxReal distance)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    cam->offset[2] = distance;
}

_AVX void AfxApplyCameraDistance(afxCamera cam, afxReal distance)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    cam->offset[2] += distance;
}

_AVX void AfxGetCameraElevAzimRoll(afxCamera cam, afxReal elevAzimRoll[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    AfxCopyV3d(elevAzimRoll, cam->elevAzimRoll);
}

_AVX void AfxSetCameraElevAzimRoll(afxCamera cam, afxReal const elevAzimRoll[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    AfxCopyV3d(cam->elevAzimRoll, elevAzimRoll);
}

_AVX void AfxApplyCameraElevAzimRoll(afxCamera cam, afxReal const elevAzimRoll[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    AfxAddV3d(cam->elevAzimRoll, cam->elevAzimRoll, elevAzimRoll);
}

_AVX afxReal AfxGetCameraElevation(afxCamera cam)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    return cam->elevAzimRoll[0];
}

_AVX void AfxSetCameraElevation(afxCamera cam, afxReal elev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    cam->elevAzimRoll[0] = elev;
}

_AVX void AfxApplyCameraElevation(afxCamera cam, afxReal elev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    cam->elevAzimRoll[0] += elev;
}

_AVX afxReal AfxGetCameraAzimuth(afxCamera cam)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    return cam->elevAzimRoll[1];
}

_AVX void AfxSetCameraAzimuth(afxCamera cam, afxReal azim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    cam->elevAzimRoll[1] = azim;
}

_AVX void AfxApplyCameraAzimuth(afxCamera cam, afxReal azim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    cam->elevAzimRoll[1] += azim;
}

_AVX afxReal AfxGetCameraRoll(afxCamera cam)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    return cam->elevAzimRoll[2];
}

_AVX void AfxSetCameraRoll(afxCamera cam, afxReal roll)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    cam->elevAzimRoll[2] = roll;
}

_AVX void AfxApplyCameraRoll(afxCamera cam, afxReal roll)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    cam->elevAzimRoll[2] += roll;
}

_AVX void AfxGetCameraPosition(afxCamera cam, afxReal point[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(point);
    AfxCopyAtv4d(point, cam->iv[3]);
}

_AVX void AfxGetCameraLeft(afxCamera cam, afxReal left[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(left);
    AfxNegV3d(left, cam->iv[0]);
}

_AVX void AfxGetCameraRight(afxCamera cam, afxReal right[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(right);
    AfxCopyV3d(right, cam->iv[0]);
}

_AVX void AfxGetCameraUp(afxCamera cam, afxReal up[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(up);
    AfxCopyV3d(up, cam->iv[1]);
}

_AVX void AfxGetCameraDown(afxCamera cam, afxReal down[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(down);
    AfxNegV3d(down, cam->iv[1]);
}

_AVX void AfxGetCameraForward(afxCamera cam, afxReal fwd[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(fwd);
    AfxNegV3d(fwd, cam->iv[2]);
}

_AVX void AfxGetCameraBack(afxCamera cam, afxReal back[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(back);
    AfxCopyV3d(back, cam->iv[2]);
}

_AVX void AfxResetCamera(afxCamera cam)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    afxDrawInput din = AfxGetObjectProvider(cam);
    AfxAssertObjects(1, &din, afxFcc_DIN);
    afxClipSpace clip;
    AfxDescribeClipSpace(din, &clip);

    cam->wpOverHp = 1.33f;
    cam->wrOverHr = 1.33f;
    cam->wwOverHw = 1.f;
    cam->fovY = 1.0471976f;
    cam->nearClipPlane = 0.0001f;// 0.4;
    cam->farClipPlane = 1000.f;
    cam->depthRange = clip.boundary; //afxClipBoundary_NEG_ONE_TO_ONE;
    cam->useQuatOrient = TRUE;
    cam->depthRangeEpsilon = 0.f;
    AfxZeroV3d(cam->pos);
    AfxResetQuat(cam->orient);
    AfxResetM3d(cam->orientM3d);
    AfxZeroV3d(cam->elevAzimRoll);
    AfxZeroV3d(cam->offset);
    AfxRecomputeCameraMatrices(cam);
}

_AVX void AfxSetCameraAspectRatios(afxCamera cam, afxReal physAspectRatio, afxReal const screenExtent[2], afxReal const windowExtent[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(screenExtent);
    AfxAssert(windowExtent);
    cam->wpOverHp = physAspectRatio;
    cam->wrOverHr = screenExtent[0] / screenExtent[1];
    cam->wwOverHw = windowExtent[0] / windowExtent[1];
}

_AVX void AfxApplyCameraMotion(afxCamera cam, afxReal const motion[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(motion);

    cam->pos[0] = motion[0] * cam->iv[0][0] + cam->pos[0];
    cam->pos[1] = motion[0] * cam->iv[0][1] + cam->pos[1];
    cam->pos[2] = motion[0] * cam->iv[0][2] + cam->pos[2];

    cam->pos[0] = motion[1] * cam->iv[1][0] + cam->pos[0];
    cam->pos[1] = motion[1] * cam->iv[1][1] + cam->pos[1];
    cam->pos[2] = motion[1] * cam->iv[1][2] + cam->pos[2];

    cam->pos[0] = motion[2] * cam->iv[2][0] + cam->pos[0];
    cam->pos[1] = motion[2] * cam->iv[2][1] + cam->pos[1];
    cam->pos[2] = motion[2] * cam->iv[2][2] + cam->pos[2];
}

_AVX void AfxComputeCameraProjectiveMatrices(afxCamera cam, afxReal p[4][4], afxReal ip[4][4])
{
    afxReal64 h = AfxSin(cam->fovY * 0.5f) / AfxCos(cam->fovY * 0.5f);
    p[0][0] = cam->wrOverHr / (cam->wwOverHw * cam->wpOverHp * h);
    p[0][1] = 0.f;
    p[0][2] = 0.f;
    p[0][3] = 0.f;
    p[1][0] = 0.f;
    p[1][1] = 1.f / h;
    p[1][2] = 0.f;
    p[1][3] = 0.f;
    p[2][0] = 0.f;
    p[2][1] = 0.f;
    p[2][3] =-1.f;
    p[3][0] = 0.f;
    p[3][1] = 0.f;
    p[3][3] = 0.f;
    afxClipBoundary depthRange = cam->depthRange;
    afxReal far = cam->farClipPlane;
    afxReal near = cam->nearClipPlane;
    afxReal epsilon = cam->depthRangeEpsilon;
    afxReal nearMinusFarRecip = 1.f / (near - far);
    afxReal nearTimesFar = near * far;
    
    if (depthRange == afxClipBoundary_NEG_ONE_TO_ONE)
    {
        if (far == 0.f)
        {
            p[2][2] = epsilon - 1.f;
            p[3][2] = (epsilon - 2.f) * near;
        }
        else
        {
            p[2][2] = nearMinusFarRecip * (near + far);
            p[3][2] = nearMinusFarRecip * nearTimesFar + nearMinusFarRecip * nearTimesFar;
        }
    }
    else if (depthRange != afxClipBoundary_NEG_ONE_TO_ZERO)
    {
        if (far == 0.f)
        {
            p[2][2] = epsilon - 1.f;
            p[3][2] = (epsilon - 1.f) * near;
        }
        else
        {
            p[2][2] = nearMinusFarRecip * far;
            p[3][2] = nearMinusFarRecip * nearTimesFar;
        }
    }
    else if (far != 0.f)
    {
        p[2][2] = nearMinusFarRecip * near;
        p[3][2] = nearMinusFarRecip * nearTimesFar;
    }
    ip[0][0] = 1.f / p[0][0];
    ip[0][1] = 0.f;
    ip[0][2] = 0.f;
    ip[0][3] = 0.f;
    ip[1][0] = 0.f;
    ip[1][1] = 1.f / p[1][1];
    ip[1][2] = 0.f;
    ip[1][3] = 0.f;
    ip[2][0] = 0.f;
    ip[2][1] = 0.f;
    ip[2][2] = 0.f;
    ip[2][3] = 1.f / p[3][2];
    ip[3][0] = 0.f;
    ip[3][1] = 0.f;
    ip[3][2] = 1.f / p[2][3];
    ip[3][3] = -(p[2][2] / (p[3][2] * p[2][3]));
}

_AVX void AfxComputeCameraMatrices(afxCamera cam, afxReal v[4][4], afxReal iv[4][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(v);
    AfxAssert(iv);
    afxM3d a, b, c;

    if (!cam->useQuatOrient)
        AfxCopyM3d(a, cam->orientM3d);
    else
        AfxRotationM3dFromQuat(a, cam->orient);

    afxV3d cosv, sinv;
    AfxCosV3d(cosv, cam->elevAzimRoll);
    AfxSinV3d(sinv, cam->elevAzimRoll);

    b[0][0] = cosv[1];
    b[0][1] = 0.f;
    b[0][2] = sinv[1];
    b[1][0] = 0.f;
    b[1][1] = 1.f;
    b[1][2] = 0.f;
    b[2][0] = -sinv[1];
    b[2][1] = 0.f;
    b[2][2] = cosv[1];
    AfxMultiplyM3d(c, a, b);
    b[0][0] = 1.f;
    b[0][1] = 0.f;
    b[0][2] = 0.f;
    b[1][0] = 0.f;
    b[1][1] = cosv[0];
    b[1][2] = -sinv[0];
    b[2][0] = 0.f;
    b[2][1] = sinv[0];
    b[2][2] = cosv[0];
    AfxMultiplyM3d(a, c, b);
    b[0][0] = cosv[2];
    b[0][1] = -sinv[2];
    b[0][2] = 0.f;
    b[1][0] = sinv[2];
    b[1][1] = cosv[2];
    b[1][2] = 0.f;
    b[2][0] = 0.f;
    b[2][1] = 0.f;
    b[2][2] = 1.f;
    AfxMultiplyM3d(c, a, b);

    afxV4d at;
    AfxPreMultiplyV3d(c, cam->pos, at);
    at[0] = -(at[0] + cam->offset[0]);
    at[1] = -(at[1] + cam->offset[1]);
    at[2] = -(at[2] + cam->offset[2]);
    at[3] = 1.f;
    AfxMakeM4dFromM3d(v, c, at);
    AfxPickAtm4dTransposed(v, iv);
    AfxEnsureAtm4d(iv);
    AfxPostMultiplyV3d(c, cam->offset, at);
    AfxAddV3d(iv[3], at, cam->pos);
    iv[3][3] = 1.f;
}

_AVX void AfxRecomputeCameraMatrices(afxCamera cam)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxRecomputeFrustum(&cam->frustum, cam->v, cam->p);
    AfxComputeCameraMatrices(cam, cam->v, cam->iv);
    AfxComputeCameraProjectiveMatrices(cam, cam->p, cam->ip);
}

_AVX void AfxFindWorldCoordinates(afxCamera cam, afxReal const wh[2], afxReal const screenPoint[3], afxReal worldPoint[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(wh);
    AfxAssert(screenPoint);
    AfxAssert(worldPoint);

    afxV4d v =
    {
        ((screenPoint[0] + screenPoint[0]) - wh[0]) / wh[0],
        ( screenPoint[1] + screenPoint[1]  - wh[1]) / wh[1],
        ((screenPoint[2] + screenPoint[2] - 1.f - -1.f) * 0.5f),
        1.f
    };

    afxV4d v2;
    AfxPreMultiplyV4d(cam->ip, v, v2);
    AfxPreMultiplyV4d(cam->iv, v2, v);
    AfxScaleV3d(worldPoint, v, 1.f / v[3]);
    worldPoint[3] = 1.f;
}

_AVX void AfxFindScreenCoordinates(afxCamera cam, afxReal const wh[2], afxReal const worldPoint[4], afxReal screenPoint[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(wh);
    AfxAssert(worldPoint);
    AfxAssert(worldPoint[3] == 1.f);
    AfxAssert(screenPoint);

    afxV4d v, v2;
    AfxPreMultiplyV4d(cam->v, worldPoint, v);
    AfxPreMultiplyV4d(cam->p, v, v2);
    AfxScaleV3d(v, v2, 1.f / v2[3]);

    if (cam->depthRange == afxClipBoundary_NEG_ONE_TO_ONE)
        v[2] = v[2] + v[2] - 1.f;

    AfxSetV3d(screenPoint, (v[0] + 1.0) * wh[0] * 0.5f, (v[1] + 1.0) * wh[1] * 0.5f, v[2]);
}

_AVX void AfxGetCameraPickingRay(afxCamera cam, afxReal const wh[2], afxReal const cursor[2], afxReal origin[4], afxReal normal[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(wh);
    AfxAssert(cursor);
    AfxAssert(origin);
    AfxAssert(normal);

    AfxGetCameraPosition(cam, origin);
    afxV4d v =
    {
        (cursor[0] + cursor[0] - wh[0]) / wh[0], 
        (cursor[1] + cursor[1] - wh[1]) / wh[1], 
        -1.f, 
        1.f
    };

    afxV4d v2;
    AfxPreMultiplyV4d(cam->ip, v, v2);
    v2[3] = 0.0;
    AfxPreMultiplyV4d(cam->iv, v2, v);
    
    // should normalize or zero
    afxReal len = AfxMagV3d(v);

    if (len <= 0.0000099999997f)
        AfxZeroV3d(normal);
    else
        AfxScaleV3d(normal, v, 1.f / len);
}

_AVX void AfxComputeCameraRelativePlanarBases(afxCamera cam, afxBool screenOrthogonal, afxReal const planeNormal[3], afxReal const pointOnPlane[4], afxReal xBasis[3], afxReal yBasis[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(planeNormal);
    AfxAssert(pointOnPlane);
    AfxAssert(xBasis);
    AfxAssert(yBasis);

    AfxGetCameraRight(cam, xBasis);
    afxV3d v;

    if (!screenOrthogonal) AfxCopyV3d(v, xBasis);
    else
    {
        afxV3d up;
        afxV4d origin, camToPoint;
        AfxGetCameraPosition(cam, origin);
        AfxGetCameraUp(cam, up);
        AfxSubV4d(camToPoint, pointOnPlane, origin);
        v[0] = up[2] * camToPoint[1] - up[1] * camToPoint[2];
        v[1] = up[0] * camToPoint[2] - up[2] * camToPoint[0];
        v[2] = up[1] * camToPoint[0] - up[0] * camToPoint[1];
    }

    yBasis[0] = (v[2] * planeNormal[1]) - (v[1] * planeNormal[2]);
    yBasis[1] = (v[0] * planeNormal[2]) - (v[2] * planeNormal[0]);
    yBasis[2] = (v[1] * planeNormal[0]) - (v[0] * planeNormal[1]);

    // should normalize or zero
    afxReal64 len = AfxMagV3d(yBasis);
    
    if (len <= 0.0000099999997f)
        AfxZeroV3d(yBasis);
    else
        AfxScaleV3d(yBasis, yBasis, AfxRecip(len));
}

_AVX void AfxGetCameraFrustum(afxCamera cam, afxFrustum* frustum)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(frustum);
    AfxCopyFrustum(frustum, &cam->frustum);
}

_AVX afxBool _AfxCamEventFilter(afxHandle *obj, afxHandle *watched, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxCamera cam = (void*)obj;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    (void)watched;
    (void)ev;

    switch (ev->id)
    {
    case AFX_EVENT_MSE_AXIS_UPDATED:
    {
        //afxMouse mse = (void*)watched;
        //AfxAssertObjects(1, &mse, afxFcc_MSE);

        // TODO Leva isso para o usuário

        if (AfxLmbIsPressed(0))
        {
            afxV2d delta;
            afxV3d deltaEar;
            AfxGetMouseMotion(0, delta);
            deltaEar[1] = -((afxReal)(delta[0] * AFX_PI / 180.f));
            deltaEar[0] = -((afxReal)(delta[1] * AFX_PI / 180.f));
            deltaEar[2] = 0.f;

            AfxApplyCameraElevAzimRoll(cam, deltaEar);
        }



        if (AfxRmbIsPressed(0))
        {
            afxV2d delta;
            afxV3d off;
            AfxGetMouseMotion(0, delta);
            off[0] = -((afxReal)(delta[0] * AFX_PI / 180.f));
            off[1] = -((afxReal)(delta[1] * AFX_PI / 180.f));
            off[2] = 0.f;

            AfxApplyCameraOffset(cam, off);
        }
        break;
    }
    case AFX_EVENT_MSE_WHEEL_UPDATED:
    {
        afxReal w = AfxGetMouseWheelDelta(0);
        w = w / 120.f; // WHEEL_DELTA
        AfxApplyCameraDistance(cam, w);
        break;
    }
    case AFX_EVENT_KBD_PRESSED:
    {
        //afxKeyboard kbd = (void*)watched;
        //AfxAssertObjects(1, &kbd, afxFcc_KBD);
        break;
    }
    default:break;
    }
    return FALSE;
}

_AVX afxError _AfxCamCtor(afxCamera cam, afxCookie const *cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("cam=%p", cam);
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    (void)cookie;

    cam->perspective = TRUE;

    afxM4d m, m2;
    AfxResetM4d(m);
    AfxResetM4d(m2);
    AfxRecomputeFrustum(&(cam->frustum), m, m2);
    
    AfxResetCamera(cam);

    return err;
}

_AVX afxError _AfxCamDtor(afxCamera cam)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("cam=%p", cam);
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    return err;
}

_AVX afxClassConfig const _AfxCamClsConfig =
{
    .fcc = afxFcc_CAM,
    .name = "Camera",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxCamera)),
    .mmu = NIL,
    .ctor = (void*)_AfxCamCtor,
    .dtor = (void*)_AfxCamDtor,
    .eventFilter = (void*)_AfxCamEventFilter
};

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxAcquireCameras(afxDrawInput din, afxNat cnt, afxCamera cam[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);

    afxManager* cls = AfxGetCameraClass(din);
    AfxAssertClass(cls, afxFcc_CAM);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)cam, (void const*[]) { (void*)NIL }))
        AfxThrowError();

    AfxAssertObjects(cnt, cam, afxFcc_CAM);

    return err;
}

_AVX afxNat AfxEnumerateCameras(afxDrawInput din, afxNat first, afxNat cnt, afxCamera cam[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    afxManager *cls = AfxGetCameraClass(din);
    AfxAssertClass(cls, afxFcc_CAM);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)cam);
}

_AVX afxReal AfxDeterminePhysicalAspectRatio(afxNat screenWidth, afxNat screenHeight)
{
    afxReal64 div = (afxReal64)screenWidth / (afxReal64)screenHeight;

    if (div <= 1.4)
        return 1.33f;

    if (div <= 1.6)
        return 1.5599999f;

    return 1.78f;
}

_AVX afxReal AfxDetermineAllowedCameraLodError(afxReal errInPixels, afxInt vpHeightInPixels, afxReal fovY, afxReal distanceFromCam)
{
    return AfxSin(fovY * 0.5f) / AfxCos(fovY * 0.5f) * errInPixels * distanceFromCam / ((afxReal)vpHeightInPixels * 0.5f);
}
