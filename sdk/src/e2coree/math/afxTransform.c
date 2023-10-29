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
#include "afx/math/afxMatrix.h"
#include "afx/math/afxVector.h"
#include "afx/math/afxQuaternion.h"

AFX afxTransform const AFX_TRANSFORM_ZERO;
AFX afxTransform const AFX_TRANSFORM_IDENTITY;

_AFXINL void AfxResetTransform(afxTransform* t)
{
    // Should be compatible with void MakeIdentity(transform *Result)

    afxError err = NIL;
    AfxAssert(t);
    t->flags = NIL;
    AfxZeroV3d(t->position);
    AfxResetQuat(t->orientation);
    AfxResetM3d(t->scaleShear);
}

_AFXINL void AfxZeroTransform(afxTransform* t)
{
    // Should be compatible with void ZeroTransform(transform *Result)

    afxError err = NIL;
    AfxAssert(t);
    t->flags = NIL;
    AfxZeroV3d(t->position);
    AfxZeroQuat(t->orientation);
    AfxZeroM3d(t->scaleShear);
}

_AFXINL void AfxSetTransform(afxTransform* t, afxV3d const position, afxQuat const orientation, afxM3d const scaleShear)
{
    // Should be compatible with void SetTransform(transform *Result, const float *Position3, const float *Orientation4, const float *ScaleShear3x3)

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
    // Should be compatible with void SetTransformWithIdentityCheck(transform *Result, const float *Position3, const float *Orientation4, const float *ScaleShear3x3)

    afxError err = NIL;
    AfxAssert(t);
    AfxAssert(orientation);
    AfxAssert(scaleShear);
    AfxSetTransform(t, position, orientation, scaleShear);
    t->flags = NIL | (AfxV4dIsIdentity(position) ? NIL : afxTransformFlags_ORIGIN) | (AfxQuatIsIdentity(orientation) ? NIL : afxTransformFlags_ORIENTATION) | (AfxM3dIsIdentity(scaleShear) ? NIL : afxTransformFlags_SCALESHEAR);
}

_AFXINL void AfxMultiplyTransform(afxTransform *t, afxTransform const *a, afxTransform const *b)
{
    // Should be compatible with void Multiply(transform *Result, const transform *A, const transform *B)

    afxError err = NIL;
    AfxAssert(t);
    AfxAssert(a);
    AfxAssert(b);

    afxM3d OrientationB, Temp, tmp;
    AfxM3dFromQuat(OrientationB, b->orientation);
    AfxMultiplyM3dTransposed(tmp, OrientationB, b->scaleShear);
    AfxMultiplyM3dTransposed(Temp, a->scaleShear, tmp);
    AfxMultiplyM3dTransposed(t->scaleShear, OrientationB, Temp);

    AfxTransformV3d(t->position, a->scaleShear, b->position);

    afxM3d OrientationA;
    AfxM3dFromQuat(OrientationA, a->orientation);
    AfxTransformV3d(t->position, OrientationA, t->position);
    AfxAddV3d(t->position, a->position, t->position);
    
    AfxMultiplyQuat(t->orientation, a->orientation, b->orientation);
    t->flags = b->flags | a->flags;
}

_AFXINL void AfxPremultiplyTransform(afxTransform* t, afxTransform const* pre)
{
    // void PreMultiplyBy(transform *Transform, const transform *PreMult)

    afxTransform tmp;
    AfxMultiplyTransform(&tmp, pre, t);
    AfxCopy(t, &tmp, sizeof(t));
}

_AFXINL void AfxPostmultiplyTransform(afxTransform* t, afxTransform const* post)
{
    // Should be compatible with void PostMultiplyBy(transform *Transform, const transform *PostMult)
    
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
    // Should be compatible with void LinearBlendTransform(transform *Result, const transform *A, float t, const transform *B)

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
    // Should be compatible with void BuildInverse(transform *Result, consttransform *Source)

    afxError err = NIL;
    AfxAssert(t);
    AfxAssert(in);

    afxQuat q;
    AfxInvertQuat(q, in->orientation);
    
    afxM3d iqm, ss, tmp;
    AfxM3dFromQuat(iqm, q);
    AfxInvertM3d(ss, in->scaleShear);
    AfxMultiplyM3dTransposed(tmp, ss, iqm);
    AfxMultiplyM3dTransposed(ss, iqm, tmp);

    afxV3d ip, ip2, ip3;
    AfxNegateV4d(ip, in->position);
    AfxTransformV3d(ip2, ss, ip);
    AfxTransformV3d(ip3, iqm, ip2);

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

_AFXINL afxReal AfxGetTransformDeterminant(afxTransform const* t)
{
    // Should be compatible with double GetTransformDeterminant(const transform *Transform)
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
        AfxMultiplyM3dTransposed(tmp, t->scaleShear, tmp2);
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

_AFX void AfxGetTransformMatrix(afxTransform const* t, afxReal m[4][4])
{
    // Should be compatible with void BuildCompositeTransform4x4(const transform *Transform, float *Composite4x4)     

    afxError err = NIL;
    AfxAssert(t);
    AfxAssert(m);

    if (!(t->flags & afxTransformFlags_SCALESHEAR)) AfxM4dFromQuat(m, t->orientation);
    else
    {
        afxM3d tmp, tmp2;
        AfxM3dFromQuat(tmp, t->orientation);        
        AfxMultiplyM3dTransposed(tmp2, t->scaleShear, tmp);
        AfxM4dFromM3d(m, tmp2, AFX_V4D_0001);
    }
    
    AfxCopyV3d(m[3], t->position);
    m[3][3] = 1.0;
}

_AFXINL void AfxGetTransformWorldMatrix(afxTransform const *t, afxReal const parent[4][4], afxReal w[4][4])
{
    // Should be compatible with void BuildFullWorldPoseOnly_Generic(const transform *Transform, const float *ParentMatrix, float *ResultWorldMatrix)

    afxError err = NIL;
    AfxAssert(t);
    AfxAssert(parent);
    AfxAssert(w);

    afxM4d m;
    AfxGetTransformMatrix(t, m);
    AfxMultiplyAffineM4dTransposed(w, m, parent);
}

_AFXINL void AfxGetTransformWorldAndCompositeMatrix(afxTransform const *t, afxReal const parent[4][4], afxReal const iw[4][4], afxReal composite[4][4], afxReal w[4][4])
{
    // void BuildFullWorldPoseComposite_Generic(const transform *Transform, const float *ParentMatrix, const float *InverseWorld4x4, float *ResultComposite, float *ResultWorldMatrix)

    afxError err = NIL;
    AfxAssert(t);
    AfxAssert(parent);
    AfxAssert(composite);
    AfxAssert(w);

    AfxGetTransformWorldMatrix(t, parent, w);
    AfxMultiplyAffineM4dTransposed(composite, iw, w);
}

_AFX void AfxGetTransformMatrixCompact(afxTransform const* t, afxReal m[4][3])
{
    // Should be compatible with void BuildCompositeTransform4x3(const transform *Transform, float *Composite4x3)

    afxError err = NIL;
    AfxAssert(t);
    AfxAssert(m);

    if (!(t->flags & afxTransformFlags_SCALESHEAR)) AfxM3dFromQuat(m, t->orientation);
    else
    {
        afxM3d tmp;
        AfxM3dFromQuat(tmp, t->orientation);
        AfxMultiplyM3dTransposed(m, tmp, t->scaleShear);
    }
    AfxCopyV3d(m[3], t->position);
}

_AFXINL void AfxAssimilateTransform(afxTransform* t, afxReal const affine[3], afxReal const linear[3][3], afxReal const invLinear[3][3])
{
    // Should be compatible with InPlaceSimilarityTransform(const float *Affine3, const float *Linear3x3, const float *InverseLinear3x3, float *Position3, float *Orientation4, float *ScaleShear3x3)

    afxError err = NIL;
    AfxAssert(t);
    AfxAssert(affine);
    AfxAssert(linear);
    AfxAssert(invLinear);

    AfxGetAssimilatedPositionV3d(t->position, t->position, affine, linear);
    AfxGetAssimilatedOrientation(t->orientation, t->orientation, linear, invLinear);
    AfxGetAssimilatedScalingM3d(t->scaleShear, t->scaleShear, linear, invLinear);
}

_AFXINL void AfxGetTransformedPointV3d(afxReal v[3], afxReal const in[3], afxTransform const* t)
{
    // Should be compatible with void TransformPoint(float *Dest, const transform *Transform, const float *Source)
    
    afxError err = NIL;
    AfxAssert(v);
    AfxAssert(in);
    AfxAssert(t);

    AfxTransformV3d(v, t->scaleShear, in);
    AfxGetRotatedV3d(v, v, t->orientation);
    AfxAddV3d(v, t->position, v);
}

_AFXINL void AfxGetTransformedNormalV3d(afxReal v[3], afxReal const in[3], afxTransform const* t)
{
    // Should be compatible with void TransformVector(float *Dest, const transform *Transform, const float *Source)

    afxError err = NIL;
    AfxAssert(v);
    AfxAssert(in);
    AfxAssert(t);

    AfxTransformV3d(v, t->scaleShear, in);
    AfxGetRotatedV3d(v, v, t->orientation);
}
