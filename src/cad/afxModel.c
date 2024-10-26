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

// IMPORTANT:
// Some models are not built from a mesh in the classic sense of the word. 
// Models structured out of Non-uniform rational basis spline (NURBS) also exit. 
// Technically you could build a model in many ways that do not involve a mesh in the classic sense of the word, such as atoms and Voxels. 
// When people speak of a mesh they normally refer to a polygon mesh.

_AMXINL afxBool AfxGetModelSkeleton(afxModel mdl, afxSkeleton* skeleton)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    afxSkeleton skl = mdl->skl;
    AfxTryAssertObjects(1, &skl, afxFcc_SKL); // have a skeleton is mandatory
    AFX_ASSERT(skeleton);
    *skeleton = skl;
    return !!skl;
}

_AMXINL afxBool AfxGetModelUrn(afxModel mdl, afxString* id)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AFX_ASSERT(id);
    *id = mdl->urn;
    return mdl->urn.len;
}

// DISPLACEMENT

_AMXINL void AfxDisplaceModel(afxModel mdl, afxTransform const* t)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);

    if (!t)
        AfxResetTransform(&mdl->displacement);
    else
        AfxCopyTransform(&mdl->displacement, t);
}

_AMXINL void AfxGetModelDisplacement(afxModel mdl, afxTransform* t)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AFX_ASSERT(t);
    AfxCopyTransform(&mdl->displacement, t);
}

_AMXINL void AfxComputeModelDisplacement(afxModel mdl, afxM4d m)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AFX_ASSERT(m);
    AfxComputeCompositeTransformM4d(&mdl->displacement, m);
}

// RIGGING

_AMXINL afxUnit AfxCountMeshRigs(afxModel mdl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    return mdl->rigCnt;
}

_AMXINL afxUnit AfxCountRiggedMeshes(afxModel mdl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    return mdl->usedRigCnt;
}

_AMXINL afxBool AfxIsMeshRigTransplanted(afxModel mdl, afxUnit rigIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AFX_ASSERT_RANGE(mdl->rigCnt, rigIdx, 1);
    
    // sanitize arguments
    rigIdx = AfxMin(rigIdx, mdl->rigCnt - 1);

    return mdl->rigs[rigIdx].flags & afxMeshRigFlag_TRANSPLANTED;
}

_AMXINL afxBool AfxGetMeshRigBaseSkeleton(afxModel mdl, afxUnit rigIdx, afxSkeleton* skeleton)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AFX_ASSERT_RANGE(mdl->rigCnt, rigIdx, 1);

    // sanitize arguments
    rigIdx = AfxMin(rigIdx, mdl->rigCnt - 1);

    afxSkeleton skl = mdl->rigs[rigIdx].sklOrig;
    AfxTryAssertObjects(1, &skl, afxFcc_SKL);
    AFX_ASSERT(skeleton);
    *skeleton = skl;
    return !!skl;
}

_AMXINL afxUnit const* AfxGetMeshRigBiasToJointMapping(afxModel mdl, afxUnit rigIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AFX_ASSERT_RANGE(mdl->rigCnt, rigIdx, 1);

    // sanitize arguments
    rigIdx = AfxMin(rigIdx, mdl->rigCnt - 1);

    return mdl->rigs[rigIdx].biasToJointMap;
}

_AMXINL afxUnit const* AfxGetMeshRigBiasFromJointMapping(afxModel mdl, afxUnit rigIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AFX_ASSERT_RANGE(mdl->rigCnt, rigIdx, 1);

    // sanitize arguments
    rigIdx = AfxMin(rigIdx, mdl->rigCnt - 1);

    return mdl->rigs[rigIdx].biasFromJointMap;
}

_AMXINL afxUnit AfxGetRiggedMeshes(afxModel mdl, afxUnit baseRigIdx, afxUnit rigCnt, afxMesh meshes[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AFX_ASSERT_RANGE(mdl->rigCnt, baseRigIdx, rigCnt);
    
    // sanitize arguments
    baseRigIdx = AfxMin(baseRigIdx, mdl->rigCnt - 1);
    rigCnt = AfxMin(rigCnt, mdl->rigCnt - baseRigIdx);

    afxUnit rslt = 0;
    for (afxUnit i = 0; i < rigCnt; i++)
    {
        afxUnit rigIdx = baseRigIdx + i;
        AFX_ASSERT_RANGE(mdl->rigCnt, rigIdx, 1);
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

_AMX afxError AfxRigMeshes(afxModel mdl, afxUnit baseRigIdx, afxUnit rigCnt, afxMesh const meshes[], afxSkeleton sklOrig)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AFX_ASSERT_RANGE(mdl->rigCnt, baseRigIdx, rigCnt);

    // sanitize arguments
    baseRigIdx = AfxMin(baseRigIdx, mdl->rigCnt - 1);
    rigCnt = AfxMin(rigCnt, mdl->rigCnt - baseRigIdx);

    afxSkeleton skl = mdl->skl;
    AfxTryAssertObjects(1, &skl, afxFcc_SKL);
    AfxTryAssertObjects(1, &sklOrig, afxFcc_SKL);
    afxBool transplanted = sklOrig && (sklOrig != skl);

    for (afxUnit rigIter = 0; rigIter < rigCnt; rigIter++)
    {
        afxUnit rigIdx = baseRigIdx + rigIter;
        AFX_ASSERT_RANGE(mdl->rigCnt, rigIdx, 1);
        afxMeshRig* rig = &mdl->rigs[rigIdx];
        afxMesh mshCurr = rig->msh;
        afxMesh msh = meshes ? meshes[rigIter] : NIL;

        if (mshCurr != msh)
        {
            afxUnit* biasFromJointMap = NIL;
            afxUnit* biasToJointMap = NIL;

            if (msh) // attach operation
            {
                AfxAssertObjects(1, &msh, afxFcc_MSH);
                afxMeshInfo mshi;
                AfxDescribeMesh(msh, &mshi);

                afxUnit biasCnt = mshi.biasCnt;

                if (biasCnt && !(biasToJointMap = AfxAllocate(biasCnt, sizeof(biasToJointMap[0]), 0, AfxHere()))) AfxThrowError();
                else
                {
                    if (!transplanted) biasFromJointMap = biasToJointMap;
                    else if (biasCnt && !(biasFromJointMap = AfxAllocate(biasCnt, sizeof(biasFromJointMap[0]), 0, AfxHere())))
                        AfxThrowError();

                    if (!err)
                    {
                        afxString* biasTags = AfxGetMeshBiasTags(msh, 0);
                        afxUnit biasMappedCnt = 0;

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

                        AfxLogEcho("Mesh %p rigged to model %p.\n    Listing %u bias-joint mapping..:", msh, mdl, biasCnt);

                        for (afxUnit j = 0; j < biasCnt; j++)
                        {
                            AfxLogEcho("    %3i --> %i <-- %i <%.*s> ", (afxInt)j, (afxInt)biasToJointMap[j], (afxInt)biasFromJointMap[j], AfxPushString(&biasTags[j]));
                        }
#if 0
                        for (afxUnit j = 0; j < biasCnt; j++)
                        {
                            afxString biasId = *AfxGetMeshBiasTags(msh, j);

                            if (1)
                            {
                                if (AFX_INVALID_INDEX == (biasToJointMap[j] = AfxFindJoint(skl, &biasId)))
                                {
                                    AfxLogError("Pivot '%.*s' not found in rig skeleton.", AfxPushString(&biasId));
                                    AFX_ASSERT(biasToJointMap[j] == AFX_INVALID_INDEX);
                                }

                                if (transplanted && (AFX_INVALID_INDEX == (biasFromJointMap[j] = AfxFindJoint(sklOrig, &biasId))))
                                {
                                    AfxLogError("Pivot '%.*s' not found in original skeleton.", AfxPushString(&biasId));
                                    AFX_ASSERT(biasFromJointMap[j] == AFX_INVALID_INDEX);
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

                    if (AfxIsMeshRigTransplanted(mdl, rigIdx))
                    {
                        afxSkeleton sklOrig = rig->sklOrig;
                        AfxTryAssertObjects(1, &sklOrig, afxFcc_SKL);
                        AfxReleaseObjects(1, &sklOrig);
                        rig->sklOrig = NIL;
                        rig->flags &= ~afxMeshRigFlag_TRANSPLANTED;
                    }

                    AfxReleaseObjects(1, &mshCurr);
                    rig->msh = NIL;
                    --mdl->usedRigCnt;
                }

                if (msh)
                {
                    rig->msh = msh;
                    rig->sklOrig = sklOrig;
                    rig->biasFromJointMap = biasFromJointMap;
                    rig->biasToJointMap = biasToJointMap;

                    if (transplanted)
                        rig->flags |= afxMeshRigFlag_TRANSPLANTED;

                    ++mdl->usedRigCnt;
                }
            }
        }
    }
    return err;
}

_AMXINL void AfxComputeMeshRigMatrices(afxModel mdl, afxUnit rigIdx, afxPoseBuffer const posb, afxUnit baseBiasIdx, afxUnit biasCnt, afxM4d matrices[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AFX_ASSERT_RANGE(mdl->rigCnt, rigIdx, 1);

    // sanitize arguments
    rigIdx = AfxMin(rigIdx, mdl->rigCnt - 1);

    afxMesh msh = mdl->rigs[rigIdx].msh;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    afxMeshInfo mshi;
    AfxDescribeMesh(msh, &mshi);

    AFX_ASSERT_RANGE(mshi.biasCnt, baseBiasIdx, biasCnt);
    // sanitize arguments
    baseBiasIdx = AfxMin(baseBiasIdx, mshi.biasCnt - 1);
    biasCnt = AfxMin(biasCnt, mshi.biasCnt - baseBiasIdx);

    afxM4d* w = AfxGetWorldMatrices(posb, 0);
    afxM4d* iw = AfxGetSkeletonMatrices(mdl->rigs[rigIdx].sklOrig, 0);
    afxUnit const* mapping = AfxGetMeshRigBiasToJointMapping(mdl, rigIdx);
    afxUnit const* mappingOrig = AfxGetMeshRigBiasFromJointMapping(mdl, rigIdx);

    for (afxUnit i = 0, artIdx = baseBiasIdx; artIdx < biasCnt; artIdx++)
    {
        AfxM4dMultiplyAffine(matrices[i], iw[mappingOrig[i]], w[mapping[i]]);
    }
}

_AMXINL afxBool AfxGetMeshRigTxd(afxModel mdl, afxUnit rigIdx, afxMaterial* material)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AFX_ASSERT_RANGE(mdl->rigCnt, rigIdx, 1);

    // sanitize arguments
    rigIdx = AfxMin(rigIdx, mdl->rigCnt - 1);

    afxMaterial txd;

    if ((txd = mdl->rigs[rigIdx].txd))
    {
        AfxAssertObjects(1, &txd, afxFcc_MTL);
    }
    AFX_ASSERT(material);
    *material = txd;
    return !!txd;
}

_AMXINL void AfxSetMeshRigTxd(afxModel mdl, afxUnit rigIdx, afxMaterial mtl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AFX_ASSERT_RANGE(mdl->rigCnt, rigIdx, 1);

    // sanitize arguments
    rigIdx = AfxMin(rigIdx, mdl->rigCnt - 1);

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

_AMXINL afxBool AfxFindMeshRigMaterial(afxModel mdl, afxUnit rigIdx, afxString const* id, afxMaterial* material)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    AFX_ASSERT_RANGE(mdl->rigCnt, rigIdx, 1);

    // sanitize arguments
    rigIdx = AfxMin(rigIdx, mdl->rigCnt - 1);

    afxMaterial txd = mdl->rigs[rigIdx].txd;
    afxMaterial mtl = NIL;

    if (txd)
    {
        AfxAssertObjects(1, &txd, afxFcc_MTL);
        AFX_ASSERT(id);

        if ((mtl = AfxFindSubmaterial(txd, id)))
        {
            AfxAssertObjects(1, &mtl, afxFcc_MTL);
        }
    }
    AFX_ASSERT(material);
    *material = mtl;
    return !!mtl;
}

_AMX afxError _AmxMdlDtorCb(afxModel mdl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);

    if (mdl->rigCnt)
        AfxRigMeshes(mdl, 0, mdl->rigCnt, NIL, NIL);

    afxObjectStash stashes[] =
    {
        {
            .cnt = mdl->rigCnt,
            .siz = sizeof(mdl->rigs[0]),
            .var = (void**)&mdl->rigs
        }
    };

    if (AfxDeallocateInstanceData(mdl, ARRAY_SIZE(stashes), stashes))
        AfxThrowError();

    afxSkeleton skl = mdl->skl;

    if (skl)
    {
        AfxAssertObjects(1, &skl, afxFcc_SKL);
        AfxReleaseObjects(1, &skl);
    }

    return err;
}

_AMX afxError _AmxMdlCtorCb(afxModel mdl, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdl, afxFcc_MDL);

    afxSimulation sim = args[0];
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxSkeleton skl = args[1];
    afxModelBlueprint const* mdlb = AFX_CAST(afxModelBlueprint const*, args[2]) + invokeNo;

    if (!AfxCatalogStrings(AfxGetModelUrnStringBase(sim), 1, &mdlb->skl.urn.str, &mdl->urn)) AfxThrowError();
    else
    {
        if (skl)
        {
            AfxAssertObjects(1, &skl, afxFcc_SKL);
            AfxReacquireObjects(1, &skl);
        }
        else if (AfxAssembleSkeletons(sim, 1, &mdlb->skl, &skl))
            AfxThrowError();

        if (!err)
        {
            AfxAssertObjects(1, &skl, afxFcc_SKL);
            mdl->skl = skl;
            
            afxTransform const* displacement = &mdlb->displacement;
            
            if (!displacement->flags)
                AfxResetTransform(&mdl->displacement);
            else
                AfxCopyTransform(&mdl->displacement, displacement);
            
            AfxBoxReset(&mdl->aabb);
            
            afxUnit rigCnt = AfxMax(1, mdlb->rigCnt);
            afxUnit baseMshIdx = 0;

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

            if (AfxAllocateInstanceData(mdl, ARRAY_SIZE(stashes), stashes)) AfxThrowError();
            else
            {
                AFX_ASSERT(mdl->rigs);

                AfxZero(mdl->rigs, rigCnt * sizeof(mdl->rigs[0]));
                mdl->rigCnt = rigCnt;

                AFX_ASSERT(mdl->rigCnt == rigCnt);

                if (err && AfxDeallocateInstanceData(mdl, ARRAY_SIZE(stashes), stashes))
                    AfxThrowError();
            }

            if (err)
                AfxReleaseObjects(1, &skl);
        }
    }

    if (!err)
    {
        afxString s;
        AfxGetModelUrn(mdl, &s);
        afxUnit jntCnt = AfxCountSkeletonJoints(mdl->skl, 0);
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

_AMX void AfxTransformModels(afxM3d const ltm, afxM3d const iltm, afxReal ltTol, afxV3d const atv, afxReal atTol, afxFlags flags, afxUnit cnt, afxModel models[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(atv);
    AFX_ASSERT(ltm);
    AFX_ASSERT(iltm);
    AFX_ASSERT(cnt);
    AFX_ASSERT(models);

    for (afxUnit mdlIdx = 0; mdlIdx < cnt; mdlIdx++)
    {
        afxModel mdl = models[mdlIdx];

        if (!mdl)
            continue;
        
        AfxAssertObjects(1, &mdl, afxFcc_MDL);

        afxSkeleton skl;
        if (AfxGetModelSkeleton(mdl, &skl))
        {
            AfxAssertObjects(1, &skl, afxFcc_SKL);
            AfxTransformSkeletons(ltm, iltm, ltTol, atv, atTol, 1, &skl);
        }

        afxMesh meshes[64];
        afxUnit batchSiz = 64;
        afxUnit totalMshCnt = AfxCountMeshRigs(mdl);
        afxUnit batches = totalMshCnt / batchSiz;
        afxUnit remaining = totalMshCnt % batchSiz;
        afxUnit baseSlotIdx = 0;
        afxUnit mshCnt = 0;

        for (afxUnit j = 0; j < batches; j++)
        {
            mshCnt = AfxGetRiggedMeshes(mdl, baseSlotIdx, batchSiz, meshes);
            baseSlotIdx += mshCnt;

            // WARNING: What to do if mesh is shared among other models of strange asset?
            AfxTransformMeshes(ltm, iltm, ltTol, atv, atTol, flags, mshCnt, meshes);
        }

        if (remaining)
        {
            mshCnt = AfxGetRiggedMeshes(mdl, baseSlotIdx, remaining, meshes);
            // WARNING: What to do if mesh is shared among other models of strange asset?
            AfxTransformMeshes(ltm, iltm, ltTol, atv, atTol, flags, mshCnt, meshes);
        }

        AfxAssimilateTransforms(ltm, iltm, atv, 1, &mdl->displacement, &mdl->displacement);
    }
}

_AMX afxUnit AfxEnumerateModels(afxSimulation sim, afxUnit first, afxUnit cnt, afxModel models[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxClass const* cls = AfxGetModelClass(sim);
    AfxAssertClass(cls, afxFcc_MDL);
    return AfxEnumerateClassInstances(cls, first, cnt, (afxObject*)models);
}

_AMX afxError AfxAssembleModels(afxSimulation sim, afxSkeleton skl, afxUnit cnt, afxModelBlueprint const blueprints[], afxModel models[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AfxTryAssertObjects(1, &skl, afxFcc_SKL);
    AFX_ASSERT(blueprints);
    AFX_ASSERT(models);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)AfxGetModelClass(sim);
    AfxAssertClass(cls, afxFcc_MDL);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)models, (void const*[]) { sim, skl, blueprints }))
        AfxThrowError();

    return err;
}
