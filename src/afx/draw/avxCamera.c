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
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AVX_DRAW_C
#define _AVX_CAMERA_C
#include "dev/AvxDevKit.h"

_AVXINL void AfxGetCameraMatrices(avxCamera cam, afxM4d v, afxM4d iv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(v || iv);

    if (v)
        AfxCopyM4d(v, cam->v);

    if (iv)
        AfxCopyM4d(iv, cam->iv);
}

_AVXINL void AfxGetCameraProjectiveMatrices(avxCamera cam, afxM4d p, afxM4d ip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(p || ip);

    if (p)
        AfxCopyM4d(p, cam->p);

    if (ip)
        AfxCopyM4d(ip, cam->ip);
}

_AVXINL afxReal AfxGetCameraFov(avxCamera cam)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    return cam->fovY;
}

_AVXINL void AfxSetCameraFov(avxCamera cam, afxReal fov)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    cam->fovY = fov;
}

_AVXINL void AfxSetCameraClipPlanes(avxCamera cam, afxReal near, afxReal far)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    cam->nearClipPlane = near;
    cam->farClipPlane = far;
}

_AVXINL void AfxGetCameraClipPlanes(avxCamera cam, afxReal *near, afxReal *far)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(near || far);

    if (near)
        *near = cam->nearClipPlane;

    if (far)
        *far = cam->farClipPlane;
}

_AVXINL avxClipBoundary AfxGetCameraDepthRange(avxCamera cam)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    return cam->depthRange;
}

_AVXINL void AfxSetCameraDepthRange(avxCamera cam, avxClipBoundary range)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    cam->depthRange = range;
}

_AVXINL afxReal AfxGetCameraDepthRangeEpsilon(avxCamera cam)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    return cam->depthRangeEpsilon;
}

_AVXINL void AfxSetCameraDepthRangeEpsilon(avxCamera cam, afxReal epsilon)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    cam->depthRangeEpsilon = epsilon;
}

_AVXINL void AfxGetCameraDisplacement(avxCamera cam, afxV3d offset)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxCopyV3d(offset, cam->offset);
}

_AVXINL void AfxResetCameraDisplacement(avxCamera cam, afxV3d const offset)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxCopyV3d(cam->offset, offset ? offset : AFX_V4D_IDENTITY);
}

_AVXINL void AfxApplyCameraDisplacement(avxCamera cam, afxV3d const offset)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    AfxAddV3d(cam->offset, cam->offset, offset);
}

_AVXINL afxReal AfxGetCameraDistance(avxCamera cam)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    return cam->offset[2];
}

_AVXINL void AfxSetCameraDistance(avxCamera cam, afxReal distance)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    cam->offset[2] = distance;
}

_AVXINL void AfxApplyCameraDistance(avxCamera cam, afxReal distance)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    cam->offset[2] += distance;
}

_AVXINL void AfxGetCameraOrientation(avxCamera cam, afxV3d elevAzimRoll)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(elevAzimRoll);
    AfxCopyV3d(elevAzimRoll, cam->elevAzimRoll);
}

_AVXINL void AfxResetCameraOrientation(avxCamera cam, afxV3d const elevAzimRoll)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxCopyV3d(cam->elevAzimRoll, elevAzimRoll ? elevAzimRoll : AFX_V4D_IDENTITY);
}

_AVXINL void AfxApplyCameraOrientation(avxCamera cam, afxV3d const elevAzimRoll)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(elevAzimRoll);
    AfxAddV3d(cam->elevAzimRoll, cam->elevAzimRoll, elevAzimRoll);
}

_AVXINL void AfxGetCameraOrientation2(avxCamera cam, afxReal* elevation, afxReal* azimuth, afxReal* roll)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(elevation || azimuth || roll);

    if (elevation)
        *elevation = cam->elevAzimRoll[0];

    if (azimuth)
        *azimuth = cam->elevAzimRoll[1];

    if (roll)
        *roll = cam->elevAzimRoll[2];
}

_AVXINL void AfxApplyCameraOrientation2(avxCamera cam, afxReal elevation, afxReal azimuth, afxReal roll)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    cam->elevAzimRoll[0] += elevation;
    cam->elevAzimRoll[1] += azimuth;
    cam->elevAzimRoll[2] += roll;
}

_AVXINL void AfxGetCameraPosition(avxCamera cam, afxV4d point)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(point);
    AfxCopyAtv4d(point, cam->iv[3]);
}

_AVXINL void AfxGetCameraDirectionX(avxCamera cam, afxV3d left, afxV3d right)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert2(left || right, left != right);

    if (left)
        AfxNegV3d(left, cam->iv[0]);

    if (right)
        AfxCopyV3d(right, cam->iv[0]);
}

_AVXINL void AfxGetCameraDirectionY(avxCamera cam, afxV3d down, afxV3d up)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert2(down || up, down != up);

    if (down)
        AfxNegV3d(down, cam->iv[1]);

    if (up)
        AfxCopyV3d(up, cam->iv[1]);
}

_AVXINL void AfxGetCameraDirectionZ(avxCamera cam, afxV3d back, afxV3d fwd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert2(fwd || back, fwd != back);

    if (back)
        AfxCopyV3d(back, cam->iv[2]);

    if (fwd)
        AfxNegV3d(fwd, cam->iv[2]);
}

_AVXINL void AfxResetCamera(avxCamera cam)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    cam->wpOverHp = 1.33f;
    cam->wrOverHr = 1.33f;
    cam->wwOverHw = 1.f;
    cam->fovY = 1.0471976f;
    cam->nearClipPlane = 0.0001f;// 0.4;
    cam->farClipPlane = 1000.f;
    cam->depthRange = avxClipBoundary_NEG_ONE_TO_ONE;
    cam->useQuatOrient = TRUE;
    cam->depthRangeEpsilon = 0.f;
    AfxZeroV3d(cam->pos);
    AfxResetQuat(cam->orient);
    AfxResetM3d(cam->orientM3d);
    AfxZeroV3d(cam->elevAzimRoll);
    AfxZeroV3d(cam->offset);
    AfxRecomputeCameraMatrices(cam);
}

_AVXINL void AfxSetCameraAspectRatios(avxCamera cam, afxReal physAspectRatio, afxV2d const screenExtent, afxV2d const windowExtent)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(screenExtent);
    AfxAssert(windowExtent);
    cam->wpOverHp = physAspectRatio;
    cam->wrOverHr = screenExtent[0] / screenExtent[1];
    cam->wwOverHw = windowExtent[0] / windowExtent[1];
}

_AVXINL void AfxTranslateCamera(avxCamera cam, afxV3d const motion)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(motion);

    // AfxPreMultiplyAtv3d();

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

_AVXINL void AfxComputeCameraProjectiveMatrices(avxCamera cam, afxM4d p, afxM4d ip)
{
    afxError err = NIL;
    afxReal h = AfxSinf(cam->fovY * 0.5f) / AfxCosf(cam->fovY * 0.5f);
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
    afxReal far = cam->farClipPlane;
    afxReal near = cam->nearClipPlane;
    afxReal epsilon = cam->depthRangeEpsilon;
    afxReal nearMinusFarRecip = 1.f / (near - far);
    afxReal nearTimesFar = near * far;
    
#if 0
    avxClipBoundary depthRange = cam->depthRange;

    if (depthRange == avxClipBoundary_NEG_ONE_TO_ONE)
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
    else if (depthRange != avxClipBoundary_NEG_ONE_TO_ZERO)
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
#else
    switch (cam->depthRange)
    {
    case avxClipBoundary_NEG_ONE_TO_ONE:
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
        break;
    }
    case avxClipBoundary_NEG_ONE_TO_ZERO:
    {
        if (far != 0.f)
        {
            p[2][2] = nearMinusFarRecip * near;
            p[3][2] = nearMinusFarRecip * nearTimesFar;
        }
        break;
    }
    case avxClipBoundary_ZERO_TO_ONE:
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
        break;
    }
    default: AfxThrowError(); break;
    };
#endif
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

_AVXINL void AfxComputeCameraMatrices(avxCamera cam, afxM4d v, afxM4d iv)
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
    AfxClipAtm4dTransposed(iv, v);
    AfxEnsureAtm4d(iv);
    AfxPostMultiplyV3d(c, cam->offset, at);
    AfxAddV3d(iv[3], at, cam->pos);
    iv[3][3] = 1.f;
}

_AVXINL void AfxRecomputeCameraMatrices(avxCamera cam)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxComputeCameraMatrices(cam, cam->v, cam->iv);
    AfxComputeCameraProjectiveMatrices(cam, cam->p, cam->ip);
    AfxRecomputeFrustum(&cam->frustum, cam->v, cam->p);
}

_AVXINL void AfxFindWorldCoordinates(avxCamera cam, afxV2d const wh, afxV3d const screenPoint, afxV4d worldPoint)
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

_AVXINL void AfxFindScreenCoordinates(avxCamera cam, afxV2d const wh, afxV4d const worldPoint, afxV3d screenPoint)
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

    if (cam->depthRange == avxClipBoundary_NEG_ONE_TO_ONE)
        v[2] = v[2] + v[2] - 1.f;

    AfxSetV3d(screenPoint, (v[0] + 1.f) * wh[0] * 0.5f, (v[1] + 1.f) * wh[1] * 0.5f, v[2]);
}

_AVXINL void AfxGetCameraPickingRay(avxCamera cam, afxV2d const wh, afxV2d const cursor, afxV4d origin, afxV3d normal)
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

_AVXINL void AfxComputeCameraRelativePlanarBases(avxCamera cam, afxBool screenOrthogonal, afxV3d const planeNormal, afxV4d const pointOnPlane, afxV3d xBasis, afxV3d yBasis)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(planeNormal);
    AfxAssert(pointOnPlane);
    AfxAssert(xBasis);
    AfxAssert(yBasis);

    AfxGetCameraDirectionX(cam, NIL, xBasis);
    afxV3d v;

    if (!screenOrthogonal) AfxCopyV3d(v, xBasis);
    else
    {
        afxV3d up;
        afxV4d origin, camToPoint;
        AfxGetCameraPosition(cam, origin);
        AfxGetCameraDirectionY(cam, NIL, up);
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
    
    if (len <= 0.0000099999997f)
        AfxZeroV3d(yBasis);
    else
        AfxScaleV3d(yBasis, yBasis, AfxRecip(len));
}

_AVXINL void AfxGetCameraFrustum(avxCamera cam, afxFrustum* frustum)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(frustum);
    AfxCopyFrustum(frustum, &cam->frustum);
}

#if 0
_AVX afxBool AfxProcessCameraInteraction(avxCamera cam, afxNat port, afxReal64 speed, afxReal64 dt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    afxBool updated = FALSE;

    if (AfxLmbIsPressed(port))
    {
        afxV2d delta;
        afxV3d deltaEar;
        AfxGetMouseMotion(0, delta);
        deltaEar[1] = -((afxReal)(delta[0] * AFX_PI / 180.f));
        deltaEar[0] = -((afxReal)(delta[1] * AFX_PI / 180.f));
        deltaEar[2] = 0.f;

        AfxApplyCameraElevAzimRoll(cam, deltaEar);
        updated = TRUE;
    }

    if (AfxRmbIsPressed(port))
    {
        afxV2d delta;
        afxV3d off;
        AfxGetMouseMotion(0, delta);
        off[0] = -((afxReal)(delta[0] * AFX_PI / 180.f));
        off[1] = -((afxReal)(delta[1] * AFX_PI / 180.f));
        off[2] = 0.f;

        AfxApplyCameraDisplacement(cam, off);
        updated = TRUE;
    }

    afxReal w = AfxGetMouseWheelDelta(port);
    w = w / 120.f; // WHEEL_DELTA
    AfxApplyCameraDistance(cam, w);

    if (!speed)
        speed = 30.f;

    afxReal64 frameStep = dt * speed;
    // Note: because the negZ axis is forward, we have to invert the way you'd normally think about the 'W' or 'S' key's action.
    afxReal64 fwdSpeed = (AfxGetKeyPressure(0, afxKey_W) ? -1 : 0.0f) + (AfxGetKeyPressure(0, afxKey_S) ? 1 : 0.0f);
    afxReal64 sideSpeed = (AfxGetKeyPressure(0, afxKey_A) ? -1 : 0.0f) + (AfxGetKeyPressure(0, afxKey_D) ? 1 : 0.0f);
    afxReal64 upSpeed = (AfxGetKeyPressure(0, afxKey_Q) ? -1 : 0.0f) + (AfxGetKeyPressure(0, afxKey_E) ? 1 : 0.0f);
    afxV3d v =
    {
        frameStep * sideSpeed,
        frameStep * upSpeed,
        frameStep * fwdSpeed
    };

    if (AfxSumV3d(v))
    {
        AfxTranslateCamera(cam, v);
        updated = TRUE;
    }
    return updated;
}
#endif

#if 0
_AVX afxBool _AvxCamEventFilter(afxObject *obj, afxObject *watched, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    avxCamera cam = (void*)obj;
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

            AfxApplyCameraDisplacement(cam, off);
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
#endif

_AVX afxError _AvxCamCtorCb(avxCamera cam, afxCookie const *cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    afxDrawInput din = cookie->udd[0];
    AfxAssertObjects(1, &din, afxFcc_DIN);

    cam->perspective = TRUE;

    afxM4d m, m2;
    AfxResetM4d(m);
    AfxResetM4d(m2);
    AfxRecomputeFrustum(&cam->frustum, m, m2);
    
    AfxResetCamera(cam);

    avxClipSpace clip;
    AfxDescribeClipSpace(din, &clip);
    AfxSetCameraDepthRange(cam, clip.boundary);

    return err;
}

_AVX afxError _AvxCamDtorCb(avxCamera cam)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    return err;
}

_AVX afxClassConfig const _AvxCamClsCfg =
{
    .fcc = afxFcc_CAM,
    .name = "Camera",
    .desc = "Device-aware Camera",
    .fixedSiz = sizeof(AFX_OBJECT(avxCamera)),
    .ctor = (void*)_AvxCamCtorCb,
    .dtor = (void*)_AvxCamDtorCb,
#if 0
    .eventFilter = (void*)_AvxCamEventFilter
#endif
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxAcquireCameras(afxDrawInput din, afxNat cnt, avxCamera cam[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);

    afxClass* cls = (afxClass*)AfxGetCameraClass(din);
    AfxAssertClass(cls, afxFcc_CAM);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)cam, (void const*[]) { din }))
        AfxThrowError();

    AfxAssertObjects(cnt, cam, afxFcc_CAM);

    return err;
}

_AVX afxNat AfxEnumerateCameras(afxDrawInput din, afxNat first, afxNat cnt, avxCamera cameras[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    afxClass const* cls = AfxGetCameraClass(din);
    AfxAssertClass(cls, afxFcc_CAM);
    return AfxEnumerateClassInstances(cls, first, cnt, (afxObject*)cameras);
}

_AVX afxNat AfxEvokeCameras(afxDrawInput din, afxBool(*flt)(avxCamera, void*), void* fdd, afxNat first, afxNat cnt, avxCamera cameras[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(cameras);
    AfxAssert(flt);
    AfxAssert(cnt);
    afxClass const* cls = AfxGetCameraClass(din);
    AfxAssertClass(cls, afxFcc_CAM);
    return AfxEvokeClassInstances(cls, (void*)flt, fdd, first, cnt, (afxObject*)cameras);
}

_AVX afxNat AfxInvokeCameras(afxDrawInput din, afxNat first, afxNat cnt, afxBool(*f)(avxCamera, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass const* cls = AfxGetCameraClass(din);
    AfxAssertClass(cls, afxFcc_CAM);
    return AfxInvokeClassInstances(cls, first, cnt, (void*)f, udd);
}

_AVX afxNat AfxInvokeCameras2(afxDrawInput din, afxNat first, afxNat cnt, afxBool(*flt)(avxCamera, void*), void *fdd, afxBool(*f)(avxCamera, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(cnt);
    AfxAssert(flt);
    AfxAssert(f);
    afxClass const* cls = AfxGetCameraClass(din);
    AfxAssertClass(cls, afxFcc_CAM);
    return AfxInvokeClassInstances2(cls, first, cnt, (void*)flt, fdd, (void*)f, udd);
}
