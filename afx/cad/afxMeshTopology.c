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
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#define _AVX_DRAW_C
#define _AVX_MESH_TOPOLOGY_C
#include "../dev/AvxImplKit.h"

_AVXINL afxNat AfxCountMeshSurfaces(afxMeshTopology msht, afxNat baseSurfIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);
    AfxAssertRange(msht->surfCnt, baseSurfIdx, 1);
    return msht->surfCnt - baseSurfIdx;
}

_AVXINL afxNat AfxCountMeshIndices(afxMeshTopology msht, afxNat baseIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);
    afxNat idxCnt = msht->triCnt * 3;
    AfxAssertRange(idxCnt, baseIdx, 1);
    return idxCnt - baseIdx;
}

_AVXINL afxNat AfxCountMeshTriangles(afxMeshTopology msht, afxNat baseTriIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);
    AfxAssertRange(msht->triCnt, baseTriIdx, 1);
    return msht->triCnt - baseTriIdx;
}

_AVXINL afxNat AfxCountMeshTriangleEdges(afxMeshTopology msht, afxNat baseEdgeIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);
    afxNat edgeCnt = msht->triCnt * 3;
    AfxAssertRange(edgeCnt, baseEdgeIdx, 1);
    return edgeCnt - baseEdgeIdx;
}

_AVXINL akxIndexedTriangle* AfxGetMeshTriangles(afxMeshTopology msht, afxNat baseTriIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);
    AfxAssertRange(msht->triCnt, baseTriIdx, 1);
    return &msht->tris[baseTriIdx];
}

_AVXINL afxNat* AfxGetMeshIndices(afxMeshTopology msht, afxNat baseIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);
    AfxAssertRange(msht->triCnt, baseIdx / 3, 1);
    return &msht->tris[baseIdx / 3][baseIdx % 3];
}

_AVX afxNat AfxDetermineMeshIndexSize(afxMeshTopology msht)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);

    if (!msht->minIdxSiz || !msht->maxIdxValue)
    {
        afxNat maxIdxValue = 0;

        for (afxNat i = msht->triCnt; i-- > 0;)
            for (afxNat j = 3; j-- > 0;)
                if (maxIdxValue < msht->tris[i][j])
                    maxIdxValue = msht->tris[i][j];

        msht->maxIdxValue = maxIdxValue;
        msht->minIdxSiz = maxIdxValue > AFX_N8_MAX ? (maxIdxValue > AFX_N16_MAX ? (maxIdxValue > AFX_N32_MAX ? sizeof(afxNat64) : sizeof(afxNat32)) : sizeof(afxNat16)) : sizeof(afxNat8);
    }
    return msht->minIdxSiz;
}

_AVXINL afxBool AfxDescribeMeshCoverage(afxMeshTopology msht, afxNat surIdx, akxMeshSurface* desc)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);
    AfxAssertRange(msht->surfCnt, surIdx, 1);
    akxMeshSurface *mshs = &msht->surfaces[surIdx];
    //AfxAssertType(mshs, afxFcc_MSHS);
    AfxAssert(desc);
    *desc = *mshs;
    return (AFX_INVALID_INDEX != mshs->mtlIdx);
}

_AVXINL akxMeshSurface* AfxGetMeshSurface(afxMeshTopology msht, afxNat surIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);
    AfxAssertRange(msht->surfCnt, surIdx, 1);
    akxMeshSurface *mshs = &msht->surfaces[surIdx];
    //AfxAssertType(mshs, afxFcc_MSHS);
    return mshs;
}

_AVX void AfxRemapMeshCoverage(afxMeshTopology msht, afxNat remapCnt, afxNat const remapTable[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);

    for (afxNat i = msht->surfCnt; i-- > 0;)
    {
        akxMeshSurface* mshs = &msht->surfaces[i];
        //AfxAssertType(mshs, afxFcc_MSHS);
        afxNat mtlIdx = mshs->mtlIdx;

        if (mtlIdx >= remapCnt)
        {
            AfxLogError("Mesh section had out-of-range index %u during remapping.", mtlIdx);
            AfxThrowError();
        }
        else
            mshs->mtlIdx = remapTable[mtlIdx];
    }
}

_AVX void AfxInvertMeshWinding(afxMeshTopology msht)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);

    afxNat triCnt = msht->triCnt;
    akxIndexedTriangle* tris = msht->tris;
    
    for (afxNat i = 0; i < triCnt; ++i)
    {
        afxNat tmp = tris[i][0];
        tris[i][0] = tris[i][2];
        tris[i][2] = tmp;
    }

    akxIndexedTriangle* adjacent = msht->sideToAdjacentMap;

    if (adjacent)
    {
        for (afxNat i = 0; i < triCnt; ++i)
        {
            afxNat tmp = adjacent[i][0];
            adjacent[i][0] = adjacent[i][1];
            adjacent[i][1] = tmp;

            afxNat32 faceA = adjacent[i][0] & 0xFFFFFFFC; // the high-order 30 bits specify the actual triangle index - you would multiply it by three to get its location
            afxNat32 edgeA = adjacent[i][0] & 0x00000003; // 00 would be the 0th edge, 01 the 1st, 10 the 2nd, and 11 is undefined
            adjacent[i][0] = (edgeA = (edgeA == 0x01 ? 0x00 : (edgeA == 0x00 ? 0x01 : edgeA))) | faceA;

            afxNat32 faceB = adjacent[i][1] & 0xFFFFFFFC; // the high-order 30 bits specify the actual triangle index - you would multiply it by three to get its location
            afxNat32 edgeB = adjacent[i][1] & 0x00000003; // 00 would be the 0th edge, 01 the 1st, 10 the 2nd, and 11 is undefined
            adjacent[i][1] = (edgeB = (edgeB == 0x01 ? 0x00 : (edgeB == 0x00 ? 0x01 : edgeB))) | faceB;

            afxNat32 faceC = adjacent[i][2] & 0xFFFFFFFC; // the high-order 30 bits specify the actual triangle index - you would multiply it by three to get its location
            afxNat32 edgeC = adjacent[i][2] & 0x00000003; // 00 would be the 0th edge, 01 the 1st, 10 the 2nd, and 11 is undefined
            adjacent[i][2] = (edgeC = (edgeC == 0x01 ? 0x00 : (edgeC == 0x00 ? 0x01 : edgeC))) | faceC;
        }
    }
}

_AVX afxError AfxUpdateMeshIndices(afxMeshTopology msht, afxNat baseTriIdx, afxNat triCnt, void const* src, afxNat srcIdxSiz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);
    AfxAssertRange(msht->triCnt, baseTriIdx, triCnt);
    
    //afxNat triCnt = msht->triCnt;
    akxIndexedTriangle* tris = AfxGetMeshTriangles(msht, baseTriIdx);
    
    switch (srcIdxSiz)
    {
    case sizeof(akxVertexIndex8):
    {
        akxIndexedTriangle8 const* src3 = src;

        for (afxNat i = 0; i < triCnt; i++)
            for (afxNat j = 0; j < 3; j++)
                tris[i][j] = src3[i][j];

        break;
    }
    case sizeof(akxVertexIndex16):
    {
        akxIndexedTriangle16 const* src3 = src;

        for (afxNat i = 0; i < triCnt; i++)
            for (afxNat j = 0; j < 3; j++)
                tris[i][j] = src3[i][j];
        
        break;
    }
    case sizeof(akxVertexIndex32):
    {
        akxIndexedTriangle32 const* src3 = src;

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

_AVX afxError _AvxMshtDtorCb(afxMeshTopology msht)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);
    
    for (afxNat i = msht->surfCnt; i-->0;)
    {
        akxMeshSurface* mshs = &(msht->surfaces[i]);
        //AfxAssertType(mshs, afxFcc_MSHS);

        //if (mshs->name)
        //AfxDeallocateString(&mshs->name);
    }

    if (msht->jointsForTriMap)
        AfxDeallocate(msht->jointsForTriMap);

    if (msht->triToJointMap)
        AfxDeallocate(msht->triToJointMap);

    if (msht->sideToAdjacentMap)
        AfxDeallocate(msht->sideToAdjacentMap);

    if (msht->vtxToTriMap)
        AfxDeallocate(msht->vtxToTriMap);

    if (msht->vtxToVtxMap)
        AfxDeallocate(msht->vtxToVtxMap);

    if (msht->tris)
        AfxDeallocate(msht->tris);

    if (msht->surfaces)
        AfxDeallocate(msht->surfaces);

    return err;
}

_AVX afxError _AvxMshtCtorCb(afxMeshTopology msht, void** args, afxNat invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);
    
    afxDrawInput din = args[0];
    AfxAssertObjects(1, &din, afxFcc_DIN);
    akxMeshTopologyBlueprint const* spec = args[1];
    spec += invokeNo;

    afxNat surfCnt = AfxMax(1, spec->surfCnt);
    AfxAssert(surfCnt);

    afxNat triCnt = spec->triCnt;
    AfxAssert(triCnt);

    afxNat vtxCnt = spec->vtxCnt;
    afxNat edgeCnt = triCnt * 3;

    akxIndexedTriangle* tris;

    if (!(tris = AfxAllocate(triCnt, sizeof(tris[0]), 0, AfxHere()))) AfxThrowError();
    else
    {
        akxMeshSurface* surfaces;

        if (!(surfaces = AfxAllocate(surfCnt, sizeof(surfaces[0]), 0, AfxHere()))) AfxThrowError();
        else
        {
            msht->triCnt = triCnt;
            msht->surfCnt = surfCnt;
            msht->vtxCnt = vtxCnt;

            msht->topology = avxTopology_TRI_LIST;

            msht->tris = tris;
            msht->surfaces = surfaces;
            afxNat surTriDiv = triCnt / surfCnt;
            afxNat baseTriIdx = 0;

            for (afxNat i = 0; i < surfCnt; i++)
            {
                akxMeshSurface* mshs = &surfaces[i];
                //AfxAssignFcc(mshs, afxFcc_MSHS);

                if (!spec->surfToMtlMap)
                    mshs->mtlIdx = AFX_INVALID_INDEX;
                else
                    mshs->mtlIdx = spec->surfToMtlMap[i];

                if (spec->trisForSurfMap)
                {
                    mshs->baseTriIdx = spec->baseTrisForSurfMap ? spec->baseTrisForSurfMap[i] : baseTriIdx;
                    mshs->triCnt = spec->trisForSurfMap[i];
                    baseTriIdx += mshs->triCnt;
                }
                else
                {
                    mshs->baseTriIdx = baseTriIdx;
                    mshs->triCnt = surTriDiv;
                    baseTriIdx += surTriDiv;
                }
            }

            msht->vtxToVtxMap = NIL; // vtxCnt
            msht->vtxToTriMap = NIL; // vtxCnt
            msht->sideToAdjacentMap = NIL; // triCnt * 3

            msht->jointsForTriCnt = 0;
            msht->jointsForTriMap = NIL; // jointsForTriMap[triIdx] = num of joints influencing this triangle?
            msht->triToJointCnt = 0;
            msht->triToJointMap = NIL; // triToJointMap[jntIdx] = num of triangles influenced by this joint?

            msht->maxIdxValue = 0;
            msht->minIdxSiz = 0;

            if (spec->src)
                AfxUpdateMeshIndices(msht, 0, triCnt, spec->src, spec->srcIdxSiz);

            // initialize cache
            {
                akxVertexIndexCache* cache = &msht->cache;

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
                if (msht->sideToAdjacentMap)
                    AfxDeallocate(msht->sideToAdjacentMap);

                if (msht->vtxToTriMap)
                    AfxDeallocate(msht->vtxToTriMap);

                if (msht->vtxToVtxMap)
                    AfxDeallocate(msht->vtxToVtxMap);
            }

            if (err && surfaces)
                AfxDeallocate(surfaces);
        }

        if (err && tris)
            AfxDeallocate(tris);
    }
    
    if (!err)
        AfxLogEcho("Mesh topology %p built. %u triangles (%u bytesPerIdx) and %u surfaces.", msht, msht->triCnt, AfxDetermineMeshIndexSize(msht), msht->surfCnt);

    return err;
}

_AVX afxClassSuballocation mshtSubs[] =
{
    { sizeof(AFX_OBJECT(afxMeshTopology)), 0, offsetof(AFX_OBJECT(afxMeshTopology), surfCnt), offsetof(AFX_OBJECT(afxMeshTopology), surfaces) },
    { sizeof(AFX_OBJECT(afxMeshTopology)), 0, offsetof(AFX_OBJECT(afxMeshTopology), triCnt), offsetof(AFX_OBJECT(afxMeshTopology), tris) },
};

_AVX afxClassConfig _AvxMshtClsCfg =
{
    .fcc = afxFcc_MSHT,
    .name = "MeshTopology",
    .desc = "Mesh Topology",
    .fixedSiz = sizeof(AFX_OBJECT(afxMeshTopology)),
    .ctor = (void*)_AvxMshtCtorCb,
    .dtor = (void*)_AvxMshtDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxAssembleMeshTopology(afxDrawInput din, afxNat cnt, akxMeshTopologyBlueprint const specs[], afxMeshTopology topologies[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(topologies);
    AfxAssert(specs);
    AfxAssert(cnt);
    
    afxClass* cls = (afxClass*)AfxGetMeshTopologyClass(din);
    AfxAssertClass(cls, afxFcc_MSHT);

    if (AfxAcquireObjects(cls, 1, (afxObject*)topologies, (void const*[]) { din, specs }))
        AfxThrowError();

    return err;
}

_AVX afxMeshTopology AfxBuildMeshTopology(afxDrawInput din, afxMeshBuilder const* mshb, afxNat baseSurfIdx, afxNat surfCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
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
    akxMeshTopologyBlueprint spec = { 0 };
    spec.surfCnt = surfCnt;
    spec.triCnt = usedTriCnt;
    spec.vtxCnt = 0;
    spec.src = NIL;
    spec.srcIdxSiz = 0;

    if (AfxAssembleMeshTopology(din, 1, &spec, &msht)) AfxThrowError();
    else
    {
        //AfxUpdateMeshIndices(msht, 0, spec.triCnt, );
        akxIndexedTriangle* tris = msht->tris;
        afxNat baseTriIdx = 0;

        for (afxNat i = 0; i < surfCnt; i++)
        {
            akxMeshSurface* mshs = &msht->surfaces[i];
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
