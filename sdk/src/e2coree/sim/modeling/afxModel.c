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
#define _AFX_MODEL_C
#define _AFX_MESH_C
#define _AFX_SIMULATION_C
#define _AFX_SKELETON_C
#include "qwadro/sim/modeling/afxModel.h"
#include "qwadro/sim/afxSimulation.h"
#include "qwadro/io/afxUri.h"
#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/math/afxMatrix.h"
#include "qwadro/core/afxIndexedString.h"

_AFX afxBool AfxMeshIsTransplanted(afxModel mdl, afxNat slotIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->slotCnt, slotIdx, 1);
    return mdl->slots[slotIdx].origSkl != mdl->skl;
}

_AFX afxSkeleton AfxGetMeshOriginalSkeleton(afxModel mdl, afxNat slotIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->slotCnt, slotIdx, 1);
    return mdl->slots[slotIdx].origSkl;
}

_AFX afxNat const* AfxGetMeshBoneMap(afxModel mdl, afxNat slotIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->slotCnt, slotIdx, 1);
    return mdl->slots[slotIdx].boneMap;
}

_AFX afxNat const* AfxGetMeshOriginalBoneMap(afxModel mdl, afxNat slotIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->slotCnt, slotIdx, 1);
    return mdl->slots[slotIdx].origBoneMap;
}

_AFX afxMaterial AfxGetMeshTxd(afxModel mdl, afxNat slotIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->slotCnt, slotIdx, 1);
    afxMaterial txd;

    if ((txd = mdl->slots[slotIdx].txd))
    {
        AfxAssertObjects(1, &txd, afxFcc_MTL);
    }
    return txd;
}

_AFX void AfxSetMeshTxd(afxModel mdl, afxNat slotIdx, afxMaterial mtl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->slotCnt, slotIdx, 1);
    afxMaterial curr;

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

_AFX afxMaterial AfxFindMeshMaterial(afxModel mdl, afxNat slotIdx, afxString const* id)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->slotCnt, slotIdx, 1);    
    afxMaterial txd = mdl->slots[slotIdx].txd;
    afxMaterial mtl = NIL;

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

_AFX void AwxBuildLinkedMeshMatrixArray(afxModel mdl, afxNat slotIdx, awxWorldPose const* WorldPose, afxNat firstBoneIdx, afxNat boneCnt, afxReal xformBuffer[][4][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->slotCnt, slotIdx, 1);

    afxMesh msh = mdl->slots[slotIdx].msh;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    afxNat vertebraCnt = msh->pivotCnt;

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
                afxSkeletonBone* bone = AfxGetBone(mdl->slots[slotIdx].origSkl, mdl->slots[slotIdx].origBoneMap[i]);
                AfxMultiplyAtm4(xformBuffer[i], w, bone->iw);
            }
        }
    }
}

_AFX afxError AfxRelinkMeshes(afxModel mdl, afxSkeleton origSkl, afxNat baseSlot, afxNat slotCnt, afxMesh const meshes[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->slotCnt, baseSlot, slotCnt);

    afxSkeleton skl = mdl->skl;
    AfxTryAssertObjects(1, &skl, afxFcc_SKL);
    AfxTryAssertObjects(1, &origSkl, afxFcc_SKL);
    afxBool transplanted = origSkl && (origSkl != skl);

    afxSimulation sim = AfxGetObjectProvider(mdl);
    afxMmu mmu = AfxGetSimulationMmu(sim);

    for (afxNat i = 0; i < slotCnt; i++)
    {
        afxNat slotIdx = baseSlot + i;
        AfxAssertRange(mdl->slotCnt, slotIdx, 1);
        awxLinkedMesh* slot = &mdl->slots[slotIdx];
        afxMesh mshCurr = slot->msh;
        afxMesh msh = meshes ? meshes[i] : NIL;

        if (mshCurr != msh)
        {
            afxNat* origBoneMap = NIL;
            afxNat* dstBoneIndices = NIL;

            if (msh) // attach operation
            {
                AfxAssertObjects(1, &msh, afxFcc_MSH);
                afxStringCatalog strc = msh->strc;

                afxNat pivotCnt = msh->pivotCnt;

                if (pivotCnt && !(dstBoneIndices = AfxAllocate(mmu, pivotCnt, sizeof(dstBoneIndices[0]), 0, AfxHint()))) AfxThrowError();
                else
                {
                    if (!transplanted) origBoneMap = dstBoneIndices;
                    else if (pivotCnt && !(origBoneMap = AfxAllocate(mmu, pivotCnt, sizeof(origBoneMap[0]), 0, AfxHint())))
                        AfxThrowError();

                    if (!err)
                    {
                        afxMeshPivot const* pivots = msh->pivots;

                        for (afxNat j = 0; j < pivotCnt; j++)
                        {
                            afxString const* artId = &pivots[j].id.str.str;
                            afxString pivotId;
                            
                            AfxGetMeshPivotId(msh, j, &pivotId);
                            if (1)
                            {
                                if (!AfxFindBone(skl, &pivotId, &dstBoneIndices[j]))
                                {
                                    AfxError("Pivot '%.*s' not found in the destination skeleton.", AfxPushString(&pivotId));
                                    AfxAssert(dstBoneIndices[j] == AFX_INVALID_INDEX);
                                }

                                if (transplanted && !AfxFindBone(origSkl, &pivotId, &origBoneMap[j]))
                                {
                                    AfxError("Pivot '%.*s' not found in the source skeleton.", AfxPushString(&pivotId));
                                    AfxAssert(origBoneMap[j] == AFX_INVALID_INDEX);
                                }
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

                    slot->boneMap = NIL;

                    if (slot->origBoneMap && (slot->origBoneMap != slot->boneMap))
                        AfxDeallocate(mmu, slot->origBoneMap);

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

_AFX afxNat AfxEnumerateLinkedMeshes(afxModel mdl, afxNat baseSlot, afxNat slotCnt, afxMesh msh[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->slotCnt, baseSlot, slotCnt);
    afxNat rslt = 0;

    for (afxNat i = 0; i < slotCnt; i++)
    {
        afxNat slotIdx = baseSlot + i;
        AfxAssertRange(mdl->slotCnt, slotIdx, 1);
        afxMesh msh2 = mdl->slots[slotIdx].msh;

        if (msh2)
        {
            AfxAssertObjects(1, &msh2, afxFcc_MSH);
            ++rslt;
        }
        msh[i] = msh2;
    }
    return rslt;
}

_AFX afxNat AfxGetModelCapacity(afxModel mdl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    return mdl->slotCnt;
}

_AFX afxNat AfxCountLinkedMeshes(afxModel mdl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    afxNat cap = mdl->slotCnt;
    afxNat rslt = 0;

    for (afxNat i = 0; i < cap; i++)
    {
        afxMesh msh = mdl->slots[i].msh;
        
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

_AFX afxBool AfxGetModelId(afxModel mdl, afxString* id)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssert(id);
    afxBool rslt = TRUE;

    if (mdl->idStrIdx != AFX_INVALID_INDEX)
        rslt = !!AfxResolveStrings(mdl->strc, 1, &mdl->idStrIdx, id);
    else
        AfxResetString(id);

    return rslt;
}

_AFX afxError _AfxMdlDtor(afxModel mdl)
{
    AfxEntry("mdl=%p", mdl);

    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);

    afxSimulation sim = AfxGetObjectProvider(mdl);
    afxMmu mmu = AfxGetSimulationMmu(sim);
    
    for (afxNat i = 0; i < mdl->slotCnt; i++)
        AfxRelinkMeshes(mdl, NIL, i, 1, NIL);

    if (mdl->slots)
        AfxDeallocate(mmu, mdl->slots);

    afxSkeleton skl;

    if ((skl = mdl->skl))
    {
        AfxAssertObjects(1, &skl, afxFcc_SKL);
        AfxReleaseObjects(1, (void*[]) { skl });
    }
    //if (mdl->uri)

    if (mdl->strc)
        AfxReleaseObjects(1, (void*[]) { mdl->strc });

    return err;
}

_AFX afxError _AfxMdlCtor(afxModel mdl, afxCookie const *cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);

    afxSimulation sim = cookie->udd[0];
    afxStringCatalog strc = cookie->udd[1];
    afxMesh const* meshes = cookie->udd[2];
    afxModelBlueprint const*mdlb = cookie->udd[3];
    mdlb += cookie->no;

    if (!(mdl->strc = strc))
        mdl->idStrIdx = AFX_INVALID_INDEX;
    else if (AfxCatalogStrings(strc, 1, &mdlb->id.str.str, &mdl->idStrIdx, 0))
        AfxThrowError();
    else
        AfxReacquireObjects(1, (void*[]) { strc });

    AfxMakeFixedString32(&mdl->id, &mdlb->id.str.str);

    if (!err)
    {
        afxSkeleton skl = mdlb->skl;
        afxTransform const* init = &mdlb->init;
        afxNat meshCnt = mdlb->mshCnt;
        afxNat baseMshIdx = mdlb->baseMshIdx;

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
        mdl->slots = NIL;

        if (meshCnt && !(mdl->slots = AfxAllocate(NIL, meshCnt, sizeof(mdl->slots[0]), 0, AfxHint()))) AfxThrowError();
        else
        {
            if (meshCnt)
            {
                mdl->slotCnt = meshCnt;

                AfxZero(meshCnt, sizeof(mdl->slots[0]), mdl->slots);

                if (AfxRelinkMeshes(mdl, NIL, 0, meshCnt, &meshes[baseMshIdx]))
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
            AfxEcho(echStr, mdl, AfxPushString(&mdl->id.str.str), mdl->slotCnt, mdl->skl->boneCnt);
        }
    }
    return err;
}

_AFX afxClassConfig _AfxMdlClsConfig =
{
    .fcc = afxFcc_MDL,
    .name = "Model",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxModel)),
    .mmu = NIL,
    .ctor = (void*)_AfxMdlCtor,
    .dtor = (void*)_AfxMdlDtor
};

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

_AFX void AfxTransformModels(afxReal const lt[3][3], afxReal const ilt[3][3], afxReal ltTol, afxReal const at[3], afxReal atTol, afxFlags flags, afxNat cnt, afxModel models[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(at);
    AfxAssert(lt);
    AfxAssert(ilt);
    AfxAssert(cnt);
    AfxAssert(models);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxModel mdl2 = models[i];

        if (mdl2)
        {
            AfxAssertObjects(1, &mdl2, afxFcc_MDL);
            afxSkeleton skl;

            if ((skl = AfxGetModelSkeleton(mdl2)))
            {
                AfxAssertObjects(1, &skl, afxFcc_SKL);
                AfxTransformSkeletons(lt, ilt, ltTol, at, atTol, 1, &skl);
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
                mshCnt = AfxEnumerateLinkedMeshes(mdl2, baseSlotIdx, batchSiz, meshes);
                baseSlotIdx += mshCnt;

                // WARNING: What to do if mesh is shared among other models of strange asset?
                AfxTransformMeshes(lt, ilt, ltTol, at, atTol, flags, mshCnt, meshes);
            }

            if (remaining)
            {
                mshCnt = AfxEnumerateLinkedMeshes(mdl2, baseSlotIdx, remaining, meshes);
                // WARNING: What to do if mesh is shared among other models of strange asset?
                AfxTransformMeshes(lt, ilt, ltTol, at, atTol, flags, mshCnt, meshes);
            }

            AfxAssimilateTransforms(lt, ilt, at, 1, &mdl2->init, &mdl2->init);
        }
    }
}

_AFX afxError AfxAssembleModel(afxSimulation sim, afxStringCatalog strc, afxMesh const meshes[], afxNat cnt, afxModelBlueprint const blueprints[], afxModel models[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AfxAssert(blueprints);
    AfxAssert(models);
    AfxAssert(meshes);
    AfxAssert(cnt);
    
    if (AfxAcquireObjects(AwxGetModelClass(sim), cnt, (afxObject*)models, (void const*[]) { sim, strc, meshes, blueprints }))
        AfxThrowError();

    return err;
}
