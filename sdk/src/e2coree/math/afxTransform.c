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

#include "afx/math/afxTransform.h"
#include "afx/math/afxMatrix.h"
#include "afx/math/afxVector.h"
#include "afx/math/afxQuaternion.h"

AFX afxTransform const AFX_TRANSFORM_ZERO;
AFX afxTransform const AFX_TRANSFORM_IDENTITY;

_AFXINL void AfxResetTransform(afxTransform* t)
{
    afxError err = NIL;
    AfxAssert(t);
    t->flags = NIL;
    AfxZeroV3d(t->position);
    AfxResetQuat(t->orientation);
    AfxResetM3d(t->scaleShear);
}

_AFXINL void AfxZeroTransform(afxTransform* t)
{
    afxError err = NIL;
    AfxAssert(t);
    t->flags = NIL;
    AfxZeroV3d(t->position);
    AfxZeroQuat(t->orientation);
    AfxZeroM3d(t->scaleShear);
}

_AFXINL void AfxSetTransform(afxTransform* t, afxV3d const position, afxQuat const orientation, afxM3d const scaleShear)
{
    afxError err = NIL;
    AfxAssert(t);
    AfxAssert(orientation);
    AfxAssert(scaleShear);
    t->flags = afxTransformFlags_ALL;
    AfxCopyV3d(t->position, position);
    AfxCopyQuat(t->orientation, orientation);
    AfxCopyM3d(t->scaleShear, scaleShear);
}

_AFXINL void AfxSetTransformWithIdentityCheck(afxTransform* t, afxV3d const position, afxQuat const orientation, afxM3d const scaleShear)
{
    afxError err = NIL;
    AfxAssert(t);
    AfxAssert(orientation);
    AfxAssert(scaleShear);
    AfxSetTransform(t, position, orientation, scaleShear);
    t->flags = NIL | (AfxV4dIsIdentity(position) ? NIL : afxTransformFlags_ORIGIN) | (AfxQuatIsIdentity(orientation) ? NIL : afxTransformFlags_ORIENTATION) | (AfxM3dIsIdentity(scaleShear) ? NIL : afxTransformFlags_DEFORM);
}

_AFXINL void AfxMultiplyTransform(afxTransform *t, afxTransform const *a, afxTransform const *b)
{
    afxError err = NIL;
    AfxAssert(t);
    AfxAssert(a);
    AfxAssert(b);

    afxM3d OrientationB, Temp, tmp;
    AfxM3dFromQuat(OrientationB, b->orientation);
    AfxMultiplyM3d(tmp, OrientationB, b->scaleShear);
    AfxMultiplyM3d(Temp, a->scaleShear, tmp);
    AfxMultiplyTransposedM3d(t->scaleShear, OrientationB, Temp);
    
    AfxPostMultiplyV3d(a->scaleShear, b->position, t->position);

    afxM3d OrientationA;
    AfxM3dFromQuat(OrientationA, a->orientation);
    AfxPostMultiplyV3d(OrientationA, t->position, t->position);
    AfxAddV3d(t->position, a->position, t->position);
    
    AfxMultiplyQuat(t->orientation, a->orientation, b->orientation);
    t->flags = b->flags | a->flags;
}

_AFXINL void AfxPreMultiplyTransform(afxTransform* t, afxTransform const* pre)
{
    afxTransform tmp;
    AfxMultiplyTransform(&tmp, pre, t);
    AfxCopy(1, sizeof(tmp), &tmp, t);
}

_AFXINL void AfxPostMultiplyTransform(afxTransform* t, afxTransform const* post)
{
    afxTransform tmp;
    AfxMultiplyTransform(&tmp, t, post);
    AfxCopy(1, sizeof(tmp), &tmp, t);
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

    if (!(flags & afxTransformFlags_ORIGIN)) AfxZeroV3d(t->position);
    else
    {
        AfxCombineV3d(t->position, s, a->position, time, b->position);
    }

    if (!(flags & afxTransformFlags_ORIENTATION)) AfxResetQuat(t->orientation);
    else
    {
        AfxCombineV4d(t->orientation, s, a->orientation, time, b->orientation);
        AfxNormalizeQuat(t->orientation, t->orientation);
    }

    if (!(flags & afxTransformFlags_DEFORM)) AfxResetM3d(t->scaleShear);
    else
    {
        AfxCombineM3d(t->scaleShear, s, a->scaleShear, time, b->scaleShear);
    }
}

_AFXINL void AfxCopyTransform(afxTransform *t, afxTransform const* in)
{
    afxError err = NIL;
    AfxAssert(t);
    AfxAssert(in);
    t->flags = in->flags;
    AfxCopyV3d(t->position, in->position);
    AfxCopyQuat(t->orientation, in->orientation);
    AfxCopyM3d(t->scaleShear, in->scaleShear);
}

_AFXINL void AfxGetInverseTransform(afxTransform* t, afxTransform const* in)
{
    afxError err = NIL;
    AfxAssert(t);
    AfxAssert(in);

    afxQuat q;
    AfxInvertQuat(q, in->orientation);
    
    afxM3d iqm, ss, tmp;
    AfxM3dFromQuat(iqm, q);
    AfxInverseM3d(ss, in->scaleShear);
    AfxMultiplyM3d(tmp, ss, iqm);
    AfxMultiplyTransposedM3d(ss, iqm, tmp);

    afxV4d ip, ip2, ip3;
    AfxNegV3d(ip, in->position);
    AfxPostMultiplyV3d(ss, ip, ip2);
    AfxPostMultiplyV3d(iqm, ip2, ip3);

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

    if (!(result = !!(allowedDOFs & 0x38))) AfxResetQuat(Orientation);
    else
    {
        if (!(allowedDOFs & 8))
            Orientation[0] = 0.0;

        if (!(allowedDOFs & 0x10))
            Orientation[1] = 0.0;

        if (!(allowedDOFs & 0x20))
            Orientation[2] = 0.0;

        AfxNormalizeQuat(Orientation, Orientation);
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
        AfxResetM3d(t->scaleShear);
    }
    else
    {
        if (!(allowedDOFs & 0x40))
            AfxSetV3d(t->scaleShear[0], 1, 0, 0);

        if (!(allowedDOFs & 0x80u))
            AfxSetV3d(t->scaleShear[1], 0, 1, 0);

        if (!(allowedDOFs & 0x100))
            AfxSetV3d(t->scaleShear[2], 0, 0, 1);
    }
}

_AFXINL afxReal AfxDetTransform(afxTransform const* t)
{
    return AfxDetM3d(t->scaleShear);
}

_AFX void AfxComposeTransformM4d(afxTransform const* t, afxReal m[4][4])
{
    // Should be compatible with void BuildCompositeTransform4x4(const transform *Transform, float *Composite4x4)     

    afxError err = NIL;
    AfxAssert(t);
    AfxAssert(m);

    afxTransformFlags flags = t->flags;
    afxBool hasPos = flags & afxTransformFlags_ORIGIN;
    afxBool hasRot = flags & afxTransformFlags_ORIENTATION;
    afxBool hasSs = flags & afxTransformFlags_DEFORM;

    if (hasRot || hasSs)
    {
        afxM3d tmp, tmp2;

        if (hasRot)
            AfxM3dFromQuat(hasSs ? tmp2 : tmp, t->orientation);

        if (hasSs)
        {
            if (hasRot)
                AfxMultiplyM3d(tmp, tmp2, t->scaleShear);
            else
                AfxCopyM3d(tmp, t->scaleShear);
        }
        
        AfxM4dFromTransposedM3d(m, tmp, hasPos ? t->position : AFX_V4D_W);
    }
    else
    {
        if (!hasPos)
            AfxResetM4d(m);
        else
            AfxTranslationM4d(m, t->position);
    }
}

_AFX void AfxComposeTransformCompactMatrix(afxTransform const* t, afxReal m[4][3])
{
    afxError err = NIL;
    AfxAssert(t);
    AfxAssert(m);

    afxTransformFlags flags = t->flags;
    afxBool hasPos = flags & afxTransformFlags_ORIGIN;
    afxBool hasRot = flags & afxTransformFlags_ORIENTATION;
    afxBool hasSs = flags & afxTransformFlags_DEFORM;

    if (hasRot || hasSs)
    {
        afxM3d tmp, tmp2;

        if (hasRot)
            AfxM3dFromQuat(hasSs ? tmp2 : tmp, t->orientation);

        if (hasSs)
        {
            if (hasRot)
                AfxMultiplyM3d(tmp, tmp2, t->scaleShear);
            else
                AfxCopyM3d(tmp, t->scaleShear);
        }

        AfxTransposeM3d(m, tmp);

        if (hasPos)
            AfxCopyV3d(m[3], t->position);
    }
    else
    {
        if (hasPos) AfxCopyV3d(m[3], t->position);
        else
        {
            AfxResetM3d(m);
            AfxZeroV3d(m[3]);
        }
    }
}

_AFXINL void AfxComposeTransformWorldM4d(afxTransform const *t, afxReal const parent[4][4], afxReal w[4][4])
{
    // Should be compatible with void BuildFullWorldPoseOnly_Generic(const transform *Transform, const float *ParentMatrix, float *ResultWorldMatrix)

    afxError err = NIL;
    AfxAssert(t);
    AfxAssert(parent);
    AfxAssert(w);

    afxM4d m;
    AfxComposeTransformM4d(t, m);
    AfxMultiplyTransposedAffineM4d(w, m, parent);
}

_AFXINL void AfxGetTransformWorldAndCompositeMatrix(afxTransform const *t, afxReal const parent[4][4], afxReal const iw[4][4], afxReal composite[4][4], afxReal w[4][4])
{
    // void BuildFullWorldPoseComposite_Generic(const transform *Transform, const float *ParentMatrix, const float *InverseWorld4x4, float *ResultComposite, float *ResultWorldMatrix)

    afxError err = NIL;
    AfxAssert(t);
    AfxAssert(parent);
    AfxAssert(composite);
    AfxAssert(w);

    AfxComposeTransformWorldM4d(t, parent, w);
    AfxMultiplyTransposedAffineM4d(composite, iw, w);
}

////////////////////////////////////////////////////////////////////////////////

_AFXINL void AfxAssimilateTransforms(afxReal const ltm[3][3], afxReal const iltm[3][3], afxReal const atv[4], afxNat cnt, afxTransform const in[], afxTransform out[])
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
        AfxAssimilateAffineV3d(ltm, atv, 1, &in[i].position, &out[i].position);
        AfxAssimilateQuat(ltm, iltm, 1, &in[i].orientation, &out[i].orientation);
        AfxAssimilateM3d(ltm, iltm, 1, &in[i].scaleShear, &out[i].scaleShear);
    }
}

_AFXINL void AfxTransformArrayedPointV3d(afxTransform const* t, afxNat cnt, afxReal const in[][3], afxReal out[][3])
{
    afxError err = NIL;
    AfxAssert(t);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);
    
    AfxPostMultiplyArrayedV3d(t->scaleShear, cnt, in, out);
    AfxRotateV3d(t->orientation, cnt, out, out);

    for (afxNat i = 0; i < cnt; i++)
        AfxAddV3d(out[i], t->position, out[i]);
}

_AFXINL void AfxTransformArrayedNormalV3d(afxTransform const* t, afxNat cnt, afxReal const in[][3], afxReal out[][3])
{
    afxError err = NIL;
    AfxAssert(t);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    AfxPostMultiplyArrayedV3d(t->scaleShear, cnt, in, out);
    AfxRotateV3d(t->orientation, cnt, out, out);
}

_AFXINL void AfxTransformArrayedTransposedNormalV3d(afxTransform const* t, afxNat cnt, afxReal const in[][3], afxReal out[][3])
{
    afxError err = NIL;
    AfxAssert(t);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    afxQuat iq;
    AfxConjugateQuat(iq, t->orientation);
    AfxRotateV3d(iq, cnt, in, out);
    AfxPreMultiplyArrayedV3d(t->scaleShear, cnt, in, out);
}
