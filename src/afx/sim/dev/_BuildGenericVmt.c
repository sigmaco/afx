#define _AKX_SKELETON_C
#include "AkxSimDevKit.h"

_AKX void BuildSingleCompositeFromPoseBuffer_Generic(const afxM4d InverseWorld4x4, const afxM4d WorldMatrix, afxM4d ResultComposite)
{
    AfxM4dMultiplyAffine(ResultComposite, InverseWorld4x4, WorldMatrix);
}

_AKX void BuildSingleCompositeFromPoseBufferTranspose_Generic(const afxM4d InverseWorld4x4, const afxM4d WorldMatrix, afxReal ResultComposite3x4[3][4])
{
    afxError err;
    AfxThrowError();
    AfxM4dMultiplyAffine/*Transposed*/(ResultComposite3x4, InverseWorld4x4, WorldMatrix);
}



_AKXINL void BuildPositionPoseBufferOnly_Generic(afxV3d const Position, afxM4d const ParentMatrix, afxM4d ResultWorldMatrix)
{
    AfxM4dCopy(ResultWorldMatrix, ParentMatrix);
    AfxV3dPreMultiplyAtm4d(ResultWorldMatrix[3], Position, ParentMatrix);
}

_AKXINL void BuildPositionPoseBufferComposite_Generic(afxV3d const Position, afxM4d const ParentMatrix, afxM4d const InverseWorld4x4, afxM4d ResultComposite, afxM4d ResultWorldMatrix)
{
    BuildPositionPoseBufferOnly_Generic(Position, ParentMatrix, ResultWorldMatrix);
    BuildSingleCompositeFromPoseBuffer_Generic(InverseWorld4x4, ResultWorldMatrix, ResultComposite);
}

_AKXINL void BuildFullPoseBufferOnly_Generic(afxTransform const* t, const afxM4d ParentMatrix, afxM4d ResultWorldMatrix)
{
    afxM4d t2;
    AfxComputeTransformM4d(t, t2);
    AfxM4dMultiplyAffine(ResultWorldMatrix, t2, ParentMatrix);
}

_AKXINL void BuildPositionOrientationPoseBufferOnly_Generic(afxV3d const Position, afxQuat const Orientation, afxM4d const ParentMatrix, afxM4d ResultWorldMatrix)
{
    afxTransform Transform;
    AfxResetTransform(&Transform);
    AfxV3dCopy(Transform.position, Position);
    AfxQuatCopy(Transform.orientation, Orientation);
    Transform.flags = afxTransformFlags_TRANSLATED | afxTransformFlags_ROTATED;
    BuildFullPoseBufferOnly_Generic(&Transform, ParentMatrix, ResultWorldMatrix);
}

_AKXINL void BuildPositionOrientationPoseBufferComposite_Generic(afxV3d const Position, afxQuat const Orientation, afxM4d const ParentMatrix, afxM4d const InverseWorld4x4, afxM4d ResultComposite, afxM4d ResultWorldMatrix)
{
    BuildPositionOrientationPoseBufferOnly_Generic(Position, Orientation, ParentMatrix, ResultWorldMatrix);
    BuildSingleCompositeFromPoseBuffer_Generic(InverseWorld4x4, ResultWorldMatrix, ResultComposite);
}

_AKXINL void BuildIdentityPoseBufferOnly_Generic(afxM4d const ParentMatrix, afxM4d ResultWorldMatrix)
{
    AfxM4dCopy(ResultWorldMatrix, ParentMatrix);
}

_AKXINL void BuildIdentityPoseBufferComposite_Generic(afxM4d const ParentMatrix, afxM4d const InverseWorld4x4, afxM4d ResultComposite, afxM4d ResultWorldMatrix)
{
    BuildIdentityPoseBufferOnly_Generic(ParentMatrix, ResultWorldMatrix);
    BuildSingleCompositeFromPoseBuffer_Generic(InverseWorld4x4, ResultWorldMatrix, ResultComposite);
}

_AKXINL void BuildFullPoseBufferComposite_Generic(afxTransform const* t, const afxM4d ParentMatrix, const afxM4d InverseWorld4x4, afxM4d ResultComposite, afxM4d ResultWorldMatrix)
{
    BuildFullPoseBufferOnly_Generic(t, ParentMatrix, ResultWorldMatrix);
    BuildSingleCompositeFromPoseBuffer_Generic(InverseWorld4x4, ResultWorldMatrix, ResultComposite);
}

_AKXINL void BWP_Dispatch(afxTransform *t, afxM4d const iw, afxM4d composite, afxM4d w, afxM4d const parentW)
{
    if (t->flags & afxTransformFlags_DEFORMED)
    {
        BuildFullPoseBufferComposite_Generic(t, parentW, iw, composite, w);
    }
    else if (t->flags == afxTransformFlags_TRANSLATED)
    {
        BuildPositionPoseBufferComposite_Generic(t->position, parentW, iw, composite, w);
    }
    else if (t->flags)
    {
        BuildPositionOrientationPoseBufferComposite_Generic(t->position, t->orientation, parentW, iw, composite, w);
    }
    else
    {
        BuildIdentityPoseBufferComposite_Generic(parentW, iw, composite, w);
    }
}

_AKXINL void BWPNC_Dispatch(afxTransform *t, afxM4d const parentW, afxM4d w)
{
    if (t->flags & afxTransformFlags_DEFORMED)
    {
        BuildFullPoseBufferOnly_Generic(t, parentW, w);
    }
    else if (t->flags == afxTransformFlags_TRANSLATED)
    {
        BuildPositionPoseBufferOnly_Generic(t->position, parentW, w);
    }
    else if (t->flags)
    {
        BuildPositionOrientationPoseBufferOnly_Generic(t->position, t->orientation, parentW, w);
    }
    else
    {
        BuildIdentityPoseBufferOnly_Generic(parentW, w);
    }
}

mmt Mmt =
{
    .BuildSingleCompositeFromPoseBuffer_Generic = BuildSingleCompositeFromPoseBuffer_Generic,
    .BuildSingleCompositeFromPoseBufferTranspose_Generic = BuildSingleCompositeFromPoseBufferTranspose_Generic,
    .BuildPositionPoseBufferOnly_Generic = BuildPositionPoseBufferOnly_Generic,
    .BuildPositionPoseBufferComposite_Generic = BuildPositionPoseBufferComposite_Generic,
    .BuildPositionOrientationPoseBufferOnly_Generic = BuildPositionOrientationPoseBufferOnly_Generic,
    .BuildPositionOrientationPoseBufferComposite_Generic = BuildPositionOrientationPoseBufferComposite_Generic,
    .BuildIdentityPoseBufferOnly_Generic = BuildIdentityPoseBufferOnly_Generic,
    .BuildIdentityPoseBufferComposite_Generic = BuildIdentityPoseBufferComposite_Generic,
    .BWP_Dispatch = BWP_Dispatch,
    .BWPNC_Dispatch = BWPNC_Dispatch,
    .BuildFullPoseBufferOnly_Generic = BuildFullPoseBufferOnly_Generic,
    .BuildFullPoseBufferComposite_Generic = BuildFullPoseBufferComposite_Generic,

};
