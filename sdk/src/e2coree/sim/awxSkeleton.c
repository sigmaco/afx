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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#define _AFX_SIM_C
#define _AFX_SKELETON_C
#define _AFX_SIMULATION_C
#include "qwadro/sim/afxSimulation.h"
#include "qwadro/math/afxQuaternion.h"
#include "qwadro/math/afxVector.h"
#include "qwadro/math/afxMatrix.h"

_AFX void BuildSingleCompositeFromWorldPose_Generic(const afxReal InverseWorld4x4[4][4], const afxReal WorldMatrix[4][4], afxReal ResultComposite[4][4])
{
    AfxMultiplyTransposedAtm4(ResultComposite, WorldMatrix, InverseWorld4x4);
}

_AFX void BuildSingleCompositeFromWorldPoseTranspose_Generic(const afxReal InverseWorld4x4[4][4], const afxReal WorldMatrix[4][4], afxReal ResultComposite3x4[3][4])
{
    AfxMultiplyTransposedAtm4(ResultComposite3x4, WorldMatrix, InverseWorld4x4);
}

_AFX void AfxBuildIndexedCompositeBufferTransposed(awxSkeleton skl, awxWorldPose const* wp, afxNat const* indices, afxNat idxCnt, afxReal buffer[][3][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AfxAssertRange(skl->boneCnt, idxCnt, 1);
    AfxAssertRange(wp->xformCnt, idxCnt, 1);
    AfxAssert(wp);
    AfxAssert(buffer);

    for (afxNat i = 0; i < idxCnt; i++)
    {
        BuildSingleCompositeFromWorldPoseTranspose_Generic(skl->bones[indices[i]].iw, buffer[4 * indices[i]], buffer[i]);
    }
}

_AFX void AfxBuildIndexedCompositeBuffer(awxSkeleton skl, awxWorldPose const* wp, afxNat const* indices, afxNat idxCnt, afxReal buffer[][4][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AfxAssertRange(skl->boneCnt, idxCnt, 1);
    AfxAssertRange(wp->xformCnt, idxCnt, 1);
    AfxAssert(wp);
    AfxAssert(buffer);

    for (afxNat i = 0; i < idxCnt; i++)
    {
        BuildSingleCompositeFromWorldPose_Generic(skl->bones[indices[i]].iw, wp->world[indices[i]], buffer[i]);
    }
}

_AFXINL void BuildPositionWorldPoseOnly/*_Generic*/(afxReal const Position[3], afxReal const ParentMatrix[4][4], afxReal ResultWorldMatrix[4][4])
{
    AfxCopyM4d(ResultWorldMatrix, ParentMatrix);
    AfxPreMultiplyAtv3(ParentMatrix, Position, ResultWorldMatrix[3]);
}

_AFXINL void BuildPositionWorldPoseComposite/*_Generic*/(afxReal const Position[3], afxReal const ParentMatrix[4][4], afxReal const InverseWorld4x4[4][4], afxReal ResultComposite[4][4], afxReal ResultWorldMatrix[4][4])
{
    BuildPositionWorldPoseOnly/*_Generic*/(Position, ParentMatrix, ResultWorldMatrix);
    AfxMultiplyTransposedAtm4(ResultComposite, ResultWorldMatrix, InverseWorld4x4);
}

_AFXINL void BuildPositionOrientationWorldPoseOnly/*_Generic*/(afxReal const Position[3], afxReal const Orientation[4], afxReal const ParentMatrix[4][4], afxReal ResultWorldMatrix[4][4])
{
    afxTransform Transform;
    AfxResetTransform(&Transform);
    Transform.position[2] = Position[2];
    Transform.position[0] = Position[0];
    Transform.position[1] = Position[1];
    Transform.orientation[0] = Orientation[0];
    Transform.orientation[1] = Orientation[1];
    Transform.orientation[2] = Orientation[2];
    Transform.orientation[3] = Orientation[3];
    Transform.flags = afxTransformFlags_ORIGIN | afxTransformFlags_ORIENTATION;
    AfxComposeTransformWorldM4d/*_Generic*/(&Transform, ParentMatrix, ResultWorldMatrix);
}

_AFXINL void BuildPositionOrientationWorldPoseComposite/*_Generic*/(afxReal const Position[3], afxReal const Orientation[4], afxReal const ParentMatrix[4][4], afxReal const InverseWorld4x4[4][4], afxReal ResultComposite[4][4], afxReal ResultWorldMatrix[4][4])
{
    BuildPositionOrientationWorldPoseOnly/*_Generic*/(Position, Orientation, ParentMatrix, ResultWorldMatrix);
    AfxMultiplyTransposedAtm4(ResultComposite, ResultWorldMatrix, InverseWorld4x4);
}

_AFXINL void BuildIdentityWorldPoseOnly/*_Generic*/(afxReal const ParentMatrix[4][4], afxReal ResultWorldMatrix[4][4])
{
    AfxCopyM4d(ResultWorldMatrix, ParentMatrix);
}

_AFXINL void BuildIdentityWorldPoseComposite/*_Generic*/(afxReal const ParentMatrix[4][4], afxReal const InverseWorld4x4[4][4], afxReal ResultComposite[4][4], afxReal ResultWorldMatrix[4][4])
{
    BuildIdentityWorldPoseOnly/*_Generic*/(ParentMatrix, ResultWorldMatrix);
    AfxMultiplyTransposedAtm4(ResultComposite, ResultWorldMatrix, InverseWorld4x4);
}

_AFXINL void BWP_Dispatch(afxTransform *t, afxReal const parentW[4][4], afxReal const iw[4][4], afxReal composite[4][4], afxReal w[4][4])
{
    if (t->flags & afxTransformFlags_DEFORM)
    {
        AfxGetTransformWorldAndCompositeMatrix(t, parentW, iw, composite, w);
    }
    else if (t->flags == afxTransformFlags_ORIGIN)
    {
        BuildPositionWorldPoseComposite(t->position, parentW, iw, composite, w);
    }
    else if (t->flags)
    {
        BuildPositionOrientationWorldPoseComposite(t->position, t->orientation, parentW, iw, composite, w);
    }
    else
    {
        BuildIdentityWorldPoseComposite(parentW, iw, composite, w);
    }
}

_AFXINL void BWPNC_Dispatch(afxTransform *t, afxReal const parentW[4][4], afxReal w[4][4])
{
    if (t->flags & afxTransformFlags_DEFORM)
    {
        AfxComposeTransformWorldM4d(t, parentW, w);
    }
    else if (t->flags == afxTransformFlags_ORIGIN)
    {
        BuildPositionWorldPoseOnly(t->position, parentW, w);
    }
    else if (t->flags)
    {
        BuildPositionOrientationWorldPoseOnly(t->position, t->orientation, parentW, w);
    }
    else
    {
        BuildIdentityWorldPoseOnly(parentW, w);
    }
}

_AFXINL void UpdateWorldPoseChildren(awxSkeleton skl, afxNat parentBone, awxPose* lp, afxReal const offset[4][4], awxWorldPose* wp)
{
    // Should be compatible with void UpdateWorldPoseChildren(const skeleton *Skeleton, int ParentBone, local_pose *LocalPose, const float *Offset4x4, world_pose *Result)

    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AfxAssertRange(skl->boneCnt, parentBone, 1);
    AfxAssertRange(wp->xformCnt, parentBone, 1);
    AfxAssertRange(lp->cap, parentBone, 1);

    if (!offset)
        offset = AFX_M4D_IDENTITY;

    afxNat boneCnt = skl->boneCnt;
    
    lp->traversalId += 1;

    afxM4d* composite = wp->composite ? &wp->composite[parentBone] : NIL;
    awxSkeletonBone* bone = &skl->bones[parentBone];    
    afxM4d* World = &wp->world[parentBone];
    awxPoseTransform* v13 = &lp->xforms[parentBone];
    afxNat remainBoneCnt = boneCnt - parentBone;

    if (remainBoneCnt > 0)
    {
        while (1)
        {
            afxNat v14 = bone->parentIdx;
            afxV4d const* v15 = v14 == AFX_INVALID_INDEX ? offset : (afxV4d const*)(wp->world[v14]);
            afxTransform* v16 = &v13->xform;

            if (composite)
            {
                BWP_Dispatch(v16, v15, bone->iw, composite[0], World[0]);
                ++composite;
            }
            else
                BWPNC_Dispatch(v16, v15, World[0]);

            v13->traversalId = lp->traversalId;
            ++v13;
            ++bone;
            ++World;
            --remainBoneCnt;

            if (remainBoneCnt <= 0)
                break;

            while (1)
            {
                if (bone->parentIdx != AFX_INVALID_INDEX && lp->xforms[bone->parentIdx].traversalId == lp->traversalId)
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

_AFXINL void __cdecl AfxIKUpdate(afxNat LinkCountInit, afxNat EEBoneIdx, afxReal const desiredPos[3], afxNat iterCnt, awxSkeleton skl, afxReal const ModelRootTransform[4][4], awxPose* lp, awxWorldPose* wp)
{
    // Should be compatible with void IKUpdate(int LinkCountInit, int EEBoneIndex, const float *DesiredPosition3, int IterationCount, const skeleton *Skeleton, const float *ModelRootTransform, local_pose *LocalPose, world_pose *WorldPose)

    afxNat Links[256];
    afxNat v9 = EEBoneIdx;
    afxInt v10 = 0;
    afxNat v11 = 0;
    afxNat LinkCount = 0;

    if (LinkCountInit >= 0)
    {
        awxSkeletonBone* v12 = skl->bones;

        do
        {
            Links[v11] = v9;
            v9 = v12[v9].parentIdx;
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

                    t->flags |= afxTransformFlags_ORIENTATION;
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

_AFXINL afxBool IKUpdate2Bone(afxNat EEBoneIndex, afxReal const desiredPos[3], afxReal const restrictedMovPlaneNormal[3], awxSkeleton skl, afxReal const modelRootTransform[4][4], awxPose* lp, awxWorldPose* wp, afxReal hyperExtStart, afxReal hyperExtScale)
{
    // Should be compatible with char IKUpdate2Bone(int EEBoneIndex, const float *DesiredPosition3, const float *RestrictedMovementPlaneNormal3, const skeleton *Skeleton, const float *ModelRootTransform, local_pose *LocalPose, world_pose *WorldPose, float HyperExtensionStart, float HyperExtensionScale)
    return 0;
}

_AFXINL void BuildWorldPoseNoCompositeLOD(awxSkeleton skl, afxNat firstArt, afxNat artCnt, afxNat firstValidLocalArt, afxNat validLocalArtCnt, awxPose const *lp, afxReal const offset[4][4], awxWorldPose *wp)
{
    // Should be compatible with void BuildWorldPoseNoCompositeLOD(const skeleton *Skeleton, int FirstBone, int BoneCount, int FirstValidLocalBone, int ValidLocalBoneCount, const granny::local_pose *LocalPose, const float *Offset4x4, world_pose *Result)

    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AfxAssertRange(skl->boneCnt, firstArt, artCnt);
    AfxAssertRange(wp->xformCnt, firstArt, artCnt);
    AfxAssertRange(lp->cap, firstArt, artCnt);

    if (!offset)
        offset = AFX_M4D_IDENTITY;
    
    for (afxNat i = 0; i < artCnt; i++)
    {
        awxSkeletonBone *bone = &skl->bones[firstArt + i];
        afxTransform *t = &lp->xforms[firstArt + i].xform;
        const afxV4d *parentW = bone->parentIdx == AFX_INVALID_INDEX ? offset : (const afxV4d *)(wp->world[bone->parentIdx]);

        if (validLocalArtCnt <= 0)
            t = &bone->local;

        BWPNC_Dispatch(t, parentW, wp->world[firstArt + i]);
        --validLocalArtCnt;
    }
}

_AFXINL void BuildWorldPoseLOD(awxSkeleton skl, afxNat firstArt, afxNat artCnt, afxNat firstValidLocalArt, afxNat validLocalArtCnt, awxPose const *lp, afxReal const offset[4][4], awxWorldPose *wp)
{
    // Should be compatible with void BuildWorldPoseLOD(const skeleton *Skeleton, int FirstBone, int BoneCount, int FirstValidLocalBone, int ValidLocalBoneCount, const local_pose *LocalPose, const float *Offset4x4, world_pose *Result)

    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AfxAssertRange(skl->boneCnt, firstArt, artCnt);
    AfxAssertRange(lp->cap, firstArt, artCnt);
    AfxAssertRange(wp->xformCnt, firstArt, artCnt);

    if (wp->composite)
    {
        if (!offset)
            offset = AFX_M4D_IDENTITY;
        
        for (afxNat i = 0; i < artCnt; i++)
        {
            awxSkeletonBone *bone = &skl->bones[firstArt + i];
            afxTransform *t = &lp->xforms[firstArt + i].xform;
            const afxV4d *parentW = bone->parentIdx == AFX_INVALID_INDEX ? offset : (const afxV4d *)(wp->world[bone->parentIdx]);

            if (validLocalArtCnt <= 0)
                t = &bone->local;

            BWP_Dispatch(t, parentW, bone->iw, wp->composite[firstArt + i], wp->world[firstArt + i]);
        }
    }
    else
    {
        BuildWorldPoseNoCompositeLOD(skl, firstArt, artCnt, firstValidLocalArt, validLocalArtCnt, lp, offset, wp);
    }
}

_AFX void AfxComputeSkeletonPose(awxSkeleton skl, afxNat firstArt, afxNat artCnt, awxPose const *lp, afxReal const offset[4][4], awxWorldPose *wp)
{
    // Should be compatible with void BuildWorldPose(const skeleton *Skeleton, int FirstBone, int BoneCount, const local_pose *LocalPose, const float *Offset4x4, world_pose *Result)

    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AfxAssertRange(skl->boneCnt, firstArt, artCnt);
    AfxAssertRange(lp->cap, firstArt, artCnt);
    BuildWorldPoseLOD(skl, firstArt, artCnt, firstArt, artCnt, lp, offset, wp);
}

_AFX void AfxComputeSkeletonRestWorldPose(awxSkeleton skl, afxNat firstArt, afxNat artCnt, afxReal const offset[4][4], awxWorldPose *wp)
{
    // Should be compatible with void BuildRestWorldPose(const skeleton *Skeleton, int FirstBone, int BoneCount, const float *Offset4x4, world_pose *Result)

    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AfxAssertRange(skl->boneCnt, firstArt, artCnt);
    AfxAssertRange(wp->xformCnt, firstArt, artCnt);

    if (!offset)
        offset = AFX_M4D_IDENTITY;

    for (afxNat i = 0; i < artCnt; i++)
    {
        awxSkeletonBone *bone = &skl->bones[firstArt + i];
        afxNat parentIdx = bone->parentIdx;
        const afxV4d *parentW = AFX_INVALID_INDEX == parentIdx ? offset : (const afxV4d *)(wp->world[parentIdx]);

        if (parentIdx != AFX_INVALID_INDEX)
        {
            AfxAssertRange(skl->boneCnt, parentIdx, 1);
        }

        if (wp->composite)
        {
            BWP_Dispatch(&bone->local, parentW, bone->iw, wp->composite[firstArt + i], wp->world[firstArt + i]);
        }
        else
        {
            BWPNC_Dispatch(&bone->local, parentW, wp->world[firstArt + i]);
        }
    }
}

_AFX void AfxComputeSkeletonRestLocalPose(awxSkeleton skl, afxNat firstArt, afxNat artCnt, awxPose *lp)
{
    // Should be compatible with void BuildRestLocalPose(const skeleton *Skeleton, int FirstBone, int BoneCount, local_pose *LocalPose)

    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AfxAssertRange(skl->boneCnt, firstArt, artCnt);
    AfxAssertRange(lp->cap, firstArt, artCnt);

    for (afxNat i = firstArt + artCnt; i-- > 0;)
    {
        AfxAssertRange(skl->boneCnt, i, 1);
        awxPoseTransform *t = &lp->xforms[i];
        t->cnt = 1;
        t->weight = 1.0;
        AfxCopyTransform(&t->xform, &skl->bones[i].local); // ?
    }
}

_AFX awxSkeletonBone* AfxGetBone(awxSkeleton skl, afxNat boneIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);    
    AfxAssert(boneIdx < skl->boneCnt);
    awxSkeletonBone *bone = &skl->bones[boneIdx];
    //AfxAssertObjects(1, &nod, afxFcc_NOD);
    return bone;
}

_AFX afxNat AfxCountBones(awxSkeleton skl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    return skl->boneCnt;
}

_AFX afxBool AfxFindBone(awxSkeleton skl, afxString const* id, afxNat *boneIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);

    for (afxNat i = 0; i < skl->boneCnt; i++)
    {
        if (0 == AfxCompareStringCi(&skl->bones[i].id.str, id))
        {
            *boneIdx = i;
            return TRUE;
        }
    }

    *boneIdx = AFX_INVALID_INDEX;
    return FALSE;
}

_AFX afxString const* AfxGetSkeletonId(awxSkeleton skl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    return &skl->id.str;
}

_AFX void AfxLocalPoseFromWorldPose(awxSkeleton skl, awxPose *lp, awxWorldPose const* wp, afxReal const offset[4][4], afxNat firstBone, afxNat boneCnt)
{
    // Should be compatible with void LocalPoseFromWorldPose(const skeleton *Skeleton, local_pose *Result, const world_pose *WorldPose, const float *Offset4x4, int FirstBone, int BoneCount)

    afxError err = AFX_ERR_NONE;
    AfxAssert(lp);
    AfxAssert(wp);
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AfxAssertRange(skl->boneCnt, firstBone, boneCnt);
    AfxAssertRange(lp->cap, firstBone, boneCnt);
    AfxAssertRange(wp->xformCnt, firstBone, boneCnt);

    if (!offset)
        offset = AFX_M4D_IDENTITY;

    awxSkeletonBone* bones = skl->bones;

    for (afxNat i = firstBone + boneCnt; i-- > 0;)
    {
        AfxAssertRange(skl->boneCnt, i, 1);
        afxNat parentIdx = bones[i].parentIdx;
        afxM4d InverseParent;

        if (parentIdx == AFX_INVALID_INDEX) AfxInverseAtm4(InverseParent, offset);
        else
        {
            AfxAssertRange(skl->boneCnt, parentIdx, 1);
            AfxInverseAtm4(InverseParent, wp->world[parentIdx]);
        }

        afxM4d Local;
        AfxMultiplyTransposedAtm4(Local, InverseParent, wp->world[i]);
        
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

_AFX void AfxLocalPoseFromWorldPoseNoScale(awxSkeleton skl, awxPose* lp, awxWorldPose const* wp, afxReal const offset[4][4], afxNat firstBone, afxNat boneCnt)
{
    // Should be compatible with void LocalPoseFromWorldPoseNoScale(const skeleton *Skeleton, local_pose *Result, const world_pose *WorldPose, const float *Offset4x4, int FirstBone, int BoneCount)

    afxError err = AFX_ERR_NONE;
    AfxAssert(wp);
    AfxAssert(lp);
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AfxAssertRange(skl->boneCnt, firstBone, boneCnt);
    AfxAssertRange(lp->cap, firstBone, boneCnt);
    AfxAssertRange(wp->xformCnt, firstBone, boneCnt);

    if (!offset)
        offset = AFX_M4D_IDENTITY;

    for (afxNat i = firstBone + boneCnt; i-- > 0;)
    {
        afxNat parentIdx = skl->bones[i].parentIdx;
        afxM4d InverseParent;

        if (parentIdx == AFX_INVALID_INDEX) AfxInverseAtm4(InverseParent, offset);
        else
        {
            AfxAssertRange(skl->boneCnt, parentIdx, 1);
            AfxInverseAtm4(InverseParent, wp->world[parentIdx]);
        }

        afxM4d Local;
        AfxMultiplyTransposedAtm4(Local, wp->world[i], InverseParent);
        afxTransform* t = AfxGetPoseTransform(lp, i);

        AfxCopyV3d(t->position, Local[3]);
        afxM3d Rotation;
        AfxSetTransposedM3d(Rotation, Local[0], Local[1], Local[2]);
        AfxQuatFromM3d(t->orientation, Rotation);
        AfxNormalizeQuat(t->orientation, t->orientation);
        AfxResetM3d(t->scaleShear);
        t->flags = afxTransformFlags_ORIGIN | afxTransformFlags_ORIENTATION;
    }
}

_AFX void AfxGetWorldMatrixFromLocalPose(awxSkeleton skl, afxNat artIdx, awxPose const* lp, afxReal const offset[4][4], afxReal m[4][4], afxNat const* sparseBoneArray, afxNat const* sparseBoneArrayReverse)
{
    // Should be compatible with void GetWorldMatrixFromLocalPose(const skeleton *Skeleton, int BoneIndex, const local_pose *LocalPose, const float *Offset4x4, float *Result4x4, const int *SparseBoneArray, const int *SparseBoneArrayReverse)

    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    AfxAssertRange(lp->cap, artIdx, 1);

    AfxResetM4d(m);

    if (artIdx != AFX_INVALID_INDEX)
    {
        for (afxNat i = artIdx; i != AFX_INVALID_INDEX; i = skl->bones[i].parentIdx)
        {
            AfxAssertRange(skl->boneCnt, i, 1);
            afxTransform* t = AfxGetPoseTransform(lp, sparseBoneArrayReverse ? sparseBoneArrayReverse[i] : i);
            afxM4d tmp, tmp2;
            AfxComposeTransformM4d(t, tmp);
            AfxMultiplyTransposedAtm4(tmp2, m, tmp);
            AfxCopyM4d(m, tmp2);
        }
    }

    if (offset)
    {
        afxM4d tmp2;
        AfxMultiplyTransposedAtm4(tmp2, offset, m);
        AfxCopyM4d(m, tmp2);
    }
}

_AFX void AfxGetAttachmentOffset(awxSkeleton skl, afxNat artIdx, awxPose const* lp, afxReal const offset[4][4], afxReal m[4][4], afxNat const* sparseArtArray, afxNat const* sparseArtArrayReverse)
{
    // Should be compatible with void GetAttachmentOffset(const skeleton *Skeleton, int BoneIndex, const local_pose *LocalPose, const float *Offset4x4, float *Result4x4, const int *SparseBoneArray, const int *SparseBoneArrayReverse)

    afxM4d tmp;

    AfxGetWorldMatrixFromLocalPose(skl, artIdx, lp, offset, tmp, sparseArtArray, sparseArtArrayReverse);
    AfxInverseAtm4(m, tmp);
}

_AFX afxNat AfxCountBonesForLod(awxSkeleton skl, afxReal allowedErr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    afxNat rslt = 0;

    if (allowedErr == 0.0 || !skl->lodType) rslt = skl->boneCnt;
    else
    {
        afxReal errEnd, errScaler;
        afxSimulation sim = AfxGetObjectProvider(skl);
        AfxComputeAllowedErrorValues(sim, allowedErr, &errEnd, &errScaler);

        afxNat boneCnt = skl->boneCnt;
        awxSkeletonBone* bones = skl->bones;
        
        if (boneCnt >= 4)
        {
            while (bones[rslt + 0].lodError >= errEnd)
            {
                if (bones[rslt + 1].lodError < errEnd)
                {
                    ++rslt;
                    break;
                }

                if (bones[rslt + 2].lodError < errEnd)
                {
                    rslt += 2;
                    break;
                }

                if (bones[rslt + 3].lodError < errEnd)
                {
                    rslt += 3;
                    break;
                }

                rslt += 4;

                if (rslt >= boneCnt - 3)
                    break;
            }
        }

        for (; rslt < boneCnt; rslt++)
        {
            if (bones[rslt].lodError < errEnd)
                break;
        }
    }
    return rslt;
}

_AFX afxError _AfxSklDtor(awxSkeleton skl)
{
    AfxEntry("skl=%p", skl);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);

    afxSimulation sim = AfxGetObjectProvider(skl);
    afxMmu mmu = AfxSimulationGetMemory(sim);

    for (afxNat i = 0; i < skl->boneCnt; i++)
        AfxDeallocateString(&skl->bones[i].id);

    if (skl->bones)
        AfxDeallocate(mmu, skl->bones);

    AfxDeallocateString(&skl->id);

    return err;
}

#if 0
_AFXINL void _AfxSklCtorBubbleSortOnLod(awxSkeletonBlueprint *Builder, awxSkeletonBone *Bones, afxNat* remappingTable)
{
    // void __cdecl BubbleSortOnLOD(skeleton_builder *Builder, bone *Bones, int *RemappingTable)

    awxSkeletonBone Temp; // [esp+28h] [ebp-98h]

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
    
    afxByte* v11 = AfxRequestArenaUnit(&SortArena, sizeof(awxSkeletonBone) * Builder->bones.cnt);

    for (afxNat i = Builder->bones.cnt; i--> 0;)
    {
        Bones[i].name;
    }

    awxSkeletonBone* v12 = Bones;

    if (152 * Builder->bones.cnt)
    {
        afxNat v13 = v11 - (afxByte *)Bones;
        afxNat v14 = 152 * Builder->bones.cnt;

        do
        {
            *((afxByte *)&v12->name + v13) = v12->name;
            v12 = (awxSkeletonBone *)((char *)v12 + 1);
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
                awxSkeletonBone *v18 = Bones + 1;
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
                        memcpy(&v18[-1], v18, sizeof(awxSkeletonBone));
                        memcpy(v18, &Temp, sizeof(awxSkeletonBone));
                    }

                    if (v18->lodError < (double)v18[1].lodError)
                    {
                        afxNat v21 = *(v19 - 1);
                        *(v19 - 1) = *v19;
                        *v19 = v21;
                        memcpy(&Temp, v18, sizeof(Temp));
                        memcpy(v18, &v18[1], sizeof(awxSkeletonBone));
                        memcpy(&v18[1], &Temp, sizeof(awxSkeletonBone));
                    }

                    if (v18[1].lodError < (double)v18[2].lodError)
                    {
                        afxNat v22 = *v19;
                        *v19 = v19[1];
                        v19[1] = v22;
                        memcpy(&Temp, &v18[1], sizeof(Temp));
                        memcpy(&v18[1], &v18[2], sizeof(awxSkeletonBone));
                        memcpy(&v18[2], &Temp, sizeof(awxSkeletonBone));
                    }

                    if (v18[2].lodError < (double)v18[3].lodError)
                    {
                        afxNat v23 = v19[1];
                        v19[1] = v19[2];
                        v19[2] = v23;
                        memcpy(&Temp, &v18[2], sizeof(Temp));
                        memcpy(&v18[2], &v18[3], sizeof(awxSkeletonBone));
                        memcpy(&v18[3], &Temp, sizeof(awxSkeletonBone));
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
                awxSkeletonBone *v24 = &Bones[v16 + 1];

                do
                {
                    if (v24[-1].lodError < (double)v24->lodError)
                    {
                        afxNat v25 = NewToOldMap[v16];
                        NewToOldMap[v16] = NewToOldMap[v16 + 1];
                        NewToOldMap[v16 + 1] = v25;
                        memcpy(&Temp, &v24[-1], sizeof(Temp));
                        memcpy(&v24[-1], v24, sizeof(awxSkeletonBone));
                        memcpy(v24, &Temp, sizeof(awxSkeletonBone));
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

_AFXINL void _AfxSklCtorWriteBone(awxSkeletonBuilder const* sklb, awxSkeleton skl, afxNat* boneWrittenMap, afxNat inBoneIdx, afxNat* outBoneIdx)
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
            AfxAssertRange(skl->boneCnt, parentIdx, 1);

            _AfxSklCtorWriteBone(sklb, skl, boneWrittenMap, parentIdx, outBoneIdx);
            parentIdxWritten = boneWrittenMap[parentIdx];
        }

        skl->bones[*outBoneIdx].parentIdx = parentIdxWritten;
        skl->bones[*outBoneIdx].lodError = sklb->bones[inBoneIdx].lodError;
        
        AfxResetTransform(&skl->bones[*outBoneIdx].local);

        AfxCopyTransform(&skl->bones[*outBoneIdx].local, &sklb->bones[inBoneIdx].local);
        
        AfxResetM4d(skl->bones[*outBoneIdx].iw);
        
        AfxCopyM4d(skl->bones[*outBoneIdx].iw, sklb->bones[inBoneIdx].iw);
        
        skl->bones[*outBoneIdx].extendedData = NIL;
        
        skl->bones[*outBoneIdx].extendedData = sklb->bones[inBoneIdx].extendedData;

        AfxResetString(&skl->bones[*outBoneIdx].id.str);
        AfxCloneString(&skl->bones[*outBoneIdx].id, &sklb->bones[inBoneIdx].id.str);

        //if (sklb->IndexRemapping)
            //sklb->IndexRemapping(data, inBoneIdx, *outBoneIdx);

        boneWrittenMap[inBoneIdx] = (*outBoneIdx)++;
    }
}

_AFX afxError _AfxSklCtor(awxSkeleton skl, afxCookie const *cookie)
{
    AfxEntry("skl=%p", skl);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &skl, afxFcc_SKL);

    afxSimulation sim = cookie->udd[0];
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    awxSkeletonBuilder const *sklb = cookie->udd[1];
    AfxAssert(sklb);

    afxMmu mmu = AfxSimulationGetMemory(sim);

    afxNat boneCnt = sklb->boneCnt, lodType = sklb->lodType;    
    AfxAssert(boneCnt);
    skl->bones = NIL;

    if (boneCnt && !(skl->bones = AfxAllocate(mmu, sizeof(skl->bones[0]), boneCnt, 0, AfxHint()))) AfxThrowError();
    else
    {
        afxNat *boneWrittenMap = AfxAllocate(mmu, sizeof(boneWrittenMap[0]), boneCnt, 0, AfxHint());
        AfxAssert(boneWrittenMap);

        for (afxNat i = 0; i < boneCnt; ++i)
            boneWrittenMap[i] = (afxNat)AFX_INVALID_INDEX;

        // estimate lod

        skl->boneCnt = boneCnt;
        afxNat outBoneIdx = 0;

        for (afxNat i = 0; i < skl->boneCnt; i++)
        {
            if (AFX_INVALID_INDEX == sklb->bones[i].parentIdx)
                _AfxSklCtorWriteBone(sklb, skl, boneWrittenMap, i, &outBoneIdx);
        }

        for (afxNat i = 0; i < skl->boneCnt; i++)
        {
            if (AFX_INVALID_INDEX != sklb->bones[i].parentIdx)
                _AfxSklCtorWriteBone(sklb, skl, boneWrittenMap, i, &outBoneIdx);
        }

        //_AfxSklCtorBubbleSortOnLOD(blueprint, skl->bones, remapTable);
        skl->lodType = lodType;

        //skl->boneCnt = boneCnt;
        AfxAssert(skl->boneCnt == boneCnt);

        AfxCloneString(&skl->id, &sklb->id.str.str);

        AfxDeallocate(mmu, boneWrittenMap);

        if (err && skl->bones)
            AfxDeallocate(mmu, skl->bones);
    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxBuildSkeletons(afxSimulation sim, afxNat cnt, awxSkeletonBuilder const sklb[], awxSkeleton skl[])
{
    //AfxEntry("sim=%p,name=%.*s,boneCnt=%u,boneName=%p,boneParentIdx=%p,bonePos=%p,boneOrient=%p,boneScale=%p", sim, AfxPushString(name), boneCnt, boneName, boneParentIdx, bonePos, boneOrient, boneScale);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AfxAssert(sklb);
    AfxAssert(cnt);
    AfxAssert(skl);

    if (AfxAcquireObjects(AfxGetSkeletonClass(sim), cnt, (afxObject*)skl, (void const*[]) { sim, (void*)sklb }))
        AfxThrowError();

    return err;
}

_AFX void AfxTransformSkeletons(afxReal const ltm[3][3], afxReal const iltm[3][3], afxReal const atv[4], afxReal affineTol, afxReal linearTol, afxNat cnt, awxSkeleton skl[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(ltm);
    AfxAssert(iltm);
    AfxAssert(atv);
    AfxAssert(cnt);
    AfxAssert(skl);
    (void)affineTol;
    (void)linearTol;

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxAssertObjects(1, &skl[i], afxFcc_SKL);
        afxNat boneCnt = skl[i]->boneCnt;

        for (afxNat j = 0; j < boneCnt; j++)
        {
            awxSkeletonBone* joint = &(skl[i]->bones[j]);

            AfxAssimilateTransforms(ltm, iltm, atv, 1, &joint->local, &joint->local);
            AfxAssimilateAtm4(ltm, iltm, atv, 1, &joint->iw, &joint->iw);
        }
    }
}

_AFX afxClassConfig _AfxSklClsConfig =
{
    .fcc = afxFcc_SKL,
    .name = "Skeleton",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(awxSkeleton)),
    .mmu = NIL,
    .ctor = (void*)_AfxSklCtor,
    .dtor = (void*)_AfxSklDtor
};

////////////////////////////////////////////////////////////////////////////////
// BUILDING OPERATIONS                                                        //
////////////////////////////////////////////////////////////////////////////////

_AFXINL afxError AfxBeginSkeletonBuilding(awxSkeletonBuilder* sklb, afxNat boneCnt, afxString const* id, afxNat lodType)
{
    afxError err = NIL;
    AfxAssert(sklb);
    
    AfxAssignFcc(sklb, afxFcc_SKLB);
    AfxAssert(boneCnt);
    sklb->boneCnt = boneCnt;
    sklb->bones = AfxAllocate(NIL, sizeof(sklb->bones[0]), boneCnt, 0, AfxHint());

    for (afxNat i = 0; i < boneCnt; i++)
    {
        sklb->bones[i].extendedData = NIL;
        AfxResetTransform(&sklb->bones[i].local);
        sklb->bones[i].lodError = -1.0;
        sklb->bones[i].parentIdx = AFX_INVALID_INDEX;
        AfxResetM4d(sklb->bones[i].iw);
        AfxResetBufferedString(&sklb->bones[i].id);
    }

    AfxMakeFixedString32(&sklb->id, id);
    sklb->lodType = lodType;
    return err;
}

_AFXINL void AfxEndSkeletonBuilding(awxSkeletonBuilder* sklb)
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

_AFXINL void AfxResetBoneWithIw(awxSkeletonBuilder const* sklb, afxNat boneIdx, afxString const* id, afxNat parentIdx, afxTransform const* lt, afxReal const iw[4][4])
{
    afxError err = NIL;
    AfxAssertType(sklb, afxFcc_SKLB);
    AfxAssertRange(sklb->boneCnt, boneIdx, 1);
    sklb->bones[boneIdx].lodError = -1.0;
    sklb->bones[boneIdx].parentIdx = AFX_INVALID_INDEX;
    AfxCopyTransform(&sklb->bones[boneIdx].local, lt);
    AfxCopyM4d(sklb->bones[boneIdx].iw, iw);
    AfxCloneString(&sklb->bones[boneIdx].id, id);
}

_AFXINL void AfxResetBone(awxSkeletonBuilder const* sklb, afxNat boneIdx, afxString const* id, afxNat parentIdx, afxTransform const* lt, afxTransform const* wt)
{
    afxError err = NIL;
    AfxAssertType(sklb, afxFcc_SKLB);
    AfxAssertRange(sklb->boneCnt, boneIdx, 1);
    AfxAssert(id && !AfxStringIsEmpty(id));
    afxM4d w, iw;
    AfxComposeTransformM4d(wt, w);
    AfxInverseM4d(iw, w);
    AfxResetBoneWithIw(sklb, boneIdx, id, parentIdx, lt, iw);
}
