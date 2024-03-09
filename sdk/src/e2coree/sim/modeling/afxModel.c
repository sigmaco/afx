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

_AKX afxBool AfxRiggedMeshIsTransplanted(afxModel mdl, afxNat slotIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->slotCnt, slotIdx, 1);
    return mdl->slots[slotIdx].origSkl != mdl->skl;
}

_AKX afxSkeleton AfxGetRiggedMeshOriginalSkeleton(afxModel mdl, afxNat slotIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->slotCnt, slotIdx, 1);
    return mdl->slots[slotIdx].origSkl;
}

_AKX afxNat const* AfxGetRiggedMeshMapping(afxModel mdl, afxNat slotIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->slotCnt, slotIdx, 1);
    return mdl->slots[slotIdx].boneMap;
}

_AKX afxNat const* AfxGetRiggedMeshOriginalMapping(afxModel mdl, afxNat slotIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->slotCnt, slotIdx, 1);
    return mdl->slots[slotIdx].origBoneMap;
}

_AKX afxMaterial AfxGetRiggedMeshTxd(afxModel mdl, afxNat slotIdx)
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

_AKX void AfxSetRiggedMeshTxd(afxModel mdl, afxNat slotIdx, afxMaterial mtl)
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

_AKX afxMaterial AfxFindRiggedMeshMaterial(afxModel mdl, afxNat slotIdx, afxString const* id)
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

_AKX afxNat AfxEnumerateRiggedMeshes(afxModel mdl, afxNat baseSlot, afxNat slotCnt, afxMesh meshes[])
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
        meshes[i] = msh2;
    }
    return rslt;
}

_AKX void AwxComputeRiggedMeshMatrixArray(afxModel mdl, afxNat slotIdx, awxWorldPose const* WorldPose, afxNat firstBoneIdx, afxNat boneCnt, afxM4d xformBuffer[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->slotCnt, slotIdx, 1);
    afxMesh msh = mdl->slots[slotIdx].msh;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    afxNat totalBiasCnt = msh->biasCnt;

    AfxAssertRange(totalBiasCnt, firstBoneIdx, boneCnt);
    afxNat chosenBoneCnt = firstBoneIdx + boneCnt;
    AfxAssert(totalBiasCnt >= chosenBoneCnt);

    afxV4d* w = AfxWorldPoseGetWorldMatrix(WorldPose, 0);
    afxM4d* iw = AfxGetSkeletonIwm(mdl->slots[slotIdx].origSkl, 0);
    afxNat const* mapping = AfxGetRiggedMeshMapping(mdl, slotIdx);
    afxNat const* mappingOrig = AfxGetRiggedMeshOriginalMapping(mdl, slotIdx);

    for (afxNat i = 0, artIdx = firstBoneIdx; artIdx < chosenBoneCnt; artIdx++)
    {
        AfxMultiplyAtm4d(xformBuffer[i], iw[mappingOrig[i]], w[mapping[i]]);
    }
}

_AKX afxError AfxRigMeshes(afxModel mdl, afxSkeleton origSkl, afxNat baseSlot, afxNat slotCnt, afxMesh const meshes[])
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
        awxRiggedMesh* slot = &mdl->slots[slotIdx];
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

                afxNat biasCnt = msh->biasCnt;

                if (biasCnt && !(dstBoneIndices = AfxAllocate(mmu, biasCnt, sizeof(dstBoneIndices[0]), 0, AfxHint())))
                    AfxThrowError();
                else
                {
                    if (!transplanted) origBoneMap = dstBoneIndices;
                    else if (biasCnt && !(origBoneMap = AfxAllocate(mmu, biasCnt, sizeof(origBoneMap[0]), 0, AfxHint())))
                        AfxThrowError();

                    if (!err)
                    {
                        for (afxNat j = 0; j < biasCnt; j++)
                        {
                            afxString biasId;
                            AfxGetMeshBiasName(msh, j, &biasId);

                            if (1)
                            {
                                if (!AfxFindSkeletonJoint(skl, &biasId, &dstBoneIndices[j]))
                                {
                                    AfxError("Pivot '%.*s' not found in the destination skeleton.", AfxPushString(&biasId));
                                    AfxAssert(dstBoneIndices[j] == AFX_INVALID_INDEX);
                                }

                                if (transplanted && !AfxFindSkeletonJoint(origSkl, &biasId, &origBoneMap[j]))
                                {
                                    AfxError("Pivot '%.*s' not found in the source skeleton.", AfxPushString(&biasId));
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

_AKX void AfxGetModelInitialPlacement(afxModel mdl, afxReal m[4][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxComposeTransformM4d(&mdl->init, m);
}

_AKX void AfxResetModelInitialPlacement(afxModel mdl, afxTransform const* xform)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxCopyTransform(&mdl->init, xform);
}

_AKX afxSkeleton AfxGetModelSkeleton(afxModel mdl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    afxSkeleton skl = mdl->skl;
    AfxAssertObjects(1, &skl, afxFcc_SKL);
    return skl;
}

_AKX afxBool AfxGetModelId(afxModel mdl, afxString* id)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssert(id);
    return AfxResolveStrings2(mdl->strc, 1, &mdl->id, id);
}

_AKX afxNat AfxGetModelCapacity(afxModel mdl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    return mdl->slotCnt;
}

_AKX afxNat AfxCountRiggedMeshes(afxModel mdl)
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

_AKX afxError _AfxMdlDtor(afxModel mdl)
{
    AfxEntry("mdl=%p", mdl);

    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);

    afxSimulation sim = AfxGetObjectProvider(mdl);
    afxMmu mmu = AfxGetSimulationMmu(sim);
    
    for (afxNat i = 0; i < mdl->slotCnt; i++)
        AfxRigMeshes(mdl, NIL, i, 1, NIL);

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

_AKX afxError _AfxMdlCtor(afxModel mdl, afxCookie const *cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);

    afxSimulation sim = cookie->udd[0];
    afxModelBlueprint const*mdlb = cookie->udd[1];
    afxMesh* meshes = mdlb->meshes;
    afxStringCatalog strc = mdlb->strc;
    mdlb += cookie->no;

    if (!(mdl->strc = strc))
        AfxResetString(&mdl->id);
    else if (!AfxCatalogStrings2(strc, 1, &mdlb->id.str.str, &mdl->id))
        AfxThrowError();
    else
        AfxReacquireObjects(1, (void*[]) { strc });

    if (!err)
    {
        afxSkeleton skl = mdlb->skl;
        afxTransform const* init = &mdlb->init;
        afxNat meshCnt = mdlb->mshCnt;
        afxNat baseMshIdx = 0;

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

                if (AfxRigMeshes(mdl, NIL, 0, meshCnt, &meshes[baseMshIdx]))
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
            afxString s;

            if (!mdl->strc)
                AfxResetString(&s);
            else
                AfxResolveStrings2(mdl->strc, 1, &mdl->id, &s);

            AfxEcho("Model <%.*s> assembled. %p\n    %u joints for %u rigged meshes.\n", AfxPushString(&s), mdl, mdl->skl->jointCnt, mdl->slotCnt);
        }
    }
    return err;
}

_AKX afxClassConfig _AfxMdlClsConfig =
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

_AKX afxError AfxAssembleModel(afxSimulation sim, afxNat cnt, afxModelBlueprint const blueprints[], afxModel models[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AfxAssert(blueprints);
    AfxAssert(models);
    AfxAssert(cnt);

    if (AfxAcquireObjects(AwxGetModelClass(sim), cnt, (afxObject*)models, (void const*[]) { sim, blueprints }))
        AfxThrowError();

    return err;
}

_AKX void AfxTransformModels(afxM3d const ltm, afxM3d const iltm, afxReal ltmTol, afxV3d const atv, afxReal atvTol, afxFlags flags, afxNat cnt, afxModel models[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(atv);
    AfxAssert(ltm);
    AfxAssert(iltm);
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
                AfxTransformSkeletons(ltm, iltm, ltmTol, atv, atvTol, 1, &skl);
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
                mshCnt = AfxEnumerateRiggedMeshes(mdl2, baseSlotIdx, batchSiz, meshes);
                baseSlotIdx += mshCnt;

                // WARNING: What to do if mesh is shared among other models of strange asset?
                AfxTransformMeshes(ltm, iltm, ltmTol, atv, atvTol, flags, mshCnt, meshes);
            }

            if (remaining)
            {
                mshCnt = AfxEnumerateRiggedMeshes(mdl2, baseSlotIdx, remaining, meshes);
                // WARNING: What to do if mesh is shared among other models of strange asset?
                AfxTransformMeshes(ltm, iltm, ltmTol, atv, atvTol, flags, mshCnt, meshes);
            }

            AfxAssimilateTransforms(ltm, iltm, atv, 1, &mdl2->init, &mdl2->init);
        }
    }
}
