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

#define _AFX_SIM_C
#define _AVX_ASSET_C
#define _AVX_MODEL_C
#define _AMX_SIMULATION_C
#include "../../dev/AmxImplKit.h"
#include "qwadro/inc/sim/io/akxAsset.h"

#if 0

_AMX afxError AfxDeserializeSkeletons(afxStream in, afxSimulation sim, afxUnit cnt, afxSkeleton dst[])
{
    afxError err = NIL;
    AfxAssertObjects(1, &in, afxFcc_IOB);
    AFX_ASSERT(dst);
    AFX_ASSERT(cnt);

    for (afxUnit i = 0; i < cnt; i++)
    {
        _afxSerializedChunk chnkHdr;
        AfxRead(in, 1, sizeof(chnkHdr), &chnkHdr, sizeof(chnkHdr));
        AfxAssertType(&chnkHdr, afxFcc_SKL);

        _afxSerializedSkl chnk = { 0 };
        AfxRead(in, 1, sizeof(chnk), &chnk, sizeof(chnk));
        afxUnit jointCnt = chnk.jointCnt;

        akxSkeletonBuilder sklb;
        AfxBeginSkeletonBuilding(&sklb, jointCnt, &chnk.name, chnk.lodType);

        AfxRead(in, jointCnt, sizeof(sklb.bones[0].local), &sklb.bones[0].local, sizeof(sklb.bones[0]));

        AfxRead(in, jointCnt, sizeof(sklb.bones[0].iw), &sklb.bones[0].iw, sizeof(sklb.bones[0]));

        AfxRead(in, jointCnt, sizeof(sklb.bones[0].parentIdx), &sklb.bones[0].parentIdx, sizeof(sklb.bones[0]));

        AfxRead(in, jointCnt, sizeof(sklb.bones[0].lodError), &sklb.bones[0].lodError, sizeof(sklb.bones[0]));

        AfxRead(in, jointCnt, sizeof(sklb.bones[0].id.str), &sklb.bones[0].id.str, sizeof(sklb.bones[0]));

        if (AfxBuildSkeletons(sim, NIL, 1, &sklb, &dst[i]))
            AfxThrowError();

        AfxEndSkeletonBuilding(&sklb);

        afxSkeleton skl = dst[i];
    }

    return err;
}

_AMX afxError AfxSerializeSkeletons(afxStream out, afxUnit cnt, afxSkeleton const src[])
{
    afxError err = NIL;
    AfxAssertObjects(1, &out, afxFcc_IOB);
    AFX_ASSERT(src);
    AFX_ASSERT(cnt);

    for (afxUnit i = 0; i < cnt; i++)
    {
        _afxSerializedChunk chnkHdr;
        chnkHdr.fcc = afxFcc_SKL;
        chnkHdr.len = 0;
        afxSize chnkHdrPos = AfxAskStreamPosn(out);
        AfxWrite(out, 1, sizeof(chnkHdr), &chnkHdr, sizeof(chnkHdr));
        afxSize chnkStartPos = AfxAskStreamPosn(out);

        afxSkeleton skl = src[i];
        _afxSerializedSkl chnk = { 0 };
        afxUnit jointCnt = (chnk.jointCnt = skl->jointCnt);
        AfxReflectString(&skl->id, &chnk.name);
        AfxWrite(out, 1, sizeof(chnk), &chnk, sizeof(chnk));

        AfxWrite(out, jointCnt, sizeof(skl->local[0]), &skl->local[0], sizeof(skl->local[0]));

        AfxWrite(out, jointCnt, sizeof(skl->iw[0]), &skl->iw[0], sizeof(skl->iw[0]));

        AfxWrite(out, jointCnt, sizeof(skl->parentIdx[0]), &skl->parentIdx[0], sizeof(skl->parentIdx[0]));

        AfxWrite(out, jointCnt, sizeof(skl->lodError[0]), &skl->lodError[0], sizeof(skl->lodError[0]));

        AfxWrite(out, jointCnt, sizeof(skl->joint[0]), &skl->joint[0], sizeof(skl->joint[0]));


        chnkHdr.len = AfxAskStreamPosn(out) - chnkStartPos;
        AfxWriteAt(out, chnkHdrPos, 1, sizeof(chnkHdr), &chnkHdr, sizeof(chnkHdr));
        AfxAdvanceStream(out, (afxInt)chnkHdr.len);
    }

    return err;
}
#endif

_AMX afxUnit AfxFindResourceNests(akxAsset cad, afxUnit cnt, afxFcc const fcc[], afxUnit nestIdx[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cad, afxFcc_CAD);
    AFX_ASSERT(nestIdx);
    AFX_ASSERT(fcc);
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;
    
    afxUnit nestCnt = cad->nestCnt;
    AFX_ASSERT(nestCnt);

    for (afxUnit i = 0; i < cnt; i++)
    {
        nestIdx[i] = AFX_INVALID_INDEX;
        afxFcc fcc2 = fcc[i];

        for (afxUnit j = 0; j < nestCnt; j++)
        {
            if (fcc2 == cad->nests[j].fcc)
            {
                nestIdx[i] = j;
                ++rslt;
            }

            if (rslt == cnt)
                break;
        }
    }
    return rslt;
}

_AMX afxUnit AfxCountResources(akxAsset cad, afxFcc fcc)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cad, afxFcc_CAD);
    AFX_ASSERT(fcc);
    afxUnit resCnt = 0;
    afxUnit nestIdx;

    if (AfxFindResourceNests(cad, 1, (afxFcc[]) { fcc }, &nestIdx))
    {
        AFX_ASSERT_RANGE(cad->nestCnt, nestIdx, 1);
        resCnt = cad->nests[nestIdx].resCnt;
    }
    return resCnt;
}

_AMX afxUnit AfxFindResources(akxAsset cad, afxFcc fcc, afxUnit cnt, afxString const id[], void* res[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cad, afxFcc_CAD);
    AFX_ASSERT(cnt);
    AFX_ASSERT(res);
    AFX_ASSERT(id);
    afxUnit rslt = 0;
    afxUnit nestIdx = 0;

    if (AfxFindResourceNests(cad, 1, (afxFcc[]) { fcc }, &nestIdx))
    {
        AFX_ASSERT_RANGE(cad->nestCnt, nestIdx, 1);
        afxUnit resCnt = cad->nests[nestIdx].resCnt;
        afxUnit baseResIdx = cad->nests[nestIdx].baseResIdx;

        for (afxUnit i = 0; i < resCnt; i++)
        {
            if (0 == AfxCompareStrings(&id[rslt], FALSE, 1, &(cad->resSlots[baseResIdx + i].id.str)))
            {
                res[rslt] = cad->resSlots[baseResIdx + i].obj;
                AfxAssertObjects(1, &res[rslt], fcc);
                ++rslt;
            }

            if (rslt == cnt)
                break;
        }
    }
    return rslt;
}

_AMX afxUnit AfxFindTextures(akxAsset cad, afxUnit cnt, afxString const id[], afxRaster tex[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cad, afxFcc_CAD);
    AFX_ASSERT(cnt);
    AFX_ASSERT(tex);
    AFX_ASSERT(id);
    return AfxFindResources(cad, afxFcc_RAS, cnt, id, (void**)tex);
}

_AMX afxUnit AfxFindMaterials(akxAsset cad, afxUnit cnt, afxString const id[], afxMaterial mt[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cad, afxFcc_CAD);
    AFX_ASSERT(cnt);
    AFX_ASSERT(mt);
    AFX_ASSERT(id);
    return AfxFindResources(cad, afxFcc_MTL, cnt, id, (void**)mt);
}

_AMX afxUnit AfxFindVertexDatas(akxAsset cad, afxUnit cnt, afxString const id[], afxGeometry vtd[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cad, afxFcc_CAD);
    AFX_ASSERT(cnt);
    AFX_ASSERT(vtd);
    AFX_ASSERT(id);
    return AfxFindResources(cad, afxFcc_GEO, cnt, id, (void**)vtd);
}

_AMX afxUnit AfxFindTopologies(akxAsset cad, afxUnit cnt, afxString const id[], afxMeshTopology msht[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cad, afxFcc_CAD);
    AFX_ASSERT(cnt);
    AFX_ASSERT(msht);
    AFX_ASSERT(id);
    return AfxFindResources(cad, afxFcc_MSHT, cnt, id, (void**)msht);
}

_AMX afxUnit AfxFindMeshes(akxAsset cad, afxUnit cnt, afxString const id[], afxMesh msh[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cad, afxFcc_CAD);
    AFX_ASSERT(cnt);
    AFX_ASSERT(msh);
    AFX_ASSERT(id);
    return AfxFindResources(cad, afxFcc_MSH, cnt, id, (void**)msh);
}

_AMX afxUnit AfxFindSkeletons(akxAsset cad, afxUnit cnt, afxString const id[], afxSkeleton skl[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cad, afxFcc_CAD);
    AFX_ASSERT(cnt);
    AFX_ASSERT(skl);
    AFX_ASSERT(id);
    return AfxFindResources(cad, afxFcc_SKL, cnt, id, (void**)skl);
}

_AMX afxUnit AfxFindModels(akxAsset cad, afxUnit cnt, afxString const id[], afxModel mdl[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cad, afxFcc_CAD);
    AFX_ASSERT(cnt);
    AFX_ASSERT(mdl);
    AFX_ASSERT(id);
    return AfxFindResources(cad, afxFcc_MDL, cnt, id, (void**)mdl);
}

_AMX afxUnit AfxFindAnimations(akxAsset cad, afxUnit cnt, afxString const id[], afxAnimation anim[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cad, afxFcc_CAD);
    AFX_ASSERT(cnt);
    AFX_ASSERT(anim);
    AFX_ASSERT(id);
    return AfxFindResources(cad, afxFcc_ANI, cnt, id, (void**)anim);
}

_AMX afxError _AfxCadDtor(akxAsset cad)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cad, afxFcc_CAD);

    if (cad->exporterInfo)
        AfxDeallocate(cad->exporterInfo);

    if (cad->toolInfo)
        AfxDeallocate(cad->toolInfo);

    for (afxUnit i = 0; i < cad->nestCnt; i++)
    {
        afxUnit baseResIdx = cad->nests[i].baseResIdx;

        for (afxUnit j = 0; j < cad->nests[j].resCnt; j++)
        {
            void *obj = cad->resSlots[baseResIdx + j].obj;

            if (obj)
                AfxReleaseObjects(1, (void*[]) { obj });

            AfxDeallocateUri(&(cad->resSlots[baseResIdx + j].uri));
        }
    }

    return err;
}

_AMX afxError _AfxCadCtor(akxAsset cad, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cad, afxFcc_CAD);

    afxSimulation sim = args[0];
    AfxAssertObjects(1, &sim, afxFcc_SIM);
#if 0
    afxError(*loader)(afxUri const* file, afxFlags flags) = args[1];
    AFX_ASSERT(loader);
    afxFlags flags = *(afxFlags*)cookie->udd[2];
    afxUri const *file = ((afxUri const *)cookie->udd[3]) + cookie->no;
#endif
    akxAssetBuilder const* cadb = args[1];
    void* data = ((void **)(args[2]))[invokeNo];

    cad->exporterInfo = NIL;
    cad->toolInfo = NIL;
    
    afxUri name;
    afxUnit typeCnt, totalResCnt;
    cadb->GetInfo(data, &typeCnt, &totalResCnt, &name);
    AFX_ASSERT(typeCnt);
    AFX_ASSERT(totalResCnt);

    if (typeCnt && !(cad->nests = AfxAllocate(typeCnt, sizeof(cad->nests[0]), 0, AfxHere()))) AfxThrowError();
    else
    {
        if (typeCnt)
            AfxZero2(cad->nests, sizeof(cad->nests[0]), typeCnt);

        cad->nestCnt = typeCnt;

        if (totalResCnt && !(cad->resSlots = AfxAllocate(totalResCnt, sizeof(cad->resSlots[0]), 0, AfxHere()))) AfxThrowError();
        else
        {
            if (totalResCnt)
                AfxZero2(cad->resSlots, sizeof(cad->resSlots[0]), totalResCnt);

            cad->resCap = totalResCnt;

            afxUnit baseResIdx = 0;
            AFX_ASSERT(cadb->GetTypeInfo);

            for (afxUnit i = 0; i < typeCnt; i++)
            {
                afxUnit resCnt;
                afxFcc resType;
                cadb->GetTypeInfo(data, i, &resType, &resCnt);
                cad->nests[i].baseResIdx = baseResIdx;
                cad->nests[i].resCnt = resCnt;
                cad->nests[i].fcc = resType;

                for (afxUnit j = 0; j < resCnt; j++)
                {
                    void* obj;
                    afxUri name2;
                    AfxResetUris(1, &name2);
                    obj = cadb->GetResourceInfo(data, resType, j, &name2);

                    if ((cad->resSlots[baseResIdx + j].obj = obj))
                        AfxReacquireObjects(1, (void*[]) { obj });

                    AfxMakeString32(&cad->resSlots[baseResIdx + j].id, 0);

                    AfxDuplicateUri(&cad->resSlots[baseResIdx + j].uri, &name2);
                    AfxCopyString(&cad->resSlots[baseResIdx + j].id.str, AfxGetUriString(&cad->resSlots[baseResIdx + j].uri));
                }
                baseResIdx += resCnt;
            }
        }
        cad->nestCnt = typeCnt;
    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

_AMX afxError AfxAcquireAssets(afxSimulation sim, afxUnit cnt, afxUnit const nestCnt[], afxUnit const resCap[], akxAsset cad[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    //if (AfxAcquireObjects(&sim->assets, cnt, (afxObject*)cad, (void const*[]) { (void*)sim, (void*)&nestCnt, (void*)&resCap }))
      //  AfxThrowError();

    AfxAssertObjects(cnt, cad, afxFcc_CAD);

    return err;
}

_AMX afxError AfxBuildAssets(afxSimulation sim, akxAssetBuilder const* cadb, afxUnit cnt, void* data[], akxAsset cad[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    //if (AfxAcquireObjects(&sim->assets, cnt, (afxObject*)cad, (void const*[]) { (void*)sim, (void*)cadb, data }))
      //  AfxThrowError();

    AfxAssertObjects(cnt, cad, afxFcc_CAD);

    return err;
}

_AMX afxError AfxLoadAssets(afxSimulation sim, afxError(*loader)(afxUri const* file, afxFlags flags), afxFlags flags, afxUnit cnt, afxUri const file[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    afxArray assets;
    AfxMakeArray(&assets, cnt, sizeof(akxAsset), NIL);
    AfxReserveArraySpace(&assets, cnt);

    //if (AfxAcquireObjects(&sim->assets, cnt, (afxObject*)assets.data, (void const*[]) { (void*)sim, (void*)loader, &flags, (void*)file }))
      //  AfxThrowError();

    AfxAssertObjects(cnt, assets.data, afxFcc_CAD);

    AfxCleanUpArray(&assets);

    return err;
}

_AMX void AfxTransformAssets(afxM3d const ltm, afxM3d const iltm, afxV4d const atv, afxReal affineTol, afxReal linearTol, afxFlags flags, afxUnit cnt, akxAsset cad[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(ltm);
    AFX_ASSERT(iltm);
    AFX_ASSERT(atv);
    AFX_ASSERT(cnt);
    AFX_ASSERT(cad);

    for (afxUnit i = 0; i < cnt; i++)
    {
        akxAsset cad2 = cad[i];
        AfxAssertObjects(1, &cad2, afxFcc_CAD);
        afxUnit resCnt;
        afxUnit baseResIdx;
        afxUnit nestIdx;

        if (AfxFindResourceNests(cad2, 1, (afxFcc[]) { afxFcc_SKL }, &nestIdx))
        {
            baseResIdx = cad2->nests[nestIdx].baseResIdx;
            resCnt = AfxCountResources(cad2, afxFcc_SKL);

            for (afxUnit j = 0; j < resCnt; j++)
                AfxTransformSkeletons(ltm, iltm, linearTol, atv, affineTol, 1, (afxSkeleton[]) { cad2->resSlots[baseResIdx + j].obj });
        }

        if (AfxFindResourceNests(cad2, 1, (afxFcc[]) { afxFcc_MSH }, &nestIdx))
        {
            baseResIdx = cad2->nests[nestIdx].baseResIdx;
            resCnt = AfxCountResources(cad2, afxFcc_MSH);

            for (afxUnit j = 0; j < resCnt; j++)
                AfxTransformMeshes(ltm, iltm, linearTol, atv, affineTol, flags, 1, (afxMesh[]) { cad2->resSlots[baseResIdx + j].obj });
        }

        if (AfxFindResourceNests(cad2, 1, (afxFcc[]) { afxFcc_MDL }, &nestIdx))
        {
            baseResIdx = cad2->nests[nestIdx].baseResIdx;
            resCnt = AfxCountResources(cad2, afxFcc_MDL);
            
            for (afxUnit j = 0; j < resCnt; j++)
            {
                afxModel mdl = cad2->resSlots[baseResIdx + j].obj;
                //AfxAssimilateTransforms(ltm, iltm, atv, 1, &mdl->displacement, &mdl->displacement);
            }
        }

        if (AfxFindResourceNests(cad2, 1, (afxFcc[]) { afxFcc_ANI }, &nestIdx))
        {
            baseResIdx = cad2->nests[nestIdx].baseResIdx;
            resCnt = AfxCountResources(cad2, afxFcc_ANI);

            for (afxUnit j = 0; j < resCnt; j++)
                AkxTransformAnimations(ltm, iltm, linearTol, atv, affineTol, flags, 1, (afxAnimation[]) { cad2->resSlots[baseResIdx + j].obj });
        }
    }
}

_AMX afxClassConfig _AfxCadMgrCfg =
{
    .fcc = afxFcc_CAD,
    .name = "CAD Asset",
    .fixedSiz = sizeof(AFX_OBJECT(akxAsset)),
    .ctor = (void*)_AfxCadCtor,
    .dtor = (void*)_AfxCadDtor
};


#if 0
_AMX afxError AfxDeserializeMeshTopologies(afxStream in, afxSimulation sim, afxUnit cnt, afxMeshTopology dst[])
{
    afxError err = NIL;
    AfxAssertObjects(1, &in, afxFcc_IOB);
    AFX_ASSERT(dst);
    AFX_ASSERT(cnt);

    for (afxUnit i = 0; i < cnt; i++)
    {
        _afxSerializedChunk chnkHdr;
        AfxRead(in, 1, sizeof(chnkHdr), &chnkHdr, sizeof(chnkHdr));
        AfxAssertType(&chnkHdr, afxFcc_MSHT);

        _afxSerializedMsht chnk = { 0 };
        AfxRead(in, 1, sizeof(chnk), &chnk, sizeof(chnk));

        afxMeshTopologyBlueprint spec = { 0 };
        spec.surfCnt = chnk.surCnt;
        spec.triCnt = chnk.triCnt;

        AfxAssembleMeshTopology(sim, 1, &spec, &dst[i]);

        AfxRead(in, spec.surfCnt, sizeof(dst[i]->surfaces[0]), dst[i]->surfaces, sizeof(dst[i]->surfaces[0]));

        AfxRead(in, spec.triCnt, sizeof(dst[i]->tris[0]), dst[i]->tris, sizeof(dst[i]->tris[0]));
    }

    return err;
}

_AMX afxError AfxSerializeMeshTopologies(afxStream out, afxUnit cnt, afxMeshTopology const src[])
{
    afxError err = NIL;
    AfxAssertObjects(1, &out, afxFcc_IOB);
    AFX_ASSERT(src);
    AFX_ASSERT(cnt);

    for (afxUnit i = 0; i < cnt; i++)
    {
        _afxSerializedChunk chnkHdr;
        chnkHdr.fcc = afxFcc_MSHT;
        chnkHdr.len = 0;
        afxSize chnkHdrPos = AfxAskStreamPosn(out);
        AfxWrite(out, 1, sizeof(chnkHdr), &chnkHdr, sizeof(chnkHdr));
        afxSize chnkStartPos = AfxAskStreamPosn(out);

        afxMeshTopology msht = src[i];
        _afxSerializedMsht chnk = { 0 };
        chnk.surCnt = msht->surfCnt;
        chnk.triCnt = msht->triCnt;
        AfxWrite(out, 1, sizeof(chnk), &chnk, sizeof(chnk));

        AfxWrite(out, chnk.surCnt, sizeof(msht->surfaces[0]), msht->surfaces, sizeof(msht->surfaces[0]));

        AfxWrite(out, chnk.triCnt, sizeof(msht->tris[0]), msht->tris, sizeof(msht->tris[0]));

        chnkHdr.len = AfxAskStreamPosn(out) - chnkStartPos;
        AfxWriteAt(out, chnkHdrPos, 1, sizeof(chnkHdr), &chnkHdr, sizeof(chnkHdr));
        AfxAdvanceStream(out, (afxInt)chnkHdr.len);
    }

    return err;
}
#endif