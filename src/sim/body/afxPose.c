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


#define _ASX_POSE_C
//#define _ASX_SKELETON_C
//#define _ASX_MODEL_C
//#define _ASX_ANIMATION_C
#include "../impl/asxImplementation.h"

_ASX asxArticulation* _AsxPoseGetPaArray(afxPose pose, afxUnit base)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(pose);
    AFX_ASSERT_RANGE(pose->artCnt, base, 1);
    return &pose->arts[base];
}

_ASX afxUnit AsxGetPoseCapacity(afxPose pose)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(pose);
    return pose->artCnt;
}

_ASX afxTransform* AsxGetPoseTransform(afxPose pose, afxUnit artIdx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(pose);
    AFX_ASSERT_RANGE(pose->artCnt, artIdx, 1);
    return &pose->arts[artIdx].xform;
}

_ASX void AsxCopyPose(afxPose pose, afxUnit toBaseArtIdx, afxPose from, afxUnit fromBaseArtIdx, afxUnit cnt, afxUnit const artMap[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(pose);
    AFX_ASSERT_RANGE(pose->artCnt, toBaseArtIdx, cnt);
    AFX_ASSERT(from);
    AFX_ASSERT_RANGE(from->artCnt, fromBaseArtIdx, cnt);

    if (!artMap)
    {
        // non-remapped copy

        for (afxUnit i = 0; i < cnt; i++)
        {
            asxArticulation* dst = &pose->arts[toBaseArtIdx + i];
            asxArticulation const* src = &from->arts[fromBaseArtIdx + i];

            dst->weight = src->weight;
            dst->cnt = src->cnt;
            dst->traversalId = src->traversalId;
            AfxCopyTransform(&dst->xform, &src->xform);
        }
        return;
    }

    // remapped copy

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxUnit artIdx = artMap[i];
        asxArticulation* dst = &pose->arts[toBaseArtIdx + artIdx];
        asxArticulation const* src = &from->arts[fromBaseArtIdx + artIdx];

        dst->weight = src->weight;
        dst->cnt = src->cnt;
        dst->traversalId = src->traversalId;
        AfxCopyTransform(&dst->xform, &src->xform);
    }
}

_ASX void AfxComputeAttachmentWorldMatrix(afxPose pose, afxModel skl, afxUnit jntIdx, afxUnit const sparseJntMap[], afxUnit const sparseJntMapRev[], afxM4d const displace, afxM4d m)
{
    // Should be compatible with void GetWorldMatrixFromLocalAttitude(const skeleton *Skeleton, int BoneIndex, const local_pose *LocalAttitude, const float *Offset4x4, float *Result4x4, const int *SparseBoneArray, const int *SparseBoneArrayReverse)
    // void AfxSklGetWorldMatrixFromLocalAttitude(afxSkeleton skl, afxUnit jointIdx, afxPose const lp, afxM4d const offset, afxM4d m, afxUnit const* sparseBoneArray, afxUnit const* sparseBoneArrayReverse)
    // AfxSklGetWorldMatrixFromLocalAttitude(skl, jntIdx, atti, offset, m, sparseJntMap, sparseJntMapRev);

    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_POSE, 1, &pose);
    AFX_ASSERT_RANGE(pose->artCnt, jntIdx, 1);
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &skl);
    AFX_ASSERT(m);

    afxUnit maxJntCnt = AsxCountJoints(skl, 0);
    afxUnit const* pi = _AsxMdlGetPiArray(skl, 0);
    AfxM4dReset(m);

    for (afxUnit i = jntIdx; i != AFX_INVALID_INDEX; i = pi[i])
    {
        AFX_ASSERT_RANGE(maxJntCnt, i, 1);
        afxTransform* t = AsxGetPoseTransform(pose, sparseJntMapRev ? sparseJntMapRev[i] : i);
        afxM4d tmp, tmp2;
        AfxComputeCompositeTransformM4d(t, tmp);
        AfxM4dMultiplyAffine(tmp2, m, tmp);
        AfxM4dCopy(m, tmp2);
    }

    if (displace)
    {
        afxM4d tmp2;
        AfxM4dMultiplyAffine(tmp2, m, displace);
        AfxM4dCopy(m, tmp2);
    }
}

_ASX void AfxComputeAttachmentOffset(afxPose pose, afxModel skl, afxUnit jntIdx, afxUnit const sparseJntMap[], afxUnit const sparseJntMapRev[], afxM4d const displace, afxM4d m)
{
    // void AfxSklGetSkeletonAttachmentOffset(afxSkeleton skl, afxUnit jntIdx, afxPose const pos, afxM4d const offset, afxM4d m, afxUnit const* sparseArtArray, afxUnit const* sparseArtArrayReverse)
    // AfxSklGetSkeletonAttachmentOffset(skl, jntIdx, atti, offset, m, sparseJntMap, sparseJntMapRev);

    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_POSE, 1, &pose);
    AFX_ASSERT_RANGE(pose->artCnt, jntIdx, 1);
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &skl);
    AFX_ASSERT(m);

    afxM4d tmp;
    AfxComputeAttachmentWorldMatrix(pose, skl, jntIdx, sparseJntMap, sparseJntMapRev, displace, tmp);
    AfxM4dInvertAffine(m, tmp); // invert it
}

_ASX void AfxRebuildRestPose(afxPose pose, afxModel skl, afxUnit baseJntIdx, afxUnit jntCnt)
{
    // Should be compatible with void BuildRestLocalAttitude(const skeleton *Skeleton, int FirstBone, int BoneCount, local_pose *LocalAttitude)
    // AfxSklComputeRestLocalAttitude(skl, baseJntIdx, jntCnt, atti);

    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &skl);

    AFX_ASSERT_RANGE(pose->artCnt, baseJntIdx, jntCnt);

    afxUnit maxJntCnt = AsxCountJoints(skl, 0);
    AFX_ASSERT_RANGE(maxJntCnt, baseJntIdx, jntCnt);

    afxTransform const* lt = _AsxMdlGetLtArray(skl, 0);

    for (afxUnit i = jntCnt; i-- > 0;)
    {
        afxUnit jntIdx = baseJntIdx + i;
        AFX_ASSERT_RANGE(maxJntCnt, jntIdx, 1);
        asxArticulation* pa = &pose->arts[jntIdx];
        pa->cnt = 1;
        pa->weight = 1.0;
        AfxCopyTransform(&pa->xform, &lt[jntIdx]);
    }
}

_ASX void AsxApplyPoseRootMotionVectors(afxPose pose, afxV3d const translation, afxV3d const rotation)
{
    afxTransform* t = AsxGetPoseTransform(pose, 0);
    AfxV3dAdd(t->position, t->position, translation);

    afxQuat rot;
    AfxQuatFromAngularVelocity(rot, rotation);
    AfxQuatMultiply(t->orientation, rot, t->orientation);
}

_ASX void AsxCommencePoseAccumulation(afxPose pose, afxUnit baseArtIdx, afxUnit artCnt, afxUnit const jntMap[])
{
    ++pose->traversalId;
}

_ASX void AsxConcludePoseAccumulation(afxPose pose, afxUnit baseArtIdx, afxUnit artCnt, afxModel skl, afxReal allowedErr, afxUnit const jntMap[])
{
    afxReal AllowedErrorScaler;
    afxReal AllowedErrorEnd;
    afxError err = NIL;

    afxModelInfo mdli;
    AfxDescribeModel(skl, &mdli);

    if (mdli.lodType)
    {
        AsxQueryModelErrorTolerance(skl, allowedErr, &AllowedErrorEnd, &AllowedErrorScaler); // TODO insert allowed error in skeleton due it be skl-dependent?
    }
    else
    {
        allowedErr = 0.0;
        AllowedErrorEnd = 0.0;
        AllowedErrorScaler = 0.0;
    }

    for (afxUnit artIdx = baseArtIdx; artIdx < artCnt; artIdx++)
    {
        afxUnit jointIdx = (jntMap) ? jntMap[artIdx] : artIdx;
        asxArticulation* pa = &pose->arts[artIdx];
        afxTransform const* local = &mdli.jntLt[jointIdx];

        if (pa->traversalId != pose->traversalId)
            pa->xform = *local;
        else
        {
            if (pa->weight < pose->fillThreshold)
            {
                AsxAccumulateLocalTransform(pose, artIdx, jointIdx, pose->fillThreshold - pa->weight, skl, afxQuatBlend_ACCUM_ADJACENT, local);
            }

            afxReal s = (allowedErr - mdli.jntLe[jointIdx]) * AllowedErrorScaler;

            if (s > 0.0)
            {
                if (s >= 0.99000001)
                {
                    pa->weight = 1.0;
                    pa->cnt = 1;
                    pa->xform = mdli.jntLt[jointIdx];
                }
                else
                {
                    AsxAccumulateLocalTransform(pose, artIdx, jointIdx, pa->weight / (1.0 - s) * s, skl, afxQuatBlend_ADJACENT, local);
                }
            }

            afxReal wc = 1.0;

            if (pa->cnt != 1 || pa->weight != wc)
            {
                afxReal s2 = 1.0 / pa->weight;
                AfxV3dScale(pa->xform.position, pa->xform.position, s2);
                AfxM3dScale(pa->xform.scaleShear, pa->xform.scaleShear, s2);
            }

            afxReal sq2, sq = AfxV4dSq(pa->xform.orientation);

            if (sq > 1.1 || sq < 0.89999998)
                sq2 = AfxRsqrtf(sq);
            else
                sq2 = (3.0 - sq) * (12.0 - (3.0 - sq) * (3.0 - sq) * sq) * 0.0625;

            AfxV4dScale(pa->xform.orientation, pa->xform.orientation, sq2);
        }
    }
}

_ASX void AsxAccumulateLocalTransform(afxPose pose, afxUnit artIdx, afxUnit sklJntIdx, afxReal weight, afxModel skl, afxQuatBlend blendOp, afxTransform const* t)
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_POSE, 1, &pose);
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &skl);

    afxReal orientWeight;

    asxArticulation* pa = &pose->arts[artIdx];

    afxTransform sklLt;
    AsxGetJointTransforms(skl, sklJntIdx, 1, &sklLt);

    switch (blendOp)
    {
    case afxQuatBlend_DIRECT: orientWeight = weight; break;
    case afxQuatBlend_INV: orientWeight = -weight; break;
    case afxQuatBlend_ADJACENT: orientWeight = (AfxV4dSq(sklLt.orientation) >= 0.0) ? weight : (-weight); break;
    case afxQuatBlend_ACCUM_ADJACENT:
    {
        afxReal dot;

        if (pa->traversalId == pose->traversalId)
            dot = AfxQuatDot(pa->xform.orientation, t->orientation);
        else
            dot = AfxQuatDot(sklLt.orientation, t->orientation);

        orientWeight = (dot >= 0.0) ? weight : (-weight);
        break;
    }
    default: AfxThrowError(); break;
    }

    if (pa->traversalId == pose->traversalId)
    {
        pa->xform.flags |= t->flags;
        AfxV3dMads(pa->xform.position, weight, t->position, pa->xform.position);
        AfxV3dMads(pa->xform.orientation, orientWeight, t->orientation, pa->xform.orientation);
        AfxM3dMads(pa->xform.scaleShear, weight, t->scaleShear, pa->xform.scaleShear);
        pa->weight += weight;
        ++pa->cnt;
    }
    else
    {
        if (weight == 1.0) AfxCopyTransform(&pa->xform, t);
        else
        {
            pa->xform.flags = t->flags;
            AfxV3dScale(pa->xform.position, t->position, weight);
            AfxM3dScale(pa->xform.scaleShear, t->scaleShear, weight);
        }
        AfxQuatScale(pa->xform.orientation, t->orientation, orientWeight);
        pa->weight = weight;
        pa->cnt = 1;
        pa->traversalId = pose->traversalId;
    }
}

_ASX afxUnit AsxPerformManipulatedPose(afxPose pose, afxReal startTime, afxReal duration, afxUnit iterCnt, akxTrackMask* modelMask, afxUnit cnt, afxBody bodies[])
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_POSE, 1, &pose);
    AFX_ASSERT(bodies);
    afxUnit rslt = 0;

    afxSimulation sim = AfxGetProvider(pose);

    asxTrackTarget targets[256];

    for (afxUnit mdlIdx = 0; mdlIdx < cnt; mdlIdx++)
    {
        afxBody bod = bodies[mdlIdx];

        if (!bod) continue;
        AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);

        targets[mdlIdx] = (asxTrackTarget) { 0 };
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

        asxMotive motives[256];
        if (AfxAcquireObjects((afxClass*)_AsxGetMotiveClass(sim), cnt, (afxObject*)motives, (void const*[]) { sim, pose, moto, &targets[0] }))
        {
            AfxThrowError();
        }
        AfxDisposeObjects(1, &moto);
    }
    return rslt;
}

_ASX afxReal GetTrackMaskBoneWeight(akxTrackMask* mask, afxUnit jntIdx)
{
    double result; // st7

    if (jntIdx >= mask->boneCnt)
        result = mask->defWeight;
    else
        result = mask->boneWeights[jntIdx];
    return result;
}

_ASX void AsxModulatePose(afxPose pose, afxUnit toBaseArtIdx, afxPose composite, afxUnit fromBaseArtIdx, afxUnit artCnt, afxReal weightNone, afxReal weightAll, akxTrackMask* mask, afxUnit const jntRemap[])
{
    // Should be compatible with ModulationCompositeLocalPoseSparse(BasePose, weightNone, wieghtAll, Mask, CompositePose, jntRemap)
    afxError err;

    AFX_ASSERT_RANGE(pose->artCnt, toBaseArtIdx, artCnt);
    AFX_ASSERT_RANGE(composite->artCnt, fromBaseArtIdx, artCnt);
    artCnt = AFX_MIN(AFX_MIN(artCnt, pose->artCnt), composite->artCnt);

    if (!mask) weightNone = weightAll;

    afxReal weightScale = weightAll - weightNone;
    if (!(AFX_ABS(weightScale) >= 0.001))
    {
        mask = 0;
        if (weightNone < 0.001) return;

        if (!(weightNone <= 0.99900001))
        {
            AsxCopyPose(pose, toBaseArtIdx, composite, 0, artCnt, NIL);
            return;
        }
    }

    for (afxUnit i = 0; i < artCnt; i++)
    {
        asxArticulation const* ts = &composite->arts[fromBaseArtIdx + i];
        asxArticulation* td = &pose->arts[toBaseArtIdx + i];

        afxReal weight = weightAll;

        if (mask)
        {
            afxReal f;
            if (jntRemap)
                f = GetTrackMaskBoneWeight(mask, jntRemap[i]);
            else
                f = GetTrackMaskBoneWeight(mask, i);

            weight = f * weightScale + weightNone;
        }

        td->xform.flags = ts->xform.flags | td->xform.flags;

        if (td->xform.flags & afxTransformFlag_TRANSLATED)
        {
            AfxV3dMix(td->xform.position, td->xform.position, ts->xform.position, weight);
        }

        if (td->xform.flags & afxTransformFlag_ROTATED)
        {
            AfxV4dMix(td->xform.orientation, td->xform.orientation, ts->xform.orientation, weight);
            AfxQuatNormalize(td->xform.orientation, td->xform.orientation);
        }

        if (td->xform.flags & afxTransformFlag_DEFORMED)
        {
            AfxM3dMix(td->xform.scaleShear, td->xform.scaleShear, ts->xform.scaleShear, weight);
        }
    }
}

_ASX afxError _AsxPoseDtorCb(afxPose pose)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_POSE, 1, &pose);

    afxObjectStash const stashes[] =
    {
        {
        .cnt = pose->artCnt,
        .siz = sizeof(pose->arts[0]),
        .var = (void**)&pose->arts
        }
    };

    if (AfxDeallocateInstanceData(pose, ARRAY_SIZE(stashes), stashes))
        AfxThrowError();

    return err;
}

_ASX afxError _AsxPoseCtorCb(afxPose pose, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_POSE, 1, &pose);

    afxSimulation sim = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxUnit artCnt = *(((afxUnit const*)(args[1])) + invokeNo);
    
    pose->arts = NIL;

    afxObjectStash const stashes[]=
    {
        {
        .cnt = artCnt,
        .siz = sizeof(pose->arts[0]),
        .var = (void**)&pose->arts
        }
    };
    if (AfxAllocateInstanceData(pose, ARRAY_SIZE(stashes), stashes))
    {
        AfxThrowError();
        return err;
    }

    pose->artCnt = AFX_MAX(1, artCnt);
    pose->fillThreshold = 0.2;
    pose->traversalId = 0;
    AFX_ASSERT(pose->arts);
    
    // need to be zeroed because of demilitarized traversalId comparisons.
    AfxZero(pose->arts, sizeof(pose->arts[0]) * pose->artCnt);

    return err;
}

_ASX afxClassConfig const _ASX_POSE_CLASS_CONFIG =
{
    .fcc = afxFcc_POSE,
    .name = "Pose",
    .desc = "Skeletal Pose",
    .fixedSiz = sizeof(AFX_OBJECT(afxPose)),
    .ctor = (void*)_AsxPoseCtorCb,
    .dtor = (void*)_AsxPoseDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ASX afxError AfxAcquirePoses(afxSimulation sim, afxUnit cnt, afxUnit const artCnt[], afxPose pose[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);

    afxClass* cls = (afxClass*)_AsxGetPoseClass(sim);
    AFX_ASSERT_CLASS(cls, afxFcc_POSE);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)pose, (void const*[]) { sim, artCnt }))
        AfxThrowError();

    return err;
}
