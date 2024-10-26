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


#define _AMX_POSE_C
#define _AMX_SKELETON_C
#include "../dev/AmxCadImplKit.h"

_AMX afxUnit AfxGetPoseCapacity(afxPose const lp)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(lp);
    return lp->artCnt;
}

_AMX afxTransform* AfxGetPoseTransform(afxPose const lp, afxUnit artIdx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(lp);
    AFX_ASSERT_RANGE(lp->artCnt, artIdx, 1);
    return &lp->arts[artIdx].xform;
}

_AMX void AfxCopyPose(afxPose pose, afxPose const from)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(pose);
    AFX_ASSERT(from);

    for (afxUnit i = 0; i < from->artCnt; i++)
    {
        pose->arts[i].weight = from->arts[i].weight;
        pose->arts[i].cnt = from->arts[i].cnt;
        pose->arts[i].traversalId = from->arts[i].traversalId;
        AfxCopyTransform(&pose->arts[i].xform, &from->arts[i].xform);
    }
}

_AMX void AfxApplyRootMotionVectorsToPose(afxPose pose, afxV3d const translation, afxV3d const rotation)
{
    afxTransform* t = AfxGetPoseTransform(pose, 0);
    AfxV3dAdd(t->position, t->position, translation);

    afxQuat rot;
    AfxQuatFromAngularVelocity(rot, rotation);
    AfxQuatMultiply(t->orientation, rot, t->orientation);
}

_AMX void AfxAccumulateLocalTransform(afxPose LocalPose, int LocalPoseBoneIndex, int SkeletonBoneIndex, float Weight, const afxSkeleton ReferenceSkeleton, afxQuatBlend Mode, const afxTransform *Transform)
{
    double v10; // st7
    double v11; // st6
    int v12; // eax
    afxReal f;

    afxPose v7 = LocalPose;
    akxArticulation* v8 = &LocalPose->arts[LocalPoseBoneIndex];

    if (Mode != 1)
    {
        if (Mode == 2)
        {
            if (AfxV4dSq(ReferenceSkeleton->local[SkeletonBoneIndex].orientation) >= 0.0)
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
        AfxM3dAddScaled(v8->xform.scaleShear, v8->xform.scaleShear, Transform->scaleShear, Weight);
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
            AfxM3dScale(v8->xform.scaleShear, Transform->scaleShear, Weight);
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

_AMX afxError _AmxPoseDtorCb(afxPose pose)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pose, afxFcc_POSE);

    if (pose->arts)
        AfxDeallocate(pose->arts);

    return err;
}

_AMX afxError _AmxPoseCtorCb(afxPose pose, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pose, afxFcc_POSE);

    afxSimulation sim = args[0];
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxUnit artCnt = *(((afxUnit const*)(args[1])) + invokeNo);
    
    pose->artCnt = artCnt;
    pose->fillThreshold = 0.2;
    pose->traversalId = 0;
    pose->arts = pose->artCnt ? AfxAllocate(pose->artCnt, sizeof(pose->arts[0]), 0, AfxHere()) : NIL;
    AFX_ASSERT(pose->arts);
    AfxZero2(pose->arts, sizeof(pose->arts[0]), pose->artCnt);
    
    return err;
}

_AMX afxClassConfig const _AmxPoseClsCfg =
{
    .fcc = afxFcc_POSE,
    .name = "Pose",
    .desc = "Skeletal Pose",
    .fixedSiz = sizeof(AFX_OBJECT(afxPose)),
    .ctor = (void*)_AmxPoseCtorCb,
    .dtor = (void*)_AmxPoseDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AMX afxError AfxAcquirePoses(afxSimulation sim, afxUnit cnt, afxUnit const artCnt[], afxPose lp[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    afxClass* cls = (afxClass*)AfxGetPoseClass(sim);
    AfxAssertClass(cls, afxFcc_POSE);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)lp, (void const*[]) { sim, artCnt }))
        AfxThrowError();

    return err;
}
