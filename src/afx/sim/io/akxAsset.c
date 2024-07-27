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
#define _AKX_ASSET_C
#define _AKX_MODEL_C
#define _AKX_SIMULATION_C
#include "../dev/AkxSimDevKit.h"
#include "qwadro/sim/io/akxAsset.h"

#if 0

_AKX afxError AfxDeserializeSkeletons(afxStream in, afxSimulation sim, afxNat cnt, afxSkeleton dst[])
{
    afxError err = NIL;
    AfxAssertObjects(1, &in, afxFcc_IOB);
    AfxAssert(dst);
    AfxAssert(cnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        _afxSerializedChunk chnkHdr;
        AfxRead(in, 1, sizeof(chnkHdr), &chnkHdr, sizeof(chnkHdr));
        AfxAssertType(&chnkHdr, afxFcc_SKL);

        _afxSerializedSkl chnk = { 0 };
        AfxRead(in, 1, sizeof(chnk), &chnk, sizeof(chnk));
        afxNat jointCnt = chnk.jointCnt;

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

_AKX afxError AfxSerializeSkeletons(afxStream out, afxNat cnt, afxSkeleton const src[])
{
    afxError err = NIL;
    AfxAssertObjects(1, &out, afxFcc_IOB);
    AfxAssert(src);
    AfxAssert(cnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        _afxSerializedChunk chnkHdr;
        chnkHdr.fcc = afxFcc_SKL;
        chnkHdr.len = 0;
        afxSize chnkHdrPos = AfxGetStreamPosn(out);
        AfxWrite(out, 1, sizeof(chnkHdr), &chnkHdr, sizeof(chnkHdr));
        afxSize chnkStartPos = AfxGetStreamPosn(out);

        afxSkeleton skl = src[i];
        _afxSerializedSkl chnk = { 0 };
        afxNat jointCnt = (chnk.jointCnt = skl->jointCnt);
        AfxReflectString(&skl->id, &chnk.name);
        AfxWrite(out, 1, sizeof(chnk), &chnk, sizeof(chnk));

        AfxWrite(out, jointCnt, sizeof(skl->local[0]), &skl->local[0], sizeof(skl->local[0]));

        AfxWrite(out, jointCnt, sizeof(skl->iw[0]), &skl->iw[0], sizeof(skl->iw[0]));

        AfxWrite(out, jointCnt, sizeof(skl->parentIdx[0]), &skl->parentIdx[0], sizeof(skl->parentIdx[0]));

        AfxWrite(out, jointCnt, sizeof(skl->lodError[0]), &skl->lodError[0], sizeof(skl->lodError[0]));

        AfxWrite(out, jointCnt, sizeof(skl->joint[0]), &skl->joint[0], sizeof(skl->joint[0]));


        chnkHdr.len = AfxGetStreamPosn(out) - chnkStartPos;
        AfxWriteAt(out, chnkHdrPos, 1, sizeof(chnkHdr), &chnkHdr, sizeof(chnkHdr));
        AfxAdvanceStream(out, (afxInt)chnkHdr.len);
    }

    return err;
}
#endif

_AKX afxNat AfxFindResourceNests(akxAsset cad, afxNat cnt, afxFcc const fcc[], afxNat nestIdx[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cad, afxFcc_CAD);
    AfxAssert(nestIdx);
    AfxAssert(fcc);
    AfxAssert(cnt);
    afxNat rslt = 0;
    
    afxNat nestCnt = cad->nestCnt;
    AfxAssert(nestCnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        nestIdx[i] = AFX_INVALID_INDEX;
        afxFcc fcc2 = fcc[i];

        for (afxNat j = 0; j < nestCnt; j++)
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

_AKX afxNat AfxCountResources(akxAsset cad, afxFcc fcc)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cad, afxFcc_CAD);
    AfxAssert(fcc);
    afxNat resCnt = 0;
    afxNat nestIdx;

    if (AfxFindResourceNests(cad, 1, (afxFcc[]) { fcc }, &nestIdx))
    {
        AfxAssertRange(cad->nestCnt, nestIdx, 1);
        resCnt = cad->nests[nestIdx].resCnt;
    }
    return resCnt;
}

_AKX afxNat AfxFindResources(akxAsset cad, afxFcc fcc, afxNat cnt, afxString const id[], void* res[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cad, afxFcc_CAD);
    AfxAssert(cnt);
    AfxAssert(res);
    AfxAssert(id);
    afxNat rslt = 0;
    afxNat nestIdx = 0;

    if (AfxFindResourceNests(cad, 1, (afxFcc[]) { fcc }, &nestIdx))
    {
        AfxAssertRange(cad->nestCnt, nestIdx, 1);
        afxNat resCnt = cad->nests[nestIdx].resCnt;
        afxNat baseResIdx = cad->nests[nestIdx].baseResIdx;

        for (afxNat i = 0; i < resCnt; i++)
        {
            if (0 == AfxCompareString(&id[rslt], &(cad->resSlots[baseResIdx + i].id.str)))
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

_AKX afxNat AfxFindTextures(akxAsset cad, afxNat cnt, afxString const id[], afxRaster tex[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cad, afxFcc_CAD);
    AfxAssert(cnt);
    AfxAssert(tex);
    AfxAssert(id);
    return AfxFindResources(cad, afxFcc_RAS, cnt, id, (void**)tex);
}

_AKX afxNat AfxFindMaterials(akxAsset cad, afxNat cnt, afxString const id[], afxMaterial mt[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cad, afxFcc_CAD);
    AfxAssert(cnt);
    AfxAssert(mt);
    AfxAssert(id);
    return AfxFindResources(cad, afxFcc_MTL, cnt, id, (void**)mt);
}

_AKX afxNat AfxFindVertexDatas(akxAsset cad, afxNat cnt, afxString const id[], akxVertexData vtd[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cad, afxFcc_CAD);
    AfxAssert(cnt);
    AfxAssert(vtd);
    AfxAssert(id);
    return AfxFindResources(cad, afxFcc_VTD, cnt, id, (void**)vtd);
}

_AKX afxNat AfxFindTopologies(akxAsset cad, afxNat cnt, afxString const id[], afxMeshTopology msht[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cad, afxFcc_CAD);
    AfxAssert(cnt);
    AfxAssert(msht);
    AfxAssert(id);
    return AfxFindResources(cad, afxFcc_MSHT, cnt, id, (void**)msht);
}

_AKX afxNat AfxFindMeshes(akxAsset cad, afxNat cnt, afxString const id[], afxMesh msh[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cad, afxFcc_CAD);
    AfxAssert(cnt);
    AfxAssert(msh);
    AfxAssert(id);
    return AfxFindResources(cad, afxFcc_MSH, cnt, id, (void**)msh);
}

_AKX afxNat AfxFindSkeletons(akxAsset cad, afxNat cnt, afxString const id[], afxSkeleton skl[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cad, afxFcc_CAD);
    AfxAssert(cnt);
    AfxAssert(skl);
    AfxAssert(id);
    return AfxFindResources(cad, afxFcc_SKL, cnt, id, (void**)skl);
}

_AKX afxNat AfxFindModels(akxAsset cad, afxNat cnt, afxString const id[], afxModel mdl[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cad, afxFcc_CAD);
    AfxAssert(cnt);
    AfxAssert(mdl);
    AfxAssert(id);
    return AfxFindResources(cad, afxFcc_MDL, cnt, id, (void**)mdl);
}

_AKX afxNat AfxFindAnimations(akxAsset cad, afxNat cnt, afxString const id[], akxAnimation anim[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cad, afxFcc_CAD);
    AfxAssert(cnt);
    AfxAssert(anim);
    AfxAssert(id);
    return AfxFindResources(cad, afxFcc_ANI, cnt, id, (void**)anim);
}

_AKX afxError _AfxCadDtor(akxAsset cad)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cad, afxFcc_CAD);

    if (cad->exporterInfo)
        AfxDeallocate(cad->exporterInfo);

    if (cad->toolInfo)
        AfxDeallocate(cad->toolInfo);

    for (afxNat i = 0; i < cad->nestCnt; i++)
    {
        afxNat baseResIdx = cad->nests[i].baseResIdx;

        for (afxNat j = 0; j < cad->nests[j].resCnt; j++)
        {
            void *obj = cad->resSlots[baseResIdx + j].obj;

            if (obj)
                AfxReleaseObjects(1, (void*[]) { obj });

            AfxDeallocateUri(&(cad->resSlots[baseResIdx + j].uri));
        }
    }

    return err;
}

_AKX afxError _AfxCadCtor(akxAsset cad, afxCookie const *cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cad, afxFcc_CAD);

    afxSimulation sim = cookie->udd[0];
    AfxAssertObjects(1, &sim, afxFcc_SIM);
#if 0
    afxError(*loader)(afxUri const* file, afxFlags flags) = cookie->udd[1];
    AfxAssert(loader);
    afxFlags flags = *(afxFlags*)cookie->udd[2];
    afxUri const *file = ((afxUri const *)cookie->udd[3]) + cookie->no;
#endif
    akxAssetBuilder const* cadb = cookie->udd[1];
    void* data = ((void **)(cookie->udd[2]))[cookie->no];

    cad->exporterInfo = NIL;
    cad->toolInfo = NIL;
    
    afxUri name;
    afxNat typeCnt, totalResCnt;
    cadb->GetInfo(data, &typeCnt, &totalResCnt, &name);
    AfxAssert(typeCnt);
    AfxAssert(totalResCnt);

    if (typeCnt && !(cad->nests = AfxAllocate(typeCnt, sizeof(cad->nests[0]), 0, AfxHere()))) AfxThrowError();
    else
    {
        if (typeCnt)
            AfxZero2(typeCnt, sizeof(cad->nests[0]), cad->nests);

        cad->nestCnt = typeCnt;

        if (totalResCnt && !(cad->resSlots = AfxAllocate(totalResCnt, sizeof(cad->resSlots[0]), 0, AfxHere()))) AfxThrowError();
        else
        {
            if (totalResCnt)
                AfxZero2(totalResCnt, sizeof(cad->resSlots[0]), cad->resSlots);

            cad->resCap = totalResCnt;

            afxNat baseResIdx = 0;
            AfxAssert(cadb->GetTypeInfo);

            for (afxNat i = 0; i < typeCnt; i++)
            {
                afxNat resCnt;
                afxFcc resType;
                cadb->GetTypeInfo(data, i, &resType, &resCnt);
                cad->nests[i].baseResIdx = baseResIdx;
                cad->nests[i].resCnt = resCnt;
                cad->nests[i].fcc = resType;

                for (afxNat j = 0; j < resCnt; j++)
                {
                    void* obj;
                    afxUri name2;
                    AfxResetUri(&name2);
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

_AKX afxError AfxAcquireAssets(afxSimulation sim, afxNat cnt, afxNat const nestCnt[], afxNat const resCap[], akxAsset cad[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    //if (AfxAcquireObjects(&sim->assets, cnt, (afxObject*)cad, (void const*[]) { (void*)sim, (void*)&nestCnt, (void*)&resCap }))
      //  AfxThrowError();

    AfxAssertObjects(cnt, cad, afxFcc_CAD);

    return err;
}

_AKX afxError AfxBuildAssets(afxSimulation sim, akxAssetBuilder const* cadb, afxNat cnt, void* data[], akxAsset cad[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    //if (AfxAcquireObjects(&sim->assets, cnt, (afxObject*)cad, (void const*[]) { (void*)sim, (void*)cadb, data }))
      //  AfxThrowError();

    AfxAssertObjects(cnt, cad, afxFcc_CAD);

    return err;
}

_AKX afxError AfxLoadAssets(afxSimulation sim, afxError(*loader)(afxUri const* file, afxFlags flags), afxFlags flags, afxNat cnt, afxUri const file[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    afxArray assets;
    AfxAllocateArray(&assets, cnt, sizeof(akxAsset), NIL);
    AfxReserveArraySpace(&assets, cnt);

    //if (AfxAcquireObjects(&sim->assets, cnt, (afxObject*)assets.data, (void const*[]) { (void*)sim, (void*)loader, &flags, (void*)file }))
      //  AfxThrowError();

    AfxAssertObjects(cnt, assets.data, afxFcc_CAD);

    AfxDeallocateArray(&assets);

    return err;
}

_AKX void AfxTransformAssets(afxM3d const ltm, afxM3d const iltm, afxV4d const atv, afxReal affineTol, afxReal linearTol, afxFlags flags, afxNat cnt, akxAsset cad[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(ltm);
    AfxAssert(iltm);
    AfxAssert(atv);
    AfxAssert(cnt);
    AfxAssert(cad);

    for (afxNat i = 0; i < cnt; i++)
    {
        akxAsset cad2 = cad[i];
        AfxAssertObjects(1, &cad2, afxFcc_CAD);
        afxNat resCnt;
        afxNat baseResIdx;
        afxNat nestIdx;

        if (AfxFindResourceNests(cad2, 1, (afxFcc[]) { afxFcc_SKL }, &nestIdx))
        {
            baseResIdx = cad2->nests[nestIdx].baseResIdx;
            resCnt = AfxCountResources(cad2, afxFcc_SKL);

            for (afxNat j = 0; j < resCnt; j++)
                AfxTransformSkeletons(ltm, iltm, linearTol, atv, affineTol, 1, (afxSkeleton[]) { cad2->resSlots[baseResIdx + j].obj });
        }

        if (AfxFindResourceNests(cad2, 1, (afxFcc[]) { afxFcc_MSH }, &nestIdx))
        {
            baseResIdx = cad2->nests[nestIdx].baseResIdx;
            resCnt = AfxCountResources(cad2, afxFcc_MSH);

            for (afxNat j = 0; j < resCnt; j++)
                AfxTransformMeshes(ltm, iltm, linearTol, atv, affineTol, flags, 1, (afxMesh[]) { cad2->resSlots[baseResIdx + j].obj });
        }

        if (AfxFindResourceNests(cad2, 1, (afxFcc[]) { afxFcc_MDL }, &nestIdx))
        {
            baseResIdx = cad2->nests[nestIdx].baseResIdx;
            resCnt = AfxCountResources(cad2, afxFcc_MDL);
            
            for (afxNat j = 0; j < resCnt; j++)
            {
                afxModel mdl = cad2->resSlots[baseResIdx + j].obj;
                AfxAssimilateTransforms(ltm, iltm, atv, 1, &mdl->displacement, &mdl->displacement);
            }
        }

        if (AfxFindResourceNests(cad2, 1, (afxFcc[]) { afxFcc_ANI }, &nestIdx))
        {
            baseResIdx = cad2->nests[nestIdx].baseResIdx;
            resCnt = AfxCountResources(cad2, afxFcc_ANI);

            for (afxNat j = 0; j < resCnt; j++)
                AkxTransformAnimations(ltm, iltm, linearTol, atv, affineTol, flags, 1, (akxAnimation[]) { cad2->resSlots[baseResIdx + j].obj });
        }
    }
}

_AKX afxClassConfig _AfxCadMgrCfg =
{
    .fcc = afxFcc_CAD,
    .name = "CAD Asset",
    .fixedSiz = sizeof(AFX_OBJECT(akxAsset)),
    .ctor = (void*)_AfxCadCtor,
    .dtor = (void*)_AfxCadDtor
};
