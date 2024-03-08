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
#define _AFX_SKELETON_C
#define _AFX_SIMULATION_C
#include "qwadro/sim/afxSimulation.h"
#include "qwadro/math/afxQuaternion.h"
#include "qwadro/math/afxVector.h"
#include "qwadro/math/afxMatrix.h"
#include "qwadro/sim/awxAsset.h"
#include "qwadro/core/afxIndexedString.h"

_AKX afxNat AfxCountSkeletonJoints(afxSkeleton skl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    return skl->jointCnt;
}

_AKX afxM4d* AfxGetSkeletonIwm(afxSkeleton skl, afxNat jointIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AfxAssertRange(skl->jointCnt, jointIdx, 1);
    return &skl->iw[jointIdx];
}

_AKXINL afxBool AfxGetSkeletonId(afxSkeleton skl, afxString* id)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AfxAssert(id);
    return AfxResolveStrings2(skl->strc, 1, &skl->id, id);
}

_AKX afxBool AfxGetSkeletonJointTag(afxSkeleton skl, afxNat jointIdx, afxString* id)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AfxAssertRange(skl->jointCnt, jointIdx, 1);
    AfxAssert(id);
    return AfxResolveStrings2(skl->strc, 1, &skl->label[jointIdx], id);
}

_AKXINL void AfxQuerySkeletonErrorTolerance(afxSkeleton skl, afxReal allowedErr, afxReal* allowedErrEnd, afxReal* allowedErrScaler)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);

    if (allowedErr > 0.0)
    {
        afxReal s = skl->allowedLodErrFadingFactor * allowedErr;
        allowedErrEnd[0] = s;
        allowedErrScaler[0] = 1.0 / (allowedErr - s);
    }
    else
    {
        allowedErrEnd[0] = 0.0;
        allowedErrScaler[0] = 0.0;
    }
}

_AKX afxNat AfxCountSkeletonJointsForLod(afxSkeleton skl, afxReal allowedErr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    afxNat rslt = 0;

    if (allowedErr == 0.0 || !skl->lodType) rslt = skl->jointCnt;
    else
    {
        afxReal errEnd, errScaler;
        AfxQuerySkeletonErrorTolerance(skl, allowedErr, &errEnd, &errScaler);

        afxNat jointCnt = skl->jointCnt;
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
        {
            if (lodError[rslt] < errEnd)
                break;
        }
    }
    return rslt;
}

_AKX afxNat AfxFindSkeletonJoints(afxSkeleton skl, afxStringCatalog strc, afxNat cnt, afxString const ids[], afxNat indices[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AfxAssert(indices);
    AfxAssert(cnt);
    AfxAssert(ids);
    afxNat rslt = 0;
    afxNat jointCnt = skl->jointCnt;

    for (afxNat j = 0; j < cnt; j++)
    {
        afxNat jointIdx = AFX_INVALID_INDEX;
        afxString target;

        if (!strc)
            target = ids[j];
        else
            AfxResolveStrings2(strc, 1, &ids[j], &target);

        for (afxNat i = 0; i < jointCnt; i++)
        {
            afxString jointTag;

            if (AfxGetSkeletonJointTag(skl, i, &jointTag) && (0 == AfxCompareStringCi(&target, &jointTag)))
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

_AKX afxBool AfxFindSkeletonJoint(afxSkeleton skl, afxString const* id, afxNat *jointIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    afxNat rslt = AfxFindSkeletonJoints(skl, NIL, 1, id, jointIdx);
    AfxAssert(rslt == 1);
    return rslt;
}

_AKX void AfxGetSkeletonAttachmentOffset(afxSkeleton skl, afxNat baseJoint, awxPose const* lp, afxM4d const offset, afxM4d m, afxNat const* sparseArtArray, afxNat const* sparseArtArrayReverse)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    afxM4d tmp;
    AfxGetWorldMatrixFromLocalPose(skl, baseJoint, lp, offset, tmp, sparseArtArray, sparseArtArrayReverse);
    AfxInvertAtm4d(tmp, m);
}

// Amen

_AKX afxError AfxDeserializeSkeletons(afxStream in, afxSimulation sim, afxNat cnt, afxSkeleton dst[])
{
    afxError err = NIL;
    AfxAssertObjects(1, &in, afxFcc_IOS);
    AfxAssert(dst);
    AfxAssert(cnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        _afxSerializedChunk chnkHdr;
        AfxRead(in, 1, sizeof(chnkHdr), &chnkHdr, sizeof(chnkHdr));
        AfxAssertType(&chnkHdr, afxFcc_SKL);

        _afxSerializedSkl chnk = { 0 };
        AfxRead(in, 1, sizeof(chnk), &chnk, sizeof(chnk));
        afxNat jointCnt = chnk.jointCnt;

        afxSkeletonBuilder sklb;
        AfxBeginSkeletonBuilding(&sklb, jointCnt, &chnk.name, chnk.lodType);
        
        AfxRead(in, jointCnt, sizeof(sklb.bones[0].local), &sklb.bones[0].local, sizeof(sklb.bones[0]));

        AfxRead(in, jointCnt, sizeof(sklb.bones[0].iw), &sklb.bones[0].iw, sizeof(sklb.bones[0]));

        AfxRead(in, jointCnt, sizeof(sklb.bones[0].parentIdx), &sklb.bones[0].parentIdx, sizeof(sklb.bones[0]));

        AfxRead(in, jointCnt, sizeof(sklb.bones[0].lodError), &sklb.bones[0].lodError, sizeof(sklb.bones[0]));

        AfxRead(in, jointCnt, sizeof(sklb.bones[0].id.str), &sklb.bones[0].id.str, sizeof(sklb.bones[0]));

        if (AfxBuildSkeletons(sim, NIL, 1, &sklb, &dst[i]))
            AfxThrowError();

        AfxEndSkeletonBuilding(&sklb);

        afxSkeleton skl = dst[i];
    }

    return err;
}

_AKX afxError AfxSerializeSkeletons(afxStream out, afxNat cnt, afxSkeleton const src[])
{
    afxError err = NIL;
    AfxAssertObjects(1, &out, afxFcc_IOS);
    AfxAssert(src);
    AfxAssert(cnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        _afxSerializedChunk chnkHdr;
        chnkHdr.fcc = afxFcc_SKL;
        chnkHdr.len = 0;
        afxSize chnkHdrPos = AfxAskStreamPosn(out);
        AfxWrite(out, 1, sizeof(chnkHdr), &chnkHdr, sizeof(chnkHdr));
        afxSize chnkStartPos = AfxAskStreamPosn(out);
        
        afxSkeleton skl = src[i];
        _afxSerializedSkl chnk = { 0 };
        afxNat jointCnt = (chnk.jointCnt = skl->jointCnt);
        AfxReflectString(&skl->id, &chnk.name);
        AfxWrite(out, 1, sizeof(chnk), &chnk, sizeof(chnk));

        AfxWrite(out, jointCnt, sizeof(skl->local[0]), &skl->local[0], sizeof(skl->local[0]));

        AfxWrite(out, jointCnt, sizeof(skl->iw[0]), &skl->iw[0], sizeof(skl->iw[0]));

        AfxWrite(out, jointCnt, sizeof(skl->parentIdx[0]), &skl->parentIdx[0], sizeof(skl->parentIdx[0]));

        AfxWrite(out, jointCnt, sizeof(skl->lodError[0]), &skl->lodError[0], sizeof(skl->lodError[0]));

        AfxWrite(out, jointCnt, sizeof(skl->label[0]), &skl->label[0], sizeof(skl->label[0]));


        chnkHdr.len = AfxAskStreamPosn(out) - chnkStartPos;
        AfxWriteAt(out, chnkHdrPos, 1, sizeof(chnkHdr), &chnkHdr, sizeof(chnkHdr));
        AfxSkipStream(out, (afxInt)chnkHdr.len);
    }

    return err;
}

_AKX void AfxBuildIndexedCompositeBufferTransposed(afxSkeleton skl, awxWorldPose const* wp, afxNat const* indices, afxNat idxCnt, afxReal buffer[][3][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AfxAssertRange(skl->jointCnt, idxCnt, 1);
    AfxAssertRange(wp->xformCnt, idxCnt, 1);
    AfxAssert(wp);
    AfxAssert(buffer);

    for (afxNat i = 0; i < idxCnt; i++)
    {
        Mmt.BuildSingleCompositeFromWorldPoseTranspose_Generic(skl->iw[indices[i]], wp->world[indices[i]], buffer[i]);
    }
}

_AKX void AfxBuildIndexedCompositeBuffer(afxSkeleton skl, awxWorldPose const* wp, afxNat const* indices, afxNat idxCnt, afxM4d buffer[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AfxAssertRange(skl->jointCnt, idxCnt, 1);
    AfxAssertRange(wp->xformCnt, idxCnt, 1);
    AfxAssert(wp);
    AfxAssert(buffer);

    for (afxNat i = 0; i < idxCnt; i++)
    {
        Mmt.BuildSingleCompositeFromWorldPose_Generic(skl->iw[indices[i]], wp->world[indices[i]], buffer[i]);
    }
}

_AKXINL void UpdateWorldPoseChildren(afxSkeleton skl, afxNat parentBone, awxPose* lp, afxM4d const offset, awxWorldPose* wp)
{
    // Should be compatible with void UpdateWorldPoseChildren(const skeleton *Skeleton, int ParentBone, local_pose *LocalPose, const float *Offset4x4, world_pose *Result)

    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AfxAssertRange(skl->jointCnt, parentBone, 1);
    AfxAssertRange(wp->xformCnt, parentBone, 1);
    AfxAssertRange(lp->cap, parentBone, 1);

    if (!offset)
        offset = AFX_M4D_IDENTITY;

    afxNat jointCnt = skl->jointCnt;
    
    lp->traversalId += 1;

    afxM4d* composite = wp->composite ? &wp->composite[parentBone] : NIL;
    afxNat bone = parentBone;    
    afxM4d* World = &wp->world[parentBone];
    awxPoseTransform* v13 = &lp->xforms[parentBone];
    afxNat remainBoneCnt = jointCnt - parentBone;

    if (remainBoneCnt > 0)
    {
        while (1)
        {
            afxNat v14 = skl->parentIdx[bone];
            afxV4d const* v15 = v14 == AFX_INVALID_INDEX ? offset : (afxV4d const*)(wp->world[v14]);
            afxTransform* v16 = &v13->xform;

            if (composite)
            {
                Mmt.BWP_Dispatch(v16, v15, skl->iw[bone], composite[0], World[0]);
                ++composite;
            }
            else
                Mmt.BWPNC_Dispatch(v16, v15, World[0]);

            v13->traversalId = lp->traversalId;
            ++v13;
            ++bone;
            ++World;
            --remainBoneCnt;

            if (remainBoneCnt <= 0)
                break;

            while (1)
            {
                if (skl->parentIdx[bone] != AFX_INVALID_INDEX && lp->xforms[skl->parentIdx[bone]].traversalId == lp->traversalId)
                    break;

                ++v13;
                ++bone;
                ++World;
                --remainBoneCnt;
                
                if (composite)
                    ++composite;

                if (remainBoneCnt <= 0)
                    goto LABEL_28;
            }

            if (remainBoneCnt <= 0)
                break;
        }
    }
LABEL_28:
    return;
}

_AKXINL void __cdecl AfxIKUpdate(afxNat LinkCountInit, afxNat EEBoneIdx, afxReal const desiredPos[3], afxNat iterCnt, afxSkeleton skl, afxReal const ModelRootTransform[4][4], awxPose* lp, awxWorldPose* wp)
{
    // Should be compatible with void IKUpdate(int LinkCountInit, int EEBoneIndex, const float *DesiredPosition3, int IterationCount, const skeleton *Skeleton, const float *ModelRootTransform, local_pose *LocalPose, world_pose *WorldPose)

    afxNat Links[256];
    afxNat v9 = EEBoneIdx;
    afxInt v10 = 0;
    afxNat v11 = 0;
    afxNat LinkCount = 0;

    if (LinkCountInit >= 0)
    {
        do
        {
            Links[v11] = v9;
            v9 = skl->parentIdx[v9];
            ++v10;

            if (v9 == AFX_INVALID_INDEX)
                break;

            ++v11;
        } while (v11 <= LinkCountInit);

        LinkCount = v10;
    }

    if (iterCnt > 0)
    {
        afxNat v46 = iterCnt;

        do
        {
            afxNat v13 = 1;

            if (v10 > 1)
            {
                do
                {
                    afxV3d CurrentPosition;
                    afxV4d* v14 = AfxWorldPoseGetWorldMatrix(wp, Links[0]);
                    AfxCopyV3d(CurrentPosition, v14[3]);
                    afxNat v16 = Links[v13];
                    afxTransform* t = AfxGetPoseTransform(lp, v16);
                    afxV4d* v19 = AfxWorldPoseGetWorldMatrix(wp, v16);
                    
                    afxV3d const va =
                    {
                        (desiredPos[0] - v19[3][0]) * v19[0][0] + (desiredPos[1] - v19[3][1]) * v19[0][1] + (desiredPos[2] - v19[3][2]) * v19[0][2] + (v19[3][0] * 0.0),
                        (desiredPos[0] - v19[3][0]) * v19[1][0] + (desiredPos[1] - v19[3][1]) * v19[1][1] + (desiredPos[2] - v19[3][2]) * v19[1][2] + (v19[3][1] * 0.0),
                        (desiredPos[0] - v19[3][0]) * v19[2][0] + (desiredPos[1] - v19[3][1]) * v19[2][1] + (desiredPos[2] - v19[3][2]) * v19[2][2] + (v19[3][2] * 0.0)
                    };

                    afxV3d const vb =
                    {
                        (((CurrentPosition[0] - v19[3][0]) * v19[0][0] + (CurrentPosition[1] - v19[3][1]) * v19[0][1] + (CurrentPosition[2] - v19[3][2]) * v19[0][2] + (v19[3][0] * 0.0)) + va[0]) * 0.5,
                        (((CurrentPosition[0] - v19[3][0]) * v19[1][0] + (CurrentPosition[1] - v19[3][1]) * v19[1][1] + (CurrentPosition[2] - v19[3][2]) * v19[1][2] + (v19[3][1] * 0.0)) + va[1]) * 0.5,
                        (((CurrentPosition[0] - v19[3][0]) * v19[2][0] + (CurrentPosition[1] - v19[3][1]) * v19[2][1] + (CurrentPosition[2] - v19[3][2]) * v19[2][2] + (v19[3][2] * 0.0)) + va[2]) * 0.5
                    };

                    afxQuat LocalRotationIncrement;
                    //AfxSetQuat(LocalRotationIncrement, );
                    LocalRotationIncrement[0] = vb[1] * va[2] - vb[2] * va[1];
                    LocalRotationIncrement[1] = va[0] * vb[2] - va[2] * vb[0];
                    LocalRotationIncrement[2] = va[1] * vb[0] - vb[1] * va[0];
                    LocalRotationIncrement[3] = vb[2] * va[2] + vb[1] * va[1] + vb[0] * va[0];
                    AfxEstimateNormalizedQuat(LocalRotationIncrement, LocalRotationIncrement);

                    t->flags |= afxTransformFlags_ROTATED;
                    afxQuat v40;
                    AfxMultiplyQuat(v40, t->orientation, LocalRotationIncrement);
                    AfxCopyQuat(t->orientation, v40);
                    UpdateWorldPoseChildren(skl, v16, lp, ModelRootTransform, wp);
                    ++v13;
                } while (v13 < LinkCount);

                v10 = LinkCount;
            }
            --v46;
        } while (v46);
    }
}

_AKXINL afxBool IKUpdate2Bone(afxNat EEBoneIndex, afxReal const desiredPos[3], afxReal const restrictedMovPlaneNormal[3], afxSkeleton skl, afxReal const modelRootTransform[4][4], awxPose* lp, awxWorldPose* wp, afxReal hyperExtStart, afxReal hyperExtScale)
{
    // Should be compatible with char IKUpdate2Bone(int EEBoneIndex, const float *DesiredPosition3, const float *RestrictedMovementPlaneNormal3, const skeleton *Skeleton, const float *ModelRootTransform, local_pose *LocalPose, world_pose *WorldPose, float HyperExtensionStart, float HyperExtensionScale)
    return 0;
}

_AKXINL void AfxComputeWorldPoseNoCompositeLod(afxSkeleton skl, afxNat baseJoint, afxNat jointCnt, afxNat firstValidLocalArt, afxNat validLocalArtCnt, awxPose const *lp, afxM4d const offset, awxWorldPose *wp)
{
    // Should be compatible with void BuildWorldPoseNoCompositeLOD(const skeleton *Skeleton, int FirstBone, int BoneCount, int FirstValidLocalBone, int ValidLocalBoneCount, const granny::local_pose *LocalPose, const float *Offset4x4, world_pose *Result)

    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AfxAssertRange(skl->jointCnt, baseJoint, jointCnt);
    AfxAssertRange(wp->xformCnt, baseJoint, jointCnt);
    AfxAssertRange(lp->cap, baseJoint, jointCnt);

    if (!offset)
        offset = AFX_M4D_IDENTITY;
    
    for (afxNat i = 0; i < jointCnt; i++)
    {
        afxNat jointIdx = baseJoint + i;
        AfxAssertRange(skl->jointCnt, jointIdx, 1);
        afxNat parentIdx = skl->parentIdx[jointIdx];
        afxTransform *t = &lp->xforms[jointIdx].xform;
        const afxV4d *parentW = parentIdx == AFX_INVALID_INDEX ? offset : (const afxV4d *)(wp->world[parentIdx]);

        if (validLocalArtCnt <= 0)
            t = &skl->local[jointIdx];

        Mmt.BWPNC_Dispatch(t, parentW, wp->world[jointIdx]);
        --validLocalArtCnt;
    }
}

_AKXINL void AfxComputeWorldPoseLod(afxSkeleton skl, afxNat baseJoint, afxNat jointCnt, afxNat firstValidLocalArt, afxNat validLocalArtCnt, awxPose const *lp, afxM4d const offset, awxWorldPose *wp)
{
    // Should be compatible with void BuildWorldPoseLOD(const skeleton *Skeleton, int FirstBone, int BoneCount, int FirstValidLocalBone, int ValidLocalBoneCount, const local_pose *LocalPose, const float *Offset4x4, world_pose *Result)

    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AfxAssertRange(skl->jointCnt, baseJoint, jointCnt);
    AfxAssertRange(lp->cap, baseJoint, jointCnt);
    AfxAssertRange(wp->xformCnt, baseJoint, jointCnt);

    if (wp->composite)
    {
        if (!offset)
            offset = AFX_M4D_IDENTITY;
        
        for (afxNat i = 0; i < jointCnt; i++)
        {
            afxNat jointIdx = baseJoint + i;
            AfxAssertRange(skl->jointCnt, jointIdx, 1);
            afxTransform *t = &lp->xforms[jointIdx].xform;
            afxNat parentIdx = skl->parentIdx[jointIdx];
            const afxV4d *parentW = parentIdx == AFX_INVALID_INDEX ? offset : (const afxV4d *)(wp->world[parentIdx]);

            if (validLocalArtCnt <= 0)
                t = &skl->local[jointIdx];

            Mmt.BWP_Dispatch(t, parentW, skl->iw[jointIdx], wp->composite[jointIdx], wp->world[jointIdx]);
        }
    }
    else
    {
        AfxComputeWorldPoseNoCompositeLod(skl, baseJoint, jointCnt, firstValidLocalArt, validLocalArtCnt, lp, offset, wp);
    }
}

_AKX void AfxComputeWorldPose(afxSkeleton skl, afxNat baseJoint, afxNat jointCnt, awxPose const *lp, afxM4d const offset, awxWorldPose *wp)
{
    // Should be compatible with void BuildWorldPose(const skeleton *Skeleton, int FirstBone, int BoneCount, const local_pose *LocalPose, const float *Offset4x4, world_pose *Result)

    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AfxAssertRange(skl->jointCnt, baseJoint, jointCnt);
    AfxAssertRange(lp->cap, baseJoint, jointCnt);
    AfxComputeWorldPoseLod(skl, baseJoint, jointCnt, baseJoint, jointCnt, lp, offset, wp);
}

_AKX void AfxComputeRestWorldPose(afxSkeleton skl, afxNat baseJoint, afxNat jointCnt, afxM4d const offset, awxWorldPose *wp)
{
    // Should be compatible with void BuildRestWorldPose(const skeleton *Skeleton, int FirstBone, int BoneCount, const float *Offset4x4, world_pose *Result)

    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AfxAssertRange(skl->jointCnt, baseJoint, jointCnt);
    AfxAssertRange(wp->xformCnt, baseJoint, jointCnt);

    if (!offset)
        offset = AFX_M4D_IDENTITY;

    for (afxNat i = 0; i < jointCnt; i++)
    {
        afxNat jointIdx = baseJoint + i;
        AfxAssertRange(skl->jointCnt, jointIdx, 1);
        afxNat parentIdx = skl->parentIdx[jointIdx];
        const afxV4d*parentW = AFX_INVALID_INDEX == parentIdx ? offset : wp->world[parentIdx];

        if (parentIdx != AFX_INVALID_INDEX)
        {
            AfxAssertRange(skl->jointCnt, parentIdx, 1);
        }

        if (wp->composite)
        {
            Mmt.BWP_Dispatch(&skl->local[jointIdx], parentW, skl->iw[jointIdx], wp->composite[jointIdx], wp->world[jointIdx]);
        }
        else
        {
            Mmt.BWPNC_Dispatch(&skl->local[jointIdx], parentW, wp->world[jointIdx]);
        }
    }
}

_AKX void AfxComputeRestLocalPose(afxSkeleton skl, afxNat baseJoint, afxNat jointCnt, awxPose *lp)
{
    // Should be compatible with void BuildRestLocalPose(const skeleton *Skeleton, int FirstBone, int BoneCount, local_pose *LocalPose)

    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AfxAssertRange(skl->jointCnt, baseJoint, jointCnt);
    AfxAssertRange(lp->cap, baseJoint, jointCnt);

    for (afxNat i = jointCnt; i-- > 0;)
    {
        afxNat jointIdx = baseJoint + i;
        AfxAssertRange(skl->jointCnt, jointIdx, 1);
        awxPoseTransform *t = &lp->xforms[jointIdx];
        t->cnt = 1;
        t->weight = 1.0;
        AfxCopyTransform(&t->xform, &skl->local[jointIdx]);
    }
}

_AKX void AfxLocalPoseFromWorldPose(afxSkeleton skl, awxPose *lp, awxWorldPose const* wp, afxM4d const offset, afxNat firstBone, afxNat jointCnt)
{
    // Should be compatible with void LocalPoseFromWorldPose(const skeleton *Skeleton, local_pose *Result, const world_pose *WorldPose, const float *Offset4x4, int FirstBone, int BoneCount)

    afxError err = AFX_ERR_NONE;
    AfxAssert(lp);
    AfxAssert(wp);
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AfxAssertRange(skl->jointCnt, firstBone, jointCnt);
    AfxAssertRange(lp->cap, firstBone, jointCnt);
    AfxAssertRange(wp->xformCnt, firstBone, jointCnt);

    if (!offset)
        offset = AFX_M4D_IDENTITY;

    for (afxNat i = firstBone + jointCnt; i-- > 0;)
    {
        AfxAssertRange(skl->jointCnt, i, 1);
        afxNat parentIdx = skl->parentIdx[i];
        afxM4d InverseParent;

        if (parentIdx == AFX_INVALID_INDEX) AfxInvertAtm4d(offset, InverseParent);
        else
        {
            AfxAssertRange(skl->jointCnt, parentIdx, 1);
            AfxInvertAtm4d(wp->world[parentIdx], InverseParent);
        }

        afxM4d Local;
        AfxMultiplyAtm4d(Local, InverseParent, wp->world[i]);
        
        afxM3d Linear;
        AfxSetM3d(Linear, Local[0], Local[1], Local[2]);

        afxM3d Q, Scale;

        if (!AfxPolarDecomposeM3d(Linear, 0.0000099999997, Q, Scale))
            AfxError("Can't accurately decompose MAX transform Q");

        afxV3d Position;
        AfxCopyV3d(Position, Local[3]);
        afxQuat Orientation;
        AfxQuatFromM3d(Orientation, Q);
        AfxNormalizeQuat(Orientation, Orientation);
        afxTransform* t = AfxGetPoseTransform(lp, i);
        AfxSetTransformWithIdentityCheck(t, Position, Orientation, Scale);
    }
}

_AKX void AfxLocalPoseFromWorldPoseNoScale(afxSkeleton skl, awxPose* lp, awxWorldPose const* wp, afxM4d const offset, afxNat firstBone, afxNat jointCnt)
{
    // Should be compatible with void LocalPoseFromWorldPoseNoScale(const skeleton *Skeleton, local_pose *Result, const world_pose *WorldPose, const float *Offset4x4, int FirstBone, int BoneCount)

    afxError err = AFX_ERR_NONE;
    AfxAssert(wp);
    AfxAssert(lp);
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AfxAssertRange(skl->jointCnt, firstBone, jointCnt);
    AfxAssertRange(lp->cap, firstBone, jointCnt);
    AfxAssertRange(wp->xformCnt, firstBone, jointCnt);

    if (!offset)
        offset = AFX_M4D_IDENTITY;

    for (afxNat i = firstBone + jointCnt; i-- > 0;)
    {
        afxNat parentIdx = skl->parentIdx[i];
        afxM4d InverseParent;

        if (parentIdx == AFX_INVALID_INDEX) AfxInvertAtm4d(offset, InverseParent);
        else
        {
            AfxAssertRange(skl->jointCnt, parentIdx, 1);
            AfxInvertAtm4d(wp->world[parentIdx], InverseParent);
        }

        afxM4d Local;
        AfxMultiplyAtm4d(Local, wp->world[i], InverseParent);
        afxTransform* t = AfxGetPoseTransform(lp, i);

        AfxCopyV3d(t->position, Local[3]);
        afxM3d Rotation;
        AfxSetM3dTransposed(Rotation, Local[0], Local[1], Local[2]);
        AfxQuatFromM3d(t->orientation, Rotation);
        AfxNormalizeQuat(t->orientation, t->orientation);
        AfxResetM3d(t->scaleShear);
        t->flags = afxTransformFlags_TRANSLATED | afxTransformFlags_ROTATED;
    }
}

_AKX void AfxGetWorldMatrixFromLocalPose(afxSkeleton skl, afxNat jointIdx, awxPose const* lp, afxM4d const offset, afxM4d m, afxNat const* sparseBoneArray, afxNat const* sparseBoneArrayReverse)
{
    // Should be compatible with void GetWorldMatrixFromLocalPose(const skeleton *Skeleton, int BoneIndex, const local_pose *LocalPose, const float *Offset4x4, float *Result4x4, const int *SparseBoneArray, const int *SparseBoneArrayReverse)

    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AfxAssertRange(lp->cap, jointIdx, 1);

    AfxResetM4d(m);

    if (jointIdx != AFX_INVALID_INDEX)
    {
        for (afxNat i = jointIdx; i != AFX_INVALID_INDEX; i = skl->parentIdx[i])
        {
            AfxAssertRange(skl->jointCnt, i, 1);
            afxTransform* t = AfxGetPoseTransform(lp, sparseBoneArrayReverse ? sparseBoneArrayReverse[i] : i);
            afxM4d tmp, tmp2;
            AfxComposeTransformM4d(t, tmp);
            AfxMultiplyAtm4d(tmp2, m, tmp);
            AfxCopyM4d(m, tmp2);
        }
    }

    if (offset)
    {
        afxM4d tmp2;
        AfxMultiplyAtm4d(tmp2, offset, m);
        AfxCopyM4d(m, tmp2);
    }
}

#if 0
_AKXINL void _AfxSklCtorBubbleSortOnLod(afxSkeletonBlueprint *Builder, afxSkeletonBone *Bones, afxNat* remappingTable)
{
    afxSkeletonBone Temp;

    afxArena SortArena;
    AfxAllocateArena(NIL, &SortArena, NIL, AfxHint());
    afxNat *OldToNewMap = AfxRequestArenaUnit(&SortArena, sizeof(afxNat) * Builder->bones.cnt);
    afxNat *NewToOldMap = AfxRequestArenaUnit(&SortArena, sizeof(afxNat) * Builder->bones.cnt);
    afxNat v9 = 0;
    
    if (Builder->bones.cnt > 0)
    {
        afxNat *v10 = NewToOldMap;
        do
        {
            *(int *)((char *)v10 + (char *)OldToNewMap - (char *)NewToOldMap) = AFX_INVALID_INDEX;
            *v10 = v9++;
            ++v10;
        } while (v9 < Builder->bones.cnt);
    }
    
    afxByte* v11 = AfxRequestArenaUnit(&SortArena, sizeof(afxSkeletonBone) * Builder->bones.cnt);

    for (afxNat i = Builder->bones.cnt; i--> 0;)
    {
        Bones[i].name;
    }

    afxSkeletonBone* v12 = Bones;

    if (152 * Builder->bones.cnt)
    {
        afxNat v13 = v11 - (afxByte *)Bones;
        afxNat v14 = 152 * Builder->bones.cnt;

        do
        {
            *((afxByte *)&v12->name + v13) = v12->name;
            v12 = (afxSkeletonBone *)((char *)v12 + 1);
            --v14;
        } while (v14);
    }

    afxNat v15 = Builder->bones.cnt - 1;

    if (v15 >= 0)
    {
        afxNat v35 = Builder->bones.cnt;

        do
        {
            afxNat v16 = 0;
            afxNat v17 = v15 - 1;
            afxNat v32 = v15 - 1;

            if (v15 >= 4)
            {
                afxSkeletonBone *v18 = Bones + 1;
                afxNat32 v34 = (unsigned int)v15 >> 2;
                afxNat *v19 = NewToOldMap + 2;
                afxNat j = 4 * ((unsigned int)v15 >> 2);

                do
                {
                    if (v18[-1].lodError < (double)v18->lodError)
                    {
                        afxNat v20 = *(v19 - 2);
                        *(v19 - 2) = *(v19 - 1);
                        *(v19 - 1) = v20;
                        memcpy(&Temp, &v18[-1], sizeof(Temp));
                        memcpy(&v18[-1], v18, sizeof(afxSkeletonBone));
                        memcpy(v18, &Temp, sizeof(afxSkeletonBone));
                    }

                    if (v18->lodError < (double)v18[1].lodError)
                    {
                        afxNat v21 = *(v19 - 1);
                        *(v19 - 1) = *v19;
                        *v19 = v21;
                        memcpy(&Temp, v18, sizeof(Temp));
                        memcpy(v18, &v18[1], sizeof(afxSkeletonBone));
                        memcpy(&v18[1], &Temp, sizeof(afxSkeletonBone));
                    }

                    if (v18[1].lodError < (double)v18[2].lodError)
                    {
                        afxNat v22 = *v19;
                        *v19 = v19[1];
                        v19[1] = v22;
                        memcpy(&Temp, &v18[1], sizeof(Temp));
                        memcpy(&v18[1], &v18[2], sizeof(afxSkeletonBone));
                        memcpy(&v18[2], &Temp, sizeof(afxSkeletonBone));
                    }

                    if (v18[2].lodError < (double)v18[3].lodError)
                    {
                        afxNat v23 = v19[1];
                        v19[1] = v19[2];
                        v19[2] = v23;
                        memcpy(&Temp, &v18[2], sizeof(Temp));
                        memcpy(&v18[2], &v18[3], sizeof(afxSkeletonBone));
                        memcpy(&v18[3], &Temp, sizeof(afxSkeletonBone));
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
                afxSkeletonBone *v24 = &Bones[v16 + 1];

                do
                {
                    if (v24[-1].lodError < (double)v24->lodError)
                    {
                        afxNat v25 = NewToOldMap[v16];
                        NewToOldMap[v16] = NewToOldMap[v16 + 1];
                        NewToOldMap[v16 + 1] = v25;
                        memcpy(&Temp, &v24[-1], sizeof(Temp));
                        memcpy(&v24[-1], v24, sizeof(afxSkeletonBone));
                        memcpy(v24, &Temp, sizeof(afxSkeletonBone));
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

    for (afxNat i = 0; i < Builder->bones.cnt; ++i)
        OldToNewMap[NewToOldMap[i]] = i;

    if (remappingTable)
    {
        for (afxNat k = 0; k < Builder->bones.cnt; ++k)
            remappingTable[k] = OldToNewMap[remappingTable[k]];
    }

    for (afxNat i = 0; i < Builder->bones.cnt; ++i)
    {
        if (AFX_INVALID_INDEX != Bones[i].parentIdx)
            Bones[i].parentIdx = OldToNewMap[Bones[i].parentIdx];
    }

    AfxDeallocateArena(&SortArena);
}
#endif

_AKXINL void _AfxSklCtorWriteBone(afxSkeletonBuilder const* sklb, afxSkeleton skl, afxNat* boneWrittenMap, afxNat inBoneIdx, afxNat* outBoneIdx)
{
    afxError err = NIL;    
    afxNat idx = boneWrittenMap[inBoneIdx];

    if (idx == (afxNat)-2)
    {
        AfxError("Circular parenting chain detected at bone %d", inBoneIdx);
        AfxThrowError();
    }
    else if (idx == AFX_INVALID_INDEX)
    {
        boneWrittenMap[inBoneIdx] = (afxNat)-2; // temp

        afxNat parentIdx = sklb->bones[inBoneIdx].parentIdx;
        afxNat parentIdxWritten = AFX_INVALID_INDEX;

        if (parentIdx != AFX_INVALID_INDEX)
        {
            AfxAssertRange(skl->jointCnt, parentIdx, 1);

            _AfxSklCtorWriteBone(sklb, skl, boneWrittenMap, parentIdx, outBoneIdx);
            parentIdxWritten = boneWrittenMap[parentIdx];
        }

        if (skl->parentIdx)
            skl->parentIdx[*outBoneIdx] = parentIdxWritten;

        if (skl->local)
            AfxCopyTransform(&skl->local[*outBoneIdx], &sklb->bones[inBoneIdx].local);

        if (skl->iw)
            AfxCopyM4d(skl->iw[*outBoneIdx], sklb->bones[inBoneIdx].iw);

        if (skl->lodError)
            skl->lodError[*outBoneIdx] = sklb->bones[inBoneIdx].lodError;

        if (skl->label)
            if (!AfxCatalogStrings2(skl->strc, 1, &sklb->bones[inBoneIdx].id.str.str, &skl->label[*outBoneIdx]))
                AfxThrowError();

        if (skl->udd)
            skl->udd[*outBoneIdx] = sklb->bones[inBoneIdx].extendedData;

        //if (sklb->IndexRemapping)
            //sklb->IndexRemapping(data, inBoneIdx, *outBoneIdx);

        boneWrittenMap[inBoneIdx] = (*outBoneIdx)++;
    }
}

_AKX afxError _AfxSklCtor(afxSkeleton skl, afxCookie const *cookie)
{
    AfxEntry("skl=%p", skl);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);

    afxSimulation sim = cookie->udd[0];
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxStringCatalog strc = cookie->udd[1];
    afxSkeletonBlueprint const* bluep = cookie->udd[2];
    bluep += cookie->no;
    AfxAssert(bluep);

    if (!(skl->strc = strc))
        AfxResetString(&skl->id);
    else if (!AfxCatalogStrings2(strc, 1, &bluep->id.str.str, &skl->id))
        AfxThrowError();
    else
        AfxReacquireObjects(1, (void*[]) { strc });

    skl->allowedLodErrFadingFactor = sim->allowedLodErrFadingFactor;

    if (!err)
    {
        afxMmu mmu = AfxGetSimulationMmu(sim);

        afxNat jointCnt = bluep->jointCnt;
        afxNat lodType = bluep->lodType;

        skl->jointCnt = jointCnt;
        skl->lodType = lodType;

        if (!(skl->parentIdx = AfxAllocate(mmu, jointCnt, sizeof(skl->parentIdx[0]), 0, AfxHint()))) AfxThrowError();
        else
        {
            for (afxNat i = 0; i < jointCnt; i++)
                skl->parentIdx[i] = AFX_INVALID_INDEX; // to avoid crashes we must set it.

            if (!(skl->local = AfxAllocate(mmu, jointCnt, sizeof(skl->local[0]), AFX_SIMD_ALIGN, AfxHint()))) AfxThrowError();
            else
            {
                for (afxNat i = 0; i < jointCnt; i++)
                    AfxResetTransform(&skl->local[i]);

                if (!(skl->iw = AfxAllocate(mmu, jointCnt, sizeof(skl->iw[0]), AFX_SIMD_ALIGN, AfxHint()))) AfxThrowError();
                else
                {
                    for (afxNat i = 0; i < jointCnt; i++)
                        AfxResetM4d(skl->iw[i]);

                    if (!(skl->lodError = AfxAllocate(mmu, jointCnt, sizeof(skl->lodError[0]), AFX_SIMD_ALIGN, AfxHint()))) AfxThrowError();
                    else
                    {
                        for (afxNat i = 0; i < jointCnt; i++)
                            skl->lodError[i] = -1.0;

                        skl->label = NIL;

                        if (strc && !(skl->label = AfxAllocate(mmu, jointCnt, sizeof(skl->label[0]), 0, AfxHint()))) AfxThrowError();
                        else
                        {
                            if (!bluep->joints)
                            {
                                for (afxNat i = 0; i < jointCnt; i++)
                                    AfxResetString(&skl->label[i]); // to avoid crashes we must set it.
                            }
                            else
                            {
                                if (!AfxCatalogStrings2(strc, jointCnt, bluep->joints, skl->label))
                                    AfxThrowError();
                            }

                            skl->udd = NIL;

                            if (err && skl->label)
                                AfxDeallocate(mmu, skl->label);
                        }

                        if (err && skl->lodError)
                            AfxDeallocate(mmu, skl->lodError);
                    }

                    if (err && skl->iw)
                        AfxDeallocate(mmu, skl->iw);
                }

                if (err && skl->local)
                    AfxDeallocate(mmu, skl->local);
            }

            if (err && skl->parentIdx)
                AfxDeallocate(mmu, skl->parentIdx);
        }
        
        
        AfxAssert(jointCnt);
        //skl->bones = NIL;

        //if (!(skl->bones = AfxAllocate(mmu, jointCnt, sizeof(skl->bones[0]), 0, AfxHint()))) AfxThrowError();
        //else
        {
#if 0
            afxNat *boneWrittenMap = AfxAllocate(mmu, jointCnt, sizeof(boneWrittenMap[0]), 0, AfxHint());
            AfxAssert(boneWrittenMap);

            for (afxNat i = 0; i < jointCnt; ++i)
                boneWrittenMap[i] = (afxNat)AFX_INVALID_INDEX;
#endif

            // estimate lod

            //for (afxNat i = 0; i < skl->jointCnt; i++)
            {
                // to avoid crashes we must set it.
                //skl->bones[i].idStrIdx = AFX_INVALID_INDEX;
                //skl->bones[i].parentIdx = AFX_INVALID_INDEX;
            }

#if 0
            afxNat outBoneIdx = 0;

            if (!skipBoneWrite)
            {
                for (afxNat i = 0; i < skl->jointCnt; i++)
                {
                    if (AFX_INVALID_INDEX == sklb->bones[i].parentIdx)
                        _AfxSklCtorWriteBone(sklb, skl, boneWrittenMap, i, &outBoneIdx);
                }

                for (afxNat i = 0; i < skl->jointCnt; i++)
                {
                    if (AFX_INVALID_INDEX != sklb->bones[i].parentIdx)
                        _AfxSklCtorWriteBone(sklb, skl, boneWrittenMap, i, &outBoneIdx);
                }
            }

            //_AfxSklCtorBubbleSortOnLOD(blueprint, skl->bones, remapTable);
#endif
            
            //skl->jointCnt = jointCnt;
            AfxAssert(skl->jointCnt == jointCnt);

#if 0
            AfxDeallocate(mmu, boneWrittenMap);
#endif
            
            //if (err && skl->bones)
                //AfxDeallocate(mmu, skl->bones);
        }
    }
    return err;
}

_AKX afxError _AfxSklDtor(afxSkeleton skl)
{
    AfxEntry("skl=%p", skl);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);

    afxSimulation sim = AfxGetObjectProvider(skl);
    afxMmu mmu = AfxGetSimulationMmu(sim);

    //for (afxNat i = 0; i < skl->jointCnt; i++)
        //AfxDeallocateString(&skl->bones[i].id);

    //if (skl->bones)
        //AfxDeallocate(mmu, skl->bones);

    if (skl->label)
        AfxDeallocate(mmu, skl->label);

    if (skl->lodError)
        AfxDeallocate(mmu, skl->lodError);

    if (skl->iw)
        AfxDeallocate(mmu, skl->iw);

    if (skl->local)
        AfxDeallocate(mmu, skl->local);

    if (skl->parentIdx)
        AfxDeallocate(mmu, skl->parentIdx);

    if (skl->strc)
        AfxReleaseObjects(1, (void*[]) { skl->strc });

    return err;
}

_AKX afxClassConfig _AfxSklClsConfig =
{
    .fcc = afxFcc_SKL,
    .name = "Skeleton",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxSkeleton)),
    .mmu = NIL,
    .ctor = (void*)_AfxSklCtor,
    .dtor = (void*)_AfxSklDtor
};

////////////////////////////////////////////////////////////////////////////////

_AKX afxError AfxAcquireSkeletons(afxSimulation sim, afxStringCatalog strc, afxNat cnt, afxSkeletonBlueprint const sklb[], afxSkeleton skeletons[])
{
    //AfxEntry("sim=%p,name=%.*s,jointCnt=%u,boneName=%p,boneParentIdx=%p,bonePos=%p,boneOrient=%p,boneScale=%p", sim, AfxPushString(name), jointCnt, boneName, boneParentIdx, bonePos, boneOrient, boneScale);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AfxAssert(skeletons);
    AfxAssert(sklb);
    AfxAssert(cnt);

    if (AfxAcquireObjects(AfxGetSkeletonClass(sim), cnt, (afxObject*)skeletons, (void const*[]) { sim, strc, (void*)sklb, FALSE }))
        AfxThrowError();

    return err;
}

_AKX afxError AfxBuildSkeletons(afxSimulation sim, afxStringCatalog strc, afxNat cnt, afxSkeletonBuilder const builders[], afxSkeleton skeletons[])
{
    //AfxEntry("sim=%p,name=%.*s,jointCnt=%u,boneName=%p,boneParentIdx=%p,bonePos=%p,boneOrient=%p,boneScale=%p", sim, AfxPushString(name), jointCnt, boneName, boneParentIdx, bonePos, boneOrient, boneScale);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AfxAssert(builders);
    AfxAssert(cnt);
    AfxAssert(skeletons);

    afxMmu mmu = AfxGetSimulationMmu(sim);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxSkeletonBuilder const* sklb = &builders[i];
        afxSkeletonBlueprint bluep = { 0 };
        bluep.jointCnt = sklb->jointCnt;
        AfxMakeString32(&bluep.id, &sklb->id.str.str);
        bluep.lodType = sklb->lodType;

        if (AfxAcquireSkeletons(sim, strc, 1, &bluep, &skeletons[i]))
            AfxThrowError();

        afxSkeleton skl = skeletons[i];

        afxNat *boneWrittenMap = AfxAllocate(mmu, skl->jointCnt, sizeof(boneWrittenMap[0]), 0, AfxHint());
        AfxAssert(boneWrittenMap);

        for (afxNat i = 0; i < skl->jointCnt; ++i)
            boneWrittenMap[i] = (afxNat)AFX_INVALID_INDEX;

        afxNat outBoneIdx = 0;

        for (afxNat i = 0; i < skl->jointCnt; i++)
        {
            if (AFX_INVALID_INDEX == sklb->bones[i].parentIdx)
                _AfxSklCtorWriteBone(sklb, skl, boneWrittenMap, i, &outBoneIdx);
        }

        for (afxNat i = 0; i < skl->jointCnt; i++)
        {
            if (AFX_INVALID_INDEX != sklb->bones[i].parentIdx)
                _AfxSklCtorWriteBone(sklb, skl, boneWrittenMap, i, &outBoneIdx);
        }
        
        //_AfxSklCtorBubbleSortOnLOD(blueprint, skl->bones, remapTable);

        AfxDeallocate(mmu, boneWrittenMap);

        afxString s;

        if (!skl->strc)
            AfxResetString(&s);
        else
            AfxResolveStrings2(skl->strc, 1, &skl->id, &s);

        AfxEcho("Skeleton <%.*s> assembled. %p\n    Listing %u joints:", AfxPushString(&s), skl, skl->jointCnt);

        for (afxNat i = 0; i < skl->jointCnt; i++)
        {
            if (!skl->strc)
                AfxResetString(&s);
            else
                AfxResolveStrings2(skl->strc, 1, &skl->label[i], &s);

            AfxLogMessageFormatted(0xFF, "\n    %u <%.*s> %i", i, AfxPushString(&s), skl->parentIdx[i]);
        }

    }

    return err;
}

_AKX void AfxTransformSkeletons(afxM3d const ltm, afxM3d const iltm, afxReal linearTol, afxV4d const atv, afxReal affineTol, afxNat cnt, afxSkeleton skeletons[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(ltm);
    AfxAssert(iltm);
    AfxAssert(atv);
    AfxAssert(cnt);
    AfxAssert(skeletons);
    (void)affineTol;
    (void)linearTol;

    for (afxNat i = 0; i < cnt; i++)
    {
        afxSkeleton skl = skeletons[i];
        AfxAssertObjects(1, &skl, afxFcc_SKL);
        afxNat jointCnt = skl->jointCnt;

        for (afxNat j = 0; j < jointCnt; j++)
        {
            AfxAssimilateTransforms(ltm, iltm, atv, 1, &skl->local[j], &skl->local[j]);
            AfxAssimilateAtm4d(ltm, iltm, atv, 1, &skl->iw[j], &skl->iw[j]);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

_AKXINL afxError AfxBeginSkeletonBuilding(afxSkeletonBuilder* sklb, afxNat jointCnt, afxString const* id, afxNat lodType)
{
    afxError err = NIL;
    AfxAssert(sklb);
    
    AfxAssignFcc(sklb, afxFcc_SKLB);
    AfxAssert(jointCnt);
    sklb->jointCnt = jointCnt;
    sklb->bones = AfxAllocate(NIL, jointCnt, sizeof(sklb->bones[0]), 0, AfxHint());

    for (afxNat i = 0; i < jointCnt; i++)
    {
        sklb->bones[i].extendedData = NIL;
        AfxResetTransform(&sklb->bones[i].local);
        sklb->bones[i].lodError = -1.0;
        sklb->bones[i].parentIdx = AFX_INVALID_INDEX;
        AfxResetM4d(sklb->bones[i].iw);
        AfxMakeString32(&sklb->bones[i].id, NIL);
    }

    AfxMakeString32(&sklb->id, id);
    sklb->lodType = lodType;
    return err;
}

_AKXINL void AfxEndSkeletonBuilding(afxSkeletonBuilder* sklb)
{
    afxError err = NIL;
    AfxAssertType(sklb, afxFcc_SKLB);

    if (sklb->bones)
    {
        AfxDeallocate(NIL, sklb->bones);
        sklb->bones = NIL;
    }
    AfxAssignFcc(sklb, NIL);
}

_AKXINL void AfxResetBoneWithIw(afxSkeletonBuilder const* sklb, afxNat jointIdx, afxString const* id, afxNat parentIdx, afxTransform const* lt, afxM4d const iw, afxReal lodError)
{
    afxError err = NIL;
    AfxAssertType(sklb, afxFcc_SKLB);
    AfxAssertRange(sklb->jointCnt, jointIdx, 1);
    sklb->bones[jointIdx].lodError = lodError;
    sklb->bones[jointIdx].parentIdx = parentIdx;
    AfxCopyTransform(&sklb->bones[jointIdx].local, lt);
    AfxCopyM4d(sklb->bones[jointIdx].iw, iw);
    AfxMakeString32(&sklb->bones[jointIdx].id, id);
}

_AKXINL void AfxResetBone(afxSkeletonBuilder const* sklb, afxNat jointIdx, afxString const* id, afxNat parentIdx, afxTransform const* lt, afxTransform const* wt, afxReal lodError)
{
    afxError err = NIL;
    AfxAssertType(sklb, afxFcc_SKLB);
    AfxAssertRange(sklb->jointCnt, jointIdx, 1);
    AfxAssert(id && !AfxStringIsEmpty(id));
    afxM4d w, iw;
    AfxComposeTransformM4d(wt, w);
    AfxInvertM4d(w, iw);
    AfxResetBoneWithIw(sklb, jointIdx, id, parentIdx, lt, iw, lodError);
}
