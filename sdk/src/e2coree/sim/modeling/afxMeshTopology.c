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

        afxBufferSpecification spec;
        spec.siz = msht->vtxIdxCnt * idxSiz;
        spec.src = msht->vtxIdx;
        spec.usage = afxBufferUsage_INDEX;

        if (AfxAcquireBuffers(sim->dctx, 1, &cache->buf, &spec)) AfxThrowError();
        else
        {
            AfxAssertObjects(1, &cache->buf, afxFcc_BUF);

            cache->base = 0;
            cache->range = spec.siz;
            cache->stride = cache->range / msht->vtxIdxCnt;

            cache->idxSiz = idxSiz;
        }

        afxBuffer buf;

        if ((buf = cache->buf))
        {
            AfxAssertObjects(1, &buf, afxFcc_BUF);

            AfxAssert(cache->stride);
            AfxAssertRange(AfxGetBufferSize(buf), cache->base, cache->range);
            //AfxUpdateBuffer2(cache->buf, cache->base, cache->range, cache->stride, msht->vtxIdx, sizeof(msht->vtxIdx[0]));

            afxBufferRegion rgn;
            rgn.base = cache->base;
            rgn.range = cache->range;
            rgn.stride = cache->stride;
            
            rgn.offset = 0;
            rgn.unitSiz = cache->idxSiz;
            AfxUpdateBufferRegion(cache->buf, &rgn, msht->vtxIdx, sizeof(msht->vtxIdx[0]));
        }
    }
    return err;
}

_AFX afxNat AfxDetermineMeshIndexSize(afxMeshTopology msht)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);
    return msht->vtxIdxCnt > AFX_N8_MAX ? (msht->vtxIdxCnt > AFX_N16_MAX ? (msht->vtxIdxCnt > AFX_N32_MAX ? sizeof(afxNat64) : sizeof(afxNat32)) : sizeof(afxNat16)) : sizeof(afxNat8);
}

_AFX afxNat* AfxGetMeshIndices(afxMeshTopology msht, afxNat baseIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);
    AfxAssertRange(msht->vtxIdxCnt, baseIdx, 1);
    return &msht->vtxIdx[baseIdx];
}

_AFX afxNat AfxCountMeshIndices(afxMeshTopology msht)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);
    return msht->vtxIdxCnt;
}

_AFX afxNat AfxCountMeshTriangles(afxMeshTopology msht)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);
    return AfxCountMeshIndices(msht) / 3;
}

_AFX void AfxGetMeshPrimitives(afxMeshTopology msht, afxNat surfaceIdx, afxNat* firstTriIdx, afxNat* triCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);
    AfxAssertRange(msht->surfaceCnt, surfaceIdx, 1);
    afxMeshSurface *mshs = &msht->surfaces[surfaceIdx];
    AfxAssertType(mshs, afxFcc_MSHS);

    if (firstTriIdx)
        *firstTriIdx = mshs->firstTriIdx;

    if (triCnt)
        *triCnt = mshs->triCnt;
}

_AFX afxBool AfxGetMeshCoverage(afxMeshTopology msht, afxNat surfaceIdx, afxNat* mtlIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);
    AfxAssertRange(msht->surfaceCnt, surfaceIdx, 1);
    afxMeshSurface *mshs = &msht->surfaces[surfaceIdx];
    AfxAssertType(mshs, afxFcc_MSHS);

    if (mtlIdx)
        *mtlIdx = mshs->mtlIdx;

    return (AFX_INVALID_INDEX != mshs->mtlIdx);
}

_AFX void AfxSetMeshCoverage(afxMeshTopology msht, afxNat surfaceIdx, afxNat mtlIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);
    AfxAssertRange(msht->surfaceCnt, surfaceIdx, 1);
    afxMeshSurface *mshs = &msht->surfaces[surfaceIdx];
    AfxAssertType(mshs, afxFcc_MSHS);
    mshs->mtlIdx = mtlIdx;
}

_AFX afxMeshSurface* AfxGetMeshSurface(afxMeshTopology msht, afxNat surfaceIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);
    AfxAssertRange(msht->surfaceCnt, surfaceIdx, 1);
    afxMeshSurface *mshs = &msht->surfaces[surfaceIdx];
    AfxAssertType(mshs, afxFcc_MSHS);
    return mshs;
}

_AFX afxNat AfxCountMeshSurfaces(afxMeshTopology msht)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);
    return msht->surfaceCnt;
}

_AFX void AfxInvertMeshWinding(afxMeshTopology msht)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);

    afxNat vtxIdxCnt = msht->vtxIdxCnt;
    afxNat* vtxIdx = msht->vtxIdx;
    
    for (afxNat i = 0, triCnt = vtxIdxCnt / 3; triCnt-- > 0; i += 3)
    {
        afxNat a = vtxIdx[i + 0];
        vtxIdx[i + 0] = vtxIdx[i + 2];
        vtxIdx[i + 2] = a;
    }

    afxNat sideToNeighborCnt = msht->sideToNeighborCnt;
    afxNat* neighbor = msht->sideToNeighborMap;

    for (afxNat i = 0, edgeCnt = sideToNeighborCnt / 3; edgeCnt-- > 0; i += 3)
    {
        afxNat a = vtxIdx[i + 0];
        vtxIdx[i + 0] = vtxIdx[i + 2];
        vtxIdx[i + 2] = a;

        afxNat aa = neighbor[i + 0];
        afxNat bb = neighbor[i + 1];
        neighbor[i + 0] = bb;
        neighbor[i + 1] = aa;

        afxNat32 faceA = neighbor[i + 0] & 0xFFFFFFFC; // the high-order 30 bits specify the actual triangle index - you would multiply it by three to get its location
        afxNat32 edgeA = neighbor[i + 0] & 0x00000003; // 00 would be the 0th edge, 01 the 1st, 10 the 2nd, and 11 is undefined
        neighbor[i + 0] = (edgeA = (edgeA == 0x01 ? 0x00 : (edgeA == 0x00 ? 0x01 : edgeA))) | faceA;

        afxNat32 faceB = neighbor[i + 1] & 0xFFFFFFFC; // the high-order 30 bits specify the actual triangle index - you would multiply it by three to get its location
        afxNat32 edgeB = neighbor[i + 1] & 0x00000003; // 00 would be the 0th edge, 01 the 1st, 10 the 2nd, and 11 is undefined
        neighbor[i + 1] = (edgeB = (edgeB == 0x01 ? 0x00 : (edgeB == 0x00 ? 0x01 : edgeB))) | faceB;

        afxNat32 faceC = neighbor[i + 2] & 0xFFFFFFFC; // the high-order 30 bits specify the actual triangle index - you would multiply it by three to get its location
        afxNat32 edgeC = neighbor[i + 2] & 0x00000003; // 00 would be the 0th edge, 01 the 1st, 10 the 2nd, and 11 is undefined
        neighbor[i + 2] = (edgeC = (edgeC == 0x01 ? 0x00 : (edgeC == 0x00 ? 0x01 : edgeC))) | faceC;
    }
}

_AFX void AfxRemapMeshCoverage(afxMeshTopology msht, afxNat remapCnt, afxNat const remapTable[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);

    for (afxNat i = msht->surfaceCnt; i-- > 0;)
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

    for (afxNat i = 0; i < msht->surfaceCnt; i++)
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

    if (msht->vtxIdxCnt)
        AfxDeallocate(mem, msht->vtxIdx);

    if (msht->surfaceCnt)
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
    void* data = cookie->udd[2] ? ((void **)(cookie->udd[2]))[cookie->no] : NIL;
    AfxAssert(data);

    afxContext mem = AfxSimulationGetMemory(sim);

    afxNat totalIdxCnt, surCnt;
    mshb->GetTopologyInfo(data, &totalIdxCnt, &surCnt);
    totalIdxCnt *= 3;

    msht->surfaces = NIL;
    msht->surfaceCnt = surCnt;

    if (msht->surfaceCnt && !(msht->surfaces = AfxAllocate(mem, msht->surfaceCnt * sizeof(msht->surfaces[0]), 0, AfxSpawnHint()))) AfxThrowError();
    else
    {
        msht->vtxIdx = NIL;
        msht->vtxIdxCnt = totalIdxCnt;

        if (msht->vtxIdxCnt && !(msht->vtxIdx = AfxAllocate(mem, msht->vtxIdxCnt * sizeof(msht->vtxIdx[0]), 0, AfxSpawnHint()))) AfxThrowError();
        else
        {
            msht->vtxToVtxCnt = 0; // vtxCnt;
            msht->vtxToVtxMap = NIL;
            
            if (msht->vtxToVtxCnt && !(msht->vtxToVtxMap = AfxAllocate(mem, msht->vtxToVtxCnt * sizeof(msht->vtxToVtxMap[0]), 0, AfxSpawnHint()))) AfxThrowError();
            else
            {
                msht->vtxToTriCnt = 0; // vtxCnt;
                msht->vtxToTriMap = NIL;

                if (msht->vtxToTriCnt && !(msht->vtxToTriMap = AfxAllocate(mem, msht->vtxToTriCnt * sizeof(msht->vtxToTriMap[0]), 0, AfxSpawnHint()))) AfxThrowError();
                else
                {
                    msht->sideToNeighborCnt = 0; // totalIdxCnt;
                    msht->sideToNeighborMap = NIL;

                    if (msht->sideToNeighborCnt && !(msht->sideToNeighborMap = AfxAllocate(mem, msht->sideToNeighborCnt * sizeof(msht->sideToNeighborMap[0]), 0, AfxSpawnHint()))) AfxThrowError();
                    else
                    {
                        msht->jointsForTriCnt = 0;
                        msht->jointsForTriMap = NIL; // jointsForTriMap[triIdx] = num of joints influencing this triangle?
                        msht->triToJointCnt = 0;
                        msht->triToJointMap = NIL; // triToJointMap[jntIdx] = num of triangles influenced by this joint?

                        afxNat* indices = msht->vtxIdx;

                        afxNat addedIdxCnt = 0;
                        afxNat surfaceCnt = 0;

                        for (afxNat i = 0; i < surCnt; i++)
                        {
                            afxMeshSurface* mshs = &msht->surfaces[surfaceCnt];
                            AfxAssignTypeFcc(mshs, afxFcc_MSHS);
                            mshs->firstTriIdx = addedIdxCnt;
                            AfxAssert(mshb->GetSurfaceData);
                            addedIdxCnt += (mshs->triCnt = mshb->GetSurfaceData(data, i, &mshs->mtlIdx, &indices[addedIdxCnt]));
                            mshs->firstTriIdx /= 3;
                            mshs->triCnt /= 3;
                            ++surfaceCnt;
                            // name and aabb?
                        }

                        AfxAssert(surfaceCnt == surCnt);
                        AfxAssert(totalIdxCnt == addedIdxCnt);

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
                            cache->idxSiz = 0;
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

            if (err && msht->vtxIdx)
                AfxDeallocate(mem, msht->vtxIdx);
        }

        if (err && msht->surfaces)
            AfxDeallocate(mem, msht->surfaces);
    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxBuildMeshTopologies(afxSimulation sim, afxMeshBuilder const* mshb, afxNat cnt, void *data[], afxMeshTopology msht[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AfxAssert(mshb);

    if (AfxAcquireObjects(AfxGetMeshTopologyClass(sim), cnt, (afxHandle*)msht, (void*[]) { sim, (void*)mshb, data }))
        AfxThrowError();

    return err;
}

_AFX afxClassConfig _AfxMshtClsConfig =
{
    .fcc = afxFcc_MSHT,
    .name = "Mesh Topology",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxMeshTopology)),
    .ctx = NIL,
    .ctor = (void*)_AfxMshtCtor,
    .dtor = (void*)_AfxMshtDtor
};
