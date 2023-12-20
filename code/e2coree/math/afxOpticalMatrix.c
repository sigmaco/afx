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

#include "qwadro/math/afxMatrix.h"
#include "qwadro/math/afxOpticalMatrix.h"
#include "qwadro/math/afxMathDefs.h"
#include "qwadro/math/afxReal.h"
#include "qwadro/math/afxVector.h"
#include "qwadro/math/afxPlane.h"

////////////////////////////////////////////////////////////////////////////////
// Projective transformation matrix operations                                //
////////////////////////////////////////////////////////////////////////////////

_AFXINL void AfxComputeShadowM4d(afxReal m[4][4], afxPlane const p, afxReal const lightPos[3])
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

_AFXINL void AfxCubemapMatrix_Direct3D(afxReal m[4][4], afxNat face)
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

_AFXINL void AfxCubemapMatrix_OpenGL(afxReal m[4][4], afxNat face)
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

_AFXINL void AfxComputeLookToMatrix(afxReal m[4][4], afxReal const eye[3], afxReal const dir[3], afxReal const up[3], afxBool lh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(!AfxV3dIsZero(dir));
    AfxAssert(!AfxV3dIsInfinite(dir));
    AfxAssert(!AfxV3dIsZero(up));
    AfxAssert(!AfxV3dIsInfinite(up));

    if (lh) // OpenGL
    {
        // Should be compatible with XMMATRIX XMMatrixLookToLH(FXMVECTOR EyePosition, FXMVECTOR EyeDirection, FXMVECTOR UpDirection)

        afxV4d x, y, z;
        AfxNormalV3d(z, dir);
        z[3] = 0;
        AfxCrossV3d(x, up, z);
        x[3] = 0;
        AfxNormalV3d(x, x);
        AfxCrossV3d(y, z, x);
        y[3] = 0;

        afxV3d negEye;
        AfxNegV3d(negEye, eye);

        afxV4d const w = { AfxDotV3d(x, negEye), AfxDotV3d(y, negEye), AfxDotV3d(z, negEye), (afxReal)1 };
        AfxSetTransposedM4d(m, x, y, z, w);
    }
    else // OpenGL/Vulkan/Qwadro
    {
        // Should be compatible with glLookTo

        afxV3d target;
        AfxAddV3d(target, eye, dir);
        AfxComputeLookAtMatrix(m, eye, target, up, FALSE);
    }
}

_AFXINL void AfxComputeLookAtMatrix(afxReal m[4][4], afxReal const eye[3], afxReal const target[3], afxReal const up[3], afxBool lh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(eye);
    AfxAssert(target);
    AfxAssert(up);

    if (lh) // Direct3D
    {
        // Should be compatible with XMMATRIX XMMatrixLookAtLH(FXMVECTOR EyePosition, FXMVECTOR FocusPosition, FXMVECTOR UpDirection)

        afxV4d dir;
        AfxSubV3d(dir, target, eye);
        AfxComputeLookToMatrix(m, eye, dir, up, TRUE);
    }
    else // OpenGL/Vulkan/Qwadro
    {
        // Should be compatible with glLookAt

        afxV4d dir;
        AfxSubV3d(dir, eye, target);
        AfxComputeLookToMatrix(m, eye, dir, up, TRUE);

        afxV4d f, u, s;
        AfxSubV3d(f, target, eye);
        AfxNormalV3d(f, f);
        AfxCrossV3d(s, f, up);
        AfxCrossV3d(u, s, f);
        AfxNegV3d(f, f);
        s[3] = 0.f;
        u[3] = 0.f;
        f[3] = 0.f;
        afxV4d w = { -AfxDotV3d(s, eye), -AfxDotV3d(u, eye), AfxDotV3d(f, eye), 1.f };
        AfxSetTransposedM4d(m, s, u, f, w);
    }
}

// ORTHOGRAPHIC ////////////////////////////////////////////////////////////////

_AFXINL void AfxComputeOrthographicMatrix(afxReal m[4][4], afxReal const extent[2], afxReal near, afxReal far, afxBool lh, afxClipDepthRange cdr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(extent);
    AfxAssert(!AfxRealIsEqual(extent[0], 0.0f, 0.00001f));
    AfxAssert(!AfxRealIsEqual(extent[1], 0.0f, 0.00001f));
    AfxAssert(!AfxRealIsEqual(far, near, 0.00001f));
    AfxResetM4d(m);

    if (lh)
    {
        AfxAssert(cdr == afxClipDepthRange_ZERO_TO_ONE); // Direct3D

        // Should be compatible with XMMATRIX XMMatrixOrthographicLH(float ViewWidth, float ViewHeight, float NearZ, float FarZ)

        afxReal range = 1.f / (far - near);
        m[0][0] = 2.f / extent[0];
        m[1][1] = 2.f / extent[1];
        m[2][2] = range;
        m[3][2] = -range * near;
    }
    else
    {
        if (cdr == afxClipDepthRange_ZERO_TO_ONE) // Vulkan
        {
            // Should be compatible with XMMATRIX XMMatrixOrthographicRH(float ViewWidth, float ViewHeight, float NearZ, float FarZ)

            afxReal range = 1.f / (near - far);
            m[0][0] = 2.f / extent[0];
            m[1][1] = 2.f / extent[1];
            m[2][2] = range;
            m[3][2] = range * near;
        }
        else if (cdr == afxClipDepthRange_NEG_ONE_TO_ONE) // OpenGL
        {
            AfxResetOrthographicMatrix(m, extent[0] / extent[1], 1, FALSE, cdr);
        }
        else
        {
            AfxThrowError();
        }
    }
}

_AFXINL void AfxComputeOffcenterOrthographicMatrix(afxReal m[4][4], afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far, afxBool lh, afxClipDepthRange cdr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxResetM4d(m);

    AfxAssert(!AfxRealIsEqual(right, left, 0.00001f));
    AfxAssert(!AfxRealIsEqual(top, bottom, 0.00001f));
    AfxAssert(!AfxRealIsEqual(far, near, 0.00001f));

    if (lh)
    {
        AfxAssert(cdr == afxClipDepthRange_ZERO_TO_ONE); // Direct3D

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
        if (cdr == afxClipDepthRange_ZERO_TO_ONE) // Vulkan
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
        else if (cdr == afxClipDepthRange_NEG_ONE_TO_ONE) // OpenGL
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

_AFXINL void AfxComputeOrthographicMatrixFromAabb(afxReal m[4][4], afxAabb const aabb, afxBool lh, afxClipDepthRange cdr)
{
    // Computa uma afxM4d de projeção ortográfica desde uma afxAabb.
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(aabb); // afxAabb deve estar no view space.

    // OpenGL
    AfxComputeOffcenterOrthographicMatrix(m, aabb[0][0], aabb[1][0], aabb[0][1], aabb[1][1], -aabb[1][2], -aabb[0][2], lh, cdr);
}

_AFXINL void AfxResetOrthographicMatrix(afxReal m[4][4], afxReal aspectRatio, afxReal scale, afxBool lh, afxClipDepthRange cdr)
{
    // Computa uma afxM4d de projeção ortográfica genérica.
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(aspectRatio); // w/h

    // OpenGL

    if (aspectRatio >= 1.f)
    {
        AfxComputeOffcenterOrthographicMatrix(m, -scale * aspectRatio, scale * aspectRatio, -scale, scale, -scale - 100.f, scale + 100.f, lh, cdr);
    }
    else
    {
        aspectRatio = 1.f / aspectRatio;
        AfxComputeOffcenterOrthographicMatrix(m, -scale, scale, -scale / aspectRatio, scale / aspectRatio, -scale - 100.f, scale + 100.f, lh, cdr);
    }
}

// PERSPECTIVE /////////////////////////////////////////////////////////////////

_AFXINL void AfxComputePerspectiveMatrix(afxReal m[4][4], afxReal const extent[2], afxReal near, afxReal far, afxBool lh, afxClipDepthRange cdr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(extent);
    AfxAssert(!AfxRealIsEqual(extent[0], 0.0f, 0.00001f));
    AfxAssert(!AfxRealIsEqual(extent[1], 0.0f, 0.00001f));
    AfxAssert(!AfxRealIsEqual(far, near, 0.00001f));
    AfxAssert(near > 0.f && far > 0.f);
    AfxZeroM4d(m);

    if (lh)
    {
        AfxAssert(cdr == afxClipDepthRange_ZERO_TO_ONE); // Direct3D

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
        if (cdr == afxClipDepthRange_ZERO_TO_ONE) // Vulkan
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
        else if (cdr == afxClipDepthRange_NEG_ONE_TO_ONE) // OpenGL
        {
            AfxThrowError();
        }
        else
        {
            AfxThrowError();
        }
    }
}

_AFXINL void AfxComputeFovPerspectiveMatrix(afxReal m[4][4], afxReal fovY, afxReal aspectRatio, afxReal near, afxReal far, afxBool lh, afxClipDepthRange cdr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(!AfxRealIsEqual(fovY, 0.0f, 0.00001f * 2.0f));
    AfxAssert(!AfxRealIsEqual(aspectRatio, 0.0f, 0.00001f));
    AfxAssert(!AfxRealIsEqual(far, near, 0.00001f));
    AfxAssert(near > 0.f && far > 0.f);
    AfxZeroM4d(m);

    if (lh)
    {
        AfxAssert(cdr == afxClipDepthRange_ZERO_TO_ONE); // Direct3D

        // Should be compatible with XMMATRIX XMMatrixPerspectiveFovLH(float FovAngleY, float AspectRatio, float NearZ, float FarZ)

        afxReal h = AfxCos(0.5f * fovY) / AfxSin(0.5f * fovY);
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
        if (cdr == afxClipDepthRange_ZERO_TO_ONE) // Vulkan
        {
            // Should be compatible with XMMATRIX XMMatrixPerspectiveFovRH(float FovAngleY, float AspectRatio, float NearZ, float FarZ)

            afxReal h = AfxCos(0.5f * fovY) / AfxSin(0.5f * fovY);
            afxReal w = h / aspectRatio;
            afxReal range = far / (near - far);
            m[0][0] = w;
            m[1][1] = h;
            m[2][2] = range;
            m[2][3] = -1.f;
            m[3][2] = range * near;
        }
        else if (cdr == afxClipDepthRange_NEG_ONE_TO_ONE) // OpenGL
        {
            // Should be compatible with glPerspective

            afxReal f = 1.f / AfxTan(fovY * 0.5f);
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

_AFXINL void AfxComputeOffcenterPerspectiveMatrix(afxReal m[4][4], afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far, afxBool lh, afxClipDepthRange cdr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(!AfxRealIsEqual(right, left, 0.00001f));
    AfxAssert(!AfxRealIsEqual(top, bottom, 0.00001f));
    AfxAssert(!AfxRealIsEqual(far, near, 0.00001f));
    AfxAssert(near > 0.f && far > 0.f);
    AfxZeroM4d(m);

    if (lh)
    {
        AfxAssert(cdr == afxClipDepthRange_ZERO_TO_ONE); // Direct3D

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
        if (cdr == afxClipDepthRange_ZERO_TO_ONE) // Vulkan
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
        else if (cdr == afxClipDepthRange_NEG_ONE_TO_ONE) // OpenGL
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

_AFXINL void AfxResetPerspectiveMatrix(afxReal m[4][4], afxReal aspectRatio, afxBool lh, afxClipDepthRange cdr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxComputeFovPerspectiveMatrix(m, AFX_PI / 4.f, aspectRatio, 0.01f, 100.f, lh, cdr);
}

// MakeView
// Memory layout:

_AFXINL void AfxComputeRenderWareViewM4d(afxReal view[4][4], afxReal const cam[4][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(view);
    AfxAssert(cam);

    afxM4d inv;
    AfxInverseM4d(inv, cam);
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

    afxReal rw = 1.f / vp->extent[0];
    afxReal rh = 1.f / vp->extent[1];
    afxReal rd = 1.f / (vp->depth[1] - vp->depth[0]);

    p[0][0] = rw;
    p[0][1] = 0.f;
    p[0][2] = 0.f;
    p[0][3] = 0.f;

    p[1][0] = 0.f;
    p[1][1] = rh;
    p[1][2] = 0.f;
    p[1][3] = 0.f;

    p[2][0] = vp->offset[0] * rw;
    p[2][1] = vp->offset[1] * rh;
    p[3][0] = -p[2][0];
    p[3][1] = -p[2][1];

    if (perspective)
    {
        p[2][2] = (vp->depth[1] + vp->depth[0]) * rd;
        p[2][3] = 1.f;

        p[3][2] = -2.f * vp->depth[0] * vp->depth[1] * rd;
        p[3][3] = 0.f;
    }
    else
    {
        p[2][2] = 2.f * rd;
        p[2][3] = 0.f;

        p[3][2] = -(vp->depth[1] + vp->depth[0]) * rd;
        p[3][3] = 1.f;
    }
}

// DecomposePerspective

_AFXINL void AfxDecomposePerspectiveM4d(afxReal m[4][4], afxReal extent[2], afxReal* near, afxReal* far)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);

    // OpenGL
    AfxThrowError();
    *near = m[3][2] / (m[2][2] - 1.f);
    *far = m[3][2] / (m[2][2] + 1.f);
}

_AFXINL void AfxDecomposeFovPerspectiveM4d(afxReal m[4][4], afxReal* fovY, afxReal* aspectRatio, afxReal* near, afxReal* far)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);

    // OpenGL
    *fovY = 2.f * AfxAtan(1.f / m[1][1]);
    *aspectRatio = m[1][1] / m[0][0];
    *near = m[3][2] / (m[2][2] - 1.f);
    *far = m[3][2] / (m[2][2] + 1.f);
}

_AFXINL void AfxDecomposeOffcenterPerspectiveM4d(afxReal m[4][4], afxReal* left, afxReal* right, afxReal* top, afxReal* bottom, afxReal* near, afxReal* far)
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
