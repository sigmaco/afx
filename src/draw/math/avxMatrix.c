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

// This code is part of SIGMA Foundation Math <https://sigmaco.org/math>

#include "../impl/avxImplementation.h"
#include "qwadro/inc/math/afxMatrix.h"
#include "qwadro/inc/draw/math/avxMatrix.h"
#include "qwadro/inc/math/afxMathDefs.h"
#include "qwadro/inc/math/afxVector.h"
#include "qwadro/inc/math/bound/afxPlane.h"

#if 0
avxClipSpace const  AVX_CLIP_SPACE_QWADRO =
{
    .nonRh = FALSE,
    .boundary = avxClipSpaceDepth_ZERO_TO_ONE,
    .boundMin = {-1,-1, 0 },
    .boundMax = { 1, 1, 1 }
};

avxClipSpace const  AVX_CLIP_SPACE_OPENGL =
{
    .nonRh = FALSE,
    .boundary = avxClipSpaceDepth_NEG_ONE_TO_ONE,
    .boundMin = {-1,-1,-1 },
    .boundMax = { 1, 1, 1 }
};

avxClipSpace const  AVX_CLIP_SPACE_VULKAN =
{
    .nonRh = FALSE,
    .boundary = avxClipSpaceDepth_ZERO_TO_ONE,
    .boundMin = {-1,-1, 0 },
    .boundMax = { 1, 1, 1 }
};

avxClipSpace const  AVX_CLIP_SPACE_D3D =
{
    .nonRh = TRUE,
    .boundary = avxClipSpaceDepth_ZERO_TO_ONE,
    .boundMin = {-1,-1, 0 },
    .boundMax = { 1, 1, 1 }
};
#endif

////////////////////////////////////////////////////////////////////////////////
// Projective transformation matrix operations                                //
////////////////////////////////////////////////////////////////////////////////

_AVXINL void AfxComputeShadowM4d(afxM4d m, afxPlane const p, afxReal const lightPos[3])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(m);
    AFX_ASSERT(!AfxV3dIsZero(p.normal));
    AFX_ASSERT(!AfxV3dIsInfinite(p.normal));

    // plane must be normalized
    afxReal d = AfxPlaneFindV3d(&p, lightPos);
#if !0
    m[0][0] = d - lightPos[0] * p.normal[0];
    m[1][0] = -lightPos[0] * p.normal[1];
    m[2][0] = -lightPos[0] * p.normal[2];
    m[3][0] = -lightPos[0] * p.offset;

    m[0][1] = -lightPos[1] * p.normal[0];
    m[1][1] = d - lightPos[1] * p.normal[1];
    m[2][1] = -lightPos[1] * p.normal[2];
    m[3][1] = -lightPos[1] * p.offset;

    m[0][2] = -lightPos[2] * p.normal[0];
    m[1][2] = -lightPos[2] * p.normal[1];
    m[2][2] = d - lightPos[2] * p.normal[2];
    m[3][2] = -lightPos[2] * p.offset;

    m[0][3] = -p.normal[0];
    m[1][3] = -p.normal[1];
    m[2][3] = -p.normal[2];
    m[3][3] = d - p.offset;
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

_AVXINL void AfxCubemapMatrix_Direct3D(afxM4d m, afxUnit face)
{
    afxError err = NIL;
    AFX_ASSERT(m);
    AfxM4dZero(m);

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

_AVXINL void AfxCubemapMatrix_OpenGL(afxM4d m, afxUnit face)
{
    afxError err = NIL;
    AFX_ASSERT(m);
    AfxM4dZero(m);

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

_AVXINL void AfxComputeLookToMatrix(afxM4d m, afxV3d const eye, afxV3d const dir, afxV3d const up, afxBool nonRh)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(m);
    AFX_ASSERT(!AfxV3dIsZero(dir));
    AFX_ASSERT(!AfxV3dIsInfinite(dir));
    AFX_ASSERT(!AfxV3dIsZero(up));
    AFX_ASSERT(!AfxV3dIsInfinite(up));

    if (nonRh) // Direct3D
    {
        // Should be compatible with XMMATRIX XMMatrixLookToLH(FXMVECTOR EyePosition, FXMVECTOR EyeDirection, FXMVECTOR UpDirection)

        afxV4d x, y, z;
        AfxV3dNormalize(z, dir);
        z[3] = 0;
        AfxV3dCross(x, up, z);
        x[3] = 0;
        AfxV3dNormalize(x, x);
        AfxV3dCross(y, z, x);
        y[3] = 0;

        afxV3d negEye;
        AfxV3dNeg(negEye, eye);

        afxV4d const w = { AfxV3dDot(x, negEye), AfxV3dDot(y, negEye), AfxV3dDot(z, negEye), (afxReal)1 };
        AfxM4dSet(m, x, y, z, w);
    }
    else // OpenGL/Vulkan/Qwadro
    {
        // Should be compatible with glLookTo

        afxV3d target;
        AfxV3dAdd(target, eye, dir);
        AfxComputeLookAtMatrix(m, eye, target, up, FALSE);
    }
}

_AVXINL void AfxComputeLookAtMatrix(afxM4d m, afxV3d const eye, afxV3d const target, afxV3d const up, afxBool nonRh)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(m);
    AFX_ASSERT(eye);
    AFX_ASSERT(target);
    AFX_ASSERT(up);

    if (nonRh) // Direct3D
    {
        // Should be compatible with XMMATRIX XMMatrixLookAtLH(FXMVECTOR EyePosition, FXMVECTOR FocusPosition, FXMVECTOR UpDirection)

        afxV4d dir;
        AfxV3dSub(dir, target, eye);
        AfxComputeLookToMatrix(m, eye, dir, up, nonRh);
    }
    else // OpenGL/Vulkan/Qwadro
    {
        // Should be compatible with glLookAt

        afxV4d dir;
        AfxV3dSub(dir, eye, target);
        AfxComputeLookToMatrix(m, eye, dir, up, nonRh);

        afxV4d f, u, s;
        AfxV3dSub(f, target, eye);
        AfxV3dNormalize(f, f);
        AfxV3dCross(s, f, up);
        AfxV3dCross(u, s, f);
        AfxV3dNeg(f, f);
        s[3] = 0.f;
        u[3] = 0.f;
        f[3] = 0.f;
        afxV4d w = { -AfxV3dDot(s, eye), -AfxV3dDot(u, eye), AfxV3dDot(f, eye), 1.f };
        AfxM4dSet(m, s, u, f, w);
    }
}

// ORTHOGRAPHIC ////////////////////////////////////////////////////////////////

_AVXINL void AfxComputeOrthographicMatrix(afxM4d m, afxV2d const extent, afxReal near, afxReal far, afxBool nonRh, avxClipSpaceDepth clip)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(m);
    AFX_ASSERT(extent);
    AFX_ASSERT(!AfxRealIsEqual(extent[0], 0.0f, 0.00001f));
    AFX_ASSERT(!AfxRealIsEqual(extent[1], 0.0f, 0.00001f));
    AFX_ASSERT(!AfxRealIsEqual(far, near, 0.00001f));
    AfxM4dReset(m);

    if (nonRh)
    {
        AFX_ASSERT(clip == avxClipSpaceDepth_ZERO_TO_ONE); // Direct3D

        // Should be compatible with XMMATRIX XMMatrixOrthographicLH(float ViewWidth, float ViewHeight, float NearZ, float FarZ)

        afxReal range = 1.f / (far - near);
        m[0][0] = 2.f / extent[0];
        m[1][1] = 2.f / extent[1];
        m[2][2] = range;
        m[3][2] = -range * near;
    }
    else
    {
        if (clip == avxClipSpaceDepth_ZERO_TO_ONE) // Vulkan
        {
            // Should be compatible with XMMATRIX XMMatrixOrthographicRH(float ViewWidth, float ViewHeight, float NearZ, float FarZ)

            afxReal range = 1.f / (near - far);
            m[0][0] = 2.f / extent[0];
            m[1][1] = 2.f / extent[1];
            m[2][2] = range;
            m[3][2] = range * near;
        }
        else if (clip == avxClipSpaceDepth_NEG_ONE_TO_ONE) // OpenGL
        {
            AfxComputeBasicOrthographicMatrix(m, extent[0] / extent[1], 1, far, nonRh, clip);
        }
        else
        {
            AfxThrowError();
        }
    }
}

_AVXINL void AfxComputeOffcenterOrthographicMatrix(afxM4d m, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far, afxBool nonRh, avxClipSpaceDepth clip)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(m);
    AfxM4dReset(m);

    AFX_ASSERT(!AfxRealIsEqual(right, left, 0.00001f));
    AFX_ASSERT(!AfxRealIsEqual(top, bottom, 0.00001f));
    AFX_ASSERT(!AfxRealIsEqual(far, near, 0.00001f));

    if (nonRh)
    {
        AFX_ASSERT(clip == avxClipSpaceDepth_ZERO_TO_ONE); // Direct3D

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
        if (clip == avxClipSpaceDepth_ZERO_TO_ONE) // Vulkan
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
        else if (clip == avxClipSpaceDepth_NEG_ONE_TO_ONE) // OpenGL
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

_AVXINL void AfxComputeBoundingOrthographicMatrix(afxM4d m, afxBox const aabb, afxBool nonRh, avxClipSpaceDepth clip)
{
    // Computa uma afxM4d de projeção ortográfica desde uma afxBox.
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(m);
    //AFX_ASSERT(aabb); // afxBox deve estar no view space.

    // OpenGL
    AfxComputeOffcenterOrthographicMatrix(m, aabb.sup[0], aabb.inf[0], aabb.sup[1], aabb.inf[1], -aabb.inf[2], -aabb.sup[2], nonRh, clip);
}

_AVXINL void AfxComputeBasicOrthographicMatrix(afxM4d m, afxReal aspectRatio, afxReal scale, afxReal range, afxBool nonRh, avxClipSpaceDepth clip)
{
    // Computa uma afxM4d de projeção ortográfica genérica.
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(m);
    AFX_ASSERT(aspectRatio); // w/h
    
    // OpenGL

    if (aspectRatio >= 1.f)
    {
        AfxComputeOffcenterOrthographicMatrix(m, -scale * aspectRatio, scale * aspectRatio, -scale, scale, -scale - range, scale + range, nonRh, clip);
    }
    else
    {
        aspectRatio = 1.f / aspectRatio;
        AfxComputeOffcenterOrthographicMatrix(m, -scale, scale, -scale / aspectRatio, scale / aspectRatio, -scale - range, scale + range, nonRh, clip);
    }
}

// PERSPECTIVE /////////////////////////////////////////////////////////////////

_AVXINL void AfxComputePerspectiveMatrix(afxM4d m, afxV2d const extent, afxReal near, afxReal far, afxBool nonRh, avxClipSpaceDepth clip)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(m);
    AFX_ASSERT(extent);
    AFX_ASSERT(!AfxRealIsEqual(extent[0], 0.0f, 0.00001f));
    AFX_ASSERT(!AfxRealIsEqual(extent[1], 0.0f, 0.00001f));
    AFX_ASSERT(!AfxRealIsEqual(far, near, 0.00001f));
    AFX_ASSERT(near > 0.f && far > 0.f);
    AfxM4dZero(m);

    if (nonRh)
    {
        AFX_ASSERT(clip == avxClipSpaceDepth_ZERO_TO_ONE); // Direct3D

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
        if (clip == avxClipSpaceDepth_ZERO_TO_ONE) // Vulkan
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
        else if (clip == avxClipSpaceDepth_NEG_ONE_TO_ONE) // OpenGL
        {
            AfxThrowError();
        }
        else
        {
            AfxThrowError();
        }
    }
}

_AVXINL void AfxComputeFovPerspectiveMatrix(afxM4d m, afxReal fovY, afxReal aspectRatio, afxReal near, afxReal far, afxBool nonRh, avxClipSpaceDepth clip)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(m);
    AFX_ASSERT(!AfxRealIsEqual(fovY, 0.0f, 0.00001f * 2.0f));
    AFX_ASSERT(!AfxRealIsEqual(aspectRatio, 0.0f, 0.00001f));
    AFX_ASSERT(!AfxRealIsEqual(far, near, 0.00001f));
    AFX_ASSERT(near > 0.f && far > 0.f);
    AfxM4dZero(m);

    if (nonRh)
    {
        AFX_ASSERT(clip == avxClipSpaceDepth_ZERO_TO_ONE); // Direct3D

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
        if (clip == avxClipSpaceDepth_ZERO_TO_ONE) // Vulkan
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
        else if (clip == avxClipSpaceDepth_NEG_ONE_TO_ONE) // OpenGL
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

_AVXINL void AfxComputeOffcenterPerspectiveMatrix(afxM4d m, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far, afxBool nonRh, avxClipSpaceDepth clip)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(m);
    AFX_ASSERT(!AfxRealIsEqual(right, left, 0.00001f));
    AFX_ASSERT(!AfxRealIsEqual(top, bottom, 0.00001f));
    AFX_ASSERT(!AfxRealIsEqual(far, near, 0.00001f));
    AFX_ASSERT(near > 0.f && far > 0.f);
    AfxM4dZero(m);

    if (nonRh)
    {
        AFX_ASSERT(clip == avxClipSpaceDepth_ZERO_TO_ONE); // Direct3D

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
        if (clip == avxClipSpaceDepth_ZERO_TO_ONE) // Vulkan
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
        else if (clip == avxClipSpaceDepth_NEG_ONE_TO_ONE) // OpenGL
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

_AVXINL void AfxComputeBasicPerspectiveMatrix(afxM4d m, afxReal aspectRatio, afxReal range, afxBool nonRh, avxClipSpaceDepth clip)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(m);
    AfxComputeFovPerspectiveMatrix(m, AFX_PI / 4.f, aspectRatio, 0.01f, range, nonRh, clip);
}

// MakeView
// Memory layout:

_AVXINL void AfxComputeRenderWareViewM4d(afxM4d m, afxM4d const cam)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(m);
    AFX_ASSERT(cam);

    afxM4d inv;
    AfxM4dInvert(inv, cam);
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

_AVXINL void AfxComputeRenderWareProjectionM4d(afxM4d m, afxViewport const* vp, afxBool perspective)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(m);
    AFX_ASSERT(vp);
    AFX_ASSERT(vp->maxDepth);
    AFX_ASSERT(vp->minDepth < vp->maxDepth);
    AFX_ASSERT(vp->origin);
    AFX_ASSERT(vp->extent);

    afxReal rw = 1.f / vp->extent[0];
    afxReal rh = 1.f / vp->extent[1];
    afxReal rd = 1.f / (vp->maxDepth - vp->minDepth);

    m[0][0] = rw;
    m[0][1] = 0.f;
    m[0][2] = 0.f;
    m[0][3] = 0.f;

    m[1][0] = 0.f;
    m[1][1] = rh;
    m[1][2] = 0.f;
    m[1][3] = 0.f;

    m[2][0] = vp->origin[0] * rw;
    m[2][1] = vp->origin[1] * rh;
    m[3][0] = -m[2][0];
    m[3][1] = -m[2][1];

    if (perspective)
    {
        m[2][2] = (vp->maxDepth + vp->minDepth) * rd;
        m[2][3] = 1.f;

        m[3][2] = -2.f * vp->minDepth * vp->maxDepth * rd;
        m[3][3] = 0.f;
    }
    else
    {
        m[2][2] = 2.f * rd;
        m[2][3] = 0.f;

        m[3][2] = -(vp->maxDepth + vp->minDepth) * rd;
        m[3][3] = 1.f;
    }
}

// DecomposePerspective

_AVXINL void AfxDecomposePerspectiveM4d(afxM4d m, afxV2d const extent, afxReal* near, afxReal* far)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(m);

    // OpenGL
    AfxThrowError();
    *near = m[3][2] / (m[2][2] - 1.f);
    *far = m[3][2] / (m[2][2] + 1.f);
}

_AVXINL void AfxDecomposeFovPerspectiveM4d(afxM4d m, afxReal* fovY, afxReal* aspectRatio, afxReal* near, afxReal* far)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(m);

    // OpenGL
    *fovY = 2.f * AfxAtanf(1.f / m[1][1]);
    *aspectRatio = m[1][1] / m[0][0];
    *near = m[3][2] / (m[2][2] - 1.f);
    *far = m[3][2] / (m[2][2] + 1.f);
}

_AVXINL void AfxDecomposeOffcenterPerspectiveM4d(afxM4d m, afxReal* left, afxReal* right, afxReal* top, afxReal* bottom, afxReal* near, afxReal* far)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(m);

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
