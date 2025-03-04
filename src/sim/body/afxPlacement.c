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

#define _ASX_PLACEMENT_C
//#define _ASX_POSE_C
//#define _ASX_MODEL_C
#include "../impl/asxImplementation.h"

_ASX afxUnit AfxGetPlacementCapacity(afxPlacement plce)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_PLCE, 1, &plce);
    return plce->matCnt;
}

_ASX afxM4d* _AsxPlceGetWorldArray(afxPlacement plce, afxUnit base)
{
    // AfxPostureGetWorldMatrixArray

    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_PLCE, 1, &plce);
    AFX_ASSERT_RANGE(plce->matCnt, base, 1);
    return &plce->world[base];
}

_ASX afxM4d* _AsxPlceGetDeltaArray(afxPlacement plce, afxUnit base)
{
    // AfxPostureGetCompositeMatrixArray

    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_PLCE, 1, &plce);
    AFX_ASSERT_RANGE(plce->matCnt, base, 1);
    return &plce->delta[base];
}

_ASX afxM4d* AfxGetPlacementDeltas(afxPlacement plce, afxUnit baseArtIdx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_PLCE, 1, &plce);
    AFX_ASSERT_RANGE(plce->matCnt, baseArtIdx, 1);
    return &plce->delta[baseArtIdx];
}

_ASX afxM4d* AfxGetPlacementMatrices(afxPlacement plce, afxUnit baseArtIdx)
{
    //float (*__cdecl GetPosture4x4(const world_pose *Posture, int BoneIndex))[4]
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_PLCE, 1, &plce);
    AFX_ASSERT_RANGE(plce->matCnt, baseArtIdx, 1);
    return &plce->world[baseArtIdx];
}

_ASX void AfxBuildPlacement(afxPlacement plce, afxPose pose, afxModel skl, afxUnit baseJntIdx, afxUnit jntCnt, afxUnit baseReqJnt, afxUnit reqJntCnt, afxM4d const displace, afxBool skipDeltas)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &skl);
    afxUnit totalJntCnt = AsxCountJoints(skl, 0);
    AFX_ASSERT_RANGE(totalJntCnt, baseJntIdx, jntCnt);
    AFX_ASSERT_RANGE(plce->matCnt, baseJntIdx, jntCnt);

    if (!displace)
        displace = AFX_M4D_IDENTITY;

    afxM4d const* iw = _AsxMdlGetIwArray(skl, 0);
    afxUnit const* pi = _AsxMdlGetPiArray(skl, 0);
    afxTransform const* lt = _AsxMdlGetLtArray(skl, 0);
    afxM4d* w = plce->world;
    afxM4d* delta = plce->delta;

    if (!pose)
    {
        // compute the rest pose whether a attitude is not specified.
        // rest pose is computed for every joint ignoring baseReqJnt/reqJntCnt;
        // rest pose always compute composite delta matrices.
        // Should be compatible with void BuildRestPosture(const skeleton *Skeleton, int FirstBone, int BoneCount, const float *Offset4x4, world_pose *Result)
        // AfxSklComputeRestPosture(skl, baseJntIdx, jntCnt, offset, pose);

        for (afxUnit i = 0; i < jntCnt; i++)
        {
            afxUnit jntIdx = baseJntIdx + i;
            AFX_ASSERT_RANGE(totalJntCnt, jntIdx, 1);

            afxV4d const* parent;
            afxUnit parentIdx = pi[jntIdx];
            if (parentIdx == AFX_INVALID_INDEX) parent = displace;
            else
            {
                AFX_ASSERT_RANGE(totalJntCnt, parentIdx, 1);
                parent = w[parentIdx];
            }

            if (delta) // always computed if present for rest pose.
            {
                BWP_Dispatch(&lt[jntIdx], iw[jntIdx], delta[jntIdx], w[jntIdx], parent);
            }
            else
            {
                BWPNC_Dispatch(&lt[jntIdx], parent, w[jntIdx]);
            }
        }
    }
    else
    {
        afxUnit poseCap = AsxGetPoseCapacity(pose);
        AFX_ASSERT_RANGE(poseCap, baseJntIdx, jntCnt);
        asxArticulation const* pa = _AsxPoseGetPaArray(pose, 0);

        if (skipDeltas || !delta)
        {
            // Should be compatible with void BuildPostureNoCompositeLOD(const skeleton *Skeleton, int FirstBone, int BoneCount, int FirstValidLocalBone, int ValidLocalBoneCount, const granny::local_pose *LocalAttitude, const float *Offset4x4, world_pose *Result)
            // AfxSklComputePostureNoCompositeLod(skl, baseJntIdx, jntCnt, baseReqJnt, reqJntCnt, atti, offset, pose);

            for (afxUnit i = 0; i < jntCnt; i++)
            {
                afxUnit jntIdx = baseJntIdx + i;
                AFX_ASSERT_RANGE(totalJntCnt, jntIdx, 1);

                afxV4d const* parent;
                afxUnit parentIdx = pi[jntIdx];
                if (parentIdx == AFX_INVALID_INDEX) parent = displace;
                else
                {
                    AFX_ASSERT_RANGE(totalJntCnt, parentIdx, 1);
                    parent = w[parentIdx];
                }

                afxTransform const *t = &pa[jntIdx].xform;

                if (reqJntCnt <= 0)
                    t = &lt[jntIdx];

                BWPNC_Dispatch(t, parent, w[jntIdx]);
                --reqJntCnt;
            }
        }
        else
        {
            // Should be compatible with void BuildPostureLOD(const skeleton *Skeleton, int FirstBone, int BoneCount, int FirstValidLocalBone, int ValidLocalBoneCount, const local_pose *LocalAttitude, const float *Offset4x4, world_pose *Result)
            // AfxSklComputePostureLod(skl, baseJntIdx, jntCnt, baseReqJnt, reqJntCnt, atti, offset, pose);

            for (afxUnit i = 0; i < jntCnt; i++)
            {
                afxUnit jntIdx = baseJntIdx + i;
                AFX_ASSERT_RANGE(totalJntCnt, jntIdx, 1);

                afxV4d const* parent;
                afxUnit parentIdx = pi[jntIdx];
                if (parentIdx == AFX_INVALID_INDEX) parent = displace;
                else
                {
                    AFX_ASSERT_RANGE(totalJntCnt, parentIdx, 1);
                    parent = w[parentIdx];
                }

                afxTransform const *t = &pa[jntIdx].xform;

                if (reqJntCnt <= 0)
                    t = &lt[jntIdx];

                BWP_Dispatch(t, iw[jntIdx], delta[jntIdx], w[jntIdx], parent);
                --reqJntCnt;
            }
        }
    }
}

_ASX void AfxRebuildPose(afxPlacement plce, afxModel skl, afxUnit baseJntIdx, afxUnit jntCnt, afxM4d const displace, afxBool rigid, afxPose pose)
{
    // Computes the local pose from the world pose.
    // If rigid is argumented as TRUE, the local pose will not include any scale/shear.

    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &skl);
    afxUnit totalJntCnt = AsxCountJoints(skl, 0);
    AFX_ASSERT_RANGE(totalJntCnt, baseJntIdx, jntCnt);
    afxUnit poseCap = AsxGetPoseCapacity(pose);
    AFX_ASSERT_RANGE(poseCap, baseJntIdx, jntCnt);
    AFX_ASSERT_RANGE(plce->matCnt, baseJntIdx, jntCnt);
    AFX_ASSERT(pose);
    AFX_ASSERT(plce);

    if (!displace)
        displace = AFX_M4D_IDENTITY;

    afxUnit const* pi = _AsxMdlGetPiArray(skl, 0);

    if (rigid)
    {
        //AfxSklLocalAttitudeFromPostureNoScale(skl, atti, pose, offset, baseJntIdx, jntCnt);
        // Should be compatible with void LocalAttitudeFromPostureNoScale(const skeleton *Skeleton, local_pose *Result, const world_pose *Posture, const float *Offset4x4, int FirstBone, int BoneCount)

        for (afxUnit i = jntCnt; i-- > 0;)
        {
            afxUnit jointIdx = baseJntIdx + i;

            afxM4d InverseParent;
            afxUnit parentIdx = pi[jointIdx];
            if (parentIdx == AFX_INVALID_INDEX) AfxM4dInvertAffine(InverseParent, displace);
            else
            {
                AFX_ASSERT_RANGE(totalJntCnt, parentIdx, 1);
                AfxM4dInvertAffine(InverseParent, plce->world[parentIdx]);
            }

            afxM4d Local;
            AfxM4dMultiplyAffine(Local, plce->world[jointIdx], InverseParent);
            afxTransform* t = AsxGetPoseTransform(pose, jointIdx);

            AfxV3dCopy(t->position, Local[3]);
            afxM3d Rotation;
            AfxM3dSet(Rotation, Local[0], Local[1], Local[2]);
            AfxQuatRotationFromM3d(t->orientation, Rotation);
            AfxQuatNormalize(t->orientation, t->orientation);
            AfxM3dReset(t->scaleShear);
            t->flags = afxTransformFlag_TRANSLATED | afxTransformFlag_ROTATED;
        }
    }
    else
    {
        // Should be compatible with void LocalAttitudeFromPosture(const skeleton *Skeleton, local_pose *Result, const world_pose *Posture, const float *Offset4x4, int FirstBone, int BoneCount)
        //AfxSklLocalAttitudeFromPosture(skl, atti, pose, offset, baseJntIdx, jntCnt);

        for (afxUnit i = jntCnt; i-- > 0;)
        {
            afxUnit jointIdx = baseJntIdx + i;
            AFX_ASSERT_RANGE(totalJntCnt, jointIdx, 1);

            afxM4d InverseParent;
            afxUnit parentIdx = pi[jointIdx];
            if (parentIdx == AFX_INVALID_INDEX) AfxM4dInvertAffine(InverseParent, displace);
            else
            {
                AFX_ASSERT_RANGE(totalJntCnt, parentIdx, 1);
                AfxM4dInvertAffine(InverseParent, plce->world[parentIdx]);
            }

            afxM4d Local;
            AfxM4dMultiplyAffine(Local, plce->world[jointIdx], InverseParent);

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
            afxTransform* t = AsxGetPoseTransform(pose, jointIdx);
            AfxSetTransform(t, Position, Orientation, Scale, TRUE);
        }
    }
}

_ASX void AfxBuildCompositeMatrices(afxPlacement plce, afxModel skl, afxUnit baseJnt, afxUnit cnt, afxBool /*3x4*/transposed, afxM4d matrices[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_PLCE, 1, &plce);
    AFX_ASSERT_RANGE(plce->matCnt, baseJnt, cnt);
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &skl);
    afxUnit totalJntCnt = AsxCountJoints(skl, 0);
    AFX_ASSERT_RANGE(totalJntCnt, baseJnt, cnt);
    AFX_ASSERT(matrices);

    afxM4d const* iw = _AsxMdlGetIwArray(skl, 0);
    afxM4d const* w = plce->world;

    if (transposed)
    {
        // Should be compatible with BuildCompositeBufferTransposed(skl, firstBone, cnt, wp, compositeBuffer)

        for (afxUnit i = 0; i < cnt; i++)
        {
            afxUnit jntIdx = baseJnt + i;
            BuildSingleCompositeFromWorldPoseTranspose_Generic(iw[jntIdx], w[jntIdx], matrices[i]);
        }
    }
    else
    {
        // Should be compatible with BuildCompositeBuffer(skl, firstBone, cnt, wp, compositeBuffer)

        for (afxUnit i = 0; i < cnt; i++)
        {
            afxUnit jntIdx = baseJnt + i;
            BuildSingleCompositeFromWorldPose_Generic(iw[jntIdx], w[jntIdx], matrices[i]);
        }
    }
}

_ASX void AfxBuildIndexedCompositeMatrices(afxPlacement plce, afxModel skl, afxUnit cnt, afxUnit const jntMap[], afxBool /*3x4*/transposed, afxM4d matrices[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_PLCE, 1, &plce);
    afxUnit totalMatCnt = plce->matCnt;
    AFX_ASSERT_RANGE(totalMatCnt, 0, cnt);
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &skl);
    afxUnit totalJntCnt = AsxCountJoints(skl, 0);
    AFX_ASSERT_RANGE(totalJntCnt, 0, cnt);
    AFX_ASSERT(matrices);
    AFX_ASSERT(jntMap);

    afxM4d const* iw = _AsxMdlGetIwArray(skl, 0);
    afxM4d const* w = plce->world;

    if (transposed)
    {
        // void AfxSklBuildIndexedCompositeBufferTransposed(afxSkeleton skl, afxPlacement const posb, afxUnit const* indices, afxUnit idxCnt, afxReal buffer[][3][4])
        // AfxSklBuildIndexedCompositeBufferTransposed(skl, pose, mdl->rigs[rigIdx].biasToJointMap, cnt, buffer);

        for (afxUnit i = 0; i < cnt; i++)
        {
            afxUnit jntIdx = jntMap[i];
            AFX_ASSERT_RANGE(totalJntCnt, jntIdx, 1);
            AFX_ASSERT_RANGE(totalMatCnt, jntIdx, 1);
            BuildSingleCompositeFromWorldPoseTranspose_Generic(iw[jntIdx], w[jntIdx], matrices[i]);
        }
    }
    else
    {
        // void AfxSklBuildIndexedCompositeBuffer(afxSkeleton skl, afxPlacement const wp, afxUnit const* indices, afxUnit idxCnt, afxM4d buffer[])
        // AfxSklBuildIndexedCompositeBuffer(skl, pose, mdl->rigs[rigIdx].biasToJointMap, cnt, buffer);

        // joints used to be biasToJointMap of model's mesh rig.

        for (afxUnit i = 0; i < cnt; i++)
        {
            afxUnit jntIdx = jntMap[i];
            AFX_ASSERT_RANGE(totalJntCnt, jntIdx, 1);
            AFX_ASSERT_RANGE(totalMatCnt, jntIdx, 1);
            BuildSingleCompositeFromWorldPose_Generic(iw[jntIdx], w[jntIdx], matrices[i]);
        }
    }
}

_ASX afxError _AsxPosbDtorCb(afxPlacement plce)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_PLCE, 1, &plce);

    if (plce->delta)
        AfxDeallocate((void**)&plce->delta, AfxHere());

    if (plce->world)
        AfxDeallocate((void**)&plce->world, AfxHere());

    return err;
}

_ASX afxError _AsxPosbCtorCb(afxPlacement plce, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_PLCE, 1, &plce);

    afxSimulation sim = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxUnit artCnt = *(((afxUnit const*)(args[1])) + invokeNo);
    afxBool excludeComposite = args[2] ? *(((afxBool const*)(args[2])) + invokeNo) : TRUE;
    
    plce->matCnt = artCnt;
    AfxAllocate(plce->matCnt * sizeof(plce->world[0]), 0, AfxHere(), (void**)&plce->world);
    plce->delta = NIL;
    
    if ((!excludeComposite) && plce->matCnt)
        AfxAllocate(plce->matCnt * sizeof(plce->delta[0]), 0, AfxHere(), (void**)&plce->delta);

    return err;
}

_ASX afxClassConfig const _ASX_PLCE_CLASS_CONFIG =
{
    .fcc = afxFcc_PLCE,
    .name = "Placement",
    .desc = "Skeletal Placement Buffer",
    .fixedSiz = sizeof(AFX_OBJECT(afxPlacement)),
    .ctor = (void*)_AsxPosbCtorCb,
    .dtor = (void*)_AsxPosbDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ASX afxError AfxAcquirePlacements(afxSimulation sim, afxUnit cnt, afxUnit const artCnt[], afxBool const excludeComposite[], afxPlacement placements[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);

    afxClass* cls = (afxClass*)_AsxGetPlacementClass(sim);
    AFX_ASSERT_CLASS(cls, afxFcc_PLCE);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)placements, (void const*[]) { sim, artCnt, excludeComposite }))
        AfxThrowError();

    return err;
}
