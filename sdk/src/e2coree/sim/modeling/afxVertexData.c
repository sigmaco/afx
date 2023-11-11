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

            afxSimulation sim = AfxGetObjectProvider(vtd);
            AfxAssertObjects(1, &sim, afxFcc_SIM);

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

        if (cnt >= rslt)
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
        if (!(vtd->attrs[attrIdx].data = AfxAllocate(NIL, vtd->vtxCnt * unitSiz, AFX_SIMD_ALIGN, AfxSpawnHint())))
            AfxThrowError();

    if (vtd->attrs[attrIdx].data)
    {
        AfxStream(vtxCnt, srcStride, unitSiz, src, vtd->attrs[attrIdx].data);
    }
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

    if (vtd->weights)
        AfxDeallocate(mem, vtd->weights);

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
    void* data = cookie->udd[2] ? ((void **)(cookie->udd[2]))[cookie->no] : NIL;
    AfxAssert(data);

    afxContext mem = AfxSimulationGetMemory(sim);

    afxBool isBiased = FALSE;
    afxNat vtxCnt = { 0 }, weightCnt = { 0 }, attrCnt = { 0 };
    AfxAssert(mshb->GetVertexInfo);
    mshb->GetVertexInfo(data, &vtxCnt, &isBiased, &weightCnt, &attrCnt);

    vtd->weightCnt = weightCnt;
    vtd->vtxCnt = vtxCnt;
    vtd->biases = NIL;
    vtd->weights = NIL;

    if (isBiased)
    {
        if (weightCnt && !(vtd->weights = AfxAllocate(mem, weightCnt * sizeof(vtd->weights[0]), 0, AfxSpawnHint()))) AfxThrowError();
        else
        {
            if (vtd->weights)
            {
                AfxAssert(mshb->GetVertexWeights);

                for (afxNat i = 0; i < weightCnt; i++)
                    mshb->GetVertexWeights(data, i, 1, vtd->weights);
            }
        }

        if (vtxCnt && !(vtd->biases = AfxAllocate(mem, vtxCnt * sizeof(vtd->biases[0]), 0, AfxSpawnHint()))) AfxThrowError();
        else
        {
            if (vtd->biases)
            {
                AfxAssert(mshb->GetVertexBiases);

                for (afxNat i = 0; i < vtxCnt; i++)
                    mshb->GetVertexBiases(data, 0, vtxCnt, vtd->biases);
            }
        }
    }

    afxNat cacheCnt = 0;
    afxNat cacheAttrCnt[16] = { 0 };
    afxNat cacheAttrMap[16][16] = { 0 };

    if (attrCnt && !(vtd->attrs = AfxAllocate(NIL, attrCnt * sizeof(vtd->attrs[0]), NIL, AfxSpawnHint()))) AfxThrowError();
    else
    {
        if (vtd->attrs)
        {
            AfxAssert(mshb->GetVertexSpecs);

            for (afxNat i = 0; i < attrCnt; i++)
            {
                afxBool hasData = FALSE;
                afxVertexAttrSpec spec = { 0 };
                mshb->GetVertexSpecs(data, i, 1, &spec, &hasData);

                AfxString8(&vtd->attrs[i].id);
                afxString tmp;
                AfxMakeString(&tmp, spec.id, 0);
                AfxCopyString(&vtd->attrs[i].id.str, &tmp);

                vtd->attrs[i].usage = spec.usage;
                vtd->attrs[i].flags = NIL;

                if (vtd->attrs[i].usage & afxVertexUsage_POS)
                    vtd->attrs[i].flags |= afxVertexFlag_AFFINE | afxVertexFlag_LINEAR;

                if (vtd->attrs[i].usage & afxVertexUsage_POS)
                    vtd->attrs[i].flags |= afxVertexFlag_AFFINE | afxVertexFlag_LINEAR;

                vtd->attrs[i].flags |= spec.flags;
                AfxAssert(spec.fmt < afxVertexFormat_TOTAL);
                vtd->attrs[i].fmt = spec.fmt;
                vtd->attrs[i].data = NIL;
                vtd->attrs[i].cacheIdx = spec.cacheIdx;

                afxSize vtxSiz = AfxVertexFormatGetSize(vtd->attrs[i].fmt);

                if (hasData && !(vtd->attrs[i].data = AfxAllocate(NIL, vtxSiz * vtxCnt, AFX_SIMD_ALIGN, AfxSpawnHint()))) AfxThrowError();
                else
                {
                    if (vtd->attrs[i].data)
                    {
                        AfxAssert(mshb->GetVertexData);
                        mshb->GetVertexData(data, i, 0, vtxCnt, vtd->attrs[i].data);
                    }
                }

                afxNat cacheIdx = vtd->attrs[i].cacheIdx;

                if (!cacheAttrCnt[cacheIdx])
                    ++cacheCnt;

                cacheAttrMap[cacheIdx][cacheAttrCnt[cacheIdx]] = i;
                ++cacheAttrCnt[cacheIdx];
            }
            vtd->attrCnt = attrCnt;
        }
    }

    vtd->caches = NIL;

    if (cacheCnt && !(vtd->caches = AfxAllocate(NIL, cacheCnt * sizeof(vtd->caches[0]), NIL, AfxSpawnHint()))) AfxThrowError();
    else
    {
        if (vtd->caches)
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

                if (!(cache->comps = AfxAllocate(NIL, cache->compCnt * sizeof(cache->comps[0]), NIL, AfxSpawnHint()))) AfxThrowError();
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

_AFX afxError AfxBuildVertexDatas(afxSimulation sim, afxMeshBuilder const* mshb, afxNat cnt, void *data[], afxVertexData vtd[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AfxAssert(mshb);

    if (AfxAcquireObjects(AfxGetMeshDataClass(sim), cnt, (afxHandle*)vtd, (void*[]) { sim, (void*)mshb, data, 0, 0 }))
        AfxThrowError();

    return err;
}

_AFX void AfxTransformVertexDatas(afxReal const lt[3][3], afxReal const ilt[3][3], afxReal const at[3], afxBool renormalize, afxNat cnt, afxVertexData vtd[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(at);
    AfxAssert(lt);
    AfxAssert(ilt);
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
                    AfxPostMultiplyArrayedV3d(lt, vtxCnt, data, data);

                    if (!deltaFlag)
                    {
                        if (fmt == afxVertexFormat_V4D)
                            for (afxNat k = 0; k < vtxCnt; k++)
                                AfxAddV4d(((afxV4d*)data)[k], ((afxV4d*)data)[k], at);
                        else if (fmt == afxVertexFormat_V3D)
                            for (afxNat k = 0; k < vtxCnt; k++)
                                AfxAddV3d(((afxV3d*)data)[k], ((afxV3d*)data)[k], at);
                    }
                }
                else if (usage & afxVertexUsage_TAN)
                {
                    AfxPostMultiplyArrayedV3d(lt, vtxCnt, data, data);
                }
                else if (usage & afxVertexUsage_BIT)
                {
                    AfxPostMultiplyArrayedV3d(lt, vtxCnt, data, data);
                }
                

                if (usage & afxVertexUsage_TBC)
                {
                    if (deltaFlag)
                    {
                        AfxPostMultiplyArrayedV3d(lt, vtxCnt, data, data);
                    }
                    else
                    {
                        AfxPostMultiplyArrayedV3d(ilt, vtxCnt, data, data);
                    }
                }
                else if (usage & afxVertexUsage_NRM)
                {
                    if (deltaFlag)
                    {
                        AfxPostMultiplyArrayedV3d(lt, vtxCnt, data, data);
                    }
                    else
                    {
                        AfxPostMultiplyArrayedV3d(ilt, vtxCnt, data, data);
                    }
                }

                if (renormalize)
                {
                    if (fmt == afxVertexFormat_V4D)
                        AfxNormalizeOrZeroArrayedV4d(vtxCnt, data, data);
                    else if (fmt == afxVertexFormat_V3D)
                        AfxNormalizeOrZeroArrayedV3d(vtxCnt, data, data);
                }
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
                                AfxGetNormalizedV3d(((afxV3d*)data)[k], ((afxV3d*)data)[k]);

                        break;
                    }
                    case afxVertexFormat_V4D:
                    {
                        AfxThrowError(); // inoperant

                        if (linearFlag)
                            AfxPostMultiplyArrayedNormalV4d(lt, vtxCnt, data, data);

                        if (invLinearFlag)
                            AfxPostMultiplyArrayedNormalV4d(ilt, vtxCnt, data, data);

                        if (affineFlag)
                            for (afxNat k = 0; k < vtxCnt; k++)
                                AfxAddV3d(((afxV4d*)data)[k], ((afxV4d*)data)[k], at);

                        if (renormalize)
                            for (afxNat k = 0; k < vtxCnt; k++)
                                AfxGetNormalizedV4d(((afxV4d*)data)[k], ((afxV4d*)data)[k]);

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
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxVertexData)),
    .ctx = NIL,
    .ctor = (void*)_AfxVtdCtor,
    .dtor = (void*)_AfxVtdDtor
};
