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


#define _AFX_SIM_C
#define _AFX_POSE_C
#define _AFX_SKELETON_C
#include "qwadro/sim/afxSimulation.h"
#include "qwadro/math/afxQuaternion.h"

_AKX afxNat AfxGetPoseCapacity(akxPose const lp)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(lp);
    return lp->artCnt;
}

_AKX afxTransform* AfxGetPoseTransform(akxPose const lp, afxNat artIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(lp);
    AfxAssertRange(lp->artCnt, artIdx, 1);
    return &lp->arts[artIdx].xform;
}

_AKX void AfxCopyPose(akxPose pose, akxPose const from)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(pose);
    AfxAssert(from);

    for (afxNat i = 0; i < from->artCnt; i++)
    {
        pose->arts[i].weight = from->arts[i].weight;
        pose->arts[i].cnt = from->arts[i].cnt;
        pose->arts[i].traversalId = from->arts[i].traversalId;
        AfxCopyTransform(&pose->arts[i].xform, &from->arts[i].xform);
    }
}

_AKX void AfxApplyRootMotionVectorsToPose(akxPose pose, afxV3d const translation, afxV3d const rotation)
{
    afxTransform* t = AfxGetPoseTransform(pose, 0);
    AfxAddV3d(t->position, t->position, translation);

    afxQuat rot;
    AfxMakeQuatFromAngularVelocity(rot, rotation);
    AfxMultiplyQuat(t->orientation, rot, t->orientation);
}

_AKX void AfxAccumulateLocalTransform(akxPose LocalPose, int LocalPoseBoneIndex, int SkeletonBoneIndex, float Weight, const afxSkeleton ReferenceSkeleton, quaternion_mode Mode, const afxTransform *Transform)
{
    double v10; // st7
    double v11; // st6
    int v12; // eax
    afxReal f;

    akxPose v7 = LocalPose;
    akxArticulation* v8 = &LocalPose->arts[LocalPoseBoneIndex];

    if (Mode != 1)
    {
        if (Mode == 2)
        {
            if (AfxSqV4d(ReferenceSkeleton->local[SkeletonBoneIndex].orientation) >= 0.0)
            {
                f = Weight;
                goto LABEL_13;
            }
        }
        else
        {
            if (Mode != 3)
            {
                f = Weight;
                goto LABEL_13;
            }

            if (v8->traversalId == LocalPose->traversalId)
            {
                v10 = v8->xform.orientation[3] * Transform->orientation[3] + v8->xform.orientation[2] * Transform->orientation[2] + v8->xform.orientation[1] * Transform->orientation[1];
                v11 = v8->xform.orientation[0];
            }
            else
            {
                v10 = ReferenceSkeleton->local[SkeletonBoneIndex].orientation[3] * Transform->orientation[3] + ReferenceSkeleton->local[SkeletonBoneIndex].orientation[2] * Transform->orientation[2] + ReferenceSkeleton->local[SkeletonBoneIndex].orientation[1] * Transform->orientation[1];
                v11 = ReferenceSkeleton->local[SkeletonBoneIndex].orientation[0];
            }

            if (v10 + v11 * Transform->orientation[0] >= 0.0)
            {
                f = Weight;
                goto LABEL_13;
            }
        }
    }

    f = -Weight;

LABEL_13:
    if (v8->traversalId == LocalPose->traversalId)
    {
        v8->xform.flags |= Transform->flags;
        v8->xform.position[0] = Weight * Transform->position[0] + v8->xform.position[0];
        v8->xform.position[1] = Weight * Transform->position[1] + v8->xform.position[1];
        v8->xform.position[2] = Weight * Transform->position[2] + v8->xform.position[2];
        v8->xform.orientation[0] = f * Transform->orientation[0] + v8->xform.orientation[0];
        v8->xform.orientation[1] = f * Transform->orientation[1] + v8->xform.orientation[1];
        v8->xform.orientation[2] = f * Transform->orientation[2] + v8->xform.orientation[2];
        v8->xform.orientation[3] = f * Transform->orientation[3] + v8->xform.orientation[3];
        AfxAddScaledM3d(v8->xform.scaleShear, v8->xform.scaleShear, Transform->scaleShear, Weight);
        v12 = v8->cnt + 1;
        v8->weight = Weight + v8->weight;
        v8->cnt = v12;
    }
    else
    {
        if (Weight == 1.0)
        {
            AfxCopyTransform(&v8->xform, Transform);
        }
        else
        {
            v8->xform.flags = Transform->flags;
            v8->xform.position[0] = Weight * Transform->position[0];
            v8->xform.position[1] = Weight * Transform->position[1];
            v8->xform.position[2] = Weight * Transform->position[2];
            AfxScaleM3d(Weight, Transform->scaleShear, v8->xform.scaleShear);
            v7 = LocalPose;
        }

        v8->xform.orientation[0] = f * Transform->orientation[0];
        v8->xform.orientation[1] = f * Transform->orientation[1];
        v8->xform.orientation[2] = f * Transform->orientation[2];
        v8->xform.orientation[3] = f * Transform->orientation[3];
        v8->weight = Weight;
        v8->cnt = 1;
        v8->traversalId = v7->traversalId;
    }
}

_AKX afxError _AkxPoseDtor(akxPose pose)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pose, afxFcc_POSE);

    if (pose->arts)
        AfxDeallocate(pose->arts);

    return err;
}

_AKX afxError _AkxPoseCtor(akxPose pose, afxCookie const *cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pose, afxFcc_POSE);

    afxSimulation sim = cookie->udd[0];
    afxNat artCnt = *(((afxNat const*)(cookie->udd[1])) + cookie->no);
    
    pose->artCnt = artCnt;
    pose->fillThreshold = 0.2;
    pose->traversalId = 0;
    pose->arts = pose->artCnt ? AfxAllocate(pose->artCnt, sizeof(pose->arts[0]), 0, AfxHere()) : NIL;
    AfxAssert(pose->arts);
    AfxZero2(pose->artCnt, sizeof(pose->arts[0]), pose->arts);
    
    return err;
}

_AKX afxClassConfig const _AkxPoseMgrCfg =
{
    .fcc = afxFcc_POSE,
    .name = "Skeletal Pose",
    .fixedSiz = sizeof(AFX_OBJECT(akxPose)),
    .ctor = (void*)_AkxPoseCtor,
    .dtor = (void*)_AkxPoseDtor
};

////////////////////////////////////////////////////////////////////////////////

_AKX afxError AfxAcquirePoses(afxSimulation sim, afxNat cnt, afxNat const artCnt[], akxPose lp[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    afxClass *cls = AfxGetPoseClass(sim);
    AfxAssertClass(cls, afxFcc_POSE);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)lp, (void const*[]) { sim, artCnt }))
        AfxThrowError();

    return err;
}
