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
#define _AFX_VERTEX_DATA_C
#define _AFX_SIMULATION_C

#define _AVX_DRAW_C
#define _AVX_VERTEX_BUFFER_C

#include "qwadro/cad/akxVertexData.h"
#include "qwadro/sim/afxSimulation.h"
#include "qwadro/math/afxMatrix.h"
#include "qwadro/draw/pipe/afxDrawOps.h"
#include "qwadro/mem/afxMappedString.h"

_AKX afxError AkxCmdBindVertexDataCache(avxCmdb cmdb, afxNat slotIdx, akxVertexData vtd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);

    for (afxNat i = 0; i < 2; i++)
    {
        akxVertexDataCache const* cache = &vtd->cache[i];
        afxVertexBuffer vbuf = AfxGetLinker(&cache->vbuf);

        if (vbuf)
        {
            AvxCmdBindVertexSources(cmdb, i, 1, (afxBuffer[]) { AfxGetVertexBufferStorage(vbuf) }, &cache->base, &cache->range, &cache->stride);
        }
    }
    return err;
}

_AKXINL void packVdCb(akxVertexDataCache* cache, akxVertexData vtd)
{

}

_AKX afxError AkxBufferizeVertexData(afxDrawInput din, akxVertexData vtd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);

    if (vtd->cached)
        return err;

    vtd->cached = TRUE;

    afxSimulation sim = AfxGetObjectProvider(vtd);
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    afxString const positionals[] = { AfxString("pos"), AfxString("pvt"), AfxString("wgt") };
    afxNat cacheSiz[2] = { 0 };
    
    for (afxNat i = 0; i < vtd->attrCnt; i++)
    {
        akxVertexDataAttr* attr = &vtd->attrs[i];
        afxVertexFormat fmt = attr->fmt;
        AfxAssert(fmt < afxVertexFormat_TOTAL);
        afxVertexFormat cachedFmt = NIL;
        afxNat cacheIdx;

        afxString str;
        AfxResolveStrings2(vtd->strb, 1, &attr->id, &str);

        if (AFX_INVALID_INDEX != AfxStringsAreEqual(&str, 3, positionals))
        {
            if (0 == AfxCompareString(&str, &positionals[0]))
            {
                cachedFmt = afxVertexFormat_V3D;
            }
            else if (0 == AfxCompareString(&str, &positionals[1]))
            {
                cachedFmt = afxVertexFormat_V4B;
            }
            else if (0 == AfxCompareString(&str, &positionals[2]))
            {
                cachedFmt = afxVertexFormat_V4B;
            }
            else
            {
                // pointless
                AfxThrowError();
            }
            cacheIdx = 0;
        }
        else
        {
            cacheIdx = 1;
            cachedFmt = attr->fmt;
        }

        attr->cacheIdx = cacheIdx;
        attr->cachedOffset = cacheSiz[cacheIdx];        
        cacheSiz[cacheIdx] += AfxVertexFormatGetSize(cachedFmt);
        attr->cachedFmt = cachedFmt;
    }
    
    afxVertexBufferSpecification vboSpec = { 0 };
    vboSpec.access = afxBufferAccess_W;
    vboSpec.bufCap = AFX_ALIGN(cacheSiz[0] * vtd->vtxCnt, 64) + AFX_ALIGN(cacheSiz[1] * vtd->vtxCnt, 64);
    afxVertexBuffer vbo;
    AfxAcquireVertexBuffers(din, 1, &vboSpec, &vbo);

    for (afxNat i = 0; i < 2; i++)
    {
        if (cacheSiz[i])
        {
            akxVertexDataCache* cache = &vtd->cache[i];

            AfxPushLinkage(&cache->vbuf, &vbo->rooms);
            cache->base = i ? vtd->cache[i - 1].range : 0;
            cache->range = AFX_ALIGN(cacheSiz[i] * vtd->vtxCnt, 64);
            cache->stride = cacheSiz[i];
        }
    }

    for (afxNat j = 0; j < vtd->attrCnt; j++)
    {
        akxVertexDataAttr* attr = &vtd->attrs[j];
        AfxAssert(attr->data);
        afxVertexFormat fmt = attr->fmt;
        AfxAssert(fmt < afxVertexFormat_TOTAL);

        akxVertexDataCache* cache = &vtd->cache[attr->cacheIdx];
        afxBuffer buf = AfxGetVertexBufferStorage(vbo);
        AfxAssert(cache->stride);
        AfxAssertRange(AfxGetBufferCapacity(buf), cache->base, cache->range);
        //AfxUpdateBuffer2(buf, cache->base, cache->range, cache->stride, vtd->attrs[attrIdx].data, srcStride);

        afxBufferRegion rgn;
        rgn.base = cache->base;
        rgn.range = cache->range;
        rgn.stride = cache->stride;

        rgn.offset = attr->cachedOffset;
        rgn.unitSiz = AfxVertexFormatGetSize(attr->cachedFmt);
        afxSize srcStride = AfxVertexFormatGetSize(fmt);
        AfxAssert(srcStride);

        if (attr->data)
            AfxUpdateBufferRegion(buf, &rgn, attr->data, srcStride);
    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AKX afxNat AkxFindVertexAttributes(akxVertexData vtd, afxNat cnt, afxString const id[], afxNat attrIdx[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssert(attrIdx);
    afxNat rslt = 0;

    for (afxNat i = 0; i < cnt; i++)
        attrIdx[i] = AFX_INVALID_INDEX;

    for (afxNat i = 0; i < vtd->attrCnt; i++)
    {
        afxString str;
        AfxResolveStrings2(vtd->strb, 1, &vtd->attrs[i].id, &str);

        if (0 == AfxCompareString(&id[rslt], &str))
            attrIdx[rslt++] = i;

        if (cnt == rslt)
            break;
    }
    return rslt;
}

_AKX afxVertexFormat AkxGetVertexAttributeFormat(akxVertexData vtd, afxNat attrIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssertRange(vtd->attrCnt, attrIdx, 1);
    return vtd->attrs[attrIdx].fmt;
}

_AKX akxVertexUsage AkxGetVertexAttributeUsage(akxVertexData vtd, afxNat attrIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssertRange(vtd->attrCnt, attrIdx, 1);
    return vtd->attrs[attrIdx].usage;
}

_AKX akxVertexFlags AkxGetVertexAttributeFlags(akxVertexData vtd, afxNat attrIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssertRange(vtd->attrCnt, attrIdx, 1);
    return vtd->attrs[attrIdx].flags;
}

_AKX afxError AkxGetVertexAttributeInfo(akxVertexData vtd, afxNat attrIdx, afxVertexFormat* fmt, akxVertexUsage* usage, akxVertexFlags* flags)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssertRange(vtd->attrCnt, attrIdx, 1);

    if (fmt)
        *fmt = vtd->attrs[attrIdx].fmt;

    if (usage)
        *usage = vtd->attrs[attrIdx].usage;

    if (flags)
        *flags = vtd->attrs[attrIdx].flags;

    return err;
}

_AKX afxNat AkxCountVertices(akxVertexData vtd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    return vtd->vtxCnt;
}

_AKX afxError AkxUpdateVertexData(akxVertexData vtd, afxNat attrIdx, afxNat baseVtx, afxNat vtxCnt, void const* src, afxNat32 srcStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssertRange(vtd->attrCnt, attrIdx, 1);
    AfxAssertRange(vtd->vtxCnt, baseVtx, vtxCnt);
    AfxAssert(src);
    //AfxAssert(srcStride);
#if 0
    afxByte const* src2 = src;
    afxVertexFormat fmt = vtd->attrs[attrIdx].fmt;
    afxNat32 unitSiz = AfxVertexFormatGetSize(fmt);
    afxNat srcIdx = vtd->attrs[attrIdx].srcIdx;
    afxSize combOff = vtd->srcs[srcIdx].offset + (baseVtx * unitSiz);
    afxBuffer buf = vtd->srcs[srcIdx].buf;
    AfxAssertObjects(1, &buf, afxFcc_BUF);

    if (AfxUpdateBuffer2(buf, combOff, vtxCnt * unitSiz, unitSiz, src, srcStride))
        AfxThrowError();
#endif

    afxVertexFormat fmt = vtd->attrs[attrIdx].fmt;
    afxNat32 unitSiz = AfxVertexFormatGetSize(fmt);

    if (!vtd->attrs[attrIdx].data && !(vtd->attrs[attrIdx].data = AfxAllocate(vtd->vtxCnt, unitSiz, AFX_SIMD_ALIGN, AfxHere())))
        AfxThrowError();

    if (!err)
    {
        AfxStream2(vtxCnt, src, srcStride, &(vtd->attrs[attrIdx].dataBytemap[unitSiz * baseVtx]), unitSiz);
        //AfxStream(vtxCnt, srcStride, unitSiz, src, vtd->attrs[attrIdx].data);
    }
    return err;
}

_AKX void* AkxExposeVertexData(akxVertexData vtd, afxNat attrIdx, afxNat baseVtx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssertRange(vtd->attrCnt, attrIdx, 1);
    AfxAssertRange(vtd->vtxCnt, baseVtx, 1);

    afxVertexFormat fmt = vtd->attrs[attrIdx].fmt;
    afxNat32 unitSiz = AfxVertexFormatGetSize(fmt);
    afxByte *data = vtd->attrs[attrIdx].data;

    if (!data)
        AfxThrowError();
    else
        data += baseVtx * unitSiz;

    return data;
}

_AKX afxError AkxFillVertexData(akxVertexData vtd, afxNat attrIdx, afxNat baseVtx, afxNat vtxCnt, void const* src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssertRange(vtd->attrCnt, attrIdx, 1);
    AfxAssertRange(vtd->vtxCnt, baseVtx, vtxCnt);
    AfxAssert(src);

    afxVertexFormat fmt = vtd->attrs[attrIdx].fmt;
    afxNat32 unitSiz = AfxVertexFormatGetSize(fmt);

    if (!vtd->attrs[attrIdx].data)
        if (!(vtd->attrs[attrIdx].data = AfxAllocate(vtd->vtxCnt, unitSiz, AFX_SIMD_ALIGN, AfxHere())))
            AfxThrowError();

    if (!err)
    {
        AfxStream(vtxCnt, 0, unitSiz, src, vtd->attrs[attrIdx].data);
    }
    return err;
}

_AKX afxError AkxNormalizeVertexData(akxVertexData vtd, afxNat attrIdx, afxNat baseVtx, afxNat vtxCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssertRange(vtd->attrCnt, attrIdx, 1);
    AfxAssertRange(vtd->vtxCnt, baseVtx, vtxCnt);
    
    afxVertexFormat fmt = vtd->attrs[attrIdx].fmt;
    AfxAssert(fmt == afxVertexFormat_V2D || fmt == afxVertexFormat_V3D || fmt == afxVertexFormat_V4D);

    afxNat32 unitSiz = AfxVertexFormatGetSize(fmt);

    if (!vtd->attrs[attrIdx].data)
        if (!(vtd->attrs[attrIdx].data = AfxAllocate(vtd->vtxCnt, unitSiz, AFX_SIMD_ALIGN, AfxHere())))
            AfxThrowError();

    if (!err)
    {
        switch (fmt)
        {
        case afxVertexFormat_V2D:
        {
            AfxNormalizeArrayedV2d(vtxCnt, vtd->attrs[attrIdx].data, vtd->attrs[attrIdx].data);
            break;
        }
        case afxVertexFormat_V3D:
        {
            AfxNormalizeArrayedV3d(vtxCnt, vtd->attrs[attrIdx].data, vtd->attrs[attrIdx].data);
            break;
        }
        case afxVertexFormat_V4D:
        {
            AfxNormalizeArrayedV4d(vtxCnt, vtd->attrs[attrIdx].data, vtd->attrs[attrIdx].data);
            break;
        }
        default: AfxThrowError(); break;
        }
    }
    return err;
}

_AKX afxError AkxZeroVertexData(akxVertexData vtd, afxNat attrIdx, afxNat baseVtx, afxNat vtxCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssertRange(vtd->attrCnt, attrIdx, 1);
    AfxAssertRange(vtd->vtxCnt, baseVtx, vtxCnt);
    
    if (AkxFillVertexData(vtd, attrIdx, baseVtx, vtxCnt, AFX_M4D_ZERO))
        AfxThrowError();

    return err;
}

_AKX afxError _AkxVtdDtor(akxVertexData vtd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    
    afxSimulation sim = AfxGetObjectProvider(vtd);
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxMmu mmu = AfxGetSimulationMmu(sim);

    for (afxNat i = 0; i < vtd->attrCnt; i++)
    {
        if (vtd->attrs[i].data)
            AfxDeallocate(vtd->attrs[i].data);
    }

    if (vtd->attrs)
        AfxDeallocate(vtd->attrs);

    if (vtd->strb)
        AfxReleaseObjects(1, (void*[]) { vtd->strb });

    return err;
}

_AKX afxError _AkxVtdCtor(akxVertexData vtd, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    
    afxSimulation sim = cookie->udd[0];
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxStringBase strb = cookie->udd[1];
    akxVertexAttrSpec const* attrSpecs = cookie->udd[2];
    akxVertexDataSpec const* spec = cookie->udd[3];
    spec += cookie->no;

    if ((vtd->strb = strb))
        AfxReacquireObjects(1, &strb);

    afxMmu mmu = AfxGetSimulationMmu(sim);

    afxNat totalBiasCnt = spec->biasCnt;
    afxNat vtxCnt = spec->vtxCnt;

    AfxAssert(vtxCnt); // at least one
    vtd->vtxCnt = vtxCnt;
            
    afxNat attrCnt = spec->attrCnt;
    akxVertexDataAttr* attrs;

    if (!(attrs = AfxAllocate(attrCnt, sizeof(attrs[0]), NIL, AfxHere()))) AfxThrowError();
    else
    {
        vtd->attrCnt = attrCnt;
        vtd->attrs = attrs;

        for (afxNat i = 0; i < attrCnt; i++)
        {
            akxVertexAttrSpec const* attrSpec = &attrSpecs[i];

            afxBool hasData = TRUE;

            afxString tmp;
            AfxMakeString(&tmp, attrSpec->id, 0);

            if (!strb)
                AfxResetString(&attrs[i].id);
            else if (!AfxCatalogStrings2(strb, 1, &tmp, &attrs[i].id))
                AfxThrowError();

            attrs[i].usage = attrSpec->usage;
            attrs[i].flags = NIL;

            if (attrs[i].usage & akxVertexUsage_POS)
                attrs[i].flags |= akxVertexFlag_AFFINE | akxVertexFlag_LINEAR;

            attrs[i].flags |= attrSpec->flags;
            AfxAssert(attrSpec->fmt < afxVertexFormat_TOTAL);
            attrs[i].fmt = attrSpec->fmt;
            attrs[i].data = NIL;

            attrs[i].cachedFmt = NIL;
            attrs[i].cachedOffset = 0;
            attrs[i].cacheIdx = AFX_INVALID_INDEX;

            afxSize vtxSiz = AfxVertexFormatGetSize(attrs[i].fmt);

            if (attrSpec->src)
                if (AkxUpdateVertexData(vtd, i, 0, vtxCnt, attrSpec->src, attrSpec->srcStride))
                    AfxThrowError();
        }
    }
    return err;
}

_AKX afxClassConfig _AkxVtdMgrCfg =
{
    .fcc = afxFcc_VTD,
    .name = "Vertex Data",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(akxVertexData)),
    .mmu = NIL,
    .ctor = (void*)_AkxVtdCtor,
    .dtor = (void*)_AkxVtdDtor
};

////////////////////////////////////////////////////////////////////////////////

_AKX afxError AkxAcquireVertexDatas(afxSimulation sim, afxStringBase strb, akxVertexAttrSpec const attrSpec[], afxNat cnt, akxVertexDataSpec const specs[], akxVertexData datas[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AfxAssert(datas);
    AfxAssert(specs);
    AfxAssert(cnt);

    if (AfxAcquireObjects(AkxGetMeshDataClass(sim), cnt, (afxObject*)datas, (void const*[]) { sim, strb, attrSpec, specs }))
        AfxThrowError();

    return err;
}

_AKX akxVertexData AkxBuildVertexData(afxSimulation sim, afxStringBase strb, afxMeshBuilder const* mshb)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AfxAssert(mshb);
    akxVertexData vtd = NIL;

    akxVertexDataSpec spec;
    spec.vtxCnt = mshb->vtxCnt;
    spec.vtxSrc = mshb->vtx;
    spec.vtxSrcStride = sizeof(mshb->vtx[0]);
    spec.biasCnt = mshb->biases.cnt;
    spec.biasSrcStride = mshb->biases.unitSiz;
    spec.biasSrc = mshb->biases.data;
    spec.baseAttrIdx = 0;
    spec.attrCnt = 3;

    for (afxNat i = 0; i < spec.biasCnt; i++)
    {
        afxVertexBias const* vb = AfxGetArrayUnit(&mshb->biases, i);
        AfxAssert(1.0 >= vb->weight);
        AfxAssertRange(mshb->artCnt, vb->pivotIdx, 1);
    }

    akxVertexAttrSpec attrSpecs[16];

    attrSpecs[0].id = "pos";
    attrSpecs[0].flags = akxVertexFlag_AFFINE | akxVertexFlag_LINEAR;
    attrSpecs[0].fmt = afxVertexFormat_V4D;
    attrSpecs[0].usage = akxVertexUsage_POS | akxVertexUsage_POSITIONAL;
    attrSpecs[0].src = mshb->posn;
    attrSpecs[0].srcStride = sizeof(mshb->posn[0]);

    attrSpecs[1].id = "nrm";
    attrSpecs[1].flags = akxVertexFlag_LINEAR;
    attrSpecs[1].fmt = afxVertexFormat_V3D;
    attrSpecs[1].usage = akxVertexUsage_NRM | akxVertexUsage_TANGENT;
    attrSpecs[1].src = mshb->nrm;
    attrSpecs[1].srcStride = sizeof(mshb->nrm[0]);

    attrSpecs[2].id = "uv";
    attrSpecs[2].flags = NIL;
    attrSpecs[2].fmt = afxVertexFormat_V2D;
    attrSpecs[2].usage = akxVertexUsage_UV;
    attrSpecs[2].src = mshb->uv;
    attrSpecs[2].srcStride = sizeof(mshb->uv[0]);

    if (AkxAcquireVertexDatas(sim, strb, attrSpecs, 1, &spec, &vtd))
        AfxThrowError();

    return vtd;
}

_AKX void AkxTransformVertexDatas(afxM3d const ltm, afxM3d const iltm, afxV4d const atv, afxBool renormalize, afxNat cnt, akxVertexData vtd[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(atv);
    AfxAssert(ltm);
    AfxAssert(iltm);
    AfxAssert(cnt);
    AfxAssert(vtd);

    for (afxNat i = 0; i < cnt; i++)
    {
        akxVertexData vtd2 = vtd[i];

        if (vtd2)
        {
            AfxAssertObjects(1, &vtd2, afxFcc_VTD);

            afxNat baseVtx = 0;
            afxNat vtxCnt = vtd2->vtxCnt;

            for (afxNat j = 0; j < vtd2->attrCnt; j++)
            {
                void* data = AkxExposeVertexData(vtd2, j, baseVtx);
                akxVertexFlags flags = AkxGetVertexAttributeFlags(vtd2, j);
                afxBool linearFlag = flags & akxVertexFlag_LINEAR;
                afxBool invLinearFlag = flags & akxVertexFlag_LINEAR_INV;
                afxBool affineFlag = flags & akxVertexFlag_AFFINE;
                afxBool deltaFlag = flags & akxVertexFlag_DELTA;
                afxBool normalizedFlag = flags & akxVertexFlag_NORMALIZED;
                akxVertexUsage usage = AkxGetVertexAttributeUsage(vtd2, j);
                afxVertexFormat fmt = AkxGetVertexAttributeFormat(vtd2, j);
                afxBool normalized;

                if (usage & akxVertexUsage_POS)
                {
                    if (fmt == afxVertexFormat_V4D)
                    {
                        AfxPostMultiplyArrayedLtv4d(ltm, vtxCnt, data, data);

                        if (!deltaFlag)
                            for (afxNat k = 0; k < vtxCnt; k++)
                                AfxAddV4d(((afxV4d*)data)[k], ((afxV4d*)data)[k], atv);
                    }
                    else if (fmt == afxVertexFormat_V3D)
                    {
                        AfxPostMultiplyArrayedV3d(ltm, vtxCnt, data, data);

                        if (!deltaFlag)
                            for (afxNat k = 0; k < vtxCnt; k++)
                                AfxAddV3d(((afxV3d*)data)[k], ((afxV3d*)data)[k], atv);
                    }

                    normalized = FALSE;
                }
#if !0
                else if (usage & akxVertexUsage_TAN)
                {
                    AfxPostMultiplyArrayedV3d(ltm, vtxCnt, data, data);

                    normalizedFlag = TRUE;
                    normalized = TRUE;
                }
                else if (usage & akxVertexUsage_BIT)
                {
                    AfxPostMultiplyArrayedV3d(ltm, vtxCnt, data, data);

                    normalizedFlag = TRUE;
                    normalized = TRUE;
                }                
                else if (usage & akxVertexUsage_TBC)
                {
                    if (deltaFlag)
                    {
                        AfxPostMultiplyArrayedV3d(ltm, vtxCnt, data, data);
                    }
                    else
                    {
                        AfxPostMultiplyArrayedV3d(iltm, vtxCnt, data, data);
                    }

                    normalizedFlag = TRUE;
                    normalized = TRUE;
                }
                else if (usage & akxVertexUsage_NRM)
                {
                    if (deltaFlag)
                    {
                        AfxPostMultiplyArrayedV3d(ltm, vtxCnt, data, data);
                    }
                    else
                    {
                        AfxPostMultiplyArrayedV3d(iltm, vtxCnt, data, data);
                    }

                    normalizedFlag = TRUE;
                    normalized = TRUE;
                }

                if (renormalize && normalized)
                {
                    if (fmt == afxVertexFormat_V2D)
                        AfxZeroOrNormalizeArrayedV2d(vtxCnt, data, data);
                    else if (fmt == afxVertexFormat_V3D)
                        AfxZeroOrNormalizeArrayedV3d(vtxCnt, data, data);
                    else if (fmt == afxVertexFormat_V4D)
                        AfxZeroOrNormalizeArrayedV4d(vtxCnt, data, data);
                }
#endif
#if 0
                if (data && AkxGetVertexAttributeFlags(vtd2, j) & akxVertexUsage_SPATIAL)
                {
                    switch (AkxGetVertexAttributeFormat(vtd2, j))
                    {
                    case afxVertexFormat_V2D:
                    {
                        AfxThrowError();
                        break;
                    }
                    case afxVertexFormat_V3D:
                    {
                        AfxThrowError(); // inoperant

                        if (linearFlag)
                            AfxPostMultiplyArrayedV3d(lt, vtxCnt, data, data);

                        if (invLinearFlag)
                            AfxPostMultiplyArrayedV3d(ilt, vtxCnt, data, data);

                        if (affineFlag)
                            for (afxNat k = 0; k < vtxCnt; k++)
                                AfxAddV3d(((afxV3d*)data)[k], ((afxV3d*)data)[k], at);

                        if (renormalize)
                            for (afxNat k = 0; k < vtxCnt; k++)
                                AfxNormalizeV3d(((afxV3d*)data)[k], ((afxV3d*)data)[k]);

                        break;
                    }
                    case afxVertexFormat_V4D:
                    {
                        AfxThrowError(); // inoperant

                        if (linearFlag)
                            AfxPostMultiplyArrayedLtv4d(lt, vtxCnt, data, data);

                        if (invLinearFlag)
                            AfxPostMultiplyArrayedLtv4d(ilt, vtxCnt, data, data);

                        if (affineFlag)
                            for (afxNat k = 0; k < vtxCnt; k++)
                                AfxAddV3d(((afxV4d*)data)[k], ((afxV4d*)data)[k], at);

                        if (renormalize)
                            for (afxNat k = 0; k < vtxCnt; k++)
                                AfxNormalizeV4d(((afxV4d*)data)[k], ((afxV4d*)data)[k]);

                        break;
                    }
                    default: AfxThrowError(); break;
                    }
                }
#endif
            }
        }
    }
}