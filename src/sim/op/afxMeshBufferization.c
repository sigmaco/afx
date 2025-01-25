/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *        Q W A D R O   V I D E O   G R A P H I C S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

//#define _AVX_DRAW_C
#define _ASX_MESH_C
//#define _AVX_MESH_TOPOLOGY_C
#define _AVX_VERTEX_BUFFER_C
//#define _AVX_GEOMETRY_C
#define _AVX_INDEX_BUFFER_C
//#define _AVX_DRAW_INPUT_C
#include "../impl/asxImplementation.h"

AFX_DEFINE_STRUCT(avxIndexBuffer)
{
    afxChain    rows;
    afxBuffer   buf;
    afxChain    rooms;
};

AFX_DEFINE_STRUCT(freeVBlistEntry)
{
    freeVBlistEntry*        next;
    freeVBlistEntry*        prev;
    afxBuffer               vbo;
    afxUnit32                offset;
    afxUnit32                size;
};

AFX_DEFINE_STRUCT(createdVBlistEntry)
{
    createdVBlistEntry*     next;
    afxBuffer               vbo;
};

AFX_OBJECT(afxBufferizer)
{
    afxDrawInput        din;
    afxUnit32           DefaultVBSize;

    freeVBlistEntry*    freelist;
    createdVBlistEntry* vblist;
    afxSlabAllocator    FreeVBFreeList;
    afxSlabAllocator    CreatedVBFreeList;
};

#if !0
afxBool AfxBufferizeIndices(afxBufferizer vbMgr, afxUnit32 stride, afxUnit32 size, avxIndexCache* cache)
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
        
        afxDrawSystem dsys = AfxGetDrawInputContext(vbMgr->din);

        afxBuffer vbo;
        afxBufferInfo spec = { 0 };
        spec.cap = freelist->size;
        spec.flags = afxBufferFlag_W;
        spec.usage = afxBufferUsage_INDEX;

        if (AfxAcquireBuffers(dsys, 1, &spec, &vbo))
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

void DestroyIndexBuffer(afxBufferizer vbMgr, afxUnit32 stride, afxUnit32 size, afxBuffer vbo, afxUnit32 offset)
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

afxBool _IndexBufferManagerOpen(afxBufferizer vbMgr)
{
    afxError err = NIL;
    //avxVertexBufferizer* vbMgr = AfxAllocate(1, sizeof(vbMgr[0]), 0, AfxHere());
    //*vbMgrPtr = vbMgr;

    vbMgr->DefaultVBSize = 128 * 1024;
    AfxDeploySlabAllocator(&vbMgr->FreeVBFreeList, sizeof(freeVBlistEntry), 100);
    AfxDeploySlabAllocator(&vbMgr->CreatedVBFreeList, sizeof(createdVBlistEntry), 100);

    return TRUE;
}

void _IndexBufferManagerClose(afxBufferizer vbMgr)
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

_ASX afxError AfxBufferizeMesh(afxMesh msh, afxUnit morphIdx, avxVertexCache* vtxCache, avxIndexCache* idxCache)
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
        afxString const positionals[] = { AfxString("pos"), AfxString("pvt"), AfxString("wgt") };

        afxUnit cacheStride[2] = { 0 };
        afxUnit cacheIdx[AFX_MAX_VERTEX_ATTRIBUTES];
        afxUnit cachedAttrOffset[AFX_MAX_VERTEX_ATTRIBUTES];
        avxFormat cachedAttrFmt[AFX_MAX_VERTEX_ATTRIBUTES];

        for (afxUnit i = 0; i < msh->attrCnt; i++)
        {
            avxMeshAttr* attr = &msh->attrInfo[i];
            avxFormat fmt = attr->fmt;
            AFX_ASSERT(fmt < avxFormat_TOTAL);

            switch (AfxCompareStrings(&msh->attrIds[i], 0, FALSE, ARRAY_SIZE(positionals), positionals))
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
            AfxDescribePixelFormat(fmt, &pfd);
            cacheStride[cacheIdx[i]] += pfd.stride;//AfxVertexFormatGetSize(cachedAttrFmt[i]);
        }

        avxVertexCache* cache = &msh->vtxCache;
        AfxPushLink(&cache->vbuf, NIL);
        cache->vin = NIL;

        afxBufferInfo vboSpec = { 0 };
        vboSpec.flags = afxBufferFlag_W;
        vboSpec.usage = afxBufferUsage_VERTEX;
        vboSpec.cap = 0;

        for (afxUnit i = 0; i < 2; i++)
        {
            cache->streams[i].base = i ? msh->vtxCache.streams[i - 1].range : 0;
            cache->streams[i].range = AFX_ALIGNED_SIZE(cacheStride[i] * msh->vtxCnt, AFX_BUF_ALIGNMENT);
            cache->streams[i].stride = cacheStride[i];

            vboSpec.cap += cache->streams[i].range;
        }

        afxBuffer buf;
        afxSimulation sim = AfxGetProvider(msh);
        afxDrawSystem dsys = AfxGetSimulationDrawSystem(sim);
        AfxAcquireBuffers(/*din*/dsys, 1, &vboSpec, &buf);
        cache->buf = buf;

        for (afxUnit srcIdx = 0; srcIdx < 2; srcIdx++)
        {
            AFX_ASSERT(cacheStride[srcIdx]);
            AFX_ASSERT_RANGE(AfxGetBufferCapacity(cache->buf, 0), cache->streams[srcIdx].base, cache->streams[srcIdx].range);
            void* dst;

            if (AfxMapBuffer(cache->buf, cache->streams[srcIdx].base, cache->streams[srcIdx].range, NIL, &dst)) AfxThrowError();
            else
            {
                for (afxUnit j = 0; j < msh->attrCnt; j++)
                {
                    if (cacheIdx[j] == srcIdx)
                    {
                        avxMeshAttr* attr = &msh->attrInfo[j];
                        //AFX_ASSERT(data);
                        avxFormat fmt = attr->fmt;
                        AFX_ASSERT(fmt < avxFormat_TOTAL);

                        afxByte* data = AfxAccessVertexData(msh, j, morphIdx, 0);

                        if (data)
                        {
                            avxFormatDescription pfd;
                            AfxDescribePixelFormat(fmt, &pfd);
                            afxSize srcStride = pfd.stride;// AfxVertexFormatGetSize(fmt);
                            AFX_ASSERT(srcStride);
                            AfxStream3(msh->vtxCnt, data, 0, srcStride, dst, cachedAttrOffset[j], cache->streams[srcIdx].stride);
                        }
                    }
                }
                AfxUnmapBuffer(cache->buf, TRUE);
            }
        }

        if (vtxCache)
            *vtxCache = *cache;
    }

    //avxIndexCache* cache = &msh->cache;

    if (!msh->ibo)
    {
        afxSimulation sim = AfxGetProvider(msh);
        AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
        afxDrawSystem dsys = AfxGetSimulationDrawSystem(sim);

        afxUnit idxSiz = msh->minIdxSiz;

        afxBufferInfo bufi = { 0 };
        bufi.cap = msh->idxCnt * idxSiz;
        bufi.flags = afxBufferFlag_W;
        bufi.usage = afxBufferUsage_INDEX;
        if (AfxAcquireBuffers(dsys, 1, &bufi, &msh->ibo)) AfxThrowError();
        else
        {
            AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &msh->ibo);

            msh->iboBase = 0;
            msh->iboRange = bufi.cap;
            msh->iboStride = msh->iboRange / msh->idxCnt;
            AFX_ASSERT(msh->iboStride == idxSiz);
            //cache->idxSiz = idxSiz;
        }

        afxBuffer buf;

        if ((buf = msh->ibo))
        {
            AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);

            AFX_ASSERT(msh->iboStride);
            AFX_ASSERT_RANGE(AfxGetBufferCapacity(buf, 0), msh->iboBase, msh->iboRange);
            //AfxUpdateBuffer2(cache->buf, cache->base, cache->range, cache->stride, msht->vtxIdx, sizeof(msht->vtxIdx[0]));

            afxUnit* indices = msh->indices;

            afxBufferIo iop = { 0 };
            iop.dstOffset = msh->iboBase;
            iop.dstStride = msh->iboStride;
            iop.srcStride = sizeof(indices[0]);
            iop.rowCnt = msh->idxCnt;
            AfxUpdateBuffer(msh->ibo, 1, &iop, 0, indices);

#if 0//_AFX_DEBUG
            void* p;
            AfxWaitForDrawBridge(dsys, 0, 0);
            AfxMapBuffer(cache->buf, cache->base, cache->range, NIL, &p);

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

            AfxUnmapBuffer(cache->buf, FALSE);
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
