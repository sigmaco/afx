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
#define _AMX_MODEL_C
#define _AMX_ANIMATION_C
#include "../impl/amxImplementation.h"

_AMX afxUnit AfxGetPoseCapacity(afxPose const pose)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(pose);
    return pose->artCnt;
}

_AMX afxTransform* AfxGetPoseTransform(afxPose const pose, afxUnit artIdx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(pose);
    AFX_ASSERT_RANGE(pose->artCnt, artIdx, 1);
    return &pose->arts[artIdx].xform;
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

_AMX void AfxAccumulateLocalTransform(afxPose pose, int LocalAttitudeBoneIndex, int SkeletonBoneIndex, float Weight, const afxModel ReferenceSkeleton, afxQuatBlend Mode, const afxTransform *Transform)
{
    afxError err = NIL;
    double v10; // st7
    afxReal f;

    akxArticulation* pt = &pose->arts[LocalAttitudeBoneIndex];

#if !0
    switch (Mode)
    {
    case afxQuatBlend_DIRECT:
    {
        f = Weight;
        break;
    }
    case afxQuatBlend_INV:
    {
        f = -Weight;
        break;
    }
    case afxQuatBlend_ADJACENT:
    {
        if (AfxV4dSq(ReferenceSkeleton->jntLt[SkeletonBoneIndex].orientation) >= 0.0)
            f = Weight;
        else
            f = -Weight;

        break;
    }
    case afxQuatBlend_ACCUM_ADJACENT:
    {
        if (pt->traversalId == pose->traversalId)
            v10 = AfxQuatDot(pt->xform.orientation, Transform->orientation);
        else
            v10 = AfxQuatDot(ReferenceSkeleton->jntLt[SkeletonBoneIndex].orientation, Transform->orientation);

        if (v10 >= 0.0)
            f = Weight;
        else
            f = -Weight;

        break;
    }
    default: AfxThrowError(); break;
    }
#else

    double v11; // st6

    if (Mode != 1)
    {
        if (Mode == 2)
        {
            if (AfxV4dSq(ReferenceSkeleton->jntLt[SkeletonBoneIndex].orientation) >= 0.0)
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

            if (pt->traversalId == LocalAttitude->traversalId)
            {
                v10 = pt->xform.orientation[3] * Transform->orientation[3] + pt->xform.orientation[2] * Transform->orientation[2] + pt->xform.orientation[1] * Transform->orientation[1];
                v11 = pt->xform.orientation[0];
            }
            else
            {
                v10 = ReferenceSkeleton->jntLt[SkeletonBoneIndex].orientation[3] * Transform->orientation[3] + ReferenceSkeleton->jntLt[SkeletonBoneIndex].orientation[2] * Transform->orientation[2] + ReferenceSkeleton->jntLt[SkeletonBoneIndex].orientation[1] * Transform->orientation[1];
                v11 = ReferenceSkeleton->jntLt[SkeletonBoneIndex].orientation[0];
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
#endif

    if (pt->traversalId == pose->traversalId)
    {
        pt->xform.flags |= Transform->flags;
        AfxV3dMads(pt->xform.position, Weight, Transform->position, pt->xform.position);
        AfxV3dMads(pt->xform.orientation, f, Transform->orientation, pt->xform.orientation);
        AfxM3dMads(pt->xform.scaleShear, Weight, Transform->scaleShear, pt->xform.scaleShear);
        pt->weight += Weight;
        ++pt->cnt;
    }
    else
    {
        if (Weight == 1.0) AfxCopyTransform(&pt->xform, Transform);
        else
        {
            pt->xform.flags = Transform->flags;
            AfxV3dScale(pt->xform.position, Transform->position, Weight);
            AfxM3dScale(pt->xform.scaleShear, Transform->scaleShear, Weight);
        }
        AfxQuatScale(pt->xform.orientation, Transform->orientation, f);
        pt->weight = Weight;
        pt->cnt = 1;
        pt->traversalId = pose->traversalId;
    }
}

_AMX afxUnit AfxPerformManipulatedPose(afxPose pose, afxReal startTime, afxReal duration, afxUnit iterCnt, akxTrackMask* modelMask, afxUnit cnt, afxBody bodies[])
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_POSE, 1, &pose);
    AFX_ASSERT(bodies);
    afxUnit rslt = 0;

    afxSimulation sim = AfxGetProvider(pose);

    amxTrackTarget targets[256];

    for (afxUnit mdlIdx = 0; mdlIdx < cnt; mdlIdx++)
    {
        afxBody bod = bodies[mdlIdx];

        if (!bod) continue;
        AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);

        targets[mdlIdx] = (amxTrackTarget) { 0 };
        targets[mdlIdx].ModelMask = modelMask;
        targets[mdlIdx].OnInstance = bod;
    }

    afxCapstan moto;
    afxCapstanConfig motoCfg = { 0 };
    motoCfg.currClock = startTime;
    motoCfg.localDur = duration;
    motoCfg.iterCnt = iterCnt;

    if (AfxAcquireCapstans(sim, &motoCfg, 1, &moto))
    {
        AfxThrowError();
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_MOTO, 1, &moto);

        amxMotive motives;
        if (AfxAcquireObjects((afxClass*)_AmxGetMotiveClass(sim), cnt, (afxObject*)motives, (void const*[]) { sim, pose, moto, &targets[0] }))
        {
            AfxThrowError();
        }
        AfxDisposeObjects(1, &moto);
    }
    return rslt;
}

_AMX afxError _AmxPoseDtorCb(afxPose pose)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_POSE, 1, &pose);

    if (pose->arts)
        AfxDeallocate((void**)&pose->arts, AfxHere());

    return err;
}

_AMX afxError _AmxPoseCtorCb(afxPose pose, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_POSE, 1, &pose);

    afxSimulation sim = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxUnit artCnt = *(((afxUnit const*)(args[1])) + invokeNo);
    
    pose->artCnt = AfxMax(1, artCnt);
    pose->fillThreshold = 0.2;
    pose->traversalId = 0;
    AfxAllocate(pose->artCnt * sizeof(pose->arts[0]), 0, AfxHere(), (void**)&pose->arts);
    AFX_ASSERT(pose->arts);
    AfxZero2(pose->arts, sizeof(pose->arts[0]), pose->artCnt);
    
    return err;
}

_AMX afxClassConfig const _AMX_POSE_CLASS_CONFIG =
{
    .fcc = afxFcc_POSE,
    .name = "Pose",
    .desc = "Skeletal Pose",
    .fixedSiz = sizeof(AFX_OBJECT(afxPose)),
    .ctor = (void*)_AmxPoseCtorCb,
    .dtor = (void*)_AmxPoseDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AMX afxError AfxAcquirePoses(afxSimulation sim, afxUnit cnt, afxUnit const artCnt[], afxPose pose[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);

    afxClass* cls = (afxClass*)_AmxGetPoseClass(sim);
    AFX_ASSERT_CLASS(cls, afxFcc_POSE);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)pose, (void const*[]) { sim, artCnt }))
        AfxThrowError();

    return err;
}
