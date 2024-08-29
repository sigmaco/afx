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
#include "../dev/AvxImplKit.h"

_AVXINL void AfxGetCameraMatrices(afxCamera cam, afxM4d v, afxM4d iv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(v || iv);

    if (v)
        AfxM4dCopy(v, cam->v);

    if (iv)
        AfxM4dCopy(iv, cam->iv);
}

_AVXINL void AfxGetCameraProjectiveMatrices(afxCamera cam, afxM4d p, afxM4d ip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(p || ip);

    if (p)
        AfxM4dCopy(p, cam->p);

    if (ip)
        AfxM4dCopy(ip, cam->ip);
}

_AVXINL afxReal AfxGetCameraFov(afxCamera cam)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    return cam->fovY;
}

_AVXINL void AfxSetCameraFov(afxCamera cam, afxReal fov)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    cam->fovY = fov;
}

_AVXINL void AfxSetCameraClipPlanes(afxCamera cam, afxReal near, afxReal far)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    cam->nearClipPlane = near;
    cam->farClipPlane = far;
}

_AVXINL void AfxGetCameraClipPlanes(afxCamera cam, afxReal *near, afxReal *far)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(near || far);

    if (near)
        *near = cam->nearClipPlane;

    if (far)
        *far = cam->farClipPlane;
}

_AVXINL avxClipBoundary AfxGetCameraDepthRange(afxCamera cam)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    return cam->depthRange;
}

_AVXINL void AfxSetCameraDepthRange(afxCamera cam, avxClipBoundary range)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    cam->depthRange = range;
}

_AVXINL afxReal AfxGetCameraDepthRangeEpsilon(afxCamera cam)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    return cam->depthRangeEpsilon;
}

_AVXINL void AfxSetCameraDepthRangeEpsilon(afxCamera cam, afxReal epsilon)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    cam->depthRangeEpsilon = epsilon;
}

_AVXINL void AfxGetCameraDisplacement(afxCamera cam, afxV3d displacement)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxV3dCopy(displacement, cam->displacement);
}

_AVXINL void AfxResetCameraDisplacement(afxCamera cam, afxV3d const displacement)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxV3dCopy(cam->displacement, displacement ? displacement : AFX_V4D_IDENTITY);
}

_AVXINL void AfxApplyCameraDisplacement(afxCamera cam, afxV3d const displacement)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    AfxV3dAdd(cam->displacement, cam->displacement, displacement);
}

_AVXINL afxReal AfxGetCameraDistance(afxCamera cam)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    return cam->displacement[2];
}

_AVXINL void AfxSetCameraDistance(afxCamera cam, afxReal distance)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    cam->displacement[2] = distance;
}

_AVXINL void AfxApplyCameraDistance(afxCamera cam, afxReal distance)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    cam->displacement[2] += distance;
}

_AVXINL void AfxGetCameraOrientation(afxCamera cam, afxV3d elevAzimRoll)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(elevAzimRoll);
    AfxV3dCopy(elevAzimRoll, cam->elevAzimRoll);
}

_AVXINL void AfxResetCameraOrientation(afxCamera cam, afxV3d const elevAzimRoll)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxV3dCopy(cam->elevAzimRoll, elevAzimRoll ? elevAzimRoll : AFX_V4D_IDENTITY);
}

_AVXINL void AfxApplyCameraOrientation(afxCamera cam, afxV3d const elevAzimRoll)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(elevAzimRoll);
    AfxV3dAdd(cam->elevAzimRoll, cam->elevAzimRoll, elevAzimRoll);
}

_AVXINL void AfxGetCameraOrientation2(afxCamera cam, afxReal* elevation, afxReal* azimuth, afxReal* roll)
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

_AVXINL void AfxApplyCameraOrientation2(afxCamera cam, afxReal elevation, afxReal azimuth, afxReal roll)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    cam->elevAzimRoll[0] += elevation;
    cam->elevAzimRoll[1] += azimuth;
    cam->elevAzimRoll[2] += roll;
}

_AVXINL void AfxGetCameraPosition(afxCamera cam, afxV4d point)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(point);
    AfxV4dCopyAtv3d(point, cam->iv[3]);
}

_AVXINL void AfxGetCameraDirectionX(afxCamera cam, afxV3d left, afxV3d right)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert2(left || right, left != right);

    if (left)
        AfxV3dNeg(left, cam->iv[0]);

    if (right)
        AfxV3dCopy(right, cam->iv[0]);
}

_AVXINL void AfxGetCameraDirectionY(afxCamera cam, afxV3d down, afxV3d up)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert2(down || up, down != up);

    if (down)
        AfxV3dNeg(down, cam->iv[1]);

    if (up)
        AfxV3dCopy(up, cam->iv[1]);
}

_AVXINL void AfxGetCameraDirectionZ(afxCamera cam, afxV3d near, afxV3d far)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert2(far || near, far != near);

    if (near)
        AfxV3dCopy(near, cam->iv[2]);

    if (far)
        AfxV3dNeg(far, cam->iv[2]);
}

_AVXINL void AfxResetCamera(afxCamera cam)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    cam->wpOverHp = 1.33f;
    cam->wrOverHr = 1.33f;
    cam->wwOverHw = 1.f;
    cam->fovY = 1.0471976f;
    cam->nearClipPlane = 0.0001f;// 0.4;
    cam->farClipPlane = 1000000.0;// 1000.f;
    cam->depthRange = avxClipBoundary_NEG_ONE_TO_ONE;
    cam->useQuatOrient = TRUE;
    cam->depthRangeEpsilon = 0.f;
    AfxV3dZero(cam->pos);
    AfxQuatReset(cam->orient);
    AfxM3dReset(cam->orientM3d);
    AfxV3dZero(cam->elevAzimRoll);
    AfxV3dZero(cam->displacement);
    AfxRecomputeCameraMatrices(cam);
}

_AVXINL void AfxSetCameraAspectRatios(afxCamera cam, afxReal physAspectRatio, afxV2d const screenExtent, afxV2d const windowExtent)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(screenExtent);
    AfxAssert(windowExtent);
    cam->wpOverHp = physAspectRatio;
    cam->wrOverHr = screenExtent[0] / screenExtent[1];
    cam->wwOverHw = windowExtent[0] / windowExtent[1];
}

_AVXINL void AfxTranslateCamera(afxCamera cam, afxV3d const motion)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(motion);

    //AfxV3dPreMultiplyLtm4d();

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

_AVXINL void AfxComputeCameraProjectiveMatrices(afxCamera cam, afxM4d p, afxM4d ip)
{
    afxError err = NIL;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    afxReal half = AfxSinf(cam->fovY * 0.5f) / AfxCosf(cam->fovY * 0.5f);
    p[0][0] = cam->wrOverHr / (cam->wwOverHw * cam->wpOverHp * half);
    p[0][1] = 0.f;
    p[0][2] = 0.f;
    p[0][3] = 0.f;
    p[1][0] = 0.f;
    p[1][1] = 1.f / half;
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
    afxReal nearMinFarRecip = 1.f / (near - far);
    afxReal nearTimesFar = near * far;
    
    if (cam->depthRange == avxClipBoundary_NEG_ONE_TO_ONE)
    {
        if (far == 0.f)
        {
            p[2][2] = epsilon - 1.f;
            p[3][2] = (epsilon - 2.f) * near;
        }
        else
        {
            p[2][2] = (near + far) * nearMinFarRecip;
            p[3][2] = nearMinFarRecip * nearTimesFar + nearMinFarRecip * nearTimesFar;
        }
    }
    else if (cam->depthRange != avxClipBoundary_NEG_ONE_TO_ZERO)
    {
        if (far == 0.f)
        {
            p[2][2] = epsilon - 1.f;
            p[3][2] = (epsilon - 1.f) * near;
        }
        else
        {
            p[2][2] = nearMinFarRecip * far;
            p[3][2] = nearMinFarRecip * nearTimesFar;
        }
    }
    else if (far != 0.f)
    {
        p[2][2] = nearMinFarRecip * near;
        p[3][2] = nearMinFarRecip * nearTimesFar;
    }
    else
    {
        AfxThrowError();
        //p[2][2];
        //p[3][2];
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
    ip[3][3] =-(p[2][2] / (p[3][2] * p[2][3]));
}
#if 0
_AVXINL void AfxComputeCameraProjectiveMatrices(afxCamera cam, afxM4d p, afxM4d ip)
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
#endif

_AVXINL void AfxComputeCameraMatrices(afxCamera cam, afxM4d v, afxM4d iv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(v);
    AfxAssert(iv);
    afxM3d a, b, c;

    if (!cam->useQuatOrient)
        AfxM3dCopy(a, cam->orientM3d);
    else
        AfxM3dRotationFromQuat(a, cam->orient);

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
    AfxM3dMultiply(c, a, b);
    b[0][0] = 1.f;
    b[0][1] = 0.f;
    b[0][2] = 0.f;
    b[1][0] = 0.f;
    b[1][1] = cosv[0];
    b[1][2] = -sinv[0];
    b[2][0] = 0.f;
    b[2][1] = sinv[0];
    b[2][2] = cosv[0];
    AfxM3dMultiply(a, c, b);
    b[0][0] = cosv[2];
    b[0][1] = -sinv[2];
    b[0][2] = 0.f;
    b[1][0] = sinv[2];
    b[1][1] = cosv[2];
    b[1][2] = 0.f;
    b[2][0] = 0.f;
    b[2][1] = 0.f;
    b[2][2] = 1.f;
    AfxM3dMultiply(c, a, b);

    afxV4d at;
    AfxV3dPreMultiplyM3d(at, cam->pos, c);
    at[0] = -(at[0] + cam->displacement[0]);
    at[1] = -(at[1] + cam->displacement[1]);
    at[2] = -(at[2] + cam->displacement[2]);
    at[3] = 1.f;
    AfxM4dCopyM3d(v, c, at);
    AfxM4dTransposeAtm(iv, v);
    AfxM4dEnsureAffine(iv);
    AfxV3dPostMultiplyM3d(at, c, cam->displacement);
    AfxV3dAdd(iv[3], at, cam->pos);
    iv[3][3] = 1.f;
}

_AVXINL void AfxRecomputeCameraMatrices(afxCamera cam)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxComputeCameraMatrices(cam, cam->v, cam->iv);
    AfxComputeCameraProjectiveMatrices(cam, cam->p, cam->ip);
    AfxRecomputeFrustum(&cam->frustum, cam->v, cam->p);
}

_AVXINL void AfxFindWorldCoordinates(afxCamera cam, afxV2d const wh, afxV3d const screenPoint, afxV4d worldPoint)
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
    AfxV4dPreMultiplyM4d(v2, v, cam->ip);
    AfxV4dPreMultiplyM4d(v, v2, cam->iv);
    AfxV3dScale(worldPoint, v, 1.f / v[3]);
    worldPoint[3] = 1.f;
}

_AVXINL void AfxFindScreenCoordinates(afxCamera cam, afxV2d const wh, afxV4d const worldPoint, afxV3d screenPoint)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(wh);
    AfxAssert(worldPoint);
    AfxAssert(worldPoint[3] == 1.f);
    AfxAssert(screenPoint);

    afxV4d v, v2;
    AfxV4dPreMultiplyM4d(v, worldPoint, cam->v);
    AfxV4dPreMultiplyM4d(v2, v, cam->p);
    AfxV3dScale(v, v2, 1.f / v2[3]);

    if (cam->depthRange == avxClipBoundary_NEG_ONE_TO_ONE)
        v[2] = v[2] + v[2] - 1.f;

    AfxV3dSet(screenPoint, (v[0] + 1.f) * wh[0] * 0.5f, (v[1] + 1.f) * wh[1] * 0.5f, v[2]);
}

_AVXINL void AfxGetCameraPickingRay(afxCamera cam, afxV2d const wh, afxV2d const cursor, afxV4d origin, afxV3d normal)
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
    AfxV4dPreMultiplyM4d(v2, v, cam->ip);
    v2[3] = 0.0;
    AfxV4dPreMultiplyM4d(v, v2, cam->iv);
    
    // should normalize or zero
    afxReal len = AfxV3dMag(v);

    if (len <= 0.0000099999997f)
        AfxV3dZero(normal);
    else
        AfxV3dScale(normal, v, 1.f / len);
}

_AVXINL void AfxComputeCameraRelativePlanarBases(afxCamera cam, afxBool screenOrthogonal, afxV3d const planeNormal, afxV4d const pointOnPlane, afxV3d xBasis, afxV3d yBasis)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(planeNormal);
    AfxAssert(pointOnPlane);
    AfxAssert(xBasis);
    AfxAssert(yBasis);

    AfxGetCameraDirectionX(cam, NIL, xBasis);
    afxV3d v;

    if (!screenOrthogonal) AfxV3dCopy(v, xBasis);
    else
    {
        afxV3d up;
        afxV4d origin, camToPoint;
        AfxGetCameraPosition(cam, origin);
        AfxGetCameraDirectionY(cam, NIL, up);
        AfxV4dSub(camToPoint, pointOnPlane, origin);
        v[0] = up[2] * camToPoint[1] - up[1] * camToPoint[2];
        v[1] = up[0] * camToPoint[2] - up[2] * camToPoint[0];
        v[2] = up[1] * camToPoint[0] - up[0] * camToPoint[1];
    }

    yBasis[0] = (v[2] * planeNormal[1]) - (v[1] * planeNormal[2]);
    yBasis[1] = (v[0] * planeNormal[2]) - (v[2] * planeNormal[0]);
    yBasis[2] = (v[1] * planeNormal[0]) - (v[0] * planeNormal[1]);

    // should normalize or zero
    afxReal len = AfxV3dMag(yBasis);
    
    if (len <= 0.0000099999997f)
        AfxV3dZero(yBasis);
    else
        AfxV3dScale(yBasis, yBasis, AfxRecip(len));
}

_AVXINL void AfxGetCameraFrustum(afxCamera cam, afxFrustum* frustum)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssert(frustum);
    AfxFrustumCopy(frustum, &cam->frustum);
}

#if 0
_AVX afxBool AfxProcessCameraInteraction(afxCamera cam, afxNat port, afxReal64 speed, afxReal64 dt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    afxBool updated = FALSE;

    if (AfxIsLmbPressed(port))
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

    if (AfxIsRmbPressed(port))
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

    if (AfxV3dSum(v))
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

        if (AfxIsLmbPressed(0))
        {
            afxV2d delta;
            afxV3d deltaEar;
            AfxGetMouseMotion(0, delta);
            deltaEar[1] = -((afxReal)(delta[0] * AFX_PI / 180.f));
            deltaEar[0] = -((afxReal)(delta[1] * AFX_PI / 180.f));
            deltaEar[2] = 0.f;

            AfxApplyCameraElevAzimRoll(cam, deltaEar);
        }



        if (AfxIsRmbPressed(0))
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

_AVX afxError _AvxCamCtorCb(afxCamera cam, void** args, afxNat invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);

    afxDrawInput din = args[0];
    AfxAssertObjects(1, &din, afxFcc_DIN);

    cam->perspective = TRUE;

    afxM4d m, m2;
    AfxM4dReset(m);
    AfxM4dReset(m2);
    AfxRecomputeFrustum(&cam->frustum, m, m2);
    
    AfxResetCamera(cam);

    avxClipSpace clip;
    AfxDescribeClipSpace(din, &clip);
    AfxSetCameraDepthRange(cam, clip.boundary);

    return err;
}

_AVX afxError _AvxCamDtorCb(afxCamera cam)
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
    .fixedSiz = sizeof(AFX_OBJECT(afxCamera)),
    .ctor = (void*)_AvxCamCtorCb,
    .dtor = (void*)_AvxCamDtorCb,
#if 0
    .eventFilter = (void*)_AvxCamEventFilter
#endif
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxAcquireCameras(afxDrawInput din, afxNat cnt, afxCamera cameras[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);

    afxClass* cls = (afxClass*)AfxGetCameraClass(din);
    AfxAssertClass(cls, afxFcc_CAM);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)cameras, (void const*[]) { din }))
        AfxThrowError();

    AfxAssertObjects(cnt, cameras, afxFcc_CAM);

    return err;
}
