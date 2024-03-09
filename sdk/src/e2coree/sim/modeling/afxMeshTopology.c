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
#define _AFX_MESH_TOPOLOGY_C
#define _AFX_SIMULATION_C
#include "qwadro/sim/modeling/afxMeshTopology.h"
#include "qwadro/sim/afxSimulation.h"
#include "qwadro/sim/awxAsset.h"

_AKX afxNat AfxCountMeshSurfaces(afxMeshTopology msht)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);
    return msht->surfCnt;
}

_AKX afxNat AfxCountMeshIndices(afxMeshTopology msht)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);
    return msht->triCnt * 3;
}

_AKX afxNat AfxCountMeshTriangles(afxMeshTopology msht)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);
    return msht->triCnt;
}

_AKX afxNat AfxCountMeshTriangleEdges(afxMeshTopology msht)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);
    return msht->triCnt * 3;
}

_AKX afxIndexedTriangle* AfxGetMeshTriangles(afxMeshTopology msht, afxNat baseTriIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);
    AfxAssertRange(msht->triCnt, baseTriIdx, 1);
    return &msht->tris[baseTriIdx];
}

_AKX afxNat* AfxGetMeshIndices(afxMeshTopology msht, afxNat baseIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);
    AfxAssertRange(msht->triCnt, baseIdx / 3, 1);
    return &msht->tris[baseIdx / 3][baseIdx % 3];
}

_AKX afxNat AfxDetermineMeshIndexSize(afxMeshTopology msht)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);
#if 0
    afxNat surfCnt = msht->surfCnt;
    afxMeshSurface* surfaces = msht->surfaces;
    AfxAssert(surfCnt);
    afxNat triCnt = 0;

    for (afxNat i = 0; i < surfCnt; i++)
    {
        afxMeshSurface* surface = &surfaces[i];
        afxNat triCnt2 = surface->triCnt;

        if (triCnt < triCnt2)
            triCnt = triCnt2;
    }
#endif
    afxNat majIdx = 0;

    for (afxNat i = msht->triCnt; i-- > 0;)
        for (afxNat j = 3; j-- > 0;)
            if (majIdx < msht->tris[i][j])
                majIdx = msht->tris[i][j];

    return majIdx > AFX_N8_MAX ? (majIdx > AFX_N16_MAX ? (majIdx > AFX_N32_MAX ? sizeof(afxNat64) : sizeof(afxNat32)) : sizeof(afxNat16)) : sizeof(afxNat8);
#if 0
    afxNat idxCnt = triCnt * 3;
    return idxCnt > AFX_N8_MAX ? (idxCnt > AFX_N16_MAX ? (idxCnt > AFX_N32_MAX ? sizeof(afxNat64) : sizeof(afxNat32)) : sizeof(afxNat16)) : sizeof(afxNat8);
#endif
}

_AKX afxBool AfxDescribeMeshCoverage(afxMeshTopology msht, afxNat surIdx, afxMeshSurface* desc)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);
    AfxAssertRange(msht->surfCnt, surIdx, 1);
    afxMeshSurface *mshs = &msht->surfaces[surIdx];
    //AfxAssertType(mshs, afxFcc_MSHS);
    AfxAssert(desc);
    *desc = *mshs;
    return (AFX_INVALID_INDEX != mshs->mtlIdx);
}

_AKX afxMeshSurface* AfxGetMeshSurface(afxMeshTopology msht, afxNat surIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);
    AfxAssertRange(msht->surfCnt, surIdx, 1);
    afxMeshSurface *mshs = &msht->surfaces[surIdx];
    //AfxAssertType(mshs, afxFcc_MSHS);
    return mshs;
}

_AKX afxError AfxBufferizeMeshTopology(afxMeshTopology msht)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);
    afxMeshTopologyCache* cache = &msht->cache;

    if (!cache->buf)
    {
        afxSimulation sim = AfxGetObjectProvider(msht);
        AfxAssertObjects(1, &sim, afxFcc_SIM);

        afxNat idxSiz = AfxDetermineMeshIndexSize(msht);
        afxNat idxCnt = AfxCountMeshIndices(msht);

        afxBufferSpecification spec;
        spec.siz = idxCnt * idxSiz;
        spec.src = NIL;
        spec.access = afxBufferAccess_W;
        spec.usage = afxBufferUsage_INDEX;

        if (AfxAcquireBuffers(sim->dctx, 1, &spec, &cache->buf)) AfxThrowError();
        else
        {
            AfxAssertObjects(1, &cache->buf, afxFcc_BUF);

            cache->base = 0;
            cache->range = spec.siz;
            cache->stride = cache->range / idxCnt;
            AfxAssert(cache->stride == idxSiz);
            //cache->idxSiz = idxSiz;
        }

        afxBuffer buf;

        if ((buf = cache->buf))
        {
            AfxAssertObjects(1, &buf, afxFcc_BUF);

            AfxAssert(cache->stride);
            AfxAssertRange(AfxGetBufferCapacity(buf), cache->base, cache->range);
            //AfxUpdateBuffer2(cache->buf, cache->base, cache->range, cache->stride, msht->vtxIdx, sizeof(msht->vtxIdx[0]));

            afxBufferRegion rgn = { 0 };
            rgn.base = cache->base;
            rgn.range = cache->range;
            rgn.stride = cache->stride;
            
            rgn.offset = 0;
            rgn.unitSiz = cache->stride;
            AfxUpdateBufferRegion(cache->buf, &rgn, msht->tris, sizeof(msht->tris[0][0]));

            // debug
            void* p = AfxMapBufferRange(cache->buf, cache->base, cache->range, afxBufferAccess_W);

            for (afxNat i = 0; i < msht->triCnt; i++)
            {
                if (idxSiz == 1)
                {
                    afxIndexedTriangle8 *tris = p;
                    AfxAssert(msht->tris[i][0] == tris[i][0]);
                    AfxAssert(msht->tris[i][1] == tris[i][1]);
                    AfxAssert(msht->tris[i][2] == tris[i][2]);
                }
                else if (idxSiz == 2)
                {
                    afxIndexedTriangle16 *tris = p;
                    AfxAssert(msht->tris[i][0] == tris[i][0]);
                    AfxAssert(msht->tris[i][1] == tris[i][1]);
                    AfxAssert(msht->tris[i][2] == tris[i][2]);

                    tris[i][0] = msht->tris[i][0];
                    tris[i][1] = msht->tris[i][1];
                    tris[i][2] = msht->tris[i][2];
                }
                else if (idxSiz == 4)
                {
                    afxIndexedTriangle *tris = p;
                    AfxAssert(msht->tris[i][0] == tris[i][0]);
                    AfxAssert(msht->tris[i][1] == tris[i][1]);
                    AfxAssert(msht->tris[i][2] == tris[i][2]);
                }

                if (err)
                {
                    int a = 1;
                }
            }

            AfxUnmapBufferRange(cache->buf);
        }
    }
    return err;
}

_AKX void AfxRemapMeshCoverage(afxMeshTopology msht, afxNat remapCnt, afxNat const remapTable[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);

    for (afxNat i = msht->surfCnt; i-- > 0;)
    {
        afxMeshSurface* mshs = &msht->surfaces[i];
        //AfxAssertType(mshs, afxFcc_MSHS);
        afxNat mtlIdx = mshs->mtlIdx;

        if (mtlIdx >= remapCnt)
        {
            AfxError("Mesh section had out-of-range index %u during remapping.", mtlIdx);
            AfxThrowError();
        }
        else
            mshs->mtlIdx = remapTable[mtlIdx];
    }
}

_AKX void AfxInvertMeshWinding(afxMeshTopology msht)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);

    afxNat triCnt = msht->triCnt;
    afxIndexedTriangle* tris = msht->tris;
    
    for (afxNat i = 0, triCnt2 = triCnt; triCnt2-- > 0; ++i)
    {
        afxNat a = tris[i][0];
        tris[i][0] = tris[i][2];
        tris[i][2] = a;
    }

    afxIndexedTriangle* neighbor = msht->sideToNeighborMap;

    if (neighbor)
    {
        for (afxNat i = 0; triCnt-- > 0; ++i)
        {
            afxNat a = neighbor[i][0];
            neighbor[i][0] = neighbor[i][1];
            neighbor[i][1] = a;

            afxNat32 faceA = neighbor[i][0] & 0xFFFFFFFC; // the high-order 30 bits specify the actual triangle index - you would multiply it by three to get its location
            afxNat32 edgeA = neighbor[i][0] & 0x00000003; // 00 would be the 0th edge, 01 the 1st, 10 the 2nd, and 11 is undefined
            neighbor[i][0] = (edgeA = (edgeA == 0x01 ? 0x00 : (edgeA == 0x00 ? 0x01 : edgeA))) | faceA;

            afxNat32 faceB = neighbor[i][1] & 0xFFFFFFFC; // the high-order 30 bits specify the actual triangle index - you would multiply it by three to get its location
            afxNat32 edgeB = neighbor[i][1] & 0x00000003; // 00 would be the 0th edge, 01 the 1st, 10 the 2nd, and 11 is undefined
            neighbor[i][1] = (edgeB = (edgeB == 0x01 ? 0x00 : (edgeB == 0x00 ? 0x01 : edgeB))) | faceB;

            afxNat32 faceC = neighbor[i][2] & 0xFFFFFFFC; // the high-order 30 bits specify the actual triangle index - you would multiply it by three to get its location
            afxNat32 edgeC = neighbor[i][2] & 0x00000003; // 00 would be the 0th edge, 01 the 1st, 10 the 2nd, and 11 is undefined
            neighbor[i][2] = (edgeC = (edgeC == 0x01 ? 0x00 : (edgeC == 0x00 ? 0x01 : edgeC))) | faceC;
        }
    }
}

_AKX afxError AfxUpdateMeshIndices(afxMeshTopology msht, afxNat baseTriIdx, afxNat triCnt, void const* src, afxNat srcIdxSiz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);
    AfxAssertRange(msht->triCnt, baseTriIdx, triCnt);
    
    //afxNat triCnt = msht->triCnt;
    afxIndexedTriangle* tris = AfxGetMeshTriangles(msht, baseTriIdx);
    
    switch (srcIdxSiz)
    {
    case sizeof(afxVertexIndex8):
    {
        afxIndexedTriangle8 const* src3 = src;

        for (afxNat i = 0; i < triCnt; i++)
            for (afxNat j = 0; j < 3; j++)
                tris[i][j] = src3[i][j];

        break;
    }
    case sizeof(afxVertexIndex16):
    {
        afxIndexedTriangle16 const* src3 = src;

        for (afxNat i = 0; i < triCnt; i++)
            for (afxNat j = 0; j < 3; j++)
                tris[i][j] = src3[i][j];
        
        break;
    }
    case sizeof(afxVertexIndex32):
    {
        afxIndexedTriangle32 const* src3 = src;

        for (afxNat i = 0; i < triCnt; i++)
            for (afxNat j = 0; j < 3; j++)
                tris[i][j] = src3[i][j];

        break;
    }
    case sizeof(afxVertexIndex64):
    {
        afxIndexedTriangle64 const* src3 = src;

        for (afxNat i = 0; i < triCnt; i++)
            for (afxNat j = 0; j < 3; j++)
                tris[i][j] = src3[i][j];

        break;
    }
    default:
        AfxThrowError();
    }
    return err;
}

_AKX afxError _AfxMshtDtor(afxMeshTopology msht)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);
    AfxEntry("msht=%p", msht);

    afxSimulation sim = AfxGetObjectProvider(msht);
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxMmu mmu = AfxGetSimulationMmu(sim);

    for (afxNat i = msht->surfCnt; i-->0;)
    {
        afxMeshSurface* mshs = &(msht->surfaces[i]);
        //AfxAssertType(mshs, afxFcc_MSHS);

        //if (mshs->name)
        //AfxDeallocateString(&mshs->name);
    }

    if (msht->jointsForTriMap)
        AfxDeallocate(mmu, msht->jointsForTriMap);

    if (msht->triToJointMap)
        AfxDeallocate(mmu, msht->triToJointMap);

    if (msht->sideToNeighborMap)
        AfxDeallocate(mmu, msht->sideToNeighborMap);

    if (msht->vtxToTriMap)
        AfxDeallocate(mmu, msht->vtxToTriMap);

    if (msht->vtxToVtxMap)
        AfxDeallocate(mmu, msht->vtxToVtxMap);

    if (msht->tris)
        AfxDeallocate(mmu, msht->tris);

    if (msht->surfaces)
        AfxDeallocate(mmu, msht->surfaces);

    return err;
}

_AKX afxError _AfxMshtCtor(afxMeshTopology msht, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);
    AfxEntry("msht=%p", msht);

    afxSimulation sim = cookie->udd[0];
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxMeshTopologySpec const* spec = cookie->udd[1];
    spec += cookie->no;

    afxMmu mmu = AfxGetSimulationMmu(sim);

    afxNat surfCnt = spec->surCnt;
    AfxAssert(surfCnt);

    afxNat triCnt = spec->triCnt;
    AfxAssert(triCnt);

    afxNat vtxCnt = spec->vtxCnt;
    afxNat edgeCnt = triCnt * 3;

    afxIndexedTriangle* tris;

    if (!(tris = AfxAllocate(mmu, triCnt, sizeof(tris[0]), 0, AfxHint()))) AfxThrowError();
    else
    {
        afxMeshSurface* surfaces;

        if (!(surfaces = AfxAllocate(mmu, surfCnt, sizeof(surfaces[0]), 0, AfxHint()))) AfxThrowError();
        else
        {
            msht->triCnt = triCnt;
            msht->surfCnt = surfCnt;
            msht->vtxCnt = vtxCnt;

            msht->tris = tris;
            msht->surfaces = surfaces;

            for (afxNat i = 0; i < surfCnt; i++)
            {
                afxMeshSurface* mshs = &surfaces[i];
                //AfxAssignFcc(mshs, afxFcc_MSHS);
                mshs->baseTriIdx = 0;
                mshs->triCnt = 0;
                mshs->mtlIdx = AFX_INVALID_INDEX;

                if (spec->sur)
                    spec->sur(i, mshs, spec->udd);
            }

            msht->vtxToVtxMap = NIL; // vtxCnt
            msht->vtxToTriMap = NIL; // vtxCnt
            msht->sideToNeighborMap = NIL; // triCnt * 3

            msht->jointsForTriCnt = 0;
            msht->jointsForTriMap = NIL; // jointsForTriMap[triIdx] = num of joints influencing this triangle?
            msht->triToJointCnt = 0;
            msht->triToJointMap = NIL; // triToJointMap[jntIdx] = num of triangles influenced by this joint?

            if (spec->src)
                AfxUpdateMeshIndices(msht, 0, triCnt, spec->src, spec->srcIdxSiz);

            // initialize cache
            {
                afxMeshTopologyCache* cache = &msht->cache;

                AfxResetLinkage(&cache->stream);

                cache->buf = NIL;
                cache->base = 0;
                cache->range = 0;
                cache->stride = 0;

                cache->msht = msht;
                //cache->idxSiz = 0;
            }

            if (err)
            {
                if (msht->sideToNeighborMap)
                    AfxDeallocate(mmu, msht->sideToNeighborMap);

                if (msht->vtxToTriMap)
                    AfxDeallocate(mmu, msht->vtxToTriMap);

                if (msht->vtxToVtxMap)
                    AfxDeallocate(mmu, msht->vtxToVtxMap);
            }

            if (err && surfaces)
                AfxDeallocate(mmu, surfaces);
        }

        if (err && tris)
            AfxDeallocate(mmu, tris);
    }
    
    if (!err)
        AfxEcho("Mesh topology %p built. With %u triangles (%u bytes per index) arranged in %u surfaces.", msht, msht->triCnt, AfxDetermineMeshIndexSize(msht), msht->surfCnt);

    return err;
}

_AKX afxClassSuballocation mshtSubs[] =
{
    { sizeof(AFX_OBJECT(afxMeshTopology)), 0, offsetof(AFX_OBJECT(afxMeshTopology), surfCnt), offsetof(AFX_OBJECT(afxMeshTopology), surfaces) },
    { sizeof(AFX_OBJECT(afxMeshTopology)), 0, offsetof(AFX_OBJECT(afxMeshTopology), triCnt), offsetof(AFX_OBJECT(afxMeshTopology), tris) },
};

_AKX afxClassConfig _AfxMshtClsConfig =
{
    .fcc = afxFcc_MSHT,
    .name = "Mesh Topology",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxMeshTopology)),
    .mmu = NIL,
    .ctor = (void*)_AfxMshtCtor,
    .dtor = (void*)_AfxMshtDtor
};

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

_AKX afxError AfxAcquireMeshTopology(afxSimulation sim, afxNat cnt, afxMeshTopologySpec const specs[], afxMeshTopology topologies[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AfxAssert(topologies);
    AfxAssert(specs);
    AfxAssert(cnt);
    
    if (AfxAcquireObjects(AwxGetMeshTopologyClass(sim), 1, (afxObject*)topologies, (void const*[]) { sim, specs }))
        AfxThrowError();

    return err;
}

_AKX afxMeshTopology AfxBuildMeshTopology(afxSimulation sim, afxMeshBuilder const* mshb, afxNat baseSurfIdx, afxNat surfCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AfxAssert(mshb);

    // count used triangles
    afxNat usedTriCnt = 0;

    for (afxNat i = 0; i < mshb->triCnt; i++)
    {
        afxNat triSurfIdx = mshb->faces[i].surfIdx;
        AfxAssertRange(mshb->surfCnt, triSurfIdx, 1);

        if ((triSurfIdx >= baseSurfIdx) && (triSurfIdx < baseSurfIdx + surfCnt))
            ++usedTriCnt;
    }

    afxMeshTopology msht = NIL;
    afxMeshTopologySpec spec = { 0 };
    spec.surCnt = surfCnt;
    spec.triCnt = usedTriCnt;
    spec.vtxCnt = 0;
    spec.src = NIL;
    spec.srcIdxSiz = 0;

    if (AfxAcquireMeshTopology(sim, 1, &spec, &msht)) AfxThrowError();
    else
    {
        //AfxUpdateMeshIndices(msht, 0, spec.triCnt, );
        afxIndexedTriangle* tris = msht->tris;
        afxNat baseTriIdx = 0;

        for (afxNat i = 0; i < surfCnt; i++)
        {
            afxMeshSurface* mshs = &msht->surfaces[i];
            //AfxAssignTypeFcc(mshs, afxFcc_MSHS);
            mshs->mtlIdx = i;

            afxNat surTriCnt = 0;

            for (afxNat j = 0; j < mshb->triCnt; j++)
            {
                afxNat triSurfIdx = mshb->faces[j].surfIdx;

                if (triSurfIdx == i)
                {
                    afxNat triIdx = baseTriIdx + surTriCnt;
                    tris[triIdx][0] = mshb->faces[j].vtxIdx[0];
                    tris[triIdx][1] = mshb->faces[j].vtxIdx[1];
                    tris[triIdx][2] = mshb->faces[j].vtxIdx[2];
                    ++surTriCnt;
                }
            }

            AfxAssertRange(usedTriCnt, baseTriIdx, surTriCnt - 1);
            mshs->baseTriIdx = baseTriIdx;
            mshs->triCnt = surTriCnt;
            baseTriIdx += surTriCnt;
            // name and aabb?
        }

        AfxAssert(baseTriIdx == usedTriCnt);
    }
    return msht;
}


_AKX afxError AfxDeserializeMeshTopologies(afxStream in, afxSimulation sim, afxNat cnt, afxMeshTopology dst[])
{
    afxError err = NIL;
    AfxAssertObjects(1, &in, afxFcc_IOS);
    AfxAssert(dst);
    AfxAssert(cnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        _afxSerializedChunk chnkHdr;
        AfxRead(in, 1, sizeof(chnkHdr), &chnkHdr, sizeof(chnkHdr));
        AfxAssertType(&chnkHdr, afxFcc_MSHT);

        _afxSerializedMsht chnk = { 0 };
        AfxRead(in, 1, sizeof(chnk), &chnk, sizeof(chnk));
        
        afxMeshTopologySpec spec = { 0 };
        spec.surCnt = chnk.surCnt;
        spec.triCnt = chnk.triCnt;

        AfxAcquireMeshTopology(sim, 1, &spec, &dst[i]);

        AfxRead(in, spec.surCnt, sizeof(dst[i]->surfaces[0]), dst[i]->surfaces, sizeof(dst[i]->surfaces[0]));

        AfxRead(in, spec.triCnt, sizeof(dst[i]->tris[0]), dst[i]->tris, sizeof(dst[i]->tris[0]));
    }

    return err;
}

_AKX afxError AfxSerializeMeshTopologies(afxStream out, afxNat cnt, afxMeshTopology const src[])
{
    afxError err = NIL;
    AfxAssertObjects(1, &out, afxFcc_IOS);
    AfxAssert(src);
    AfxAssert(cnt);

    for (afxNat i = 0; i < cnt; i++)
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
        AfxSkipStream(out, (afxInt)chnkHdr.len);
    }

    return err;
}
