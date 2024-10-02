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

#define _AMX_DRAW_C
#define _AMX_MODEL_C
#include "../dev/AmxCadImplKit.h"

_AMXINL afxSkeleton AfxGetModelSkeleton(afxModel mdl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    afxSkeleton skl = mdl->skl;
    AfxAssertObjects(1, &skl, afxFcc_SKL); // have a skeleton is mandatory
    return skl;
}

_AMXINL afxBool AfxGetModelUrn(afxModel mdl, afxString* id)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssert(id);
    *id = mdl->urn;
    return mdl->urn.len;
}

_AMXINL afxNat AfxCountModelRigs(afxModel mdl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    return mdl->rigCnt;
}

_AMXINL afxNat AfxCountRiggedMeshes(afxModel mdl)
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

_AMXINL void AfxComputeModelDisplacement(afxModel mdl, afxM4d m)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssert(m);
    AfxComputeCompositeTransformM4d(&mdl->displacement, m);
}

_AMXINL void AfxUpdateModelDisplacement(afxModel mdl, afxTransform const* t)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssert(t);
    AfxCopyTransform(&mdl->displacement, t);
}

_AMXINL afxBool AfxRiggedMeshIsTransplanted(afxModel mdl, afxNat rigIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->rigCnt, rigIdx, 1);
    afxSkeleton sklOrig = mdl->rigs[rigIdx].sklOrig;
    AfxTryAssertObjects(1, &sklOrig, afxFcc_SKL);
    return (sklOrig && sklOrig != mdl->skl);
}

_AMXINL afxSkeleton AfxGetRiggedMeshSkeleton(afxModel mdl, afxNat rigIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->rigCnt, rigIdx, 1);
    return mdl->rigs[rigIdx].sklOrig;
}

_AMXINL afxNat const* AfxGetRiggedMeshBiasToJointMapping(afxModel mdl, afxNat rigIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->rigCnt, rigIdx, 1);
    return mdl->rigs[rigIdx].biasToJointMap;
}

_AMXINL afxNat const* AfxGetRiggedMeshBiasFromJointMapping(afxModel mdl, afxNat rigIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->rigCnt, rigIdx, 1);
    return mdl->rigs[rigIdx].biasFromJointMap;
}

_AMXINL afxMaterial AfxGetRiggedMeshTxd(afxModel mdl, afxNat rigIdx)
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

_AMXINL void AfxSetRiggedMeshTxd(afxModel mdl, afxNat rigIdx, afxMaterial mtl)
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
            AfxReacquireObjects(1, &mtl);
        }

        mdl->rigs[rigIdx].txd = mtl;

        if (curr)
        {
            AfxAssertObjects(1, &curr, afxFcc_MTL);
            AfxReleaseObjects(1, &curr);
        }
    }
}

_AMXINL afxMaterial AfxFindRiggedMeshMaterial(afxModel mdl, afxNat rigIdx, afxString const* id)
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

_AMXINL afxNat AfxEnumerateRiggedMeshes(afxModel mdl, afxNat baseRig, afxNat rigCnt, afxMesh meshes[])
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

_AMXINL void AfxComputeRiggedMeshMatrices(afxModel mdl, afxNat rigIdx, afxPoseBuffer const posb, afxNat baseBias, afxNat biasCnt, afxM4d m[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->rigCnt, rigIdx, 1);
    afxMesh msh = mdl->rigs[rigIdx].msh;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    afxNat totalBiasCnt = AfxCountMeshBiases(msh, 0);

    AfxAssertRange(totalBiasCnt, baseBias, biasCnt);
    afxNat chosenBiasCnt = baseBias + biasCnt;
    AfxAssert(totalBiasCnt >= chosenBiasCnt);

    afxM4d* w = AfxGetWorldMatrices(posb, 0);
    afxM4d* iw = AfxGetSkeletonMatrices(mdl->rigs[rigIdx].sklOrig, 0);
    afxNat const* mapping = AfxGetRiggedMeshBiasToJointMapping(mdl, rigIdx);
    afxNat const* mappingOrig = AfxGetRiggedMeshBiasFromJointMapping(mdl, rigIdx);

    for (afxNat i = 0, artIdx = baseBias; artIdx < chosenBiasCnt; artIdx++)
    {
        AfxM4dMultiplyAffine(m[i], iw[mappingOrig[i]], w[mapping[i]]);
    }
}

_AMX afxError AfxRigMeshes(afxModel mdl, afxSkeleton sklOrig, afxNat baseRig, afxNat rigCnt, afxMesh const meshes[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AfxAssertRange(mdl->rigCnt, baseRig, rigCnt);

    afxSkeleton skl = mdl->skl;
    AfxTryAssertObjects(1, &skl, afxFcc_SKL);
    AfxTryAssertObjects(1, &sklOrig, afxFcc_SKL);
    afxBool transplanted = sklOrig && (sklOrig != skl);

    for (afxNat i = 0; i < rigCnt; i++)
    {
        afxNat rigIdx = baseRig + i;
        AfxAssertRange(mdl->rigCnt, rigIdx, 1);
        akxMeshRig* rig = &mdl->rigs[rigIdx];
        afxMesh mshCurr = rig->msh;
        afxMesh msh = meshes ? meshes[i] : NIL;

        if (mshCurr != msh)
        {
            afxNat* biasFromJointMap = NIL;
            afxNat* biasToJointMap = NIL;

            if (msh) // attach operation
            {
                AfxAssertObjects(1, &msh, afxFcc_MSH);

                afxNat biasCnt = AfxCountMeshBiases(msh, 0);

                if (biasCnt && !(biasToJointMap = AfxAllocate(biasCnt, sizeof(biasToJointMap[0]), 0, AfxHere()))) AfxThrowError();
                else
                {
                    if (!transplanted) biasFromJointMap = biasToJointMap;
                    else if (biasCnt && !(biasFromJointMap = AfxAllocate(biasCnt, sizeof(biasFromJointMap[0]), 0, AfxHere())))
                        AfxThrowError();

                    if (!err)
                    {
                        afxString* biasTags = AfxGetMeshBiasTags(msh, 0);
                        afxNat biasMappedCnt = 0;

                        if (biasCnt != (biasMappedCnt = AfxFindSkeletonJoints(skl, biasCnt, biasTags, biasToJointMap)))
                        {
                            //AfxLogAdvertence("%u biases mapped to joints from %u", biasMappedCnt, biasCnt);
                        }

                        if (transplanted)
                        {
                            if (biasCnt != (biasMappedCnt = AfxFindSkeletonJoints(skl, biasCnt, biasTags, biasFromJointMap)))
                            {
                                //AfxLogAdvertence("%u joints to biases mapped from %u", biasMappedCnt, biasCnt);
                            }
                        }

                        AfxLogEcho("Mesh %p linked to model %p.\n    Listing %u bias-joint mapping..:", msh, mdl, biasCnt);

                        for (afxNat j = 0; j < biasCnt; j++)
                        {
                            AfxLogEcho("    %3i --> %i <-- %i <%.*s> ", (afxInt)j, (afxInt)biasToJointMap[j], (afxInt)biasFromJointMap[j], AfxPushString(&biasTags[j]));
                        }
#if 0
                        for (afxNat j = 0; j < biasCnt; j++)
                        {
                            afxString biasId = *AfxGetMeshBiasTags(msh, j);

                            if (1)
                            {
                                if (AFX_INVALID_INDEX == (biasToJointMap[j] = AfxFindJoint(skl, &biasId)))
                                {
                                    AfxLogError("Pivot '%.*s' not found in rig skeleton.", AfxPushString(&biasId));
                                    AfxAssert(biasToJointMap[j] == AFX_INVALID_INDEX);
                                }

                                if (transplanted && (AFX_INVALID_INDEX == (biasFromJointMap[j] = AfxFindJoint(sklOrig, &biasId))))
                                {
                                    AfxLogError("Pivot '%.*s' not found in original skeleton.", AfxPushString(&biasId));
                                    AfxAssert(biasFromJointMap[j] == AFX_INVALID_INDEX);
                                }
                            }
                        }

                        err = NIL; // skip any soft fail by bone indexing.
#endif                    
                        AfxReacquireObjects(1, &msh);

                        if (sklOrig && (sklOrig != mdl->skl))
                            AfxReacquireObjects(1, &sklOrig);
                    }

                    if (err && biasFromJointMap && biasFromJointMap != biasToJointMap)
                        AfxDeallocate(biasFromJointMap);

                    if (err && biasToJointMap)
                        AfxDeallocate(biasToJointMap);
                }
            }

            if (!err)
            {
                if (mshCurr) // detach operation
                {
                    AfxAssertObjects(1, &mshCurr, afxFcc_MSH);

                    if (rig->biasFromJointMap && (rig->biasFromJointMap != rig->biasToJointMap))
                    {
                        AfxDeallocate(rig->biasFromJointMap);
                        rig->biasFromJointMap = NIL;
                    }

                    if (rig->biasToJointMap)
                    {
                        AfxDeallocate(rig->biasToJointMap);
                        rig->biasToJointMap = NIL;
                    }

                    if (AfxRiggedMeshIsTransplanted(mdl, rigIdx))
                    {
                        afxSkeleton sklOrig = rig->sklOrig;
                        AfxTryAssertObjects(1, &sklOrig, afxFcc_SKL);
                        AfxReleaseObjects(1, &sklOrig);
                        rig->sklOrig = NIL;
                    }

                    AfxReleaseObjects(1, &mshCurr);
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

_AMX afxError _AmxMdlDtorCb(afxModel mdl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);

    if (mdl->rigCnt)
        AfxRigMeshes(mdl, NIL, 0, mdl->rigCnt, NIL);

    afxObjectStash stashes[] =
    {
        {
            .cnt = mdl->rigCnt,
            .siz = sizeof(mdl->rigs[0]),
            .var = (void**)&mdl->rigs
        }
    };

    if (AfxDeallocateInstanceData(mdl, AFX_COUNTOF(stashes), stashes))
        AfxThrowError();

    afxSkeleton skl = mdl->skl;

    if (skl)
    {
        AfxAssertObjects(1, &skl, afxFcc_SKL);
        AfxReleaseObjects(1, &skl);
    }

    return err;
}

_AMX afxError _AmxMdlCtorCb(afxModel mdl, void** args, afxNat invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);

    afxSimulation sim = args[0];
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    akxModelBlueprint const*mdlb = args[1];
    afxMesh* meshes = mdlb->meshes;
    mdlb += invokeNo;

    if (!AfxCatalogStrings(AfxGetModelUrnStringBase(sim), 1, &mdlb->id.str, &mdl->urn)) AfxThrowError();
    else
    {
        afxSkeleton skl = mdlb->skl;
        afxTransform const* displacement = &mdlb->displacement;
        afxNat rigCnt = AfxMax(1, mdlb->rigCnt);
        afxNat baseMshIdx = 0;

        if ((mdl->skl = skl))
        {
            AfxAssertObjects(1, &skl, afxFcc_SKL);
            AfxReacquireObjects(1, &skl);
        }

        if (displacement)
            AfxCopyTransform(&mdl->displacement, displacement);
        else
            AfxResetTransform(&mdl->displacement);

        AfxBoxReset(mdl->aabb);

        mdl->rigCnt = 0;
        mdl->rigs = NIL;

        afxObjectStash stashes[] =
        {
            {
                .cnt = rigCnt,
                .siz = sizeof(mdl->rigs[0]),
                .var = (void**)&mdl->rigs
            }
        };

        if (AfxAllocateInstanceData(mdl, AFX_COUNTOF(stashes), stashes)) AfxThrowError();
        else
        {
            AfxAssert(mdl->rigs);

            AfxZero(mdl->rigs, rigCnt * sizeof(mdl->rigs[0]));
            mdl->rigCnt = rigCnt;

            if (AfxRigMeshes(mdl, NIL, 0, rigCnt, &meshes[baseMshIdx]))
                AfxThrowError();

            AfxAssert(mdl->rigCnt == rigCnt);

            if (err)
            {
                AfxDeallocateInstanceData(mdl, AFX_COUNTOF(stashes), stashes);
            }
        }

        if (err)
        {
            AfxReleaseObjects(1, &skl);
        }
    }

    if (!err)
    {
        afxString s;
        AfxGetModelUrn(mdl, &s);
        afxNat jntCnt = AfxCountSkeletonJoints(mdl->skl, 0);
        AfxLogEcho("Animable model <%.*s> assembled at %p. %u mesh rigs for %u joints.", AfxPushString(&s), mdl, mdl->rigCnt, jntCnt);
    }
    return err;
}

_AMX afxClassConfig _AmxMdlClsCfg =
{
    .fcc = afxFcc_MDL,
    .name = "Model",
    .desc = "Animable Model",
    .fixedSiz = sizeof(AFX_OBJECT(afxModel)),
    .ctor = (void*)_AmxMdlCtorCb,
    .dtor = (void*)_AmxMdlDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AMX afxError AfxAssembleModels(afxSimulation sim, afxNat cnt, akxModelBlueprint const blueprints[], afxModel models[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AfxAssert(blueprints);
    AfxAssert(models);
    AfxAssert(cnt);

    afxClass* cls = (afxClass*)AfxGetModelClass(sim);
    AfxAssertClass(cls, afxFcc_MDL);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)models, (void const*[]) { sim, blueprints }))
        AfxThrowError();

    return err;
}

_AMX afxNat AfxEnumerateModels(afxSimulation sim, afxNat base, afxNat cnt, afxModel models[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxClass const* cls = AfxGetModelClass(sim);
    AfxAssertClass(cls, afxFcc_MDL);
    return AfxEnumerateClassInstances(cls, base, cnt, (afxObject*)models);
}

_AMX void AfxTransformModels(afxM3d const ltm, afxM3d const iltm, afxReal ltmTol, afxV3d const atv, afxReal atvTol, afxFlags flags, afxNat cnt, afxModel models[])
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
