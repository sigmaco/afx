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
#define _AVX_MESH_C
#define _AVX_MESH_TOPOLOGY_C
#include "../dev/AvxImplKit.h"

////////////////////////////////////////////////////////////////////////////////

_AVX afxBool AfxGetMeshUrn(afxMesh msh, afxString* id)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxAssert(id);
    AfxReflectString(&msh->urn, id);
    return msh->urn.len;
}

_AVX afxNat AfxCountMeshMorphs(afxMesh msh, afxNat baseMorphIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxAssertRange(msh->morphCnt, baseMorphIdx, 1);
    return msh->morphCnt - baseMorphIdx;
}

_AVX afxNat AfxCountMeshVertices(afxMesh msh, afxNat morphIdx, afxNat baseVtxIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxAssertRange(msh->morphCnt, morphIdx, 1);
    AfxAssert(msh->morphs);
    afxMeshMorph* mshm = &msh->morphs[morphIdx];
    AfxAssertRange(mshm->vtxCnt, baseVtxIdx, 1);
    return mshm->vtxCnt - baseVtxIdx;
}

_AVX afxNat AfxCountMeshBiases(afxMesh msh, afxNat baseBiasIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxAssertRange(msh->biasCnt, baseBiasIdx, 1);
    return msh->biasCnt - baseBiasIdx;
}

_AVX afxBool AfxMeshIsDeformable(afxMesh msh)
{
    // The vertices in a Qwadro mesh are either deforming or rigid. 
    // Deforming meshes are ones that are bound to multiple bones, whereas rigid meshes are ones that are bound to a single bone (and thus move "rigidly" with that bone). 

    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    return (1 < AfxCountMeshBiases(msh, 0));
}

_AVX avxTopology AfxGetMeshTopology(afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    return msh->topology;
}

_AVX afxMeshMorph* AfxGetMeshMorphs(afxMesh msh, afxNat baseMorphIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxAssertRange(msh->morphCnt, baseMorphIdx, 1);
    afxMeshMorph* mshm = &msh->morphs[baseMorphIdx];
    //AfxAssertType(mshm, afxFcc_MSHM);
    return mshm;
}

_AVX afxMeshBias* AfxGetMeshBiases(afxMesh msh, afxNat baseBiasIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxAssertRange(msh->biasCnt, baseBiasIdx, 1);
    afxMeshBias* mshv = &msh->biases[baseBiasIdx];
    //AfxAssertType(mshv, afxFcc_MSHV);
    return mshv;
}

_AVX afxString* AfxGetMeshBiasTags(afxMesh msh, afxNat baseBiasIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxAssertRange(msh->biasCnt, baseBiasIdx, 1);
    return &msh->biasTagMap[baseBiasIdx];
}

_AVXINL afxNat AfxCountMeshSurfaces(afxMesh msh, afxNat baseSurfIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxAssertRange(msh->surfCnt, baseSurfIdx, 1);
    return msh->surfCnt - baseSurfIdx;
}

_AVXINL afxNat AfxCountMeshIndices(afxMesh msh, afxNat baseIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    afxNat idxCnt = msh->triCnt * 3;
    AfxAssertRange(idxCnt, baseIdx, 1);
    return idxCnt - baseIdx;
}

_AVXINL afxNat AfxCountMeshTriangles(afxMesh msh, afxNat baseTriIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxAssertRange(msh->triCnt, baseTriIdx, 1);
    return msh->triCnt - baseTriIdx;
}

_AVXINL afxNat AfxCountMeshTriangleEdges(afxMesh msh, afxNat baseEdgeIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    afxNat edgeCnt = msh->triCnt * 3;
    AfxAssertRange(edgeCnt, baseEdgeIdx, 1);
    return edgeCnt - baseEdgeIdx;
}

_AVXINL afxIndexedTriangle* AfxGetMeshTriangles(afxMesh msh, afxNat baseTriIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxAssertRange(msh->triCnt, baseTriIdx, 1);
    return &msh->tris[baseTriIdx];
}

_AVXINL afxNat* AfxGetMeshIndices(afxMesh msh, afxNat baseIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxAssertRange(msh->triCnt, baseIdx / 3, 1);
    return &msh->tris[baseIdx / 3][baseIdx % 3];
}

_AVX afxNat AfxDetermineMeshIndexSize(afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);

    if (!msh->minIdxSiz || !msh->maxIdxValue)
    {
        afxNat maxIdxValue = 0;

        for (afxNat i = msh->triCnt; i-- > 0;)
            for (afxNat j = 3; j-- > 0;)
                if (maxIdxValue < msh->tris[i][j])
                    maxIdxValue = msh->tris[i][j];

        msh->maxIdxValue = maxIdxValue;
        msh->minIdxSiz = maxIdxValue > AFX_N8_MAX ? (maxIdxValue > AFX_N16_MAX ? (maxIdxValue > AFX_N32_MAX ? sizeof(afxNat64) : sizeof(afxNat32)) : sizeof(afxNat16)) : sizeof(afxNat8);
    }
    return msh->minIdxSiz;
}

_AVXINL afxBool AfxDescribeMeshCoverage(afxMesh msh, afxNat surIdx, afxMeshSurface* desc)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxAssertRange(msh->surfCnt, surIdx, 1);
    afxMeshSurface *mshs = &msh->surfaces[surIdx];
    //AfxAssertType(mshs, afxFcc_MSHS);
    AfxAssert(desc);
    *desc = *mshs;
    return (AFX_INVALID_INDEX != mshs->mtlIdx);
}

_AVXINL afxMeshSurface* AfxGetMeshSurface(afxMesh msh, afxNat surIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxAssertRange(msh->surfCnt, surIdx, 1);
    afxMeshSurface *mshs = &msh->surfaces[surIdx];
    //AfxAssertType(mshs, afxFcc_MSHS);
    return mshs;
}

_AVX void AfxRemapMeshCoverage(afxMesh msh, afxNat remapCnt, afxNat const remapTable[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);

    for (afxNat i = msh->surfCnt; i-- > 0;)
    {
        afxMeshSurface* mshs = &msh->surfaces[i];
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

_AVX void AfxInvertMeshWinding(afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);

    afxNat triCnt = msh->triCnt;
    afxIndexedTriangle* tris = msh->tris;
    
    for (afxNat i = 0; i < triCnt; ++i)
    {
        afxNat tmp = tris[i][0];
        tris[i][0] = tris[i][2];
        tris[i][2] = tmp;
    }

    afxIndexedTriangle* adjacent = msh->sideToAdjacentMap;

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

_AVX afxError AfxUpdateMeshIndices(afxMesh msh, afxNat baseTriIdx, afxNat triCnt, void const* src, afxNat srcIdxSiz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxAssertRange(msh->triCnt, baseTriIdx, triCnt);
    
    //afxNat triCnt = msh->triCnt;
    afxIndexedTriangle* tris = AfxGetMeshTriangles(msh, baseTriIdx);
    
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
    default:
        AfxThrowError();
    }

    if (!err)
    {
        afxNat surCnt = AfxCountMeshSurfaces(msh, 0);
        afxNat triCnt = AfxCountMeshTriangles(msh, 0);
        afxIndexedTriangle* tris = AfxGetMeshTriangles(msh, 0);

        if (!baseTriIdx)
            AfxResetBoxes(surCnt, msh->surfAabbMap);

        afxGeometry geo;
        afxVertexFormat fmt;
        afxNat baseVtx, vtxCnt;
        AfxGetMeshVertices(msh, 0, &geo, &baseVtx, &vtxCnt);
        AkxGetVertexAttributeInfo(geo, 0, &fmt, NIL, NIL);
        void* data = AfxExposeGeometry(geo, 0, baseVtx);

        for (afxNat i = 0; i < triCnt; i++)
        {
            AfxAssertRange(vtxCnt, tris[i][0], 1);
            AfxAssertRange(vtxCnt, tris[i][1], 1);
            AfxAssertRange(vtxCnt, tris[i][2], 1);

            if ((vtxCnt < tris[i][0]) || (vtxCnt < tris[i][1]) || (vtxCnt < tris[i][2]))
            {
                AfxThrowError();
            }
        }

        if (fmt == afxVertexFormat_V3D)
        {
            for (afxNat j = 0; j < surCnt; j++)
            {
                afxMeshSurface* mshs = AfxGetMeshSurface(msh, j);
                afxV3d* data3 = data;

                for (afxNat i = 0; i < mshs->triCnt; i++)
                {
                    for (afxNat k = 0; k < 3; k++)
                        AfxAabbAbsorbAtv3d(msh->surfAabbMap[j], 1, &data3[tris[mshs->baseTriIdx + i][k]]);
                }
            }
        }
        else if (fmt == afxVertexFormat_V4D)
        {
            for (afxNat j = 0; j < surCnt; j++)
            {
                afxMeshSurface* mshs = AfxGetMeshSurface(msh, j);
                afxV4d* data3 = data;

                for (afxNat i = 0; i < mshs->triCnt; i++)
                {
                    for (afxNat k = 0; k < 3; k++)
                        AfxAabbAbsorbAtv4d(msh->surfAabbMap[j], 1, &data3[tris[mshs->baseTriIdx + i][k]]);
                }
            }
        }
        else AfxThrowError();
    }
    return err;
}

_AVX afxBool AfxGetMeshVertices(afxMesh msh, afxNat morphIdx, afxGeometry* data, afxNat* baseVtx, afxNat* vtxCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxAssertRange(msh->morphCnt, morphIdx, 1);
    afxGeometry geo = NIL;

    if (morphIdx < msh->morphCnt)
    {
        afxMeshMorph const* mshm = &msh->morphs[morphIdx];
        geo = mshm->geo;
        AfxAssertObjects(1, &geo, afxFcc_VTD);
        AfxAssert(data);
        *data = geo;
        *baseVtx = mshm->baseVtx;
        *vtxCnt = mshm->vtxCnt;
    }
    return !!geo;
}

_AVX afxError _AvxMshDtorCb(afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    
    if (msh->morphs)
    {
        afxNat morphCnt = msh->morphCnt;

        for (afxNat i = 0; i < morphCnt; i++)
        {
            afxMeshMorph* morph = &msh->morphs[i];
            AfxAssertObjects(1, &morph->geo, afxFcc_VTD);
            AfxReleaseObjects(1, &morph->geo);
        }
    }

    afxObjectStash stashes[] =
    {
        {
            .cnt = msh->triCnt,
            .siz = sizeof(msh->tris[0]),
            .var = (void**)&msh->tris
        },
        {
            .cnt = msh->surfCnt,
            .siz = sizeof(msh->surfaces[0]),
            .var = (void**)&msh->surfaces
        },
        {
            .cnt = msh->surfCnt,
            .siz = sizeof(msh->surfAabbMap[0]),
            .align = AFX_SIMD_ALIGN,
            .var = (void**)&msh->surfAabbMap
        },
        {
            .cnt = msh->sideToAdjacentMap ? msh->vtxCnt : 0,
            .siz = sizeof(msh->sideToAdjacentMap[0]),
            .var = (void**)&msh->sideToAdjacentMap
        },
        {
            .cnt = msh->biasCnt,
            .siz = sizeof(msh->biases[0]),
            .align = AFX_SIMD_ALIGN,
            .var = (void**)&msh->biases
        },
        {
            .cnt = msh->biasCnt,
            .siz = sizeof(msh->biasTagMap[0]),
            .align = AFX_SIMD_ALIGN,
            .var = (void**)&msh->biasTagMap
        },
        {
            .cnt = msh->biasCnt,
            .siz = sizeof(msh->biasObbMap[0]),
            .align = AFX_SIMD_ALIGN,
            .var = (void**)&msh->biasObbMap
        },
        {
            .cnt = msh->morphCnt,
            .siz = sizeof(msh->morphs[0]),
            .var = (void**)&msh->morphs
        },
        {
            .cnt = msh->morphCnt - 1,
            .siz = sizeof(msh->extraMorphTagMap[0]),
            .align = AFX_SIMD_ALIGN,
            .var = (void**)&msh->extraMorphTagMap
        },
        {
            .cnt = msh->vtxToVtxMap ? msh->vtxCnt : 0,
            .siz = sizeof(msh->vtxToVtxMap[0]),
            .var = (void**)&msh->vtxToVtxMap
        },
        {
            .cnt = msh->vtxToTriMap ? msh->vtxCnt : 0,
            .siz = sizeof(msh->vtxToTriMap[0]),
            .var = (void**)&msh->vtxToTriMap
        },
    };

    if (AfxDeallocateInstanceData(msh, AFX_COUNTOF(stashes), stashes))
        AfxThrowError();

    AfxAssert(!msh->morphs);
    AfxAssert(!msh->extraMorphTagMap);
    AfxAssert(!msh->biases);
    AfxAssert(!msh->biasTagMap);
    AfxAssert(!msh->biasObbMap);

    return err;
}

_AVX afxError _AvxMshCtorCb(afxMesh msh, void** args, afxNat invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);

    afxDrawInput din = args[0];
    AfxAssertObjects(1, &din, afxFcc_DIN);
    afxMeshBlueprint const* blueprint = args[1];

    if (!AfxCatalogStrings(AfxGetMorphTagStringBase(din), 1, &blueprint->urn.str, &msh->urn))
        AfxThrowError();

    afxNat surfCnt = AfxMax(1, blueprint->surfCnt);
    AfxAssert(surfCnt);

    afxNat triCnt = blueprint->triCnt;
    AfxAssert(triCnt);

    afxNat vtxCnt = blueprint->vtxCnt;
    afxNat edgeCnt = triCnt * 3;

    afxNat extraMorphCnt = blueprint->extraMorphCnt;
    afxNat morphCnt = 1 + extraMorphCnt;
    afxNat biasCnt = AfxMax(1, blueprint->biasCnt);

    msh->morphs = NIL;
    msh->extraMorphTagMap = NIL;
    msh->biases = NIL;
    msh->biasTagMap = NIL;
    msh->biasObbMap = NIL;
    msh->surfAabbMap = NIL;
    msh->topology = avxTopology_TRI_LIST;

    afxObjectStash stashes[] =
    {
        {
            .cnt = triCnt,
            .siz = sizeof(msh->tris[0]),
            .var = (void**)&msh->tris
        },
        {
            .cnt = surfCnt,
            .siz = sizeof(msh->surfaces[0]),
            .var = (void**)&msh->surfaces
        },
        {
            .cnt = surfCnt,
            .siz = sizeof(msh->surfAabbMap[0]),
            .align = AFX_SIMD_ALIGN,
            .var = (void**)&msh->surfAabbMap
        },
        {
            .cnt = blueprint->sideToAdjacentMap ? vtxCnt : 0,
            .siz = sizeof(msh->sideToAdjacentMap[0]),
            .var = (void**)&msh->sideToAdjacentMap
        },
        {
            .cnt = biasCnt,
            .siz = sizeof(msh->biases[0]),
            .align = AFX_SIMD_ALIGN,
            .var = (void**)&msh->biases
        },
        {
            .cnt = biasCnt,
            .siz = sizeof(msh->biasTagMap[0]),
            .align = AFX_SIMD_ALIGN,
            .var = (void**)&msh->biasTagMap
        },
        {
            .cnt = biasCnt,
            .siz = sizeof(msh->biasObbMap[0]),
            .align = AFX_SIMD_ALIGN,
            .var = (void**)&msh->biasObbMap
        },
        {
            .cnt = morphCnt,
            .siz = sizeof(msh->morphs[0]),
            .var = (void**)&msh->morphs
        },
        {
            .cnt = extraMorphCnt,
            .siz = sizeof(msh->extraMorphTagMap[0]),
            .align = AFX_SIMD_ALIGN,
            .var = (void**)&msh->extraMorphTagMap
        },
        {
            .cnt = blueprint->vtxToVtxMap ? vtxCnt : 0,
            .siz = sizeof(msh->vtxToVtxMap[0]),
            .var = (void**)&msh->vtxToVtxMap
        },
        {
            .cnt = blueprint->vtxToTriMap ? vtxCnt : 0,
            .siz = sizeof(msh->vtxToTriMap[0]),
            .var = (void**)&msh->vtxToTriMap
        },
    };

    if (AfxAllocateInstanceData(msh, AFX_COUNTOF(stashes), stashes)) AfxThrowError();
    else
    {
        AfxAssert(msh->morphs);
        AfxAssert(!extraMorphCnt || msh->extraMorphTagMap);
        AfxAssert(!biasCnt || msh->biases);
        AfxAssert(!biasCnt || msh->biasTagMap);
        AfxAssert(!biasCnt || msh->biasObbMap);

        for (afxNat i = 0; i < morphCnt; i++)
        {
            afxMeshMorph const* src;

            if (i == 0)
            {
                if (!blueprint->geo)
                {
                    AfxThrowError();
                    break;
                }

                afxMeshMorph const firstMorph =
                {
                    .geo = blueprint->geo,
                    .baseVtx = blueprint->baseVtx,
                    .vtxCnt = blueprint->vtxCnt,
                    .delta = FALSE
                };
                src = &firstMorph;
            }
            else if (blueprint->extraMorphs) src = &blueprint->extraMorphs[i];
            else src = (afxMeshMorph const[]) { 0 };

            afxGeometry geo = src->geo;
            AfxAssertObjects(1, &geo, afxFcc_VTD);
            AfxReacquireObjects(1, &geo);
            afxNat maxVtxCnt = geo ? AfxCountVertices(geo) : 0;
            AfxAssertRange(maxVtxCnt, src->baseVtx, src->vtxCnt);
            afxNat vtxCnt = src->vtxCnt ? src->vtxCnt : maxVtxCnt - src->baseVtx;

            afxMeshMorph* morph = &msh->morphs[i];
            morph->geo = geo;
            morph->baseVtx = src->baseVtx;
            morph->vtxCnt = vtxCnt;
            morph->delta = !!src->delta;
        }

        msh->morphCnt = morphCnt;

        if (extraMorphCnt)
        {
            if (!blueprint->extraMorphTagMap) for (afxNat i = 0; i < extraMorphCnt; i++)
                AfxResetString(&msh->extraMorphTagMap[i]);
            else if (extraMorphCnt != (afxNat)AfxCatalogStrings(AfxGetMorphTagStringBase(din), extraMorphCnt, blueprint->extraMorphTagMap, msh->extraMorphTagMap))
                AfxThrowError();
        }

        if (!err)
        {
            afxIndexedTriangle* tris = msh->tris;
            afxMeshSurface* surfaces = msh->surfaces;

            msh->triCnt = triCnt;
            msh->surfCnt = surfCnt;
            msh->vtxCnt = vtxCnt;

            msh->topology = avxTopology_TRI_LIST;

            msh->tris = tris;
            msh->surfaces = surfaces;
            afxNat surTriDiv = triCnt / surfCnt;
            afxNat baseTriIdx = 0;

            for (afxNat i = 0; i < surfCnt; i++)
            {
                afxMeshSurface* mshs = &surfaces[i];
                //AfxAssignFcc(mshs, afxFcc_MSHS);

                if (!blueprint->surfToMtlMap)
                    mshs->mtlIdx = AFX_INVALID_INDEX;
                else
                    mshs->mtlIdx = blueprint->surfToMtlMap[i];

                if (blueprint->trisForSurfMap)
                {
                    mshs->baseTriIdx = blueprint->baseTrisForSurfMap ? blueprint->baseTrisForSurfMap[i] : baseTriIdx;
                    mshs->triCnt = blueprint->trisForSurfMap[i];
                    baseTriIdx += mshs->triCnt;
                }
                else
                {
                    mshs->baseTriIdx = baseTriIdx;
                    mshs->triCnt = surTriDiv;
                    baseTriIdx += surTriDiv;
                }
            }

            msh->vtxToVtxMap = NIL; // vtxCnt
            msh->vtxToTriMap = NIL; // vtxCnt
            msh->sideToAdjacentMap = NIL; // triCnt * 3

            msh->jointsForTriCnt = 0;
            msh->jointsForTriMap = NIL; // jointsForTriMap[triIdx] = num of joints influencing this triangle?
            msh->triToJointCnt = 0;
            msh->triToJointMap = NIL; // triToJointMap[jntIdx] = num of triangles influenced by this joint?

            msh->maxIdxValue = 0;
            msh->minIdxSiz = 0;

            // initialize cache
            {
                afxVertexIndexCache* cache = &msh->cache;

                AfxResetLinkage(&cache->stream);

                cache->buf = NIL;
                cache->base = 0;
                cache->range = 0;
                cache->stride = 0;

                cache->msh = msh;
                //cache->idxSiz = 0;
            }

            msh->biasCnt = biasCnt;

            AfxAssert(biasCnt < 2 || blueprint->biases);

            if (biasCnt)
            {
                if (!blueprint->biases) for (afxNat i = 0; i < biasCnt; i++)
                    AfxResetString(&msh->biasTagMap[i]);
                else if (biasCnt != (afxNat)AfxCatalogStrings(AfxGetPivotTagStringBase(din), biasCnt, blueprint->biases, msh->biasTagMap))
                    AfxThrowError();

                for (afxNat i = 0; i < biasCnt; i++)
                {
                    afxMeshBias* bias = &msh->biases[i];
                    AfxBoxReset(bias->obb);
                    bias->triCnt = 0;
                    bias->tris = NIL;
                }

                AfxResetBoxes(biasCnt, msh->biasObbMap);
            }

            if (!err)
            {
                msh->extData = NIL;
            }
        }

        if (err)
        {
            AfxAssert(msh->morphs);

            if (msh->morphs)
            {
                for (afxNat i = 0; i < morphCnt; i++)
                {
                    afxMeshMorph* morph = &msh->morphs[i];
                    AfxAssertObjects(1, &morph->geo, afxFcc_VTD);
                    AfxReleaseObjects(1, &morph->geo);
                }
            }
        }

        if (err)
        {
            if (AfxDeallocateInstanceData(msh, AFX_COUNTOF(stashes), stashes))
                AfxThrowError();
        }
    }

    // EXHIBIT STATISTICAS

    if (!err)
    {
        afxString s;
        AfxGetMeshUrn(msh, &s);

        afxNat vtxAttrCnt = AkxCountVertexAttributes(msh->morphs[0].geo);
        afxNat mshtSurCnt = AfxCountMeshSurfaces(msh, 0);
        afxNat mshtTriCnt = AfxCountMeshSurfaces(msh, 0);

        AfxLogEcho("%.*s Mesh <%.*s> assembled at %p.\n    %u vertices with %u attributes.\n    %u triangles (%u bytes per index) arranged in %u surfaces.\n    Listing %u biases..:",
            AfxPushString(msh->biasCnt > 1 ? &AfxStaticString("Skinned") : &AfxStaticString("Rigid")),
            AfxPushString(&s), msh, msh->morphs[0].vtxCnt, vtxAttrCnt, mshtTriCnt, AfxDetermineMeshIndexSize(msh), mshtSurCnt, msh->biasCnt
        );

        for (afxNat i = 0; i < msh->biasCnt; i++)
        {
            s = msh->biasTagMap[i];
            AfxLogEcho("    %3u <%.*s> %3u", i, AfxPushString(&s), msh->biases ? msh->biases[i].triCnt : 0);
        }
    }
    return err;
}

_AVX afxClassConfig _AvxMshClsCfg =
{
    .fcc = afxFcc_MSH,
    .name = "Mesh",
    .desc = "Rigid/Deformable Indexed Triangle Mesh",
    .fixedSiz = sizeof(AFX_OBJECT(afxMesh)),
    .ctor = (void*)_AvxMshCtorCb,
    .dtor = (void*)_AvxMshDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxNat AfxEnumerateMeshes(afxDrawInput din, afxNat base, afxNat cnt, afxMesh meshes[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    afxClass const* cls = AfxGetMeshClass(din);
    AfxAssertClass(cls, afxFcc_MSH);
    return AfxEnumerateClassInstances(cls, base, cnt, (afxObject*)meshes);
}

_AVX afxError AfxAssembleMeshes(afxDrawInput din, afxNat cnt, afxMeshBlueprint const blueprints[], afxMesh meshes[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(blueprints);
    AfxAssert(meshes);
    AfxAssert(cnt);

    afxClass* cls = (afxClass*)AfxGetMeshClass(din);
    AfxAssertClass(cls, afxFcc_MSH);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)meshes, (void const*[]) { din, blueprints }))
        AfxThrowError();

    return err;
}

_AVX afxError AfxBuildMeshes(afxDrawInput din, afxNat cnt, afxMeshBuilder const builders[], afxMesh meshes[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(cnt);
    AfxAssert(builders);
    AfxAssert(meshes);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxMeshBuilder const* bld = &builders[i];
        afxGeometry geo = AfxBuildGeometry(din, bld);
        
        afxMeshBlueprint blueprint = { 0 };
        blueprint.geo = geo;
        blueprint.biasCnt = bld->artCnt;
        blueprint.biases = bld->pivots;
        AfxMakeString32(&blueprint.urn, &bld->id.str);

        // count used triangles
        afxNat usedTriCnt = 0;
        afxNat baseSurfIdx = 0;

        for (afxNat i = 0; i < bld->triCnt; i++)
        {
            afxNat triSurfIdx = bld->faces[i].surfIdx;
            AfxAssertRange(bld->surfCnt, triSurfIdx, 1);

            if ((triSurfIdx >= baseSurfIdx) && (triSurfIdx < baseSurfIdx + bld->surfCnt))
                ++usedTriCnt;
        }

        blueprint.surfCnt = bld->surfCnt;
        blueprint.triCnt = usedTriCnt;
        blueprint.vtxCnt = 0;

        if (AfxAssembleMeshes(din, 1, &blueprint, &meshes[i]))
        {
            AfxThrowError();
            AfxReleaseObjects(i, (void**)meshes);
            break;
        }

        //AfxUpdateMeshIndices(msht, 0, spec.triCnt, );
        afxIndexedTriangle* tris = meshes[i]->tris;
        afxNat baseTriIdx = 0;

        for (afxNat k = 0; k < bld->surfCnt; k++)
        {
            afxMeshSurface* mshs = &meshes[i]->surfaces[k];
            //AfxAssignTypeFcc(mshs, afxFcc_MSHS);
            mshs->mtlIdx = k;

            afxNat surTriCnt = 0;

            for (afxNat j = 0; j < bld->triCnt; j++)
            {
                afxNat triSurfIdx = bld->faces[j].surfIdx;

                if (triSurfIdx == k)
                {
                    afxNat triIdx = baseTriIdx + surTriCnt;
                    tris[triIdx][0] = bld->faces[j].vtxIdx[0];
                    tris[triIdx][1] = bld->faces[j].vtxIdx[1];
                    tris[triIdx][2] = bld->faces[j].vtxIdx[2];
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

        AfxReleaseObjects(1, (void*[]) { geo });
    }

    return err;
}

_AVX void AfxTransformMeshes(afxM3d const ltm, afxM3d const iltm, afxReal ltTol, afxV3d const atv, afxReal atTol, afxFlags flags, afxNat cnt, afxMesh meshes[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(ltm);
    AfxAssert(iltm);
    AfxAssert(atv);
    AfxAssert(cnt);
    AfxAssert(meshes);
    //AfxThrowError();
    (void)atTol;
    (void)ltTol;
    afxMesh msh;

    for (afxNat mshIdx = 0; mshIdx < cnt; mshIdx++)
    {
        if ((msh = meshes[mshIdx]))
        {
            AfxAssertObjects(1, &msh, afxFcc_MSH);

            afxGeometry geo;
            afxNat baseVtx, vtxCnt;
            AfxGetMeshVertices(msh, 0, &geo, &baseVtx, &vtxCnt);
            AfxTryAssertObjects(1, &geo, afxFcc_VTD);
            
            AfxTransformGeometries(ltm, iltm, atv, (flags & 1), 1, &geo); // sinalizado para renormalizar normals

            afxNat morphCnt = AfxCountMeshMorphs(msh, 0);

            for (afxNat j = 0; j < morphCnt; j++)
            {
                afxMeshMorph* mshm = AfxGetMeshMorphs(msh, j);
                geo = mshm->geo;
                AfxTryAssertObjects(1, &geo, afxFcc_VTD);
                AfxAssert(!mshm->delta || AkxGetVertexAttributeFlags(geo, 0) & afxVertexFlag_DELTA);

                if (mshm->delta)
                    AfxTransformGeometries(ltm, iltm, atv, FALSE, 1, &geo);
                else
                    AfxTransformGeometries(ltm, iltm, atv, (flags & 1), 1, &geo);
            }

            if ((flags & 2) && (AfxM3dDet(ltm) < 0.0)) // se sinalizado para reordenar índices de faces
                AfxInvertMeshWinding(msh);

            afxNat biasCnt = AfxCountMeshBiases(msh, 0);

#if 0
            for (afxNat j = 0; j < biasCnt; j++)
            {
                afxMeshBias* mshv = AfxGetMeshBiases(msh2, j);
                AfxTransformObbs(ltm, atv, 1, &mshv->aabb, &mshv->aabb);
            }
#endif
        }
    }
}

_AVX void AfxRenormalizeMeshes(afxNat cnt, afxMesh meshes[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(meshes);
    AfxAssert(cnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxMesh msh2 = meshes[i];
        AfxAssertObjects(1, &msh2, afxFcc_MSH);
        afxGeometry geo = msh2->morphs[0].geo;
        
        afxString const attrs[] =
        {
            AfxString("posn"),
            AfxString("nrm")
        };
        afxNat attrIdx[2];
        AkxFindVertexAttributes(geo, 2, attrs, attrIdx);
        afxV4d* posn = AfxExposeGeometry(geo, attrIdx[0], 0);
        afxV3d* nrm = AfxExposeGeometry(geo, attrIdx[1], 0);

        afxIndexedTriangle const* tris = AfxGetMeshTriangles(msh2, 0);
        afxNat triCnt = AfxCountMeshTriangles(msh2, 0);
        afxNat vtxCnt = AfxCountMeshVertices(msh2, 0, 0);

        afxM4d zero = { 0 };
        AfxUpdateGeometry(geo, attrIdx[1], 0, vtxCnt, zero, 0);

        for (afxNat j = 0; j < triCnt; j++)
        {
            afxNat const ia = tris[j][0];
            afxNat const ib = tris[j][1];
            afxNat const ic = tris[j][2];

            afxV4d e1, e2, no;
            AfxV4dSub(e1, posn[ia], posn[ib]);
            AfxV4dSub(e2, posn[ic], posn[ib]);
            AfxV3dCross(no, e1, e2);

            AfxV3dAdd(nrm[ia], nrm[ia], no);
            AfxV3dAdd(nrm[ib], nrm[ib], no);
            AfxV3dAdd(nrm[ic], nrm[ic], no);
        }

        AfxNormalizeGeometry(geo, attrIdx[1], 0, vtxCnt);
    }
}
