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
#include "qwadro/sim/modeling/awxModel.h"
#include "qwadro/sim/awxSimulation.h"
#include "qwadro/core/afxUri.h"
#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/math/afxMatrix.h"

_AFX afxBool AwxLinkedMeshIsTransplanted(awxModel mdl, afxNat slotIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->slotCnt, slotIdx, 1);
    return mdl->slots[slotIdx].origSkl != mdl->skl;
}

_AFX awxSkeleton AwxGetLinkedMeshOriginalSkeleton(awxModel mdl, afxNat slotIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->slotCnt, slotIdx, 1);
    return mdl->slots[slotIdx].origSkl;
}

_AFX afxNat const* AwxGetLinkedMeshBoneMap(awxModel mdl, afxNat slotIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->slotCnt, slotIdx, 1);
    return mdl->slots[slotIdx].boneMap;
}

_AFX afxNat const* AwxGetLinkedMeshOriginalBoneMap(awxModel mdl, afxNat slotIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->slotCnt, slotIdx, 1);
    return mdl->slots[slotIdx].origBoneMap;
}

_AFX awxMaterial AwxGetLinkedMeshTxd(awxModel mdl, afxNat slotIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->slotCnt, slotIdx, 1);
    awxMaterial txd;

    if ((txd = mdl->slots[slotIdx].txd))
    {
        AfxAssertObjects(1, &txd, afxFcc_MTL);
    }
    return txd;
}

_AFX void AwxSetLinkedMeshTxd(awxModel mdl, afxNat slotIdx, awxMaterial mtl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->slotCnt, slotIdx, 1);
    awxMaterial curr;

    if (mtl != (curr = mdl->slots[slotIdx].txd))
    {
        if (mtl)
        {
            AfxAssertObjects(1, &mtl, afxFcc_MTL);
            AfxReacquireObjects(1, (void*[]) { mtl });
        }

        mdl->slots[slotIdx].txd = mtl;

        if (curr)
        {
            AfxAssertObjects(1, &curr, afxFcc_MTL);
            AfxReleaseObjects(1, (void*[]) { curr });
        }
    }
}

_AFX awxMaterial AwxFindLinkedMeshMaterial(awxModel mdl, afxNat slotIdx, afxString const* id)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->slotCnt, slotIdx, 1);    
    awxMaterial txd = mdl->slots[slotIdx].txd;
    awxMaterial mtl = NIL;

    if (txd)
    {
        AfxAssertObjects(1, &txd, afxFcc_MTL);
        AfxAssert(id);
        
        if ((mtl = AfxFindSubmaterial(txd, id)))
        {
            AfxAssertObjects(1, &mtl, afxFcc_MTL);
        }
    }
    return mtl;
}

_AFX void AwxBuildLinkedMeshMatrixArray(awxModel mdl, afxNat slotIdx, awxWorldPose const* WorldPose, afxNat firstBoneIdx, afxNat boneCnt, afxReal xformBuffer[][4][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->slotCnt, slotIdx, 1);

    awxMesh msh = mdl->slots[slotIdx].msh;
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
                afxV4d* w = AfxWorldPoseGetWorldMatrix(WorldPose, mdl->slots[slotIdx].boneMap[i]);
                awxSkeletonBone* bone = AfxGetBone(mdl->slots[slotIdx].origSkl, mdl->slots[slotIdx].origBoneMap[i]);
                AfxMultiplyAtm4(xformBuffer[i], w, bone->iw);
            }
        }
    }
}

_AFX afxError AwxRelinkMeshes(awxModel mdl, awxSkeleton origSkl, afxNat baseSlot, afxNat slotCnt, awxMesh meshes[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->slotCnt, baseSlot, slotCnt);

    awxSkeleton skl = mdl->skl;
    AfxTryAssertObjects(1, &skl, afxFcc_SKL);
    AfxTryAssertObjects(1, &origSkl, afxFcc_SKL);
    afxBool transplanted = origSkl && (origSkl != skl);

    awxSimulation sim = AfxGetObjectProvider(mdl);
    afxMmu mmu = AfxSimulationGetMemory(sim);

    for (afxNat i = 0; i < slotCnt; i++)
    {
        afxNat slotIdx = baseSlot + i;
        AfxAssertRange(mdl->slotCnt, slotIdx, 1);
        awxLinkedMesh* slot = &mdl->slots[slotIdx];
        awxMesh mshCurr = slot->msh;
        awxMesh msh = meshes[i];

        if (mshCurr != msh)
        {
            afxNat* origBoneMap = NIL;
            afxNat* dstBoneIndices = NIL;

            if (msh) // attach operation
            {
                AfxAssertObjects(1, &msh, afxFcc_MSH);

                afxNat artCnt = msh->vertebraCnt;

                if (artCnt && !(dstBoneIndices = AfxAllocate(mmu, sizeof(dstBoneIndices[0]), artCnt, 0, AfxHint()))) AfxThrowError();
                else
                {
                    if (!transplanted) origBoneMap = dstBoneIndices;
                    else if (artCnt && !(origBoneMap = AfxAllocate(mmu, sizeof(origBoneMap[0]), artCnt, 0, AfxHint())))
                        AfxThrowError();

                    if (!err)
                    {
                        for (afxNat j = 0; j < artCnt; j++)
                        {
                            afxString const* artId = &msh->vertebras[j].id.str.str;

                            if (!AfxFindBone(skl, artId, &dstBoneIndices[j]))
                            {
                                AfxError("Articulation '%.*s' missing in the destination skeleton.", AfxPushString(artId));
                                AfxAssert(dstBoneIndices[j] == AFX_INVALID_INDEX);
                            }

                            if (transplanted && !AfxFindBone(origSkl, artId, &origBoneMap[j]))
                            {
                                AfxError("Articulation '%.*s' missing in the source skeleton.", AfxPushString(artId));
                                AfxAssert(origBoneMap[j] == AFX_INVALID_INDEX);
                            }
                        }
                        err = NIL; // skip any soft fail by bone indexing.
                         
                        AfxReacquireObjects(1, (void*[]) { msh });

                        if (origSkl && (origSkl != mdl->skl))
                            AfxReacquireObjects(1, (void*[]) { origSkl });
                    }

                    if (err && dstBoneIndices)
                        AfxDeallocate(mmu, dstBoneIndices);
                }
            }

            if (!err)
            {
                if (mshCurr) // detach operation
                {
                    if (slot->boneMap)
                        AfxDeallocate(mmu, slot->boneMap);

                    if (slot->origBoneMap && (slot->origBoneMap != slot->boneMap))
                        AfxDeallocate(mmu, slot->origBoneMap);

                    slot->boneMap = NIL;
                    slot->origBoneMap = NIL;

                    if (slot->origSkl && (slot->origSkl != mdl->skl))
                    {
                        AfxReleaseObjects(1, (void*[]) { slot->origSkl });
                        slot->origSkl = NIL;
                    }

                    AfxAssertObjects(1, &mshCurr, afxFcc_MSH);
                    AfxReleaseObjects(1, (void*[]) { mshCurr });
                    slot->msh = NIL;
                }

                if (msh)
                {
                    slot->msh = msh;
                    slot->origSkl = origSkl;
                    slot->origBoneMap = origBoneMap;
                    slot->boneMap = dstBoneIndices;
                }
            }
        }
    }
    return err;
}

_AFX afxNat AwxGetLinkedMeshes(awxModel mdl, afxNat baseSlot, afxNat slotCnt, awxMesh msh[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->slotCnt, baseSlot, slotCnt);
    afxNat rslt = 0;

    for (afxNat i = 0; i < slotCnt; i++)
    {
        afxNat slotIdx = baseSlot + i;
        AfxAssertRange(mdl->slotCnt, slotIdx, 1);
        awxMesh msh2 = mdl->slots[slotIdx].msh;

        if (msh2)
        {
            AfxAssertObjects(1, &msh2, afxFcc_MSH);
            ++rslt;
        }
        msh[i] = msh2;
    }
    return rslt;
}

_AFX afxNat AwxGetMeshLinkageCapacity(awxModel mdl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    return mdl->slotCnt;
}

_AFX afxNat AwxCountLinkedMeshes(awxModel mdl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    afxNat cap = mdl->slotCnt;
    afxNat rslt = 0;

    for (afxNat i = 0; i < cap; i++)
    {
        awxMesh msh = mdl->slots[i].msh;
        
        if (msh)
        {
            AfxTryAssertObjects(1, &msh, afxFcc_MSH);
            ++rslt;
        }        
    }
    return rslt;
}

_AFX void AwxGetModelInitialPlacement(awxModel mdl, afxReal m[4][4])
{
    // void GetModelInitialPlacement4x4(const model *Model, float *Placement4x4)

    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxComposeTransformM4d(&mdl->init, m);
}

_AFX void AwxResetModelInitialPlacement(awxModel mdl, afxTransform const* xform)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxCopyTransform(&mdl->init, xform);
}

_AFX awxSkeleton AwxGetModelSkeleton(awxModel mdl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);

    awxSkeleton skl = mdl->skl;
    AfxAssertObjects(1, &skl, afxFcc_SKL);

    return skl;
}

_AFX void AwxRelinkModelSkeleton(awxModel mdl, awxSkeleton skl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    awxSkeleton skl2 = mdl->skl;

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

_AFX afxString const* AwxGetModelId(awxModel mdl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    return &mdl->id.str;
}

_AFX afxError _AfxMdlDtor(awxModel mdl)
{
    AfxEntry("mdl=%p", mdl);

    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);

    awxSimulation sim = AfxGetObjectProvider(mdl);
    afxMmu mmu = AfxSimulationGetMemory(sim);
    
    for (afxNat i = 0; i < mdl->slotCnt; i++)
        AwxRelinkMeshes(mdl, NIL, i, 1, &mdl->slots[i].msh);

    if (mdl->slots)
        AfxDeallocate(mmu, mdl->slots);

    if (mdl->skl)
        AwxRelinkModelSkeleton(mdl, NIL);

    //if (mdl->uri)
    AfxDeallocateString(&mdl->id);

    return err;
}

_AFX afxError _AfxMdlCtor(awxModel mdl, afxCookie const *cookie)
{
    AfxEntry("mdl=%p", mdl);

    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);

    afxString const* id = cookie->udd[0];
    awxSkeleton skl = cookie->udd[1];
    afxTransform const* init = cookie->udd[2];
    afxNat meshCnt = *((afxNat*)cookie->udd[3]);
    awxMesh* meshes = cookie->udd[4];

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

    AfxResetAabb(mdl->aabb);

    mdl->slotCnt = 0;

    if (meshCnt && !(mdl->slots = AfxAllocate(NIL, sizeof(mdl->slots[0]), meshCnt, 0, AfxHint()))) AfxThrowError();
    else
    {
        if (meshCnt)
        {
            mdl->slotCnt = meshCnt;

            AfxZero(meshCnt, sizeof(mdl->slots[0]), mdl->slots);

            if (AwxRelinkMeshes(mdl, NIL, 0, meshCnt, meshes))
                AfxThrowError();
        }

        AfxAssert(mdl->slotCnt == meshCnt);
    }

    if (err)
    {
        AfxReleaseObjects(1, (void*[]) { skl });
    }

    if (!err)
    {
        afxChar const* echStr = "Model %p assembled. Id: \"%.*s\"\n    %u meshes.\n    %u joints\n";
        AfxEcho(echStr, mdl, AfxPushString(&mdl->id.str), mdl->slotCnt, mdl->skl->boneCnt);
    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

_AFX awxModel AwxAssembleModel(awxSimulation sim, afxString const* id, awxSkeleton skl, afxTransform const* init, afxNat mshCnt, awxMesh msh[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AfxAssert(id);
    AfxAssert(skl);
    //AfxAssert(init);
    AfxAssert(mshCnt);
    awxModel mdl;

    if (AfxAcquireObjects(AwxGetModelClass(sim), 1, (afxObject*)&mdl, (void const*[]) { (void*)id, skl, (void*)init, &mshCnt, msh }))
        AfxThrowError();

    return mdl;
}

_AFX void AwxTransformModels(afxReal const lt[3][3], afxReal const ilt[3][3], afxReal const at[3], afxReal atTol, afxReal ltTol, afxFlags flags, afxNat cnt, awxModel mdl[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(at);
    AfxAssert(lt);
    AfxAssert(ilt);
    AfxAssert(cnt);
    AfxAssert(mdl);

    for (afxNat i = 0; i < cnt; i++)
    {
        awxModel mdl2 = mdl[i];

        if (mdl2)
        {
            AfxAssertObjects(1, &mdl2, afxFcc_MDL);
            awxSkeleton skl;

            if ((skl = AwxGetModelSkeleton(mdl2)))
            {
                AfxAssertObjects(1, &skl, afxFcc_SKL);
                AfxTransformSkeletons(lt, ilt, at, atTol, ltTol, 1, &skl);
            }

            awxMesh meshes[64];
            afxNat batchSiz = 64;
            afxNat totalMshCnt = AwxGetMeshLinkageCapacity(mdl2);
            afxNat batches = totalMshCnt / batchSiz;
            afxNat remaining = totalMshCnt % batchSiz;
            afxNat baseSlotIdx = 0;
            afxNat mshCnt = 0;

            for (afxNat j = 0; j < batches; j++)
            {
                mshCnt = AwxGetLinkedMeshes(mdl2, baseSlotIdx, batchSiz, meshes);
                baseSlotIdx += mshCnt;

                // WARNING: What to do if mesh is shared among other models of strange asset?
                AwxTransformMeshes(lt, ilt, at, atTol, ltTol, flags, mshCnt, meshes);
            }

            if (remaining)
            {
                mshCnt = AwxGetLinkedMeshes(mdl2, baseSlotIdx, remaining, meshes);
                // WARNING: What to do if mesh is shared among other models of strange asset?
                AwxTransformMeshes(lt, ilt, at, atTol, ltTol, flags, mshCnt, meshes);
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
    .size = sizeof(AFX_OBJECT(awxModel)),
    .mmu = NIL,
    .ctor = (void*)_AfxMdlCtor,
    .dtor = (void*)_AfxMdlDtor
};
