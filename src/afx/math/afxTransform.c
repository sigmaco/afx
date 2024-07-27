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

// This code is part of SIGMA Foundation Math <https://sigmaco.org/math>

#include "qwadro/math/afxTransform.h"
#include "qwadro/math/afxMatrix.h"
#include "qwadro/math/afxVector.h"
#include "qwadro/math/afxQuaternion.h"
#include "qwadro/io/afxStream.h"

afxTransform const AFX_TRANSFORM_ZERO =
{
    .flags = NIL,
    .position = { 0.f, 0.f, 0.f },
    .orientation = { 0.f, 0.f, 0.f, 1.f },
    .scaleShear = { { 0.f, 0.f, 0.f}, { 0.f, 0.f, 0.f}, { 0.f, 0.f, 0.f } }
};

afxTransform const AFX_TRANSFORM_IDENTITY =
{
    .flags = NIL,
    .position = { 0.f, 0.f, 0.f },
    .orientation = { 0.f, 0.f, 0.f, 1.f },
    .scaleShear = { { 1.f, 0.f, 0.f}, { 0.f, 1.f, 0.f}, { 0.f, 0.f, 1.f } }
};

_AFXINL afxReal AfxDetTransform(afxTransform const* t)
{
    return AfxM3dDet(t->scaleShear);
}

_AFXINL void AfxResetTransform(afxTransform* t)
{
    afxError err = NIL;
    AfxAssert(t);
    t->flags = NIL;
    AfxV3dZero(t->position);
    AfxQuatReset(t->orientation);
    AfxM3dReset(t->scaleShear);
}

_AFXINL void AfxZeroTransform(afxTransform* t)
{
    afxError err = NIL;
    AfxAssert(t);
    t->flags = NIL;
    AfxV3dZero(t->position);
    AfxQuatZero(t->orientation);
    AfxM3dZero(t->scaleShear);
}

_AFXINL void AfxSetTransform(afxTransform* t, afxV3d const position, afxQuat const orientation, afxM3d const scaleShear)
{
    afxError err = NIL;
    AfxAssert(t);
    AfxAssert(orientation);
    AfxAssert(scaleShear);
    t->flags = afxTransformFlags_ALL;
    AfxV3dCopy(t->position, position);
    AfxQuatCopy(t->orientation, orientation);
    AfxM3dCopy(t->scaleShear, scaleShear);
}

_AFXINL void AfxSetTransformWithIdentityCheck(afxTransform* t, afxV3d const position, afxQuat const orientation, afxM3d const scaleShear)
{
    afxError err = NIL;
    AfxAssert(t);
    AfxAssert(orientation);
    AfxAssert(scaleShear);
    AfxSetTransform(t, position, orientation, scaleShear);
    t->flags = NIL;

    if (!AfxV3dIsZero(position))
        t->flags |= afxTransformFlags_TRANSLATED;
    
    if (!AfxQuatIsIdentity(orientation))
        t->flags |= afxTransformFlags_ROTATED;
        
    if (!AfxM3dIsIdentity(scaleShear))
        t->flags |= afxTransformFlags_DEFORMED;
}

_AFXINL void AfxCopyTransform(afxTransform *t, afxTransform const* in)
{
    afxError err = NIL;
    AfxAssert(t);
    AfxAssert(in);
    t->flags = in->flags;
    AfxV3dCopy(t->position, in->position);
    AfxQuatCopy(t->orientation, in->orientation);
    AfxM3dCopy(t->scaleShear, in->scaleShear);
}

_AFXINL void AfxMultiplyTransform(afxTransform *t, afxTransform const *a, afxTransform const *b)
{
    afxError err = NIL;
    AfxAssert(t);
    AfxAssert(a);
    AfxAssert(b);

    afxM3d OrientationB, Temp, tmp;
    AfxM3dRotationFromQuat(OrientationB, b->orientation);
    AfxM3dMultiply(tmp, OrientationB, b->scaleShear);
    AfxM3dMultiply(Temp, a->scaleShear, tmp);
    TransposeMatrixMultiply3x3(t->scaleShear, OrientationB, Temp);
    
    AfxV3dPostMultiplyM3d(t->position, a->scaleShear, b->position);

    afxM3d OrientationA;
    AfxM3dRotationFromQuat(OrientationA, a->orientation);
    AfxV3dPostMultiplyM3d(t->position, OrientationA, t->position);
    AfxV3dAdd(t->position, t->position, a->position);
    
    AfxQuatMultiply(t->orientation, a->orientation, b->orientation);
    t->flags = b->flags | a->flags;
}

_AFXINL void AfxPreMultiplyTransform(afxTransform* t, afxTransform const* pre)
{
    afxTransform tmp;
    AfxMultiplyTransform(&tmp, pre, t);
    AfxCopyTransform(t, &tmp);
}

_AFXINL void AfxPostMultiplyTransform(afxTransform* t, afxTransform const* post)
{
    afxTransform tmp;
    AfxMultiplyTransform(&tmp, t, post);
    AfxCopyTransform(t, &tmp);
}

_AFXINL void AfxLerpTransform(afxTransform *t, afxTransform const* a, afxTransform const* b, afxReal time)
{
    afxError err = NIL;
    AfxAssert(t);
    AfxAssert(a);
    AfxAssert(b);

    afxTransformFlags flags = b->flags | a->flags;
    t->flags = flags;
    afxReal s = 1.f - time;

    if (!(flags & afxTransformFlags_TRANSLATED)) AfxV3dZero(t->position);
    else
    {
        AfxCombineV3d(t->position, s, a->position, time, b->position);
    }

    if (!(flags & afxTransformFlags_ROTATED)) AfxQuatReset(t->orientation);
    else
    {
        AfxCombineV4d(t->orientation, s, a->orientation, time, b->orientation);
        AfxQuatNormalize(t->orientation, t->orientation);
    }

    if (!(flags & afxTransformFlags_DEFORMED)) AfxM3dReset(t->scaleShear);
    else
    {
        AfxM3dCombine(t->scaleShear, s, a->scaleShear, time, b->scaleShear);
    }
}

_AFXINL void AfxInvertTransform(afxTransform const* in, afxTransform* t)
{
    afxError err = NIL;
    AfxAssert(t);
    AfxAssert(in);

    afxQuat q;
    AfxQuatConj(q, in->orientation);
    
    afxM3d iqm, ss, tmp;
    AfxM3dRotationFromQuat(iqm, q);
    AfxM3dInvert(ss, in->scaleShear);
    AfxM3dMultiply(tmp, ss, iqm);
    TransposeMatrixMultiply3x3(ss, iqm, tmp);

    afxV4d ip, ip2, ip3;
    AfxV3dNeg(ip, in->position);
    AfxV3dPostMultiplyM3d(ip2, ss, ip);
    AfxV3dPostMultiplyM3d(ip3, iqm, ip2);

    AfxSetTransformWithIdentityCheck(t, ip3, q, ss);
}

_AFXINL afxBool ClipPositionDofs(afxV4d Position, afxNat32 allowedDOFs)
{
    afxBool result;

    if (!(result = !!(allowedDOFs & 7))) AfxResetV4d(Position);
    else
    {
        if (!(allowedDOFs & 1))
            *Position = 0;

        if (!(allowedDOFs & 2))
            Position[1] = 0;

        if (!(allowedDOFs & 4))
            Position[2] = 0;
    }
    return result;
}

_AFXINL afxBool ClipOrientationDofs(afxQuat Orientation, afxNat32 allowedDOFs)
{
    afxBool result;

    if (!(result = !!(allowedDOFs & 0x38))) AfxQuatReset(Orientation);
    else
    {
        if (!(allowedDOFs & 8))
            Orientation[0] = 0.0;

        if (!(allowedDOFs & 0x10))
            Orientation[1] = 0.0;

        if (!(allowedDOFs & 0x20))
            Orientation[2] = 0.0;

        AfxQuatNormalize(Orientation, Orientation);
    }
    return result;
}

_AFXINL void AfxClipTransformDofs(afxTransform *t, afxFlags allowedDOFs)
{
    // Should be compatible with void ClipTransformDOFs(transform *Result, unsigned int AllowedDOFs)

    afxError err = NIL;
    AfxAssert(t);
    AfxAssert(allowedDOFs);

    if (!ClipPositionDofs(t->position, allowedDOFs))
        t->flags &= 0xFFFFFFFE;

    if (!ClipOrientationDofs(t->orientation, allowedDOFs))
        t->flags &= 0xFFFFFFFD;

    if (!(allowedDOFs & 0x1C0))
    {
        t->flags &= 0xFFFFFFFB;
        AfxM3dReset(t->scaleShear);
    }
    else
    {
        if (!(allowedDOFs & 0x40))
            AfxV3dSet(t->scaleShear[0], 1, 0, 0);

        if (!(allowedDOFs & 0x80u))
            AfxV3dSet(t->scaleShear[1], 0, 1, 0);

        if (!(allowedDOFs & 0x100))
            AfxV3dSet(t->scaleShear[2], 0, 0, 1);
    }
}

_AFX void AfxComputeTransformM4d(afxTransform const* t, afxM4d m)
{
    // Should be compatible with void BuildCompositeTransform4x4(const transform *Transform, float *Composite4x4)     

    afxError err = NIL;
    AfxAssert(t);
    AfxAssert(m);

    afxTransformFlags flags = t->flags;
    afxBool hasPos = flags & afxTransformFlags_TRANSLATED;
    afxBool hasRot = flags & afxTransformFlags_ROTATED;
    afxBool hasSs = flags & afxTransformFlags_DEFORMED;

    if (hasRot || hasSs)
    {
        afxM3d tmp, tmp2;

        //if (hasRot)
            AfxM3dRotationFromQuat(hasSs ? tmp2 : tmp, t->orientation);

        if (hasSs)
        {
            if (hasRot)
                AfxM3dMultiply(tmp, tmp2, t->scaleShear);
            else
                AfxM3dCopy(tmp, t->scaleShear);
        }
        
        AfxM4dTransposeM3d(m, tmp, hasPos ? t->position : AFX_V4D_W);
    }
    else
    {
        if (!hasPos)
            AfxM4dReset(m);
        else
            AfxM4dTranslation(m, t->position);
    }
}

_AFX void AfxComposeTransformCompactMatrix(afxTransform const* t, afxV3d m[4])
{
    afxError err = NIL;
    AfxAssert(t);
    AfxAssert(m);

    afxTransformFlags flags = t->flags;
    afxBool hasPos = flags & afxTransformFlags_TRANSLATED;
    afxBool hasRot = flags & afxTransformFlags_ROTATED;
    afxBool hasSs = flags & afxTransformFlags_DEFORMED;

    if (hasRot || hasSs)
    {
        afxM3d tmp, tmp2;

        if (hasRot)
            AfxM3dRotationFromQuat(hasSs ? tmp2 : tmp, t->orientation);

        if (hasSs)
        {
            if (hasRot)
                AfxM3dMultiply(tmp, tmp2, t->scaleShear);
            else
                AfxM3dCopy(tmp, t->scaleShear);
        }

        AfxM3dTranspose(m, tmp);

        if (hasPos)
            AfxV3dCopy(m[3], t->position);
    }
    else
    {
        if (hasPos) AfxV3dCopy(m[3], t->position);
        else
        {
            AfxM3dReset(m);
            AfxV3dZero(m[3]);
        }
    }
}

_AFXINL void AfxTransformArrayedAtv3d(afxTransform const* t, afxNat cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = NIL;
    AfxAssert(t);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    AfxM3dPostMultiplyV3d(t->scaleShear, cnt, in, out);
    AfxQuatRotateV3dArray(t->orientation, cnt, out, out);

    for (afxNat i = 0; i < cnt; i++)
        AfxV3dAdd(out[i], t->position, out[i]);
}

_AFXINL void AfxTransformArrayedLtv3d(afxTransform const* t, afxNat cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = NIL;
    AfxAssert(t);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    AfxM3dPostMultiplyV3d(t->scaleShear, cnt, in, out);
    AfxQuatRotateV3dArray(t->orientation, cnt, out, out);
}

_AFXINL void AfxTransformArrayedLtv3dTransposed(afxTransform const* t, afxNat cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = NIL;
    AfxAssert(t);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    // Compatible with TransformVectorInPlaceTransposed(in/out, t)

    afxQuat iq;
    AfxQuatConj(iq, t->orientation);
    AfxQuatRotateV3dArray(iq, cnt, in, out);
    AfxM3dPreMultiplyV3d(t->scaleShear, cnt, out, out);
}

////////////////////////////////////////////////////////////////////////////////

_AFXINL void AfxAssimilateTransforms(afxM3d const ltm, afxM3d const iltm, afxV4d const atv, afxNat cnt, afxTransform const in[], afxTransform out[])
{
    afxError err = NIL;
    AfxAssert(ltm);
    AfxAssert(iltm);
    AfxAssert(atv);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxAssimilateAtv3d(ltm, atv, 1, &in[i].position, &out[i].position);
        AfxAssimilateQuat(ltm, iltm, 1, &in[i].orientation, &out[i].orientation);
        AfxAssimilateLtm3d(ltm, iltm, 1, &in[i].scaleShear, &out[i].scaleShear);
    }
}
