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

#include "qwadro/math/afxTrigonometry.h"


////////////////////////////////////////////////////////////////////////////////
// Arithmetica                                                                //
////////////////////////////////////////////////////////////////////////////////

// Add

_AFXINL void AfxV2dAdd(afxV2d v, afxV2d const a, afxV2d const b)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    v[0] = a[0] + b[0];
    v[1] = a[1] + b[1];
}

_AFXINL void AfxV3dAdd(afxV3d v, afxV3d const a, afxV3d const b)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    v[0] = a[0] + b[0];
    v[1] = a[1] + b[1];
    v[2] = a[2] + b[2];
}

_AFXINL void AfxV4dAdd(afxV4d v, afxV4d const a, afxV4d const b)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    v[0] = a[0] + b[0];
    v[1] = a[1] + b[1];
    v[2] = a[2] + b[2];
    v[3] = a[3] + b[3];
}

// Sub

_AFXINL void AfxV2dSub(afxV2d v, afxV2d const a, afxV2d const b)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    v[0] = a[0] - b[0];
    v[1] = a[1] - b[1];
}

_AFXINL void AfxV3dSub(afxV3d v, afxV3d const a, afxV3d const b)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    v[0] = a[0] - b[0];
    v[1] = a[1] - b[1];
    v[2] = a[2] - b[2];
}

_AFXINL void AfxV4dSub(afxV4d v, afxV4d const a, afxV4d const b)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    v[0] = a[0] - b[0];
    v[1] = a[1] - b[1];
    v[2] = a[2] - b[2];
    v[3] = a[3] - b[3];
}

// Div

_AFXINL void AfxV2dDiv(afxV2d v, afxV2d const a, afxV2d const b)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    v[0] = a[0] / b[0];
    v[1] = a[1] / b[1];
}

_AFXINL void AfxV3dDiv(afxV3d v, afxV3d const a, afxV3d const b)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    v[0] = a[0] / b[0];
    v[1] = a[1] / b[1];
    v[2] = a[2] / b[2];
}

_AFXINL void AfxV4dDiv(afxV4d v, afxV4d const a, afxV4d const b)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    v[0] = a[0] / b[0];
    v[1] = a[1] / b[1];
    v[2] = a[2] / b[2];
    v[3] = a[3] / b[3];
}

// Divs

_AFXINL void AfxV2dDivs(afxV2d v, afxV2d const a, afxReal div)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(div);
    v[0] = a[0] / div;
    v[1] = a[1] / div;
}

_AFXINL void AfxV3dDivs(afxV3d v, afxV3d const a, afxReal div)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(div);
    v[0] = a[0] / div;
    v[1] = a[1] / div;
    v[2] = a[2] / div;
}

_AFXINL void AfxV4dDivs(afxV4d v, afxV4d const a, afxReal div)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(div);
    v[0] = a[0] / div;
    v[1] = a[1] / div;
    v[2] = a[2] / div;
    v[3] = a[3] / div;
}

// Mod

_AFXINL void AfxV2dMod(afxV2d v, afxV2d const a, afxV2d const b)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);

    AfxV2dDiv(v, a, b);
    AfxTruncateV2d(v, v);
    AfxV2dResub(v, b, v, a);
}

_AFXINL void AfxV3dMod(afxV3d v, afxV3d const a, afxV3d const b)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);

    AfxV3dDiv(v, a, b);
    AfxTruncateV3d(v, v);
    AfxV3dResub(v, b, v, a);
}

_AFXINL void AfxV4dMod(afxV4d v, afxV4d const a, afxV4d const b)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);

    AfxV4dDiv(v, a, b);
    AfxTruncateV4d(v, v);
    AfxV4dResub(v, b, v, a);
}

// Scale

_AFXINL void AfxV2dScale(afxV2d v, afxV2d const a, afxReal lambda)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    v[0] = a[0] * lambda;
    v[1] = a[1] * lambda;
}

_AFXINL void AfxV3dScale(afxV3d v, afxV3d const a, afxReal lambda)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    v[0] = a[0] * lambda;
    v[1] = a[1] * lambda;
    v[2] = a[2] * lambda;
}

_AFXINL void AfxV4dScale(afxV4d v, afxV4d const a, afxReal lambda)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    v[0] = a[0] * lambda;
    v[1] = a[1] * lambda;
    v[2] = a[2] * lambda;
    v[3] = a[3] * lambda;
}

// Half

_AFXINL void AfxV2dHalf(afxV2d v, afxV2d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = in[0] * 0.5f;
    v[1] = in[1] * 0.5f;
}

_AFXINL void AfxV3dHalf(afxV3d v, afxV3d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = in[0] * 0.5f;
    v[1] = in[1] * 0.5f;
    v[2] = in[2] * 0.5f;
}

_AFXINL void AfxV4dHalf(afxV4d v, afxV4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = in[0] * 0.5f;
    v[1] = in[1] * 0.5f;
    v[2] = in[2] * 0.5f;
    v[3] = in[3] * 0.5f;
}

// Multiply

_AFXINL void AfxV2dMultiply(afxV2d v, afxV2d const a, afxV2d const b)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    v[0] = a[0] * b[0];
    v[1] = a[1] * b[1];
}

_AFXINL void AfxV3dMultiply(afxV3d v, afxV3d const a, afxV3d const b)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    v[0] = a[0] * b[0];
    v[1] = a[1] * b[1];
    v[2] = a[2] * b[2];
}

_AFXINL void AfxV4dMultiply(afxV4d v, afxV4d const a, afxV4d const b)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    v[0] = a[0] * b[0];
    v[1] = a[1] * b[1];
    v[2] = a[2] * b[2];
    v[3] = a[3] * b[3];
}

// Mad

_AFXINL void AfxV2dMad(afxV2d v, afxV2d const add, afxV2d const mul, afxV2d const f)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(add);
    AFX_ASSERT(mul);
    AFX_ASSERT(f);
    v[0] = add[0] + (mul[0] * f[0]);
    v[1] = add[1] + (mul[1] * f[1]);
}

_AFXINL void AfxV3dMad(afxV3d v, afxV3d const add, afxV3d const mul, afxV3d const f)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(add);
    AFX_ASSERT(mul);
    AFX_ASSERT(f);
    v[0] = add[0] + (mul[0] * f[0]);
    v[1] = add[1] + (mul[1] * f[1]);
    v[2] = add[2] + (mul[2] * f[2]);
}

_AFXINL void AfxV4dMad(afxV4d v, afxV4d const add, afxV4d const mul, afxV4d const f)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(add);
    AFX_ASSERT(mul);
    AFX_ASSERT(f);
    v[0] = add[0] + (mul[0] * f[0]);
    v[1] = add[1] + (mul[1] * f[1]);
    v[2] = add[2] + (mul[2] * f[2]);
    v[3] = add[3] + (mul[3] * f[3]);
}

// AddScaled

_AFXINL void AfxV2dMads(afxV2d v, afxV2d const add, afxV2d const mul, afxReal lambda)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(mul);
    AFX_ASSERT(add);
    v[0] = add[0] + (mul[0] * lambda);
    v[1] = add[1] + (mul[1] * lambda);
}

_AFXINL void AfxV3dMads(afxV3d v, afxV3d const add, afxV3d const mul, afxReal lambda)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(mul);
    AFX_ASSERT(add);
    v[0] = add[0] + (mul[0] * lambda);
    v[1] = add[1] + (mul[1] * lambda);
    v[2] = add[2] + (mul[2] * lambda);
}

_AFXINL void AfxV4dMads(afxV4d v, afxV4d const add, afxV4d const mul, afxReal lambda)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(mul);
    AFX_ASSERT(add);
    v[0] = add[0] + (mul[0] * lambda);
    v[1] = add[1] + (mul[1] * lambda);
    v[2] = add[2] + (mul[2] * lambda);
    v[3] = add[3] + (mul[3] * lambda);
}

// Resub

_AFXINL void AfxV2dResub(afxV2d v, afxV2d const a, afxV2d const b, afxV2d const c)
{
    // Should be compatible with XMVectorNegativeMultiplySubtract

    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    AFX_ASSERT(c);

    v[0] = c[0] - (a[0] * b[0]);
    v[1] = c[1] - (a[1] * b[1]);
}

_AFXINL void AfxV3dResub(afxV3d v, afxV3d const a, afxV3d const b, afxV3d const c)
{
    // Should be compatible with XMVectorNegativeMultiplySubtract

    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    AFX_ASSERT(c);

    v[0] = c[0] - (a[0] * b[0]);
    v[1] = c[1] - (a[1] * b[1]);
    v[2] = c[2] - (a[2] * b[2]);
}

_AFXINL void AfxV4dResub(afxV4d v, afxV4d const a, afxV4d const b, afxV4d const c)
{
    // Should be compatible with XMVectorNegativeMultiplySubtract

    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    AFX_ASSERT(c);

    v[0] = c[0] - (a[0] * b[0]);
    v[1] = c[1] - (a[1] * b[1]);
    v[2] = c[2] - (a[2] * b[2]);
    v[3] = c[3] - (a[3] * b[3]);
}

// Recip

_AFXINL void AfxV2dRecip(afxV2d v, afxV2d const in)
{
    // Should be compatible with XMVECTOR XMVectorReciprocal(FXMVECTOR V)

    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    AfxV2dDiv(v, AFX_V2D_ONE, in);
}

_AFXINL void AfxV3dRecip(afxV3d v, afxV3d const in)
{
    // Should be compatible with XMVECTOR XMVectorReciprocal(FXMVECTOR V)

    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    AfxV3dDiv(v, AFX_V3D_ONE, in);
}

_AFXINL void AfxV4dRecip(afxV4d v, afxV4d const in)
{
    // Should be compatible with XMVECTOR XMVectorReciprocal(FXMVECTOR V)

    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    AfxV4dDiv(v, AFX_V4D_ONE, in);
}

// Negate

_AFXINL void AfxV2dNeg(afxV2d v, afxV2d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = -(in[0]);
    v[1] = -(in[1]);
}

_AFXINL void AfxV3dNeg(afxV3d v, afxV3d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = -(in[0]);
    v[1] = -(in[1]);
    v[2] = -(in[2]);
}

_AFXINL void AfxV4dNeg(afxV4d v, afxV4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = -(in[0]);
    v[1] = -(in[1]);
    v[2] = -(in[2]);
    v[3] = -(in[3]);
}

_AFXINL void AfxV4dNegAffine(afxV4d v, afxV4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = -(in[0]);
    v[1] = -(in[1]);
    v[2] = -(in[2]);
    v[3] = (in[3]);
}


