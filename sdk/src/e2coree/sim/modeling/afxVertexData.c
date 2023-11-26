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
#include "afx/sim/modeling/afxVertexData.h"
#include "afx/sim/afxSimulation.h"
#include "afx/math/afxMatrix.h"

_AFX void* AfxExposeVertexData(afxVertexData vtd, afxNat attrIdx, afxNat baseVtxIdx)
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

_AFX afxError AfxBindVertexData(afxVertexData vtd, afxDrawScript dscr)
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
        afxVertexDataCache* cache = AfxGetVertexDataCache(vtd, i);

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

            if (vtd->attrs[cache->comps[j].attrIdx].usage & afxVertexUsage_POS)
                locations[attrCnt] = 0;
            else if (vtd->attrs[cache->comps[j].attrIdx].usage & afxVertexUsage_JNT)
                locations[attrCnt] = 1;
            else if (vtd->attrs[cache->comps[j].attrIdx].usage & afxVertexUsage_WGT)
                locations[attrCnt] = 2;
            else if (vtd->attrs[cache->comps[j].attrIdx].usage & afxVertexUsage_NRM)
                locations[attrCnt] = 3;
            else if (vtd->attrs[cache->comps[j].attrIdx].usage & afxVertexUsage_TAN)
                locations[attrCnt] = 4;
            else if (vtd->attrs[cache->comps[j].attrIdx].usage & afxVertexUsage_BIT)
                locations[attrCnt] = 5;
            else if (vtd->attrs[cache->comps[j].attrIdx].usage & afxVertexUsage_UV)
                locations[attrCnt] = 6;

            ++attrCnt;
        }
    }

    AfxCmdBindVertexSources(dscr, 0, srcCnt, srcBufs, srcBases, srcRanges);

    AfxCmdResetVertexStreams(dscr, streamCnt, srcIndices, srcStrides, NIL);

    //AfxCmdResetVertexAttributes(dscr, attrCnt, locations, fmts, streamIndices, offsets);

    return err;
}

_AFX afxError AfxBufferizeVertexData(afxVertexData vtd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);

    afxSimulation sim = AfxGetObjectProvider(vtd);
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    for (afxNat i = 0; i < vtd->cacheCnt; i++)
    {
        afxVertexDataCache* cache = &vtd->caches[i];

        if (!cache->buf)
        {
            afxNat vtxSiz = 0;
            afxVertexUsage usage = NIL;

            for (afxNat j = 0; j < cache->compCnt; j++)
            {
                afxVertexDataComponent* comp = &cache->comps[j];

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
            spec.usage = afxBufferUsage_VERTEX;

            if (AfxAcquireBuffers(sim->dctx, 1, &cache->buf, &spec)) AfxThrowError();
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
                    afxVertexDataComponent* comp = &cache->comps[j];
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

_AFX afxVertexDataCache* AfxGetVertexDataCache(afxVertexData vtd, afxNat cacheIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssertRange(vtd->cacheCnt, cacheIdx, 1);
    return &vtd->caches[cacheIdx];
}

_AFX afxNat AfxFindVertexDataAttributes(afxVertexData vtd, afxNat cnt, afxString const id[], afxNat attrIdx[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssert(attrIdx);
    afxNat rslt = 0;

    for (afxNat i = 0; i < cnt; i++)
        attrIdx[i] = AFX_INVALID_INDEX;

    for (afxNat i = 0; i < vtd->attrCnt; i++)
    {
        if (0 == AfxCompareString(&id[rslt], &vtd->attrs[i].id.str))
            attrIdx[rslt++] = i;

        if (cnt == rslt)
            break;
    }
    return rslt;
}

_AFX afxVertexFormat AfxGetVertexAttributeFormat(afxVertexData vtd, afxNat attrIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssertRange(vtd->attrCnt, attrIdx, 1);
    return vtd->attrs[attrIdx].fmt;
}

_AFX afxVertexUsage AfxGetVertexAttributeUsage(afxVertexData vtd, afxNat attrIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssertRange(vtd->attrCnt, attrIdx, 1);
    return vtd->attrs[attrIdx].usage;
}

_AFX afxVertexFlags AfxGetVertexAttributeFlags(afxVertexData vtd, afxNat attrIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssertRange(vtd->attrCnt, attrIdx, 1);
    return vtd->attrs[attrIdx].flags;
}

_AFX afxNat AfxGetVertexAttributeCache(afxVertexData vtd, afxNat attrIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssertRange(vtd->attrCnt, attrIdx, 1);
    return vtd->attrs[attrIdx].cacheIdx;
}

_AFX afxError AfxGetVertexAttributeInfo(afxVertexData vtd, afxNat attrIdx, afxVertexFormat* fmt, afxVertexUsage* usage, afxVertexFlags* flags, afxNat* cacheIdx)
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

_AFX afxError AfxUpdateVertexData(afxVertexData vtd, afxNat attrIdx, afxNat baseVtxIdx, afxNat vtxCnt, void const* src, afxNat32 srcStride)
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

_AFX afxError AfxFillVertexData(afxVertexData vtd, afxNat attrIdx, afxNat baseVtxIdx, afxNat vtxCnt, void const* src)
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

_AFX afxError AfxNormalizeVertexData(afxVertexData vtd, afxNat attrIdx, afxNat baseVtxIdx, afxNat vtxCnt)
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

_AFX afxError AfxZeroVertexData(afxVertexData vtd, afxNat attrIdx, afxNat baseVtxIdx, afxNat vtxCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssertRange(vtd->attrCnt, attrIdx, 1);
    AfxAssertRange(vtd->vtxCnt, baseVtxIdx, vtxCnt);
    
    if (AfxFillVertexData(vtd, attrIdx, baseVtxIdx, vtxCnt, AFX_M4D_ZERO))
        AfxThrowError();

    return err;
}

_AFX afxError _AfxVtdDtor(afxVertexData vtd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxEntry("vtd=%p", vtd);

    afxSimulation sim = AfxGetObjectProvider(vtd);
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxContext mem = AfxSimulationGetMemory(sim);

    if (vtd->vtx)
        AfxDeallocate(mem, vtd->vtx);

    if (vtd->biases)
        AfxDeallocate(mem, vtd->biases);

    AfxThrowError();

    return err;
}

_AFX afxError _AfxVtdCtor(afxVertexData vtd, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxEntry("vtd=%p", vtd);

    afxSimulation sim = cookie->udd[0];
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxMeshBuilder const *mshb = cookie->udd[1];
    AfxAssert(mshb);
    AfxAssertType(mshb, afxFcc_MSHB);

    afxContext mem = AfxSimulationGetMemory(sim);

    afxNat pivotCnt = mshb->artCnt;
    afxNat totalBiasCnt = mshb->biases.cnt;
    afxNat vtxCnt = mshb->vtxCnt;

    vtd->biasCnt = totalBiasCnt;
    vtd->biases = NIL;

    if (totalBiasCnt && !(vtd->biases = AfxAllocate(mem, sizeof(vtd->biases[0]), totalBiasCnt, NIL, AfxHint()))) AfxThrowError();
    else
    {
        for (afxNat i = 0; i < totalBiasCnt; i++)
        {
            afxVertexBias const* vb = AfxGetArrayUnit(&mshb->biases, i);
            vtd->biases[i].pivotIdx = vb->pivotIdx;
            vtd->biases[i].weight = vb->weight;
            AfxAssert(1.0 >= vtd->biases[i].weight);
            AfxAssertRange(pivotCnt, vtd->biases[i].pivotIdx, 1);
        }

        vtd->vtxCnt = vtxCnt;
        vtd->vtx = NIL;

        if (vtxCnt && !(vtd->vtx = AfxAllocate(mem, sizeof(vtd->vtx[0]), vtxCnt, NIL, AfxHint()))) AfxThrowError();
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
        afxVertexAttrSpec spec[16];
        void*specData[16];

        spec[0].id = "posn";
        spec[0].cacheIdx = 0;
        spec[0].flags = afxVertexFlag_AFFINE | afxVertexFlag_LINEAR;
        spec[0].fmt = afxVertexFormat_V4D;
        spec[0].usage = afxVertexUsage_POS | afxVertexUsage_POSITIONAL;
        specData[0] = mshb->posn;

        spec[1].id = "nrm";
        spec[1].cacheIdx = 0;
        spec[1].flags = afxVertexFlag_LINEAR;
        spec[1].fmt = afxVertexFormat_V3D;
        spec[1].usage = afxVertexUsage_NRM | afxVertexUsage_TANGENT;
        specData[1] = mshb->nrm;

        spec[2].id = "uv";
        spec[2].cacheIdx = 0;
        spec[2].flags = NIL;
        spec[2].fmt = afxVertexFormat_V2D;
        spec[2].usage = afxVertexUsage_UV;
        specData[2] = mshb->uv;

        vtd->attrCnt = attrCnt;
        vtd->attrs = NIL;
        
        if (attrCnt && !(vtd->attrs = AfxAllocate(mem, sizeof(vtd->attrs[0]), attrCnt, NIL, AfxHint()))) AfxThrowError();
        else
        {
            for (afxNat i = 0; i < attrCnt; i++)
            {
                afxBool hasData = TRUE;
                
                AfxString8(&vtd->attrs[i].id);
                afxString tmp;
                AfxMakeString(&tmp, spec[i].id, 0);
                AfxCopyString(&vtd->attrs[i].id.str, &tmp);

                vtd->attrs[i].usage = spec[i].usage;
                vtd->attrs[i].flags = NIL;

                if (vtd->attrs[i].usage & afxVertexUsage_POS)
                    vtd->attrs[i].flags |= afxVertexFlag_AFFINE | afxVertexFlag_LINEAR;

                if (vtd->attrs[i].usage & afxVertexUsage_POS)
                    vtd->attrs[i].flags |= afxVertexFlag_AFFINE | afxVertexFlag_LINEAR;

                vtd->attrs[i].flags |= spec[i].flags;
                AfxAssert(spec[i].fmt < afxVertexFormat_TOTAL);
                vtd->attrs[i].fmt = spec[i].fmt;
                vtd->attrs[i].data = NIL;
                vtd->attrs[i].cacheIdx = spec[i].cacheIdx;

                afxSize vtxSiz = AfxVertexFormatGetSize(vtd->attrs[i].fmt);

                if (!(vtd->attrs[i].data = AfxAllocate(mem, vtxSiz, vtxCnt, AFX_SIMD_ALIGN, AfxHint()))) AfxThrowError();
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

        if (cacheCnt && !(vtd->caches = AfxAllocate(mem, sizeof(vtd->caches[0]), cacheCnt, NIL, AfxHint()))) AfxThrowError();
        else
        {
            for (afxNat i = 0; i < cacheCnt; i++)
            {
                afxVertexDataCache* cache = &vtd->caches[i];

                AfxResetLinkage(&cache->stream);

                cache->buf = NIL;
                cache->base = 0;
                cache->range = 0;
                cache->stride = 0;

                cache->vtd = vtd;
                cache->usage = NIL;

                cache->compCnt = cacheAttrCnt[i];
                AfxAssert(cache->compCnt);

                if (!(cache->comps = AfxAllocate(mem, sizeof(cache->comps[0]), cache->compCnt, NIL, AfxHint()))) AfxThrowError();
                else
                {
                    for (afxNat j = 0; j < cache->compCnt; j++)
                    {
                        afxVertexDataComponent* comp = &cache->comps[j];

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

_AFX afxVertexData AfxBuildVertexData(afxSimulation sim, afxMeshBuilder const* mshb)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AfxAssert(mshb);
    afxVertexData vtd = NIL;

    if (AfxAcquireObjects(AfxGetMeshDataClass(sim), 1, (afxHandle*)&vtd, (void*[]) { sim, (void*)mshb }))
        AfxThrowError();

    return vtd;
}

_AFX void AfxTransformVertexDatas(afxReal const ltm[3][3], afxReal const iltm[3][3], afxReal const atv[4], afxBool renormalize, afxNat cnt, afxVertexData vtd[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(atv);
    AfxAssert(ltm);
    AfxAssert(iltm);
    AfxAssert(cnt);
    AfxAssert(vtd);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxVertexData vtd2 = vtd[i];

        if (vtd2)
        {
            AfxAssertObjects(1, &vtd2, afxFcc_VTD);

            afxNat baseVtxIdx = 0;
            afxNat vtxCnt = vtd2->vtxCnt;

            for (afxNat j = 0; j < vtd2->attrCnt; j++)
            {
                void* data = AfxExposeVertexData(vtd2, j, baseVtxIdx);
                afxVertexFlags flags = AfxGetVertexAttributeFlags(vtd2, j);
                afxBool linearFlag = flags & afxVertexFlag_LINEAR;
                afxBool invLinearFlag = flags & afxVertexFlag_LINEAR_INV;
                afxBool affineFlag = flags & afxVertexFlag_AFFINE;
                afxBool deltaFlag = flags & afxVertexFlag_DELTA;
                afxVertexUsage usage = AfxGetVertexAttributeUsage(vtd2, j);
                afxVertexFormat fmt = AfxGetVertexAttributeFormat(vtd2, j);

                if (usage & afxVertexUsage_POS)
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
                else if (usage & afxVertexUsage_TAN)
                {
                    AfxPostMultiplyArrayedV3d(ltm, vtxCnt, data, data);
                }
                else if (usage & afxVertexUsage_BIT)
                {
                    AfxPostMultiplyArrayedV3d(ltm, vtxCnt, data, data);
                }
                

                if (usage & afxVertexUsage_TBC)
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
                else if (usage & afxVertexUsage_NRM)
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
                if (data && AfxGetVertexAttributeFlags(vtd2, j) & afxVertexUsage_SPATIAL)
                {
                    switch (AfxGetVertexAttributeFormat(vtd2, j))
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
    .size = sizeof(AFX_OBJECT(afxVertexData)),
    .ctx = NIL,
    .ctor = (void*)_AfxVtdCtor,
    .dtor = (void*)_AfxVtdDtor
};
