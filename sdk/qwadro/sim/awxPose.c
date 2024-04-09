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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */


#define _AFX_SIM_C
#define _AFX_POSE_C
#define _AFX_SKELETON_C
#include "qwadro/sim/afxSimulation.h"
#include "qwadro/math/afxQuaternion.h"

_AKX afxNat AfxGetPoseCapacity(awxPose const *lp)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(lp);
    return lp->artCnt;
}

_AKX afxTransform* AfxGetPoseTransform(awxPose const *lp, afxNat artIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(lp);
    AfxAssertRange(lp->artCnt, artIdx, 1);
    return &lp->arts[artIdx].xform;
}

_AKX void AfxCopyPose(awxPose *pose, awxPose const *from)
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

_AKX void AfxApplyRootMotionVectorsToPose(awxPose *pose, afxV3d const translation, afxV3d const rotation)
{
    afxTransform* t = AfxGetPoseTransform(pose, 0);
    AfxAddV3d(t->position, t->position, translation);

    afxQuat rot;
    AfxMakeQuatFromAngularVelocity(rot, rotation);
    AfxMultiplyQuat(t->orientation, rot, t->orientation);
}

_AKX void AfxAccumulateLocalTransform(awxPose *LocalPose, int LocalPoseBoneIndex, int SkeletonBoneIndex, float Weight, const afxSkeleton ReferenceSkeleton, quaternion_mode Mode, const afxTransform *Transform)
{
    double v10; // st7
    double v11; // st6
    int v12; // eax
    afxReal f;

    awxPose* v7 = LocalPose;
    awxArticulation* v8 = &LocalPose->arts[LocalPoseBoneIndex];

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

_AKX void AfxReleasePoses(afxNat cnt, awxPose *lp[])
{
    afxError err = AFX_ERR_NONE;

    for (afxNat i = 0; i < cnt; i++)
    {
        if (lp[i]->arts)
            AfxDeallocate(lp[i]->arts);

        AfxDeallocate(lp[i]);
    }
}

_AKX afxError AfxAcquirePoses(void *sim, afxNat cnt, afxNat const artCnt[], awxPose *lp[])
{
    afxError err = AFX_ERR_NONE;

    for (afxNat i = 0; i < cnt; i++)
    {
        lp[i] = AfxAllocate(1, sizeof(*lp[0]), 0, AfxHere());
        AfxAssert(lp[i]);
        lp[i]->artCnt = artCnt[i];
        lp[i]->fillThreshold = 0.2;
        lp[i]->traversalId = 0;
        lp[i]->arts = lp[i]->artCnt ? AfxAllocate(lp[i]->artCnt, sizeof(lp[i]->arts[0]), 0, AfxHere()) : NIL;
        AfxAssert(lp[i]->arts);
        AfxZero2(lp[i]->artCnt, sizeof(lp[i]->arts[0]), lp[i]->arts);
    }
    return err;
}
