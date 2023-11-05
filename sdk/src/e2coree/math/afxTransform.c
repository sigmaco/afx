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
    t->flags = NIL | (AfxV4dIsIdentity(position) ? NIL : afxTransformFlags_ORIGIN) | (AfxQuatIsIdentity(orientation) ? NIL : afxTransformFlags_ORIENTATION) | (AfxM3dIsIdentity(scaleShear) ? NIL : afxTransformFlags_SCALESHEAR);
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
    
    AfxPostMultiplyV3d(t->position, a->scaleShear, b->position);

    afxM3d OrientationA;
    AfxM3dFromQuat(OrientationA, a->orientation);
    AfxPostMultiplyV3d(t->position, OrientationA, t->position);
    AfxAddV3d(t->position, a->position, t->position);
    
    AfxMultiplyQuat(t->orientation, a->orientation, b->orientation);
    t->flags = b->flags | a->flags;
}

_AFXINL void AfxPreMultiplyTransform(afxTransform* t, afxTransform const* pre)
{
    afxTransform tmp;
    AfxMultiplyTransform(&tmp, pre, t);
    AfxCopy(t, &tmp, sizeof(t));
}

_AFXINL void AfxPostMultiplyTransform(afxTransform* t, afxTransform const* post)
{
    afxTransform tmp;
    AfxMultiplyTransform(&tmp, t, post);
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

_AFXINL void AfxLerpTransform(afxTransform *t, afxTransform const* a, afxTransform const* b, afxReal time)
{
    afxError err = NIL;
    AfxAssert(t);
    AfxAssert(a);
    AfxAssert(b);

    t->flags = b->flags | a->flags;
    afxReal s = 1.0 - time;

    if (!(t->flags & afxTransformFlags_ORIGIN)) AfxZeroV3d(t->position);
    else
    {
        t->position[0] = s * a->position[0] + time * b->position[0];
        t->position[1] = s * a->position[1] + time * b->position[1];
        t->position[2] = s * a->position[2] + time * b->position[2];
    }

    if (!(t->flags & afxTransformFlags_ORIENTATION)) AfxResetQuat(t->orientation);
    else
    {
        ScaleVectorPlusScaleVector4(t->orientation, s, a->orientation, time, b->orientation);
        AfxNormalizeQuat(t->orientation, t->orientation);
    }

    if (!(t->flags & afxTransformFlags_SCALESHEAR)) AfxResetM3d(t->scaleShear);
    else
    {
        ScaleMatrixPlusScaleMatrix3x3(t->scaleShear, s, a->scaleShear, time, b->scaleShear);
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
    AfxInvertM3d(ss, in->scaleShear);
    AfxMultiplyM3d(tmp, ss, iqm);
    AfxMultiplyTransposedM3d(ss, iqm, tmp);

    afxV4d ip, ip2, ip3;
    AfxNegateV3d(ip, in->position);
    AfxPostMultiplyV3d(ip2, ss, ip);
    AfxPostMultiplyV3d(ip3, iqm, ip2);

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

#if 0
_AFXINL void AfxComposeTransform(afxTransform const* t, afxReal m[9], afxNat stride)
{
    // Should be compatible with  void BuildCompositeTransform(const transform *Transform, int Stride, float *Composite3x3)

    afxError err = NIL;
    AfxAssert(t);
    AfxAssert(m);

    afxM3d tmp;

    if (!(t->flags & afxTransformFlags_SCALESHEAR)) AfxM4dFromQuat(tmp, t->orientation);
    else
    {
        afxM3d tmp2;
        AfxM3dFromQuat(tmp2, t->orientation);
        AfxMultiplyTransposedM3d(tmp, t->scaleShear, tmp2);
    }

    m[0 * stride + 0] = tmp[0][0];
    m[0 * stride + 1] = tmp[0][1];
    m[0 * stride + 2] = tmp[0][2];
    m[1 * stride + 0] = tmp[1][0];
    m[1 * stride + 1] = tmp[1][1];
    m[1 * stride + 2] = tmp[1][2];
    m[2 * stride + 0] = tmp[2][0];
    m[2 * stride + 1] = tmp[2][1];
    m[2 * stride + 2] = tmp[2][2];
}
#endif

_AFX void AfxComposeTransformM4d(afxTransform const* t, afxReal m[4][4])
{
    // Should be compatible with void BuildCompositeTransform4x4(const transform *Transform, float *Composite4x4)     

    afxError err = NIL;
    AfxAssert(t);
    AfxAssert(m);
    afxM4d tmp;

    if (!(t->flags & afxTransformFlags_SCALESHEAR)) AfxM4dFromQuat(tmp, t->orientation);
    else
    {
        afxM3d tmp3, tmp2;
        AfxM3dFromQuat(tmp3, t->orientation);        
        AfxMultiplyM3d(tmp2, tmp3, t->scaleShear); // está certo isso?
        AfxM4dFromM3d(tmp, tmp2, AFX_V4D_0001);
    }
    AfxTransposeM4d(m, tmp);
    AfxCopyV3d(m[3], t->position);
    m[3][3] = 1.0;
}

_AFX void AfxComposeTransformCompactMatrix(afxTransform const* t, afxReal m[4][3])
{
    afxError err = NIL;
    AfxAssert(t);
    AfxAssert(m);
    afxM3d tmp;

    if (!(t->flags & afxTransformFlags_SCALESHEAR)) AfxM3dFromQuat(tmp, t->orientation);
    else
    {
        afxM3d tmp2;
        AfxM3dFromQuat(tmp2, t->orientation);
        AfxMultiplyTransposedM3d(tmp, tmp2, t->scaleShear); // está certo isso?
    }
    AfxTransposeM3d(m, tmp);
    AfxCopyV3d(m[3], t->position);
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

_AFXINL void AfxAssimilateTransforms(afxReal const lt[3][3], afxReal const ilt[3][3], afxReal const at[3], afxNat cnt, afxTransform const in[], afxTransform out[])
{
    afxError err = NIL;
    AfxAssert(lt);
    AfxAssert(ilt);
    AfxAssert(at);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxAssimilatePointV3d(lt, at, 1, &in[i].position, &out[i].position);
        AfxAssimilateQuat(lt, ilt, 1, &in[i].orientation, &out[i].orientation);
        AfxAssimilateM3d(lt, ilt, 1, &in[i].scaleShear, &out[i].scaleShear);
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
