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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#define _AFX_SIM_C
#define _AFX_CAMERA_C
#define _AFX_SIMULATION_C
#include "afx/sim/rendering/afxCamera.h"
#include "afx/sim/afxSimulation.h"
#include "../afxSimulParadigms.h"
#include "afx/math/afxQuaternion.h"
#include "afx/math/afxMatrix.h"
#include "afx/math/afxVector.h"
#include "afx/math/afxFrustum.h"

_AFX void AfxGetCameraViewMatrices(afxCamera cam, afxReal v[4][4], afxReal iv[4][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    if (v)
        AfxCopyM4d(v, cam->v);

    if (iv)
        AfxCopyM4d(iv, cam->iv);
}

_AFX void AfxGetCameraProjectionMatrices(afxCamera cam, afxReal p[4][4], afxReal ip[4][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    if (p)
        AfxCopyM4d(p, cam->p);

    if (ip)
        AfxCopyM4d(ip, cam->ip);
}

_AFX afxReal AfxGetCameraFov(afxCamera cam)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    return cam->fovY;
}

_AFX void AfxSetCameraFov(afxCamera cam, afxReal fov)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    cam->fovY = fov;
}

_AFX void AfxSetCameraClipPlanes(afxCamera cam, afxReal near, afxReal far)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    cam->nearClipPlane = near;
    cam->farClipPlane = far;
}

_AFX void AfxGetCameraClipPlanes(afxCamera cam, afxReal *near, afxReal *far)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    near[0] = cam->nearClipPlane;
    far[1] = cam->farClipPlane;
}

_AFX afxCameraDepthRange AfxGetCameraDepthRange(afxCamera cam)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    return cam->depthRange;
}

_AFX void AfxSetCameraDepthRange(afxCamera cam, afxCameraDepthRange range)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    cam->depthRange = range;
}

_AFX afxReal AfxGetCameraDepthRangeEpsilon(afxCamera cam)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    return cam->depthRangeEpsilon;
}

_AFX void AfxSetCameraDepthRangeEpsilon(afxCamera cam, afxReal epsilon)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    cam->depthRangeEpsilon = epsilon;
}

_AFX void AfxGetCameraOffset(afxCamera cam, afxReal offset[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    AfxCopyV3d(offset, cam->offset);
}

_AFX void AfxSetCameraOffset(afxCamera cam, afxReal const offset[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    AfxCopyV3d(cam->offset, offset);
}

_AFX void AfxApplyCameraOffset(afxCamera cam, afxReal const offset[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    AfxAddV3d(cam->offset, cam->offset, offset);
}

_AFX afxReal AfxGetCameraDistance(afxCamera cam)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    return cam->offset[2];
}

_AFX void AfxSetCameraDistance(afxCamera cam, afxReal distance)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    cam->offset[2] = distance;
}

_AFX void AfxApplyCameraDistance(afxCamera cam, afxReal distance)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    cam->offset[2] += distance;
}

_AFX void AfxGetCameraElevAzimRoll(afxCamera cam, afxReal elevAzimRoll[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    AfxCopyV3d(elevAzimRoll, cam->elevAzimRoll);
}

_AFX void AfxSetCameraElevAzimRoll(afxCamera cam, afxReal const elevAzimRoll[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    AfxCopyV3d(cam->elevAzimRoll, elevAzimRoll);
}

_AFX void AfxApplyCameraElevAzimRoll(afxCamera cam, afxReal const elevAzimRoll[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    AfxAddV3d(cam->elevAzimRoll, cam->elevAzimRoll, elevAzimRoll);
}

_AFX afxReal AfxGetCameraElevation(afxCamera cam)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    return cam->elevAzimRoll[0];
}

_AFX void AfxSetCameraElevation(afxCamera cam, afxReal elev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    cam->elevAzimRoll[0] = elev;
}

_AFX void AfxApplyCameraElevation(afxCamera cam, afxReal elev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    cam->elevAzimRoll[0] += elev;
}

_AFX afxReal AfxGetCameraAzimuth(afxCamera cam)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    return cam->elevAzimRoll[1];
}

_AFX void AfxSetCameraAzimuth(afxCamera cam, afxReal azim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    cam->elevAzimRoll[1] = azim;
}

_AFX void AfxApplyCameraAzimuth(afxCamera cam, afxReal azim)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    cam->elevAzimRoll[1] += azim;
}

_AFX afxReal AfxGetCameraRoll(afxCamera cam)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    return cam->elevAzimRoll[2];
}

_AFX void AfxSetCameraRoll(afxCamera cam, afxReal roll)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    cam->elevAzimRoll[2] = roll;
}

_AFX void AfxApplyCameraRoll(afxCamera cam, afxReal roll)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    cam->elevAzimRoll[2] += roll;
}

_AFX void AfxGetCameraPosition(afxCamera cam, afxReal point[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(point);
    AfxV4dFromAffineV3d(point, cam->iv[3]);
}

_AFX void AfxGetCameraLeft(afxCamera cam, afxReal left[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(left);
    AfxNegateV3d(left, cam->iv[0]);
}

_AFX void AfxGetCameraRight(afxCamera cam, afxReal right[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(right);
    AfxCopyV3d(right, cam->iv[0]);
}

_AFX void AfxGetCameraUp(afxCamera cam, afxReal up[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(up);
    AfxCopyV3d(up, cam->iv[1]);
}

_AFX void AfxGetCameraDown(afxCamera cam, afxReal down[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(down);
    AfxNegateV3d(down, cam->iv[1]);
}

_AFX void AfxGetCameraForward(afxCamera cam, afxReal fwd[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(fwd);
    AfxNegateV3d(fwd, cam->iv[2]);
}

_AFX void AfxGetCameraBack(afxCamera cam, afxReal back[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(back);
    AfxCopyV3d(back, cam->iv[2]);
}

_AFX void AfxResetCamera(afxCamera cam)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    cam->wpOverHp = 1.33;
    cam->wrOverHr = 1.33;
    cam->wwOverHw = 1.0;
    cam->fovY = 1.0471976;
    cam->nearClipPlane = 0.4;
    cam->farClipPlane = 1000.0;
    cam->depthRange = afxCameraDepthRange_NEGONE2ONE;
    cam->useQuatOrient = TRUE;
    cam->depthRangeEpsilon = 0.0;
    AfxZeroV3d(cam->pos);
    AfxResetQuat(cam->orient);
    AfxResetM3d(cam->orientM3d);
    AfxZeroV3d(cam->elevAzimRoll);
    AfxZeroV3d(cam->offset);
    AfxRecomputeCameraMatrices(cam);
}

_AFX void AfxSetCameraAspectRatios(afxCamera cam, afxReal physAspectRatio, afxReal const screenExtent[2], afxReal const windowExtent[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(screenExtent);
    AfxAssert(windowExtent);
    cam->wpOverHp = physAspectRatio;
    cam->wrOverHr = screenExtent[0] / screenExtent[1];
    cam->wwOverHw = windowExtent[0] / windowExtent[1];
}

_AFX void AfxApplyCameraMotion(afxCamera cam, afxReal const motion[3])
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

_AFX void AfxComputeCameraProjectionMatrices(afxCamera cam, afxReal p[4][4], afxReal ip[4][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(p);
    AfxAssert(ip);

    afxReal halfFovY = cam->fovY * 0.5f;
    afxReal halfFovSinOverCos = AfxSin(halfFovY) / AfxCos(halfFovY);    
    afxReal nearMinusFarRecip = 1.f / (cam->nearClipPlane - cam->farClipPlane);
    afxReal nearTimesFar = cam->nearClipPlane * cam->farClipPlane;
    afxReal farClipPlane = cam->farClipPlane;

    AfxZeroM4d(p);
    p[0][0] = cam->wrOverHr / (cam->wwOverHw * cam->wpOverHp * halfFovSinOverCos);
    p[1][1] = 1.f / halfFovSinOverCos;
    p[2][3] = -1.f;

    if (cam->depthRange == afxCameraDepthRange_NEGONE2ONE)
    {
        if (farClipPlane == 0.f)
        {
            p[2][2] = cam->depthRangeEpsilon - 1.f;
            p[3][2] = (cam->depthRangeEpsilon - 2.f) * cam->nearClipPlane;
        }
        else
        {
            p[2][2] = (cam->nearClipPlane + cam->farClipPlane) * nearMinusFarRecip;
            p[3][2] = nearMinusFarRecip * nearTimesFar + nearMinusFarRecip * nearTimesFar;
        }
    }
    else if (cam->depthRange != afxCameraDepthRange_NEGONE2ZERO)
    {
        if (farClipPlane == 0.f)
        {
            p[2][2] = cam->depthRangeEpsilon - 1.f;
            p[3][2] = (cam->depthRangeEpsilon - 1.f) * cam->nearClipPlane;
        }
        else
        {
            p[2][2] = nearMinusFarRecip * cam->farClipPlane;
            p[3][2] = nearMinusFarRecip * nearTimesFar;
        }
    }
    else if (farClipPlane != 0.f)
    {
        p[2][2] = nearMinusFarRecip * cam->nearClipPlane;
        p[3][2] = nearMinusFarRecip * nearTimesFar;
    }

    AfxZeroM4d(ip);
    ip[0][0] = 1.f / p[0][0];    
    ip[1][1] = 1.f / p[1][1];    
    ip[2][3] = 1.f / p[3][2];
    ip[3][2] = 1.f / p[2][3];
    ip[3][3] = -(p[2][2] / (p[3][2] * p[2][3]));    
}

_AFX void AfxComputeCameraViewMatrices(afxCamera cam, afxReal v[4][4], afxReal iv[4][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(v);
    AfxAssert(iv);
    afxM3d a, b, c;

    if (!cam->useQuatOrient)
        AfxCopyM3d(a, cam->orientM3d);
    else
        AfxM3dFromQuat(a, cam->orient);

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
    AfxPreMultiplyV3d(at, cam->pos, c);
    at[0] = -(at[0] + cam->offset[0]);
    at[1] = -(at[1] + cam->offset[1]);
    at[2] = -(at[2] + cam->offset[2]);
    at[3] = 1.f;
    AfxM4dFromM3d(v, c, at);
    AfxCopyTransposedAffineM4d(iv, v);
    AfxEnsureAffineM4d(iv);
    AfxPostMultiplyV3d(at, c, cam->offset);
    AfxAddV3d(iv[3], at, cam->pos);
    iv[3][3] = 1.f;
}

_AFX void AfxRecomputeCameraMatrices(afxCamera cam)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxRecomputeFrustum(&cam->frustum, cam->v, cam->p);
    AfxComputeCameraViewMatrices(cam, cam->v, cam->iv);
    AfxComputeCameraProjectionMatrices(cam, cam->p, cam->ip);
}

_AFX void AfxFindWorldCoordinates(afxCamera cam, afxReal const wh[2], afxReal const screenPoint[3], afxReal worldPoint[4])
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
        ((screenPoint[2] + screenPoint[2] - 1.0 - -1.0) * 0.5),
        1.0
    };

    afxV4d v2;
    AfxPreMultiplyV4d(v2, v, cam->ip);
    AfxPreMultiplyV4d(v, v2, cam->iv);
    AfxScaleV3d(worldPoint, v, 1.0 / v[3]);
    worldPoint[3] = 1.0;
}

_AFX void AfxFindScreenCoordinates(afxCamera cam, afxReal const wh[2], afxReal const worldPoint[4], afxReal screenPoint[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(wh);
    AfxAssert(worldPoint);
    AfxAssert(worldPoint[3] == 1.f);
    AfxAssert(screenPoint);

    afxV4d v, v2;
    AfxPreMultiplyV4d(v, worldPoint, cam->v);
    AfxPreMultiplyV4d(v2, v, cam->p);    
    AfxScaleV3d(v, v2, 1.0 / v2[3]);

    if (cam->depthRange == afxCameraDepthRange_NEGONE2ONE)
        v[2] = v[2] + v[2] - 1.0;

    AfxSetV3d(screenPoint, (v[0] + 1.0) * wh[0] * 0.5, (v[1] + 1.0) * wh[1] * 0.5, v[2]);
}

_AFX void AfxGetCameraPickingRay(afxCamera cam, afxReal const wh[2], afxReal const cursor[2], afxReal origin[4], afxReal normal[3])
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
        -1.0, 
        1.0
    };

    afxV4d v2;
    AfxPreMultiplyV4d(v2, v, cam->ip);
    v2[3] = 0.0;
    AfxPreMultiplyV4d(v, v2, cam->iv);
    
    // should normalize or zero
    afxReal len = AfxMagV3d(v);

    if (len <= 0.0000099999997)
        AfxZeroV3d(normal);
    else
        AfxScaleV3d(normal, v, 1.0 / len);
}

_AFX void AfxComputeCameraRelativePlanarBases(afxCamera cam, afxBool screenOrthogonal, afxReal const planeNormal[3], afxReal const pointOnPlane[4], afxReal xBasis[3], afxReal yBasis[3])
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
    afxReal len = AfxMagV3d(yBasis);
    
    if (len <= 0.0000099999997)
        AfxZeroV3d(yBasis);
    else
        AfxScaleV3d(yBasis, yBasis, 1.0 / len);
}

_AFX void AfxGetCameraFrustum(afxCamera cam, afxFrustum* frustum)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(frustum);
    AfxCopyFrustum(frustum, &cam->frustum);
}

_AFX afxBool _AfxCamEventFilter(afxInstance *obj, afxInstance *watched, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxCamera cam = (void*)obj;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    (void)watched;
    (void)ev;

    switch (ev->type)
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
            AfxGetLastMouseMotion(0, delta);
            deltaEar[1] = -((afxReal)(delta[0] * AFX_PI / 180.0));
            deltaEar[0] = -((afxReal)(delta[1] * AFX_PI / 180.0));
            deltaEar[2] = 0;

            AfxApplyCameraElevAzimRoll(cam, deltaEar);
        }
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

_AFX afxError _AfxCamDtor(afxCamera cam)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("cam=%p", cam);
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    return err;
}

_AFX afxError _AfxCamCtor(afxCamera cam, afxCookie const *cookie)
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

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxAcquireCameras(afxSimulation sim, afxNat cnt, afxCamera cam[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    if (AfxAcquireObjects(&sim->cameras, cnt, (afxHandle*)cam, (void*[]) { (void*)NIL }))
        AfxThrowError();

    AfxAssertObjects(cnt, cam, afxFcc_CAM);

    return err;
}

_AFX afxReal AfxTryFindPhysicalAspectRatio(afxInt screenWidth, afxInt screenHeight)
{
    afxReal div = (double)screenWidth / (double)screenHeight;

    if (div <= 1.4)
        return 1.33;

    if (div <= 1.6)
        return 1.5599999;

    return 1.78;
}

_AFX afxReal AfxFindAllowedCameraLodError(afxReal errInPixels, afxInt vpHeightInPixels, afxReal fovY, afxReal distanceFromCam)
{
    return AfxSin(fovY * 0.5) / AfxCos(fovY * 0.5) * errInPixels * distanceFromCam / ((afxReal)vpHeightInPixels * 0.5);
}

_AFX afxClassConfig _AfxCamClsConfig =
{
    .fcc = afxFcc_CAM,
    .name = "Camera",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxCamera)),
    .ctx = NIL,
    .ctor = (void*)_AfxCamCtor,
    .dtor = (void*)_AfxCamDtor,
    .eventFilter = (void*)_AfxCamEventFilter
};
