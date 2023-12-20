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
#define _AFX_VERTEX_DATA_C
#define _AFX_SIMULATION_C
#include "qwadro/sim/modeling/awxVertexData.h"
#include "qwadro/sim/awxSimulation.h"
#include "qwadro/math/afxMatrix.h"

_AFX void* AwxExposeVertexData(awxVertexData vtd, afxNat attrIdx, afxNat baseVtxIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssertRange(vtd->attrCnt, attrIdx, 1);
    AfxAssertRange(vtd->vtxCnt, baseVtxIdx, 1);
    
    afxVertexFormat fmt = vtd->attrs[attrIdx].fmt;
    afxNat32 unitSiz = AfxVertexFormatGetSize(fmt);
    afxByte *data = vtd->attrs[attrIdx].data;

    if (!data)
        AfxThrowError();
    else
        data += baseVtxIdx * unitSiz;

    return data;
}

_AFX afxError AwxBindVertexData(awxVertexData vtd, afxDrawScript dscr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);

    afxNat srcCnt = 0;
    afxBuffer srcBufs[16];
    afxNat32 srcBases[16];
    afxNat32 srcRanges[16];
    afxNat32 srcStrides[16];

    afxNat streamCnt = 0;
    afxNat srcIndices[16];

    afxNat attrCnt = 0;
    afxNat locations[16];
    afxVertexFormat fmts[16];
    afxNat streamIndices[16];
    afxNat32 offsets[16];

    for (afxNat i = 0; i < vtd->cacheCnt; i++)
    {
        awxVertexDataCache* cache = AwxGetVertexDataCache(vtd, i);

        srcBufs[srcCnt] = cache->buf;
        srcBases[srcCnt] = cache->base;
        srcRanges[srcCnt] = cache->range;
        ++srcCnt;
        
        srcIndices[streamCnt] = i;
        srcStrides[streamCnt] = cache->stride;
        ++streamCnt;

        for (afxNat j = 0; j < cache->compCnt; j++)
        {
            fmts[attrCnt] = cache->comps[j].fmt;
            streamIndices[attrCnt] = i;
            offsets[attrCnt] = cache->comps[j].offset;

            if (vtd->attrs[cache->comps[j].attrIdx].usage & awxVertexUsage_POS)
                locations[attrCnt] = 0;
            else if (vtd->attrs[cache->comps[j].attrIdx].usage & awxVertexUsage_JNT)
                locations[attrCnt] = 1;
            else if (vtd->attrs[cache->comps[j].attrIdx].usage & awxVertexUsage_WGT)
                locations[attrCnt] = 2;
            else if (vtd->attrs[cache->comps[j].attrIdx].usage & awxVertexUsage_NRM)
                locations[attrCnt] = 3;
            else if (vtd->attrs[cache->comps[j].attrIdx].usage & awxVertexUsage_TAN)
                locations[attrCnt] = 4;
            else if (vtd->attrs[cache->comps[j].attrIdx].usage & awxVertexUsage_BIT)
                locations[attrCnt] = 5;
            else if (vtd->attrs[cache->comps[j].attrIdx].usage & awxVertexUsage_UV)
                locations[attrCnt] = 6;

            ++attrCnt;
        }
    }

    AfxCmdBindVertexSources(dscr, 0, srcCnt, srcBufs, srcBases, srcRanges);

    //AfxCmdResetVertexStreams(dscr, streamCnt, srcIndices, srcStrides, NIL);

    //AfxCmdResetVertexAttributes(dscr, attrCnt, locations, fmts, streamIndices, offsets);

    return err;
}

_AFX afxError AwxBufferizeVertexData(awxVertexData vtd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);

    awxSimulation sim = AfxGetObjectProvider(vtd);
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    for (afxNat i = 0; i < vtd->cacheCnt; i++)
    {
        awxVertexDataCache* cache = &vtd->caches[i];

        if (!cache->buf)
        {
            afxNat vtxSiz = 0;
            awxVertexUsage usage = NIL;

            for (afxNat j = 0; j < cache->compCnt; j++)
            {
                awxVertexDataComponent* comp = &cache->comps[j];

                afxNat attrIdx = comp->attrIdx;
                AfxAssertRange(vtd->attrCnt, attrIdx, 1);
                afxVertexFormat fmt = vtd->attrs[attrIdx].fmt;
                AfxAssert(fmt < afxVertexFormat_TOTAL);
                comp->fmt = fmt;
                usage |= vtd->attrs[attrIdx].usage;
                comp->offset = vtxSiz;
                vtxSiz += AfxVertexFormatGetSize(fmt);
                AfxAssert(vtxSiz);
            }

            afxBufferSpecification spec;
            spec.siz = vtd->vtxCnt * vtxSiz;
            spec.src = NIL;
            spec.access = afxBufferAccess_W;
            spec.usage = afxBufferUsage_VERTEX;

            if (AfxAcquireBuffers(sim->dctx, 1, &spec, &cache->buf)) AfxThrowError();
            else
            {
                AfxAssertObjects(1, &cache->buf, afxFcc_BUF);

                cache->base = 0;
                cache->range = spec.siz;
                cache->stride = cache->range / vtd->vtxCnt;

                cache->usage = usage;
            }

            afxBuffer buf;

            if ((buf = cache->buf))
            {
                AfxAssertObjects(1, &buf, afxFcc_BUF);
                afxNat compCnt = cache->compCnt;

                for (afxNat j = 0; j < compCnt; j++)
                {
                    awxVertexDataComponent* comp = &cache->comps[j];
                    afxNat attrIdx = comp->attrIdx;
                    AfxAssertRange(vtd->attrCnt, attrIdx, 1);
                    AfxAssert(vtd->attrs[attrIdx].data);
                    afxVertexFormat fmt = vtd->attrs[attrIdx].fmt;
                    AfxAssert(fmt < afxVertexFormat_TOTAL);
                    afxSize srcStride = AfxVertexFormatGetSize(fmt);
                    AfxAssert(srcStride);
                    AfxAssert(cache->stride);
                    AfxAssertRange(AfxGetBufferSize(buf), cache->base, cache->range);
                    //AfxUpdateBuffer2(buf, cache->base, cache->range, cache->stride, vtd->attrs[attrIdx].data, srcStride);

                    afxBufferRegion rgn;
                    rgn.base = cache->base;
                    rgn.range = cache->range;
                    rgn.stride = cache->stride;

                    rgn.offset = comp->offset;
                    rgn.unitSiz = AfxVertexFormatGetSize(comp->fmt);

                    if (vtd->attrs[attrIdx].data)
                        AfxUpdateBufferRegion(buf, &rgn, vtd->attrs[attrIdx].data, srcStride);
                    // COMO USAR A OFFSET RELATIVA???
                }
            }
        }
    }

    return err;
}

_AFX awxVertexDataCache* AwxGetVertexDataCache(awxVertexData vtd, afxNat cacheIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssertRange(vtd->cacheCnt, cacheIdx, 1);
    return &vtd->caches[cacheIdx];
}

_AFX afxNat AwxFindVertexDataAttributes(awxVertexData vtd, afxNat cnt, afxString const id[], afxNat attrIdx[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssert(attrIdx);
    afxNat rslt = 0;

    for (afxNat i = 0; i < cnt; i++)
        attrIdx[i] = AFX_INVALID_INDEX;

    for (afxNat i = 0; i < vtd->attrCnt; i++)
    {
        if (0 == AfxCompareString(&id[rslt], &vtd->attrs[i].id.str.str))
            attrIdx[rslt++] = i;

        if (cnt == rslt)
            break;
    }
    return rslt;
}

_AFX afxVertexFormat AwxGetVertexAttributeFormat(awxVertexData vtd, afxNat attrIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssertRange(vtd->attrCnt, attrIdx, 1);
    return vtd->attrs[attrIdx].fmt;
}

_AFX awxVertexUsage AwxGetVertexAttributeUsage(awxVertexData vtd, afxNat attrIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssertRange(vtd->attrCnt, attrIdx, 1);
    return vtd->attrs[attrIdx].usage;
}

_AFX awxVertexFlags AwxGetVertexAttributeFlags(awxVertexData vtd, afxNat attrIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssertRange(vtd->attrCnt, attrIdx, 1);
    return vtd->attrs[attrIdx].flags;
}

_AFX afxNat AwxGetVertexAttributeCache(awxVertexData vtd, afxNat attrIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssertRange(vtd->attrCnt, attrIdx, 1);
    return vtd->attrs[attrIdx].cacheIdx;
}

_AFX afxError AwxGetVertexAttributeInfo(awxVertexData vtd, afxNat attrIdx, afxVertexFormat* fmt, awxVertexUsage* usage, awxVertexFlags* flags, afxNat* cacheIdx)
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

    if (cacheIdx)
        *cacheIdx = vtd->attrs[attrIdx].cacheIdx;

    return err;
}

_AFX afxError AwxUpdateVertexData(awxVertexData vtd, afxNat attrIdx, afxNat baseVtxIdx, afxNat vtxCnt, void const* src, afxNat32 srcStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssertRange(vtd->attrCnt, attrIdx, 1);
    AfxAssertRange(vtd->vtxCnt, baseVtxIdx, vtxCnt);
    AfxAssert(src);
    AfxAssert(srcStride);
#if 0
    afxByte const* src2 = src;
    afxVertexFormat fmt = vtd->attrs[attrIdx].fmt;
    afxNat32 unitSiz = AfxVertexFormatGetSize(fmt);
    afxNat srcIdx = vtd->attrs[attrIdx].srcIdx;
    afxSize combOff = vtd->srcs[srcIdx].offset + (baseVtxIdx * unitSiz);
    afxBuffer buf = vtd->srcs[srcIdx].buf;
    AfxAssertObjects(1, &buf, afxFcc_BUF);

    if (AfxUpdateBuffer2(buf, combOff, vtxCnt * unitSiz, unitSiz, src, srcStride))
        AfxThrowError();
#endif

    afxVertexFormat fmt = vtd->attrs[attrIdx].fmt;
    afxNat32 unitSiz = AfxVertexFormatGetSize(fmt);

    if (!vtd->attrs[attrIdx].data)
        if (!(vtd->attrs[attrIdx].data = AfxAllocate(NIL, unitSiz, vtd->vtxCnt, AFX_SIMD_ALIGN, AfxHint())))
            AfxThrowError();

    if (!err)
    {
        AfxStream(vtxCnt, srcStride, unitSiz, src, vtd->attrs[attrIdx].data);
    }
    return err;
}

_AFX afxError AwxFillVertexData(awxVertexData vtd, afxNat attrIdx, afxNat baseVtxIdx, afxNat vtxCnt, void const* src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssertRange(vtd->attrCnt, attrIdx, 1);
    AfxAssertRange(vtd->vtxCnt, baseVtxIdx, vtxCnt);
    AfxAssert(src);

    afxVertexFormat fmt = vtd->attrs[attrIdx].fmt;
    afxNat32 unitSiz = AfxVertexFormatGetSize(fmt);

    if (!vtd->attrs[attrIdx].data)
        if (!(vtd->attrs[attrIdx].data = AfxAllocate(NIL, unitSiz, vtd->vtxCnt, AFX_SIMD_ALIGN, AfxHint())))
            AfxThrowError();

    if (!err)
    {
        AfxStream(vtxCnt, 0, unitSiz, src, vtd->attrs[attrIdx].data);
    }
    return err;
}

_AFX afxError AwxNormalizeVertexData(awxVertexData vtd, afxNat attrIdx, afxNat baseVtxIdx, afxNat vtxCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssertRange(vtd->attrCnt, attrIdx, 1);
    AfxAssertRange(vtd->vtxCnt, baseVtxIdx, vtxCnt);
    
    afxVertexFormat fmt = vtd->attrs[attrIdx].fmt;
    AfxAssert(fmt == afxVertexFormat_V2D || fmt == afxVertexFormat_V3D || fmt == afxVertexFormat_V4D);

    afxNat32 unitSiz = AfxVertexFormatGetSize(fmt);

    if (!vtd->attrs[attrIdx].data)
        if (!(vtd->attrs[attrIdx].data = AfxAllocate(NIL, unitSiz, vtd->vtxCnt, AFX_SIMD_ALIGN, AfxHint())))
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

_AFX afxError AwxZeroVertexData(awxVertexData vtd, afxNat attrIdx, afxNat baseVtxIdx, afxNat vtxCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssertRange(vtd->attrCnt, attrIdx, 1);
    AfxAssertRange(vtd->vtxCnt, baseVtxIdx, vtxCnt);
    
    if (AwxFillVertexData(vtd, attrIdx, baseVtxIdx, vtxCnt, AFX_M4D_ZERO))
        AfxThrowError();

    return err;
}

_AFX afxError _AfxVtdDtor(awxVertexData vtd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxEntry("vtd=%p", vtd);

    awxSimulation sim = AfxGetObjectProvider(vtd);
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxMmu mmu = AfxSimulationGetMemory(sim);

    if (vtd->vtx)
        AfxDeallocate(mmu, vtd->vtx);

    if (vtd->biases)
        AfxDeallocate(mmu, vtd->biases);

    AfxThrowError();

    return err;
}

_AFX afxError _AfxVtdCtor(awxVertexData vtd, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxEntry("vtd=%p", vtd);

    awxSimulation sim = cookie->udd[0];
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    awxMeshBuilder const *mshb = cookie->udd[1];
    AfxAssert(mshb);
    AfxAssertType(mshb, afxFcc_MSHB);

    afxMmu mmu = AfxSimulationGetMemory(sim);

    afxNat pivotCnt = mshb->artCnt;
    afxNat totalBiasCnt = mshb->biases.cnt;
    afxNat vtxCnt = mshb->vtxCnt;

    vtd->biasCnt = totalBiasCnt;
    vtd->biases = NIL;

    if (totalBiasCnt && !(vtd->biases = AfxAllocate(mmu, sizeof(vtd->biases[0]), totalBiasCnt, NIL, AfxHint()))) AfxThrowError();
    else
    {
        for (afxNat i = 0; i < totalBiasCnt; i++)
        {
            awxVertexBias const* vb = AfxGetArrayUnit(&mshb->biases, i);
            vtd->biases[i].pivotIdx = vb->pivotIdx;
            vtd->biases[i].weight = vb->weight;
            AfxAssert(1.0 >= vtd->biases[i].weight);
            AfxAssertRange(pivotCnt, vtd->biases[i].pivotIdx, 1);
        }

        vtd->vtxCnt = vtxCnt;
        vtd->vtx = NIL;

        if (vtxCnt && !(vtd->vtx = AfxAllocate(mmu, sizeof(vtd->vtx[0]), vtxCnt, NIL, AfxHint()))) AfxThrowError();
        else
        {
            for (afxNat i = 0; i < vtxCnt; i++)
            {
                afxVertex const* vd = &mshb->vtx[i];
                vtd->vtx[i].baseBiasIdx = vd->baseBiasIdx;
                vtd->vtx[i].biasCnt = vd->biasCnt;
                AfxAssertRange(totalBiasCnt, vtd->vtx[i].baseBiasIdx, vtd->vtx[i].biasCnt);
            }
        }

        afxNat attrCnt = 3;
        afxNat cacheCnt = 0;
        afxNat cacheAttrCnt[16] = { 0 };
        afxNat cacheAttrMap[16][16] = { 0 };
        awxVertexAttrSpec spec[16];
        void*specData[16];

        spec[0].id = "posn";
        spec[0].cacheIdx = 0;
        spec[0].flags = awxVertexFlag_AFFINE | awxVertexFlag_LINEAR;
        spec[0].fmt = afxVertexFormat_V4D;
        spec[0].usage = awxVertexUsage_POS | awxVertexUsage_POSITIONAL;
        specData[0] = mshb->posn;

        spec[1].id = "nrm";
        spec[1].cacheIdx = 0;
        spec[1].flags = awxVertexFlag_LINEAR;
        spec[1].fmt = afxVertexFormat_V3D;
        spec[1].usage = awxVertexUsage_NRM | awxVertexUsage_TANGENT;
        specData[1] = mshb->nrm;

        spec[2].id = "uv";
        spec[2].cacheIdx = 0;
        spec[2].flags = NIL;
        spec[2].fmt = afxVertexFormat_V2D;
        spec[2].usage = awxVertexUsage_UV;
        specData[2] = mshb->uv;

        vtd->attrCnt = attrCnt;
        vtd->attrs = NIL;
        
        if (attrCnt && !(vtd->attrs = AfxAllocate(mmu, sizeof(vtd->attrs[0]), attrCnt, NIL, AfxHint()))) AfxThrowError();
        else
        {
            for (afxNat i = 0; i < attrCnt; i++)
            {
                afxBool hasData = TRUE;
                
                AfxMakeFixedString8(&vtd->attrs[i].id, 0);
                afxString tmp;
                AfxMakeString(&tmp, spec[i].id, 0);
                AfxCopyString(&vtd->attrs[i].id.str, &tmp);

                vtd->attrs[i].usage = spec[i].usage;
                vtd->attrs[i].flags = NIL;

                if (vtd->attrs[i].usage & awxVertexUsage_POS)
                    vtd->attrs[i].flags |= awxVertexFlag_AFFINE | awxVertexFlag_LINEAR;

                if (vtd->attrs[i].usage & awxVertexUsage_POS)
                    vtd->attrs[i].flags |= awxVertexFlag_AFFINE | awxVertexFlag_LINEAR;

                vtd->attrs[i].flags |= spec[i].flags;
                AfxAssert(spec[i].fmt < afxVertexFormat_TOTAL);
                vtd->attrs[i].fmt = spec[i].fmt;
                vtd->attrs[i].data = NIL;
                vtd->attrs[i].cacheIdx = spec[i].cacheIdx;

                afxSize vtxSiz = AfxVertexFormatGetSize(vtd->attrs[i].fmt);

                if (!(vtd->attrs[i].data = AfxAllocate(mmu, vtxSiz, vtxCnt, AFX_SIMD_ALIGN, AfxHint()))) AfxThrowError();
                else
                {
                    if (specData[i])
                    {
                        AfxCopy(vtxCnt, vtxSiz, specData[i], vtd->attrs[i].data);
                    }
                    else
                    {

                    }
                }

                afxNat cacheIdx = vtd->attrs[i].cacheIdx;

                if (!cacheAttrCnt[cacheIdx])
                    ++cacheCnt;

                cacheAttrMap[cacheIdx][cacheAttrCnt[cacheIdx]] = i;
                ++cacheAttrCnt[cacheIdx];
            }
        }

        vtd->caches = NIL;

        if (cacheCnt && !(vtd->caches = AfxAllocate(mmu, sizeof(vtd->caches[0]), cacheCnt, NIL, AfxHint()))) AfxThrowError();
        else
        {
            for (afxNat i = 0; i < cacheCnt; i++)
            {
                awxVertexDataCache* cache = &vtd->caches[i];

                AfxResetLinkage(&cache->stream);

                cache->buf = NIL;
                cache->base = 0;
                cache->range = 0;
                cache->stride = 0;

                cache->vtd = vtd;
                cache->usage = NIL;

                cache->compCnt = cacheAttrCnt[i];
                AfxAssert(cache->compCnt);

                if (!(cache->comps = AfxAllocate(mmu, sizeof(cache->comps[0]), cache->compCnt, NIL, AfxHint()))) AfxThrowError();
                else
                {
                    for (afxNat j = 0; j < cache->compCnt; j++)
                    {
                        awxVertexDataComponent* comp = &cache->comps[j];

                        comp->attrIdx = cacheAttrMap[i][j];
                        comp->fmt = NIL;
                        comp->offset = 0;
                    }
                }
            }
            vtd->cacheCnt = cacheCnt;
        }
    }
    return err;
}

_AFX awxVertexData AwxBuildVertexData(awxSimulation sim, awxMeshBuilder const* mshb)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AfxAssert(mshb);
    awxVertexData vtd = NIL;

    if (AfxAcquireObjects(AwxGetMeshDataClass(sim), 1, (afxObject*)&vtd, (void const*[]) { sim, (void*)mshb }))
        AfxThrowError();

    return vtd;
}

_AFX void AwxTransformVertexDatas(afxReal const ltm[3][3], afxReal const iltm[3][3], afxReal const atv[4], afxBool renormalize, afxNat cnt, awxVertexData vtd[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(atv);
    AfxAssert(ltm);
    AfxAssert(iltm);
    AfxAssert(cnt);
    AfxAssert(vtd);

    for (afxNat i = 0; i < cnt; i++)
    {
        awxVertexData vtd2 = vtd[i];

        if (vtd2)
        {
            AfxAssertObjects(1, &vtd2, afxFcc_VTD);

            afxNat baseVtxIdx = 0;
            afxNat vtxCnt = vtd2->vtxCnt;

            for (afxNat j = 0; j < vtd2->attrCnt; j++)
            {
                void* data = AwxExposeVertexData(vtd2, j, baseVtxIdx);
                awxVertexFlags flags = AwxGetVertexAttributeFlags(vtd2, j);
                afxBool linearFlag = flags & awxVertexFlag_LINEAR;
                afxBool invLinearFlag = flags & awxVertexFlag_LINEAR_INV;
                afxBool affineFlag = flags & awxVertexFlag_AFFINE;
                afxBool deltaFlag = flags & awxVertexFlag_DELTA;
                awxVertexUsage usage = AwxGetVertexAttributeUsage(vtd2, j);
                afxVertexFormat fmt = AwxGetVertexAttributeFormat(vtd2, j);

                if (usage & awxVertexUsage_POS)
                {
                    if (fmt == afxVertexFormat_V4D)
                        AfxPostMultiplyArrayedLtv4(ltm, vtxCnt, data, data);
                    else if (fmt == afxVertexFormat_V3D)
                        AfxPostMultiplyArrayedV3d(ltm, vtxCnt, data, data);

                    if (!deltaFlag)
                    {
                        if (fmt == afxVertexFormat_V4D)
                            for (afxNat k = 0; k < vtxCnt; k++)
                                AfxAddV4d(((afxV4d*)data)[k], ((afxV4d*)data)[k], atv);
                        else if (fmt == afxVertexFormat_V3D)
                            for (afxNat k = 0; k < vtxCnt; k++)
                                AfxAddV3d(((afxV3d*)data)[k], ((afxV3d*)data)[k], atv);
                    }
                }
#if !0
                else if (usage & awxVertexUsage_TAN)
                {
                    AfxPostMultiplyArrayedV3d(ltm, vtxCnt, data, data);
                }
                else if (usage & awxVertexUsage_BIT)
                {
                    AfxPostMultiplyArrayedV3d(ltm, vtxCnt, data, data);
                }
                

                if (usage & awxVertexUsage_TBC)
                {
                    if (deltaFlag)
                    {
                        AfxPostMultiplyArrayedV3d(ltm, vtxCnt, data, data);
                    }
                    else
                    {
                        AfxPostMultiplyArrayedV3d(iltm, vtxCnt, data, data);
                    }
                }
                else if (usage & awxVertexUsage_NRM)
                {
                    if (deltaFlag)
                    {
                        AfxPostMultiplyArrayedV3d(ltm, vtxCnt, data, data);
                    }
                    else
                    {
                        AfxPostMultiplyArrayedV3d(iltm, vtxCnt, data, data);
                    }
                }

                if (renormalize)
                {
                    if (fmt == afxVertexFormat_V4D)
                        AfxNormalizeOrZeroArrayedV4d(vtxCnt, data, data);
                    else if (fmt == afxVertexFormat_V3D)
                        AfxNormalizeOrZeroArrayedV3d(vtxCnt, data, data);
                }
#endif
#if 0
                if (data && AwxGetVertexAttributeFlags(vtd2, j) & awxVertexUsage_SPATIAL)
                {
                    switch (AwxGetVertexAttributeFormat(vtd2, j))
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
                                AfxNormalV3d(((afxV3d*)data)[k], ((afxV3d*)data)[k]);

                        break;
                    }
                    case afxVertexFormat_V4D:
                    {
                        AfxThrowError(); // inoperant

                        if (linearFlag)
                            AfxPostMultiplyArrayedLtv4(lt, vtxCnt, data, data);

                        if (invLinearFlag)
                            AfxPostMultiplyArrayedLtv4(ilt, vtxCnt, data, data);

                        if (affineFlag)
                            for (afxNat k = 0; k < vtxCnt; k++)
                                AfxAddV3d(((afxV4d*)data)[k], ((afxV4d*)data)[k], at);

                        if (renormalize)
                            for (afxNat k = 0; k < vtxCnt; k++)
                                AfxNormalV4d(((afxV4d*)data)[k], ((afxV4d*)data)[k]);

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

_AFX afxClassConfig _AfxVtdClsConfig =
{
    .fcc = afxFcc_VTD,
    .name = "Vertex Data",
    .unitsPerPage = 8,
    .size = sizeof(AFX_OBJECT(awxVertexData)),
    .mmu = NIL,
    .ctor = (void*)_AfxVtdCtor,
    .dtor = (void*)_AfxVtdDtor
};
