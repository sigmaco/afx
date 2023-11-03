/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#define _AFX_MODEL_C
#define _AFX_MESH_C
#define _AFX_SIMULATION_C
#define _AFX_SKELETON_C
#include "afx/sim/modeling/afxModel.h"
#include "afx/sim/afxSimulation.h"
#include "afx/core/afxUri.h"
#include "afx/draw/afxDrawSystem.h"
#include "afx/math/afxMatrix.h"

_AFX afxBool AfxBoundMeshIsTransferred(afxModel mdl, afxNat slotIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->meshSlotCnt, slotIdx, 1);
    return mdl->meshSlots[slotIdx].srcSkl != mdl->skl;
}

_AFX afxSkeleton AfxGetBoundMeshOriginalSkeleton(afxModel mdl, afxNat slotIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->meshSlotCnt, slotIdx, 1);
    return mdl->meshSlots[slotIdx].srcSkl;
}

_AFX afxNat const* AfxGetBoundMeshBoneIndices(afxModel mdl, afxNat slotIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->meshSlotCnt, slotIdx, 1);
    return mdl->meshSlots[slotIdx].boneIndices;
}

_AFX afxNat const* AfxGetBoundMeshOriginalBoneIndices(afxModel mdl, afxNat slotIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->meshSlotCnt, slotIdx, 1);
    return mdl->meshSlots[slotIdx].srcBoneIndices;
}

_AFX void AfxBuildBoundMeshRigMatrixArray(afxModel mdl, afxNat slotIdx, afxWorldPose const* WorldPose, afxNat firstBoneIdx, afxNat boneCnt, afxReal xformBuffer[][4][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->meshSlotCnt, slotIdx, 1);

    afxMesh msh = mdl->meshSlots[slotIdx].msh;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    afxNat vertebraCnt = msh->vertebraCnt;

    if (firstBoneIdx < 0 || (firstBoneIdx >= vertebraCnt))
    {
        AfxError("firstBoneIdx %d is out of range [0, %d]", firstBoneIdx, vertebraCnt);
        AfxThrowError();
    }
    else
    {
        afxNat chosenBoneCnt = firstBoneIdx + boneCnt;

        if (chosenBoneCnt < 0 || chosenBoneCnt >= vertebraCnt + 1)
        {
            AfxError("firstBoneIdx + boneCnt %d is out of range [0, %d]", chosenBoneCnt, vertebraCnt + 1);
            AfxThrowError();
        }
        else
        {
            for (afxNat i = 0, artIdx = firstBoneIdx; artIdx < chosenBoneCnt; artIdx++)
            {
                afxV4d* w = AfxWorldPoseGetWorldMatrix(WorldPose, mdl->meshSlots[slotIdx].boneIndices[i]);
                afxSkeletonBone* bone = AfxGetBone(mdl->meshSlots[slotIdx].srcSkl, mdl->meshSlots[slotIdx].srcBoneIndices[i]);
                AfxMultiplyAffineM4d(xformBuffer[i], w, bone->iw);
            }
        }
    }
}

_AFX afxError AfxBindModelMeshes(afxModel mdl, afxSkeleton srcSkl, afxNat first, afxNat cnt, afxMesh meshes[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->meshSlotCnt, first, cnt);

    afxSkeleton dstSkl = mdl->skl;
    AfxTryAssertObjects(1, &dstSkl, afxFcc_SKL);
    AfxTryAssertObjects(1, &srcSkl, afxFcc_SKL);
    afxBool transfered = srcSkl && (srcSkl != dstSkl);

    afxSimulation sim = AfxGetObjectProvider(mdl);
    afxContext mem = AfxSimulationGetMemory(sim);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxNat slotIdx = first + i;
        AfxAssertRange(mdl->meshSlotCnt, slotIdx, 1);
        afxMeshSlot* slot = &mdl->meshSlots[slotIdx];
        afxMesh mshCurr = slot->msh;
        afxMesh msh = meshes[i];

        if (mshCurr != msh)
        {
            afxNat* srcBoneIndices = NIL;
            afxNat* dstBoneIndices = NIL;

            if (msh) // attach operation
            {
                AfxAssertObjects(1, &msh, afxFcc_MSH);

                afxNat artCnt = msh->vertebraCnt;

                if (artCnt && !(dstBoneIndices = AfxAllocate(mem, artCnt * sizeof(slot->boneIndices[0]), 0, AfxSpawnHint()))) AfxThrowError();
                else
                {
                    if (!transfered) srcBoneIndices = dstBoneIndices;
                    else if (artCnt && !(srcBoneIndices = AfxAllocate(mem, artCnt * sizeof(slot->srcBoneIndices[0]), 0, AfxSpawnHint())))
                        AfxThrowError();

                    if (!err)
                    {
                        for (afxNat i = 0; i < artCnt; i++)
                        {
                            afxString const* artName = &msh->vertebras[i].name;

                            if (!AfxFindBone(dstSkl, artName, &slot->boneIndices[i]))
                            {
                                AfxError("Unable to find vertebra '%.*s' in the destination skeleton", AfxPushString(artName));
                                AfxAssert(slot->boneIndices[i] == AFX_INVALID_INDEX);
                            }

                            if (transfered && !AfxFindBone(srcSkl, artName, &slot->srcBoneIndices[i]))
                            {
                                AfxError("Unable to find vertebra '%.*s' in the source skeleton", AfxPushString(artName));
                                AfxAssert(slot->srcBoneIndices[i] == AFX_INVALID_INDEX);
                            }
                        }
                        err = NIL; // skip any soft fail by bone indexing.

                        AfxReacquireObjects(1, (void*[]) { msh });

                        if (srcSkl && (srcSkl != mdl->skl))
                            AfxReacquireObjects(1, (void*[]) { srcSkl });
                    }

                    if (err && dstBoneIndices)
                        AfxDeallocate(mem, dstBoneIndices);
                }
            }

            if (!err)
            {
                if (mshCurr) // detach operation
                {
                    if (slot->boneIndices)
                        AfxDeallocate(mem, slot->boneIndices);

                    if (slot->srcBoneIndices && (slot->srcBoneIndices != slot->boneIndices))
                        AfxDeallocate(mem, slot->srcBoneIndices);

                    slot->boneIndices = NIL;
                    slot->srcBoneIndices = NIL;

                    if (slot->srcSkl && (slot->srcSkl != mdl->skl))
                    {
                        AfxReleaseObjects(1, (void*[]) { slot->srcSkl });
                        slot->srcSkl = NIL;
                    }

                    if (mshCurr)
                    {
                        AfxAssertObjects(1, &mshCurr, afxFcc_MSH);
                        AfxReleaseObjects(1, (void*[]) { mshCurr });
                        slot->msh = NIL;
                    }
                }

                if (msh)
                {
                    slot->msh = msh;
                    slot->srcSkl = srcSkl;
                    slot->srcBoneIndices = srcBoneIndices;
                    slot->boneIndices = dstBoneIndices;
                }
            }
        }
    }
    return err;
}

_AFX afxNat AfxGetBoundMeshes(afxModel mdl, afxNat baseSlotIdx, afxNat slotCnt, afxMesh msh[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->meshSlotCnt, baseSlotIdx, slotCnt);
    afxNat rslt = 0;

    for (afxNat i = 0; i < slotCnt; i++)
    {
        afxNat slotIdx = baseSlotIdx + i;
        AfxAssertRange(mdl->meshSlotCnt, slotIdx, 1);
        afxMesh msh2 = mdl->meshSlots[slotIdx].msh;
        AfxTryAssertObjects(1, &msh2, afxFcc_MSH);
        msh[i] = msh2;
    }
    return rslt;
}

_AFX afxNat AfxCountModelMeshes(afxModel mdl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    return mdl->meshSlotCnt;
}

_AFX void AfxGetModelInitialPlacement(afxModel mdl, afxReal m[4][4])
{
    // void GetModelInitialPlacement4x4(const model *Model, float *Placement4x4)

    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxComposeTransformMatrix(&mdl->init, m);
}

_AFX void AfxSetModelInitialPlacement(afxModel mdl, afxTransform const* xform)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxCopyTransform(&mdl->init, xform);
}

_AFX afxSkeleton AfxGetModelSkeleton(afxModel mdl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);

    afxSkeleton skl = mdl->skl;
    AfxAssertObjects(1, &skl, afxFcc_SKL);

    return skl;
}

_AFX void AfxSetModelSkeleton(afxModel mdl, afxSkeleton skl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    afxSkeleton skl2 = mdl->skl;

    if (skl != skl2)
    {
        if (skl2)
        {
            AfxAssertObjects(1, &skl2, afxFcc_SKL);
            AfxReleaseObjects(1, (void*[]) { skl2 });
        }

        if ((mdl->skl = skl))
        {
            AfxAssertObjects(1, &skl, afxFcc_SKL);
            AfxReacquireObjects(1, (void*[]) { skl });
        }
    }
}

_AFX afxUri const* AfxGetModelUri(afxModel mdl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    return &mdl->uri;
}

_AFX afxError _AfxMdlDtor(afxModel mdl)
{
    AfxEntry("mdl=%p", mdl);

    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);

    afxSimulation sim = AfxGetObjectProvider(mdl);
    afxContext mem = AfxSimulationGetMemory(sim);

    for (afxNat i = 0; i < mdl->meshSlotCnt; i++)
        AfxBindModelMeshes(mdl, NIL, i, 1, NIL);

    if (mdl->meshSlots)
        AfxDeallocate(mem, mdl->meshSlots);

    if (mdl->skl)
        AfxSetModelSkeleton(mdl, NIL);

    //if (mdl->uri)
    AfxUriDeallocate(&mdl->uri);

    return err;
}

_AFX afxError _AfxMdlCtor(afxModel mdl, afxCookie const *cookie)
{
    AfxEntry("mdl=%p", mdl);

    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);

    afxUri const* name = cookie->udd[0];
    afxSkeleton skl = cookie->udd[1];
    afxTransform const* init = cookie->udd[2];
    afxNat meshCnt = *((afxNat*)cookie->udd[3]);
    afxMesh* meshes = cookie->udd[4];

    AfxCloneUri(&mdl->uri, name);

    if ((mdl->skl = skl))
    {
        AfxAssertObjects(1, &skl, afxFcc_SKL);
        AfxReacquireObjects(1, (void*[]) { skl });
    }

    if (init)
        AfxCopyTransform(&mdl->init, init);
    else
        AfxZeroTransform(&mdl->init);

    AfxResetAabb(&mdl->aabb);

    mdl->meshSlotCnt = 0;

    if (meshCnt && !(mdl->meshSlots = AfxAllocate(NIL, meshCnt * sizeof(mdl->meshSlots[0]), 0, AfxSpawnHint()))) AfxThrowError();
    else
    {
        if (meshCnt)
        {
            mdl->meshSlotCnt = meshCnt;

            AfxZero(mdl->meshSlots, meshCnt * sizeof(mdl->meshSlots[0]));

            if (AfxBindModelMeshes(mdl, NIL, 0, meshCnt, meshes))
                AfxThrowError();
        }

        AfxAssert(mdl->meshSlotCnt == meshCnt);
    }

    if (err)
    {
        AfxReleaseObjects(1, (void*[]) { skl });
    }
    return err;
}

_AFX afxModel AfxAssembleModel(afxSimulation sim, afxUri const* name, afxSkeleton skl, afxTransform const* init, afxNat mshCnt, afxMesh msh[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AfxAssert(name);
    AfxAssert(skl);
    //AfxAssert(init);
    AfxAssert(mshCnt);
    afxModel mdl;

    if (AfxAcquireObjects(AfxGetModelClass(sim), 1, (afxHandle*)&mdl, (void*[]) { (void*)name, skl, (void*)init, &mshCnt, msh }))
        AfxThrowError();

    return mdl;
}

_AFX void AfxTransformModels(afxReal const at[3], afxReal const lt[3][3], afxReal const ilt[3][3], afxReal affineTol, afxReal linearTol, afxFlags flags, afxNat cnt, afxModel mdl[])
{
    // Should be compatible with voidTransformModel(model *Model, const float *Affine3, const float *Linear3x3, const float *InverseLinear3x3, float AffineTolerance, float LinearTolerance, unsigned int Flags)

    afxError err = AFX_ERR_NONE;
    AfxAssert(at);
    AfxAssert(lt);
    AfxAssert(ilt);
    AfxAssert(cnt);
    AfxAssert(mdl);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxModel mdl2 = mdl[i];

        if (mdl2)
        {
            AfxAssertObjects(1, &mdl2, afxFcc_MDL);
            afxSkeleton skl;

            if ((skl = AfxGetModelSkeleton(mdl2)))
                AfxTransformSkeleton(skl, at, lt, ilt);

            afxMesh meshes[64];
            afxNat batchSiz = 64;
            afxNat totalMshCnt = AfxCountModelMeshes(mdl2);
            afxNat batches = totalMshCnt / batchSiz;
            afxNat remaining = totalMshCnt % batchSiz;
            afxNat baseSlotIdx = 0;
            afxNat mshCnt = 0;

            for (afxNat j = 0; j < batches; j++)
            {
                mshCnt = AfxGetBoundMeshes(mdl2, baseSlotIdx, batchSiz, meshes);
                baseSlotIdx += mshCnt;

                // WARNING: What to do if mesh is shared among other models of strange asset?
                AfxTransformMeshes(at, lt, ilt, affineTol, linearTol, flags, mshCnt, meshes);
            }

            if (remaining)
            {
                mshCnt = AfxGetBoundMeshes(mdl2, baseSlotIdx, remaining, meshes);
                // WARNING: What to do if mesh is shared among other models of strange asset?
                AfxTransformMeshes(at, lt, ilt, affineTol, linearTol, flags, mshCnt, meshes);
            }

            AfxAssimilateTransforms(lt, ilt, at, 1, &mdl2->init, &mdl2->init);
        }
    }
}

_AFX afxClassConfig _AfxMdlClsConfig =
{
    .fcc = afxFcc_MDL,
    .name = "Model",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxModel)),
    .ctx = NIL,
    .ctor = (void*)_AfxMdlCtor,
    .dtor = (void*)_AfxMdlDtor
};
