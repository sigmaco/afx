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

#define _AMX_SKELETON_C
//#define _AMX_SIMULATION_C
#define _AMX_POSE_BUFFER_C
#define _AMX_POSE_C
#include "../dev/AmxCadImplKit.h"

_AMXINL afxBool AfxGetSkeletonUrn(afxSkeleton skl, afxString* id)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AFX_ASSERT(id);
    *id = skl->urn;
    return skl->urn.len;
}

_AMXINL afxUnit* AfxGetSkeletonParentIndexes(afxSkeleton skl, afxUnit baseJntIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AFX_ASSERT_RANGE(skl->jointCnt, baseJntIdx, 1);
    return &skl->parentIdx[baseJntIdx];
}

_AMX afxError AfxReparentSkeletonJoints(afxSkeleton skl, afxUnit baseJntIdx, afxUnit cnt, void const* indices, afxUnit stride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AFX_ASSERT_RANGE(skl->jointCnt, baseJntIdx, cnt);
    //AFX_ASSERT(indices);

    afxUnit cnt2 = AfxMin(cnt, skl->jointCnt - baseJntIdx);

    if (!indices) for (afxUnit i = 0; i < cnt2; i++) skl->parentIdx[i] = AFX_INVALID_INDEX;
    else switch (stride)
    {
    case sizeof(afxUnit8):
    {
        for (afxUnit i = 0; i < cnt2; i++)
        {
            afxUnit8 const* src = indices;
            afxUnit parentIdx = src[i];
            AfxAssertOR(parentIdx == AFX_INVALID_INDEX, parentIdx < skl->jointCnt);
            skl->parentIdx[i] = parentIdx;
        }
        break;
    }
    case sizeof(afxUnit16):
    {
        for (afxUnit i = 0; i < cnt2; i++)
        {
            afxUnit16 const* src = indices;
            afxUnit parentIdx = src[i];
            AfxAssertOR(parentIdx == AFX_INVALID_INDEX, parentIdx < skl->jointCnt);
            skl->parentIdx[i] = parentIdx;
        }
        break;
    }
    case sizeof(afxUnit32):
    {
        for (afxUnit i = 0; i < cnt2; i++)
        {
            afxUnit32 const* src = indices;
            afxUnit parentIdx = src[i];
            AfxAssertOR(parentIdx == AFX_INVALID_INDEX, parentIdx < skl->jointCnt);
            skl->parentIdx[i] = parentIdx;
        }
        break;
    }
    default: AfxThrowError(); break;
    }
    return err;
}

_AMXINL afxM4d* AfxGetSkeletonMatrices(afxSkeleton skl, afxUnit baseJntIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AFX_ASSERT_RANGE(skl->jointCnt, baseJntIdx, 1);
    return &skl->iw[baseJntIdx];
}

_AMX afxError AfxResetSkeletonMatrices(afxSkeleton skl, afxUnit baseJntIdx, afxUnit cnt, void const* matrices, afxUnit stride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AFX_ASSERT_RANGE(skl->jointCnt, baseJntIdx, cnt);
    
    afxUnit cnt2 = AfxMin(cnt, skl->jointCnt - baseJntIdx);

    if (!matrices) for (afxUnit i = 0; i < cnt2; i++) AfxM4dReset(skl->iw[i]);
    else switch (stride)
    {
    case 0:
    {
        afxM4d const* src = matrices;

        for (afxUnit i = 0; i < cnt2; i++)
            AfxM4dCopyAtm(skl->iw[i], src[0]);

        break;
    }
    case sizeof(afxM4d):
    {
        afxM4d const* src = matrices;

        for (afxUnit i = 0; i < cnt2; i++)
            AfxM4dCopyAtm(skl->iw[i], src[i]);

        break;
    }
    case sizeof(afxAtm3d):
    {
        afxAtm3d const* src = (afxAtm3d const*)matrices;

        for (afxUnit i = 0; i < cnt2; i++)
        {
            AfxM4dSetAffine(skl->iw[i], src[i][0], src[i][1], src[i][2], src[i][3]);
        }
        break;
    }
    case sizeof(afxM3d):
    {
        afxM3d const* src = (afxM3d const*)matrices;

        for (afxUnit i = 0; i < cnt2; i++)
        {
            AfxM4dCopyM3d(skl->iw[i], src[i], AFX_V4D_IDENTITY);
        }
        break;
    }
    default:
    {
        AfxThrowError();
        break;
    }
    }

    return err;
}

_AMXINL afxTransform* AfxGetSkeletonTransform(afxSkeleton skl, afxUnit jntIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AFX_ASSERT_RANGE(skl->jointCnt, jntIdx, 1);
    return &skl->local[jntIdx];
}

_AMX afxError AfxResetSkeletonTransforms(afxSkeleton skl, afxUnit baseJntIdx, afxUnit cnt, afxTransform const transforms[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AFX_ASSERT_RANGE(skl->jointCnt, baseJntIdx, cnt);
    
    afxUnit cnt2 = AfxMin(cnt, skl->jointCnt - baseJntIdx);

    if (!transforms) for (afxUnit i = 0; i < cnt2; i++) AfxResetTransform(&skl->local[i]);
    else for (afxUnit i = 0; i < cnt2; i++)
        AfxCopyTransform(&skl->local[i], &transforms[i]);

    return err;
}

_AMX afxError AfxResetSkeletonLodErrors(afxSkeleton skl, afxUnit baseJntIdx, afxUnit cnt, afxReal const lodErrors[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AFX_ASSERT_RANGE(skl->jointCnt, baseJntIdx, cnt);
    
    afxUnit cnt2 = AfxMin(cnt, skl->jointCnt - baseJntIdx);

    if (!lodErrors) for (afxUnit i = 0; i < cnt2; i++) skl->lodError[i] = -1.0;
    else for (afxUnit i = 0; i < cnt2; i++)
        skl->lodError[i] = lodErrors[i];

    return err;
}

_AMXINL afxString* AfxGetSkeletonJoints(afxSkeleton skl, afxUnit jointIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AFX_ASSERT_RANGE(skl->jointCnt, jointIdx, 1);
    return &skl->joints[jointIdx];
}

_AMXINL void AfxQuerySkeletonErrorTolerance(afxSkeleton skl, afxReal allowedErr, afxReal* allowedErrEnd, afxReal* allowedErrScaler)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);

    if (allowedErr > 0.0)
    {
        afxReal s = skl->allowedLodErrFadingFactor * allowedErr;
        *allowedErrEnd = s;
        *allowedErrScaler = 1.0 / (allowedErr - s);
    }
    else
    {
        *allowedErrEnd = 0.0;
        *allowedErrScaler = 0.0;
    }
}

_AMX afxUnit AfxCountSkeletonJoints(afxSkeleton skl, afxReal allowedErr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    afxUnit rslt = 0;

    if (allowedErr == 0.0 || !skl->lodType) rslt = skl->jointCnt;
    else
    {
        afxReal errEnd, errScaler;
        AfxQuerySkeletonErrorTolerance(skl, allowedErr, &errEnd, &errScaler);

        afxUnit jointCnt = skl->jointCnt;
        afxReal* lodError = skl->lodError;

        if (jointCnt >= 4)
        {
            while (lodError[rslt + 0] >= errEnd)
            {
                if (lodError[rslt + 1] < errEnd)
                {
                    ++rslt;
                    break;
                }

                if (lodError[rslt + 2] < errEnd)
                {
                    rslt += 2;
                    break;
                }

                if (lodError[rslt + 3] < errEnd)
                {
                    rslt += 3;
                    break;
                }

                rslt += 4;

                if (rslt >= jointCnt - 3)
                    break;
            }
        }

        for (; rslt < jointCnt; rslt++)
            if (lodError[rslt] < errEnd)
                break;
    }
    return rslt;
}

_AMX afxUnit AfxFindSkeletonJoints(afxSkeleton skl, afxUnit cnt, afxString const ids[], afxUnit indices[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AFX_ASSERT(indices);
    AFX_ASSERT(cnt);
    AFX_ASSERT(ids);
    afxUnit rslt = 0;
    afxUnit jointCnt = skl->jointCnt;

    for (afxUnit j = 0; j < cnt; j++)
    {
        afxUnit jointIdx = AFX_INVALID_INDEX;

        for (afxUnit i = 0; i < jointCnt; i++)
        {
            afxString jointTag = *AfxGetSkeletonJoints(skl, i);

            if ((!jointTag.len && !ids[j].len) || (jointTag.len && (0 == AfxCompareStrings(&ids[j], TRUE, 1, &jointTag))))
            {
                jointIdx = i;
                ++rslt;
                break;

            }
        }
        indices[j] = jointIdx;
    }
    return rslt;
}

_AMX void AfxGetSkeletonAttachmentOffset(afxSkeleton skl, afxUnit jntIdx, afxPose const pos, afxM4d const offset, afxM4d m, afxUnit const* sparseArtArray, afxUnit const* sparseArtArrayReverse)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    afxM4d tmp;
    AfxGetWorldMatrixFromLocalPose(skl, jntIdx, pos, offset, tmp, sparseArtArray, sparseArtArrayReverse);
    AfxM4dInvertAffine(m, tmp);
}

_AMX void AfxBuildIndexedCompositeBufferTransposed(afxSkeleton skl, afxPoseBuffer const posb, afxUnit const* indices, afxUnit idxCnt, afxReal buffer[][3][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AFX_ASSERT(skl->jointCnt >= idxCnt);
    AFX_ASSERT(posb);
    AFX_ASSERT(posb->matCnt >= idxCnt);
    AFX_ASSERT(buffer);

    for (afxUnit i = 0; i < idxCnt; i++)
    {
        AFX_ASSERT_RANGE(skl->jointCnt, indices[i], 1);
        AFX_ASSERT_RANGE(posb->matCnt, indices[i], 1);
        BuildSingleCompositeFromWorldPoseTranspose_Generic(skl->iw[indices[i]], posb->world[indices[i]], buffer[i]);
    }
}

_AMX void AfxBuildIndexedCompositeBuffer(afxSkeleton skl, afxPoseBuffer const wp, afxUnit const* indices, afxUnit idxCnt, afxM4d buffer[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AFX_ASSERT(skl->jointCnt >= idxCnt);
    AFX_ASSERT(wp);
    AFX_ASSERT(wp->matCnt >= idxCnt);
    AFX_ASSERT(buffer);

    for (afxUnit i = 0; i < idxCnt; i++)
    {
        AFX_ASSERT_RANGE(skl->jointCnt, indices[i], 1);
        AFX_ASSERT_RANGE(wp->matCnt, indices[i], 1);
        BuildSingleCompositeFromWorldPose_Generic(skl->iw[indices[i]], wp->world[indices[i]], buffer[i]);
    }
}

_AMXINL void AfxComputePoseBufferNoCompositeLod(afxSkeleton skl, afxUnit baseJnt, afxUnit jntCnt, afxUnit firstValidLocalArt, afxUnit validLocalArtCnt, afxPose const pos, afxM4d const offset, afxPoseBuffer posb)
{
    // Should be compatible with void BuildPoseBufferNoCompositeLOD(const skeleton *Skeleton, int FirstBone, int BoneCount, int FirstValidLocalBone, int ValidLocalBoneCount, const granny::local_pose *LocalPose, const float *Offset4x4, world_pose *Result)

    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AFX_ASSERT_RANGE(skl->jointCnt, baseJnt, jntCnt);
    AFX_ASSERT_RANGE(posb->matCnt, baseJnt, jntCnt);
    AFX_ASSERT_RANGE(pos->artCnt, baseJnt, jntCnt);

    if (!offset)
        offset = AFX_M4D_IDENTITY;
    
    afxM4d* w = posb->world;

    for (afxUnit i = 0; i < jntCnt; i++)
    {
        afxUnit jntIdx = baseJnt + i;
        AFX_ASSERT_RANGE(skl->jointCnt, jntIdx, 1);
        afxUnit parentIdx = skl->parentIdx[jntIdx];

        if (parentIdx != AFX_INVALID_INDEX)
        {
            AFX_ASSERT_RANGE(skl->jointCnt, parentIdx, 1);
        }

        afxTransform *t = &pos->arts[jntIdx].xform;

        if (validLocalArtCnt <= 0)
            t = &skl->local[jntIdx];

        BWPNC_Dispatch(t, ((parentIdx == AFX_INVALID_INDEX) ? offset : w[parentIdx]), w[jntIdx]);
        --validLocalArtCnt;
    }
}

_AMXINL void AfxComputePoseBufferLod(afxSkeleton skl, afxUnit baseJnt, afxUnit jntCnt, afxUnit firstValidLocalArt, afxUnit validLocalArtCnt, afxPose const pos, afxM4d const offset, afxPoseBuffer posb)
{
    // Should be compatible with void BuildPoseBufferLOD(const skeleton *Skeleton, int FirstBone, int BoneCount, int FirstValidLocalBone, int ValidLocalBoneCount, const local_pose *LocalPose, const float *Offset4x4, world_pose *Result)

    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AFX_ASSERT_RANGE(skl->jointCnt, baseJnt, jntCnt);
    AFX_ASSERT_RANGE(posb->matCnt, baseJnt, jntCnt);
    AFX_ASSERT_RANGE(pos->artCnt, baseJnt, jntCnt);

    if (posb->composite)
    {
        if (!offset)
            offset = AFX_M4D_IDENTITY;

        afxM4d* iw = skl->iw;
        afxM4d* w = posb->world;
        afxM4d* composite = posb->composite;

        for (afxUnit i = 0; i < jntCnt; i++)
        {
            afxUnit jntIdx = baseJnt + i;
            AFX_ASSERT_RANGE(skl->jointCnt, jntIdx, 1);
            afxUnit parentIdx = skl->parentIdx[jntIdx];

            if (parentIdx != AFX_INVALID_INDEX)
            {
                AFX_ASSERT_RANGE(skl->jointCnt, parentIdx, 1);
            }

            afxTransform *t = &pos->arts[jntIdx].xform;

            if (validLocalArtCnt <= 0)
                t = &skl->local[jntIdx];

            BWP_Dispatch(t, iw[jntIdx], composite[jntIdx], w[jntIdx], ((parentIdx == AFX_INVALID_INDEX) ? offset : w[parentIdx]));
        }
    }
    else
    {
        AfxComputePoseBufferNoCompositeLod(skl, baseJnt, jntCnt, firstValidLocalArt, validLocalArtCnt, pos, offset, posb);
    }
}

_AMX void AfxComputePoseBuffer(afxSkeleton skl, afxUnit baseJnt, afxUnit jntCnt, afxPose const pos, afxM4d const offset, afxPoseBuffer posb)
{
    // Should be compatible with void BuildPoseBuffer(const skeleton *Skeleton, int FirstBone, int BoneCount, const local_pose *LocalPose, const float *Offset4x4, world_pose *Result)

    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AFX_ASSERT_RANGE(pos->artCnt, baseJnt, jntCnt);
    AFX_ASSERT_RANGE(skl->jointCnt, baseJnt, jntCnt);
    AFX_ASSERT_RANGE(posb->matCnt, baseJnt, jntCnt);
    AfxComputePoseBufferLod(skl, baseJnt, jntCnt, baseJnt, jntCnt, pos, offset, posb);
}

_AMX void AfxComputeRestPoseBuffer(afxSkeleton skl, afxUnit baseJnt, afxUnit jntCnt, afxM4d const offset, afxPoseBuffer posb)
{
    // Should be compatible with void BuildRestPoseBuffer(const skeleton *Skeleton, int FirstBone, int BoneCount, const float *Offset4x4, world_pose *Result)

    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AFX_ASSERT_RANGE(skl->jointCnt, baseJnt, jntCnt);
    AFX_ASSERT_RANGE(posb->matCnt, baseJnt, jntCnt);

    if (!offset)
        offset = AFX_M4D_IDENTITY;

    afxM4d* w = posb->world;
    afxM4d* iw = skl->iw;
    afxM4d* composite = posb->composite;
    afxTransform* lt = skl->local;

    for (afxUnit i = 0; i < jntCnt; i++)
    {
        afxUnit jointIdx = baseJnt + i;
        AFX_ASSERT_RANGE(skl->jointCnt, jointIdx, 1);
        afxUnit parentIdx = skl->parentIdx[jointIdx];
        
        if (parentIdx != AFX_INVALID_INDEX)
        {
            AFX_ASSERT_RANGE(skl->jointCnt, parentIdx, 1);
        }

        if (posb->composite)
        {
            BWP_Dispatch(&lt[jointIdx], iw[jointIdx], composite[jointIdx], w[jointIdx], ((AFX_INVALID_INDEX == parentIdx) ? offset : w[parentIdx]));
        }
        else
        {
            BWPNC_Dispatch(&lt[jointIdx], ((AFX_INVALID_INDEX == parentIdx) ? offset : w[parentIdx]), w[jointIdx]);
        }
    }
}

_AMX void AfxComputeRestLocalPose(afxSkeleton skl, afxUnit baseJnt, afxUnit jntCnt, afxPose pos)
{
    // Should be compatible with void BuildRestLocalPose(const skeleton *Skeleton, int FirstBone, int BoneCount, local_pose *LocalPose)

    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AFX_ASSERT_RANGE(pos->artCnt, baseJnt, jntCnt);
    AFX_ASSERT_RANGE(skl->jointCnt, baseJnt, jntCnt);

    for (afxUnit i = jntCnt; i-- > 0;)
    {
        afxUnit jointIdx = baseJnt + i;
        AFX_ASSERT_RANGE(skl->jointCnt, jointIdx, 1);
        akxArticulation* t = &pos->arts[jointIdx];
        t->cnt = 1;
        t->weight = 1.0;
        AfxCopyTransform(&t->xform, &skl->local[jointIdx]);
    }
}

_AMX void AfxLocalPoseFromPoseBuffer(afxSkeleton skl, afxPose pos, afxPoseBuffer const posb, afxM4d const offset, afxUnit firstBone, afxUnit jointCnt)
{
    // Should be compatible with void LocalPoseFromPoseBuffer(const skeleton *Skeleton, local_pose *Result, const world_pose *PoseBuffer, const float *Offset4x4, int FirstBone, int BoneCount)

    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(pos);
    AFX_ASSERT(posb);
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AFX_ASSERT_RANGE(skl->jointCnt, firstBone, jointCnt);
    AFX_ASSERT_RANGE(pos->artCnt, firstBone, jointCnt);
    AFX_ASSERT_RANGE(posb->matCnt, firstBone, jointCnt);

    if (!offset)
        offset = AFX_M4D_IDENTITY;

    for (afxUnit i = jointCnt; i-- > 0;)
    {
        afxUnit jointIdx = firstBone + i;
        AFX_ASSERT_RANGE(skl->jointCnt, jointIdx, 1);
        afxUnit parentIdx = skl->parentIdx[jointIdx];
        afxM4d InverseParent;

        if (parentIdx == AFX_INVALID_INDEX) AfxM4dInvertAffine(InverseParent, offset);
        else
        {
            AFX_ASSERT_RANGE(skl->jointCnt, parentIdx, 1);
            AfxM4dInvertAffine(InverseParent, posb->world[parentIdx]);
        }

        afxM4d Local;
        AfxM4dMultiplyAffine(Local, posb->world[jointIdx], InverseParent);
        
        afxM3d Linear;
        AfxM3dSet(Linear, Local[0], Local[1], Local[2]);

        afxM3d Q, Scale;

        if (!AfxPolarDecomposeM3d(Linear, 0.0000099999997, Q, Scale))
            AfxLogError("Can't accurately decompose MAX transform Q");

        afxV3d Position;
        AfxV3dCopy(Position, Local[3]);
        afxQuat Orientation;
        AfxQuatRotationFromM3d(Orientation, Q);
        AfxQuatNormalize(Orientation, Orientation);
        afxTransform* t = AfxGetPoseTransform(pos, jointIdx);
        AfxSetTransform(t, Position, Orientation, Scale, TRUE);
    }
}

_AMX void AfxLocalPoseFromPoseBufferNoScale(afxSkeleton skl, afxPose lp, afxPoseBuffer const wp, afxM4d const offset, afxUnit firstBone, afxUnit jointCnt)
{
    // Should be compatible with void LocalPoseFromPoseBufferNoScale(const skeleton *Skeleton, local_pose *Result, const world_pose *PoseBuffer, const float *Offset4x4, int FirstBone, int BoneCount)

    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(wp);
    AFX_ASSERT(lp);
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AFX_ASSERT_RANGE(skl->jointCnt, firstBone, jointCnt);
    AFX_ASSERT_RANGE(lp->artCnt, firstBone, jointCnt);
    AFX_ASSERT_RANGE(wp->matCnt, firstBone, jointCnt);

    if (!offset)
        offset = AFX_M4D_IDENTITY;

    for (afxUnit i = jointCnt; i-- > 0;)
    {
        afxUnit jointIdx = firstBone + i;
        afxUnit parentIdx = skl->parentIdx[jointIdx];
        afxM4d InverseParent;

        if (parentIdx == AFX_INVALID_INDEX) AfxM4dInvertAffine(InverseParent, offset);
        else
        {
            AFX_ASSERT_RANGE(skl->jointCnt, parentIdx, 1);
            AfxM4dInvertAffine(InverseParent, wp->world[parentIdx]);
        }

        afxM4d Local;
        AfxM4dMultiplyAffine(Local, wp->world[jointIdx], InverseParent);
        afxTransform* t = AfxGetPoseTransform(lp, jointIdx);

        AfxV3dCopy(t->position, Local[3]);
        afxM3d Rotation;
        AfxM3dSet(Rotation, Local[0], Local[1], Local[2]);
        AfxQuatRotationFromM3d(t->orientation, Rotation);
        AfxQuatNormalize(t->orientation, t->orientation);
        AfxM3dReset(t->scaleShear);
        t->flags = afxTransformFlag_TRANSLATED | afxTransformFlag_ROTATED;
    }
}

_AMX void AfxGetWorldMatrixFromLocalPose(afxSkeleton skl, afxUnit jointIdx, afxPose const lp, afxM4d const offset, afxM4d m, afxUnit const* sparseBoneArray, afxUnit const* sparseBoneArrayReverse)
{
    // Should be compatible with void GetWorldMatrixFromLocalPose(const skeleton *Skeleton, int BoneIndex, const local_pose *LocalPose, const float *Offset4x4, float *Result4x4, const int *SparseBoneArray, const int *SparseBoneArrayReverse)

    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AFX_ASSERT_RANGE(lp->artCnt, jointIdx, 1);

    AfxM4dReset(m);

    for (afxUnit i = jointIdx; i != AFX_INVALID_INDEX; i = skl->parentIdx[i])
    {
        AFX_ASSERT_RANGE(skl->jointCnt, i, 1);
        afxTransform* t = AfxGetPoseTransform(lp, sparseBoneArrayReverse ? sparseBoneArrayReverse[i] : i);
        afxM4d tmp, tmp2;
        AfxComputeCompositeTransformM4d(t, tmp);
        AfxM4dMultiplyAffine(tmp2, m, tmp);
        AfxM4dCopy(m, tmp2);
    }

    if (offset)
    {
        afxM4d tmp2;
        AfxM4dMultiplyAffine(tmp2, m, offset);
        AfxM4dCopy(m, tmp2);
    }
}

#if 0
_AMXINL void _AfxSklCtorBubbleSortOnLod(afxSkeletonBlueprint *Builder, akxSkeletonBone *Bones, afxUnit* remappingTable)
{
    akxSkeletonBone Temp;

    afxArena SortArena;
    AfxDeployArena(NIL, &SortArena, NIL, AfxHere());
    afxUnit *OldToNewMap = AfxAllocateArena(&SortArena, sizeof(afxUnit) * Builder->bones.cnt);
    afxUnit *NewToOldMap = AfxAllocateArena(&SortArena, sizeof(afxUnit) * Builder->bones.cnt);
    afxUnit v9 = 0;
    
    if (Builder->bones.cnt > 0)
    {
        afxUnit *v10 = NewToOldMap;
        do
        {
            *(int *)((char *)v10 + (char *)OldToNewMap - (char *)NewToOldMap) = AFX_INVALID_INDEX;
            *v10 = v9++;
            ++v10;
        } while (v9 < Builder->bones.cnt);
    }
    
    afxByte* v11 = AfxAllocateArena(&SortArena, sizeof(akxSkeletonBone) * Builder->bones.cnt);

    for (afxUnit i = Builder->bones.cnt; i--> 0;)
    {
        Bones[i].name;
    }

    akxSkeletonBone* v12 = Bones;

    if (152 * Builder->bones.cnt)
    {
        afxUnit v13 = v11 - (afxByte *)Bones;
        afxUnit v14 = 152 * Builder->bones.cnt;

        do
        {
            *((afxByte *)&v12->name + v13) = v12->name;
            v12 = (akxSkeletonBone *)((char *)v12 + 1);
            --v14;
        } while (v14);
    }

    afxUnit v15 = Builder->bones.cnt - 1;

    if (v15 >= 0)
    {
        afxUnit v35 = Builder->bones.cnt;

        do
        {
            afxUnit v16 = 0;
            afxUnit v17 = v15 - 1;
            afxUnit v32 = v15 - 1;

            if (v15 >= 4)
            {
                akxSkeletonBone *v18 = Bones + 1;
                afxUnit32 v34 = (unsigned int)v15 >> 2;
                afxUnit *v19 = NewToOldMap + 2;
                afxUnit j = 4 * ((unsigned int)v15 >> 2);

                do
                {
                    if (v18[-1].lodError < (double)v18->lodError)
                    {
                        afxUnit v20 = *(v19 - 2);
                        *(v19 - 2) = *(v19 - 1);
                        *(v19 - 1) = v20;
                        memcpy(&Temp, &v18[-1], sizeof(Temp));
                        memcpy(&v18[-1], v18, sizeof(akxSkeletonBone));
                        memcpy(v18, &Temp, sizeof(akxSkeletonBone));
                    }

                    if (v18->lodError < (double)v18[1].lodError)
                    {
                        afxUnit v21 = *(v19 - 1);
                        *(v19 - 1) = *v19;
                        *v19 = v21;
                        memcpy(&Temp, v18, sizeof(Temp));
                        memcpy(v18, &v18[1], sizeof(akxSkeletonBone));
                        memcpy(&v18[1], &Temp, sizeof(akxSkeletonBone));
                    }

                    if (v18[1].lodError < (double)v18[2].lodError)
                    {
                        afxUnit v22 = *v19;
                        *v19 = v19[1];
                        v19[1] = v22;
                        memcpy(&Temp, &v18[1], sizeof(Temp));
                        memcpy(&v18[1], &v18[2], sizeof(akxSkeletonBone));
                        memcpy(&v18[2], &Temp, sizeof(akxSkeletonBone));
                    }

                    if (v18[2].lodError < (double)v18[3].lodError)
                    {
                        afxUnit v23 = v19[1];
                        v19[1] = v19[2];
                        v19[2] = v23;
                        memcpy(&Temp, &v18[2], sizeof(Temp));
                        memcpy(&v18[2], &v18[3], sizeof(akxSkeletonBone));
                        memcpy(&v18[3], &Temp, sizeof(akxSkeletonBone));
                    }
                    v19 += 4;
                    v18 += 4;
                    --v34;
                } while (v34);

                v16 = j;
                v17 = v32;
            }

            if (v16 <= v17)
            {
                akxSkeletonBone *v24 = &Bones[v16 + 1];

                do
                {
                    if (v24[-1].lodError < (double)v24->lodError)
                    {
                        afxUnit v25 = NewToOldMap[v16];
                        NewToOldMap[v16] = NewToOldMap[v16 + 1];
                        NewToOldMap[v16 + 1] = v25;
                        memcpy(&Temp, &v24[-1], sizeof(Temp));
                        memcpy(&v24[-1], v24, sizeof(akxSkeletonBone));
                        memcpy(v24, &Temp, sizeof(akxSkeletonBone));
                    }
                    v17 = v32;
                    ++v16;
                    ++v24;
                } while (v16 <= v32);

            }
            v15 = v17;
            --v35;
        } while (v35);
    }

    for (afxUnit i = 0; i < Builder->bones.cnt; ++i)
        OldToNewMap[NewToOldMap[i]] = i;

    if (remappingTable)
    {
        for (afxUnit k = 0; k < Builder->bones.cnt; ++k)
            remappingTable[k] = OldToNewMap[remappingTable[k]];
    }

    for (afxUnit i = 0; i < Builder->bones.cnt; ++i)
    {
        if (AFX_INVALID_INDEX != Bones[i].parentIdx)
            Bones[i].parentIdx = OldToNewMap[Bones[i].parentIdx];
    }

    AfxDismantleArena(&SortArena);
}
#endif

#if 0
_AMXINL void _AkxSklCtorWriteBone(akxSkeletonBuilder const* sklb, afxSkeleton skl, afxUnit* boneWrittenMap, afxUnit inBoneIdx, afxUnit* outBoneIdx)
{
    afxError err = NIL;    
    afxUnit idx = boneWrittenMap[inBoneIdx];

    if (idx == (afxUnit)-2)
    {
        AfxLogError("Circular parenting chain detected at bone %d", inBoneIdx);
        AfxThrowError();
    }
    else if (idx == AFX_INVALID_INDEX)
    {
        boneWrittenMap[inBoneIdx] = (afxUnit)-2; // temp

        afxUnit parentIdx = sklb->bones[inBoneIdx].parentIdx;
        afxUnit parentIdxWritten = AFX_INVALID_INDEX;

        if (parentIdx != AFX_INVALID_INDEX)
        {
            AFX_ASSERT_RANGE(skl->jointCnt, parentIdx, 1);

            _AkxSklCtorWriteBone(sklb, skl, boneWrittenMap, parentIdx, outBoneIdx);
            parentIdxWritten = boneWrittenMap[parentIdx];
        }

        if (skl->parentIdx)
            skl->parentIdx[*outBoneIdx] = parentIdxWritten;

        if (skl->local)
            AfxCopyTransform(&skl->local[*outBoneIdx], &sklb->bones[inBoneIdx].local);

        if (skl->iw)
            AfxM4dCopy(skl->iw[*outBoneIdx], sklb->bones[inBoneIdx].iw);

        if (skl->lodError)
            skl->lodError[*outBoneIdx] = sklb->bones[inBoneIdx].lodError;

        if (skl->joint)
            if (!AfxCatalogStrings2(skl->strb, 1, &sklb->bones[inBoneIdx].id.str, &skl->joint[*outBoneIdx]))
                AfxThrowError();

        if (skl->udd)
            skl->udd[*outBoneIdx] = sklb->bones[inBoneIdx].extendedData;

        //if (sklb->IndexRemapping)
            //sklb->IndexRemapping(data, inBoneIdx, *outBoneIdx);

        boneWrittenMap[inBoneIdx] = (*outBoneIdx)++;
    }
}
#endif

_AMXINL void _AkxSklWriteBone(afxSkeleton skl, afxUnit const* draftParents, afxTransform const* draftLocals, afxM4d const* draftIws, afxReal const* draftErrors, afxString const* draftIds, void** draftUdds, afxUnit* boneWrittenMap, afxUnit inBoneIdx, afxUnit* outBoneIdx)
{
    afxError err = NIL;
    afxUnit idx = boneWrittenMap[inBoneIdx];

    if (idx == (afxUnit)-2)
    {
        AfxLogError("Circular parenting chain detected at bone %d", inBoneIdx);
        AfxThrowError();
    }
    else if (idx == AFX_INVALID_INDEX)
    {
        boneWrittenMap[inBoneIdx] = (afxUnit)-2; // temp

        afxUnit parentIdx = draftParents[inBoneIdx];
        afxUnit parentIdxWritten = AFX_INVALID_INDEX;

        if (parentIdx != AFX_INVALID_INDEX)
        {
            AFX_ASSERT_RANGE(skl->jointCnt, parentIdx, 1);

            _AkxSklWriteBone(skl, draftParents, draftLocals, draftIws, draftErrors, draftIds, draftUdds, boneWrittenMap, parentIdx, outBoneIdx);
            parentIdxWritten = boneWrittenMap[parentIdx];
        }

        if (skl->parentIdx)
            skl->parentIdx[*outBoneIdx] = parentIdxWritten;

        if (skl->local)
            AfxCopyTransform(&skl->local[*outBoneIdx], &draftLocals[inBoneIdx]);

        if (skl->iw)
            AfxM4dCopy(skl->iw[*outBoneIdx], draftIws[inBoneIdx]);

        if (skl->lodError)
            skl->lodError[*outBoneIdx] = draftErrors[inBoneIdx];

        if (skl->joints)
            if (!AfxCatalogStrings(AfxGetPivotTagStringBase(AfxGetSimulationDrawInput(AfxGetProvider(skl))), 1, &draftIds[inBoneIdx], &skl->joints[*outBoneIdx]))
                AfxThrowError();

        if (skl->udd)
            skl->udd[*outBoneIdx] = draftUdds[inBoneIdx];

        //if (sklb->IndexRemapping)
            //sklb->IndexRemapping(data, inBoneIdx, *outBoneIdx);

        boneWrittenMap[inBoneIdx] = (*outBoneIdx)++;
    }
}

_AMX afxError _AkxSklRewriteBones(afxSkeleton skl, afxUnit const* draftParents, afxTransform const* draftLocals, afxM4d const* draftIws, afxReal const* draftErrors, afxString const* draftIds, void** draftUdds)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);

    // estimate lod

    afxUnit *boneWrittenMap = AfxAllocate(skl->jointCnt, sizeof(boneWrittenMap[0]), 0, AfxHere());
    AFX_ASSERT(boneWrittenMap);

    for (afxUnit i = 0; i < skl->jointCnt; ++i)
        boneWrittenMap[i] = (afxUnit)AFX_INVALID_INDEX;

    afxUnit outBoneIdx = 0;

    for (afxUnit i = 0; i < skl->jointCnt; i++)
    {
        if (AFX_INVALID_INDEX == draftParents[i])
            _AkxSklWriteBone(skl, draftParents, draftLocals, draftIws, draftErrors, draftIds, draftUdds, boneWrittenMap, i, &outBoneIdx);
    }

    for (afxUnit i = 0; i < skl->jointCnt; i++)
    {
        if (AFX_INVALID_INDEX != draftParents[i])
            _AkxSklWriteBone(skl, draftParents, draftLocals, draftIws, draftErrors, draftIds, draftUdds, boneWrittenMap, i, &outBoneIdx);
    }

    //_AfxSklCtorBubbleSortOnLOD(blueprint, skl->bones, remapTable);

    AfxDeallocate(boneWrittenMap);
    return err;
}

_AMX afxError _AmxSklCtorCb(afxSkeleton skl, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);

    afxSimulation sim = args[0];
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxSkeletonBlueprint const* bluep = args[1];
    //AFX_ASSERT(bluep);
    
    if (bluep) bluep += invokeNo;
    else
    {
        static afxSkeletonBlueprint const staticSklb = { 0 };
        bluep = &staticSklb;
    }

    skl->allowedLodErrFadingFactor = /*_AkxGetAllowedLodErrorFadingFactor(sim);*/ 0.80000001;

    if (!AfxCatalogStrings(AfxGetModelUrnStringBase(sim), 1, &bluep->urn.str, &skl->urn)) AfxThrowError();
    else
    {
        afxUnit jointCnt = AfxMax(1, bluep->jointCnt);
        afxUnit lodType = bluep->lodType;

        skl->jointCnt = jointCnt;
        skl->lodType = lodType;

        afxObjectStash stashes[] =
        {
            {
                .cnt = skl->jointCnt,
                .siz = sizeof(skl->joints[0]),
                .var = (void**)&skl->joints
            },
            {
                .cnt = skl->jointCnt,
                .siz = sizeof(skl->lodError[0]),
                .align = AFX_SIMD_ALIGNMENT,
                .var = (void**)&skl->lodError
            },
            {
                .cnt = skl->jointCnt,
                .siz = sizeof(skl->iw[0]),
                .align = AFX_SIMD_ALIGNMENT,
                .var = (void**)&skl->iw
            },
            {
                .cnt = skl->jointCnt,
                .siz = sizeof(skl->local[0]),
                .align = AFX_SIMD_ALIGNMENT,
                .var = (void**)&skl->local
            },
            {
                .cnt = skl->jointCnt,
                .siz = sizeof(skl->parentIdx[0]),
                .var = (void**)&skl->parentIdx
            }
        };

        if (AfxAllocateInstanceData(skl, ARRAY_SIZE(stashes), stashes)) AfxThrowError();
        else
        {
            AFX_ASSERT(skl->parentIdx);
            AFX_ASSERT(skl->lodError);
            AFX_ASSERT(skl->joints);
            AFX_ASSERT(skl->local);
            AFX_ASSERT(skl->iw);

            if (!bluep->joints) for (afxUnit i = 0; i < jointCnt; i++)
                AfxResetStrings(1, &skl->joints[i]);
            else
            {
                if (!AfxCatalogStrings(AfxGetPivotTagStringBase(AfxGetSimulationDrawInput(sim)), jointCnt, bluep->joints, skl->joints))
                    AfxThrowError();
            }

            if (!err)
            {
                skl->udd = NIL;

                AfxReparentSkeletonJoints(skl, 0, jointCnt, NIL, 0);
                AfxResetSkeletonMatrices(skl, 0, jointCnt, NIL, 0);
                AfxResetSkeletonTransforms(skl, 0, jointCnt, NIL);
                AfxResetSkeletonLodErrors(skl, 0, jointCnt, NIL);

                //skl->jointCnt = jointCnt;
                AFX_ASSERT(skl->jointCnt == jointCnt);

                afxString s;
                AfxGetSkeletonUrn(skl, &s);

                AfxLogEcho("Skeleton <%.*s> assembled at %p.\n    Listing %u joints..:", AfxPushString(&s), skl, skl->jointCnt);

                for (afxUnit i = 0; i < skl->jointCnt; i++)
                {
                    s = skl->joints[i];
                    s = *AfxGetSkeletonJoints(skl, i);
                    AfxLogEcho("    %3u <%.*s> %3u", i, AfxPushString(&s), skl->parentIdx[i]);
                }
            }

            if (err)
                AfxDeallocateInstanceData(skl, ARRAY_SIZE(stashes), stashes);
        }
    }
    return err;
}

_AMX afxError _AmxSklDtorCb(afxSkeleton skl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);

    afxObjectStash stashes[] =
    {
        {
            .cnt = skl->jointCnt,
            .siz = sizeof(skl->joints[0]),
            .var = (void**)&skl->joints
        },
        {
            .cnt = skl->jointCnt,
            .siz = sizeof(skl->lodError[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&skl->lodError
        },
        {
            .cnt = skl->jointCnt,
            .siz = sizeof(skl->iw[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&skl->iw
        },
        {
            .cnt = skl->jointCnt,
            .siz = sizeof(skl->local[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&skl->local
        },
        {
            .cnt = skl->jointCnt,
            .siz = sizeof(skl->parentIdx[0]),
            .var = (void**)&skl->parentIdx
        }
    };

    if (AfxDeallocateInstanceData(skl, ARRAY_SIZE(stashes), stashes))
        AfxThrowError();

    AFX_ASSERT(!skl->parentIdx);
    AFX_ASSERT(!skl->lodError);
    AFX_ASSERT(!skl->joints);
    AFX_ASSERT(!skl->local);
    AFX_ASSERT(!skl->iw);

    return err;
}

_AMX afxClassConfig _AmxSklClsCfg =
{
    .fcc = afxFcc_SKL,
    .name = "Skeleton",
    .desc = "Transformation Skeleton",
    .fixedSiz = sizeof(AFX_OBJECT(afxSkeleton)),
    .ctor = (void*)_AmxSklCtorCb,
    .dtor = (void*)_AmxSklDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AMX void AfxTransformSkeletons(afxM3d const ltm, afxM3d const iltm, afxReal linearTol, afxV4d const atv, afxReal affineTol, afxUnit cnt, afxSkeleton skeletons[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(ltm);
    AFX_ASSERT(iltm);
    AFX_ASSERT(atv);
    AFX_ASSERT(cnt);
    AFX_ASSERT(skeletons);
    (void)affineTol;
    (void)linearTol;

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxSkeleton skl = skeletons[i];

        if (!skl)
            continue;

        AfxAssertObjects(1, &skl, afxFcc_SKL);
        afxUnit jointCnt = skl->jointCnt;

        AfxAssimilateTransforms(ltm, iltm, atv, jointCnt, skl->local, skl->local);
        AfxAssimilateAtm4d(ltm, iltm, atv, jointCnt, skl->iw, skl->iw);
    }
}

_AMX afxError AfxAssembleSkeletons(afxSimulation sim, afxUnit cnt, afxSkeletonBlueprint const sklb[], afxSkeleton skeletons[])
{
    //AfxEntry("sim=%p,name=%.*s,jointCnt=%u,boneName=%p,boneParentIdx=%p,bonePos=%p,boneOrient=%p,boneScale=%p", sim, AfxPushString(name), jointCnt, boneName, boneParentIdx, bonePos, boneOrient, boneScale);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AFX_ASSERT(skeletons);
    AFX_ASSERT(sklb);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)AfxGetSkeletonClass(sim);
    AfxAssertClass(cls, afxFcc_SKL);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)skeletons, (void const*[]) { sim, (void*)sklb, FALSE }))
        AfxThrowError();

    return err;
}
