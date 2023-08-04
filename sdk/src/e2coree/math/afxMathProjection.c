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
 *                               (c) 2017 Federação SIGMA
 *                                    www.sigmaco.org
 */

#include "afx/math/afxMathDefs.h"
#include "afx/math/afxScalar.h"

// BaryCentric

_AFXINL void AfxV2dMakeBaryCentric(afxV2d v, afxV2d const a, afxV2d const b, afxV2d const c, afxReal f, afxReal g)
{
    // Result = Position0 + f * (Position1 - Position0) + g * (Position2 - Position0)
    // Adapted from DirectXMath. Works?

    afxV2d p10;
    AfxV2dSub(b, a, p10);

    afxV2d fv;
    AfxV2dSetScalar(fv, f);

    afxV2d p20;
    AfxV2dSub(c, a, p20);

    afxV2d gv;
    AfxV2dSetScalar(gv, g);

    afxV2d tmp;
    AfxV2dCombineMulAndAdd(p10, fv, a, tmp);
    AfxV2dCombineMulAndAdd(p20, gv, tmp, v);
}

_AFXINL void AfxV3dMakeBaryCentric(afxV3d v, afxV3d const a, afxV3d const b, afxV3d const c, afxReal f, afxReal g)
{
    // Result = Position0 + f * (Position1 - Position0) + g * (Position2 - Position0)
    // From DirectXMath
    // https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxvec4barycentric

    afxV3d p10;
    AfxV3dSub(b, a, p10);

    afxV3d fv;
    AfxV3dSetScalar(fv, f);

    afxV3d p20;
    AfxV3dSub(c, a, p20);

    afxV3d gv;
    AfxV3dSetScalar(gv, g);

    afxV3d tmp;
    AfxV3dCombineMulAndAdd(p10, fv, a, tmp);
    AfxV3dCombineMulAndAdd(p20, gv, tmp, v);
}

_AFXINL void AfxV4dMakeBaryCentric(afxV4d v, afxV4d const a, afxV4d const b, afxV4d const c, afxReal f, afxReal g)
{
    // Result = Position0 + f * (Position1 - Position0) + g * (Position2 - Position0)
    // From DirectXMath
    // https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxvec4barycentric

    afxV4d p10;
    AfxV4dSub(b, a, p10);

    afxV4d fv;
    AfxV4dSetScalar(fv, f);

    afxV4d p20;
    AfxV4dSub(c, a, p20);

    afxV4d gv;
    AfxV4dSetScalar(gv, g);

    afxV4d tmp;
    AfxV4dCombineMulAndAdd(p10, fv, a, tmp);
    AfxV4dCombineMulAndAdd(p20, gv, tmp, v);
}

// BaryCentric2

_AFXINL void AfxV2dMakeBaryCentric2(afxV2d v, afxV2d const a, afxV2d const b, afxV2d const c, afxV2d const f, afxV2d const g)
{
    // Result = Position0 + f * (Position1 - Position0) + g * (Position2 - Position0)
    // From DirectXMath

    afxV2d p10;
    AfxV2dSub(b, a, p10);

    afxV2d p20;
    AfxV2dSub(c, a, p20);

    afxV2d tmp;
    AfxV2dCombineMulAndAdd(p10, f, a, tmp);
    AfxV2dCombineMulAndAdd(p20, g, tmp, v);
}

_AFXINL void AfxV3dMakeBaryCentric2(afxV3d v, afxV3d const a, afxV3d const b, afxV3d const c, afxV3d const f, afxV3d const g)
{
    // Result = Position0 + f * (Position1 - Position0) + g * (Position2 - Position0)
    // From DirectXMath

    afxV3d p10;
    AfxV3dSub(b, a, p10);

    afxV3d p20;
    AfxV3dSub(c, a, p20);

    afxV3d tmp;
    AfxV3dCombineMulAndAdd(p10, f, a, tmp);
    AfxV3dCombineMulAndAdd(p20, g, tmp, v);
}

_AFXINL void AfxV4dMakeBaryCentric2(afxV4d v, afxV4d const a, afxV4d const b, afxV4d const c, afxV4d const f, afxV4d const g)
{
    // Result = Position0 + f * (Position1 - Position0) + g * (Position2 - Position0)
    // From DirectXMath

    afxV4d p10;
    AfxV4dSub(b, a, p10);

    afxV4d p20;
    AfxV4dSub(c, a, p20);

    afxV4d tmp;
    AfxV4dCombineMulAndAdd(p10, f, a, tmp);
    AfxV4dCombineMulAndAdd(p20, g, tmp, v);
}

// MakeOrthographicOffCenter(LH/RH)
// Memory layout: row-major
// Coordinate system: RH and LH.
// Inspired on DirectX

// The AfxM4dMakeOrthographic2(LH/RH) function is a special case of the AfxM4dMakeOrthographicOffCenterLH/RH function.
// To create the same projection using AfxM4dMakeOrthographicOffCenterLH/RH, use the following values : l = -w / 2, r = w / 2, b = -h / 2, and t = h / 2.

_AFXINL void AfxM4dMakeOrthographicOffCenterLH(afxM4d m, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(far);
    AfxAssert(near < far);

    m[0][0] = AfxScalar(2) / (right - left);
    m[0][1] = AfxScalar(0);
    m[0][2] = AfxScalar(0);
    m[0][3] = AfxScalar(0);

    m[1][0] = AfxScalar(0);
    m[1][1] = AfxScalar(2) / (top - bottom);
    m[1][2] = AfxScalar(0);
    m[1][3] = AfxScalar(0);

    m[2][0] = AfxScalar(0);
    m[2][1] = AfxScalar(0);
    m[2][2] = AfxScalar(1) / (far - near); // difference
    m[2][3] = AfxScalar(0);

    m[3][0] = (left + right) / (left - right);
    m[3][1] = (top + bottom) / (bottom - top);
    m[3][2] = near / (near - far);
    m[3][3] = AfxScalar(1);
}

_AFXINL void AfxM4dMakeOrthographicOffCenterRH(afxM4d m, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(far);
    AfxAssert(near < far);

    m[0][0] = AfxScalar(2) / (right - left);
    m[0][1] = AfxScalar(0);
    m[0][2] = AfxScalar(0);
    m[0][3] = AfxScalar(0);

    m[1][0] = AfxScalar(0);
    m[1][1] = AfxScalar(2) / (top - bottom);
    m[1][2] = AfxScalar(0);
    m[1][3] = AfxScalar(0);

    m[2][0] = AfxScalar(0);
    m[2][1] = AfxScalar(0);
    m[2][2] = AfxScalar(1) / (near - far); // difference
    m[2][3] = AfxScalar(0);

    m[3][0] = (left + right) / (left - right);
    m[3][1] = (top + bottom) / (bottom - top);
    m[3][2] = near / (near - far);
    m[3][3] = AfxScalar(1);
}

// MakeOrthographic2(LH/RH)
// Memory layout: row-major
// Coordinate system: RH and LH.
// Inspired on DirectX

// All the parameters of the AfxM4dMakeOrthographic2(LH/RH) function are distances in camera space. The parameters describe the dimensions of the view volume.

_AFXINL void AfxM4dMakeOrthographicLH(afxM4d m, afxV2d extent, afxReal near, afxReal far)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(far);
    AfxAssert(near < far);

    m[0][0] = AfxScalar(2) / extent[0];
    m[0][1] = AfxScalar(0);
    m[0][2] = AfxScalar(0);
    m[0][3] = AfxScalar(0);

    m[1][0] = AfxScalar(0);
    m[1][1] = AfxScalar(2) / extent[1];
    m[1][2] = AfxScalar(0);
    m[1][3] = AfxScalar(0);

    m[2][0] = AfxScalar(0);
    m[2][1] = AfxScalar(0);
    m[2][2] = AfxScalar(1) / (far - near); // difference
    m[2][3] = AfxScalar(0);

    m[3][0] = AfxScalar(0);
    m[3][1] = AfxScalar(0);
    m[3][2] = -near / (far - near); // should we use parenthesis here?
    m[3][3] = AfxScalar(1);
}

_AFXINL void AfxM4dMakeOrthographicRH(afxM4d m, afxV2d extent, afxReal near, afxReal far)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(far);
    AfxAssert(near < far);

    m[0][0] = AfxScalar(2) / extent[0];
    m[0][1] = AfxScalar(0);
    m[0][2] = AfxScalar(0);
    m[0][3] = AfxScalar(0);

    m[1][0] = AfxScalar(0);
    m[1][1] = AfxScalar(2) / extent[1];
    m[1][2] = AfxScalar(0);
    m[1][3] = AfxScalar(0);

    m[2][0] = AfxScalar(0);
    m[2][1] = AfxScalar(0);
    m[2][2] = AfxScalar(1) / (near - far); // difference
    m[2][3] = AfxScalar(0);

    m[3][0] = AfxScalar(0);
    m[3][1] = AfxScalar(0);
    m[3][2] = near / (near - far); // difference
    m[3][3] = AfxScalar(1);
}

// AfxM4dMakePerspective2(LH/RH)
// Memory layout: row-major
// Coordinate system: RH and LH.
// Inspired on DirectX

// All the parameters of the AfxM4dMakePerspective2(LH/RH) function are distances in camera space. The parameters describe the dimensions of the view volume.

_AFXINL void AfxM4dMakePerspectiveLH(afxM4d m, afxV2d extent, afxReal near, afxReal far)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(far);
    AfxAssert(near < far);

    m[0][0] = AfxScalar(2) * near / extent[0];
    m[0][1] = AfxScalar(0);
    m[0][2] = AfxScalar(0);
    m[0][3] = AfxScalar(0);

    m[1][0] = AfxScalar(0);
    m[1][1] = AfxScalar(2) * near / extent[1];
    m[1][2] = AfxScalar(0);
    m[1][3] = AfxScalar(0);

    m[2][0] = AfxScalar(0);
    m[2][1] = AfxScalar(0);
    m[2][2] = far / (far - near);
    m[2][3] = AfxScalar(1);

    m[3][0] = AfxScalar(0);
    m[3][1] = AfxScalar(0);
    m[3][2] = near * far / (near - far);
    m[3][3] = AfxScalar(0);
}

_AFXINL void AfxM4dMakePerspectiveRH(afxM4d m, afxV2d extent, afxReal near, afxReal far)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(far);
    AfxAssert(near < far);

    m[0][0] = AfxScalar(2) * near / extent[0];
    m[0][1] = AfxScalar(0);
    m[0][2] = AfxScalar(0);
    m[0][3] = AfxScalar(0);

    m[1][0] = AfxScalar(0);
    m[1][1] = AfxScalar(2) * near / extent[1];
    m[1][2] = AfxScalar(0);
    m[1][3] = AfxScalar(0);

    m[2][0] = AfxScalar(0);
    m[2][1] = AfxScalar(0);
    m[2][2] = far / (near - far);
    m[2][3] = AfxScalar(-1);

    m[3][0] = AfxScalar(0);
    m[3][1] = AfxScalar(0);
    m[3][2] = near * far / (near - far);
    m[3][3] = AfxScalar(0);
}

// AfxM4dMakePerspectiveOffCenter(LH/RH)
// Memory layout: row-major
// Coordinate system: RH and LH.
// Inspired on DirectX

// All the parameters of the AfxM4dMakePerspective2(LH/RH) function are distances in camera space. The parameters describe the dimensions of the view volume.

_AFXINL void AfxM4dMakePerspectiveOffCenterLH(afxM4d m, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(far);
    AfxAssert(near < far);

    m[0][0] = 2 * near / (right - left);
    m[0][1] = 0;
    m[0][2] = 0;
    m[0][3] = 0;

    m[1][0] = 0;
    m[1][1] = 2 * near / (top - bottom);
    m[1][2] = 0;
    m[1][3] = 0;

    m[2][0] = (left + right) / (left - right);
    m[2][1] = (top + bottom) / (bottom - top); // difference
    m[2][2] = far / (far - near); // difference
    m[2][3] = 1;

    m[3][0] = 0;
    m[3][1] = 0;
    m[3][2] = near * far / (near - far);
    m[3][3] = 0;
}

_AFXINL void AfxM4dMakePerspectiveOffCenterRH(afxM4d m, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(far);
    AfxAssert(near < far);

    m[0][0] = 2 * near / (right - left);
    m[0][1] = 0;
    m[0][2] = 0;
    m[0][3] = 0;

    m[1][0] = 0;
    m[1][1] = 2 * near / (top - bottom);
    m[1][2] = 0;
    m[1][3] = 0;

    m[2][0] = (left + right) / (right - left);
    m[2][1] = (top + bottom) / (top - bottom); // difference
    m[2][2] = far / (near - far); // difference
    m[2][3] = -1;

    m[3][0] = 0;
    m[3][1] = 0;
    m[3][2] = near * far / (near - far);
    m[3][3] = 0;
}

// MakeLookAt2(LH/RH)
// Memory layout: row-major
// Coordinate system: LH and RH
// Inspired on DirectX

_AFXINL void AfxM4dMakeLookAtLH(afxM4d m, afxV3d const eye, afxV3d const point, afxV3d const up)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(eye);
    AfxAssert(point);
    AfxAssert(up);

    afxV3d z, x, y;
    AfxV3dSub(point, eye, z);
    AfxV3dNormalize(z, z);
    AfxV3dCross(up, z, x);
    AfxV3dNormalize(x, x);
    AfxV3dCross(z, x, y);

    m[0][0] = x[0];
    m[0][1] = y[0];
    m[0][2] = z[0];
    m[0][3] = AfxScalar(0);

    m[1][0] = x[1];
    m[1][1] = y[1];
    m[1][2] = z[1];
    m[1][3] = AfxScalar(0);

    m[2][0] = x[2];
    m[2][1] = y[2];
    m[2][2] = z[2];
    m[2][3] = AfxScalar(0);

    m[3][0] = -AfxV3dDot(x, eye);
    m[3][1] = -AfxV3dDot(y, eye);
    m[3][2] = -AfxV3dDot(z, eye);
    m[3][3] = AfxScalar(1);
}

_AFXINL void AfxM4dMakeLookAtRH(afxM4d m, afxV3d const eye, afxV3d const point, afxV3d const up)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(eye);
    AfxAssert(point);
    AfxAssert(up);

    afxV3d z, x, y;
    AfxV3dSub(eye, point, z);
    AfxV3dNormalize(z, z);
    AfxV3dCross(up, z, x);
    AfxV3dNormalize(x, x);
    AfxV3dCross(z, x, y);

    m[0][0] = x[0];
    m[0][1] = y[0];
    m[0][2] = z[0];
    m[0][3] = AfxScalar(0);

    m[1][0] = x[1];
    m[1][1] = y[1];
    m[1][2] = z[1];
    m[1][3] = AfxScalar(0);

    m[2][0] = x[2];
    m[2][1] = y[2];
    m[2][2] = z[2];
    m[2][3] = AfxScalar(0);

    m[3][0] = -AfxV3dDot(x, eye);
    m[3][1] = -AfxV3dDot(y, eye);
    m[3][2] = -AfxV3dDot(z, eye);
    m[3][3] = AfxScalar(1);
}

// GAMBIARRA

_AFXINL void AfxCorrectRwMatrix(afxM4d m, afxM4d const in)
{
    AfxM4dCopy(m, in);
    m[0][3] = m[1][3] = m[2][3] = AfxScalar(0);
    m[3][3] = AfxScalar(1);
}

// MakeView
// Memory layout:

_AFXINL void AfxM4dMakeRwView(afxM4d view, afxM4d cam)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(view);
    AfxAssert(cam);

    // librw
    // View Matrix

    afxM4d inv;
    AfxM4dInvert(cam, inv);
    // Since we're looking into positive Z,
    // flip X dst ge a left handed view space.
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

_AFXINL void AfxM4dMakeRwProjection(afxM4d proj, afxV2d offset, afxV2d extent, afxReal nearPlane, afxReal farPlane, afxBool perspective) // viewport offset and extent
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(proj);
    AfxAssert(farPlane);
    AfxAssert(nearPlane < farPlane);
    AfxAssert(offset);
    AfxAssert(extent);

    // librw
    // Projection Matrix
    afxReal invwx = 1.0f / extent[0];
    afxReal invwy = 1.0f / extent[1];
    afxReal invz = 1.0f / (farPlane - nearPlane);

    proj[0][0] = invwx;
    proj[0][1] = 0.0f;
    proj[0][2] = 0.0f;
    proj[0][3] = 0.0f;

    proj[1][0] = 0.0f;
    proj[1][1] = invwy;
    proj[1][2] = 0.0f;
    proj[1][3] = 0.0f;

    proj[2][0] = offset[0] * invwx;
    proj[2][1] = offset[1] * invwy;
    proj[3][0] = -proj[2][0];
    proj[3][1] = -proj[2][1];

    if (perspective)
    {
        proj[2][2] = (farPlane + nearPlane) * invz;
        proj[2][3] = 1.0f;

        proj[3][2] = -2.0f * nearPlane * farPlane * invz;
        proj[3][3] = 0.0f;
    }
    else
    {
        proj[2][2] = 2.0f * invz;
        proj[2][3] = 0.0f;

        proj[3][2] = -(farPlane + nearPlane) * invz;
        proj[3][3] = 1.0f;
    }
}
