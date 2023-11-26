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
#define _AFX_MESH_TOPOLOGY_C
#define _AFX_SIMULATION_C
#include "afx/sim/modeling/afxMeshTopology.h"
#include "afx/sim/afxSimulation.h"

_AFX afxError AfxBufferizeMeshTopology(afxMeshTopology msht)
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
        spec.usage = afxBufferUsage_INDEX;

        if (AfxAcquireBuffers(sim->dctx, 1, &cache->buf, &spec)) AfxThrowError();
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
            AfxAssertRange(AfxGetBufferSize(buf), cache->base, cache->range);
            //AfxUpdateBuffer2(cache->buf, cache->base, cache->range, cache->stride, msht->vtxIdx, sizeof(msht->vtxIdx[0]));

            afxBufferRegion rgn = { 0 };
            rgn.base = cache->base;
            rgn.range = cache->range;
            rgn.stride = cache->stride;
            
            rgn.offset = 0;
            rgn.unitSiz = cache->stride;
            AfxUpdateBufferRegion(cache->buf, &rgn, msht->tris, sizeof(msht->tris[0][0]));

            // debug
            void* p = AfxMapBufferRange(cache->buf, cache->base, cache->range, AFX_BUF_MAP_R);

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

_AFX afxNat AfxDetermineMeshIndexSize(afxMeshTopology msht)
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

_AFX afxIndexedTriangle* AfxGetMeshTriangles(afxMeshTopology msht, afxNat baseTriIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);
    AfxAssertRange(msht->triCnt, baseTriIdx, 1);
    return &msht->tris[baseTriIdx];
}

_AFX afxNat* AfxGetMeshIndices(afxMeshTopology msht, afxNat baseIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);
    AfxAssertRange(msht->triCnt, baseIdx / 3, 1);
    return &msht->tris[baseIdx / 3][baseIdx % 3];
}

_AFX afxNat AfxCountMeshIndices(afxMeshTopology msht)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);
    return msht->triCnt * 3;
}

_AFX afxNat AfxCountMeshTriangles(afxMeshTopology msht)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);
    return msht->triCnt;
}

_AFX afxNat AfxCountMeshTriangleEdges(afxMeshTopology msht)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);
    return msht->sideToNeighborCnt / 3;
}

_AFX void AfxGetMeshPrimitives(afxMeshTopology msht, afxNat surfIdx, afxNat* baseTriIdx, afxNat* triCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);
    AfxAssertRange(msht->surfCnt, surfIdx, 1);
    afxMeshSurface *mshs = &msht->surfaces[surfIdx];
    AfxAssertType(mshs, afxFcc_MSHS);

    if (baseTriIdx)
        *baseTriIdx = mshs->baseTriIdx;

    if (triCnt)
        *triCnt = mshs->triCnt;
}

_AFX afxBool AfxGetMeshCoverage(afxMeshTopology msht, afxNat surfIdx, afxNat* mtlIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);
    AfxAssertRange(msht->surfCnt, surfIdx, 1);
    afxMeshSurface *mshs = &msht->surfaces[surfIdx];
    AfxAssertType(mshs, afxFcc_MSHS);

    if (mtlIdx)
        *mtlIdx = mshs->mtlIdx;

    return (AFX_INVALID_INDEX != mshs->mtlIdx);
}

_AFX void AfxSetMeshCoverage(afxMeshTopology msht, afxNat surfIdx, afxNat mtlIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);
    AfxAssertRange(msht->surfCnt, surfIdx, 1);
    afxMeshSurface *mshs = &msht->surfaces[surfIdx];
    AfxAssertType(mshs, afxFcc_MSHS);
    mshs->mtlIdx = mtlIdx;
}

_AFX afxMeshSurface* AfxGetMeshSurface(afxMeshTopology msht, afxNat surfIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);
    AfxAssertRange(msht->surfCnt, surfIdx, 1);
    afxMeshSurface *mshs = &msht->surfaces[surfIdx];
    AfxAssertType(mshs, afxFcc_MSHS);
    return mshs;
}

_AFX afxNat AfxCountMeshSurfaces(afxMeshTopology msht)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);
    return msht->surfCnt;
}

_AFX void AfxInvertMeshWinding(afxMeshTopology msht)
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

    afxNat edgeCnt = msht->sideToNeighborCnt / 3;
    afxIndexedTriangle* neighbor = msht->sideToNeighborMap;

    for (afxNat i = 0; edgeCnt-- > 0; ++i)
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

_AFX void AfxRemapMeshCoverage(afxMeshTopology msht, afxNat remapCnt, afxNat const remapTable[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);

    for (afxNat i = msht->surfCnt; i-- > 0;)
    {
        afxMeshSurface* mshs = &msht->surfaces[i];
        AfxAssertType(mshs, afxFcc_MSHS);
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

_AFX afxError _AfxMshtDtor(afxMeshTopology msht)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);
    AfxEntry("msht=%p", msht);

    afxSimulation sim = AfxGetObjectProvider(msht);
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxContext mem = AfxSimulationGetMemory(sim);

    for (afxNat i = msht->surfCnt; i-- > 0;)
    {
        afxMeshSurface* mshs = &(msht->surfaces[i]);
        AfxAssertType(mshs, afxFcc_MSHS);

        //if (mshs->name)
        //AfxDeallocateString(&mshs->name);
    }

    if (msht->jointsForTriCnt)
        AfxDeallocate(mem, msht->jointsForTriMap);

    if (msht->triToJointCnt)
        AfxDeallocate(mem, msht->triToJointMap);

    if (msht->sideToNeighborCnt)
        AfxDeallocate(mem, msht->sideToNeighborMap);

    if (msht->vtxToTriCnt)
        AfxDeallocate(mem, msht->vtxToTriMap);

    if (msht->vtxToVtxCnt)
        AfxDeallocate(mem, msht->vtxToVtxMap);

    if (msht->triCnt)
        AfxDeallocate(mem, msht->tris);

    if (msht->surfCnt)
        AfxDeallocate(mem, msht->surfaces);

    return err;
}

_AFX afxError _AfxMshtCtor(afxMeshTopology msht, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);
    AfxEntry("msht=%p", msht);

    afxSimulation sim = cookie->udd[0];
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxMeshBuilder const *mshb = cookie->udd[1];
    AfxAssert(mshb);
    AfxAssertType(mshb, afxFcc_MSHB);
    afxNat baseSurfIdx = *(afxNat*)(cookie->udd[2]);
    afxNat surfCnt = *(afxNat*)(cookie->udd[3]);
    AfxAssertRange(mshb->surfCnt, baseSurfIdx, surfCnt);

    afxContext mem = AfxSimulationGetMemory(sim);

    afxNat triCnt = 0;// mshb->triCnt;

    for (afxNat i = 0; i < mshb->triCnt; i++)
    {
        afxNat triSurfIdx = mshb->faces[i].surfIdx;
        AfxAssertRange(mshb->surfCnt, triSurfIdx, 1);

        if ((triSurfIdx >= baseSurfIdx) && (triSurfIdx < baseSurfIdx + surfCnt))
            ++triCnt;
    }

    //afxNat surfCnt = mshb->surfCnt;
    msht->surfCnt = surfCnt;
    msht->surfaces = NIL;
    
    if (surfCnt && !(msht->surfaces = AfxAllocate(mem, sizeof(msht->surfaces[0]), surfCnt, 0, AfxHint()))) AfxThrowError();
    else
    {
        msht->tris = NIL;
        msht->triCnt = triCnt;

        if (triCnt && !(msht->tris = AfxAllocate(mem, sizeof(msht->tris[0]), triCnt, 0, AfxHint()))) AfxThrowError();
        else
        {
            afxIndexedTriangle* tris = msht->tris;

            msht->vtxToVtxCnt = 0; // vtxCnt;
            msht->vtxToVtxMap = NIL;
            
            if (msht->vtxToVtxCnt && !(msht->vtxToVtxMap = AfxAllocate(mem, sizeof(msht->vtxToVtxMap[0]), msht->vtxToVtxCnt, 0, AfxHint()))) AfxThrowError();
            else
            {
                msht->vtxToTriCnt = 0; // vtxCnt;
                msht->vtxToTriMap = NIL;

                if (msht->vtxToTriCnt && !(msht->vtxToTriMap = AfxAllocate(mem, sizeof(msht->vtxToTriMap[0]), msht->vtxToTriCnt, 0, AfxHint()))) AfxThrowError();
                else
                {
                    msht->sideToNeighborCnt = 0;// triCnt * 3 * int; // totalIdxCnt;
                    msht->sideToNeighborMap = NIL;

                    if (msht->sideToNeighborCnt && !(msht->sideToNeighborMap = AfxAllocate(mem, sizeof(msht->sideToNeighborMap[0]), msht->sideToNeighborCnt, 0, AfxHint()))) AfxThrowError();
                    else
                    {
                        msht->jointsForTriCnt = 0;
                        msht->jointsForTriMap = NIL; // jointsForTriMap[triIdx] = num of joints influencing this triangle?
                        msht->triToJointCnt = 0;
                        msht->triToJointMap = NIL; // triToJointMap[jntIdx] = num of triangles influenced by this joint?

                        afxNat baseTriIdx = 0;

                        for (afxNat i = 0; i < surfCnt; i++)
                        {
                            afxMeshSurface* mshs = &msht->surfaces[i];
                            AfxAssignTypeFcc(mshs, afxFcc_MSHS);                            
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

                            AfxAssertRange(triCnt, baseTriIdx, surTriCnt - 1);
                            mshs->baseTriIdx = baseTriIdx;
                            mshs->triCnt = surTriCnt;
                            baseTriIdx += surTriCnt;
                            // name and aabb?
                        }

                        AfxAssert(triCnt == baseTriIdx);

#if 0
                        if (mshb->GetCoincidentMap)
                            mshb->GetCoincidentMap(data, 0, vtxCnt, msht->vtxToVtxMap);

                        if (mshb->GetTriangleMap)
                            mshb->GetTriangleMap(data, 0, vtxCnt, msht->vtxToTriMap);

                        if (mshb->GetEdgeMap)
                            mshb->GetEdgeMap(data, 0, totalIdxCnt / 3, indices, msht->sideToNeighborMap);
#endif

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

                        if (err && msht->sideToNeighborMap)
                            AfxDeallocate(mem, msht->sideToNeighborMap);
                    }

                    if (err && msht->vtxToTriMap)
                        AfxDeallocate(mem, msht->vtxToTriMap);
                }

                if (err && msht->vtxToVtxMap)
                    AfxDeallocate(mem, msht->vtxToVtxMap);
            }

            if (err && msht->tris)
                AfxDeallocate(mem, msht->tris);
        }

        if (err && msht->surfaces)
            AfxDeallocate(mem, msht->surfaces);
    }
    
    if (!err)
        AfxEcho("Mesh topology %p built. With %u triangles (%u bytes per index) arranged in %u surfaces.", msht, msht->triCnt, AfxDetermineMeshIndexSize(msht), msht->surfCnt);

    return err;
}

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

_AFX afxMeshTopology AfxBuildMeshTopology(afxSimulation sim, afxMeshBuilder const* mshb, afxNat baseSurfIdx, afxNat surfCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AfxAssert(mshb);
    afxMeshTopology msht = NIL;

    if (AfxAcquireObjects(AfxGetMeshTopologyClass(sim), 1, (afxHandle*)&msht, (void*[]) { sim, (void*)mshb, &baseSurfIdx, &surfCnt }))
        AfxThrowError();

    return msht;
}

_AFX afxClassConfig _AfxMshtClsConfig =
{
    .fcc = afxFcc_MSHT,
    .name = "Mesh Topology",
    .unitsPerPage = 8,
    .size = sizeof(AFX_OBJECT(afxMeshTopology)),
    .ctx = NIL,
    .ctor = (void*)_AfxMshtCtor,
    .dtor = (void*)_AfxMshtDtor
};
