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
#include "qwadro/mem/afxMappedString.h"

_AKX afxBool AfxRiggedMeshIsTransplanted(afxModel mdl, afxNat rigIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->rigCnt, rigIdx, 1);
    afxSkeleton sklOrig = mdl->rigs[rigIdx].sklOrig;
    AfxTryAssertObjects(1, &sklOrig, afxFcc_SKL);
    return (sklOrig && sklOrig != mdl->skl);
}

_AKX afxSkeleton AfxGetRiggedMeshDonor(afxModel mdl, afxNat rigIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->rigCnt, rigIdx, 1);
    return mdl->rigs[rigIdx].sklOrig;
}

_AKX afxNat const* AfxGetRiggedMeshBiasToJointMapping(afxModel mdl, afxNat rigIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->rigCnt, rigIdx, 1);
    return mdl->rigs[rigIdx].biasToJointMap;
}

_AKX afxNat const* AfxGetRiggedMeshBiasFromJointMapping(afxModel mdl, afxNat rigIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->rigCnt, rigIdx, 1);
    return mdl->rigs[rigIdx].biasFromJointMap;
}

_AKX afxMaterial AfxGetRiggedMeshTxd(afxModel mdl, afxNat rigIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->rigCnt, rigIdx, 1);
    afxMaterial txd;

    if ((txd = mdl->rigs[rigIdx].txd))
    {
        AfxAssertObjects(1, &txd, afxFcc_MTL);
    }
    return txd;
}

_AKX void AfxSetRiggedMeshTxd(afxModel mdl, afxNat rigIdx, afxMaterial mtl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->rigCnt, rigIdx, 1);
    afxMaterial curr;

    if (mtl != (curr = mdl->rigs[rigIdx].txd))
    {
        if (mtl)
        {
            AfxAssertObjects(1, &mtl, afxFcc_MTL);
            AfxReacquireObjects(1, (void*[]) { mtl });
        }

        mdl->rigs[rigIdx].txd = mtl;

        if (curr)
        {
            AfxAssertObjects(1, &curr, afxFcc_MTL);
            AfxReleaseObjects(1, (void*[]) { curr });
        }
    }
}

_AKX afxMaterial AfxFindRiggedMeshMaterial(afxModel mdl, afxNat rigIdx, afxString const* id)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->rigCnt, rigIdx, 1);    
    afxMaterial txd = mdl->rigs[rigIdx].txd;
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

_AKX afxNat AfxEnumerateRiggedMeshes(afxModel mdl, afxNat baseRig, afxNat rigCnt, afxMesh meshes[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->rigCnt, baseRig, rigCnt);
    afxNat rslt = 0;

    for (afxNat i = 0; i < rigCnt; i++)
    {
        afxNat rigIdx = baseRig + i;
        AfxAssertRange(mdl->rigCnt, rigIdx, 1);
        afxMesh msh2 = mdl->rigs[rigIdx].msh;

        if (msh2)
        {
            AfxAssertObjects(1, &msh2, afxFcc_MSH);
            ++rslt;
        }
        meshes[i] = msh2;
    }
    return rslt;
}

_AKX void AfxComputeRiggedMeshMatrices(afxModel mdl, afxNat rigIdx, awxPoseBuffer const* posb, afxNat baseBias, afxNat biasCnt, afxM4d m[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->rigCnt, rigIdx, 1);
    afxMesh msh = mdl->rigs[rigIdx].msh;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    afxNat totalBiasCnt = msh->biasCnt;

    AfxAssertRange(totalBiasCnt, baseBias, biasCnt);
    afxNat chosenBiasCnt = baseBias + biasCnt;
    AfxAssert(totalBiasCnt >= chosenBiasCnt);

    afxM4d* w = AfxGetWorldMatrices(posb, 0);
    afxM4d* iw = AfxGetSkeletonIwMatrices(mdl->rigs[rigIdx].sklOrig, 0);
    afxNat const* mapping = AfxGetRiggedMeshBiasToJointMapping(mdl, rigIdx);
    afxNat const* mappingOrig = AfxGetRiggedMeshBiasFromJointMapping(mdl, rigIdx);

    for (afxNat i = 0, artIdx = baseBias; artIdx < chosenBiasCnt; artIdx++)
    {
        AfxMultiplyAtm4d(m[i], iw[mappingOrig[i]], w[mapping[i]]);
    }
}

_AKX afxError AfxRigMeshes(afxModel mdl, afxSkeleton sklOrig, afxNat baseRig, afxNat rigCnt, afxMesh const meshes[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->rigCnt, baseRig, rigCnt);

    afxSkeleton skl = mdl->skl;
    AfxTryAssertObjects(1, &skl, afxFcc_SKL);
    AfxTryAssertObjects(1, &sklOrig, afxFcc_SKL);
    afxBool transplanted = sklOrig && (sklOrig != skl);

    afxSimulation sim = AfxGetObjectProvider(mdl);
    afxMmu mmu = AfxGetSimulationMmu(sim);

    for (afxNat i = 0; i < rigCnt; i++)
    {
        afxNat rigIdx = baseRig + i;
        AfxAssertRange(mdl->rigCnt, rigIdx, 1);
        afxMeshRig* rig = &mdl->rigs[rigIdx];
        afxMesh mshCurr = rig->msh;
        afxMesh msh = meshes ? meshes[i] : NIL;

        if (mshCurr != msh)
        {
            afxNat* biasFromJointMap = NIL;
            afxNat* biasToJointMap = NIL;

            if (msh) // attach operation
            {
                AfxAssertObjects(1, &msh, afxFcc_MSH);
                afxStringBase strb = msh->strb;

                afxNat biasCnt = msh->biasCnt;

                if (biasCnt && !(biasToJointMap = AfxAllocate(biasCnt, sizeof(biasToJointMap[0]), 0, AfxHere())))
                    AfxThrowError();
                else
                {
                    if (!transplanted) biasFromJointMap = biasToJointMap;
                    else if (biasCnt && !(biasFromJointMap = AfxAllocate(biasCnt, sizeof(biasFromJointMap[0]), 0, AfxHere())))
                        AfxThrowError();

                    if (!err)
                    {
                        for (afxNat j = 0; j < biasCnt; j++)
                        {
                            afxString biasId;
                            AfxGetMeshBiasName(msh, j, &biasId);

                            if (1)
                            {
                                if (AFX_INVALID_INDEX == (biasToJointMap[j] = AfxFindSkeletonJoint(skl, &biasId)))
                                {
                                    AfxLogError("Pivot '%.*s' not found in the destination skeleton.", AfxPushString(&biasId));
                                    AfxAssert(biasToJointMap[j] == AFX_INVALID_INDEX);
                                }

                                if (transplanted && (AFX_INVALID_INDEX == (biasFromJointMap[j] = AfxFindSkeletonJoint(sklOrig, &biasId))))
                                {
                                    AfxLogError("Pivot '%.*s' not found in the source skeleton.", AfxPushString(&biasId));
                                    AfxAssert(biasFromJointMap[j] == AFX_INVALID_INDEX);
                                }
                            }
                        }

                        err = NIL; // skip any soft fail by bone indexing.
                         
                        AfxReacquireObjects(1, (void*[]) { msh });

                        if (sklOrig && (sklOrig != mdl->skl))
                            AfxReacquireObjects(1, (void*[]) { sklOrig });
                    }

                    if (err && biasToJointMap)
                        AfxDeallocate(biasToJointMap);
                }
            }

            if (!err)
            {
                if (mshCurr) // detach operation
                {
                    if (rig->biasToJointMap)
                    {
                        AfxDeallocate(rig->biasToJointMap);
                        rig->biasToJointMap = NIL;
                    }

                    if (rig->biasFromJointMap && (rig->biasFromJointMap != rig->biasToJointMap))
                    {
                        AfxDeallocate(rig->biasFromJointMap);
                        rig->biasFromJointMap = NIL;
                    }

                    if (AfxRiggedMeshIsTransplanted(mdl, rigIdx))
                    {
                        afxSkeleton sklOrig = rig->sklOrig;
                        AfxTryAssertObjects(1, &sklOrig, afxFcc_SKL);
                        AfxReleaseObjects(1, (void*[]) { sklOrig });
                        rig->sklOrig = NIL;
                    }

                    AfxAssertObjects(1, &mshCurr, afxFcc_MSH);
                    AfxReleaseObjects(1, (void*[]) { mshCurr });
                    rig->msh = NIL;
                }

                if (msh)
                {
                    rig->msh = msh;
                    rig->sklOrig = sklOrig;
                    rig->biasFromJointMap = biasFromJointMap;
                    rig->biasToJointMap = biasToJointMap;
                }
            }
        }
    }
    return err;
}

_AKX void AfxComputeModelDisplacement(afxModel mdl, afxM4d m)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssert(m);
    AfxComputeTransformM4d(&mdl->displacement, m);
}

_AKX void AfxUpdateModelDisplacement(afxModel mdl, afxTransform const* xform)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssert(xform);
    AfxCopyTransform(&mdl->displacement, xform);
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
    return AfxResolveStrings2(mdl->strb, 1, &mdl->id, id);
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
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);

    if (mdl->rigs)
    {
        for (afxNat i = 0; i < mdl->rigCnt; i++)
            AfxRigMeshes(mdl, NIL, i, 1, NIL);

        AfxDeallocate(mdl->rigs);
    }

    afxSkeleton skl;

    if ((skl = mdl->skl))
    {
        AfxAssertObjects(1, &skl, afxFcc_SKL);
        AfxReleaseObjects(1, (void*[]) { skl });
    }
    //if (mdl->uri)

    if (mdl->strb)
        AfxReleaseObjects(1, (void*[]) { mdl->strb });

    return err;
}

_AKX afxError _AfxMdlCtor(afxModel mdl, afxCookie const *cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);

    afxSimulation sim = cookie->udd[0];
    afxModelBlueprint const*mdlb = cookie->udd[1];
    afxMesh* meshes = mdlb->meshes;
    afxStringBase strb = mdlb->strb;
    mdlb += cookie->no;

    if (!(mdl->strb = strb))
        AfxResetString(&mdl->id);
    else if (!AfxCatalogStrings2(strb, 1, &mdlb->id.str.str, &mdl->id))
        AfxThrowError();
    else
        AfxReacquireObjects(1, (void*[]) { strb });

    if (!err)
    {
        afxSkeleton skl = mdlb->skl;
        afxTransform const* displacement = &mdlb->displacement;
        afxNat rigCnt = mdlb->rigCnt;
        afxNat baseMshIdx = 0;

        if ((mdl->skl = skl))
        {
            AfxAssertObjects(1, &skl, afxFcc_SKL);
            AfxReacquireObjects(1, (void*[]) { skl });
        }

        if (displacement)
            AfxCopyTransform(&mdl->displacement, displacement);
        else
            AfxResetTransform(&mdl->displacement);

        AfxResetAabb(mdl->aabb);

        mdl->rigCnt = 0;
        mdl->rigs = NIL;

        if (rigCnt && !(mdl->rigs = AfxAllocate(rigCnt, sizeof(mdl->rigs[0]), 0, AfxHere()))) AfxThrowError();
        else
        {
            if (rigCnt)
            {
                mdl->rigCnt = rigCnt;

                AfxZero(mdl->rigs, rigCnt * sizeof(mdl->rigs[0]));

                if (AfxRigMeshes(mdl, NIL, 0, rigCnt, &meshes[baseMshIdx]))
                    AfxThrowError();
            }

            AfxAssert(mdl->rigCnt == rigCnt);
        }

        if (err)
        {
            AfxReleaseObjects(1, (void*[]) { skl });
        }

        if (!err)
        {
            afxString s;

            if (!mdl->strb)
                AfxResetString(&s);
            else
                AfxResolveStrings2(mdl->strb, 1, &mdl->id, &s);

            AfxLogEcho("Model <%.*s> assembled. %p\n    %u joints for %u rigged meshes.\n", AfxPushString(&s), mdl, mdl->skl->jointCnt, mdl->rigCnt);
        }
    }
    return err;
}

_AKX afxClassConfig _AfxMdlMgrCfg =
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

            AfxAssimilateTransforms(ltm, iltm, atv, 1, &mdl2->displacement, &mdl2->displacement);
        }
    }
}
