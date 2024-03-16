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
    AfxAssertRange(mdl->rigCnt, slotIdx, 1);
    return mdl->rigs[slotIdx].sklOrig != mdl->skl;
}

_AKX afxSkeleton AfxGetRiggedMeshOriginalSkeleton(afxModel mdl, afxNat slotIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->rigCnt, slotIdx, 1);
    return mdl->rigs[slotIdx].sklOrig;
}

_AKX afxNat const* AfxGetRiggedMeshMapping(afxModel mdl, afxNat slotIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->rigCnt, slotIdx, 1);
    return mdl->rigs[slotIdx].jointMap;
}

_AKX afxNat const* AfxGetRiggedMeshOriginalMapping(afxModel mdl, afxNat slotIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->rigCnt, slotIdx, 1);
    return mdl->rigs[slotIdx].jointMapOrig;
}

_AKX afxMaterial AfxGetRiggedMeshTxd(afxModel mdl, afxNat slotIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->rigCnt, slotIdx, 1);
    afxMaterial txd;

    if ((txd = mdl->rigs[slotIdx].txd))
    {
        AfxAssertObjects(1, &txd, afxFcc_MTL);
    }
    return txd;
}

_AKX void AfxSetRiggedMeshTxd(afxModel mdl, afxNat slotIdx, afxMaterial mtl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->rigCnt, slotIdx, 1);
    afxMaterial curr;

    if (mtl != (curr = mdl->rigs[slotIdx].txd))
    {
        if (mtl)
        {
            AfxAssertObjects(1, &mtl, afxFcc_MTL);
            AfxReacquireObjects(1, (void*[]) { mtl });
        }

        mdl->rigs[slotIdx].txd = mtl;

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
    AfxAssertRange(mdl->rigCnt, slotIdx, 1);    
    afxMaterial txd = mdl->rigs[slotIdx].txd;
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

_AKX afxNat AfxEnumerateRiggedMeshes(afxModel mdl, afxNat baseSlot, afxNat rigCnt, afxMesh meshes[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->rigCnt, baseSlot, rigCnt);
    afxNat rslt = 0;

    for (afxNat i = 0; i < rigCnt; i++)
    {
        afxNat slotIdx = baseSlot + i;
        AfxAssertRange(mdl->rigCnt, slotIdx, 1);
        afxMesh msh2 = mdl->rigs[slotIdx].msh;

        if (msh2)
        {
            AfxAssertObjects(1, &msh2, afxFcc_MSH);
            ++rslt;
        }
        meshes[i] = msh2;
    }
    return rslt;
}

_AKX void AfxComputeRiggedMeshMatrices(afxModel mdl, afxNat slotIdx, awxPoseBuffer const* posb, afxNat baseJoint, afxNat jointCnt, afxM4d m[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->rigCnt, slotIdx, 1);
    afxMesh msh = mdl->rigs[slotIdx].msh;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    afxNat totalBiasCnt = msh->biasCnt;

    AfxAssertRange(totalBiasCnt, baseJoint, jointCnt);
    afxNat chosenBoneCnt = baseJoint + jointCnt;
    AfxAssert(totalBiasCnt >= chosenBoneCnt);

    afxV4d* w = AfxPoseBufferGetWorldMatrix(posb, 0);
    afxM4d* iw = AfxGetSkeletonIwm(mdl->rigs[slotIdx].sklOrig, 0);
    afxNat const* mapping = AfxGetRiggedMeshMapping(mdl, slotIdx);
    afxNat const* mappingOrig = AfxGetRiggedMeshOriginalMapping(mdl, slotIdx);

    for (afxNat i = 0, artIdx = baseJoint; artIdx < chosenBoneCnt; artIdx++)
    {
        AfxMultiplyAtm4d(m[i], iw[mappingOrig[i]], w[mapping[i]]);
    }
}

_AKX afxError AfxRigMeshes(afxModel mdl, afxSkeleton sklOrig, afxNat baseSlot, afxNat rigCnt, afxMesh const meshes[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->rigCnt, baseSlot, rigCnt);

    afxSkeleton skl = mdl->skl;
    AfxTryAssertObjects(1, &skl, afxFcc_SKL);
    AfxTryAssertObjects(1, &sklOrig, afxFcc_SKL);
    afxBool transplanted = sklOrig && (sklOrig != skl);

    afxSimulation sim = AfxGetObjectProvider(mdl);
    afxMmu mmu = AfxGetSimulationMmu(sim);

    for (afxNat i = 0; i < rigCnt; i++)
    {
        afxNat slotIdx = baseSlot + i;
        AfxAssertRange(mdl->rigCnt, slotIdx, 1);
        afxMeshRig* slot = &mdl->rigs[slotIdx];
        afxMesh mshCurr = slot->msh;
        afxMesh msh = meshes ? meshes[i] : NIL;

        if (mshCurr != msh)
        {
            afxNat* jointMapOrig = NIL;
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
                    if (!transplanted) jointMapOrig = dstBoneIndices;
                    else if (biasCnt && !(jointMapOrig = AfxAllocate(mmu, biasCnt, sizeof(jointMapOrig[0]), 0, AfxHint())))
                        AfxThrowError();

                    if (!err)
                    {
                        for (afxNat j = 0; j < biasCnt; j++)
                        {
                            afxString biasId;
                            AfxGetMeshBiasName(msh, j, &biasId);

                            if (1)
                            {
                                if (AFX_INVALID_INDEX != (dstBoneIndices[j] = AfxFindSkeletonJoint(skl, &biasId)))
                                {
                                    AfxError("Pivot '%.*s' not found in the destination skeleton.", AfxPushString(&biasId));
                                    AfxAssert(dstBoneIndices[j] == AFX_INVALID_INDEX);
                                }

                                if (transplanted && (AFX_INVALID_INDEX != (jointMapOrig[j] = AfxFindSkeletonJoint(sklOrig, &biasId))))
                                {
                                    AfxError("Pivot '%.*s' not found in the source skeleton.", AfxPushString(&biasId));
                                    AfxAssert(jointMapOrig[j] == AFX_INVALID_INDEX);
                                }
                            }
                        }

                        err = NIL; // skip any soft fail by bone indexing.
                         
                        AfxReacquireObjects(1, (void*[]) { msh });

                        if (sklOrig && (sklOrig != mdl->skl))
                            AfxReacquireObjects(1, (void*[]) { sklOrig });
                    }

                    if (err && dstBoneIndices)
                        AfxDeallocate(mmu, dstBoneIndices);
                }
            }

            if (!err)
            {
                if (mshCurr) // detach operation
                {
                    if (slot->jointMap)
                        AfxDeallocate(mmu, slot->jointMap);

                    slot->jointMap = NIL;

                    if (slot->jointMapOrig && (slot->jointMapOrig != slot->jointMap))
                        AfxDeallocate(mmu, slot->jointMapOrig);

                    slot->jointMapOrig = NIL;

                    if (slot->sklOrig && (slot->sklOrig != mdl->skl))
                    {
                        AfxReleaseObjects(1, (void*[]) { slot->sklOrig });
                        slot->sklOrig = NIL;
                    }

                    AfxAssertObjects(1, &mshCurr, afxFcc_MSH);
                    AfxReleaseObjects(1, (void*[]) { mshCurr });
                    slot->msh = NIL;
                }

                if (msh)
                {
                    slot->msh = msh;
                    slot->sklOrig = sklOrig;
                    slot->jointMapOrig = jointMapOrig;
                    slot->jointMap = dstBoneIndices;
                }
            }
        }
    }
    return err;
}

_AKX void AfxComputeModelBaseOffset(afxModel mdl, afxM4d m)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssert(m);
    AfxComputeTransformM4d(&mdl->init, m);
}

_AKX void AfxUpdateModelBaseOffset(afxModel mdl, afxTransform const* xform)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssert(xform);
    AfxCopyTransform(&mdl->init, xform);
}

_AKX afxSkeleton AfxGetModelSkeleton(afxModel mdl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    afxSkeleton skl = mdl->skl;
    AfxAssertObjects(1, &skl, afxFcc_SKL); // have a skeleton is mandatory
    return skl;
}

_AKX afxBool AfxGetModelId(afxModel mdl, afxString* id)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssert(id);
    return AfxResolveStrings2(mdl->strc, 1, &mdl->id, id);
}

_AKX afxNat AfxCountModelRigs(afxModel mdl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    return mdl->rigCnt;
}

_AKX afxNat AfxCountRiggedMeshes(afxModel mdl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    afxNat cap = mdl->rigCnt;
    afxNat rslt = 0;

    for (afxNat i = 0; i < cap; i++)
    {
        afxMesh msh = mdl->rigs[i].msh;

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
    
    for (afxNat i = 0; i < mdl->rigCnt; i++)
        AfxRigMeshes(mdl, NIL, i, 1, NIL);

    if (mdl->rigs)
        AfxDeallocate(mmu, mdl->rigs);

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

        mdl->rigCnt = 0;
        mdl->rigs = NIL;

        if (meshCnt && !(mdl->rigs = AfxAllocate(NIL, meshCnt, sizeof(mdl->rigs[0]), 0, AfxHint()))) AfxThrowError();
        else
        {
            if (meshCnt)
            {
                mdl->rigCnt = meshCnt;

                AfxZero(meshCnt, sizeof(mdl->rigs[0]), mdl->rigs);

                if (AfxRigMeshes(mdl, NIL, 0, meshCnt, &meshes[baseMshIdx]))
                    AfxThrowError();
            }

            AfxAssert(mdl->rigCnt == meshCnt);
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

            AfxEcho("Model <%.*s> assembled. %p\n    %u joints for %u rigged meshes.\n", AfxPushString(&s), mdl, mdl->skl->jointCnt, mdl->rigCnt);
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
            afxNat totalMshCnt = AfxCountModelRigs(mdl2);
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
