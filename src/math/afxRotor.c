/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA Advanced Math Extensions for Qwadro

#include "qwadro/math/afxRotor.h"
#include "qwadro/math/afxQuaternion.h"
#include "qwadro/math/afxMatrix.h"
#include "qwadro/math/afxVector.h"

_AFXINL void AfxSetBiv3d(afxV3d biv, afxReal b01, afxReal b02, afxReal b12)
{
    afxError err = { 0 };
    AFX_ASSERT(biv);
    AfxV3dSet(biv, b01, b02, b12);
}

// Wedge product

_AFXINL void AfxWedgeBiv3d(afxV3d biv, afxV3d const u, afxV3d const v)
{
    biv[0] = u[0] * v[1] - u[1] * v[0]; // XY
    biv[1] = u[0] * v[2] - u[2] * v[0]; // XZ
    biv[2] = u[1] * v[2] - u[2] * v[1]; // YZ    
}


_AFX afxRotor AFX_ROTOR_ZERO = { 0, 0, 0, 0 };
_AFX afxRotor AFX_ROTOR_IDENTITY = { 0, 0, 0, 1 };

_AFXINL void AfxResetRotor(afxRotor r)
{
    afxError err = { 0 };
    AFX_ASSERT(r);
    AfxV4dCopy(r, AFX_ROTOR_IDENTITY);
}

_AFXINL void AfxZeroRotor(afxRotor r)
{
    afxError err = { 0 };
    AFX_ASSERT(r);
    AfxV4dCopy(r, AFX_ROTOR_ZERO);
}

_AFXINL void AfxSetRotor(afxRotor r, afxReal b01, afxReal b02, afxReal b12, afxReal a)
{
    afxError err = { 0 };
    AFX_ASSERT(r);
    AfxV4dSet(r, b01, b02, b12, a);
}

// computa o rotor que rotaciona um unit vector a um outro
// uses the usual trick to get the half angle

_AFXINL void AfxMakeRotor(afxRotor r, afxV3d const from, afxV3d const to)
{
    afxError err = { 0 };
    AFX_ASSERT(r);
    AFX_ASSERT(from);
    AFX_ASSERT(to);
    r[3] = 1.f + AfxV3dDot(to, from);
    // the left side of the products have b a, not a b, so flip
    afxV3d minusb;
    AfxWedgeBiv3d(r, to, from);    
    AfxNormalizeRotor(r, r);
}

// angle+plane, plane deve ser normalized

_AFXINL void AfxMakeRotorFromBiv3d(afxRotor r, afxV3d const plane, afxReal /*rad*/angle)
{
    afxError err = { 0 };
    AFX_ASSERT(r);
    AFX_ASSERT(plane);
    afxReal sina = AfxSinf(angle / 2.f);
    // O lado esquerdo dos produtos têm b a, não a b
    AfxV3dScale(r, plane, -sina);
    r[3] = AfxCosf(angle / 2.f);
}

// Rotor3-Rotor3 product
// non-optimized

_AFXINL void AfxMultiplyRotor(afxRotor r, afxRotor const a, afxRotor const b)
{
    afxError err = { 0 };
    AFX_ASSERT(r);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    // here we just expanded the geometric product rules

    r[0] = a[0] * b[3] + a[3] * b[0] + a[2] * b[1] - a[1] * b[2];
    r[1] = a[1] * b[3] + a[3] * b[1] - a[2] * b[0] + a[0] * b[2];
    r[2] = a[2] * b[3] + a[3] * b[2] + a[1] * b[0] - a[0] * b[1];    
    r[3] = a[3] * b[3] - a[0] * b[0] - a[1] * b[1] - a[2] * b[2];
}

/// R x R*
// non-optimized

_AFXINL void AfxRotateVectorByRotor(afxV3d v, afxRotor const p, afxV3d const x)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(p);
    AFX_ASSERT(x);
    // q = P x
    afxV3d q;
    q[0] = p[3] * x[0] + x[1] * p[0] + x[2] * p[1];
    q[1] = p[3] * x[1] - x[0] * p[0] + x[2] * p[2];
    q[2] = p[3] * x[2] - x[0] * p[1] - x[1] * p[2];

    afxReal q012 = x[0] * p[2] - x[1] * p[1] + x[2] * p[0]; // trivector

    // r = q P*
    v[0] = p[3] * q[0] + q[1] * p[0] + q[2] * p[1] + q012 * p[2];
    v[1] = p[3] * q[1] - q[0] * p[0] - q012 * p[1] + q[2] * p[2];
    v[2] = p[3] * q[2] + q012 * p[0] - q[0] * p[1] - q[1] * p[2];

    // trivector part of the result is always zero
}

_AFXINL void AfxConjugateRotor(afxRotor r, afxRotor const in)
{
    afxError err = { 0 };
    AFX_ASSERT(r);
    AFX_ASSERT(in);
    AfxV4dNegAffine(r, in);
}

_AFXINL afxReal afxMagRotorSq(afxRotor const r)
{
    afxError err = { 0 };
    AFX_ASSERT(r);
    return AfxV4dDot(r, r);
}

_AFXINL afxReal afxMagRotor(afxRotor const r)
{
    afxError err = { 0 };
    AFX_ASSERT(r);
    return AfxV4dMag(r);
}

_AFXINL void AfxNormalizeRotor(afxRotor r, afxRotor const in)
{
    afxError err = { 0 };
    AFX_ASSERT(r);
    AFX_ASSERT(in);
    afxReal l = afxMagRotor(r);
    r[3] = in[3] / l;
    r[0] = in[0] / l;
    r[1] = in[1] / l;
    r[2] = in[2] / l;
}

_AFXINL void AfxMakeM3dFromRotor(afxM3d m, afxRotor const r)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(r);
    afxV3d x, y, z;
    AfxRotateVectorByRotor(x, r, AFX_V3D_X);
    AfxRotateVectorByRotor(y, r, AFX_V3D_Y);
    AfxRotateVectorByRotor(z, r, AFX_V3D_Z);
    AfxM3dSet(m, x, y, z);
}

// rotate a rotor by another

_AFXINL void AfxRotateRotor(afxRotor r, afxRotor const other)
{
    afxError err = { 0 };
    AFX_ASSERT(r);
    AFX_ASSERT(other);
    afxRotor t, i;
    AfxConjugateRotor(i, r);
    AfxMultiplyRotor(t, r, other);
    AfxMultiplyRotor(r, t, i);
}

// geometric product (for reference), produces twice the angle, negative direction

_AFXINL void AfxGeoRotor(afxRotor r, afxV3d const a, afxV3d const b)
{    
    afxError err = { 0 };
    AFX_ASSERT(r);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    afxV3d w;
    AfxWedgeBiv3d(w, a, b);
    return AfxMakeRotorFromBiv3d(r, w, AfxV3dDot(a, b));
}
