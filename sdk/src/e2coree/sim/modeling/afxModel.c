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
#define _AFX_MODEL_C
#define _AFX_MESH_C
#define _AFX_SIMULATION_C
#define _AFX_SKELETON_C
#include "afx/sim/modeling/afxModel.h"
#include "afx/sim/afxSimulation.h"
#include "afx/core/afxUri.h"
#include "afx/draw/afxDrawSystem.h"
#include "afx/math/afxMatrix.h"

_AFX afxBool AfxAttachedMeshIsTransferred(afxModel mdl, afxNat slotIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->cap, slotIdx, 1);
    return mdl->meshSlots[slotIdx].srcSkl != mdl->skl;
}

_AFX afxSkeleton AfxGetAttachedMeshOriginalSkeleton(afxModel mdl, afxNat slotIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->cap, slotIdx, 1);
    return mdl->meshSlots[slotIdx].srcSkl;
}

_AFX afxNat const* AfxGetAttachedMeshBoneIndices(afxModel mdl, afxNat slotIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->cap, slotIdx, 1);
    return mdl->meshSlots[slotIdx].boneIndices;
}

_AFX afxNat const* AfxGetAttachedMeshOriginalBoneIndices(afxModel mdl, afxNat slotIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->cap, slotIdx, 1);
    return mdl->meshSlots[slotIdx].srcBoneIndices;
}

_AFX void AfxBuildAttachedMeshMatrixArray(afxModel mdl, afxNat slotIdx, afxWorldPose const* WorldPose, afxNat firstBoneIdx, afxNat boneCnt, afxReal xformBuffer[][4][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->cap, slotIdx, 1);

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
                AfxMultiplyAtm4(xformBuffer[i], w, bone->iw);
            }
        }
    }
}

_AFX afxError AfxAttachMeshes(afxModel mdl, afxSkeleton srcSkl, afxNat first, afxNat cnt, afxMesh meshes[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->cap, first, cnt);

    afxSkeleton dstSkl = mdl->skl;
    AfxTryAssertObjects(1, &dstSkl, afxFcc_SKL);
    AfxTryAssertObjects(1, &srcSkl, afxFcc_SKL);
    afxBool transfered = srcSkl && (srcSkl != dstSkl);

    afxSimulation sim = AfxGetObjectProvider(mdl);
    afxContext mem = AfxSimulationGetMemory(sim);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxNat slotIdx = first + i;
        AfxAssertRange(mdl->cap, slotIdx, 1);
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

                if (artCnt && !(dstBoneIndices = AfxAllocate(mem, sizeof(dstBoneIndices[0]), artCnt, 0, AfxHint()))) AfxThrowError();
                else
                {
                    if (!transfered) srcBoneIndices = dstBoneIndices;
                    else if (artCnt && !(srcBoneIndices = AfxAllocate(mem, sizeof(srcBoneIndices[0]), artCnt, 0, AfxHint())))
                        AfxThrowError();

                    if (!err)
                    {
                        for (afxNat j = 0; j < artCnt; j++)
                        {
                            afxString const* artId = &msh->vertebras[j].id.str;

                            if (!AfxFindBone(dstSkl, artId, &dstBoneIndices[j]))
                            {
                                AfxError("Unable to find vertebra '%.*s' in the destination skeleton", AfxPushString(artId));
                                AfxAssert(dstBoneIndices[j] == AFX_INVALID_INDEX);
                            }

                            if (transfered && !AfxFindBone(srcSkl, artId, &srcBoneIndices[j]))
                            {
                                AfxError("Unable to find vertebra '%.*s' in the source skeleton", AfxPushString(artId));
                                AfxAssert(srcBoneIndices[j] == AFX_INVALID_INDEX);
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

                    AfxAssertObjects(1, &mshCurr, afxFcc_MSH);
                    AfxReleaseObjects(1, (void*[]) { mshCurr });
                    slot->msh = NIL;
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

_AFX afxNat AfxGetAttachedMeshes(afxModel mdl, afxNat baseSlotIdx, afxNat slotCnt, afxMesh msh[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->cap, baseSlotIdx, slotCnt);
    afxNat rslt = 0;

    for (afxNat i = 0; i < slotCnt; i++)
    {
        afxNat slotIdx = baseSlotIdx + i;
        AfxAssertRange(mdl->cap, slotIdx, 1);
        afxMesh msh2 = mdl->meshSlots[slotIdx].msh;
        AfxTryAssertObjects(1, &msh2, afxFcc_MSH);
        msh[i] = msh2;
    }
    return rslt;
}

_AFX afxNat AfxGetModelCapacity(afxModel mdl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    return mdl->cap;
}

_AFX afxNat AfxCountAttachedMeshes(afxModel mdl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    afxNat cap = mdl->cap;
    afxNat rslt = 0;

    for (afxNat i = 0; i < cap; i++)
    {
        afxMesh msh = mdl->meshSlots[i].msh;
        
        if (msh)
        {
            AfxTryAssertObjects(1, &msh, afxFcc_MSH);
            ++rslt;
        }        
    }
    return rslt;
}

_AFX void AfxGetModelInitialPlacement(afxModel mdl, afxReal m[4][4])
{
    // void GetModelInitialPlacement4x4(const model *Model, float *Placement4x4)

    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxComposeTransformM4d(&mdl->init, m);
}

_AFX void AfxResetModelInitialPlacement(afxModel mdl, afxTransform const* xform)
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

_AFX void AfxRelinkModelSkeleton(afxModel mdl, afxSkeleton skl)
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

_AFX afxString const* AfxGetModelId(afxModel mdl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    return &mdl->id;
}

_AFX afxError _AfxMdlDtor(afxModel mdl)
{
    AfxEntry("mdl=%p", mdl);

    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);

    afxSimulation sim = AfxGetObjectProvider(mdl);
    afxContext mem = AfxSimulationGetMemory(sim);
    
    for (afxNat i = 0; i < mdl->cap; i++)
        AfxAttachMeshes(mdl, NIL, i, 1, &mdl->meshSlots[i].msh);

    if (mdl->meshSlots)
        AfxDeallocate(mem, mdl->meshSlots);

    if (mdl->skl)
        AfxRelinkModelSkeleton(mdl, NIL);

    //if (mdl->uri)
    AfxDeallocateString(&mdl->id);

    return err;
}

_AFX afxError _AfxMdlCtor(afxModel mdl, afxCookie const *cookie)
{
    AfxEntry("mdl=%p", mdl);

    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);

    afxString const* id = cookie->udd[0];
    afxSkeleton skl = cookie->udd[1];
    afxTransform const* init = cookie->udd[2];
    afxNat meshCnt = *((afxNat*)cookie->udd[3]);
    afxMesh* meshes = cookie->udd[4];

    AfxCloneString(&mdl->id, id);

    if ((mdl->skl = skl))
    {
        AfxAssertObjects(1, &skl, afxFcc_SKL);
        AfxReacquireObjects(1, (void*[]) { skl });
    }

    if (init)
        AfxCopyTransform(&mdl->init, init);
    else
        AfxResetTransform(&mdl->init);

    AfxResetAabb(&mdl->aabb);

    mdl->cap = 0;

    if (meshCnt && !(mdl->meshSlots = AfxAllocate(NIL, sizeof(mdl->meshSlots[0]), meshCnt, 0, AfxHint()))) AfxThrowError();
    else
    {
        if (meshCnt)
        {
            mdl->cap = meshCnt;

            AfxZero(meshCnt, sizeof(mdl->meshSlots[0]), mdl->meshSlots);

            if (AfxAttachMeshes(mdl, NIL, 0, meshCnt, meshes))
                AfxThrowError();
        }

        AfxAssert(mdl->cap == meshCnt);
    }

    if (err)
    {
        AfxReleaseObjects(1, (void*[]) { skl });
    }

    if (!err)
    {
        afxChar const* echStr = "Model %p assembled. Id: \"%.*s\"\n    %u meshes.\n    %u joints\n";
        AfxEcho(echStr, mdl, AfxPushString(&mdl->id), mdl->cap, mdl->skl->boneCnt);
    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

_AFX afxModel AfxAssembleModel(afxSimulation sim, afxString const* id, afxSkeleton skl, afxTransform const* init, afxNat mshCnt, afxMesh msh[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AfxAssert(id);
    AfxAssert(skl);
    //AfxAssert(init);
    AfxAssert(mshCnt);
    afxModel mdl;

    if (AfxAcquireObjects(AfxGetModelClass(sim), 1, (afxHandle*)&mdl, (void*[]) { (void*)id, skl, (void*)init, &mshCnt, msh }))
        AfxThrowError();

    return mdl;
}

_AFX void AfxTransformModels(afxReal const lt[3][3], afxReal const ilt[3][3], afxReal const at[3], afxReal atTol, afxReal ltTol, afxFlags flags, afxNat cnt, afxModel mdl[])
{
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
            {
                AfxAssertObjects(1, &skl, afxFcc_SKL);
                AfxTransformSkeletons(lt, ilt, at, atTol, ltTol, 1, &skl);
            }

            afxMesh meshes[64];
            afxNat batchSiz = 64;
            afxNat totalMshCnt = AfxGetModelCapacity(mdl2);
            afxNat batches = totalMshCnt / batchSiz;
            afxNat remaining = totalMshCnt % batchSiz;
            afxNat baseSlotIdx = 0;
            afxNat mshCnt = 0;

            for (afxNat j = 0; j < batches; j++)
            {
                mshCnt = AfxGetAttachedMeshes(mdl2, baseSlotIdx, batchSiz, meshes);
                baseSlotIdx += mshCnt;

                // WARNING: What to do if mesh is shared among other models of strange asset?
                AfxTransformMeshes(lt, ilt, at, atTol, ltTol, flags, mshCnt, meshes);
            }

            if (remaining)
            {
                mshCnt = AfxGetAttachedMeshes(mdl2, baseSlotIdx, remaining, meshes);
                // WARNING: What to do if mesh is shared among other models of strange asset?
                AfxTransformMeshes(lt, ilt, at, atTol, ltTol, flags, mshCnt, meshes);
            }

            AfxAssimilateTransforms(lt, ilt, at, 1, &mdl2->init, &mdl2->init);
        }
    }
}

_AFX afxClassConfig _AfxMdlClsConfig =
{
    .fcc = afxFcc_MDL,
    .name = "Model",
    .unitsPerPage = 8,
    .size = sizeof(AFX_OBJECT(afxModel)),
    .ctx = NIL,
    .ctor = (void*)_AfxMdlCtor,
    .dtor = (void*)_AfxMdlDtor
};
