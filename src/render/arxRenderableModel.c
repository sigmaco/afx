/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *          Q W A D R O   4 D   R E N D E R I N G   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _ARX_MESH_C
#define _ARX_SCENE_C
#define _ARX_MODEL_C
#define _ARX_TERRAIN_C
#define _ARX_RENDER_CONTEXT_C
#include "ddi/arxImpl_Input.h"
#include "../draw/ddi/avxImplementation.h"
#include "qwadro/render/arxScene.h"

AFX_DEFINE_STRUCT(avxIndexBuffer)
{
    afxChain    rows;
    avxBuffer   buf;
    afxChain    rooms;
};

AFX_DEFINE_STRUCT(freeVBlistEntry)
{
    freeVBlistEntry*        next;
    freeVBlistEntry*        prev;
    avxBuffer               vbo;
    afxUnit32                offset;
    afxUnit32                size;
};

AFX_DEFINE_STRUCT(createdVBlistEntry)
{
    createdVBlistEntry*     next;
    avxBuffer               vbo;
};

AFX_OBJECT(arxBufferizer)
{
    arxRenderware        rwe;
    afxUnit32           DefaultVBSize;

    freeVBlistEntry*    freelist;
    createdVBlistEntry* vblist;
    afxSlabAllocator    FreeVBFreeList;
    afxSlabAllocator    CreatedVBFreeList;
};

#if !0
afxBool ArxBufferizeIndices(arxBufferizer vbMgr, afxUnit32 stride, afxUnit32 size, arxIndexCache* cache)
{
    // based on RwD3D9CreateVertexBuffer

    afxError err;
    AFX_ASSERT(stride <= size);
    AFX_ASSERT((stride & 0x3) == 0);
    AFX_ASSERT(cache);
    
    freeVBlistEntry* freelist = vbMgr->freelist;

    while (freelist) // Search for an available free space
    {
        if (freelist->size >= size)
            break;

        freelist = freelist->next;
    }

    if (!freelist)
    {
        // If there is not free space, create a new VB
        freelist = AfxPushSlabUnit(&vbMgr->FreeVBFreeList);
        AFX_ASSERT(freelist);
        freelist->offset = 0;
        freelist->size = (((vbMgr->DefaultVBSize + (stride - 1)) / stride) * stride);

        if (size >= freelist->size)
            freelist->size = size;
        
        afxDrawSystem dsys = ArxGetRenderwareDrawSystem(vbMgr->rwe);

        avxBuffer vbo;
        avxBufferInfo spec = { 0 };
        spec.size = freelist->size;
        spec.flags = avxBufferFlag_W;
        spec.usage = avxBufferUsage_INDEX;

        if (AvxAcquireBuffers(dsys, 1, &spec, &vbo))
        {
            AfxPopSlabUnit(&vbMgr->FreeVBFreeList, freelist);            
            return FALSE;
        }
        
        freelist->vbo = vbo;
        freelist->next = vbMgr->freelist; // Add it to the begining
        freelist->prev = NULL;

        if (vbMgr->freelist)
            vbMgr->freelist->prev = freelist;

        vbMgr->freelist = freelist;

        // Add an entry to the vb list
        createdVBlistEntry* vblist = AfxPushSlabUnit(&vbMgr->CreatedVBFreeList);
        vblist->vbo = vbo;
        vblist->next = vbMgr->vblist;
        vbMgr->vblist = vblist;

#if defined(RWDEBUG)
        VBCreated++;
#endif
    }

    cache->buf = freelist->vbo;
    cache->base = freelist->offset;
    afxUnit32 nextOffset = (cache->base) + size;

    AFX_ASSERT(freelist->size >= (nextOffset - freelist->offset));
    freelist->size -= nextOffset - freelist->offset;
    freelist->offset = nextOffset;

    if (freelist->size <= 0)
    {
        if (freelist->prev)
            freelist->prev->next = freelist->next;

        if (freelist->next)
            freelist->next->prev = freelist->prev;

        if (vbMgr->freelist == freelist)
            vbMgr->freelist = freelist->next;

        AfxPopSlabUnit(&vbMgr->FreeVBFreeList, freelist);
    }

#if defined(RWDEBUG)
    BlocksCreated++;
    BytesReserved += size;
#endif

    return TRUE;
}

void DestroyIndexBuffer(arxBufferizer vbMgr, afxUnit32 stride, afxUnit32 size, avxBuffer vbo, afxUnit32 offset)
{
    // based on RwD3D9DestroyVertexBuffer

    afxError err;
    AFX_ASSERT(stride <= size);
    AFX_ASSERT((stride & 0x3) == 0);
    AFX_ASSERT(vbo != NULL);

    // Search for an available free space to add
    freeVBlistEntry* freelist = vbMgr->freelist;

    while (freelist)
    {
        freeVBlistEntry* nextFreelist = freelist->next;

        if (freelist->vbo == vbo)
        {
            // Could be added to the end of the free space?

            if ((freelist->offset + freelist->size) == offset) break;
            else
            {
                // Could be added to the begin of the free space?

                if (offset + size == freelist->offset)
                    break;
            }
        }
        freelist = nextFreelist;
    }

    if (freelist)
    {
        afxBool canCompactMore = FALSE;
        afxUnit32 newoffset = offset;
        afxUnit32 newSize = size;
        freeVBlistEntry* oldFreelist = freelist;

        do
        {
            // Could be added to the end of the free space?

            if ((freelist->offset + freelist->size) == newoffset) freelist->size += newSize;
            else
            {
                // Could be added to the begin of the free space?

                if (newoffset + newSize == freelist->offset)
                {
                    freelist->offset = newoffset;
                    freelist->size += newSize;
                }
            }

            // Could be compact more?
            newoffset = freelist->offset;
            newSize = freelist->size;
            oldFreelist = freelist;
            canCompactMore = FALSE;
            freelist = vbMgr->freelist;

            while (freelist)
            {
                freeVBlistEntry* nextFreelist = freelist->next;

                if (freelist->vbo == vbo)
                {
                    // Could be added to the end of the free space?

                    if ((freelist->offset + freelist->size) == newoffset)
                    {
                        canCompactMore = TRUE;
                        break;
                    }
                    else
                    {
                        // Could be added to the begin of the free space?

                        if (newoffset + newSize == freelist->offset)
                        {
                            canCompactMore = TRUE;
                            break;
                        }
                    }
                }
                freelist = nextFreelist;
            }

            // Remove old list

            if (canCompactMore)
            {
                if (oldFreelist->prev)
                    oldFreelist->prev->next = oldFreelist->next;

                if (oldFreelist->next)
                    oldFreelist->next->prev = oldFreelist->prev;

                if (vbMgr->freelist == oldFreelist)
                    vbMgr->freelist = oldFreelist->next;

                AfxPopSlabUnit(&vbMgr->FreeVBFreeList, oldFreelist);
            }
        } while (canCompactMore);
    }
    else
    {
        freelist = AfxPushSlabUnit(&vbMgr->FreeVBFreeList);
        freelist->offset = offset;
        freelist->size = size;
        freelist->vbo = vbo;

        // Add it to the begining
        freelist->next = vbMgr->freelist;
        freelist->prev = NULL;

        if (vbMgr->freelist)
            vbMgr->freelist->prev = freelist;

        vbMgr->freelist = freelist;
    }

#if defined(RWDEBUG)
    BlocksCreated--;
    BytesReserved -= size;
#endif
}

afxBool _IndexBufferManagerOpen(arxBufferizer vbMgr)
{
    afxError err = NIL;
    //avxVertexBufferizer* vbMgr = AfxAllocate(1, sizeof(vbMgr[0]), 0, AfxHere());
    //*vbMgrPtr = vbMgr;

    vbMgr->DefaultVBSize = 128 * 1024;
    AfxDeploySlabAllocator(&vbMgr->FreeVBFreeList, sizeof(freeVBlistEntry), 100);
    AfxDeploySlabAllocator(&vbMgr->CreatedVBFreeList, sizeof(createdVBlistEntry), 100);

    return TRUE;
}

void _IndexBufferManagerClose(arxBufferizer vbMgr)
{
    //avxVertexBufferizer* vbMgr = *vbMgrPtr;
#if defined(RWDEBUG)
    AFX_ASSERT(BlocksCreated == 0);
    AFX_ASSERT(BytesReserved == 0);
#endif /* defined(RWDEBUG) */

    /*  Destroy the free list */
    while (vbMgr->freelist != NULL)
    {
        freeVBlistEntry   *nextFreelist = vbMgr->freelist->next;

        AfxPopSlabUnit(&vbMgr->FreeVBFreeList, vbMgr->freelist);

        vbMgr->freelist = nextFreelist;
    }

    /*  Destroy the VB list */
    while (vbMgr->vblist != NULL)
    {
        createdVBlistEntry   *nextvblist = vbMgr->vblist->next;

        if (vbMgr->vblist->vbo != NULL)
        {
            AfxDisposeObjects(1, &(vbMgr->vblist->vbo));
        }

        AfxPopSlabUnit(&vbMgr->CreatedVBFreeList, vbMgr->vblist);

        vbMgr->vblist = nextvblist;
    }

    AfxDismantleSlabAllocator(&vbMgr->CreatedVBFreeList);
    AfxDismantleSlabAllocator(&vbMgr->FreeVBFreeList);

#if defined(RWDEBUG)
    BlocksCreated = 0;
    VBCreated = 0;
    BytesReserved = 0;
#endif /* defined(RWDEBUG) */
}

#endif

_ASX afxError ArxBufferizeMesh(arxMesh msh, afxUnit morphIdx, arxVertexCache* vtxCache, arxIndexCache* idxCache)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    
    if (msh->vtxCache.buf)
    {
        if (vtxCache)
            *vtxCache = msh->vtxCache;
    }
    else
    {
        afxString const positionals[] = { AFX_STRING("pos"), AFX_STRING("pvt"), AFX_STRING("wgt") };

        afxUnit cacheStride[2] = { 0 };
        afxUnit cacheIdx[ARX_MAX_VERTEX_ATTRIBUTES];
        afxUnit cachedAttrOffset[ARX_MAX_VERTEX_ATTRIBUTES];
        avxFormat cachedAttrFmt[ARX_MAX_VERTEX_ATTRIBUTES];

        for (afxUnit i = 0; i < msh->attrCnt; i++)
        {
            arxMeshAttr* attr = &msh->attrInfo[i];
            avxFormat fmt = attr->fmt;
            AFX_ASSERT(fmt < avxFormat_TOTAL);

            afxUnit sIdx = AFX_INVALID_INDEX;
            AfxCompareStrings(&msh->attrIds[i], 0, FALSE, ARRAY_SIZE(positionals), positionals, &sIdx);

            switch (sIdx)
            {
            case 0: // pos
            {
                cachedAttrFmt[i] = avxFormat_RGB32f;
                cacheIdx[i] = 0;
                break;
            }
            case 1: // pvt
            {
                switch (fmt)
                {
                case avxFormat_R16u:
                case avxFormat_R32u:
                case avxFormat_R8i:
                case avxFormat_R16i:
                case avxFormat_R32i:
                    cachedAttrFmt[i] = avxFormat_R8u;
                    break;
                default:
                    cachedAttrFmt[i] = fmt;
                    break;
                }
                cacheIdx[i] = 0;
                break;
            }
            case 2: // wgt
            {
                switch (fmt)
                {
                case avxFormat_R32f:
                    cachedAttrFmt[i] = avxFormat_R8un;
                    break;
                case avxFormat_RG32f:
                    cachedAttrFmt[i] = avxFormat_RG8un;
                    break;
                case avxFormat_RGB32f:
                    cachedAttrFmt[i] = avxFormat_RGB8un;
                    break;
                case avxFormat_RGBA32f:
                    cachedAttrFmt[i] = avxFormat_RGBA8un;
                    break;
                default:
                    cachedAttrFmt[i] = fmt;
                    break;
                }
                cacheIdx[i] = 0;
                break;
            }
            default:
            {
                cachedAttrFmt[i] = attr->fmt;
                cacheIdx[i] = 1;
                break;
            }
            }
            cachedAttrOffset[i] = cacheStride[cacheIdx[i]];
            avxFormatDescription pfd;
            AvxDescribeFormats(1, &fmt, &pfd);
            cacheStride[cacheIdx[i]] += pfd.stride;//AfxVertexFormatGetSize(cachedAttrFmt[i]);
        }

        arxVertexCache* cache = &msh->vtxCache;
        AfxPushLink(&cache->vbuf, NIL);
        cache->vin = NIL;

        avxBufferInfo vboSpec = { 0 };
        vboSpec.flags = avxBufferFlag_W;
        vboSpec.usage = avxBufferUsage_VERTEX;
        vboSpec.size = 0;

        for (afxUnit i = 0; i < 2; i++)
        {
            cache->streams[i].base = i ? msh->vtxCache.streams[i - 1].range : 0;
            cache->streams[i].range = AFX_ALIGN_SIZE(cacheStride[i] * msh->vtxCnt, AVX_BUFFER_ALIGNMENT);
            cache->streams[i].stride = cacheStride[i];

            vboSpec.size += cache->streams[i].range;
        }

        avxBuffer buf;
        arxRenderware rwe = AfxGetHost(msh);
        afxDrawSystem dsys = ArxGetRenderwareDrawSystem(rwe);
        AvxAcquireBuffers(/*rwe*/dsys, 1, &vboSpec, &buf);
        cache->buf = buf;

        for (afxUnit srcIdx = 0; srcIdx < 2; srcIdx++)
        {
            AFX_ASSERT(cacheStride[srcIdx]);
            AFX_ASSERT_RANGE(AvxGetBufferCapacity(cache->buf, 0), cache->streams[srcIdx].base, cache->streams[srcIdx].range);
            void* dst;

            if (AvxMapBuffer(cache->buf, cache->streams[srcIdx].base, cache->streams[srcIdx].range, NIL, &dst)) AfxThrowError();
            else
            {
                for (afxUnit j = 0; j < msh->attrCnt; j++)
                {
                    if (cacheIdx[j] == srcIdx)
                    {
                        arxMeshAttr* attr = &msh->attrInfo[j];
                        //AFX_ASSERT(data);
                        avxFormat fmt = attr->fmt;
                        AFX_ASSERT(fmt < avxFormat_TOTAL);

                        afxByte* data = ArxAccessVertexData(msh, j, morphIdx, 0);

                        if (data)
                        {
                            avxFormatDescription pfd;
                            AvxDescribeFormats(1, &fmt, &pfd);
                            afxSize srcStride = pfd.stride;// AfxVertexFormatGetSize(fmt);
                            AFX_ASSERT(srcStride);
                            AfxStream3(msh->vtxCnt, data, 0, srcStride, dst, cachedAttrOffset[j], cache->streams[srcIdx].stride);
                        }
                    }
                }
                AvxUnmapBuffer(cache->buf, TRUE);
            }
        }

        if (vtxCache)
            *vtxCache = *cache;
    }

    //arxIndexCache* cache = &msh->cache;

    if (!msh->ibo)
    {
        arxRenderware rwe = AfxGetHost(msh);
        afxDrawSystem dsys = ArxGetRenderwareDrawSystem(rwe);

        afxUnit idxSiz = msh->minIdxSiz;
        idxSiz = AFX_MAX(sizeof(afxUnit16), idxSiz); // atleast 16-bit.

        avxBufferInfo bufi = { 0 };
        bufi.size = msh->idxCnt * idxSiz;
        bufi.flags = avxBufferFlag_W;
        bufi.usage = avxBufferUsage_INDEX;
        if (AvxAcquireBuffers(dsys, 1, &bufi, &msh->ibo)) AfxThrowError();
        else
        {
            AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &msh->ibo);

            msh->iboBase = 0;
            msh->iboRange = bufi.size;
            msh->iboStride = msh->iboRange / msh->idxCnt;
            AFX_ASSERT(msh->iboStride == idxSiz);
            //cache->idxSiz = idxSiz;
        }

        avxBuffer buf;

        if ((buf = msh->ibo))
        {
            AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);

            AFX_ASSERT(msh->iboStride);
            AFX_ASSERT_RANGE(AvxGetBufferCapacity(buf, 0), msh->iboBase, msh->iboRange);
            //AvxUpdateBuffer2(cache->buf, cache->base, cache->range, cache->stride, msht->vtxIdx, sizeof(msht->vtxIdx[0]));

            afxUnit* indices = msh->indices;

            avxBufferIo iop = { 0 };
            iop.dstOffset = msh->iboBase;
            iop.dstStride = msh->iboStride;
            iop.srcStride = sizeof(indices[0]);
            iop.rowCnt = msh->idxCnt;
            AvxUpdateBuffer(msh->ibo, 1, &iop, indices, 0);

#if 0//_AFX_DEBUG
            void* p;
            AfxWaitForDrawBridge(dsys, 0, 0);
            AvxMapBuffer(cache->buf, cache->base, cache->range, NIL, &p);

            for (afxUnit i = 0; i < mshi.idxCnt; i++)
            {
                if (mshi.minIdxSiz == 1)
                {
                    afxIndex8* idx8 = p;
                    AFX_ASSERT(indices[i] == idx8[i]);
                }
                else if (mshi.minIdxSiz == 2)
                {
                    afxIndex16* idx16 = p;
                    AFX_ASSERT(indices[i] == idx16[i]);
                }
                else if (mshi.minIdxSiz == 4)
                {
                    afxIndex* idx32 = p;
                    AFX_ASSERT(indices[i] == idx32[i]);
                }

                if (err)
                {
                    int a = 1;
                }
            }

            AvxUnmapBuffer(cache->buf, FALSE);
#endif
        }
    }

    if (idxCache)
    {
        idxCache->buf = msh->ibo;
        idxCache->base = msh->iboBase;
        idxCache->range = msh->iboRange;
        idxCache->stride = msh->iboStride;
    }
    return err;
}

_ARX afxError ArxCmdRenderMeshes(arxRenderContext rctx, afxUnit cnt, arxMesh meshes[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);

    arxRenderFrame* frame = &rctx->frames[rctx->frameIdx];

    for (afxUnit mshIdx = 0; mshIdx < cnt; mshIdx++)
    {
        arxMesh msh = meshes[mshIdx];
        if (!msh) continue;
        AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

        arxMeshInfo mshi;
        ArxDescribeMesh(msh, &mshi);

        _arxVisualMesh* instMsh;
        afxUnit mshId = AfxGetObjectId(msh);
        if (!AfxGetPoolUnit(&rctx->visualMeshes, mshId, &instMsh))
        {
            AfxRequestPoolUnitsAt(&rctx->visualMeshes, 1, &mshId, &instMsh);

            AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

            instMsh->msh = msh;

            ArxBufferizeMesh(msh, 0, &instMsh->vtxCache, &instMsh->idxCache);
        }

        avxBufferedStream vtxSrcs[] =
        {
            {
                .buf = instMsh->vtxCache.buf,
                .offset = instMsh->vtxCache.streams[0].base,
                .range = instMsh->vtxCache.streams[0].range,
                .stride = instMsh->vtxCache.streams[0].stride
            },
            {
                .buf = instMsh->vtxCache.buf,
                .offset = instMsh->vtxCache.streams[1].base,
                .range = instMsh->vtxCache.streams[1].range,
                .stride = instMsh->vtxCache.streams[1].stride
            }
        };
        AvxCmdBindVertexBuffers(frame->drawDctx, 0, /*ARRAY_SIZE(vtxSrcs)*/2, vtxSrcs);

        AvxCmdBindIndexBuffer(frame->drawDctx, instMsh->idxCache.buf,
            instMsh->idxCache.base,
            instMsh->idxCache.range,
            instMsh->idxCache.stride);

        arxMeshMorph mshm;
        ArxDescribeMeshMorphes(msh, 0, 1, &mshm);

        afxUnit mtlDataOffset = 0;

        for (afxUnit mtlIdx = 0; mtlIdx < mshi.mtlCnt; mtlIdx++)
        {
            //rig.txd;
            // TODO: write materials
            mtlDataOffset = 0;
        }

        //AvxCmdBindBuffers(rctx->drawDctx, 2, 0, 1, (avxBufferedMap[]) { {.buf = visualMsh->mtlUbo } });

        afxUnit surfCnt = mshi.secCnt;

        for (afxUnit j = 0; j < surfCnt; j++)
        {
            arxMeshSection sec;
            ArxDescribeMeshSections(msh, j, 1, &sec);

            if (sec.mtlIdx != AFX_INVALID_INDEX)
            {
                // Push textures

                _arxVisualMaterial* visualMtl;
                //AfxGetPoolUnit(&rctx->visualMaterials, mdl->rigs[mshIdx]->mtlToTxdMap[sec.mtlIdx], &visualMtl);

                //AvxCmdBindBuffers(frame->drawDctx, 2, 0, 1, &AVX_BUFFERED_MAP(visualMtl->dataUbo, visualMtl->dataUboBase, visualMtl->dataUboRange, 0));
            }

            afxUnit idxCnt = (sec.triCnt * 3);
            afxUnit firstIdx = (sec.baseTriIdx * 3);

            DrawIndexedIndirectCmd cmd;
            cmd.cmd.baseIdx = firstIdx;
            cmd.cmd.baseInst = 0;// scn->framesets[frameIdx].indDrawCnt;
            cmd.cmd.idxCnt = idxCnt;
            cmd.cmd.instCnt = 1;
            cmd.cmd.vtxOffset = mshm.baseVtx;
            cmd.mtlDataOffset = frame->mtlDataUboBaseOffset;
            cmd.jntMapOffset = frame->biasMapFboBaseIdx;// biasMapOffset;
            cmd.mtxStackOffset = frame->mtxStackUboBaseOffset;// mtxBufOffset;

            AvxCmdPushConstants(frame->drawDctx, 0, sizeof(afxUnit32) * 3, &cmd.mtlDataOffset);

            AvxCmdDrawIndexed(frame->drawDctx, idxCnt, 1, firstIdx, mshm.baseVtx, 0);
        }
    }
    return err;
}

_ARX afxError ArxCmdRenderModels(arxRenderContext rctx, afxUnit cnt, arxModel models[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);

    arxRenderFrame* frame = &rctx->frames[rctx->frameIdx];

    for (afxUnit mdlIdx = 0; mdlIdx < cnt; mdlIdx++)
    {
        arxModel mdl = models[mdlIdx];
        if (!mdl) continue;
        AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);

        arxModelInfo mdli;
        ArxDescribeModel(mdl, &mdli);

        _arxVisualModel* instMdl;
        afxUnit mdlId = AfxGetObjectId(mdl);
        if (!AfxGetPoolUnit(&rctx->visualModels, mdlId, &instMdl))
        {
            AfxRequestPoolUnitsAt(&rctx->visualModels, 1, &mdlId, &instMdl);

            afxUnit biasMapPop = 0;
            afxUnit biasMapBufSiz = 0;

            for (afxUnit rigIdx = 0; rigIdx < mdli.rigCnt; rigIdx++)
            {
                arxMesh msh = NIL;
                ArxGetRiggedMeshes(mdl, rigIdx, 1, &msh);

                arxMeshRigInfo rig;
                ArxDescribeMeshRig(mdl, rigIdx, &rig);

                if (!msh) continue;
                AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

                arxMeshInfo mshi;
                ArxDescribeMesh(msh, &mshi);

                biasMapBufSiz += mshi.biasCnt * sizeof(rig.biasToJnt[0]);
                biasMapPop += mshi.biasCnt;
            }

            avxBuffer buf;
            avxBufferInfo bufi = { 0 };
            bufi.size = biasMapBufSiz;
            bufi.usage = avxBufferUsage_FETCH;
            bufi.flags = avxBufferFlag_WX/* | avxBufferFlag_COHERENT*/;
            bufi.fmt = avxFormat_R32u;
            AvxAcquireBuffers(rctx->dsys, 1, &bufi, &buf);

            instMdl->mdl = mdl;
            instMdl->biasMapPop = biasMapPop;
            instMdl->biasMapFbo = buf;
            instMdl->biasMapFboBase = 0;
            instMdl->biasMapFboRange = bufi.size;

            afxUnit biasMapBufBase = 0;

            for (afxUnit rigIdx = 0; rigIdx < mdli.rigCnt; rigIdx++)
            {
                arxMesh msh = NIL;
                ArxGetRiggedMeshes(mdl, rigIdx, 1, &msh);

                arxMeshRigInfo rig;
                ArxDescribeMeshRig(mdl, rigIdx, &rig);

                if (!msh) continue;
                AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

                arxMeshInfo mshi;
                ArxDescribeMesh(msh, &mshi);

                afxUnit biasMapSiz = mshi.biasCnt * sizeof(rig.biasToJnt[0]);
                AvxCmdUpdateBuffer(frame->transferDctx, instMdl->biasMapFbo, biasMapBufBase, biasMapSiz, rig.biasToJnt);
                biasMapBufBase += biasMapSiz;
            }
        }

        frame->mtlDataUboBaseOffset = 0;
        frame->biasMapFboBaseOffset = instMdl->biasMapFboBase;
        frame->biasMapFboBaseIdx = 0;
        AvxCmdBindBuffers(frame->drawDctx, avxBus_DRAW, 3, 31, 1, (avxBufferedMap[]) { {.buf = instMdl->biasMapFbo } });

        afxUnit biasMapBufBaseOffset = 0;
        afxUnit biasMapBufBaseIdx = 0;

        for (afxUnit rigIdx = 0; rigIdx < mdli.rigCnt; rigIdx++)
        {
            arxMesh msh = NIL;
            ArxGetRiggedMeshes(mdl, rigIdx, 1, &msh);

            arxMeshRigInfo rig;
            ArxDescribeMeshRig(mdl, rigIdx, &rig);

            if (!msh) continue;
            AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

            arxMeshInfo mshi;
            ArxDescribeMesh(msh, &mshi);

            ArxStageMaterials(rctx, mshi.mtlCnt, mdl->rigs[rigIdx]->mtlToTxdMap);


            afxUnit biasMapSiz = mshi.biasCnt * sizeof(rig.biasToJnt[0]);
            frame->biasMapFboBaseOffset = instMdl->biasMapFboBase + biasMapBufBaseOffset;
            frame->biasMapFboBaseIdx = biasMapBufBaseIdx;
            biasMapBufBaseIdx += mshi.biasCnt;
            biasMapBufBaseOffset += biasMapSiz;

            ArxCmdRenderMeshes(rctx, 1, &msh);
        }
    }
    return err;
}

void cullAABBs2(const afxBox *boxes, size_t numBoxes, const afxFrustum *frustum, afxBool *culledResults);

afxBool RenderCells(arxRenderContext rctx, afxDrawContext dctx, avxVertexInput vin, arxTerrain ter, afxFrustum* frustum, afxBool showFaces, afxBool showDbgLines)
{
    afxBool result;

    afxUnit cellsCulledCnt = 0;
    afxUnit cellsToBeDrawnCnt = 0;
    afxUnit cellsToBeDrawnIdx[64] = { 0 };

    for (afxUnit i = 0; i < ter->secCnt; i++)
    {
        // Get the current cell dimensions.
        afxBox aabb = ter->sectors[i].aabb;

        // Check to see if the positions are in this cell.
        //if (AfxDoesFrustumCullAabbs(frustum, 1, &aabb))
        afxBool culled = TRUE;
        cullAABBs2(&aabb, 1, frustum, &culled);

        //culled = FALSE;
        if (i >= 63) break;

        if (culled)
        {
            // Increment the number of cells that were culled.
            ++cellsCulledCnt;
        }
        else
        {
            cellsToBeDrawnIdx[cellsToBeDrawnCnt] = i;
            ++cellsToBeDrawnCnt;
        }
    }

    if (showDbgLines)
    {
#if 0
        if (!ter->dbgLinesVbo)
        {
            // We store edges as 24 16-bit indices
            // 2730 * 24 = 65520 indices
            // It will crash if there are more than 2730 sectors.

            avxBuffer bufs[2];
            avxBufferInfo bufis[2] = { 0 };
            bufis[0].size = ter->secCnt * 8 * sizeof(afxV3d);
            bufis[0].usage = avxBufferUsage_VERTEX;
            bufis[0].flags = avxBufferFlag_WX;
            bufis[1].size = ter->secCnt * 24 * sizeof(afxUnit16);
            bufis[1].usage = avxBufferUsage_INDEX;
            bufis[1].flags = avxBufferFlag_WX;
            AvxAcquireBuffers(ter->dsys, 2, bufis, bufs);

            afxV3d* vertices = NIL;
            afxUnit16* indices = NIL;
            avxBufferedMap maps[2] = { 0 };
            maps[0].buf = bufs[0];
            maps[0].range = bufis[0].size;
            maps[1].buf = bufs[1];
            maps[1].range = bufis[1].size;
            AvxMapBuffers(ter->dsys, 2, maps, (void**[]) { &vertices, &indices });

            ArxCmdPostVertices(rctx, ter->secCnt * 8, sizeof(afxV3d), &vertices);
            ArxCmdPostVertexIndices(rctx, ter->secCnt * 24, sizeof(afxUnit16), &indices);
            AsxGenerateIndexedLinesForAabbs(ter->secCnt, ter->secAabb, vertices, indices, NIL, NIL, NIL);
#if 0
            afxUnit baseIdx = 0;
            for (afxUnit i = 0; i < ter->secCnt; i++)
            {
                _asxTerrSec* sec = &ter->sectors[i];
                //baseIdx = sec->gridNode;
                AsxGenerateIndexedLinesForAabbs(1, &sec->aabb, &vertices[i * AFX_NUM_BOX_CORNERS], baseIdx * AFX_NUM_BOX_EDGE_VERTICES, &indices[i * AFX_NUM_BOX_EDGE_VERTICES] /*i == 0 ? indices : 0*/, NIL, NIL, NIL);
                baseIdx += AFX_NUM_BOX_EDGE_VERTICES;
            }
#endif       
            AvxUnmapBuffers(ter->dsys, 2, maps);
            ter->dbgLinesVbo = bufs[0];
            ter->dbgLinesVboRange = bufis[0].size;
            ter->dbgLinesVboStride = sizeof(afxV3d);
            ter->dbgLinesIbo = bufs[1];
            ter->dbgLinesIboRange = bufis[1].size;
            ter->dbgLinesIboStride = sizeof(afxUnit16);
        }
#endif

        AvxCmdBindPipeline(dctx, 0, ter->dbgAabbPip, vin, NIL);

        afxV3d* vertices = NIL;
        afxUnit16* indices = NIL;
        ArxCmdPostVertices(rctx, ter->secCnt * 8, sizeof(afxV3d), &vertices);
        ArxCmdPostVertexIndices(rctx, ter->secCnt * 24, sizeof(afxUnit16), &indices);
        AsxGenerateIndexedLinesForAabbs(ter->secCnt, ter->secAabb, vertices, indices, NIL, NIL, NIL);

        afxReal*color;
        ArxCmdPostUniform(rctx, 0, 1, sizeof(afxV4d), &color);
        AfxV4dSet(color, 0, 0.3, 0.3, 1);
        AvxCmdPushConstants(dctx, 0, sizeof(afxM4d), AFX_M4D_IDENTITY);
        avxBufferedStream stream = { 0 };
        stream.buf = ter->dbgLinesVbo;
        stream.offset = 0;
        stream.range = ter->dbgLinesVboRange;
        stream.stride = ter->dbgLinesVboStride;
        //AvxCmdBindVertexBuffers(dctx, 0, 1, &stream);
        //AvxCmdBindIndexBuffer(dctx, ter->dbgLinesIbo, 0, ter->dbgLinesIboRange, ter->dbgLinesIboStride);

#if !0
        AvxCmdDrawIndexed(dctx, ter->secCnt * 24, 1, 0, 0, 0);
#else
        afxUnit vtxOffset = 0;
        afxUnit idxBase = 0;
        //for (afxUnit i = 0; i < cellsToBeDrawnCnt; i++)
        for (afxUnit i = 0; i < ter->secCnt; i++)
        {
            _asxTerrSec* sec = &ter->sectors[i];

            AvxCmdDrawIndexed(dctx, 24, 1, idxBase, vtxOffset, 0);
            vtxOffset += 8 * 12;
            idxBase += 24;
        }
#endif
    }

#if !0
    if (showFaces)
    {
        //for (afxUnit cellIdx = 0; cellIdx < cellsToBeDrawnCnt; cellIdx++)
        for (afxUnit cellIdx = 0; cellIdx < ter->secCnt; cellIdx++)
        {
            //_arxTerrSec* sec = &ter->sectors[cellsToBeDrawnIdx[cellIdx]];
            _arxTerrSec* sec = &ter->sectors[cellIdx];

            if (!sec->vbo)
            {
                struct
                {
                    afxV3d pos;
                    afxV3d nrm;
                    afxV3d tan;
                    afxV3d bit;
                    afxV2d uv;
                }*vertices = NIL;
                afxUnit16* indices = NIL;

                avxBuffer bufs[2];
                avxBufferInfo bufis[2] = { 0 };
                bufis[0].size = sec->vtxCnt * sizeof(vertices[0]);
                bufis[0].usage = avxBufferUsage_VERTEX;
                bufis[0].flags = avxBufferFlag_WX;
                bufis[1].size = sec->vtxIdxCnt * sizeof(indices[0]);
                bufis[1].usage = avxBufferUsage_INDEX;
                bufis[1].flags = avxBufferFlag_WX;
                AvxAcquireBuffers(ter->dsys, 2, bufis, bufs);
#if !0
                sec->vbo = bufs[0];
                sec->vboRange = bufis[0].size;
                sec->vboStride = sizeof(vertices[0]);
                sec->ibo = bufs[1];
                sec->iboRange = bufis[1].size;
                sec->iboStride = sizeof(indices[0]);

                avxBufferedMap maps[2] = { 0 };
                maps[0].buf = bufs[0];
                maps[0].range = bufis[0].size;
                maps[1].buf = bufs[1];
                maps[1].range = bufis[1].size;
                AvxMapBuffers(ter->dsys, 2, maps, (void**[]) { &vertices, &indices });

                AfxStream2(sec->vtxIdxCnt, sec->visuVtxMap, sizeof(sec->visuVtxMap[0]), indices, sizeof(indices[0]));
                AfxStream2(sec->vtxCnt, sec->visuVtxPos, sizeof(sec->visuVtxPos[0]), vertices, sizeof(vertices[0]));

                AvxUnmapBuffers(ter->dsys, 2, maps);
#endif
            }
        }

        AvxCmdBindPipeline(dctx, 0, ter->meshPip, NIL, NIL);

        if (!ter->texd)
        {
            avxRasterInfo rasi = { 0 };
            rasi.usage = avxRasterUsage_TEXTURE;
            AvxLoadRasters(ter->dsys, 1, &rasi, AfxUri("../dirt01d.tga"), NIL, &ter->texd);
        }

        AvxCmdBindRasters(dctx, avxBus_DRAW, 0, 1, 1, &ter->texd);

        //for (afxUnit i = 0; i < cellsToBeDrawnCnt; i++)
        for (afxUnit i = 0; i < ter->secCnt; i++)
        {
            //_arxTerrSec* sec = &ter->sectors[cellsToBeDrawnIdx[i]];
            _arxTerrSec* sec = &ter->sectors[i];

            avxBufferedStream stream = { 0 };
            stream.buf = sec->vbo;
            stream.offset = sec->vboBase;
            stream.range = sec->vboRange;
            stream.stride = sec->vboStride;
            AvxCmdBindVertexBuffers(dctx, 0, 1, &stream);
            AvxCmdBindIndexBuffer(dctx, sec->ibo, sec->iboBase, sec->iboRange, sec->iboStride);

            AvxCmdDrawIndexed(dctx, sec->vtxIdxCnt, 1, 0, 0, 0);
        }
    }
#endif
    return TRUE;
}

_ARX afxError ArxCmdRenderTerrain(arxRenderContext rctx, arxTerrain ter)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);

    arxRenderFrame* frame = &rctx->frames[rctx->frameIdx];

    arxMesh msh;
    if (ArxGetTerrainMeshes(ter, 0, 1, &msh))
    {
        arxMeshInfo mshi;
        ArxDescribeMesh(msh, &mshi);

        arxIndexCache idxCache;
        arxVertexCache vtxCache;
        ArxBufferizeMesh(msh, 0, &vtxCache, &idxCache); // request drawable cache.

        avxBufferedStream vtxSrcs[] =
        {
            {
                .buf = vtxCache.buf,
                .offset = vtxCache.streams[0].base,
                .range = vtxCache.streams[0].range,
                .stride = vtxCache.streams[0].stride
            },
            {
                .buf = vtxCache.buf,
                .offset = vtxCache.streams[1].base,
                .range = vtxCache.streams[1].range,
                .stride = vtxCache.streams[1].stride
            }
        };

        AvxCmdBindVertexBuffers(frame->drawDctx, 0, ARRAY_SIZE(vtxSrcs), vtxSrcs);

        AvxCmdBindIndexBuffer(frame->drawDctx, idxCache.buf, idxCache.base, idxCache.range, idxCache.stride);

        AvxCmdDrawIndexed(frame->drawDctx, mshi.idxCnt, 1, 0, NIL, 0);
    }

    return err;
}
