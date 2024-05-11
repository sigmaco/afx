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
 *                       (c) 2017 SIGMA, Engitech, Scitech, Serpro
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA Foundation Math <https://sigmaco.org/math>

#include "qwadro/math/afxMatrix.h"
#include "qwadro/math/afxOpticalMatrix.h"
#include "qwadro/math/afxMathDefs.h"
#include "qwadro/math/afxVector.h"
#include "qwadro/math/afxPlane.h"

afxClipSpace const  AFX_CLIP_SPACE_QWADRO =
{
    .nonRh = FALSE,
    .boundary = afxClipBoundary_ZERO_TO_ONE,
    .boundMin = {-1,-1, 0 },
    .boundMax = { 1, 1, 1 }
};

afxClipSpace const  AFX_CLIP_SPACE_OPENGL =
{
    .nonRh = FALSE,
    .boundary = afxClipBoundary_NEG_ONE_TO_ONE,
    .boundMin = {-1,-1,-1 },
    .boundMax = { 1, 1, 1 }
};

afxClipSpace const  AFX_CLIP_SPACE_VULKAN =
{
    .nonRh = FALSE,
    .boundary = afxClipBoundary_ZERO_TO_ONE,
    .boundMin = {-1,-1, 0 },
    .boundMax = { 1, 1, 1 }
};

afxClipSpace const  AFX_CLIP_SPACE_D3D =
{
    .nonRh = TRUE,
    .boundary = afxClipBoundary_ZERO_TO_ONE,
    .boundMin = {-1,-1, 0 },
    .boundMax = { 1, 1, 1 }
};

////////////////////////////////////////////////////////////////////////////////
// Projective transformation matrix operations                                //
////////////////////////////////////////////////////////////////////////////////

_AFXINL void AfxComputeShadowM4d(afxM4d m, afxPlane const p, afxReal const lightPos[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(!AfxV3dIsZero(p));
    AfxAssert(!AfxV3dIsInfinite(p));

    // plane must be normalized
    afxReal d = AfxFindPlaneDistance(p, lightPos);
#if !0
    m[0][0] = d - lightPos[0] * p[0];
    m[1][0] = -lightPos[0] * p[1];
    m[2][0] = -lightPos[0] * p[2];
    m[3][0] = -lightPos[0] * p[AFX_PLANE_OFFSET];

    m[0][1] = -lightPos[1] * p[0];
    m[1][1] = d - lightPos[1] * p[1];
    m[2][1] = -lightPos[1] * p[2];
    m[3][1] = -lightPos[1] * p[AFX_PLANE_OFFSET];

    m[0][2] = -lightPos[2] * p[0];
    m[1][2] = -lightPos[2] * p[1];
    m[2][2] = d - lightPos[2] * p[2];
    m[3][2] = -lightPos[2] * p[AFX_PLANE_OFFSET];

    m[0][3] = -p[0];
    m[1][3] = -p[1];
    m[2][3] = -p[2];
    m[3][3] = d - p[AFX_PLANE_OFFSET];
#else
    m[0][0] = d - lightPos[0] * p->normal[0];
    m[0][1] =    -lightPos[1] * p->normal[0];
    m[0][2] =    -lightPos[2] * p->normal[0];
    m[0][3] =    -p->normal[0];
    m[1][0] =    -lightPos[0] * p->normal[1];
    m[1][1] = d - lightPos[1] * p->normal[1];
    m[1][2] =    -lightPos[2] * p->normal[1];
    m[1][3] =    -p->normal[1];
    m[2][0] =    -lightPos[0] * p->normal[2];
    m[2][1] =    -lightPos[1] * p->normal[2];
    m[2][2] = d - lightPos[2] * p->normal[2];
    m[2][3] =    -p->normal[2];
    m[3][0] =    -lightPos[0] * p->offset;
    m[3][1] =    -lightPos[1] * p->offset;
    m[3][2] =    -lightPos[2] * p->offset;
    m[3][3] = d - p->offset;
#endif
}

_AFXINL void AfxCubemapMatrix_Direct3D(afxM4d m, afxNat face)
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

_AFXINL void AfxCubemapMatrix_OpenGL(afxM4d m, afxNat face)
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
        m[2][2] =-1;
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

_AFXINL void AfxComputeLookToMatrix(afxM4d m, afxV3d const eye, afxV3d const dir, afxV3d const up, afxClipSpace const* clip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(!AfxV3dIsZero(dir));
    AfxAssert(!AfxV3dIsInfinite(dir));
    AfxAssert(!AfxV3dIsZero(up));
    AfxAssert(!AfxV3dIsInfinite(up));

    if (clip->nonRh) // Direct3D
    {
        // Should be compatible with XMMATRIX XMMatrixLookToLH(FXMVECTOR EyePosition, FXMVECTOR EyeDirection, FXMVECTOR UpDirection)

        afxV4d x, y, z;
        AfxNormalizeV3d(z, dir);
        z[3] = 0;
        AfxCrossV3d(x, up, z);
        x[3] = 0;
        AfxNormalizeV3d(x, x);
        AfxCrossV3d(y, z, x);
        y[3] = 0;

        afxV3d negEye;
        AfxNegV3d(negEye, eye);

        afxV4d const w = { AfxDotV3d(x, negEye), AfxDotV3d(y, negEye), AfxDotV3d(z, negEye), (afxReal)1 };
        AfxSetM4d(m, x, y, z, w);
    }
    else // OpenGL/Vulkan/Qwadro
    {
        // Should be compatible with glLookTo

        afxV3d target;
        AfxAddV3d(target, eye, dir);
        AfxComputeLookAtMatrix(m, eye, target, up, FALSE);
    }
}

_AFXINL void AfxComputeLookAtMatrix(afxM4d m, afxV3d const eye, afxV3d const target, afxV3d const up, afxClipSpace const* clip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(eye);
    AfxAssert(target);
    AfxAssert(up);

    if (clip->nonRh) // Direct3D
    {
        // Should be compatible with XMMATRIX XMMatrixLookAtLH(FXMVECTOR EyePosition, FXMVECTOR FocusPosition, FXMVECTOR UpDirection)

        afxV4d dir;
        AfxSubV3d(dir, target, eye);
        AfxComputeLookToMatrix(m, eye, dir, up, clip);
    }
    else // OpenGL/Vulkan/Qwadro
    {
        // Should be compatible with glLookAt

        afxV4d dir;
        AfxSubV3d(dir, eye, target);
        AfxComputeLookToMatrix(m, eye, dir, up, clip);

        afxV4d f, u, s;
        AfxSubV3d(f, target, eye);
        AfxNormalizeV3d(f, f);
        AfxCrossV3d(s, f, up);
        AfxCrossV3d(u, s, f);
        AfxNegV3d(f, f);
        s[3] = 0.f;
        u[3] = 0.f;
        f[3] = 0.f;
        afxV4d w = { -AfxDotV3d(s, eye), -AfxDotV3d(u, eye), AfxDotV3d(f, eye), 1.f };
        AfxSetM4d(m, s, u, f, w);
    }
}

// ORTHOGRAPHIC ////////////////////////////////////////////////////////////////

_AFXINL void AfxComputeOrthographicMatrix(afxM4d m, afxV2d const extent, afxReal near, afxReal far, afxClipSpace const* clip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(extent);
    AfxAssert(!AfxRealIsEqual(extent[0], 0.0f, 0.00001f));
    AfxAssert(!AfxRealIsEqual(extent[1], 0.0f, 0.00001f));
    AfxAssert(!AfxRealIsEqual(far, near, 0.00001f));
    AfxResetM4d(m);

    if (clip->nonRh)
    {
        AfxAssert(clip->boundary == afxClipBoundary_ZERO_TO_ONE); // Direct3D

        // Should be compatible with XMMATRIX XMMatrixOrthographicLH(float ViewWidth, float ViewHeight, float NearZ, float FarZ)

        afxReal range = 1.f / (far - near);
        m[0][0] = 2.f / extent[0];
        m[1][1] = 2.f / extent[1];
        m[2][2] = range;
        m[3][2] = -range * near;
    }
    else
    {
        if (clip->boundary == afxClipBoundary_ZERO_TO_ONE) // Vulkan
        {
            // Should be compatible with XMMATRIX XMMatrixOrthographicRH(float ViewWidth, float ViewHeight, float NearZ, float FarZ)

            afxReal range = 1.f / (near - far);
            m[0][0] = 2.f / extent[0];
            m[1][1] = 2.f / extent[1];
            m[2][2] = range;
            m[3][2] = range * near;
        }
        else if (clip->boundary == afxClipBoundary_NEG_ONE_TO_ONE) // OpenGL
        {
            AfxComputeBasicOrthographicMatrix(m, extent[0] / extent[1], 1, far, clip);
        }
        else
        {
            AfxThrowError();
        }
    }
}

_AFXINL void AfxComputeOffcenterOrthographicMatrix(afxM4d m, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far, afxClipSpace const* clip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxResetM4d(m);

    AfxAssert(!AfxRealIsEqual(right, left, 0.00001f));
    AfxAssert(!AfxRealIsEqual(top, bottom, 0.00001f));
    AfxAssert(!AfxRealIsEqual(far, near, 0.00001f));

    if (clip->nonRh)
    {
        AfxAssert(clip->boundary == afxClipBoundary_ZERO_TO_ONE); // Direct3D

        // Should be compatible with XMMATRIX XM_CALLCONV XMMatrixOrthographicOffCenterLH(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop, float NearZ, float FarZ)

        afxReal rw = 1.f / (right - left);
        afxReal rh = 1.f / (top - bottom);
        afxReal range = 1.f / (far - near); // [ 0, 1 ]
        m[0][0] = 2.f * rw;
        m[1][1] = 2.f * rh;
        m[2][2] = range;
        m[3][0] = -(left + right) * rw;
        m[3][1] = -(top + bottom) * rh;
        m[3][2] = -range * near;
    }
    else
    {
        if (clip->boundary == afxClipBoundary_ZERO_TO_ONE) // Vulkan
        {
            // Should be compatible with XMMATRIX XM_CALLCONV XMMatrixOrthographicOffCenterRH(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop, float NearZ, float FarZ)

            afxReal rw = 1.f / (right - left);
            afxReal rh = 1.f / (top - bottom);
            afxReal range = 1.f / (near - far); // [ 0, 1 ]
            m[0][0] = 2.f * rw;
            m[1][1] = 2.f * rh;
            m[2][2] = range;
            m[3][0] = -(left + right) * rw;
            m[3][1] = -(top + bottom) * rh;
            m[3][2] = range * near;
        }
        else if (clip->boundary == afxClipBoundary_NEG_ONE_TO_ONE) // OpenGL
        {
            // Should be compatible with glOrtho
            
            afxReal rw = 1.f / (right - left);
            afxReal rh = 1.f / (top - bottom);
            afxReal range = -1.f / (far - near); // [ -1, 1 ]
            m[0][0] = 2.f * rw;
            m[1][1] = 2.f * rh;
            m[2][2] = 2.f * range;
            m[3][0] = -(right + left) * rw;
            m[3][1] = -(top + bottom) * rh;
            m[3][2] = (far + near) * range;
        }
        else
        {
            AfxThrowError();
        }
    }
}

_AFXINL void AfxComputeBoundingOrthographicMatrix(afxM4d m, afxAabb const aabb, afxClipSpace const* clip)
{
    // Computa uma afxM4d de projeção ortográfica desde uma afxAabb.
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(aabb); // afxAabb deve estar no view space.

    // OpenGL
    AfxComputeOffcenterOrthographicMatrix(m, aabb[0][0], aabb[1][0], aabb[0][1], aabb[1][1], -aabb[1][2], -aabb[0][2], clip);
}

_AFXINL void AfxComputeBasicOrthographicMatrix(afxM4d m, afxReal aspectRatio, afxReal scale, afxReal range, afxClipSpace const* clip)
{
    // Computa uma afxM4d de projeção ortográfica genérica.
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(aspectRatio); // w/h
    
    // OpenGL

    if (aspectRatio >= 1.f)
    {
        AfxComputeOffcenterOrthographicMatrix(m, -scale * aspectRatio, scale * aspectRatio, -scale, scale, -scale - range, scale + range, clip);
    }
    else
    {
        aspectRatio = 1.f / aspectRatio;
        AfxComputeOffcenterOrthographicMatrix(m, -scale, scale, -scale / aspectRatio, scale / aspectRatio, -scale - range, scale + range, clip);
    }
}

// PERSPECTIVE /////////////////////////////////////////////////////////////////

_AFXINL void AfxComputePerspectiveMatrix(afxM4d m, afxV2d const extent, afxReal near, afxReal far, afxClipSpace const* clip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(extent);
    AfxAssert(!AfxRealIsEqual(extent[0], 0.0f, 0.00001f));
    AfxAssert(!AfxRealIsEqual(extent[1], 0.0f, 0.00001f));
    AfxAssert(!AfxRealIsEqual(far, near, 0.00001f));
    AfxAssert(near > 0.f && far > 0.f);
    AfxZeroM4d(m);

    if (clip->nonRh)
    {
        AfxAssert(clip->boundary == afxClipBoundary_ZERO_TO_ONE); // Direct3D

        // Should be compatible with XMMATRIX XMMatrixPerspectiveLH(float ViewWidth, float ViewHeight, float NearZ, float FarZ)
        
        afxReal near2 = 2.f * near;
        afxReal range = far / (far - near);
        m[0][0] = near2 / extent[0];
        m[1][1] = near2 / extent[1];
        m[2][2] = range;
        m[2][3] = 1.f;
        m[3][2] = -range * near;
    }
    else
    {
        if (clip->boundary == afxClipBoundary_ZERO_TO_ONE) // Vulkan
        {
            // Should be compatible with XMMATRIX XMMatrixPerspectiveRH(float ViewWidth, float ViewHeight, float NearZ, float FarZ)

            afxReal near2 = 2.f * near;
            afxReal range = far / (near - far);
            m[0][0] = near2 / extent[0];
            m[1][1] = near2 / extent[1];
            m[2][2] = range;
            m[2][3] = -1.f;
            m[3][2] = range * near;
        }
        else if (clip->boundary == afxClipBoundary_NEG_ONE_TO_ONE) // OpenGL
        {
            AfxThrowError();
        }
        else
        {
            AfxThrowError();
        }
    }
}

_AFXINL void AfxComputeFovPerspectiveMatrix(afxM4d m, afxReal fovY, afxReal aspectRatio, afxReal near, afxReal far, afxClipSpace const* clip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(!AfxRealIsEqual(fovY, 0.0f, 0.00001f * 2.0f));
    AfxAssert(!AfxRealIsEqual(aspectRatio, 0.0f, 0.00001f));
    AfxAssert(!AfxRealIsEqual(far, near, 0.00001f));
    AfxAssert(near > 0.f && far > 0.f);
    AfxZeroM4d(m);

    if (clip->nonRh)
    {
        AfxAssert(clip->boundary == afxClipBoundary_ZERO_TO_ONE); // Direct3D

        // Should be compatible with XMMATRIX XMMatrixPerspectiveFovLH(float FovAngleY, float AspectRatio, float NearZ, float FarZ)

        afxReal h = AfxCosf(0.5f * fovY) / AfxSinf(0.5f * fovY);
        afxReal w = h / aspectRatio;
        afxReal range = far / (far - near);
        m[0][0] = w;
        m[1][1] = h;
        m[2][2] = range;
        m[2][3] = 1.f;
        m[3][2] = -range * near;
    }
    else
    {
        if (clip->boundary == afxClipBoundary_ZERO_TO_ONE) // Vulkan
        {
            // Should be compatible with XMMATRIX XMMatrixPerspectiveFovRH(float FovAngleY, float AspectRatio, float NearZ, float FarZ)

            afxReal h = AfxCosf(0.5f * fovY) / AfxSinf(0.5f * fovY);
            afxReal w = h / aspectRatio;
            afxReal range = far / (near - far);
            m[0][0] = w;
            m[1][1] = h;
            m[2][2] = range;
            m[2][3] = -1.f;
            m[3][2] = range * near;
        }
        else if (clip->boundary == afxClipBoundary_NEG_ONE_TO_ONE) // OpenGL
        {
            // Should be compatible with glPerspective

            afxReal f = 1.f / AfxTanf(fovY * 0.5f);
            afxReal range = 1.f / (near - far);
            m[0][0] = f / aspectRatio;
            m[1][1] = f;
            m[2][2] = (near + far) * range;
            m[2][3] = -1.f;
            m[3][2] = 2.f * near * far * range;
        }
        else
        {
            AfxThrowError();
        }
    }
}

_AFXINL void AfxComputeOffcenterPerspectiveMatrix(afxM4d m, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far, afxClipSpace const* clip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(!AfxRealIsEqual(right, left, 0.00001f));
    AfxAssert(!AfxRealIsEqual(top, bottom, 0.00001f));
    AfxAssert(!AfxRealIsEqual(far, near, 0.00001f));
    AfxAssert(near > 0.f && far > 0.f);
    AfxZeroM4d(m);

    if (clip->nonRh)
    {
        AfxAssert(clip->boundary == afxClipBoundary_ZERO_TO_ONE); // Direct3D

        // Should be compatible with XMMATRIX XMMatrixPerspectiveOffCenterLH(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop, float NearZ, float FarZ)

        afxReal near2 = 2.f * near;
        afxReal rw = 1.f / (right - left);
        afxReal rh = 1.f / (top - bottom);
        afxReal range = far / (far - near);
        m[0][0] = near2 * rw;
        m[1][1] = near2 * rh;
        m[2][0] = -(left + right) * rw;
        m[2][1] = -(top + bottom) * rh;
        m[2][2] = range;
        m[2][3] = 1.f;
        m[3][2] = -range * near;
    }
    else
    {
        if (clip->boundary == afxClipBoundary_ZERO_TO_ONE) // Vulkan
        {
            // Should be compatible with XMMATRIX XMMatrixPerspectiveOffCenterRH(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop, float NearZ, float FarZ)

            afxReal rw = 1.f / (right - left);
            afxReal rh = 1.f / (top - bottom);
            afxReal range = far / (near - far);
            afxReal near2 = 2.f * near;
            m[0][0] = near2 * rw;
            m[1][1] = near2 * rh;
            m[2][0] = (left + right) * rw;
            m[2][1] = (top + bottom) * rh;
            m[2][2] = range;
            m[2][3] = -1.f;
            m[3][2] = range * near;
        }
        else if (clip->boundary == afxClipBoundary_NEG_ONE_TO_ONE) // OpenGL
        {
            // Preprojective matrix
            // Should be compatible with glFrustum

            afxReal rw = 1.f / (right - left);
            afxReal rh = 1.f / (top - bottom);
            afxReal range = -1.f / (far - near);
            afxReal near2 = 2.f * near;
            m[0][0] = near2 * rw;
            m[1][1] = near2 * rh;
            m[2][0] = (right + left) * rw;
            m[2][1] = (top + bottom) * rh;
            m[2][2] = (far + near) * range;
            m[2][3] = -1.f;
            m[3][2] = far * near2 * range;
        }
        else
        {
            AfxThrowError();
        }
    }
}

_AFXINL void AfxComputeBasicPerspectiveMatrix(afxM4d m, afxReal aspectRatio, afxReal range, afxClipSpace const* clip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxComputeFovPerspectiveMatrix(m, AFX_PI / 4.f, aspectRatio, 0.01f, range, clip);
}

// MakeView
// Memory layout:

_AFXINL void AfxComputeRenderWareViewM4d(afxM4d m, afxM4d const cam)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(cam);

    afxM4d inv;
    AfxInvertM4d(cam, inv);
    m[0][0] = -inv[0][0];
    m[0][1] = inv[0][1];
    m[0][2] = inv[0][2];
    m[0][3] = 0.0f;
    m[1][0] = -inv[1][0];
    m[1][1] = inv[1][1];
    m[1][2] = inv[1][2];
    m[1][3] = 0.0f;
    m[2][0] = -inv[2][0];
    m[2][1] = inv[2][1];
    m[2][2] = inv[2][2];
    m[2][3] = 0.0f;
    m[3][0] = -inv[3][0];
    m[3][1] = inv[3][1];
    m[3][2] = inv[3][2];
    m[3][3] = 1.0f;
}

// MakeProjection
// Memory layout:

_AFXINL void AfxComputeRenderWareProjectionM4d(afxM4d m, afxViewport const* vp, afxBool perspective)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(vp);
    AfxAssert(vp->depth[1]);
    AfxAssert(vp->depth[0] < vp->depth[1]);
    AfxAssert(vp->offset);
    AfxAssert(vp->extent);

    afxReal rw = 1.f / vp->extent[0];
    afxReal rh = 1.f / vp->extent[1];
    afxReal rd = 1.f / (vp->depth[1] - vp->depth[0]);

    m[0][0] = rw;
    m[0][1] = 0.f;
    m[0][2] = 0.f;
    m[0][3] = 0.f;

    m[1][0] = 0.f;
    m[1][1] = rh;
    m[1][2] = 0.f;
    m[1][3] = 0.f;

    m[2][0] = vp->offset[0] * rw;
    m[2][1] = vp->offset[1] * rh;
    m[3][0] = -m[2][0];
    m[3][1] = -m[2][1];

    if (perspective)
    {
        m[2][2] = (vp->depth[1] + vp->depth[0]) * rd;
        m[2][3] = 1.f;

        m[3][2] = -2.f * vp->depth[0] * vp->depth[1] * rd;
        m[3][3] = 0.f;
    }
    else
    {
        m[2][2] = 2.f * rd;
        m[2][3] = 0.f;

        m[3][2] = -(vp->depth[1] + vp->depth[0]) * rd;
        m[3][3] = 1.f;
    }
}

// DecomposePerspective

_AFXINL void AfxDecomposePerspectiveM4d(afxM4d m, afxV2d const extent, afxReal* near, afxReal* far)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);

    // OpenGL
    AfxThrowError();
    *near = m[3][2] / (m[2][2] - 1.f);
    *far = m[3][2] / (m[2][2] + 1.f);
}

_AFXINL void AfxDecomposeFovPerspectiveM4d(afxM4d m, afxReal* fovY, afxReal* aspectRatio, afxReal* near, afxReal* far)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);

    // OpenGL
    *fovY = 2.f * AfxAtanf(1.f / m[1][1]);
    *aspectRatio = m[1][1] / m[0][0];
    *near = m[3][2] / (m[2][2] - 1.f);
    *far = m[3][2] / (m[2][2] + 1.f);
}

_AFXINL void AfxDecomposeOffcenterPerspectiveM4d(afxM4d m, afxReal* left, afxReal* right, afxReal* top, afxReal* bottom, afxReal* near, afxReal* far)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);

    // OpenGL
    afxReal n = m[3][2] / (m[2][2] - 1.f);
    afxReal f = m[3][2] / (m[2][2] + 1.f);
    *left = (n / m[0][0]) * (m[2][0] - 1.f);
    *right = (n / m[0][0]) * (m[2][0] + 1.f);
    *top = (n / m[1][1]) * (m[2][1] + 1.f);
    *bottom = (n / m[1][1]) * (m[2][1] - 1.f);
    *near = n;
    *far = f;
}
