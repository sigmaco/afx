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

#include "afx/math/afxTransform.h"

AFX afxTransform const AFX_TRANSFORM_ZERO;
AFX afxTransform const AFX_TRANSFORM_IDENTITY;


_AFXINL void AfxTransformMakeIdentity(afxTransform *t)
{
    afxError err = NIL;
    AfxAssert(t);
    AfxV4dMakeIdentity(t->origin);
    AfxQuatMakeIdentity(t->orientation);
    AfxM3dMakeIdentity(t->scaleShear);
    t->flags = NIL;
}

_AFXINL void AfxTransformZero(afxTransform *t)
{
    afxError err = NIL;
    AfxAssert(t);
    AfxV4dZero(t->origin);
    AfxQuatZero(t->orientation);
    AfxM3dZero(t->scaleShear);
    t->flags = AFX_TRANSFORM_FLAG_ALL;
}

_AFXINL void AfxTransformSet(afxTransform *t, afxV4d const origin, afxQuat const orientation, afxM3d const scaleShear)
{
    afxError err = NIL;
    AfxAssert(t);
    AfxAssert(orientation);
    AfxAssert(scaleShear);
    AfxV4dCopy(t->origin, origin);
    AfxQuatCopy(t->orientation, orientation);
    AfxM3dCopy(t->scaleShear, scaleShear);
    t->flags = AFX_TRANSFORM_FLAG_ALL;
}

_AFXINL void AfxTransformSetWithIdentityCheck(afxTransform *t, afxV4d const origin, afxQuat const orientation, afxM3d const scaleShear)
{
    afxError err = NIL;
    AfxAssert(t);
    AfxAssert(orientation);
    AfxAssert(scaleShear);
    AfxTransformSet(t, origin, orientation, scaleShear);
    t->flags = NIL | (AfxV4dIsIdentity(origin) ? NIL : AFX_TRANSFORM_FLAG_ORIGIN) | (AfxQuatIsIdentity(orientation) ? NIL : AFX_TRANSFORM_FLAG_ORIENTATION) | (AfxM3dIsIdentity(scaleShear) ? NIL : AFX_TRANSFORM_FLAG_SCALESHEAR);
}

_AFXINL void AfxTransformMultiply(afxTransform const *t, afxTransform const *other, afxTransform *out)
{
    afxError err = NIL;
    AfxAssert(out);
    AfxAssert(t);
    AfxAssert(other);

    afxM3d OrientationA;
    afxM3d OrientationB;
    afxM3d Temp, tmp;

    AfxQuatMakeRotationM3d(other->orientation, OrientationB);
    AfxM3dMultiply(OrientationB, other->scaleShear, tmp);
    AfxM3dMultiply(tmp, out->scaleShear, Temp);
    AfxM3dMultiply(OrientationB, Temp, out->scaleShear);

    AfxM3dCopy(Temp, out->scaleShear);
    AfxM3dTranspose(Temp, out->scaleShear);
    out->origin[0] = t->scaleShear[0][1] * other->origin[1] + t->scaleShear[0][2] * other->origin[2] + other->origin[0] * t->scaleShear[0][0];
    out->origin[1] = t->scaleShear[1][0] * other->origin[0] + t->scaleShear[1][1] * other->origin[1] + t->scaleShear[1][2] * other->origin[2];
    out->origin[2] = t->scaleShear[2][0] * other->origin[0] + t->scaleShear[2][1] * other->origin[1] + t->scaleShear[2][2] * other->origin[2];
    afxReal v6 = out->origin[0];
    afxReal v7 = out->origin[1];
    afxReal v8 = out->origin[2];
    AfxQuatMakeRotationM3d(t->orientation, OrientationA);
    out->origin[0] = OrientationA[0][2] * v8 + OrientationA[0][1] * v7 + OrientationA[0][0] * v6;
    out->origin[1] = OrientationA[1][2] * v8 + OrientationA[1][1] * v7 + OrientationA[1][0] * v6;
    out->origin[2] = OrientationA[2][0] * v6 + OrientationA[2][2] * v8 + OrientationA[2][1] * v7;
    out->origin[0] = t->origin[0] + out->origin[0];
    out->origin[1] = t->origin[1] + out->origin[1];
    out->origin[2] = t->origin[2] + out->origin[2];

    AfxQuatMultiply(t->orientation, other->orientation, out->orientation);
    out->flags = other->flags | t->flags;
}

_AFXINL void AfxTransformPreMultiply(afxTransform *t, afxTransform const* pre)
{
    afxTransform tmp;
    AfxTransformMultiply(pre, t, &tmp);
    AfxCopy(t, &tmp, sizeof(t));
}

_AFXINL void AfxTransformPostMultiply(afxTransform *t, afxTransform const* post)
{
    afxTransform tmp;
    AfxTransformMultiply(t, post, &tmp);
    AfxCopy(t, &tmp, sizeof(t));
}

_AFXINL void ScaleVectorPlusScaleVector4(afxV4d Dest, afxReal C0, afxV4d const V0, afxReal C1, afxV4d const V1)
{
    Dest[0] = C0 * V0[0] + C1 * V1[0];
    Dest[1] = C0 * V0[1] + C1 * V1[1];
    Dest[2] = C0 * V0[2] + C1 * V1[2];
    Dest[3] = C0 * V0[3] + C1 * V1[3];
}

_AFXINL void ScaleMatrixPlusScaleMatrix3x3(afxM3d Dest, float C0, afxM3d const M0, float C1, afxM3d const M1)
{
    Dest[0][0] = C0 * M0[0][0] + C1 * M1[0][0];
    Dest[0][1] = C0 * M0[0][1] + C1 * M1[0][1];
    Dest[0][2] = C0 * M0[0][2] + C1 * M1[0][2];
    Dest[1][0] = C0 * M0[1][0] + C1 * M1[1][0];
    Dest[1][1] = C0 * M0[1][1] + C1 * M1[1][1];
    Dest[1][2] = C0 * M0[1][2] + C1 * M1[1][2];
    Dest[2][0] = C0 * M0[2][0] + C1 * M1[2][0];
    Dest[2][1] = C0 * M0[2][1] + C1 * M1[2][1];
    Dest[2][2] = C0 * M0[2][2] + C1 * M1[2][2];
}

_AFXINL void AfxTransformBlendLinearly(afxTransform *t, afxTransform const* a, afxTransform const* b, afxReal time)
{
    afxError err = NIL;
    AfxAssert(t);
    AfxAssert(a);
    AfxAssert(b);

    t->flags = b->flags | a->flags;

    if (t->flags & AFX_TRANSFORM_FLAG_ORIGIN)
    {
        afxReal v5 = 1.0 - time;
        t->origin[0] = v5 * a->origin[0] + time * b->origin[0];
        t->origin[1] = v5 * a->origin[1] + time * b->origin[1];
        t->origin[2] = v5 * a->origin[2] + time * b->origin[2];
    }
    else
    {
        AfxV4dMakeIdentity(t->origin);
    }

    if (t->flags & AFX_TRANSFORM_FLAG_ORIENTATION)
    {
        afxReal C0 = 1.0 - time;
        ScaleVectorPlusScaleVector4(t->orientation, C0, a->orientation, time, b->orientation);
        AfxQuatNormalize(t->orientation, t->orientation);
    }
    else
    {
        AfxQuatMakeIdentity(t->orientation);
    }

    if (t->flags & AFX_TRANSFORM_FLAG_SCALESHEAR)
    {
        afxReal v7 = 1.0 - time;
        ScaleMatrixPlusScaleMatrix3x3(t->scaleShear, v7, a->scaleShear, time, b->scaleShear);
    }
    else
    {
        AfxM3dMakeIdentity(t->scaleShear);
    }
}

_AFXINL void AfxTransformCopy(afxTransform *t, afxTransform const* in)
{
    afxError err = NIL;
    AfxAssert(t);
    AfxAssert(in);
    AfxQuatCopy(t->orientation, in->orientation);
    AfxM3dCopy(t->scaleShear, in->scaleShear);
    AfxV4dCopy(t->origin, in->origin);
    t->flags = in->flags;
}

_AFXINL void AfxTransformInvert(afxTransform const *t, afxTransform *out)
{
    afxError err = NIL;
    AfxAssert(out);
    AfxAssert(t);

    afxQuat q;
    AfxQuatInvert(t->orientation, q);
    afxM3d iqm;
    AfxQuatMakeRotationM3d(q, iqm);
    afxM3d ss;
    AfxM3dInvert(t->scaleShear, ss);
    afxM3d tmp;
    AfxM3dMultiply(ss, iqm, tmp);
    AfxM3dMultiply(iqm, tmp, ss);
    AfxM3dCopy(tmp, ss);
    AfxM3dTranspose(tmp, ss);
    
    afxV4d ip;
    AfxV4dNegatePoint(t->origin, ip);
    
    afxV4d ip2;
    ip2[0] = ss[0][2] * ip[2] + ss[0][1] * ip[1] + ss[0][0] * ip[0];
    ip2[1] = ss[1][0] * ip[0] + ss[1][2] * ip[2] + ss[1][1] * ip[1];
    ip2[2] = ss[2][1] * ip[1] + ss[2][0] * ip[0] + ss[2][2] * ip[2];
    
    afxV4d ip3;
    ip3[0] = iqm[0][2] * ip2[2] + iqm[0][1] * ip2[1] + iqm[0][0] * ip2[0];
    ip3[1] = iqm[1][0] * ip2[0] + iqm[1][2] * ip2[2] + iqm[1][1] * ip2[1];
    ip3[2] = iqm[2][1] * ip2[1] + iqm[2][0] * ip2[0] + iqm[2][2] * ip2[2];
    ip3[3] = 1;

    AfxTransformSetWithIdentityCheck(out, ip3, q, ss);
}

char ClipPositionDofs(afxV4d Position, unsigned int AllowedDOFs)
{
    char result; // al

    if (AllowedDOFs & 7)
    {
        if (!(AllowedDOFs & 1))
            *Position = 0.0;
        if (!(AllowedDOFs & 2))
            Position[1] = 0.0;
        if (!(AllowedDOFs & 4))
            Position[2] = 0.0;
        result = 1;
    }
    else
    {
        Position[2] = 0.0;
        Position[1] = 0.0;
        Position[0] = 0.0;
        result = 0;
    }
    return result;
}

char ClipOrientationDofs(afxQuat Orientation, unsigned int AllowedDOFs)
{
    char result; // al

    if (AllowedDOFs & 0x38)
    {
        if (!(AllowedDOFs & 8))
            Orientation[0] = 0.0;

        if (!(AllowedDOFs & 0x10))
            Orientation[1] = 0.0;

        if (!(AllowedDOFs & 0x20))
            Orientation[2] = 0.0;

        AfxQuatNormalize(Orientation, Orientation);
        result = 1;
    }
    else
    {
        Orientation[2] = 0.0;
        Orientation[1] = 0.0;
        Orientation[0] = 0.0;
        Orientation[3] = 1.0;
        result = 0;
    }
    return result;
}

_AFXINL void AfxTransformClipDofs(afxTransform *t, afxFlags AllowedDOFs)
{
    if (!ClipPositionDofs(t->origin, AllowedDOFs))
        t->flags &= 0xFFFFFFFE;

    if (!ClipOrientationDofs(t->orientation, AllowedDOFs))
        t->flags &= 0xFFFFFFFD;

    if (AllowedDOFs & 0x1C0)
    {
        if (!(AllowedDOFs & 0x40))
        {
            t->scaleShear[0][0] = 1.0;
            t->scaleShear[1][0] = 0.0;
            t->scaleShear[2][0] = 0.0;
        }

        if ((AllowedDOFs & 0x80u) == 0)
        {
            t->scaleShear[0][1] = 0.0;
            t->scaleShear[1][1] = 1.0;
            t->scaleShear[2][1] = 0.0;
        }

        if (!(AllowedDOFs & 0x100))
        {
            t->scaleShear[0][2] = 0.0;
            t->scaleShear[1][2] = 0.0;
            t->scaleShear[2][2] = 1.0;
        }
    }
    else
    {
        AfxM3dMakeIdentity(t->scaleShear);
    }
}

_AFXINL afxReal AfxTransformDet(afxTransform const *t)
{
    return (t->scaleShear[1][1] * t->scaleShear[2][2] - t->scaleShear[1][2] * t->scaleShear[2][1]) * t->scaleShear[0][0]
         - (t->scaleShear[2][2] * t->scaleShear[1][0] - t->scaleShear[1][2] * t->scaleShear[2][0]) * t->scaleShear[0][1]
         + (t->scaleShear[2][1] * t->scaleShear[1][0] - t->scaleShear[1][1] * t->scaleShear[2][0]) * t->scaleShear[0][2];
}

_AFXINL void AfxTransformComposeAffineM4d(afxTransform const *t, afxM4d m)
{
    // From void __cdecl granny::BuildCompositeTransform4x4(const granny::transform *Transform, float *Composite4x4)     
    afxV3d *v2;
    afxM3d Orientation;
    afxM3d MultBuffer;

    AfxQuatMakeRotationM3d(t->orientation, Orientation);

    if (t->flags & AFX_TRANSFORM_FLAG_SCALESHEAR)
    {
        AfxM3dMultiply(Orientation, t->scaleShear, MultBuffer);
        v2 = MultBuffer;
    }
    else
    {
        v2 = Orientation;
    }

    m[0][0] = v2[0][0]; // está transpondo?
    m[0][1] = v2[1][0];
    m[0][2] = v2[2][0];
    m[0][3] = 0.0;
    
    m[1][0] = v2[0][1];
    m[1][1] = v2[1][1];
    m[1][2] = v2[2][1];    
    m[1][3] = 0.0;

    m[2][0] = v2[0][2];
    m[2][1] = v2[1][2];
    m[2][2] = v2[2][2];
    m[2][3] = 0.0;

    m[3][0] = t->origin[0];
    m[3][1] = t->origin[1];
    m[3][2] = t->origin[2];
    m[3][3] = 1.0;
}
