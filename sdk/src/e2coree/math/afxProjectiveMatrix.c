/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#include "afx/math/afxMatrix.h"
#include "afx/math/afxProjectiveMatrix.h"
#include "afx/math/afxMathDefs.h"
#include "afx/math/afxReal.h"
#include "afx/math/afxVector.h"
#include "afx/math/afxPlane.h"

////////////////////////////////////////////////////////////////////////////////
// Projective transformation matrix operations                                //
////////////////////////////////////////////////////////////////////////////////

_AFXINL void AfxComputeShadowM4d(afxM4d m, afxPlane const* p, afxReal const lightPos[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);

    afxReal d = AfxFindPlaneDistance(p, lightPos);
#if !0
    m[0][0] = d - lightPos[0] * p->normal[0];
    m[1][0] = -lightPos[0] * p->normal[1];
    m[2][0] = -lightPos[0] * p->normal[2];
    m[3][0] = -lightPos[0] * p->offset;

    m[0][1] = -lightPos[1] * p->normal[0];
    m[1][1] = d - lightPos[1] * p->normal[1];
    m[2][1] = -lightPos[1] * p->normal[2];
    m[3][1] = -lightPos[1] * p->offset;

    m[0][2] = -lightPos[2] * p->normal[0];
    m[1][2] = -lightPos[2] * p->normal[1];
    m[2][2] = d - lightPos[2] * p->normal[2];
    m[3][2] = -lightPos[2] * p->offset;

    m[0][3] = -p->normal[0];
    m[1][3] = -p->normal[1];
    m[2][3] = -p->normal[2];
    m[3][3] = d - p->offset;
#else
    m[0][0] = d - lightPos[0] * p->normal[0];
    m[0][1] =   -lightPos[1] * p->normal[0];
    m[0][2] =   -lightPos[2] * p->normal[0];
    m[0][3] =   -p->normal[0];
    m[1][0] =   -lightPos[0] * p->normal[1];
    m[1][1] = d - lightPos[1] * p->normal[1];
    m[1][2] =   -lightPos[2] * p->normal[1];
    m[1][3] =   -p->normal[1];
    m[2][0] =   -lightPos[0] * p->normal[2];
    m[2][1] =   -lightPos[1] * p->normal[2];
    m[2][2] = d - lightPos[2] * p->normal[2];
    m[2][3] =   -p->normal[2];
    m[3][0] =   -lightPos[0] * p->offset;
    m[3][1] =   -lightPos[1] * p->offset;
    m[3][2] =   -lightPos[2] * p->offset;
    m[3][3] = d - p->offset;
#endif
}

_AFXINL void AfxCubemapM4d_Direct3D(afxM4d m, afxNat face)
{
    afxError err = NIL;
    AfxAssert(m);
    AfxZeroM4d(m);

    switch (face)
    {
    case 0: // +X
        m[0][2] = 1;
        m[1][1] = 1;
        m[2][0] =-1;
        m[3][3] = 1;
        break;
    case 1: // -X
        m[0][2] =-1;
        m[1][1] = 1;
        m[2][0] = 1;
        m[3][3] = 1;
        break;
    case 2: // +Y
        m[0][0] = 1;
        m[2][1] =-1;
        m[1][2] = 1;
        m[3][3] = 1;
        break;
    case 3: // -Y
        m[0][0] = 1;
        m[2][1] = 1;
        m[1][2] =-1;
        m[3][3] = 1;
        break;
    case 4: // +Z
        m[0][0] = 1;
        m[1][1] = 1;
        m[2][2] = 1;
        m[3][3] = 1;
        break;
    case 5: // -Z
        m[0][0] =-1;
        m[1][1] = 1;
        m[2][2] =-1;
        m[3][3] = 1;
        break;
    };
}

_AFXINL void AfxCubemapM4d_OpenGL(afxM4d m, afxNat face)
{
    afxError err = NIL;
    AfxAssert(m);
    AfxZeroM4d(m);

    switch (face)
    {
    case 0: // +X
        m[2][0] = 1;
        m[1][1] = 1;
        m[0][2] =-1;
        m[3][3] = 1;
        break;
    case 1: // -X
        m[2][0] =-1;
        m[1][1] = 1;
        m[0][2] = 1;
        m[3][3] = 1;
        break;
    case 2: // +Y
        m[0][0] =-1;
        m[2][1] =-1;
        m[1][2] =-1;
        m[3][3] = 1;
        break;
    case 3: // -Y
        m[0][0] =-1;
        m[2][1] = 1;
        m[1][2] = 1;
        m[3][3] = 1;
        break;
    case 4: // +Z
        m[0][0] =-1;
        m[1][1] = 1;
        m[2][2] =-1;;
        m[3][3] = 1;
        break;
    case 5: // -Z
        m[0][0] = 1;
        m[1][1] = 1;
        m[2][2] = 1;
        m[3][3] = 1;
        break;
    }
}

_AFXINL void AfxComputeLookToM4d_LH(afxReal m[4][4], afxReal const eye[3], afxReal const dir[3], afxReal const up[3])
{
    // Should be compatible with XMMATRIX XMMatrixLookToLH(FXMVECTOR EyePosition, FXMVECTOR EyeDirection, FXMVECTOR UpDirection)

    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(!AfxV3dIsZero(dir));
    AfxAssert(!AfxV3dIsInfinite(dir));
    AfxAssert(!AfxV3dIsZero(up));
    AfxAssert(!AfxV3dIsInfinite(up));

    afxV4d x, y, z;
    AfxGetNormalizedV3d(z, dir);
    z[3] = 0;
    AfxCrossV3d(x, up, z);
    x[3] = 0;
    AfxGetNormalizedV3d(x, x);
    AfxCrossV3d(y, z, x);
    y[3] = 0;

    afxV3d negEye;
    AfxNegateV3d(negEye, eye);

    afxV4d const w = { AfxDotV3d(x, negEye), AfxDotV3d(y, negEye), AfxDotV3d(z, negEye), (afxReal)1 };
    AfxSetM4dTransposed(m, x, y, z, w);    
}

_AFXINL void AfxComputeLookToM4d_RH(afxReal m[4][4], afxReal const eye[3], afxReal const dir[3], afxReal const up[3])
{
    // Should be compatible with XMMATRIX XMMatrixLookToRH(FXMVECTOR EyePosition, FXMVECTOR EyeDirection, FXMVECTOR UpDirection)

    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(!AfxV3dIsZero(dir));
    AfxAssert(!AfxV3dIsInfinite(dir));
    AfxAssert(!AfxV3dIsZero(up));
    AfxAssert(!AfxV3dIsInfinite(up));

    afxV3d negDir;
    AfxNegateV3d(negDir, dir);
    AfxComputeLookToM4d_LH(m, eye, negDir, up);
}

_AFXINL void AfxComputeLookAtM4d_LH(afxReal m[4][4], afxReal const eye[3], afxReal const target[3], afxReal const up[3])
{
    // Should be compatible with XMMATRIX XMMatrixLookAtLH(FXMVECTOR EyePosition, FXMVECTOR FocusPosition, FXMVECTOR UpDirection)

    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(eye);
    AfxAssert(target);
    AfxAssert(up);
#if 0
    afxV4d z, x, y;
    AfxSubV3d(z, target, eye);
    z[3] = 0;
    AfxGetNormalizedV3d(z, z);
    AfxCrossV3d(x, up, z);
    x[3] = 0;
    AfxGetNormalizedV3d(x, x);
    AfxCrossV3d(y, z, x);
    y[3] = 0;

    afxV4d const w = { -AfxDotV3d(x, eye), -AfxDotV3d(y, eye), -AfxDotV3d(z, eye), (afxReal)1 };
    AfxSetM4dTransposed(m, x, y, z, w);
#else
    afxV4d dir;
    AfxSubV3d(dir, target, eye);
    AfxComputeLookToM4d_LH(m, eye, dir, up);
#endif
}

_AFXINL void AfxComputeLookAtM4d_RH(afxReal m[4][4], afxReal const eye[3], afxReal const target[3], afxReal const up[3])
{
    // Should be compatible with XMMATRIX XMMatrixLookAtRH(FXMVECTOR EyePosition, FXMVECTOR FocusPosition, FXMVECTOR UpDirection)

    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(eye);
    AfxAssert(target);
    AfxAssert(up);

#if 0
    afxV4d z, x, y;
    AfxSubV3d(z, eye, target);
    z[3] = 0;
    AfxGetNormalizedV3d(z, z);
    AfxCrossV3d(x, up, z);
    x[3] = 0;
    AfxGetNormalizedV3d(x, x);
    AfxCrossV3d(y, z, x);
    y[3] = 0;
    
    afxV4d const w = { -AfxDotV3d(x, eye), -AfxDotV3d(y, eye), -AfxDotV3d(z, eye), (afxReal)1 };
    AfxSetM4dTransposed(m, x, y, z, w);
#else
    afxV4d negDir;
    AfxSubV3d(negDir, eye, target);
    AfxComputeLookToM4d_LH(m, eye, negDir, up);
#endif
}

_AFXINL void AfxComputeOffcenterOrthographicM4d_LH(afxReal m[4][4], afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far)
{
    // Should be compatible with XMMATRIX XM_CALLCONV XMMatrixOrthographicOffCenterLH(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop, float NearZ, float FarZ)

    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(!AfxRealIsEqual(right, left, 0.00001f));
    AfxAssert(!AfxRealIsEqual(top, bottom, 0.00001f));
    AfxAssert(!AfxRealIsEqual(far, near, 0.00001f));

    AfxResetM4d(m);

    afxReal rw = 1.0f / (right - left);
    afxReal rh = 1.0f / (top - bottom);
    afxReal range = 1.0f / (far - near);

    m[0][0] = rw + rw;
    m[1][1] = rh + rh;
    m[2][2] = range;
    m[3][0] = -(left + right) * rw;
    m[3][1] = -(top + bottom) * rh;
    m[3][2] = -range * near;
}

_AFXINL void AfxComputeOffcenterOrthographicM4d_RH(afxReal m[4][4], afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far)
{
    // Should be compatible with XMMATRIX XM_CALLCONV XMMatrixOrthographicOffCenterLH(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop, float NearZ, float FarZ)

    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(!AfxRealIsEqual(right, left, 0.00001f));
    AfxAssert(!AfxRealIsEqual(top, bottom, 0.00001f));
    AfxAssert(!AfxRealIsEqual(far, near, 0.00001f));
    
    AfxResetM4d(m);

    afxReal rw = 1.0f / (right - left);
    afxReal rh = 1.0f / (top - bottom);
    afxReal range = 1.0f / (far - near);

    m[0][0] = rw + rw;
    m[1][1] = rh + rh;
    m[2][2] = range;
    m[3][0] = -(left + right) * rw;
    m[3][1] = -(top + bottom) * rh;
    m[3][2] = range * near;
}

_AFXINL void AfxComputeOrthographicM4d_LH(afxReal m[4][4], afxReal const extent[2], afxReal near, afxReal far)
{
    // Should be compatible with XMMATRIX XMMatrixOrthographicLH(float ViewWidth, float ViewHeight, float NearZ, float FarZ)

    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(extent);
    AfxAssert(!AfxRealIsEqual(extent[0], 0.0f, 0.00001f));
    AfxAssert(!AfxRealIsEqual(extent[1], 0.0f, 0.00001f));
    AfxAssert(!AfxRealIsEqual(far, near, 0.00001f));

    AfxResetM4d(m);

    afxReal range = 1.f / (far - near);

    m[0][0] = AfxScalar(2) / extent[0];
    m[1][1] = AfxScalar(2) / extent[1];
    m[2][2] = range;
    m[3][2] = -range * near;
}

_AFXINL void AfxComputeOrthographicM4d_RH(afxReal m[4][4], afxReal const extent[2], afxReal near, afxReal far)
{
    // Should be compatible with XMMATRIX XMMatrixOrthographicRH(float ViewWidth, float ViewHeight, float NearZ, float FarZ)

    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(extent);
    AfxAssert(!AfxRealIsEqual(extent[0], 0.0f, 0.00001f));
    AfxAssert(!AfxRealIsEqual(extent[1], 0.0f, 0.00001f));
    AfxAssert(!AfxRealIsEqual(far, near, 0.00001f));

    AfxResetM4d(m);

    afxReal range = 1.f / (near - far);

    m[0][0] = AfxScalar(2) / extent[0];
    m[1][1] = AfxScalar(2) / extent[1];
    m[2][2] = range;
    m[3][2] = range * near;
}

_AFXINL void AfxComputePerspectiveM4d_LH(afxReal m[4][4], afxReal const extent[2], afxReal near, afxReal far)
{
    // Should be compatible with XMMATRIX XMMatrixPerspectiveLH(float ViewWidth, float ViewHeight, float NearZ, float FarZ)

    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(near > 0.f && far > 0.f);
    AfxAssert(extent);
    AfxAssert(!AfxRealIsEqual(extent[0], 0.0f, 0.00001f));
    AfxAssert(!AfxRealIsEqual(extent[1], 0.0f, 0.00001f));
    AfxAssert(!AfxRealIsEqual(far, near, 0.00001f));

    AfxZeroM4d(m);

    afxReal near2 = near + near;
    afxReal range = far / (far - near);

    m[0][0] = near2 / extent[0];
    m[1][1] = near2 / extent[1];
    m[2][2] = range;
    m[2][3] = 1.f;
    m[3][2] = -range * near;
}

_AFXINL void AfxComputePerspectiveM4d_RH(afxReal m[4][4], afxReal const extent[2], afxReal near, afxReal far)
{
    // Should be compatible with XMMATRIX XMMatrixPerspectiveRH(float ViewWidth, float ViewHeight, float NearZ, float FarZ)

    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(near > 0.f && far > 0.f);
    AfxAssert(extent);
    AfxAssert(!AfxRealIsEqual(extent[0], 0.0f, 0.00001f));
    AfxAssert(!AfxRealIsEqual(extent[1], 0.0f, 0.00001f));
    AfxAssert(!AfxRealIsEqual(far, near, 0.00001f));

    AfxZeroM4d(m);

    afxReal near2 = near + near;
    afxReal range = far / (near - far);

    m[0][0] = near2 / extent[0];
    m[1][1] = near2 / extent[1];
    m[2][2] = range;
    m[2][3] = -1.f;
    m[3][2] = range * near;
}

_AFXINL void AfxComputeFovPerspectiveM4dLH(afxReal m[4][4], afxReal fovY, afxReal aspectRatio, afxReal near, afxReal far)
{
    // Should be compatible with XMMATRIX XMMatrixPerspectiveFovLH(float FovAngleY, float AspectRatio, float NearZ, float FarZ)

    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(near > 0.f && far > 0.f);
    AfxAssert(!AfxRealIsEqual(fovY, 0.0f, 0.00001f * 2.0f));
    AfxAssert(!AfxRealIsEqual(aspectRatio, 0.0f, 0.00001f));
    AfxAssert(!AfxRealIsEqual(far, near, 0.00001f));

    AfxZeroM4d(m);

    afxReal h = AfxCos(0.5f * fovY) / AfxSin(0.5f * fovY);
    afxReal w = h / aspectRatio;
    afxReal range = far / (near - far);

    m[0][0] = w;
    m[1][1] = h;
    m[2][2] = range;
    m[2][3] = 1.f;
    m[3][2] = -range * near;
}

_AFXINL void AfxComputeFovPerspectiveM4dRH(afxReal m[4][4], afxReal fovY, afxReal aspectRatio, afxReal near, afxReal far)
{
    // Should be compatible with XMMATRIX XMMatrixPerspectiveFovRH(float FovAngleY, float AspectRatio, float NearZ, float FarZ)

    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(near > 0.f && far > 0.f);
    AfxAssert(!AfxRealIsEqual(fovY, 0.0f, 0.00001f * 2.0f));
    AfxAssert(!AfxRealIsEqual(aspectRatio, 0.0f, 0.00001f));
    AfxAssert(!AfxRealIsEqual(far, near, 0.00001f));

    AfxZeroM4d(m);

    afxReal h = AfxCos(0.5f * fovY) / AfxSin(0.5f * fovY);
    afxReal w = h / aspectRatio;
    afxReal range = far / (near - far);

    m[0][0] = w;
    m[1][1] = h;
    m[2][2] = range;
    m[2][3] = -1.f;
    m[3][2] = range * near;
}

_AFXINL void AfxComputeOffcenterPerspectiveM4d_LH(afxReal m[4][4], afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far)
{
    // Should be compatible with XMMATRIX XMMatrixPerspectiveOffCenterLH(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop, float NearZ, float FarZ)

    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(near > 0.f && far > 0.f);
    AfxAssert(!AfxRealIsEqual(right, left, 0.00001f));
    AfxAssert(!AfxRealIsEqual(top, bottom, 0.00001f));
    AfxAssert(!AfxRealIsEqual(far, near, 0.00001f));

    AfxZeroM4d(m);

    afxReal near2 = near + near;
    afxReal rw = 1.0f / (right - left);
    afxReal rh = 1.0f / (top - bottom);
    afxReal range = far / (far - near);

    m[0][0] = near2 * rw;
    m[1][1] = near2 * rh;
    m[2][0] = -(left + right) * rw;
    m[2][1] = -(top + bottom) * rh;
    m[2][2] = range;
    m[2][3] = 1.f;
    m[3][2] = -range * near;
}

_AFXINL void AfxComputeOffcenterPerspectiveM4d_RH(afxReal m[4][4], afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far)
{
    // Should be compatible with XMMATRIX XMMatrixPerspectiveOffCenterRH(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop, float NearZ, float FarZ)

    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(near > 0.f && far > 0.f);
    AfxAssert(!AfxRealIsEqual(right, left, 0.00001f));
    AfxAssert(!AfxRealIsEqual(top, bottom, 0.00001f));
    AfxAssert(!AfxRealIsEqual(far, near, 0.00001f));

    AfxZeroM4d(m);

    afxReal near2 = near + near;
    afxReal rw = 1.0f / (right - left);
    afxReal rh = 1.0f / (top - bottom);
    afxReal range = far / (near - far);

    m[0][0] = near2 * rw;
    m[1][1] = near2 * rh;
    m[2][0] = (left + right) * rw;
    m[2][1] = (top + bottom) * rh;
    m[2][2] = range;
    m[2][3] = -1.f;
    m[3][2] = range * near;
}

// MakeView
// Memory layout:

_AFXINL void AfxComputeRenderWareViewM4d(afxReal view[4][4], afxReal const cam[4][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(view);
    AfxAssert(cam);

    afxM4d inv;
    AfxInvertM4d(inv, cam);
    view[0][0] = -inv[0][0];
    view[0][1] = inv[0][1];
    view[0][2] = inv[0][2];
    view[0][3] = 0.0f;
    view[1][0] = -inv[1][0];
    view[1][1] = inv[1][1];
    view[1][2] = inv[1][2];
    view[1][3] = 0.0f;
    view[2][0] = -inv[2][0];
    view[2][1] = inv[2][1];
    view[2][2] = inv[2][2];
    view[2][3] = 0.0f;
    view[3][0] = -inv[3][0];
    view[3][1] = inv[3][1];
    view[3][2] = inv[3][2];
    view[3][3] = 1.0f;
}

// MakeProjection
// Memory layout:

_AFXINL void AfxComputeRenderWareProjectionM4d(afxReal p[4][4], afxViewport const* vp, afxBool perspective)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(p);
    AfxAssert(vp);
    AfxAssert(vp->depth[1]);
    AfxAssert(vp->depth[0] < vp->depth[1]);
    AfxAssert(vp->offset);
    AfxAssert(vp->extent);

    afxReal invwx = 1.0f / vp->extent[0];
    afxReal invwy = 1.0f / vp->extent[1];
    afxReal invz = 1.0f / (vp->depth[1] - vp->depth[0]);

    p[0][0] = invwx;
    p[0][1] = 0.0f;
    p[0][2] = 0.0f;
    p[0][3] = 0.0f;

    p[1][0] = 0.0f;
    p[1][1] = invwy;
    p[1][2] = 0.0f;
    p[1][3] = 0.0f;

    p[2][0] = vp->offset[0] * invwx;
    p[2][1] = vp->offset[1] * invwy;
    p[3][0] = -p[2][0];
    p[3][1] = -p[2][1];

    if (perspective)
    {
        p[2][2] = (vp->depth[1] + vp->depth[0]) * invz;
        p[2][3] = 1.0f;

        p[3][2] = -2.0f * vp->depth[0] * vp->depth[1] * invz;
        p[3][3] = 0.0f;
    }
    else
    {
        p[2][2] = 2.0f * invz;
        p[2][3] = 0.0f;

        p[3][2] = -(vp->depth[1] + vp->depth[0]) * invz;
        p[3][3] = 1.0f;
    }
}
