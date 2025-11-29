/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

 // This software is part of Advanced Video Graphics Extensions & Experiments.

#define _AVX_DRAW_C
#define _AVX_VERTEX_INPUT_C
#include "avxIcd.h"

_AVX afxDrawSystem AvxGetVertexInputHost(avxVertexInput vin)
{
    afxError err = { 0 };
    // @vin must be a valid avxVertexInput handle.
    AFX_ASSERT_OBJECTS(afxFcc_VIN, 1, &vin);
    afxDrawSystem dsys = AfxGetHost(vin);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    return dsys;
}

_AVX afxFlags AvxGetVertexInputFlags(avxVertexInput vin, afxFlags mask)
{
    afxError err = { 0 };
    // @vin must be a valid avxVertexInput handle.
    AFX_ASSERT_OBJECTS(afxFcc_VIN, 1, &vin);
    return (!mask) ? vin->flags : (vin->flags & mask);
}

_AVX afxUnit AvxDescribeVertexLayout(avxVertexInput vin, avxVertexLayout* layout)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_VIN, 1, &vin);
    AFX_ASSERT(layout);

    layout->tag = vin->tag;
    layout->binCnt = vin->binCnt;
    //layout->attrCnt = vin->attrCnt;

    for (afxUnit i = 0; i < vin->totalAttrCnt; i++)
    {
        avxVertexAttr const* va = &vin->attrs[i];
        layout->attrs[i].fmt = va->fmt;
        layout->attrs[i].location = va->location;
        layout->attrs[i].offset = va->offset;
    }

    for (afxUnit i = 0; i < vin->binCnt; i++)
    {
        avxVertexStream const* vs = &vin->bins[i];
        layout->bins[i].pin = vs->pin;
        layout->bins[i].minStride = vs->minStride;
        layout->bins[i].instRate = vs->instRate;
        layout->bins[i].baseAttrIdx = vs->baseAttrIdx;
        layout->bins[i].attrCnt = vs->attrCnt;
    }
    return vin->totalAttrCnt;
}

/*
    The AvxQueryVertexStride() function computes the total size in bytes of all vertex attributes per source (stream), 
    starting from @baseSrcIdx, and sums their sizes into a single value (totalSiz) returned. The idea is to find out the aggregate 
    stride across multiple vertex buffers.
*/

_AVX afxUnit AvxQueryVertexStride(avxVertexInput vin, afxUnit baseSrcIdx, afxUnit cnt, afxUnit vtxSrcSiz[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_VIN, 1, &vin);

    baseSrcIdx = AFX_MIN(baseSrcIdx, vin->binCnt - 1);
    cnt = AFX_MIN(cnt, vin->binCnt - baseSrcIdx);

    afxUnit totalSiz = 0;
    for (afxUnit i = 0; i < cnt; i++)
    {
        afxUnit pin = baseSrcIdx + i;
        avxVertexStream const* vf = &vin->bins[pin];
        afxUnit siz = 0;

        for (afxUnit j = 0; j < vf->attrCnt; j++)
        {
            afxUnit attrIdx = vf->baseAttrIdx + j;
            avxVertexAttr const* via = &vin->attrs[attrIdx];

            avxFormatDescription pfd;
            AvxDescribeFormats(1, &via->fmt, &pfd);
            siz += pfd.stride;
        }

        if (vtxSrcSiz) vtxSrcSiz[i] = siz;

        totalSiz += siz;
    }
    return totalSiz;
}

void AvxFetchVertices
(
    avxVertexInput vin, 
    avxBufferedStream const streams[],
    afxUnit baseInst,
    afxUnit instCnt,
    afxUnit baseVtx,
    afxUnit vtxCnt,
    avxVertexData* outData
)
{
    afxError err = { 0 };
    afxUnit dstIdx = 0;
    afxUnit binCnt = vin->binCnt;
    avxVertexStream const* bins = vin->bins;
    avxVertexAttr const* attrs = vin->attrs;

    afxByte const* bufPtr[AVX_MAX_VERTEX_SOURCES] = { 0 };
    afxUnit srcStride[AVX_MAX_VERTEX_SOURCES] = { 0 };
    afxUnit srcAttrCnt[AVX_MAX_VERTEX_SOURCES] = { 0 };
    afxUnit srcBaseAttr[AVX_MAX_VERTEX_SOURCES] = { 0 };
    afxUnit srcInstRate[AVX_MAX_VERTEX_SOURCES] = { 0 };

    for (afxUnit i = 0; i < binCnt; ++i)
    {
        avxVertexStream const* fetch = &bins[i];
        avxBufferedStream const* stream = &streams[fetch->pin];
        avxBuffer buf = stream->buf;
        if (!buf) continue;
        AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
        afxSize bufCap = AvxGetBufferCapacity(buf, 0);
        AFX_ASSERT_RANGE(bufCap, stream->offset, stream->range);
        bufPtr[i] = (afxByte const*)AvxGetBufferAddress(buf, stream->offset);
        srcStride[i] = stream->stride;
        srcAttrCnt[i] = fetch->attrCnt;
        srcBaseAttr[i] = fetch->baseAttrIdx;
        srcInstRate[i] = fetch->instRate;
    }

    for (afxUnit inst = 0; inst < instCnt; ++inst)
    {
        afxUnit instanceIndex = baseInst + inst;

        // Pre-fetch all per-instance data.
        for (afxUnit i = 0; i < binCnt; ++i)
        {
            if (srcInstRate[i] == 0) continue; // skip per-vertex

            afxUnit elemIdx = instanceIndex / srcInstRate[i];
            afxByte const* basePtr = bufPtr[i] + elemIdx * srcStride[i];

            for (afxUnit j = 0; j < srcAttrCnt[i]; ++j)
            {
                afxUnit attrIdx = srcBaseAttr[i] + j;
                avxVertexAttr const* attr = &attrs[attrIdx];
                void const* attrPtr = basePtr + attr->offset;

                // Copy the same data into all vertices of this instance.
                for (afxUnit vtx = 0; vtx < vtxCnt; ++vtx)
                {
                    avxVertexData* dst = &outData[dstIdx + vtx];
                    AvxTranscodeFormat(attr->fmt, attr->fmt, 1, attrPtr, dst, 0, 0);
                    //_AvxDecodeAttribute(attrPtr, attr->fmt, dst, attr->location);
                }
            }
        }

        // Fetch per-vertex data.
        for (afxUnit vtx = 0; vtx < vtxCnt; ++vtx)
        {
            afxUnit vertexIndex = baseVtx + vtx;
            avxVertexData* dst = &outData[dstIdx++];

            for (afxUnit i = 0; i < binCnt; ++i)
            {
                if (srcInstRate[i] > 0) continue; // already handled

                afxUnit elemIdx = vertexIndex;

                afxByte const* basePtr = bufPtr[i] + elemIdx * srcStride[i];

                for (afxUnit j = 0; j < srcAttrCnt[i]; ++j)
                {
                    afxUnit attrIdx = srcBaseAttr[i] + j;
                    avxVertexAttr const* attr = &attrs[attrIdx];
                    void const* attrPtr = basePtr + attr->offset;
                    AvxTranscodeFormat(attr->fmt, attr->fmt, 1, attrPtr, dst, 0, 0);
                    //_AvxDecodeAttribute(attrPtr, attr->fmt, dst, attr->location);
                }
            }
        }
    }
}

void AvxFetchIndexedVertices
(
    avxVertexInput vin,
    avxBufferedStream const streams[],
    avxBufferedStream const* ibo,
    afxUnit baseInst,
    afxUnit instCnt,
    afxUnit baseIdx,
    afxUnit idxCnt,
    afxInt vtxOffset,
    avxVertexData* outData
)
{
    afxError err = { 0 };
    afxUnit dstIdx = 0;
    afxUnit binCnt = vin->binCnt;
    avxVertexStream const* bins = vin->bins;
    avxVertexAttr const* attrs = vin->attrs;

    afxByte const* bufPtr[AVX_MAX_VERTEX_SOURCES] = { 0 };
    afxUnit srcStride[AVX_MAX_VERTEX_SOURCES] = { 0 };
    afxUnit srcAttrCnt[AVX_MAX_VERTEX_SOURCES] = { 0 };
    afxUnit srcBaseAttr[AVX_MAX_VERTEX_SOURCES] = { 0 };
    afxUnit srcInstRate[AVX_MAX_VERTEX_SOURCES] = { 0 };

    for (afxUnit i = 0; i < binCnt; ++i)
    {
        avxVertexStream const* fetch = &bins[i];
        avxBufferedStream const* stream = &streams[fetch->pin];
        avxBuffer buf = stream->buf;
        if (!buf) continue;
        AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
        afxSize bufCap = AvxGetBufferCapacity(buf, 0);
        AFX_ASSERT_RANGE(bufCap, stream->offset, stream->range);
        bufPtr[i] = (afxByte const*)AvxGetBufferAddress(buf, stream->offset);
        srcStride[i] = stream->stride;
        srcAttrCnt[i] = fetch->attrCnt;
        srcBaseAttr[i] = fetch->baseAttrIdx;
        srcInstRate[i] = fetch->instRate;
    }

    avxBuffer ibuf = ibo->buf;
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &ibuf);
    afxSize ibufCap = AvxGetBufferCapacity(ibuf, 0);
    AFX_ASSERT_RANGE(ibufCap, ibo->offset, ibo->range);
    afxByte const* ibufPtr = (afxByte const*)AvxGetBufferAddress(ibuf, ibo->offset);
    afxUnit idxStride = ibo->stride;
    
    for (afxUnit inst = 0; inst < instCnt; ++inst)
    {
        afxUnit instId = baseInst + inst;

        // Pre-fetch all instanced data (for current instance).
        for (afxUnit s = 0; s < binCnt; ++s)
        {
            if (srcInstRate[s] == 0) continue;

            afxUnit elemIdx = instId / srcInstRate[s];
            afxByte const* basePtr = bufPtr[s] + elemIdx * srcStride[s];

            for (afxUnit j = 0; j < srcAttrCnt[s]; ++j)
            {
                afxUnit attrIdx = srcBaseAttr[s] + j;
                avxVertexAttr const* attr = &attrs[attrIdx];
                void const* attrPtr = basePtr + attr->offset;

                // Apply to all indexed vertices in this instance.
                for (afxUnit idx = 0; idx < idxCnt; ++idx)
                {
                    avxVertexData* dst = &outData[dstIdx + idx];
                    AvxTranscodeFormat(attr->fmt, attr->fmt, 1, attrPtr, dst, 0, 0);
                    //_AvxDecodeAttribute(attrPtr, attr->fmt, dst, attr->location);
                }
            }
        }

        if (idxStride == 2)
        {
            // Fetch per-indexed vertex.
            for (afxUnit idx = 0; idx < idxCnt; ++idx)
            {
                afxUnit indexOffset = baseIdx + idx;
                afxUnit16 const* indexBase = (afxUnit16 const*)(ibufPtr + indexOffset * ibo->stride);
                afxUnit vertexIndex = ((afxUnit16 const*)indexBase)[0];
                vertexIndex += vtxOffset;

                avxVertexData* dst = &outData[dstIdx++];

                for (afxUnit s = 0; s < binCnt; ++s)
                {
                    if (srcInstRate[s] > 0) continue; // already handled

                    uint8_t const* basePtr = bufPtr[s] + vertexIndex * srcStride[s];

                    for (afxUnit j = 0; j < srcAttrCnt[s]; ++j)
                    {
                        afxUnit attrIdx = srcBaseAttr[s] + j;
                        avxVertexAttr const* attr = &attrs[attrIdx];
                        void const* attrPtr = basePtr + attr->offset;

                        AvxTranscodeFormat(attr->fmt, attr->fmt, 1, attrPtr, dst, 0, 0);
                        //_AvxDecodeAttribute(attrPtr, attr->fmt, dst, attr->location);
                    }
                }
            }
        }
        else if (idxStride == 4)
        {
            // Fetch per-indexed vertex.
            for (afxUnit idx = 0; idx < idxCnt; ++idx)
            {
                afxUnit indexOffset = baseIdx + idx;
                afxUnit32 const* indexBase = (afxUnit32 const*)(ibufPtr + indexOffset * idxStride);
                afxUnit vertexIndex = ((afxUnit32 const*)indexBase)[0];
                vertexIndex += vtxOffset;

                avxVertexData* dst = &outData[dstIdx++];

                for (afxUnit s = 0; s < binCnt; ++s)
                {
                    if (srcInstRate[s] > 0) continue; // already handled

                    afxByte const* basePtr = bufPtr[s] + vertexIndex * srcStride[s];

                    for (afxUnit j = 0; j < srcAttrCnt[s]; ++j)
                    {
                        afxUnit attrIdx = srcBaseAttr[s] + j;
                        avxVertexAttr const* attr = &attrs[attrIdx];
                        void const* attrPtr = basePtr + attr->offset;

                        AvxTranscodeFormat(attr->fmt, attr->fmt, 1, attrPtr, dst, 0, 0);
                        //_AvxDecodeAttribute(attrPtr, attr->fmt, dst, attr->location);
                    }
                }
            }
        }
    }
}

_AVX afxError _AvxVtxdDtorCb(avxVertexInput vin)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_VIN, 1, &vin);
    //afxDrawSystem dsys = AfxGetHost(vin);

    afxObjectStash const stashes[] =
    {
        {
            .cnt = vin->totalAttrCnt,
            .siz = sizeof(vin->attrs[0]),
            .var = (void**)&vin->attrs
        },
        {
            .cnt = vin->binCnt,
            .siz = sizeof(vin->bins[0]),
            .var = (void**)&vin->bins
        }
    };

    if (AfxDeallocateInstanceData(vin, ARRAY_SIZE(stashes), stashes))
    {
        AfxThrowError();
        return err;
    }

    return err;
}

_AVX afxError _AvxVtxdCtorCb(avxVertexInput vin, void** args, afxUnit invokeNo)
{
    (void)invokeNo;
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_VIN, 1, &vin);

    afxDrawSystem dsys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    avxVertexLayout const* layout = AFX_CAST(avxVertexLayout const*, args[1]) + invokeNo;
    AFX_ASSERT(args[1]);

    vin->tag = layout->tag;
    vin->udd = layout->udd;
    vin->flags = layout->flags;

    afxBool attrUsed[AVX_MAX_VERTEX_ATTRIBS] = { FALSE };
    afxUnit highestUsedAttrIdx = 0;

    for (afxUnit i = 0; i < layout->binCnt; ++i)
    {
        avxVertexStream const* fetch = &layout->bins[i];

        // Skip unused or empty bindings.
        if (fetch->attrCnt == 0)
            continue;

        // Validate attr bounds.
        afxUnit start = fetch->baseAttrIdx;
        afxUnit end = start + fetch->attrCnt;

        if (start >= AVX_MAX_VERTEX_ATTRIBS || end > AVX_MAX_VERTEX_ATTRIBS)
        {
            // Out of bounds.
            AfxThrowError();
            return err;
        }

        // Check for ownership overlaps.
        for (afxUnit j = start; j < end; ++j)
        {
            if (attrUsed[j])
            {
                // Attribute already owned by another stream.
                AfxThrowError();
                return err;
            }
            attrUsed[j] = TRUE;

            // Track highest used attr index.
            if (j + 1 > highestUsedAttrIdx)
                highestUsedAttrIdx = j + 1;
        }

        // Optionally clamp or clean zeroed-out attribute entries if desired.
        // Or adjust attrCnt if some trailing entries are clearly invalid.
        // This depends on whether you want to sanitize silently or return an error.
    }

    afxUnit totalAttrCnt = 0;
    totalAttrCnt = highestUsedAttrIdx;
#if 0
#ifdef AVX_VALIDATION_ENABLED

    if (layout->binCnt)
    {
        AFX_ASSERT(layout->bins);

        for (afxUnit i = 0; i < layout->binCnt; i++)
        {
            avxVertexStream const* vs = &layout->bins[i];

            for (afxUnit j = 0; j < i; j++)
            {
                avxVertexStream const* vs2 = &layout->bins[j];
                AFX_ASSERT(vs->pin != vs2->pin);

                if (vs->pin == vs2->pin)
                    AfxThrowError();

                for (afxUnit k = 0; k < vs->attrCnt; k++)
                {
                    afxUnit attrIdx = vs->baseAttrIdx + k;
                    avxVertexAttr const* va = &layout->attrs[attrIdx];

                    for (afxUnit l = 0; l < k; l++)
                    {
                        afxUnit prevAttrIdx = vs2->baseAttrIdx + l;
                        avxVertexAttr const* va2 = &layout->attrs[prevAttrIdx];
                        AFX_ASSERT(va->location != va2->location);

                        if (va->location == va2->location)
                            AfxThrowError();
                        else
                            ++totalAttrCnt;
                    }
                }
            }
        }
    }

    if (totalAttrCnt)
    {
        AFX_ASSERT(layout->attrs);

        for (afxUnit i = 0; i < totalAttrCnt; i++)
        {
            avxVertexAttr const* va = &layout->attrs[i];

            for (afxUnit j = 0; j < i; j++)
            {
                avxVertexAttr const* va2 = &layout->attrs[j];
                AFX_ASSERT(va->location != va2->location);

                if (va->location == va2->location)
                    AfxThrowError();
            }
        }
    }
#endif
#endif

    if (err)
        return err;

    afxObjectStash const stashes[] =
    {
        {
            .cnt = totalAttrCnt,
            .siz = sizeof(vin->attrs[0]),
            .var = (void**)&vin->attrs
        },
        {
            .cnt = layout->binCnt,
            .siz = sizeof(vin->bins[0]),
            .var = (void**)&vin->bins
        }
    };

    if (AfxAllocateInstanceData(vin, ARRAY_SIZE(stashes), stashes))
    {
        AfxThrowError();
        return err;
    }

    for (afxUnit i = 0; i < totalAttrCnt; i++)
    {
        avxVertexAttr* va = &vin->attrs[i];
        va->fmt = layout->attrs[i].fmt;
        va->location = layout->attrs[i].location;
        va->offset = layout->attrs[i].offset;
        //va->pin = layout->attrs[i].pin;
        //va->normalized = !!layout->attrs[i].normalized;
    }

    for (afxUnit i = 0; i < layout->binCnt; i++)
    {
        avxVertexStream* vs = &vin->bins[i];
        vs->pin = layout->bins[i].pin;
        vs->minStride = layout->bins[i].minStride;
        vs->instRate = layout->bins[i].instRate;
        vs->baseAttrIdx = layout->bins[i].baseAttrIdx;
        vs->attrCnt = layout->bins[i].attrCnt;
    }

    vin->totalAttrCnt = totalAttrCnt;
    vin->binCnt = layout->binCnt;

    if (err && AfxAllocateInstanceData(vin, ARRAY_SIZE(stashes), stashes))
        AfxThrowError();

    return err;
}

_AVX afxClassConfig const _AVX_CLASS_CONFIG_VIN =
{
    .fcc = afxFcc_VIN,
    .name = "VertexInput",
    .desc = "Pipelined Vertex Input Declaration",
    .fixedSiz = sizeof(AFX_OBJECT(avxVertexInput)),
    .ctor = (void*)_AvxVtxdCtorCb,
    .dtor = (void*)_AvxVtxdDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AvxAcquireVertexInputs(afxDrawSystem dsys, afxUnit cnt, avxVertexLayout const layouts[], avxVertexInput declarations[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(declarations);
    AFX_ASSERT(layouts);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)_AvxDsysGetDdi(dsys)->vtxdCls(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_VIN);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)declarations, (void const*[]) { dsys, layouts }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_VIN, cnt, declarations);

#if AVX_VALIDATION_ENABLED
    for (afxUnit i = 0; i < cnt; i++)
    {
        avxVertexInput vtxd = declarations[i];

        //AFX_ASSERT(vtxd->totalAttrCnt == layouts[i].totalAttrCnt);
        AFX_ASSERT(vtxd->binCnt == layouts[i].binCnt);

        for (afxUnit j = 0; j < vtxd->totalAttrCnt; j++)
        {
            AFX_ASSERT(vtxd->attrs[j].fmt == layouts[i].attrs[j].fmt);
            AFX_ASSERT(vtxd->attrs[j].location == layouts[i].attrs[j].location);
            AFX_ASSERT(vtxd->attrs[j].offset == layouts[i].attrs[j].offset);
            //AFX_ASSERT(vtxd->attrs[j].pin == layouts[i].attrs[j].pin);
        }

        for (afxUnit j = 0; j < vtxd->binCnt; j++)
        {
            AFX_ASSERT(vtxd->bins[j].pin == layouts[i].bins[j].pin);
            AFX_ASSERT(vtxd->bins[j].instRate == layouts[i].bins[j].instRate);
            //AFX_ASSERT(vtxd->bins[j].stride == layouts[i].bins[j].stride);
            AFX_ASSERT(vtxd->bins[j].baseAttrIdx == layouts[i].bins[j].baseAttrIdx);
            AFX_ASSERT(vtxd->bins[j].attrCnt == layouts[i].bins[j].attrCnt);
        }
    }
#endif
    return err;
}
