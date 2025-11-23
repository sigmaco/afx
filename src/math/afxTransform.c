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

_AFXINL void AfxCopyTransform(afxTransform *t, afxTransform const* in)
{
    afxError err = { 0 };
    AFX_ASSERT(t);
    AFX_ASSERT(in);
    t->flags = in->flags;
    AfxV3dCopy(t->position, in->position);
    AfxQuatCopy(t->orientation, in->orientation);
    AfxM3dCopy(t->scaleShear, in->scaleShear);
}

_AFXINL void AfxResetTransform(afxTransform* t)
{
    afxError err = { 0 };
    AFX_ASSERT(t);
    AfxCopyTransform(t, &AFX_TRANSFORM_IDENTITY);
}

_AFXINL void AfxZeroTransform(afxTransform* t)
{
    afxError err = { 0 };
    AFX_ASSERT(t);
    AfxCopyTransform(t, &AFX_TRANSFORM_ZERO);
}

_AFXINL void AfxSetTransform(afxTransform* t, afxV3d const position, afxQuat const orientation, afxM3d const scaleShear, afxBool check)
{
    afxError err = { 0 };
    AFX_ASSERT(t);
    AFX_ASSERT(orientation);
    AFX_ASSERT(scaleShear);
    t->flags = afxTransformFlag_ALL;
    AfxV3dCopy(t->position, position);
    AfxQuatCopy(t->orientation, orientation);
    AfxM3dCopy(t->scaleShear, scaleShear);

    if (check)
    {
        if (!AfxV3dIsZero(position))
            t->flags |= afxTransformFlag_TRANSLATED;

        if (!AfxQuatIsIdentity(orientation))
            t->flags |= afxTransformFlag_ROTATED;

        if (!AfxM3dIsIdentity(scaleShear))
            t->flags |= afxTransformFlag_DEFORMED;
    }
}

_AFXINL void AfxMakeRigidTransform(afxTransform* t, afxV3d const pos, afxQuat const orient, afxBool check)
{
    afxError err = { 0 };

    if (!(pos && orient))
    {
        AfxResetTransform(t);
        return;
    }
    
    t->flags = NIL;
    AfxM3dCopy(t->scaleShear, AFX_M3D_IDENTITY);

    if (!pos) AfxV3dZero(t->position);
    else
    {
        AfxV3dCopy(t->position, pos);

        if (!check || !AfxV3dIsZero(pos))
            t->flags |= afxTransformFlag_TRANSLATED;
    }

    if (!orient) AfxQuatReset(t->orientation);
    else
    {
        AfxQuatCopy(t->orientation, orient);

        if (!check || !AfxQuatIsIdentity(orient))
            t->flags |= afxTransformFlag_ROTATED;
    }
}

_AFXINL void AfxMultiplyTransform(afxTransform *t, afxTransform const* a, afxTransform const* b)
{
    afxError err = { 0 };
    AFX_ASSERT(t);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
#if 0
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
#endif

    afxM3d am, bm, tmp;
    AfxM3dRotationFromQuat(am, a->orientation);
    AfxM3dRotationFromQuat(bm, b->orientation);
    AfxM3dMultiply(t->scaleShear, bm, b->scaleShear);
    AfxM3dMultiply(tmp, a->scaleShear, t->scaleShear);    
    TransposeMatrixMultiply3x3(t->scaleShear, bm, tmp);
    afxV3d pos;
    AfxV3dPostMultiplyM3d(pos, a->scaleShear, b->position);
    AfxV3dPostMultiplyM3d(t->position, am, pos);
    AfxV3dAdd(t->position, a->position, t->position);
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

_AFXINL void AfxMixTransform(afxTransform *t, afxTransform const* a, afxTransform const* b, afxReal time)
{
    afxError err = { 0 };
    AFX_ASSERT(t);
    AFX_ASSERT(a);
    AFX_ASSERT(b);

    afxTransformFlags flags = b->flags | a->flags;
    t->flags = flags;

    if (!(flags & afxTransformFlag_TRANSLATED)) AfxV3dZero(t->position);
    else
    {
        AfxV3dMix(t->position, a->position, b->position, time);
    }

    if (!(flags & afxTransformFlag_ROTATED)) AfxQuatReset(t->orientation);
    else
    {
        AfxV4dMix(t->orientation, a->orientation, b->orientation, time);
        AfxQuatNormalize(t->orientation, t->orientation);
    }

    if (!(flags & afxTransformFlag_DEFORMED)) AfxM3dReset(t->scaleShear);
    else
    {
        AfxM3dMix(t->scaleShear, a->scaleShear, b->scaleShear, time);
    }
}

_AFXINL void AfxInvertTransform(afxTransform const* in, afxTransform* t)
{
    afxError err = { 0 };
    AFX_ASSERT(t);
    AFX_ASSERT(in);

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

    AfxSetTransform(t, ip3, q, ss, TRUE);
}

_AFXINL afxBool ClipPositionDofs(afxV4d Position, afxUnit32 allowedDOFs)
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

_AFXINL afxBool ClipOrientationDofs(afxQuat Orientation, afxUnit32 allowedDOFs)
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

    afxError err = { 0 };
    AFX_ASSERT(t);
    AFX_ASSERT(allowedDOFs);

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

_AFX void AfxComputeCompositeTransformM4d(afxTransform const* t, afxM4d m)
{
    // Should be compatible with void BuildCompositeTransform4x4

    afxError err = { 0 };
    AFX_ASSERT(t);
    AFX_ASSERT(m);

    afxTransformFlags flags = t->flags;
    afxBool hasPos = flags & afxTransformFlag_TRANSLATED;
    afxBool hasRot = flags & afxTransformFlag_ROTATED;
    afxBool hasSs = flags & afxTransformFlag_DEFORMED;

    if (hasSs)
    {
        afxM3d tmp, tmp2;
        AfxM3dRotationFromQuat(tmp2, t->orientation);
        AfxM3dMultiply(tmp, tmp2, t->scaleShear);
        AfxM4dTransposeM3d(m, tmp, t->position);
    }
    else
    {
        afxM3d tmp;
        AfxM3dRotationFromQuat(tmp, t->orientation);
        AfxM4dTransposeM3d(m, tmp, t->position);
    }
}

_AFX void AfxComputeCompositeTransformM4dc(afxTransform const* t, afxM4r m)
{
    // Should be compatible with void BuildCompositeTransform4x3

    afxError err = { 0 };
    AFX_ASSERT(t);
    AFX_ASSERT(m);

    afxTransformFlags flags = t->flags;
    afxBool hasPos = flags & afxTransformFlag_TRANSLATED;
    afxBool hasRot = flags & afxTransformFlag_ROTATED;
    afxBool hasSs = flags & afxTransformFlag_DEFORMED;

    if (hasSs)
    {
        afxM3d tmp;
        AfxM3dRotationFromQuat(tmp, t->orientation);
        AfxM3dMultiply(m, tmp, t->scaleShear);
        AfxV3dCopy(m[3], t->position);
    }
    else
    {
        AfxM3dRotationFromQuat(m, t->orientation);
        AfxV3dCopy(m[3], t->position);
    }
}

_AFXINL void AfxTransformArrayedAtv3d(afxTransform const* t, afxUnit cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(t);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    AfxM3dPostMultiplyV3d(t->scaleShear, cnt, in, out);
    AfxQuatRotateV3dArray(t->orientation, cnt, out, out);

    for (afxUnit i = 0; i < cnt; i++)
        AfxV3dAdd(out[i], t->position, out[i]);
}

_AFXINL void AfxTransformArrayedLtv3d(afxTransform const* t, afxUnit cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(t);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    // Compatible with TransformVectorInPlace()

    AfxM3dPostMultiplyV3d(t->scaleShear, cnt, in, out);
    AfxQuatRotateV3dArray(t->orientation, cnt, out, out);
}

_AFXINL void AfxTransformArrayedLtv3dTransposed(afxTransform const* t, afxUnit cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(t);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    // Compatible with TransformVectorInPlaceTransposed(in/out, t)

    afxQuat iq;
    AfxQuatConj(iq, t->orientation);
    AfxQuatRotateV3dArray(iq, cnt, in, out);
    AfxM3dPreMultiplyV3d(t->scaleShear, cnt, out, out);
}

void BuildIdentityWorldPoseOnly_Generic(afxM4d const ParentMatrix, afxM4d ResultWorldMatrix)
{
    AfxM4dCopy(ResultWorldMatrix, ParentMatrix);
}

void BuildPositionWorldPoseOnly_Generic(afxV3d const Position, afxM4d const ParentMatrix, afxM4d ResultWorldMatrix)
{
    AfxM4dCopy(ResultWorldMatrix, ParentMatrix);
    AfxV3dPreMultiplyAtm4d(ResultWorldMatrix[3], Position, ParentMatrix);
}

void BuildFullWorldPoseOnly_Generic(afxTransform const* t, afxM4d const ParentMatrix, afxM4d ResultWorldMatrix)
{
    afxM4d m;
    AfxComputeCompositeTransformM4d(t, m);
    AfxM4dMultiplyAffine(ResultWorldMatrix, m, ParentMatrix);
}

void BuildSingleCompositeFromWorldPose_Generic(afxM4d const InverseWorld4x4, afxM4d const WorldMatrix, afxM4d ResultComposite)
{
    AfxM4dMultiplyAffine(ResultComposite, InverseWorld4x4, WorldMatrix);
}

void BuildSingleCompositeFromWorldPoseTranspose_Generic(afxM4d const InverseWorld4x4, afxM4d const WorldMatrix, afxM4r ResultComposite3x4)
{
    //ColumnMatrixMultiply4x3Transpose(ResultComposite3x4, InverseWorld4x4, WorldMatrix);
    // 0 4 8 12
    ResultComposite3x4[0][0] = InverseWorld4x4[0][0] * WorldMatrix[0][0] + InverseWorld4x4[0][1] * WorldMatrix[1][0] + InverseWorld4x4[0][2] * WorldMatrix[2][0];
    ResultComposite3x4[0][1] = InverseWorld4x4[1][0] * WorldMatrix[0][0] + InverseWorld4x4[1][1] * WorldMatrix[1][0] + InverseWorld4x4[1][2] * WorldMatrix[2][0];
    ResultComposite3x4[0][2] = InverseWorld4x4[2][0] * WorldMatrix[0][0] + InverseWorld4x4[2][1] * WorldMatrix[1][0] + InverseWorld4x4[2][2] * WorldMatrix[2][0];

    ResultComposite3x4[1][0] = InverseWorld4x4[3][2] * WorldMatrix[2][0] + InverseWorld4x4[3][1] * WorldMatrix[1][0] + InverseWorld4x4[3][0] * WorldMatrix[0][0] + WorldMatrix[3][0];
    ResultComposite3x4[1][1] = InverseWorld4x4[0][0] * WorldMatrix[0][1] + InverseWorld4x4[0][1] * WorldMatrix[1][1] + InverseWorld4x4[0][2] * WorldMatrix[2][1];
    ResultComposite3x4[1][2] = InverseWorld4x4[1][2] * WorldMatrix[2][1] + InverseWorld4x4[1][0] * WorldMatrix[0][1] + InverseWorld4x4[1][1] * WorldMatrix[1][1];
    
    ResultComposite3x4[2][0] = InverseWorld4x4[2][1] * WorldMatrix[1][1] + InverseWorld4x4[2][2] * WorldMatrix[2][1] + InverseWorld4x4[2][0] * WorldMatrix[0][1];
    ResultComposite3x4[2][1] = InverseWorld4x4[3][1] * WorldMatrix[1][1] + InverseWorld4x4[3][2] * WorldMatrix[2][1] + InverseWorld4x4[3][0] * WorldMatrix[0][1] + WorldMatrix[3][1];
    ResultComposite3x4[2][2] = InverseWorld4x4[0][0] * WorldMatrix[0][2] + InverseWorld4x4[0][1] * WorldMatrix[1][2] + InverseWorld4x4[0][2] * WorldMatrix[2][2];
    
    ResultComposite3x4[3][0] = InverseWorld4x4[1][2] * WorldMatrix[2][2] + InverseWorld4x4[1][0] * WorldMatrix[0][2] + InverseWorld4x4[1][1] * WorldMatrix[1][2];
    ResultComposite3x4[3][1] = InverseWorld4x4[2][1] * WorldMatrix[1][2] + InverseWorld4x4[2][2] * WorldMatrix[2][2] + InverseWorld4x4[2][0] * WorldMatrix[0][2];
    ResultComposite3x4[3][2] = InverseWorld4x4[3][1] * WorldMatrix[1][2] + InverseWorld4x4[3][2] * WorldMatrix[2][2] + InverseWorld4x4[3][0] * WorldMatrix[0][2] + WorldMatrix[3][2];
}

void BuildIdentityWorldPoseComposite_Generic(afxM4d const ParentMatrix, afxM4d const InverseWorld4x4, afxM4d ResultComposite, afxM4d ResultWorldMatrix)
{
    BuildIdentityWorldPoseOnly_Generic(ParentMatrix, ResultWorldMatrix);
    BuildSingleCompositeFromWorldPose_Generic(InverseWorld4x4, ResultWorldMatrix, ResultComposite);
}

void BuildPositionWorldPoseComposite_Generic(afxV3d const Position, afxM4d const ParentMatrix, afxM4d const InverseWorld4x4, afxM4d ResultComposite, afxM4d ResultWorldMatrix)
{
    BuildPositionWorldPoseOnly_Generic(Position, ParentMatrix, ResultWorldMatrix);
    BuildSingleCompositeFromWorldPose_Generic(InverseWorld4x4, ResultWorldMatrix, ResultComposite);
}

void BuildFullWorldPoseComposite_Generic(afxTransform const* t, afxM4d const ParentMatrix, afxM4d const InverseWorld4x4, afxM4d ResultComposite, afxM4d ResultWorldMatrix)
{
    BuildFullWorldPoseOnly_Generic(t, ParentMatrix, ResultWorldMatrix);
    BuildSingleCompositeFromWorldPose_Generic(InverseWorld4x4, ResultWorldMatrix, ResultComposite);
}

void BuildPositionOrientationWorldPoseOnly_Generic(afxV3d const Position, afxQuat const Orientation, afxM4d const ParentMatrix, afxM4d ResultWorldMatrix)
{
    afxTransform t = { 0 };
    t.flags = afxTransformFlag_TRANSLATED | afxTransformFlag_ROTATED;
    AfxQuatCopy(t.orientation, Orientation);
    AfxV3dCopy(t.position, Position);
    BuildFullWorldPoseOnly_Generic(&t, ParentMatrix, ResultWorldMatrix);
}

void BuildPositionOrientationWorldPoseComposite_Generic(afxV3d const Position, afxQuat const Orientation, afxM4d const ParentMatrix, afxM4d const InverseWorld4x4, afxM4d ResultComposite, afxM4d ResultWorldMatrix)
{
    BuildPositionOrientationWorldPoseOnly_Generic(Position, Orientation, ParentMatrix, ResultWorldMatrix);
    BuildSingleCompositeFromWorldPose_Generic(InverseWorld4x4, ResultWorldMatrix, ResultComposite);
}

void BWP_Dispatch(afxTransform const* t, afxM4d const InverseWorld4x4, afxM4d Composite, afxM4d World, afxM4d const ParentWorld)
{
    afxTransformFlags flags = t->flags;

    if (flags & afxTransformFlag_DEFORMED)
    {
        BuildFullWorldPoseComposite_Generic(t, ParentWorld, InverseWorld4x4, Composite, World);
    }
    else if (flags == afxTransformFlag_TRANSLATED)
    {
        BuildPositionWorldPoseComposite_Generic(t->position, ParentWorld, InverseWorld4x4, Composite, World);
    }
    else if (flags)
    {
        BuildPositionOrientationWorldPoseComposite_Generic(t->position, t->orientation, ParentWorld, InverseWorld4x4, Composite, World);
    }
    else
    {
        BuildIdentityWorldPoseComposite_Generic(ParentWorld, InverseWorld4x4, Composite, World);
    }
}

void BWPNC_Dispatch(afxTransform const* t, afxM4d const ParentWorld, afxM4d World)
{
    afxTransformFlags flags = t->flags;

    if (flags & afxTransformFlag_DEFORMED)
    {
        BuildFullWorldPoseOnly_Generic(t, ParentWorld, World);
    }
    else if (flags == afxTransformFlag_TRANSLATED)
    {
        BuildPositionWorldPoseOnly_Generic(t->position, ParentWorld, World);
    }
    else if (flags)
    {
        BuildPositionOrientationWorldPoseOnly_Generic(t->position, t->orientation, ParentWorld, World);
    }
    else
    {
        BuildIdentityWorldPoseOnly_Generic(ParentWorld, World);
    }
}

////////////////////////////////////////////////////////////////////////////////

_AFXINL void AfxAssimilateTransforms(afxM3d const ltm, afxM3d const iltm, afxV4d const atv, afxUnit cnt, afxTransform const in[], afxTransform out[])
{
    afxError err = { 0 };
    AFX_ASSERT(ltm);
    AFX_ASSERT(iltm);
    AFX_ASSERT(atv);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
    {
        AfxAssimilateAtv3d(ltm, atv, 1, &in[i].position, &out[i].position);
        AfxAssimilateQuat(ltm, iltm, 1, &in[i].orientation, &out[i].orientation);
        AfxAssimilateLtm3d(ltm, iltm, 1, &in[i].scaleShear, &out[i].scaleShear);
    }
}
