/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *        Q W A D R O   V I D E O   G R A P H I C S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AVX_DRAW_C
#define _AVX_CAMERA_C
#include "../ddi/avxImplementation.h"

_AVX afxReal64 AvxFindPhysicalAspectRatio(afxUnit screenWidth, afxUnit screenHeight)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(screenWidth);
    AFX_ASSERT(screenHeight);
    afxReal64 ratio, div = (afxReal64)screenWidth / (afxReal64)screenHeight;

    if (div <= 1.4)
        ratio = 1.33;
    else if (div <= 1.6)
        ratio = 1.5599999;
    else
        ratio = 1.78;

    return ratio;
}

_AVX afxReal64 AvxFindAllowedCameraLodError(afxReal64 errInPixels, afxInt vpHeightInPixels, afxReal64 fovY, afxReal64 distanceFromCam)
{
    return AfxSin(fovY * 0.5) / AfxCos(fovY * 0.5) * errInPixels * distanceFromCam / ((afxReal64)vpHeightInPixels * 0.5);
}

////////////////////////////////////////////////////////////////////////////////

_AVXINL void AvxSetCameraAspectRatios(avxCamera cam, afxReal physAspectRatio, afxV2d const screenExtent, afxV2d const windowExtent)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    AFX_ASSERT(screenExtent);
    AFX_ASSERT(windowExtent);
    cam->wpOverHp = physAspectRatio;
    cam->wrOverHr = screenExtent[0] / screenExtent[1];
    cam->wwOverHw = windowExtent[0] / windowExtent[1];
    cam->shouldSyncP = TRUE;
}

_AVXINL void AvxGetCameraMatrices(avxCamera cam, afxM4d p, afxM4d pv, afxM4d v, afxM4d ip, afxM4d ipv, afxM4d iv)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    AFX_ASSERT(p || ip || v || iv || pv || ipv);
    AvxRecomputeCameraMatrices(cam);
    if (p) AfxM4dCopy(p, cam->p);
    if (ip) AfxM4dCopy(ip, cam->ip);
    if (v) AfxM4dCopy(v, cam->v);
    if (iv) AfxM4dCopy(iv, cam->iv);
    if (pv) AfxM4dCopy(pv, cam->pv);
    if (ipv) AfxM4dCopy(ipv, cam->ipv);
}

_AVXINL afxReal AvxGetCameraFov(avxCamera cam)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    return cam->fovY;
}

_AVXINL afxReal AvxAdjustCameraFov(avxCamera cam, afxReal fov)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    afxReal lastVal = cam->fovY;
    // The value 1.0471976f is approximately equal to Pi / 3, or 60 degrees in radians.
    cam->fovY = fov ? fov : 1.0471976f;
    cam->shouldSyncP = TRUE;
    return lastVal;
}

_AVXINL void AvxSetCameraClipPlanes(avxCamera cam, afxReal near, afxReal far)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    cam->nearClipPlane = near;
    cam->farClipPlane = far;
    cam->shouldSyncP = TRUE;
}

_AVXINL void AvxGetCameraFrustum(avxCamera cam, afxFrustum* frustum, afxReal* nearClipPlane, afxReal* farClipPlane)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    AFX_ASSERT(frustum || nearClipPlane || farClipPlane);
    if (nearClipPlane) *nearClipPlane = cam->nearClipPlane;
    if (farClipPlane) *farClipPlane = cam->farClipPlane;
    if (frustum) AfxCopyFrustum(frustum, &cam->frustum);
}

_AVXINL avxClipSpaceDepth AvxGetCameraDepthRange(avxCamera cam, afxReal* epsilon)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    if (epsilon) *epsilon = cam->depthRangeEpsilon;
    return cam->depthRange;
}

_AVXINL void AvxModifyCameraDepthRange(avxCamera cam, avxClipSpaceDepth range, afxReal epsilon)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    cam->depthRangeEpsilon = epsilon ? epsilon : AFX_EPSILON;
    cam->depthRange = range;
    cam->shouldSyncP = TRUE;
}

_AVXINL void AvxComputeCameraProjectionMatrices(avxCamera cam, afxM4d p, afxM4d ip)
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    
    // uses trigonometric functions (AfxSinf and AfxCosf) to get the tangent of half of the field of view angle.
    afxReal half = AfxSinf(cam->fovY * 0.5f) / AfxCosf(cam->fovY * 0.5f);
    // The initial projection matrix is set up based on the camera’s field of view and aspect ratio. 
    // This is common for perspective projections.
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

    // far and near represent the distance to the far and near clip planes, respectively. 
    // epsilon is used to prevent division by zero or other small-precision issues.
    afxReal far = cam->farClipPlane;
    afxReal near = cam->nearClipPlane;
    afxReal epsilon = cam->depthRangeEpsilon;
    // nearMinFarRecip is the reciprocal of the difference between the near and far planes, used to normalize the depth values. 
    // nearTimesFar helps in scaling the depth projection.
    afxReal nearMinFarRecip = 1.f / (near - far);
    afxReal nearTimesFar = near * far;
    
    /*
        When the far plane (far) is set to zero, it's commonly associated with a situation where there's no real "far" depth, 
        and we want to project to an infinitely distant far plane. In this context, we typically want to deal with projection 
        matrices that can handle an infinite far plane or a depth that extends indefinitely.

        When the far plane is zero, we want to ensure that the depth is handled in a way that doesn't result in division by zero errors. 
        One way to do this is to use an epsilon value (a very small number close to zero, but not exactly zero) to maintain stability in the projection. 
        This ensures the matrix still behaves reasonably when multiplied by vertices.
    */

    switch (cam->depthRange)
    {
    case avxClipSpaceDepth_NEG_ONE_TO_ONE:
    {
        // The depth mapping follows the traditional OpenGL method (near to -1 and far to 1). 
        // If far == 0.f, the projection matrix entries are adjusted using the epsilon value to avoid invalid behavior or division by zero.

        if (far == 0.f)
        {
            // This ensures the depth mapping does not result in NaN or undefined values when there is no far plane.
            p[2][2] = epsilon - 1.f;
            p[3][2] = (epsilon - 2.f) * near;
        }
        else
        {
            p[2][2] = (near + far) * nearMinFarRecip;
            p[3][2] = nearMinFarRecip * nearTimesFar + nearMinFarRecip * nearTimesFar;
        }
        break;
    }
    case avxClipSpaceDepth_ZERO_TO_ONE:
    {
        // The depth mapping follows the traditional Direct3D method (and default of Vulkan), the depth range is from 0 to 1.
        // The same handling for far == 0.f applies here with epsilon, ensuring the matrix remains valid when the far plane is not defined.
        // Otherwise, the depth range is calculated using the standard formula for the ZERO_TO_ONE space, 
        // mapping the near and far planes to the appropriate depth buffer range.

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
        break;
    }
    case avxClipSpaceDepth_NEG_ONE_TO_ZERO:
    {
        // This experimental depth mapping of Qwadro inverts the typical depth buffer behavior (where closer objects have larger depth values). 
        // Here, near = 0 and far = -1 are set in a reversed manner, making this useful for some custom depth buffer optimizations.
        // The epsilon correction is applied when far == 0.f to ensure the projection matrix remains valid and avoids division by zero.
        // This is especially important for reverse depth mapping, as it helps maintain a valid matrix while ensuring that the depth precision is enhanced when used in custom shaders or for reverse depth buffering.

        // The code comments explain the potential use cases for NEG_ONE_TO_ZERO, such as reverse depth buffering and custom projection matrices. 
        // The expected behavior in rendering techniques like shadow mapping or custom depth-based post-processing is acknowledged.
        // Similar to other cases, when far == 0.f, we use an epsilon value to ensure that the matrix remains valid.

        //

        // Expected behavior for NEG_ONE_TO_ZERO:
        // The near plane should be at 0, and the far plane should be at -1 (as opposed to traditional approaches where the far plane is positive).
        // However, it seems that this depth range might conflict with typical depth-buffering mechanisms, making it hard to find direct support in common hardware or shaders.

        // Although uncommon, there are some potential use cases for this depth range, for example:
        // Reverse Depth Buffering:
        // In some custom rendering techniques, using reverse depth buffers (where closer objects have larger depth values) can improve depth precision.
        // However, the range being from 0 to - 1 complicates things, especially when working with standard depth ranges.
        // Custom Projection Matrices:
        // Some applications or engines might use this range in specialized techniques like shadow mapping or depth-based post-processing effects where you want to invert the "depth" ordering in your projection space.

        // Ensure the range doesn't cause problems in shaders.
        // Since the NEG_ONE_TO_ZERO depth range would invert the expected depth behavior (where closer objects have a smaller depth value), 
        // it can lead to difficulties in standard shaders, depth tests, and other components that expect the depth to increase as objects move further from the camera.
        // You might want to:
        // Test depth-based operations carefully to ensure that they behave as expected.
        // Modify shaders or depth-test configurations to account for this reversed depth space.
        // Consider debugging tools to visualize the depth values in this custom space to ensure they behave as expected.

        if (far != 0.f)
        {
            p[2][2] = nearMinFarRecip * near;
            p[3][2] = nearMinFarRecip * nearTimesFar;
        }
        else
        {
            // To be tested.
            // Apply some small epsilon correction
            // Using the epsilon correction when far == 0.f to avoid division by zero and keep the projection matrix valid. 
            // This keeps the behavior consistent with the reversed Z approach.
            p[2][2] = epsilon - 1.f;
            p[3][2] = (epsilon - 1.f) * near;
        }
        break;
    }
    default: AfxThrowError(); break;
    }

    // This section calculates the inverse of the projection matrix p (stored in ip). 
    // This is common in graphics to transform coordinates back from clip space to view space. 
    // The inversion is done by manually calculating the appropriate inverse for the 4x4 matrix, 
    // which is necessary for certain calculations (like unprojection).
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
    cam->shouldSyncP = FALSE;
}

_AVXINL afxError AvxConsultCameraDisplacement(avxCamera cam, afxV3d const displace, avxBlendOp op, afxV3d displacement)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    AFX_ASSERT(displacement);

    // displace[2] is distance from target.

    if (!displace) AfxV3dCopy(displacement, cam->displacement);
    else switch (op)
    {
    case avxBlendOp_ADD:
        AfxV3dAdd(displacement, cam->displacement, displace); break;
    case avxBlendOp_SUB:
        AfxV3dSub(displacement, cam->displacement, displace); break;
    case avxBlendOp_REV_SUB:
        AfxV3dResub(displacement, cam->displacement, cam->displacement, displace); break;
    case avxBlendOp_MIN:
        AfxV3dMin(displacement, cam->displacement, displace); break;
    case avxBlendOp_MAX:
        AfxV3dMax(displacement, cam->displacement, displace); break;
    default: AfxThrowError();
        AfxV3dCopy(displacement, cam->displacement); break;
    }
    return err;
}

_AVXINL void AvxResetCameraDisplacement(avxCamera cam, afxV3d const displacement)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    
    if (!displacement)
        AfxV3dCopy(cam->displacement, AFX_V4D_IDENTITY);
    else
        AfxV3dCopy(cam->displacement, displacement);

    cam->shouldSyncV = TRUE;
}

_AVXINL afxReal AvxGetCameraDistance(avxCamera cam)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    return cam->displacement[2];
}

_AVXINL void AvxSetCameraDistance(avxCamera cam, afxReal distance)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    cam->displacement[2] = distance;
    cam->shouldSyncV = TRUE;
}

_AVXINL void AvxApplyCameraDistance(avxCamera cam, afxReal distance)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    cam->displacement[2] += distance;
    cam->shouldSyncV = TRUE;
}

_AVXINL afxError AvxConsultCameraOrientation(avxCamera cam, afxV3d const rotation, avxBlendOp op, afxV3d elevAzimRoll)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    AFX_ASSERT(elevAzimRoll);
    
    if (!rotation) AfxV3dCopy(elevAzimRoll, cam->elevAzimRoll);
    else switch (op)
    {
    case avxBlendOp_ADD:
        AfxV3dAdd(elevAzimRoll, cam->elevAzimRoll, rotation); break;
    case avxBlendOp_SUB:
        AfxV3dSub(elevAzimRoll, cam->elevAzimRoll, rotation); break;
    case avxBlendOp_REV_SUB:
        AfxV3dResub(elevAzimRoll, cam->elevAzimRoll, cam->elevAzimRoll, rotation); break;
    case avxBlendOp_MIN:
        AfxV3dMin(elevAzimRoll, cam->elevAzimRoll, rotation); break;
    case avxBlendOp_MAX:
        AfxV3dMax(elevAzimRoll, cam->elevAzimRoll, rotation); break;
    default: AfxThrowError();
        AfxV3dCopy(elevAzimRoll, cam->elevAzimRoll); break;
    }
}

_AVXINL void AvxResetCameraOrientation(avxCamera cam, afxV3d const elevAzimRoll)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);

    if (!elevAzimRoll)
        AfxV3dCopy(cam->elevAzimRoll, AFX_V4D_IDENTITY);
    else
        AfxV3dCopy(cam->elevAzimRoll, elevAzimRoll);

    cam->shouldSyncV = TRUE;
}

_AVXINL void AvxGetCameraDirectionX(avxCamera cam, afxV3d left, afxV3d right)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    AFX_ASSERT2(left || right, left != right);
    AvxRecomputeCameraMatrices(cam);
    if (left) AfxV3dNeg(left, cam->iv[0]);
    if (right) AfxV3dCopy(right, cam->iv[0]);
}

_AVXINL void AvxGetCameraDirectionY(avxCamera cam, afxV3d down, afxV3d up)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    AFX_ASSERT2(down || up, down != up);
    AvxRecomputeCameraMatrices(cam);
    if (down) AfxV3dNeg(down, cam->iv[1]);
    if (up) AfxV3dCopy(up, cam->iv[1]);
}

_AVXINL void AvxGetCameraDirectionZ(avxCamera cam, afxV3d near, afxV3d far)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    AFX_ASSERT2(far || near, far != near);
    AvxRecomputeCameraMatrices(cam);
    if (near) AfxV3dCopy(near, cam->iv[2]);
    if (far) AfxV3dNeg(far, cam->iv[2]);
}

_AVXINL void AvxGetCameraPosition(avxCamera cam, afxV3d point)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    AFX_ASSERT(point);
    AvxRecomputeCameraMatrices(cam);
    AfxV3dCopy(point, cam->iv[3]);
}

_AVXINL void AvxResetCameraPosition(avxCamera cam, afxV3d const point)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    
    if (!point)
        AfxV3dZero(cam->pos);
    else
        AfxV3dCopy(cam->pos, point);

    cam->shouldSyncV = TRUE;
}

_AVXINL void AvxTranslateCamera(avxCamera cam, afxV3d const motion)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    AFX_ASSERT(motion);

    AvxRecomputeCameraMatrices(cam);

    cam->pos[0] = motion[0] * cam->iv[0][0] + cam->pos[0];
    cam->pos[1] = motion[0] * cam->iv[0][1] + cam->pos[1];
    cam->pos[2] = motion[0] * cam->iv[0][2] + cam->pos[2];

    cam->pos[0] = motion[1] * cam->iv[1][0] + cam->pos[0];
    cam->pos[1] = motion[1] * cam->iv[1][1] + cam->pos[1];
    cam->pos[2] = motion[1] * cam->iv[1][2] + cam->pos[2];

    cam->pos[0] = motion[2] * cam->iv[2][0] + cam->pos[0];
    cam->pos[1] = motion[2] * cam->iv[2][1] + cam->pos[1];
    cam->pos[2] = motion[2] * cam->iv[2][2] + cam->pos[2];
    cam->shouldSyncV = TRUE;
}

_AVXINL void AvxComputeCameraMatrices(avxCamera cam, afxM4d v, afxM4d iv)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    AFX_ASSERT(v);
    AFX_ASSERT(iv);

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
    cam->shouldSyncV = FALSE;
}

_AVXINL void AvxRecomputeCameraMatrices(avxCamera cam)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    afxBool syncV = cam->shouldSyncV;
    afxBool syncP = cam->shouldSyncP;

    if (syncV)
        AvxComputeCameraMatrices(cam, cam->v, cam->iv);

    if (syncP)
        AvxComputeCameraProjectionMatrices(cam, cam->p, cam->ip);

    if (syncP || syncV)
    {
        AfxM4dMultiply(cam->pv, cam->p, cam->v);
        AfxM4dMultiply(cam->ipv, cam->ip, cam->iv);
        AfxMakeFrustum(&cam->frustum, cam->pv, cam->ipv);
    }
}

_AVXINL void AvxFindWorldCoordinates(avxCamera cam, afxV2d const wh, afxV3d const screenPoint, afxV4d worldPoint)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    AFX_ASSERT(wh);
    AFX_ASSERT(screenPoint);
    AFX_ASSERT(worldPoint);

    afxV4d v =
    {
        ((screenPoint[0] + screenPoint[0]) - wh[0]) / wh[0],
        ( screenPoint[1] + screenPoint[1]  - wh[1]) / wh[1],
        ((screenPoint[2] + screenPoint[2] - 1.f - -1.f) * 0.5f),
        1.f
    };

    AvxRecomputeCameraMatrices(cam);

    afxV4d v2;
    AfxV4dPreMultiplyM4d(v2, v, cam->ip);
    AfxV4dPreMultiplyM4d(v, v2, cam->iv);
    AfxV3dScale(worldPoint, v, 1.f / v[3]);
    worldPoint[3] = 1.f;
}

_AVXINL void AvxFindScreenCoordinates(avxCamera cam, afxV2d const wh, afxV4d const worldPoint, afxV3d screenPoint)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    AFX_ASSERT(wh);
    AFX_ASSERT(worldPoint);
    AFX_ASSERT(worldPoint[3] == 1.f);
    AFX_ASSERT(screenPoint);

    AvxRecomputeCameraMatrices(cam);

    afxV4d v, v2;
    AfxV4dPreMultiplyM4d(v, worldPoint, cam->v);
    AfxV4dPreMultiplyM4d(v2, v, cam->p);
    AfxV3dScale(v, v2, 1.f / v2[3]);

    if (cam->depthRange == avxClipSpaceDepth_NEG_ONE_TO_ONE)
        v[2] = v[2] + v[2] - 1.f;

    AfxV3dSet(screenPoint, (v[0] + 1.f) * wh[0] * 0.5f, (v[1] + 1.f) * wh[1] * 0.5f, v[2]);
}

_AVXINL void AvxGetCameraPickingRay(avxCamera cam, afxV2d const wh, afxV2d const cursor, afxV4d origin, afxV3d normal)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    AFX_ASSERT(wh);
    AFX_ASSERT(cursor);
    AFX_ASSERT(origin);
    AFX_ASSERT(normal);

    AvxRecomputeCameraMatrices(cam);

    AvxGetCameraPosition(cam, origin);
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

_AVXINL void AvxComputeCameraRelativePlanarBases(avxCamera cam, afxBool screenOrthogonal, afxV3d const planeNormal, afxV4d const pointOnPlane, afxV3d xBasis, afxV3d yBasis)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    AFX_ASSERT(planeNormal);
    AFX_ASSERT(pointOnPlane);
    AFX_ASSERT(xBasis);
    AFX_ASSERT(yBasis);

    AvxGetCameraDirectionX(cam, NIL, xBasis);
    afxV3d v;

    if (!screenOrthogonal) AfxV3dCopy(v, xBasis);
    else
    {
        afxV3d up;
        afxV4d origin, camToPoint;
        AvxGetCameraPosition(cam, origin);
        AvxGetCameraDirectionY(cam, NIL, up);
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

#if 0
_AVX afxBool AvxProcessCameraInteraction(avxCamera cam, afxUnit port, afxReal64 speed, afxReal64 dt)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
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
    AvxApplyCameraDistance(cam, w);

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
        AvxTranslateCamera(cam, v);
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
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    (void)watched;
    (void)ev;

    switch (ev->id)
    {
    case AFX_EVENT_MSE_AXIS_UPDATED:
    {
        //afxMouse mse = (void*)watched;
        //AFX_ASSERT_OBJECTS(afxFcc_MSE, 1, &mse);

        // TODO Leva isso para o usuário

        if (AfxIsLmbPressed(0))
        {
            afxV2d delta;
            afxV3d deltaEar;
            AfxGetMouseMotion(0, delta);
            deltaEar[1] = -AfxRadf(delta[0]);
            deltaEar[0] = -AfxRadf(delta[1]);
            deltaEar[2] = 0.f;
            afxV3d v;
            AvxConsultCameraOrientation(cam, deltaEar, avxBlendOp_ADD, v);
            AvxResetCameraOrientation(cam, v);
        }

        if (AfxIsRmbPressed(0))
        {
            afxV2d delta;
            afxV3d off;
            AfxGetMouseMotion(0, delta);
            off[0] = -AfxRadf(delta[0]);
            off[1] = -AfxRadf(delta[1]);
            off[2] = 0.f;
            afxV3d v;
            AvxConsultCameraDisplacement(cam, off, avxBlendOp_ADD, v);
            AvxResetCameraDisplacement(cam, v);
        }
        break;
    }
    case AFX_EVENT_MSE_WHEEL_UPDATED:
    {
        afxReal w = AfxGetMouseWheelDelta(0);
        w = w / 120.f; // WHEEL_DELTA
        AvxApplyCameraDistance(cam, w);
        break;
    }
    case AFX_EVENT_KBD_PRESSED:
    {
        //afxKeyboard kbd = (void*)watched;
        //AFX_ASSERT_OBJECTS(afxFcc_KBD, 1, &kbd);
        break;
    }
    default:break;
    }
    return FALSE;
}
#endif

_AVXINL void AvxResetCamera(avxCamera cam)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);

    cam->flags = NIL;

    cam->wpOverHp = 1.33f;
    cam->wrOverHr = 1.33f;
    cam->wwOverHw = 1.f;
    cam->fovY = 1.0471976f;

    cam->nearClipPlane = 0.0001f;// 0.4;
    cam->farClipPlane = 1000000.0;// 1000.f;
    cam->depthRange = avxClipSpaceDepth_NEG_ONE_TO_ONE;
    cam->depthRangeEpsilon = AFX_EPSILON;

    cam->useQuatOrient = TRUE;
    AfxV3dZero(cam->pos);
    AfxQuatReset(cam->orient);
    AfxM3dReset(cam->orientM3d);
    AfxV3dZero(cam->elevAzimRoll);
    AfxV3dZero(cam->displacement);
    cam->shouldSyncP = TRUE;
    cam->shouldSyncV = TRUE;
    AvxRecomputeCameraMatrices(cam);
}

_AVX afxError _AvxCamDtorCb(avxCamera cam)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    return err;
}

_AVX afxError _AvxCamCtorCb(avxCamera cam, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);

    afxDrawSystem dsys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    cam->perspective = TRUE;

    afxM4d m, m2;
    AfxM4dReset(m);
    AfxM4dReset(m2);
    AfxMakeFrustum(&cam->frustum, m, m2);
    
    AvxResetCamera(cam);

    // TODO: Get it from DSYS/DDEV
    afxBool nonRh = FALSE;
    AvxModifyCameraDepthRange(cam, avxClipSpaceDepth_NEG_ONE_TO_ONE, AFX_EPSILON);

    return err;
}

_AVX afxClassConfig const _AVX_CAM_CLASS_CONFIG =
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

_AVX afxError AvxAcquireCameras(afxDrawSystem dsys, afxUnit cnt, avxCamera cameras[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    afxClass* cls = (afxClass*)_AvxDsysGetImpl(dsys)->camCls(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_CAM);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)cameras, (void const*[]) { dsys }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_CAM, cnt, cameras);

    return err;
}
