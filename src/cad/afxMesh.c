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

_AVX afxDrawInput AfxGetMeshDrawInput(afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    afxDrawInput din = AfxGetProvider(msh);
    AfxAssertObjects(1, &din, afxFcc_DIN);
    return din;
}

_AVXINL avxTopology AfxGetMeshTopology(afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    return msh->topology;
}

_AVX afxBool AfxGetMeshUrn(afxMesh msh, afxString* id)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AFX_ASSERT(id);
    AfxReflectString(&msh->urn, id);
    return msh->urn.len;
}

_AVXINL afxBool AfxIsMeshDeformable(afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    return (1 < msh->biasCnt);
}

_AVXINL afxUnit* AfxGetMeshIndices(afxMesh msh, afxUnit baseTriIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AFX_ASSERT_RANGE(msh->triCnt, baseTriIdx, 1);

    // sanitize arguments
    baseTriIdx = AfxMin(baseTriIdx, msh->triCnt - 1);

    return &msh->indices[baseTriIdx * AMX_INDICES_PER_TRI];
}

_AVX void AfxDescribeMesh(afxMesh msh, afxMeshInfo* info)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AFX_ASSERT(info);
    info->topology = msh->topology;
    info->biasCnt = msh->biasCnt;
    info->triCnt = msh->triCnt;
    info->idxCnt = msh->idxCnt;
    info->edgeCnt = msh->triCnt * AMX_INDICES_PER_TRI;
    info->morphCnt = msh->morphCnt;
    info->secCnt = msh->secCnt;
    info->minIdxSiz = msh->minIdxSiz;
    AfxMakeString32(&info->urn, &msh->urn);
}

_AVX afxUnit AfxGetMeshMorphes(afxMesh msh, afxUnit baseMorphIdx, afxUnit cnt, afxMeshMorph morphes[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AFX_ASSERT_RANGE(msh->morphCnt, baseMorphIdx, cnt);
    
    // sanitize arguments
    baseMorphIdx = AfxMin(baseMorphIdx, msh->morphCnt - 1);
    afxUnit rslt = AfxMin(cnt, msh->morphCnt - baseMorphIdx);

    for (afxUnit i = 0; i < rslt; i++)
        morphes[i] = msh->morphs[baseMorphIdx + i];

    return rslt;
}

_AVX afxError AfxChangeMeshMorphes(afxMesh msh, afxUnit baseMorphIdx, afxUnit cnt, afxMeshMorph const morphes[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AFX_ASSERT_RANGE(msh->morphCnt, baseMorphIdx, cnt);
    AFX_ASSERT(morphes);

    // sanitize arguments
    baseMorphIdx = AfxMin(baseMorphIdx, msh->morphCnt - 1);
    afxUnit rslt = AfxMin(cnt, msh->morphCnt - baseMorphIdx);

    for (afxUnit i = 0; i < rslt; i++)
    {
        afxUnit morphIdx = baseMorphIdx + i;
        afxMeshMorph* mshm = &msh->morphs[morphIdx];
        
        if (mshm->geo != morphes[i].geo)
        {
            AfxReleaseObjects(1, &mshm->geo);
            AfxReacquireObjects(1, &morphes[i].geo);
        }
        
        *mshm = morphes[i];

        AfxRecomputeMeshBounds(msh, morphIdx, 0, msh->secCnt, 0);
    }
    return err;
}

_AVX afxUnit AfxGetMeshBiases(afxMesh msh, afxUnit baseBiasIdx, afxUnit cnt, afxMeshBias biases[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AFX_ASSERT_RANGE(msh->biasCnt, baseBiasIdx, cnt);
    
    // sanitize arguments
    baseBiasIdx = AfxMin(baseBiasIdx, msh->biasCnt - 1);
    afxUnit rslt = AfxMin(cnt, msh->biasCnt - baseBiasIdx);

    for (afxUnit i = 0; i < rslt; i++)
        biases[i] = msh->biases[baseBiasIdx + i];

    return rslt;
}

_AVX afxString* AfxGetMeshBiasTags(afxMesh msh, afxUnit baseBiasIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AFX_ASSERT_RANGE(msh->biasCnt, baseBiasIdx, 1);

    // sanitize arguments
    baseBiasIdx = AfxMin(baseBiasIdx, msh->biasCnt - 1);

    return &msh->biasTagMap[baseBiasIdx];
}

_AVX afxUnit AfxGetMeshSections(afxMesh msh, afxUnit baseSecIdx, afxUnit cnt, afxMeshSection sections[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AFX_ASSERT_RANGE(msh->secCnt, baseSecIdx, cnt);

    // sanitize arguments
    baseSecIdx = AfxMin(baseSecIdx, msh->secCnt - 1);
    afxUnit rslt = AfxMin(cnt, msh->secCnt - baseSecIdx);

    for (afxUnit i = 0; i < rslt; i++)
        sections[i] = msh->sections[baseSecIdx + i];

    return rslt;
}

_AVX afxUnit AfxRemapMeshCoverage(afxMesh msh, afxUnit mtlIdxCnt, afxUnit const mtlIdxLut[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AFX_ASSERT(mtlIdxLut);
    afxUnit rslt = 0;

    for (afxUnit i = 0; i < msh->secCnt; i++)
    {
        afxMeshSection mshs;
        AfxGetMeshSections(msh, i, 1, &mshs);

        if (mshs.mtlIdx < mtlIdxCnt)
        {
            mshs.mtlIdx = mtlIdxLut[mshs.mtlIdx];
            ++rslt;
        }
    }
    return rslt;
}

_AVX void AfxInvertMeshTopology(afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);

    afxUnit triCnt = msh->triCnt;
    afxUnit* indices = AfxGetMeshIndices(msh, 0);
    
    for (afxUnit i = 0; i < triCnt; i++)
    {
        afxUnit idx = indices[i * AMX_INDICES_PER_TRI + 0];
        indices[i * AMX_INDICES_PER_TRI + 0] = indices[i * AMX_INDICES_PER_TRI + 2];
        indices[i * AMX_INDICES_PER_TRI + 2] = idx;
    }

    afxUnit* adjacent = msh->sideToAdjacentMap;

    if (adjacent)
    {
        afxUnit edgeCnt = msh->idxCnt / AMX_INDICES_PER_TRI;

        for (afxUnit i = 0; i < edgeCnt; ++i)
        {
            afxUnit tmp = adjacent[i + 0];
            adjacent[i + 0] = adjacent[i + 1];
            adjacent[i + 1] = tmp;

            afxUnit32 faceA = adjacent[i + 0] & 0xFFFFFFFC; // the high-order 30 bits specify the actual triangle index - you would multiply it by three to get its location
            afxUnit32 edgeA = adjacent[i + 0] & 0x00000003; // 00 would be the 0th edge, 01 the 1st, 10 the 2nd, and 11 is undefined
            
            afxUnit32 faceB = adjacent[i + 1] & 0xFFFFFFFC; // the high-order 30 bits specify the actual triangle index - you would multiply it by three to get its location
            afxUnit32 edgeB = adjacent[i + 1] & 0x00000003; // 00 would be the 0th edge, 01 the 1st, 10 the 2nd, and 11 is undefined
            
            afxUnit32 faceC = adjacent[i + 2] & 0xFFFFFFFC; // the high-order 30 bits specify the actual triangle index - you would multiply it by three to get its location
            afxUnit32 edgeC = adjacent[i + 2] & 0x00000003; // 00 would be the 0th edge, 01 the 1st, 10 the 2nd, and 11 is undefined
            
            adjacent[i + 0] = (edgeA = (edgeA == 0x01 ? 0x00 : (edgeA == 0x00 ? 0x01 : edgeA))) | faceA;
            adjacent[i + 1] = (edgeB = (edgeB == 0x01 ? 0x00 : (edgeB == 0x00 ? 0x01 : edgeB))) | faceB;
            adjacent[i + 2] = (edgeC = (edgeC == 0x01 ? 0x00 : (edgeC == 0x00 ? 0x01 : edgeC))) | faceC;
        }
    }
}

_AVX afxError AfxRecomputeMeshBounds(afxMesh msh, afxUnit morphIdx, afxUnit baseSecIdx, afxUnit cnt, afxUnit posAttrIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AFX_ASSERT_RANGE(msh->morphCnt, morphIdx, 1);
    AFX_ASSERT_RANGE(msh->secCnt, baseSecIdx, cnt);

    // sanitize arguments
    morphIdx = AfxMin(morphIdx, msh->morphCnt - 1);
    baseSecIdx = AfxMin(baseSecIdx, msh->secCnt - 1);
    cnt = AfxMin(cnt, msh->secCnt - baseSecIdx);

    afxMeshMorph mshm;
    if (!AfxGetMeshMorphes(msh, morphIdx, 1, &mshm))
    {
        AfxThrowError();
        return err;
    }

    afxBox* morphSecAabbs = &msh->morphBasedSecAabb[morphIdx * msh->secCnt + baseSecIdx];
    AfxResetBoxes(cnt, morphSecAabbs);

    afxGeometryRow pos;
    AfxDescribeGeometryRow(mshm.geo, posAttrIdx, &pos);
    void* data = AfxAccessGeometry(mshm.geo, 0, mshm.baseVtx);

    afxUnit triCnt = msh->triCnt;
    afxUnit secCnt = msh->secCnt;

    if (pos.fmt == afxVertexFormat_V3D)
    {
        afxV3d* data3 = data;

        for (afxUnit j = 0; j < cnt; j++)
        {
            afxUnit secIdx = baseSecIdx + j;

            afxMeshSection mshs;
            AfxGetMeshSections(msh, secIdx, 1, &mshs);
            afxUnit const* indices = AfxGetMeshIndices(msh, mshs.baseTriIdx);
            afxUnit idxCnt = mshs.triCnt * AMX_INDICES_PER_TRI;

            for (afxUnit i = 0; i < idxCnt; i++)
                AfxAabbAbsorbAtv3d(&morphSecAabbs[j], 1, &data3[i]);
        }
    }
    else if (pos.fmt == afxVertexFormat_V4D)
    {
        afxV4d* data4 = data;

        for (afxUnit j = 0; j < cnt; j++)
        {
            afxUnit secIdx = baseSecIdx + j;

            afxMeshSection mshs;
            AfxGetMeshSections(msh, secIdx, 1, &mshs);
            afxUnit const* indices = AfxGetMeshIndices(msh, mshs.baseTriIdx);
            afxUnit idxCnt = mshs.triCnt * AMX_INDICES_PER_TRI;

            for (afxUnit i = 0; i < idxCnt; i++)
                AfxAabbAbsorbAtv4d(&morphSecAabbs[j], 1, &data4[i]);
        }
    }
    else AfxThrowError();

    return err;
}

_AVX afxError AfxUpdateMeshBounds(afxMesh msh, afxUnit morphIdx, afxUnit baseSecIdx, afxUnit cnt, afxBox const aabbs[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AFX_ASSERT_RANGE(msh->morphCnt, morphIdx, 1);
    AFX_ASSERT_RANGE(msh->secCnt, baseSecIdx, cnt);
    AFX_ASSERT(aabbs);

    // sanitize arguments
    morphIdx = AfxMin(morphIdx, msh->morphCnt - 1);
    baseSecIdx = AfxMin(baseSecIdx, msh->secCnt - 1);
    cnt = AfxMin(cnt, msh->secCnt - baseSecIdx);

    afxMeshMorph mshm;
    if (!AfxGetMeshMorphes(msh, morphIdx, 1, &mshm))
    {
        AfxThrowError();
        return err;
    }

    // Recompute it if <aabbs> is not specified.

    afxBox* morphSecAabbs = &msh->morphBasedSecAabb[morphIdx * msh->secCnt + baseSecIdx];
    AfxCopyBoxes(cnt, aabbs, morphSecAabbs);

    return err;
}

// UPDATE/DUMP INDEXES /////////////////////////////////////////////////////////

_AVX afxError AfxUpdateMeshIndices(afxMesh msh, afxUnit baseTriIdx, afxUnit triCnt, void const* src, afxUnit srcIdxSiz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AFX_ASSERT_RANGE(msh->triCnt, baseTriIdx, triCnt);

    // sanitize arguments
    baseTriIdx = AfxMin(baseTriIdx, msh->triCnt - 1);
    triCnt = AfxMin(triCnt, msh->triCnt - baseTriIdx);

    afxUnit idxCnt = triCnt * AMX_INDICES_PER_TRI;
    afxUnit* indices = AfxGetMeshIndices(msh, baseTriIdx);
    afxIndex8 const* src8 = src;
    afxIndex16 const* src16 = src;
    afxIndex32 const* src32 = src;

    switch (srcIdxSiz)
    {
    case sizeof(afxUnit8):  for (afxUnit i = 0; i < idxCnt; i++) indices[i] =  src8[i]; break;
    case sizeof(afxUnit16): for (afxUnit i = 0; i < idxCnt; i++) indices[i] = src16[i]; break;
    case sizeof(afxUnit32): for (afxUnit i = 0; i < idxCnt; i++) indices[i] = src32[i]; break;
    default: AfxThrowError(); break;
    }

    if (!err)
    {
#if _AFX_DEBUG
        for (afxUnit morphIdx = 0; morphIdx < msh->morphCnt; morphIdx++)
        {
            afxMeshMorph mshm;
            AfxGetMeshMorphes(msh, morphIdx, 1, &mshm);
            for (afxUnit i = 0; i < idxCnt; i++)
            {
                AFX_ASSERT_RANGE(mshm.vtxCnt, indices[i], 1);
            }
        }
#endif
        for (afxUnit secIdx = 0; secIdx < msh->secCnt; secIdx++)
        {
            afxMeshSection mshs;
            AfxGetMeshSections(msh, secIdx, 1, &mshs);

            if (baseTriIdx >= mshs.baseTriIdx && baseTriIdx + triCnt <= mshs.baseTriIdx + mshs.triCnt)
                for (afxUnit morphIdx = 0; morphIdx < msh->morphCnt; morphIdx++)
                    AfxRecomputeMeshBounds(msh, morphIdx, secIdx, 1, 0);
        }
    }
    return err;
}

_AVX afxError AfxDumpMeshIndices(afxMesh msh, afxUnit baseTriIdx, afxUnit triCnt, void* dst, afxUnit dstIdxSiz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AFX_ASSERT_RANGE(msh->triCnt, baseTriIdx, triCnt);

    // sanitize arguments
    baseTriIdx = AfxMin(baseTriIdx, msh->triCnt - 1);
    triCnt = AfxMin(triCnt, msh->triCnt - baseTriIdx);

    afxUnit const* indices = AfxGetMeshIndices(msh, baseTriIdx);

    AFX_ASSERT(dst);
    AFX_ASSERT(dstIdxSiz);
    AfxStream2(AMX_INDICES_PER_TRI * triCnt, indices, sizeof(indices[0]), dst, dstIdxSiz);

    return err;
}

// UPLOAD/DOWNLOAD INDICES /////////////////////////////////////////////////////

_AVX afxError AfxUploadMeshIndices(afxMesh msh, afxUnit baseTriIdx, afxUnit triCnt, afxStream in, afxUnit srcIdxSiz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxAssertObjects(1, &in, afxFcc_IOB);
    AFX_ASSERT_RANGE(msh->triCnt, baseTriIdx, triCnt);

    // sanitize arguments
    baseTriIdx = AfxMin(baseTriIdx, msh->triCnt - 1);
    triCnt = AfxMin(triCnt, msh->triCnt - baseTriIdx);

    afxUnit idxCnt = triCnt * AMX_INDICES_PER_TRI;
    afxUnit* indices = AfxGetMeshIndices(msh, baseTriIdx);

    if (AfxReadStream2(in, srcIdxSiz * idxCnt, srcIdxSiz, indices, sizeof(indices[0])))
        AfxThrowError();

    if (!err)
    {
#if _AFX_DEBUG
        afxMeshMorph mshm;
        AfxGetMeshMorphes(msh, 0, 1, &mshm);

        for (afxUnit i = 0; i < idxCnt; i++)
        {
            AFX_ASSERT_RANGE(mshm.vtxCnt, indices[i], 1);

            if (mshm.vtxCnt < indices[i])
                AfxThrowError();
        }
#endif
        for (afxUnit i = 0; i < msh->secCnt; i++)
        {
            afxMeshSection mshs;
            AfxGetMeshSections(msh, i, 1, &mshs);

            if (mshs.baseTriIdx >= baseTriIdx && triCnt >= mshs.triCnt)
                for (afxUnit j = 0; j < msh->morphCnt; j++)
                    AfxUpdateMeshBounds(msh, j, i, 1, NIL);
        }
    }
    return err;
}

_AVX afxError AfxDownloadMeshIndices(afxMesh msh, afxUnit baseTriIdx, afxUnit triCnt, afxStream out, afxUnit dstIdxSiz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxAssertObjects(1, &out, afxFcc_IOB);
    AFX_ASSERT_RANGE(msh->triCnt, baseTriIdx, triCnt);

    // sanitize arguments
    baseTriIdx = AfxMin(baseTriIdx, msh->triCnt - 1);
    triCnt = AfxMin(triCnt, msh->triCnt - baseTriIdx);

    afxUnit idxCnt = triCnt * AMX_INDICES_PER_TRI;
    afxUnit* indices = AfxGetMeshIndices(msh, baseTriIdx);

    if (AfxWriteStream2(out, dstIdxSiz * idxCnt, dstIdxSiz, indices, sizeof(indices[0])))
        AfxThrowError();

    return err;
}

_AVX afxError AfxRecomputeMeshNormals(afxMesh msh, afxUnit morphIdx, afxUnit posAttrIdx, afxUnit nrmAttrIdx)
{
    afxError err = NIL;
    AfxAssertObjects(1, &msh, afxFcc_MSH);

    afxMeshInfo mshi;
    AfxDescribeMesh(msh, &mshi);

    afxMeshMorph mshm;
    AfxGetMeshMorphes(msh, morphIdx, 1, &mshm);

    if (!mshm.geo) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &mshm.geo, afxFcc_GEO);

        /*
            For each face on the mesh, calc the face normal (without normalization, as just explained), and directly accumulate this normal in each vertex belonging to the face.
            After you are done with the faces, each vertex will have recieved all the face normals it was supposed to recieve. That simple.
        */

        afxUnit const* indices = AfxGetMeshIndices(msh, 0);

        afxV3d const* pos = AfxAccessGeometry(mshm.geo, posAttrIdx, mshm.baseVtx);
        afxV3d* nrm = AfxAccessGeometry(mshm.geo, nrmAttrIdx, mshm.baseVtx);

        AfxUpdateGeometry(mshm.geo, nrmAttrIdx, mshm.baseVtx, mshm.vtxCnt, AFX_V3D_ZERO, 0); // zero it

        AfxRecomputeTriangularNormals(mshi.triCnt, indices, pos, nrm);
    }
    return err;
}

_AVX afxError AfxRecomputeMeshTangents(afxMesh msh, afxUnit morphIdx, afxUnit posAttrIdx, afxUnit uvAttrIdx, afxUnit tanAttrIdx, afxUnit bitAttrIdx)
{
    afxError err = NIL;
    AfxAssertObjects(1, &msh, afxFcc_MSH);

    afxMeshInfo mshi;
    AfxDescribeMesh(msh, &mshi);

    afxMeshMorph mshm;
    AfxGetMeshMorphes(msh, morphIdx, 1, &mshm);

    if (!mshm.geo) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &mshm.geo, afxFcc_GEO);

        afxUnit const* indices = AfxGetMeshIndices(msh, 0);

        afxV3d const* pos = AfxAccessGeometry(mshm.geo, posAttrIdx, mshm.baseVtx);
        afxV2d const* uv = AfxAccessGeometry(mshm.geo, uvAttrIdx, mshm.baseVtx);

        afxV3d* tan = AfxAccessGeometry(mshm.geo, tanAttrIdx, mshm.baseVtx);
        afxV3d* bit = AfxAccessGeometry(mshm.geo, bitAttrIdx, mshm.baseVtx);

        AfxRecomputeTriangularTangents(mshi.triCnt, indices, pos, uv, 3, tan, bit);
    }
    return err;
}

_AVX afxError AfxBufferizeMesh(afxMesh msh, afxUnit morphIdx, avxVertexCache* vtxCache, avxIndexCache* idxCache)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);

    afxMeshMorph mshm;
    AfxGetMeshMorphes(msh, morphIdx, 1, &mshm);
    AfxBufferizeGeometry(mshm.geo, vtxCache);

    avxIndexCache* cache = &msh->cache;

    if (!cache->buf)
    {
        afxDrawContext dctx;
        afxDrawInput din = AfxGetMeshDrawInput(msh);
        AfxGetDrawInputContext(din, &dctx);

        afxMeshInfo mshi;
        AfxDescribeMesh(msh, &mshi);

        afxBufferInfo bufi;
        bufi.bufCap = mshi.idxCnt * mshi.minIdxSiz;
        bufi.flags = afxBufferFlag_W;
        bufi.usage = afxBufferUsage_INDEX;
        if (AfxAcquireBuffers(dctx, 1, &bufi, &cache->buf)) AfxThrowError();
        else
        {
            AfxAssertObjects(1, &cache->buf, afxFcc_BUF);

            cache->base = 0;
            cache->range = bufi.bufCap;
            cache->stride = cache->range / mshi.idxCnt;
            AFX_ASSERT(cache->stride == mshi.minIdxSiz);
            //cache->idxSiz = idxSiz;
        }

        afxBuffer buf;

        if ((buf = cache->buf))
        {
            AfxAssertObjects(1, &buf, afxFcc_BUF);

            AFX_ASSERT(cache->stride);
            AFX_ASSERT_RANGE(AfxGetBufferCapacity(buf, 0), cache->base, cache->range);
            //AfxUpdateBuffer2(cache->buf, cache->base, cache->range, cache->stride, msht->vtxIdx, sizeof(msht->vtxIdx[0]));

            afxUnit* indices = AfxGetMeshIndices(msh, 0);

            afxBufferIo iop = { 0 };
            iop.dstOffset = cache->base;
            iop.dstStride = cache->stride;
            iop.srcStride = sizeof(indices[0]);
            iop.rowCnt = mshi.idxCnt;
            AfxUpdateBuffer(cache->buf, 1, &iop, indices, 0);

#if 0//_AFX_DEBUG
            void* p;
            AfxWaitForDrawBridge(dctx, 0, 0);
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
        *idxCache = *cache;

    return err;
}

_AVX afxError _AvxMshDtorCb(afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    
    if (msh->morphs)
    {
        afxUnit morphCnt = msh->morphCnt;

        for (afxUnit i = 0; i < morphCnt; i++)
        {
            afxMeshMorph* morph = &msh->morphs[i];
            AfxAssertObjects(1, &morph->geo, afxFcc_GEO);
            AfxReleaseObjects(1, &morph->geo);
        }
    }

    afxObjectStash stashes[] =
    {
        {
            .cnt = msh->idxCnt,
            .siz = sizeof(msh->indices[0]),
            .var = (void**)&msh->indices
        },
        {
            .cnt = msh->secCnt,
            .siz = sizeof(msh->sections[0]),
            .var = (void**)&msh->sections
        },
        {
            .cnt = msh->secCnt * msh->morphCnt,
            .siz = sizeof(msh->morphBasedSecAabb[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&msh->morphBasedSecAabb
        },
        {
            .cnt = msh->sideToAdjacentMap ? msh->vtxCnt : 0,
            .siz = sizeof(msh->sideToAdjacentMap[0]),
            .var = (void**)&msh->sideToAdjacentMap
        },
        {
            .cnt = msh->biasCnt,
            .siz = sizeof(msh->biases[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&msh->biases
        },
        {
            .cnt = msh->biasCnt,
            .siz = sizeof(msh->biasTagMap[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&msh->biasTagMap
        },
        {
            .cnt = msh->biasCnt * msh->morphCnt,
            .siz = sizeof(msh->morphBasedBiasObb[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&msh->morphBasedBiasObb
        },
        {
            .cnt = msh->morphCnt,
            .siz = sizeof(msh->morphs[0]),
            .var = (void**)&msh->morphs
        },
        {
            .cnt = msh->morphCnt - 1,
            .siz = sizeof(msh->extraMorphTagMap[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&msh->extraMorphTagMap
        },
        {
            .cnt = msh->vtxToTriMap ? msh->vtxCnt : 0,
            .siz = sizeof(msh->vtxToTriMap[0]),
            .var = (void**)&msh->vtxToTriMap
        },
    };

    if (AfxDeallocateInstanceData(msh, ARRAY_SIZE(stashes), stashes))
        AfxThrowError();

    AFX_ASSERT(!msh->morphs);
    AFX_ASSERT(!msh->extraMorphTagMap);
    AFX_ASSERT(!msh->biases);
    AFX_ASSERT(!msh->biasTagMap);
    AFX_ASSERT(!msh->morphBasedBiasObb);

    return err;
}

_AVX afxError _AvxMshCtorCb(afxMesh msh, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);

    afxDrawInput din = args[0];
    AfxAssertObjects(1, &din, afxFcc_DIN);
    afxMeshBlueprint const* blueprint = AFX_CAST(afxMeshBlueprint const*, args[1]) + invokeNo;

    if (!AfxCatalogStrings(AfxGetMorphTagStringBase(din), 1, &blueprint->urn.str, &msh->urn))
        AfxThrowError();

    afxUnit secCnt = AfxMax(1, blueprint->secCnt);
    AFX_ASSERT(secCnt);

    afxUnit triCnt = blueprint->triCnt;
    AFX_ASSERT(triCnt);

    afxUnit vtxCnt = blueprint->vtxCnt;
    afxUnit edgeCnt = triCnt * AMX_INDICES_PER_TRI;
    afxUnit idxCnt = triCnt * AMX_INDICES_PER_TRI;

    afxUnit extraMorphCnt = blueprint->extraMorphCnt;
    afxUnit morphCnt = 1 + extraMorphCnt;
    afxUnit biasCnt = AfxMax(1, blueprint->biasCnt);

    msh->morphs = NIL;
    msh->extraMorphTagMap = NIL;
    msh->biases = NIL;
    msh->biasTagMap = NIL;
    msh->morphBasedBiasObb = NIL;
    msh->morphBasedSecAabb = NIL;
    msh->topology = avxTopology_TRI_LIST;

    afxObjectStash stashes[] =
    {
        {
            .cnt = idxCnt,
            .siz = sizeof(msh->indices[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&msh->indices
        },
        {
            .cnt = secCnt,
            .siz = sizeof(msh->sections[0]),
            .var = (void**)&msh->sections
        },
        {
            .cnt = morphCnt * secCnt,
            .siz = sizeof(msh->morphBasedSecAabb[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&msh->morphBasedSecAabb
        },
        {
            .cnt = blueprint->sideToAdjacentMap ? edgeCnt : 0,
            .siz = sizeof(msh->sideToAdjacentMap[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&msh->sideToAdjacentMap
        },
        {
            .cnt = biasCnt,
            .siz = sizeof(msh->biases[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&msh->biases
        },
        {
            .cnt = biasCnt,
            .siz = sizeof(msh->biasTagMap[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&msh->biasTagMap
        },
        {
            .cnt = morphCnt * biasCnt,
            .siz = sizeof(msh->morphBasedBiasObb[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&msh->morphBasedBiasObb
        },
        {
            .cnt = morphCnt,
            .siz = sizeof(msh->morphs[0]),
            .var = (void**)&msh->morphs
        },
        {
            .cnt = extraMorphCnt,
            .siz = sizeof(msh->extraMorphTagMap[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&msh->extraMorphTagMap
        },
        {
            .cnt = blueprint->vtxToTriMap ? vtxCnt : 0,
            .siz = sizeof(msh->vtxToTriMap[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&msh->vtxToTriMap
        },
    };

    if (AfxAllocateInstanceData(msh, ARRAY_SIZE(stashes), stashes)) AfxThrowError();
    else
    {
        AFX_ASSERT(msh->morphs);
        AFX_ASSERT(!extraMorphCnt || msh->extraMorphTagMap);
        AFX_ASSERT(!biasCnt || msh->biases);
        AFX_ASSERT(!biasCnt || msh->biasTagMap);
        AFX_ASSERT(!biasCnt || msh->morphBasedBiasObb);

        for (afxUnit i = 0; i < morphCnt; i++)
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

            afxMeshMorph* morph = &msh->morphs[i];

            if (!src->geo)
            {
                morph->geo = NIL;
                morph->baseVtx = 0;
                morph->vtxCnt = 0;
                morph->delta = FALSE;
            }
            else
            {
                AfxAssertObjects(1, &src->geo, afxFcc_GEO);
                AfxReacquireObjects(1, &src->geo);

                afxGeometryInfo geoi;
                AfxDescribeGeometry(src->geo, &geoi);
                AFX_ASSERT_RANGE(geoi.vtxCnt, src->baseVtx, src->vtxCnt);

                morph->geo = src->geo;
                morph->baseVtx = src->baseVtx;
                morph->vtxCnt = src->vtxCnt ? src->vtxCnt : geoi.vtxCnt - morph->baseVtx;
                morph->delta = !!src->delta;
            }
        }

        msh->morphCnt = morphCnt;

        if (extraMorphCnt)
        {
            if (!blueprint->extraMorphTagMap) for (afxUnit i = 0; i < extraMorphCnt; i++)
                AfxResetStrings(1, &msh->extraMorphTagMap[i]);
            else if (extraMorphCnt != (afxUnit)AfxCatalogStrings(AfxGetMorphTagStringBase(din), extraMorphCnt, blueprint->extraMorphTagMap, msh->extraMorphTagMap))
                AfxThrowError();
        }

        if (!err)
        {
            afxUnit* indices = msh->indices;
            afxMeshSection* sections = msh->sections;

            msh->idxCnt = idxCnt;
            msh->triCnt = triCnt;
            msh->secCnt = secCnt;
            msh->vtxCnt = vtxCnt;

            msh->topology = avxTopology_TRI_LIST;

            afxUnit surTriDiv = triCnt / secCnt;
            afxUnit baseTriIdx = 0;

            for (afxUnit i = 0; i < secCnt; i++)
            {
                afxMeshSection* mshs = &sections[i];

                if (!blueprint->trisForSurfMap)
                {
                    mshs->baseTriIdx = baseTriIdx;
                    mshs->triCnt = surTriDiv;
                    baseTriIdx += surTriDiv;
                }
                else
                {
                    if (!blueprint->baseTrisForSurfMap)
                        mshs->baseTriIdx = baseTriIdx;
                    else
                        mshs->baseTriIdx = blueprint->baseTrisForSurfMap[i];

                    mshs->triCnt = blueprint->trisForSurfMap[i];
                    baseTriIdx += mshs->triCnt;
                }

                if (!blueprint->surfToMtlMap)
                    mshs->mtlIdx = 0; // Must be at least one material
                else
                    mshs->mtlIdx = blueprint->surfToMtlMap[i];
            }

            AfxResetBoxes(morphCnt * secCnt, msh->morphBasedSecAabb);

            //msh->vtxToVtxMap = NIL; // vtxCnt
            //msh->vtxToTriMap = NIL; // vtxCnt
            //msh->sideToAdjacentMap = NIL; // triCnt * 3

            msh->jointsForTriCnt = 0;
            msh->jointsForTriMap = NIL; // jointsForTriMap[triIdx] = num of joints influencing this triangle?
            msh->triToJointCnt = 0;
            msh->triToJointMap = NIL; // triToJointMap[jntIdx] = num of triangles influenced by this joint?

            msh->minIdxSiz = (msh->morphs[0].vtxCnt > AFX_N8_MAX ? (msh->morphs[0].vtxCnt > AFX_N16_MAX ? sizeof(afxUnit32) : sizeof(afxUnit16)) : sizeof(afxUnit8));

            // initialize cache
            {
                avxIndexCache* cache = &msh->cache;

                AfxResetLinkage(&cache->stream);

                cache->buf = NIL;
                cache->base = 0;
                cache->range = 0;
                cache->stride = 0;

                cache->msh = msh;
                //cache->idxSiz = 0;
            }

            msh->biasCnt = biasCnt;

            AFX_ASSERT(biasCnt < 2 || blueprint->biases);

            if (biasCnt)
            {
                if (!blueprint->biases) for (afxUnit i = 0; i < biasCnt; i++)
                    AfxResetStrings(1, &msh->biasTagMap[i]);
                else if (biasCnt != (afxUnit)AfxCatalogStrings(AfxGetPivotTagStringBase(din), biasCnt, blueprint->biases, msh->biasTagMap))
                    AfxThrowError();

                for (afxUnit i = 0; i < biasCnt; i++)
                {
                    afxMeshBias* bias = &msh->biases[i];
                    bias->triCnt = 0;
                    bias->tris = NIL;
                }

                AfxResetBoxes(morphCnt * biasCnt, msh->morphBasedBiasObb);
            }

            if (!err)
            {
                msh->extData = NIL;
            }
        }

        if (err)
        {
            for (afxUnit i = 0; i < morphCnt; i++)
            {
                afxMeshMorph* morph = &msh->morphs[i];
                AfxAssertObjects(1, &morph->geo, afxFcc_GEO);
                AfxReleaseObjects(1, &morph->geo);
            }
        }

        if (err && AfxDeallocateInstanceData(msh, ARRAY_SIZE(stashes), stashes))
            AfxThrowError();
    }

    // EXHIBIT STATISTICAS

    if (!err)
    {
        afxString s;
        AfxGetMeshUrn(msh, &s);

        afxGeometryInfo geoi = { 0 };

        if (msh->morphs[0].geo)
            AfxDescribeGeometry(msh->morphs[0].geo, &geoi);

        afxUnit mshtSurCnt = msh->secCnt;
        afxUnit mshtTriCnt = msh->triCnt;

        AfxLogEcho("%.*s Mesh <%.*s> assembled at %p.\n    %u vertices with %u attributes.\n    %u triangles (%u bytes per index) arranged in %u sections.\n    Listing %u biases..:",
            AfxPushString(msh->biasCnt > 1 ? &AfxStaticString("Skinned") : &AfxStaticString("Rigid")),
            AfxPushString(&s), msh, msh->morphs[0].vtxCnt, geoi.attrCnt, mshtTriCnt, msh->minIdxSiz, mshtSurCnt, msh->biasCnt
        );

        for (afxUnit i = 0; i < msh->biasCnt; i++)
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

_AVX afxUnit AfxEnumerateMeshes(afxDrawInput din, afxUnit first, afxUnit cnt, afxMesh meshes[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    afxClass const* cls = AfxGetMeshClass(din);
    AfxAssertClass(cls, afxFcc_MSH);
    return AfxEnumerateClassInstances(cls, first, cnt, (afxObject*)meshes);
}

_AVX afxError AfxTransformMeshes(afxM3d const ltm, afxM3d const iltm, afxReal ltTol, afxV3d const atv, afxReal atTol, afxFlags flags, afxUnit cnt, afxMesh meshes[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(ltm);
    AFX_ASSERT(iltm);
    AFX_ASSERT(atv);
    AFX_ASSERT(cnt);
    AFX_ASSERT(meshes);
    //AfxThrowError();
    (void)atTol;
    (void)ltTol;
    
    for (afxUnit mshIdx = 0; mshIdx < cnt; mshIdx++)
    {
        afxMesh msh = meshes[mshIdx];

        if (!msh)
            continue;

        AfxAssertObjects(1, &msh, afxFcc_MSH);

        afxMeshInfo mshi;
        AfxDescribeMesh(msh, &mshi);

        for (afxUnit j = 0; j < mshi.morphCnt; j++)
        {
            afxMeshMorph mshm;
            AfxGetMeshMorphes(msh, j, 1, &mshm);

            if (!mshm.geo)
                continue;

            AfxAssertObjects(1, &mshm.geo, afxFcc_GEO);
            AFX_ASSERT(!mshm.delta);

            if (mshm.delta)
                AfxTransformGeometries(ltm, iltm, atv, FALSE, 1, &mshm.geo);
            else
                AfxTransformGeometries(ltm, iltm, atv, (flags & 1), 1, &mshm.geo); // sinalizado para renormalizar normals
        }

        if ((flags & 2) && (AfxM3dDet(ltm) < 0.0)) // se sinalizado para reordenar índices de faces
            AfxInvertMeshTopology(msh);

#if 0
        for (afxUnit j = 0; j < biasCnt; j++)
        {
            afxMeshBias* mshv = AfxGetMeshBiases(msh2, j);
            AfxTransformObbs(ltm, atv, 1, &mshv->aabb, &mshv->aabb);
        }
#endif
    }
    return err;
}

_AVX afxError AfxAssembleMeshes(afxDrawInput din, afxUnit cnt, afxMeshBlueprint const blueprints[], afxMesh meshes[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AFX_ASSERT(blueprints);
    AFX_ASSERT(meshes);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)AfxGetMeshClass(din);
    AfxAssertClass(cls, afxFcc_MSH);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)meshes, (void const*[]) { din, blueprints }))
        AfxThrowError();

    return err;
}
