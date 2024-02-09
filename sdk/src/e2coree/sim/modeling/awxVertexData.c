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
#define _AFX_VERTEX_DATA_C
#define _AFX_SIMULATION_C

#define _AFX_DRAW_C
#define _AFX_VERTEX_BUFFER_C

#include "qwadro/sim/modeling/awxVertexData.h"
#include "qwadro/sim/afxSimulation.h"
#include "qwadro/math/afxMatrix.h"
#include "qwadro/draw/pipe/afxDrawCommands.h"
#include "qwadro/core/afxIndexedString.h"

_AFX afxError AwxCmdBindVertexDataCache(afxDrawScript dscr, afxNat slotIdx, awxVertexData vtd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);

    for (afxNat i = 0; i < 2; i++)
    {
        awxVertexDataCache const* cache = &vtd->cache[i];
        afxVertexBuffer vbuf = AfxGetLinker(&cache->vbuf);

        if (vbuf)
        {
            AfxCmdBindVertexSources(dscr, i, 1, (afxBuffer[]) { AfxGetVertexBufferStorage(vbuf) }, &cache->base, &cache->range, &cache->stride);
        }
    }
    return err;
}

_AFXINL void packVdCb(awxVertexDataCache* cache, awxVertexData vtd)
{

}

_AFX afxError AwxBufferizeVertexData(afxDrawInput din, awxVertexData vtd)
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
        awxVertexDataAttr* attr = &vtd->attrs[i];
        afxVertexFormat fmt = attr->fmt;
        AfxAssert(fmt < afxVertexFormat_TOTAL);
        afxVertexFormat cachedFmt = NIL;
        afxNat cacheIdx;

        if (AFX_INVALID_INDEX != AfxStringsAreEqual(&attr->id.str.str, 3, positionals))
        {
            if (0 == AfxTestStringEquality(&attr->id.str.str, &positionals[0]))
            {
                cachedFmt = afxVertexFormat_V3D;
            }
            else if (0 == AfxTestStringEquality(&attr->id.str.str, &positionals[1]))
            {
                cachedFmt = afxVertexFormat_REAL;
            }
            else if (0 == AfxTestStringEquality(&attr->id.str.str, &positionals[2]))
            {
                cachedFmt = afxVertexFormat_REAL;
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
            awxVertexDataCache* cache = &vtd->cache[i];

            AfxPushLinkage(&cache->vbuf, &vbo->rooms);
            cache->base = i ? vtd->cache[i - 1].range : 0;
            cache->range = AFX_ALIGN(cacheSiz[i] * vtd->vtxCnt, 64);
            cache->stride = cacheSiz[i];
        }
    }

    for (afxNat j = 0; j < vtd->attrCnt; j++)
    {
        awxVertexDataAttr* attr = &vtd->attrs[j];
        AfxAssert(attr->data);
        afxVertexFormat fmt = attr->fmt;
        AfxAssert(fmt < afxVertexFormat_TOTAL);

        awxVertexDataCache* cache = &vtd->cache[attr->cacheIdx];
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
        if (0 == AfxTestStringEquality(&id[rslt], &vtd->attrs[i].id.str.str))
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

_AFX afxError AwxGetVertexAttributeInfo(awxVertexData vtd, afxNat attrIdx, afxVertexFormat* fmt, awxVertexUsage* usage, awxVertexFlags* flags)
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

_AFX afxNat AwxCountVertices(awxVertexData vtd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    return vtd->vtxCnt;
}

_AFX afxNat AwxCountVertexBiases(awxVertexData vtd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    return vtd->biasCnt;
}

_AFX afxVertexBias const* AwxGetVertexBiases(awxVertexData vtd, afxNat baseBiasIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssertRange(vtd->biasCnt, baseBiasIdx, 1);
    return &vtd->biases[baseBiasIdx];
}

_AFX afxError AwxUpdateVertexData(awxVertexData vtd, afxNat attrIdx, afxNat baseVtx, afxNat vtxCnt, void const* src, afxNat32 srcStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssertRange(vtd->attrCnt, attrIdx, 1);
    AfxAssertRange(vtd->vtxCnt, baseVtx, vtxCnt);
    AfxAssert(src);
    AfxAssert(srcStride);
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

    if (!vtd->attrs[attrIdx].data)
        if (!(vtd->attrs[attrIdx].data = AfxAllocate(NIL, vtd->vtxCnt, unitSiz, AFX_SIMD_ALIGN, AfxHint())))
            AfxThrowError();

    if (!err)
    {
        AfxStream(vtxCnt, srcStride, unitSiz, src, vtd->attrs[attrIdx].data);
    }
    return err;
}

_AFX void* AwxExposeVertexData(awxVertexData vtd, afxNat attrIdx, afxNat baseVtx)
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

_AFX afxError AwxFillVertexData(awxVertexData vtd, afxNat attrIdx, afxNat baseVtx, afxNat vtxCnt, void const* src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssertRange(vtd->attrCnt, attrIdx, 1);
    AfxAssertRange(vtd->vtxCnt, baseVtx, vtxCnt);
    AfxAssert(src);

    afxVertexFormat fmt = vtd->attrs[attrIdx].fmt;
    afxNat32 unitSiz = AfxVertexFormatGetSize(fmt);

    if (!vtd->attrs[attrIdx].data)
        if (!(vtd->attrs[attrIdx].data = AfxAllocate(NIL, vtd->vtxCnt, unitSiz, AFX_SIMD_ALIGN, AfxHint())))
            AfxThrowError();

    if (!err)
    {
        AfxStream(vtxCnt, 0, unitSiz, src, vtd->attrs[attrIdx].data);
    }
    return err;
}

_AFX afxError AwxNormalizeVertexData(awxVertexData vtd, afxNat attrIdx, afxNat baseVtx, afxNat vtxCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssertRange(vtd->attrCnt, attrIdx, 1);
    AfxAssertRange(vtd->vtxCnt, baseVtx, vtxCnt);
    
    afxVertexFormat fmt = vtd->attrs[attrIdx].fmt;
    AfxAssert(fmt == afxVertexFormat_V2D || fmt == afxVertexFormat_V3D || fmt == afxVertexFormat_V4D);

    afxNat32 unitSiz = AfxVertexFormatGetSize(fmt);

    if (!vtd->attrs[attrIdx].data)
        if (!(vtd->attrs[attrIdx].data = AfxAllocate(NIL, vtd->vtxCnt, unitSiz, AFX_SIMD_ALIGN, AfxHint())))
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

_AFX afxError AwxZeroVertexData(awxVertexData vtd, afxNat attrIdx, afxNat baseVtx, afxNat vtxCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssertRange(vtd->attrCnt, attrIdx, 1);
    AfxAssertRange(vtd->vtxCnt, baseVtx, vtxCnt);
    
    if (AwxFillVertexData(vtd, attrIdx, baseVtx, vtxCnt, AFX_M4D_ZERO))
        AfxThrowError();

    return err;
}

_AFX afxError AwxUpdateVertexBiases(awxVertexData vtd, afxNat baseBiasIdx, afxNat biasCnt, afxVertexBias const src[], afxNat srcStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssertRange(vtd->biasCnt, baseBiasIdx, biasCnt);
    AfxAssert(srcStride);
    AfxAssert(src);

    afxByte const* src2 = (afxByte const*)src;
    afxVertexBias* biases = vtd->biases;

    for (afxNat i = 0; i < biasCnt; i++)
    {
        afxVertexBias const* in = (afxVertexBias const*)src2;;
        src2 += srcStride;

        biases[i] = *in;
        //biases[i].pivotIdx = in->pivotIdx;
        //biases[i].weight = in->weight;
        AfxAssert(1.0 >= biases[i].weight);
        //AfxAssertRange(pivotCnt, biases[i].pivotIdx, 1);
    }

    return err;
}

_AFX afxError AwxUpdateVertices(awxVertexData vtd, afxNat baseVtx, afxNat vtxCnt, afxVertex const src[], afxNat srcStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssertRange(vtd->vtxCnt, baseVtx, vtxCnt);
    AfxAssert(srcStride);
    AfxAssert(src);

    afxByte const* src2 = (afxByte const*)src;
    afxVertex* vertices = vtd->vtx;
    afxNat totalBiasCnt = vtd->biasCnt;

    for (afxNat i = 0; i < vtxCnt; i++)
    {
        afxVertex const* in = (afxVertex const*)src2;
        src2 += srcStride;

        vertices[i] = *in;
        //vertices[i].baseBiasIdx = in->baseBiasIdx;
        //vertices[i].biasCnt = in->biasCnt;
        AfxAssertRange(totalBiasCnt, vertices[i].baseBiasIdx, vertices[i].biasCnt);
    }
    return err;
}

_AFX afxError _AfxVtdDtor(awxVertexData vtd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxEntry("vtd=%p", vtd);

    afxSimulation sim = AfxGetObjectProvider(vtd);
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxMmu mmu = AfxGetSimulationMmu(sim);

    for (afxNat i = 0; i < vtd->attrCnt; i++)
    {
        if (vtd->attrs[i].data)
            AfxDeallocate(mmu, vtd->attrs[i].data);
    }

    if (vtd->attrs)
        AfxDeallocate(mmu, vtd->attrs);

    if (vtd->vtx)
        AfxDeallocate(mmu, vtd->vtx);

    if (vtd->biases)
        AfxDeallocate(mmu, vtd->biases);

    if (vtd->strc)
        AfxReleaseObjects(1, (void*[]) { vtd->strc });

    return err;
}

_AFX afxError _AfxVtdCtor(awxVertexData vtd, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxEntry("vtd=%p", vtd);

    afxSimulation sim = cookie->udd[0];
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxStringCatalog strc = cookie->udd[1];
    awxVertexAttrSpec const* attrSpecs = cookie->udd[2];
    awxVertexDataSpec const* spec = cookie->udd[3];
    spec += cookie->no;

    if ((vtd->strc = strc))
        AfxReacquireObjects(1, (void*[]) { strc });

    afxMmu mmu = AfxGetSimulationMmu(sim);

    afxNat totalBiasCnt = spec->biasCnt;
    afxNat vtxCnt = spec->vtxCnt;

    AfxAssert(totalBiasCnt); // at least one
    afxVertexBias* biases;

    if (!(biases = AfxAllocate(mmu, totalBiasCnt, sizeof(biases[0]), NIL, AfxHint()))) AfxThrowError();
    else
    {
        vtd->biasCnt = totalBiasCnt;
        vtd->biases = biases;

        if (spec->biasSrc)
            AwxUpdateVertexBiases(vtd, 0, totalBiasCnt, spec->biasSrc, spec->biasSrcStride);

        AfxAssert(vtxCnt); // at least one
        afxVertex* vertices;

        if (!(vertices = AfxAllocate(mmu, vtxCnt, sizeof(vertices[0]), NIL, AfxHint()))) AfxThrowError();
        else
        {
            vtd->vtxCnt = vtxCnt;
            vtd->vtx = vertices;

            if (spec->vtxSrc)
                AwxUpdateVertices(vtd, 0, vtxCnt, spec->vtxSrc, spec->vtxSrcStride);

            afxNat attrCnt = spec->attrCnt;
            awxVertexDataAttr* attrs;

            if (!(attrs = AfxAllocate(mmu, attrCnt, sizeof(attrs[0]), NIL, AfxHint()))) AfxThrowError();
            else
            {
                vtd->attrCnt = attrCnt;
                vtd->attrs = attrs;

                for (afxNat i = 0; i < attrCnt; i++)
                {
                    awxVertexAttrSpec const* attrSpec = &attrSpecs[i];

                    afxBool hasData = TRUE;

                    AfxMakeFixedString8(&attrs[i].id, 0);
                    afxString tmp;
                    AfxMakeString(&tmp, attrSpec->id, 0);
                    AfxCopyString(&attrs[i].id.str, &tmp);

                    if (!strc)
                        attrs[i].idStrIdx = AFX_INVALID_INDEX;
                    else if (AfxCatalogStrings(strc, 1, &attrs[i].id.str.str, &attrs[i].idStrIdx, 0))
                        AfxThrowError();

                    attrs[i].usage = attrSpec->usage;
                    attrs[i].flags = NIL;

                    if (attrs[i].usage & awxVertexUsage_POS)
                        attrs[i].flags |= awxVertexFlag_AFFINE | awxVertexFlag_LINEAR;

                    if (attrs[i].usage & awxVertexUsage_POS)
                        attrs[i].flags |= awxVertexFlag_AFFINE | awxVertexFlag_LINEAR;

                    attrs[i].flags |= attrSpec->flags;
                    AfxAssert(attrSpec->fmt < afxVertexFormat_TOTAL);
                    attrs[i].fmt = attrSpec->fmt;
                    attrs[i].data = NIL;

                    attrs[i].cachedFmt = NIL;
                    attrs[i].cachedOffset = 0;
                    attrs[i].cacheIdx = AFX_INVALID_INDEX;

                    afxSize vtxSiz = AfxVertexFormatGetSize(attrs[i].fmt);

                    if (!(attrs[i].data = AfxAllocate(mmu, vtxCnt, vtxSiz, AFX_SIMD_ALIGN, AfxHint()))) AfxThrowError();
                    else
                    {
                        if (attrSpec->src)
                            AwxUpdateVertexData(vtd, i, 0, vtxCnt, attrSpec->src, attrSpec->srcStride);
                    }
                }


            }
        }
    }
    return err;
}

_AFX afxClassConfig _AfxVtdClsConfig =
{
    .fcc = afxFcc_VTD,
    .name = "Vertex Data",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(awxVertexData)),
    .mmu = NIL,
    .ctor = (void*)_AfxVtdCtor,
    .dtor = (void*)_AfxVtdDtor
};

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

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

            afxNat baseVtx = 0;
            afxNat vtxCnt = vtd2->vtxCnt;

            for (afxNat j = 0; j < vtd2->attrCnt; j++)
            {
                void* data = AwxExposeVertexData(vtd2, j, baseVtx);
                awxVertexFlags flags = AwxGetVertexAttributeFlags(vtd2, j);
                afxBool linearFlag = flags & awxVertexFlag_LINEAR;
                afxBool invLinearFlag = flags & awxVertexFlag_LINEAR_INV;
                afxBool affineFlag = flags & awxVertexFlag_AFFINE;
                afxBool deltaFlag = flags & awxVertexFlag_DELTA;
                afxBool normalized = flags & awxVertexFlag_NORMALIZED;
                awxVertexUsage usage = AwxGetVertexAttributeUsage(vtd2, j);
                afxVertexFormat fmt = AwxGetVertexAttributeFormat(vtd2, j);

                if (usage & awxVertexUsage_POS)
                {
                    if (fmt == afxVertexFormat_V4D)
                    {
                        AfxPostMultiplyArrayedLtv4(ltm, vtxCnt, data, data);

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
                }
#if !0
                else if (usage & awxVertexUsage_TAN)
                {
                    AfxPostMultiplyArrayedV3d(ltm, vtxCnt, data, data);

                    normalized = TRUE;
                }
                else if (usage & awxVertexUsage_BIT)
                {
                    AfxPostMultiplyArrayedV3d(ltm, vtxCnt, data, data);

                    normalized = TRUE;
                }                
                else if (usage & awxVertexUsage_TBC)
                {
                    if (deltaFlag)
                    {
                        AfxPostMultiplyArrayedV3d(ltm, vtxCnt, data, data);
                    }
                    else
                    {
                        AfxPostMultiplyArrayedV3d(iltm, vtxCnt, data, data);
                    }

                    normalized = TRUE;
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

                    normalized = TRUE;
                }

                if (renormalize && normalized)
                {
                    if (fmt == afxVertexFormat_V3D)
                        AfxNormalizeOrZeroArrayedV3d(vtxCnt, data, data);
                    else if (fmt == afxVertexFormat_V4D)
                        AfxNormalizeOrZeroArrayedV4d(vtxCnt, data, data);
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

_AFX afxError AwxAcquireVertexDatas(afxSimulation sim, afxStringCatalog strc, awxVertexAttrSpec const attrSpec[], afxNat cnt, awxVertexDataSpec const specs[], awxVertexData datas[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AfxAssert(datas);
    AfxAssert(specs);
    AfxAssert(cnt);

    if (AfxAcquireObjects(AwxGetMeshDataClass(sim), cnt, (afxObject*)datas, (void const*[]) { sim, strc, attrSpec, specs }))
        AfxThrowError();

    return err;
}

_AFX awxVertexData AwxBuildVertexData(afxSimulation sim, afxStringCatalog strc, afxMeshBuilder const* mshb)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AfxAssert(mshb);
    awxVertexData vtd = NIL;

    awxVertexDataSpec spec;
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

    awxVertexAttrSpec attrSpecs[16];
    
    attrSpecs[0].id = "pos";
    attrSpecs[0].flags = awxVertexFlag_AFFINE | awxVertexFlag_LINEAR;
    attrSpecs[0].fmt = afxVertexFormat_V4D;
    attrSpecs[0].usage = awxVertexUsage_POS | awxVertexUsage_POSITIONAL;
    attrSpecs[0].src = mshb->posn;
    attrSpecs[0].srcStride = sizeof(mshb->posn[0]);

    attrSpecs[1].id = "nrm";
    attrSpecs[1].flags = awxVertexFlag_LINEAR;
    attrSpecs[1].fmt = afxVertexFormat_V3D;
    attrSpecs[1].usage = awxVertexUsage_NRM | awxVertexUsage_TANGENT;
    attrSpecs[1].src = mshb->nrm;
    attrSpecs[1].srcStride = sizeof(mshb->nrm[0]);

    attrSpecs[2].id = "uv";
    attrSpecs[2].flags = NIL;
    attrSpecs[2].fmt = afxVertexFormat_V2D;
    attrSpecs[2].usage = awxVertexUsage_UV;
    attrSpecs[2].src = mshb->uv;
    attrSpecs[2].srcStride = sizeof(mshb->uv[0]);

    if (AwxAcquireVertexDatas(sim, strc, attrSpecs, 1, &spec, &vtd))
        AfxThrowError();

    return vtd;
}
