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

#define _ASX_SIM_C
#define _ASX_MESH_C
#define _ASX_MESH_TOPOLOGY_C
#include "../impl/asxImplementation.h"

#if defined(_AFX_DEBUG)
#   define ASX_DBG_ECHO_MESH 1
#endif

ASX afxError AfxAcquireMeshTopology(afxDrawInput din, afxMeshBlueprint const* blueprint, afxMeshTopology* topology);

////////////////////////////////////////////////////////////////////////////////

_ASX afxPool* _AsxGetMeshRigPool(afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    return &msh->rigs;
}

_ASX asxMeshRig* _AsxGetMeshRig(afxMesh msh, afxUnit rigId)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    asxMeshRig* rig = NIL;
    AfxGetPoolUnit(&msh->rigs, rigId, (void**)&rig);
    return rig;
}

_ASX afxSimulation AfxGetMeshContext(afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    afxSimulation sim = AfxGetProvider(msh);
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    return sim;
}

_ASX afxBool AfxGetMeshUrn(afxMesh msh, afxString* id)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    AFX_ASSERT(id);
    AfxReflectString(&msh->urn, id);
    return msh->urn.len;
}

_ASXINL afxBool AfxIsMeshDeformable(afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    return (1 < msh->biasCnt);
}

_ASXINL afxUnit* AfxGetMeshIndices(afxMesh msh, afxUnit baseTriIdx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    AFX_ASSERT_RANGE(msh->triCnt, baseTriIdx, 1);

    // sanitize arguments
    baseTriIdx = AFX_MIN(baseTriIdx, msh->triCnt - 1);

    if (!msh->indices)
    {
        if (AfxAllocate(msh->idxCnt * sizeof(msh->indices[0]), AFX_SIMD_ALIGNMENT, AfxHere(), (void**)&msh->indices))
        {
            AfxThrowError();
            return NIL;
        }
    }
    return &msh->indices[baseTriIdx * ASX_INDICES_PER_TRI];
}

_ASX void AfxDescribeMesh(afxMesh msh, afxMeshInfo* info)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    AFX_ASSERT(info);
    info->topology = msh->topology;
    info->vtxCnt = msh->vtxCnt;
    info->attrCnt = msh->attrCnt;
    info->biasCnt = msh->biasCnt;
    info->triCnt = msh->triCnt;
    info->idxCnt = msh->idxCnt;
    info->edgeCnt = msh->triCnt * ASX_INDICES_PER_TRI;
    info->morphCnt = msh->morphCnt;
    info->secCnt = msh->secCnt;
    info->mtlCnt = msh->mtlCnt;
    info->minIdxSiz = msh->minIdxSiz;
    AfxMakeString32(&info->urn, &msh->urn);
}

_ASX afxUnit AfxGetMeshMorphes(afxMesh msh, afxUnit baseMorphIdx, afxUnit cnt, afxMeshMorph morphes[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    AFX_ASSERT_RANGE(msh->morphCnt, baseMorphIdx, cnt);
    
    // sanitize arguments
    cnt = AFX_MIN(cnt, msh->morphCnt - baseMorphIdx);
    baseMorphIdx = AFX_MIN(baseMorphIdx, msh->morphCnt - 1);

    for (afxUnit i = 0; i < cnt; i++)
        morphes[i] = msh->morphs[baseMorphIdx + i];

    return cnt;
}

_ASX afxError AfxChangeMeshMorphes(afxMesh msh, afxUnit baseMorphIdx, afxUnit cnt, afxMeshMorph const morphes[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    AFX_ASSERT_RANGE(msh->morphCnt, baseMorphIdx, cnt);
    AFX_ASSERT(morphes);

    // sanitize arguments
    cnt = AFX_MIN(cnt, msh->morphCnt - baseMorphIdx);
    baseMorphIdx = AFX_MIN(baseMorphIdx, msh->morphCnt - 1);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxUnit morphIdx = baseMorphIdx + i;
        afxMeshMorph* mshm = &msh->morphs[morphIdx];
        afxMeshMorph const* in = &morphes[i];

        mshm->flags = in->flags;
        mshm->baseVtx = in->baseVtx;

        if (in->morphedAttrs && morphIdx != 0)
            mshm->morphedAttrs = in->morphedAttrs;

        AfxRecomputeMeshBounds(msh, morphIdx, 0, msh->secCnt, 0);
    }
    return err;
}

_ASX afxUnit AfxGetMeshBiases(afxMesh msh, afxUnit baseBiasIdx, afxUnit cnt, afxMeshBias biases[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    AFX_ASSERT_RANGE(msh->biasCnt, baseBiasIdx, cnt);
    AFX_ASSERT(biases);
    
    // sanitize arguments
    cnt = AFX_MIN(cnt, msh->biasCnt - baseBiasIdx);
    baseBiasIdx = AFX_MIN(baseBiasIdx, msh->biasCnt - 1);

    for (afxUnit i = 0; i < cnt; i++)
        biases[i] = msh->biases[baseBiasIdx + i];

    return cnt;
}

_ASX afxString* AfxGetMeshBiasTags(afxMesh msh, afxUnit baseBiasIdx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    AFX_ASSERT_RANGE(msh->biasCnt, baseBiasIdx, 1);

    // sanitize arguments
    baseBiasIdx = AFX_MIN(baseBiasIdx, msh->biasCnt - 1);

    return &msh->biasId[baseBiasIdx];
}

_ASX afxString* AfxGetMeshMaterials(afxMesh msh, afxUnit baseMtlIdx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    AFX_ASSERT_RANGE(msh->mtlCnt, baseMtlIdx, 1);

    // sanitize arguments
    baseMtlIdx = AFX_MIN(baseMtlIdx, msh->mtlCnt - 1);

    return &msh->materials[baseMtlIdx];
}

_ASX afxUnit AfxGetMeshSections(afxMesh msh, afxUnit baseSecIdx, afxUnit cnt, afxMeshSection sections[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    AFX_ASSERT_RANGE(msh->secCnt, baseSecIdx, cnt);
    AFX_ASSERT(sections);

    // sanitize arguments
    cnt = AFX_MIN(cnt, msh->secCnt - baseSecIdx);
    baseSecIdx = AFX_MIN(baseSecIdx, msh->secCnt - 1);

    for (afxUnit i = 0; i < cnt; i++)
        sections[i] = msh->sections[baseSecIdx + i];

    return cnt;
}

_ASX afxError AfxResetMeshSections(afxMesh msh, afxUnit baseSecIdx, afxUnit cnt, afxMeshSection const sections[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    AFX_ASSERT_RANGE(msh->secCnt, baseSecIdx, cnt);
    AFX_ASSERT(sections);

    // sanitize arguments
    cnt = AFX_MIN(cnt, msh->secCnt - baseSecIdx);
    baseSecIdx = AFX_MIN(baseSecIdx, msh->secCnt - 1);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxUnit secIdx = baseSecIdx + i;
        afxMeshSection* dst = &msh->sections[secIdx];
        afxMeshSection const* src = &sections[i];
        
        AFX_ASSERT_RANGE(msh->triCnt, src->baseTriIdx, src->triCnt);
        AFX_ASSERT_RANGE(msh->mtlCnt, src->mtlIdx, 1);

        // sanitize arguments
        dst->baseTriIdx = AFX_MIN(src->baseTriIdx, msh->triCnt - 1);
        dst->triCnt = AFX_MIN(src->triCnt, msh->triCnt - dst->baseTriIdx);
        dst->mtlIdx = AFX_MIN(src->mtlIdx, msh->mtlCnt - 1);
    }
    return err;
}

_ASX afxUnit AfxRemapMeshCoverage(afxMesh msh, afxUnit mtlIdxCnt, afxUnit const mtlIdxLut[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
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

_ASX void AfxInvertMeshTopology(afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

    afxUnit triCnt = msh->triCnt;
    afxUnit* indices = AfxGetMeshIndices(msh, 0);
    
    for (afxUnit i = 0; i < triCnt; i++)
    {
        afxUnit idx = indices[i * ASX_INDICES_PER_TRI + 0];
        indices[i * ASX_INDICES_PER_TRI + 0] = indices[i * ASX_INDICES_PER_TRI + 2];
        indices[i * ASX_INDICES_PER_TRI + 2] = idx;
    }

    afxUnit* adjacent = msh->sideToAdjacentMap;

    if (adjacent)
    {
        afxUnit edgeCnt = msh->idxCnt / ASX_INDICES_PER_TRI;

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

_ASX afxUnit AfxFindVertexAttributes(afxMesh msh, afxUnit cnt, afxString const usages[], afxUnit indices[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    AFX_ASSERT(indices);
    afxUnit rslt = 0;

    for (afxUnit i = 0; i < cnt; i++)
        indices[i] = AFX_INVALID_INDEX;

    for (afxUnit i = 0; i < msh->attrCnt; i++)
    {
        afxString str = msh->attrInfo[i].usage.s;

        if (AfxCompareStrings(&usages[rslt], 0, TRUE, 1, &str, NIL))
            indices[rslt++] = i;

        if (cnt == rslt)
            break;
    }
    return rslt;
}

_ASX afxError AfxDescribeVertexAttribute(afxMesh msh, afxUnit attrIdx, afxVertexAttribute* info)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    AFX_ASSERT_RANGE(msh->attrCnt, attrIdx, 1);
    AFX_ASSERT(info);

    // sanitize arguments
    attrIdx = AFX_MIN(attrIdx, msh->attrCnt - 1);

    info->fmt = msh->attrInfo[attrIdx].fmt;
    info->flags = msh->attrInfo[attrIdx].flags;
    AfxMakeString8(&info->usage, &msh->attrInfo[attrIdx].usage.s);
    return err;
}

_ASX afxError AfxFormatVertexAttribute(afxMesh msh, afxUnit attrIdx, avxFormat fmt, afxVertexFlags flags, afxString const* usage)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    AFX_ASSERT_RANGE(msh->attrCnt, attrIdx, 1);

    // sanitize arguments
    attrIdx = AFX_MIN(attrIdx, msh->attrCnt - 1);

    asxMeshAttr* attr = &msh->attrInfo[attrIdx];
    attr->fmt = fmt;
    attr->flags = flags;
    AfxCopyString(&attr->usage.s, usage);
    AfxResetBoxes(1, &attr->aabb, 0);
    attr->aabbUpdReq = TRUE;

    if (msh->vtxAttrData[attrIdx])
        AfxDeallocate((void**)&msh->vtxAttrData[attrIdx], AfxHere());

    return err;
}

_ASX void* AfxAccessVertexData(afxMesh msh, afxUnit attrIdx, afxUnit morphIdx, afxUnit baseVtx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    AFX_ASSERT_RANGE(msh->morphCnt, morphIdx, 1);
    AFX_ASSERT_RANGE(msh->attrCnt, attrIdx, 1);
    AFX_ASSERT_RANGE(msh->vtxCnt, baseVtx, 1);

    // sanitize arguments
    attrIdx = AFX_MIN(attrIdx, msh->attrCnt - 1);
    morphIdx = AFX_MIN(morphIdx, msh->morphCnt - 1);
    baseVtx = AFX_MIN(baseVtx, msh->vtxCnt - 1);

    avxFormat fmt = msh->attrInfo[attrIdx].fmt;
    avxFormatDescription pfd;
    AvxDescribeFormats(1, &fmt, &pfd);
    afxUnit32 unitSiz = pfd.stride;// AfxVertexFormatGetSize(fmt);
    
    afxUnit32 attrBit = AFX_BIT(attrIdx);

    if (!(msh->morphs[morphIdx].morphedAttrs & attrBit))
        return NIL;

    if (!msh->vtxAttrData[attrIdx])
    {
        afxUnit32 dataSiz = 0;

        for (afxUnit i = 0; i < msh->morphCnt; i++)
            if (msh->morphs[i].morphedAttrs & attrBit)
                dataSiz += msh->vtxCnt * unitSiz;

        if (AfxAllocate(dataSiz, AFX_SIMD_ALIGNMENT, AfxHere(), (void**)&msh->vtxAttrData[attrIdx]))
        {
            AfxThrowError();
            return NIL;
        }
    }

    afxUnit32 dataOff = 0;

    for (afxUnit i = 0; i < morphIdx; i++)
        if (msh->morphs[i].morphedAttrs & attrBit)
            dataOff += msh->vtxCnt * unitSiz;

    afxByte* data = &msh->vtxAttrData[attrIdx][dataOff + (baseVtx * unitSiz)];

    return data;
}

_ASX afxError AfxInvertVertexData(afxMesh msh, afxUnit attrIdx, afxUnit morphIdx, afxUnit baseVtx, afxUnit vtxCnt)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    AFX_ASSERT_RANGE(msh->morphCnt, morphIdx, 1);
    AFX_ASSERT_RANGE(msh->attrCnt, attrIdx, 1);
    AFX_ASSERT_RANGE(msh->vtxCnt, baseVtx, vtxCnt);

    // sanitize arguments
    morphIdx = AFX_MIN(morphIdx, msh->morphCnt - 1);
    attrIdx = AFX_MIN(attrIdx, msh->attrCnt - 1);
    baseVtx = AFX_MIN(baseVtx, msh->vtxCnt - 1);
    vtxCnt = AFX_MIN(vtxCnt, msh->vtxCnt - baseVtx);

    avxFormat fmt = msh->attrInfo[attrIdx].fmt;
    avxFormatDescription pfd;
    AvxDescribeFormats(1, &fmt, &pfd);
    afxUnit32 unitSiz = pfd.stride;// AfxVertexFormatGetSize(fmt);

    void* data;

    if (!(data = AfxAccessVertexData(msh, attrIdx, morphIdx, baseVtx)))
    {
        AfxThrowError();
        return err;
    }

    if (!(msh->attrInfo[attrIdx].flags & afxVertexFlag_NORMALIZED))
    {
        AfxThrowError();
        return err;
    }

    // Switching the winding order to clockwise allow us to mirroring geometry via negative scale transforms.

    afxV3d* nrm = AfxAccessVertexData(msh, attrIdx, morphIdx, baseVtx);

    for (afxUnit i = 3; i < vtxCnt; i++)
        AfxV3dScale(nrm[i], nrm[i], -1.f);

    msh->attrInfo[attrIdx].aabbUpdReq = TRUE;
    return err;
}

_ASX afxError AfxNormalizeVertexData(afxMesh msh, afxUnit attrIdx, afxUnit morphIdx, afxUnit baseVtx, afxUnit vtxCnt)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    AFX_ASSERT_RANGE(msh->morphCnt, morphIdx, 1);
    AFX_ASSERT_RANGE(msh->attrCnt, attrIdx, 1);
    AFX_ASSERT_RANGE(msh->vtxCnt, baseVtx, vtxCnt);

    // sanitize arguments
    morphIdx = AFX_MIN(morphIdx, msh->morphCnt - 1);
    attrIdx = AFX_MIN(attrIdx, msh->attrCnt - 1);
    baseVtx = AFX_MIN(baseVtx, msh->vtxCnt - 1);
    vtxCnt = AFX_MIN(vtxCnt, msh->vtxCnt - baseVtx);

    avxFormat fmt = msh->attrInfo[attrIdx].fmt;
    avxFormatDescription pfd;
    AvxDescribeFormats(1, &fmt, &pfd);
    afxUnit32 unitSiz = pfd.stride;// AfxVertexFormatGetSize(fmt);

    void* data;

    if (!(data = AfxAccessVertexData(msh, attrIdx, morphIdx, baseVtx)))
    {
        AfxThrowError();
        return err;
    }

    if (!(msh->attrInfo[attrIdx].flags & afxVertexFlag_NORMALIZED))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT(fmt == avxFormat_RG32f || fmt == avxFormat_RGB32f || fmt == avxFormat_RGBA32f);
    //afxUnit32 unitSiz = AfxVertexFormatGetSize(fmt);

    switch (fmt)
    {
    case avxFormat_RG32f: AfxNormalizeArrayedV2d(vtxCnt, data, data); break;
    case avxFormat_RGB32f: AfxNormalizeArrayedV3d(vtxCnt, data, data); break;
    case avxFormat_RGBA32f: AfxNormalizeArrayedV4d(vtxCnt, data, data); break;
    default: AfxThrowError(); break;
    }

    msh->attrInfo[attrIdx].aabbUpdReq = TRUE;
    return err;
}

_ASX afxError AfxUpdateVertexData(afxMesh msh, afxUnit attrIdx, afxUnit morphIdx, afxUnit baseVtx, afxUnit vtxCnt, void const* src, afxUnit32 srcStride)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    AFX_ASSERT_RANGE(msh->morphCnt, morphIdx, 1);
    AFX_ASSERT_RANGE(msh->attrCnt, attrIdx, 1);
    AFX_ASSERT_RANGE(msh->vtxCnt, baseVtx, vtxCnt);
    AFX_ASSERT(src);

    // sanitize arguments
    morphIdx = AFX_MIN(morphIdx, msh->morphCnt - 1);
    attrIdx = AFX_MIN(attrIdx, msh->attrCnt - 1);
    baseVtx = AFX_MIN(baseVtx, msh->vtxCnt - 1);
    vtxCnt = AFX_MIN(vtxCnt, msh->vtxCnt - baseVtx);

    avxFormat fmt = msh->attrInfo[attrIdx].fmt;
    avxFormatDescription pfd;
    AvxDescribeFormats(1, &fmt, &pfd);
    afxUnit32 unitSiz = pfd.stride;// AfxVertexFormatGetSize(fmt);

    void* data;

    if (!(data = AfxAccessVertexData(msh, attrIdx, morphIdx, baseVtx)))
    {
        AfxThrowError();
        return err;
    }

    AfxStream2(vtxCnt, src, srcStride, data, unitSiz);
    msh->attrInfo[attrIdx].aabbUpdReq = TRUE;
    return err;
}

_ASX afxError AfxUploadVertexData(afxMesh msh, afxUnit attrIdx, afxUnit morphIdx, afxUnit baseVtx, afxUnit vtxCnt, afxStream in, afxUnit stride)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    AFX_ASSERT_RANGE(msh->morphCnt, morphIdx, 1);
    AFX_ASSERT_RANGE(msh->attrCnt, attrIdx, 1);
    AFX_ASSERT_RANGE(msh->vtxCnt, baseVtx, vtxCnt);
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &in);

    // sanitize arguments
    morphIdx = AFX_MIN(morphIdx, msh->morphCnt - 1);
    attrIdx = AFX_MIN(attrIdx, msh->attrCnt - 1);
    baseVtx = AFX_MIN(baseVtx, msh->vtxCnt - 1);
    vtxCnt = AFX_MIN(vtxCnt, msh->vtxCnt - baseVtx);

    avxFormat fmt = msh->attrInfo[attrIdx].fmt;
    avxFormatDescription pfd;
    AvxDescribeFormats(1, &fmt, &pfd);
    afxUnit32 unitSiz = pfd.stride;// AfxVertexFormatGetSize(fmt);

    void* data;

    if (!(data = AfxAccessVertexData(msh, attrIdx, morphIdx, baseVtx)))
    {
        AfxThrowError();
        return err;
    }
    
    //AfxDoStreamInput(in, stride, vtxCnt, data, unitSiz);
    AfxReadStream2(in, vtxCnt * unitSiz, stride, data, unitSiz);
    msh->attrInfo[attrIdx].aabbUpdReq = TRUE;
    return err;
}

_ASX afxError AfxDownloadVertexData(afxMesh msh, afxUnit attrIdx, afxUnit morphIdx, afxUnit baseVtx, afxUnit vtxCnt, afxStream out, afxUnit stride)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    AFX_ASSERT_RANGE(msh->morphCnt, morphIdx, 1);
    AFX_ASSERT_RANGE(msh->attrCnt, attrIdx, 1);
    AFX_ASSERT_RANGE(msh->vtxCnt, baseVtx, vtxCnt);
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &out);

    // sanitize arguments
    morphIdx = AFX_MIN(morphIdx, msh->morphCnt - 1);
    attrIdx = AFX_MIN(attrIdx, msh->attrCnt - 1);
    baseVtx = AFX_MIN(baseVtx, msh->vtxCnt - 1);
    vtxCnt = AFX_MIN(vtxCnt, msh->vtxCnt - baseVtx);

    avxFormat fmt = msh->attrInfo[attrIdx].fmt;
    avxFormatDescription pfd;
    AvxDescribeFormats(1, &fmt, &pfd);
    afxUnit32 unitSiz = pfd.stride;// AfxVertexFormatGetSize(fmt);

    void const* data;

    if (!(data = AfxAccessVertexData(msh, attrIdx, morphIdx, baseVtx)))
    {
        AfxThrowError();
        return err;
    }

    AfxDoStreamOutput(out, stride, vtxCnt, data, unitSiz);
    return err;
}

_ASX afxError AfxRecomputeMeshBounds(afxMesh msh, afxUnit morphIdx, afxUnit baseSecIdx, afxUnit cnt, afxUnit posAttrIdx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    AFX_ASSERT_RANGE(msh->morphCnt, morphIdx, 1);
    AFX_ASSERT_RANGE(msh->secCnt, baseSecIdx, cnt);

    // sanitize arguments
    morphIdx = AFX_MIN(morphIdx, msh->morphCnt - 1);
    baseSecIdx = AFX_MIN(baseSecIdx, msh->secCnt - 1);
    cnt = AFX_MIN(cnt, msh->secCnt - baseSecIdx);

    afxMeshMorph mshm;
    if (!AfxGetMeshMorphes(msh, morphIdx, 1, &mshm))
    {
        AfxThrowError();
        return err;
    }

    afxBox* morphSecAabbs = &msh->secAabb[morphIdx * msh->secCnt];
    AfxResetBoxes(cnt, morphSecAabbs, 0);

    void* data = AfxAccessVertexData(msh, posAttrIdx, morphIdx, mshm.baseVtx);

    afxUnit triCnt = msh->triCnt;
    afxUnit secCnt = msh->secCnt;
    

    avxFormat attrFmt = msh->attrInfo[posAttrIdx].fmt;
    avxFormatDescription pfd;
    AvxDescribeFormats(1, &attrFmt, &pfd);
    afxUnit32 unitSiz = pfd.stride;// AfxVertexFormatGetSize(fmt);

    if (attrFmt == avxFormat_RGB32f)
    {
        afxV3d* data3 = data;

        for (afxUnit j = 0; j < cnt; j++)
        {
            afxUnit secIdx = baseSecIdx + j;

            afxMeshSection mshs;
            AfxGetMeshSections(msh, secIdx, 1, &mshs);
            afxUnit const* indices = AfxGetMeshIndices(msh, mshs.baseTriIdx);
            afxUnit idxCnt = mshs.triCnt * ASX_INDICES_PER_TRI;

            for (afxUnit i = 0; i < idxCnt; i++)
                AfxEmboxVectors(&morphSecAabbs[secIdx], 1, &data3[i]);
        }
    }
    else if (attrFmt == avxFormat_RGBA32f)
    {
        afxV4d* data4 = data;

        for (afxUnit j = 0; j < cnt; j++)
        {
            afxUnit secIdx = baseSecIdx + j;

            afxMeshSection mshs;
            AfxGetMeshSections(msh, secIdx, 1, &mshs);
            afxUnit const* indices = AfxGetMeshIndices(msh, mshs.baseTriIdx);
            afxUnit idxCnt = mshs.triCnt * ASX_INDICES_PER_TRI;

            for (afxUnit i = 0; i < idxCnt; i++)
                AfxEmboxPoints(&morphSecAabbs[secIdx], 1, &data4[i]);
        }
    }
    else AfxThrowError();

    return err;
}

_ASX afxError AfxUpdateMeshBounds(afxMesh msh, afxUnit morphIdx, afxUnit baseSecIdx, afxUnit cnt, afxBox const aabbs[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    AFX_ASSERT_RANGE(msh->morphCnt, morphIdx, 1);
    AFX_ASSERT_RANGE(msh->secCnt, baseSecIdx, cnt);
    AFX_ASSERT(aabbs);

    // sanitize arguments
    morphIdx = AFX_MIN(morphIdx, msh->morphCnt - 1);
    cnt = AFX_MIN(cnt, msh->secCnt - baseSecIdx);
    baseSecIdx = AFX_MIN(baseSecIdx, msh->secCnt - 1);

    afxMeshMorph mshm;
    if (!AfxGetMeshMorphes(msh, morphIdx, 1, &mshm))
    {
        AfxThrowError();
        return err;
    }

    // Recompute it if <aabbs> is not specified.

    afxBox* morphSecAabbs = &msh->secAabb[morphIdx * msh->secCnt + baseSecIdx];
    AfxCopyBoxes(cnt, aabbs, 0, morphSecAabbs, 0);

    return err;
}

// UPDATE/DUMP INDEXES /////////////////////////////////////////////////////////

_ASX afxError AfxUpdateMeshIndices(afxMesh msh, afxUnit baseTriIdx, afxUnit triCnt, void const* src, afxUnit srcIdxSiz)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    AFX_ASSERT_RANGE(msh->triCnt, baseTriIdx, triCnt);

    // sanitize arguments
    triCnt = AFX_MIN(triCnt, msh->triCnt - baseTriIdx);
    baseTriIdx = AFX_MIN(baseTriIdx, msh->triCnt - 1);

    afxUnit idxCnt = triCnt * ASX_INDICES_PER_TRI;
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
                AFX_ASSERT_RANGE(msh->vtxCnt, indices[i], 1);
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

_ASX afxError AfxDumpMeshIndices(afxMesh msh, afxUnit baseTriIdx, afxUnit triCnt, void* dst, afxUnit dstIdxSiz)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    AFX_ASSERT_RANGE(msh->triCnt, baseTriIdx, triCnt);

    // sanitize arguments
    triCnt = AFX_MIN(triCnt, msh->triCnt - baseTriIdx);
    baseTriIdx = AFX_MIN(baseTriIdx, msh->triCnt - 1);

    afxUnit const* indices = AfxGetMeshIndices(msh, baseTriIdx);

    AFX_ASSERT(dst);
    AFX_ASSERT(dstIdxSiz);
    AfxStream2(ASX_INDICES_PER_TRI * triCnt, indices, sizeof(indices[0]), dst, dstIdxSiz);

    return err;
}

// UPLOAD/DOWNLOAD INDICES /////////////////////////////////////////////////////

_ASX afxError AfxUploadMeshIndices(afxMesh msh, afxUnit baseTriIdx, afxUnit triCnt, afxStream in, afxUnit srcIdxSiz)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    AFX_ASSERT_RANGE(msh->triCnt, baseTriIdx, triCnt);

    // sanitize arguments
    triCnt = AFX_MIN(triCnt, msh->triCnt - baseTriIdx);
    baseTriIdx = AFX_MIN(baseTriIdx, msh->triCnt - 1);

    afxUnit idxCnt = triCnt * ASX_INDICES_PER_TRI;
    afxUnit* indices = AfxGetMeshIndices(msh, baseTriIdx);

    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &in);

    if (AfxReadStream2(in, srcIdxSiz * idxCnt, srcIdxSiz, indices, sizeof(indices[0])))
        AfxThrowError();

    if (!err)
    {
#if _AFX_DEBUG
        afxMeshMorph mshm;
        AfxGetMeshMorphes(msh, 0, 1, &mshm);

        for (afxUnit i = 0; i < idxCnt; i++)
        {
            AFX_ASSERT_RANGE(msh->vtxCnt, indices[i], 1);

            if (msh->vtxCnt < indices[i])
                AfxThrowError();
        }
#endif
        for (afxUnit i = 0; i < msh->secCnt; i++)
        {
            afxMeshSection mshs;
            AfxGetMeshSections(msh, i, 1, &mshs);

            if (mshs.baseTriIdx >= baseTriIdx && triCnt >= mshs.triCnt)
                for (afxUnit j = 0; j < msh->morphCnt; j++)
                    AfxRecomputeMeshBounds(msh, j, i, 1, 0);
        }
    }
    return err;
}

_ASX afxError AfxDownloadMeshIndices(afxMesh msh, afxUnit baseTriIdx, afxUnit triCnt, afxStream out, afxUnit dstIdxSiz)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    AFX_ASSERT_RANGE(msh->triCnt, baseTriIdx, triCnt);

    // sanitize arguments
    triCnt = AFX_MIN(triCnt, msh->triCnt - baseTriIdx);
    baseTriIdx = AFX_MIN(baseTriIdx, msh->triCnt - 1);

    afxUnit idxCnt = triCnt * ASX_INDICES_PER_TRI;
    afxUnit* indices = AfxGetMeshIndices(msh, baseTriIdx);

    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &out);

    if (AfxWriteStream2(out, dstIdxSiz * idxCnt, dstIdxSiz, indices, sizeof(indices[0])))
        AfxThrowError();

    return err;
}

_ASX afxError AfxRecomputeMeshNormals(afxMesh msh, afxUnit morphIdx, afxUnit posAttrIdx, afxUnit nrmAttrIdx)
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

    afxMeshInfo mshi;
    AfxDescribeMesh(msh, &mshi);

    afxMeshMorph mshm;
    AfxGetMeshMorphes(msh, morphIdx, 1, &mshm);

    /*
        For each face on the mesh, calc the face normal (without normalization, as just explained), and directly accumulate this normal in each vertex belonging to the face.
        After you are done with the faces, each vertex will have recieved all the face normals it was supposed to recieve. That simple.
    */

    afxUnit const* indices = AfxGetMeshIndices(msh, 0);

    afxV3d const* pos = AfxAccessVertexData(msh, posAttrIdx, morphIdx, mshm.baseVtx);
    afxV3d* nrm = AfxAccessVertexData(msh, nrmAttrIdx, morphIdx, mshm.baseVtx);

    AfxUpdateVertexData(msh, nrmAttrIdx, morphIdx, mshm.baseVtx, msh->vtxCnt, AFX_V3D_ZERO, 0); // zero it

    AfxComputeTriangleNormals(mshi.triCnt, indices, sizeof(indices[0]), pos, sizeof(pos[0]), nrm, sizeof(nrm[0]));
    
    return err;
}

_ASX afxError AfxRecomputeMeshTangents(afxMesh msh, afxUnit morphIdx, afxUnit posAttrIdx, afxUnit uvAttrIdx, afxUnit tanAttrIdx, afxUnit bitAttrIdx)
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

    afxMeshInfo mshi;
    AfxDescribeMesh(msh, &mshi);

    afxMeshMorph mshm;
    AfxGetMeshMorphes(msh, morphIdx, 1, &mshm);

    afxUnit const* indices = AfxGetMeshIndices(msh, 0);

    afxV3d const* pos = AfxAccessVertexData(msh, posAttrIdx, morphIdx, mshm.baseVtx);
    afxV2d const* uv = AfxAccessVertexData(msh, uvAttrIdx, morphIdx, mshm.baseVtx);

    afxV3d* tan = AfxAccessVertexData(msh, tanAttrIdx, morphIdx, mshm.baseVtx);
    afxV3d* bit = AfxAccessVertexData(msh, bitAttrIdx, morphIdx, mshm.baseVtx);

    AfxComputeTriangleTangents(mshi.triCnt, indices, sizeof(indices[0]), pos, sizeof(pos[0]), uv, sizeof(uv), FALSE, tan, sizeof(tan), bit, sizeof(bit));
    
    return err;
}

_ASX afxError _AsxMshDtorCb(afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

    afxUnit idxCnt = msh->idxCnt;
    afxUnit edgeCnt = msh->idxCnt;

    AFX_ASSERT(msh->rigs.totalUsedCnt == 0);

    for (afxUnit i = 0; i < msh->rigs.totalUsedCnt; i++)
    {
        asxMeshRig* rig;
        AfxGetPoolUnit(&msh->rigs, i, (void**)&rig);
        AFX_ASSERT(rig);

        afxModel mdl = rig->mdl;
        AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);

        AFX_ASSERT(rig->slotIdx != AFX_INVALID_INDEX);

#if 0 // Models owns meshes. We should never reach here with any valid rig linkage remaining. Someone who don't owns this is disposing it.
        if (AfxRigMeshes(mdl, NIL, rig->slotIdx, 1, NIL))
            AfxThrowError();
#endif
    }
    AfxExhaustPool(&msh->rigs, FALSE);

    if (msh->morphs)
    {
        afxUnit morphCnt = msh->morphCnt;

        for (afxUnit i = 0; i < morphCnt; i++)
        {
            afxMeshMorph* morph = &msh->morphs[i];

        }
    }

    for (afxUnit i = 0; i < msh->attrCnt; i++)
    {
        if (msh->vtxAttrData[i])
        {
            AfxDeallocate((void**)&msh->vtxAttrData[i], AfxHere());
        }
    }

    if (msh->indices)
        AfxDeallocate((void**)&msh->indices, AfxHere());

    afxObjectStash stashes[] =
    {
        {
            .cnt = msh->sideToAdjacentMap ? edgeCnt : 0,
            .siz = sizeof(msh->sideToAdjacentMap[0]),
            .var = (void**)&msh->sideToAdjacentMap
        },
        {
            .cnt = msh->mtlCnt,
            .siz = sizeof(msh->materials[0]),
            .var = (void**)&msh->materials
        },
        {
            .cnt = msh->secCnt,
            .siz = sizeof(msh->sections[0]),
            .var = (void**)&msh->sections
        },
        {
            .cnt = msh->biasCnt,
            .siz = sizeof(msh->biases[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&msh->biases
        },
        {
            .cnt = msh->biasCnt,
            .siz = sizeof(msh->biasId[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&msh->biasId
        },
        // VERTEX DATA
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
        {
            .cnt = msh->attrCnt,
            .siz = sizeof(msh->attrInfo[0]),
            .var = (void**)&msh->attrInfo
        },
        {
            .cnt = msh->attrCnt,
            .siz = sizeof(msh->attrIds[0]),
            .var = (void**)&msh->attrIds
        },
        {
            .cnt = msh->attrCnt,
            .siz = sizeof(msh->vtxAttrData[0]),
            .var = (void**)&msh->vtxAttrData
        },
        // SHAPE DATA
        {
            .cnt = msh->morphCnt,
            .siz = sizeof(msh->morphs[0]),
            .var = (void**)&msh->morphs
        },
        {
            .cnt = msh->morphCnt,
            .siz = sizeof(msh->morphTags[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&msh->morphTags
        },
        {
            .cnt = msh->secCnt * msh->morphCnt,
            .siz = sizeof(msh->secAabb[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&msh->secAabb
        },
        {
            .cnt = msh->biasCnt * msh->morphCnt,
            .siz = sizeof(msh->biasObb[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&msh->biasObb
        },
    };

    if (AfxDeallocateInstanceData(msh, ARRAY_SIZE(stashes), stashes))
        AfxThrowError();

    AFX_ASSERT(!msh->morphs);
    AFX_ASSERT(!msh->morphTags);
    AFX_ASSERT(!msh->biases);
    AFX_ASSERT(!msh->biasId);
    AFX_ASSERT(!msh->biasObb);

    AFX_ASSERT(!msh->indices);

    if (msh->vtxCache.buf)
    {
        AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &msh->vtxCache.buf);
        AfxDisposeObjects(1, &msh->vtxCache.buf);
    }

    if (msh->ibo)
    {
        AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &msh->ibo);
        AfxDisposeObjects(1, &msh->ibo);
    }

    return err;
}

_ASX afxError _AsxMshCtorCb(afxMesh msh, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

    afxSimulation sim = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxMeshBlueprint const* blueprint = AFX_CAST(afxMeshBlueprint const*, args[1]) + invokeNo;

    if (!AfxCatalogStrings(_AsxGetModelUrnStringBase(sim), 1, &blueprint->urn.s, &msh->urn))
        AfxThrowError();

    afxUnit secCnt = AFX_MAX(1, blueprint->secCnt);
    AFX_ASSERT(secCnt);

    afxUnit mtlCnt = AFX_MAX(1, blueprint->mtlCnt);
    
    afxUnit triCnt = blueprint->triCnt;
    AFX_ASSERT(triCnt);

    afxUnit attrCnt = blueprint->attrCnt;
    AFX_ASSERT(attrCnt);
    afxUnit vtxCnt = blueprint->vtxCnt;
    AFX_ASSERT(vtxCnt);
    afxUnit edgeCnt = triCnt * ASX_INDICES_PER_TRI;
    afxUnit idxCnt = triCnt * ASX_INDICES_PER_TRI;

    afxUnit morphCnt = AFX_MAX(1, blueprint->morphCnt);
    afxUnit biasCnt = AFX_MAX(1, blueprint->biasCnt);

    msh->morphs = NIL;
    msh->morphTags = NIL;
    msh->materials = NIL;
    msh->biases = NIL;
    msh->biasId = NIL;
    msh->biasObb = NIL;
    msh->secAabb = NIL;
    msh->topology = avxTopology_TRI_LIST;

    afxObjectStash stashes[] =
    {
        // TOPOLOGY DATA
        {
            .cnt = blueprint->sideToAdjacentMap ? edgeCnt : 0,
            .siz = sizeof(msh->sideToAdjacentMap[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&msh->sideToAdjacentMap
        },
        {
            .cnt = mtlCnt,
            .siz = sizeof(msh->materials[0]),
            .var = (void**)&msh->materials
        },
        {
            .cnt = secCnt,
            .siz = sizeof(msh->sections[0]),
            .var = (void**)&msh->sections
        },
        {
            .cnt = biasCnt,
            .siz = sizeof(msh->biases[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&msh->biases
        },
        {
            .cnt = biasCnt,
            .siz = sizeof(msh->biasId[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&msh->biasId
        },
        // VERTEX DATA
        
        {
            .cnt = blueprint->vtxToVtxMap ? vtxCnt : 0,
            .siz = sizeof(msh->vtxToVtxMap[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&msh->vtxToVtxMap
        },
        {
            .cnt = blueprint->vtxToTriMap ? vtxCnt : 0,
            .siz = sizeof(msh->vtxToTriMap[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&msh->vtxToTriMap
        },
        {
            .cnt = attrCnt,
            .siz = sizeof(msh->attrInfo[0]),
            .var = (void**)&msh->attrInfo
        },
        {
            .cnt = attrCnt,
            .siz = sizeof(msh->attrIds[0]),
            .var = (void**)&msh->attrIds
        },
        {
            .cnt = attrCnt,
            .siz = sizeof(msh->vtxAttrData[0]),
            .var = (void**)&msh->vtxAttrData
        },
        // SHAPE DATA
        {
            .cnt = morphCnt,
            .siz = sizeof(msh->morphs[0]),
            .var = (void**)&msh->morphs
        },
        {
            .cnt = morphCnt,
            .siz = sizeof(msh->morphTags[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&msh->morphTags
        },
        {
            .cnt = morphCnt * secCnt,
            .siz = sizeof(msh->secAabb[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&msh->secAabb
        },
        {
            .cnt = morphCnt * biasCnt,
            .siz = sizeof(msh->biasObb[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&msh->biasObb
        }
    };

    if (AfxAllocateInstanceData(msh, ARRAY_SIZE(stashes), stashes))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT(msh->morphs);
    //AFX_ASSERT(!extraMorphCnt || msh->extraMorphTagMap);
    AFX_ASSERT(!biasCnt || msh->biases);
    AFX_ASSERT(!biasCnt || msh->biasId);
    AFX_ASSERT(!biasCnt || msh->biasObb);

    msh->topology = avxTopology_TRI_LIST;
    msh->triCnt = triCnt;

    // materials

    AFX_ASSERT(mtlCnt);
    msh->mtlCnt = mtlCnt;

    if (!blueprint->materials)
        AfxResetStrings(mtlCnt, msh->materials);
    else if (mtlCnt != (afxUnit)AfxCatalogStrings(_AsxGetMorphTagStringBase(sim), mtlCnt, blueprint->materials, msh->materials))
        AfxThrowError();

    // sections

    AFX_ASSERT(secCnt);
    msh->secCnt = secCnt;

    afxUnit surTriDiv = triCnt / secCnt;
    afxUnit baseTriIdx = 0;

    for (afxUnit i = 0; i < secCnt; i++)
    {
        afxMeshSection* mshs = &msh->sections[i];

        if (!blueprint->sections)
        {
            mshs->mtlIdx = 0; // Must be at least one material
            mshs->baseTriIdx = baseTriIdx;
            mshs->triCnt = surTriDiv;
            baseTriIdx += surTriDiv;
        }
        else
        {
            afxMeshSection const* in = &blueprint->sections[i];
            AFX_ASSERT_RANGE(msh->triCnt, in->baseTriIdx, in->triCnt);
            AFX_ASSERT_RANGE(msh->mtlCnt, in->mtlIdx, 1);
            mshs->baseTriIdx = AFX_MIN(in->baseTriIdx, msh->triCnt - 1);
            mshs->triCnt = AFX_MIN(in->triCnt, msh->triCnt - mshs->baseTriIdx);
            mshs->mtlIdx = AFX_MIN(in->mtlIdx, msh->mtlCnt - 1);
            mshs->flags = in->flags;
        }
    }

    // deformation data

    AFX_ASSERT(biasCnt);
    msh->biasCnt = biasCnt;

    AFX_ASSERT(biasCnt < 2 || blueprint->biases);

    if (!blueprint->biases)
        AfxResetStrings(biasCnt, msh->biasId);
    else if (biasCnt != (afxUnit)AfxCatalogStrings(_AsxGetPivotTagStringBase(sim), biasCnt, blueprint->biases, msh->biasId))
        AfxThrowError();

    for (afxUnit i = 0; i < biasCnt; i++)
    {
        afxMeshBias* bias = &msh->biases[i];
        bias->triCnt = 0;
        bias->tris = NIL;
    }

    AfxResetBoxes(morphCnt * biasCnt, msh->biasObb, 0);

    msh->jointsForTriCnt = 0;
    msh->jointsForTriMap = NIL; // jointsForTriMap[triIdx] = num of joints influencing this triangle?
    msh->triToJointCnt = 0;
    msh->triToJointMap = NIL; // triToJointMap[jntIdx] = num of triangles influenced by this joint?

    // vertex data

    msh->vtxCnt = vtxCnt;
    msh->minIdxSiz = (msh->vtxCnt > AFX_U8_MAX ? (msh->vtxCnt > AFX_U16_MAX ? sizeof(afxUnit32) : sizeof(afxUnit16)) : sizeof(afxUnit8));
    msh->idxCnt = idxCnt;

    msh->attrCnt = attrCnt;
    afxMask allAttrEnabledMask = NIL;

    if (!blueprint->attrs)
        AfxResetStrings(attrCnt, msh->attrIds);
    else if (attrCnt != (afxUnit)AfxCatalogStrings(_AsxGetMorphTagStringBase(sim), attrCnt, blueprint->attrs, msh->attrIds))
        AfxThrowError();

    for (afxUnit i = 0; i < attrCnt; i++)
    {
        asxMeshAttr* attr = &msh->attrInfo[i];

        allAttrEnabledMask |= AFX_BIT(i);

        AfxMakeString8(&attr->usage, NIL);
        attr->flags = NIL;
        attr->fmt = avxFormat_RGBA32f;

        msh->vtxAttrData[i] = NIL;
    }

    // morph/shape data

    msh->morphCnt = morphCnt;

    for (afxUnit i = 0; i < morphCnt; i++)
    {
        afxMeshMorph* dst = &msh->morphs[i];
        afxMeshMorph const* src;

        if (i == 0)
        {
            dst->baseVtx = 0;
            dst->flags = 0;
            dst->morphedAttrs = allAttrEnabledMask;
        }
        else
        {
            if (!blueprint->morphs)
                src = (afxMeshMorph const[]) { 0 };
            else
                src = &blueprint->morphs[i];

            dst->baseVtx = src->baseVtx;
            dst->flags = (src->flags & afxMeshMorphFlag_DELTA);
            dst->morphedAttrs = src->morphedAttrs ? src->morphedAttrs : allAttrEnabledMask;
        }
    }

    if (!blueprint->morphTags)
        AfxResetStrings(morphCnt, msh->morphTags);
    else if (morphCnt != (afxUnit)AfxCatalogStrings(_AsxGetMorphTagStringBase(sim), morphCnt, blueprint->morphTags, msh->morphTags))
        AfxThrowError();

    AfxResetBoxes(morphCnt * secCnt, msh->secAabb, 0);

    msh->vtxCache.buf = NIL;
    msh->ibo = NIL;
    msh->iboBase = 0;
    msh->iboRange = 0;
    msh->iboStride = 0;

    msh->idd = NIL;
    msh->udd = NIL;

    AfxDeployPool(&msh->rigs, sizeof(asxMeshRig), 1, NIL);

    if (!err)
    {
#if 0
        // EXHIBIT STATISTICAS
        afxString s;
        AfxGetMeshUrn(msh, &s);

        AfxReportMessage("%.*s Mesh <%.*s> assembled at %p.\n    %u vertices with %u attributes.\n    %u triangles (%u bytes per index) arranged in %u sections.\n    Listing %u biases..:",
            AfxPushString(msh->biasCnt > 1 ? &AfxStaticString("Skinned") : &AfxStaticString("Rigid")),
            AfxPushString(&s), msh, msh->vtxCnt, msh->attrCnt, msh->triCnt, msh->minIdxSiz, msh->secCnt, msh->biasCnt
        );

        for (afxUnit i = 0; i < msh->biasCnt; i++)
        {
            s = msh->biasId[i];
            AfxReportMessage("    %3u <%.*s> %3u", i, AfxPushString(&s), msh->biases ? msh->biases[i].triCnt : 0);
        }
#endif
    }
    else if (AfxDeallocateInstanceData(msh, ARRAY_SIZE(stashes), stashes))
        AfxThrowError();

    return err;
}

_ASX afxClassConfig const _ASX_MSH_CLASS_CONFIG =
{
    .fcc = afxFcc_MSH,
    .name = "Mesh",
    .desc = "Topological Morphable Mesh",
    .fixedSiz = sizeof(AFX_OBJECT(afxMesh)),
    .ctor = (void*)_AsxMshCtorCb,
    .dtor = (void*)_AsxMshDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ASX afxError AfxTransformMeshes(afxM3d const ltm, afxM3d const iltm, afxReal ltTol, afxV3d const atv, afxReal atTol, afxFlags flags, afxUnit cnt, afxMesh meshes[])
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

        AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

        afxMeshInfo mshi;
        AfxDescribeMesh(msh, &mshi);

        for (afxUnit morphIdx = 0; morphIdx < mshi.morphCnt; morphIdx++)
        {
            afxMeshMorph mshm;
            AfxGetMeshMorphes(msh, morphIdx, 1, &mshm);

            afxBool renormalize = (flags & 1); // sinalizado para renormalizar normals

            if (mshm.flags & afxMeshMorphFlag_DELTA)
                renormalize = FALSE; // don't renormalize data treated as delta

            for (afxUnit attrIdx = 0; attrIdx < mshi.attrCnt; attrIdx++)
            {
                afxUnit32 attrBit = AFX_BIT(attrIdx);

                if (!(msh->morphs[morphIdx].morphedAttrs & attrBit))
                    continue;

                asxMeshAttr* va = &msh->attrInfo[attrIdx];
                    
                void* data = AfxAccessVertexData(msh, attrIdx, morphIdx, 0);
                afxBool linearFlag = va->flags & afxVertexFlag_LTM;
                afxBool invLinearFlag = va->flags & afxVertexFlag_ILTM;
                afxBool affineFlag = va->flags & afxVertexFlag_ATV;
                afxBool deltaFlag = va->flags & afxVertexFlag_DELTA;
                afxBool normalizedFlag = va->flags & afxVertexFlag_NORMALIZED;
                avxFormat fmt = va->fmt;
                afxBool normalized;

                if (deltaFlag)
                    renormalize = FALSE; // don't renormalize data treated as delta
                
#if 0
                if (va->flags & afxVertexFlag_ATV)
                {
                    if (fmt == afxVertexFormat_V4D)
                        AfxM3dPostMultiplyLtv4d(ltm, geoi.vtxCnt, data, data);
                    else if (fmt == afxVertexFormat_V3D)
                        AfxM3dPostMultiplyV3d(ltm, geoi.vtxCnt, data, data);

                    if (!(va->flags & afxVertexFlag_DELTA))
                    {
                        if (fmt == afxVertexFormat_V4D)
                            for (afxUnit k = 0; k < geoi.vtxCnt; k++)
                                AfxV4dAdd(((afxV4d*)data)[k], ((afxV4d*)data)[k], atv);
                        else if (fmt == afxVertexFormat_V3D)
                            for (afxUnit k = 0; k < geoi.vtxCnt; k++)
                                AfxV3dAdd(((afxV3d*)data)[k], ((afxV3d*)data)[k], atv);
                    }
                    normalized = FALSE;
                }
#endif
                afxString const attrs[] = { AFX_STRING("pos"), AFX_STRING("tan"), AFX_STRING("bit"), AFX_STRING("tbc"), AFX_STRING("nrm") };

                afxUnit sIdx;
                if (!AfxCompareStrings(&va->usage.s, 0, FALSE, ARRAY_SIZE(attrs), attrs, &sIdx))
                    continue;

                switch (sIdx)
                {
                case 0: // pos
                {
                    if (fmt == avxFormat_RGBA32f)
                    {
                        AfxM3dPostMultiplyLtv4d(ltm, mshi.vtxCnt, data, data);

                        if (!deltaFlag)
                            for (afxUnit k = 0; k < mshi.vtxCnt; k++)
                                AfxV4dAdd(((afxV4d*)data)[k], ((afxV4d*)data)[k], atv);
                    }
                    else if (fmt == avxFormat_RGB32f)
                    {
                        AfxM3dPostMultiplyV3d(ltm, mshi.vtxCnt, data, data);

                        if (!deltaFlag)
                            for (afxUnit k = 0; k < mshi.vtxCnt; k++)
                                AfxV3dAdd(((afxV3d*)data)[k], ((afxV3d*)data)[k], atv);
                    }

                    normalized = FALSE;
                    break;
                }
                case 1: // tan
                {
                    AfxM3dPostMultiplyV3d(ltm, mshi.vtxCnt, data, data);

                    normalizedFlag = TRUE;
                    normalized = TRUE;
                    break;
                }
                case 2: // bit
                {
                    AfxM3dPostMultiplyV3d(ltm, mshi.vtxCnt, data, data);

                    normalizedFlag = TRUE;
                    normalized = TRUE;
                    break;
                }
                case 3: // tbc
                {
                    if (deltaFlag)
                    {
                        AfxM3dPostMultiplyV3d(ltm, mshi.vtxCnt, data, data);
                    }
                    else
                    {
                        AfxM3dPostMultiplyV3d(iltm, mshi.vtxCnt, data, data);
                    }

                    normalizedFlag = TRUE;
                    normalized = TRUE;
                    break;
                }
                case 4: // nrm
                {
                    if (deltaFlag)
                    {
                        AfxM3dPostMultiplyV3d(ltm, mshi.vtxCnt, data, data);
                    }
                    else
                    {
                        AfxM3dPostMultiplyV3d(iltm, mshi.vtxCnt, data, data);
                    }

                    normalizedFlag = TRUE;
                    normalized = TRUE;
                    break;
                }
                default: break;
                }

                if (renormalize && normalized)
                {
                    if (fmt == avxFormat_RG32f)
                        AfxZeroOrNormalizeArrayedV2d(mshi.vtxCnt, data, data);
                    else if (fmt == avxFormat_RGB32f)
                        AfxZeroOrNormalizeArrayedV3d(mshi.vtxCnt, data, data);
                    else if (fmt == avxFormat_RGBA32f)
                        AfxZeroOrNormalizeArrayedV4d(mshi.vtxCnt, data, data);
                }

#if 0
                if (data && AsxGetVertexAttributeFlags(vtd2, j) & afxVertexUsage_SPATIAL)
                {
                    switch (AsxGetVertexAttributeFormat(vtd2, j))
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
                            AfxM3dPostMultiplyV3d(lt, vtxCnt, data, data);

                        if (invLinearFlag)
                            AfxM3dPostMultiplyV3d(ilt, vtxCnt, data, data);

                        if (affineFlag)
                            for (afxUnit k = 0; k < vtxCnt; k++)
                                AfxV3dAdd(((afxV3d*)data)[k], ((afxV3d*)data)[k], at);

                        if (renormalize)
                            for (afxUnit k = 0; k < vtxCnt; k++)
                                AfxV3dNormalize(((afxV3d*)data)[k], ((afxV3d*)data)[k]);

                        break;
                    }
                    case afxVertexFormat_V4D:
                    {
                        AfxThrowError(); // inoperant

                        if (linearFlag)
                            AfxM3dPostMultiplyLtv4d(lt, vtxCnt, data, data);

                        if (invLinearFlag)
                            AfxM3dPostMultiplyLtv4d(ilt, vtxCnt, data, data);

                        if (affineFlag)
                            for (afxUnit k = 0; k < vtxCnt; k++)
                                AfxV3dAdd(((afxV4d*)data)[k], ((afxV4d*)data)[k], at);

                        if (renormalize)
                            for (afxUnit k = 0; k < vtxCnt; k++)
                                AfxV4dNormalize(((afxV4d*)data)[k], ((afxV4d*)data)[k]);

                        break;
                    }
                    default: AfxThrowError(); break;
                    }
                }
#endif
            }
        }

        // When a mesh primitive uses any triangle-based topology,
        // the determinant of the transform defines the winding order of that mesh. 
        // If the determinant is a positive value, the winding order triangle faces is counterclockwise; 
        // in the opposite case, the winding order is clockwise.

        if ((flags & 2) && (AfxM3dDet(ltm) < 0.0)) // se sinalizado para reordenar índices de faces
            AfxInvertMeshTopology(msh);

        AfxTransformObbs(ltm, atv, msh->biasCnt, msh->biasObb, msh->biasObb);
    }
    return err;
}

_ASX afxUnit AfxEnumerateMeshes(afxSimulation sim, afxUnit first, afxUnit cnt, afxMesh meshes[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxClass const* cls = _AsxGetMeshClass(sim);
    AFX_ASSERT_CLASS(cls, afxFcc_MSH);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)meshes);
}

_ASX afxError AfxBuildMeshes(afxSimulation sim, afxUnit cnt, afxMeshBlueprint const blueprints[], afxMesh meshes[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    AFX_ASSERT(blueprints);
    AFX_ASSERT(meshes);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)_AsxGetMeshClass(sim);
    AFX_ASSERT_CLASS(cls, afxFcc_MSH);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)meshes, (void const*[]) { sim, blueprints }))
    {
        AfxThrowError();
        return err;
    }
    else
    {
#ifdef _ASX_DBG_ECHO_MESH
        /*
            meshes[cnt]
            +-- mesh <name> ptr
            |   +-- attributes[cnt]
            |   |   +-- <pos>
            |   |   +-- <nrm>
            |   |   +-- <uv>
            |   +-- biases[cnt]
            |   |   +-- <bias1>
            |   |   +-- <bias2>
            |   +-- materials[cnt]
            |   |   +-- <material0>
            |   |   +-- <material1>
            |   +-- morphs[cnt]
            |       +-- <morph0>
            |
        */
        AfxReportMessage("\nmeshes[%u]", cnt);
        
        for (afxUnit mshIdx = 0; mshIdx < cnt; mshIdx++)
        {
            afxMesh msh = meshes[mshIdx];

            AfxReportMessage("+-- msh <%.*s> %p", AfxPushString(&msh->urn), msh);
            AfxReportMessage("|   +-- topology : %u", msh->topology);
            AfxReportMessage("|   +-- triCnt : %u", msh->triCnt);
            AfxReportMessage("|   +-- vtxCnt : %u", msh->vtxCnt);

            AfxReportMessage("|   +-- attributes[%u]", msh->attrCnt);
            for (afxUnit i = 0; i < msh->attrCnt; i++)
            {
                AfxReportMessage("|   |   +-- <%.*s>", AfxPushString(&msh->attrIds[i]));
            }

            AfxReportMessage("|   +-- materials[%u]", msh->mtlCnt);
            for (afxUnit i = 0; i < msh->mtlCnt; i++)
            {
                AfxReportMessage("|   |   +-- <%.*s>", AfxPushString(&msh->materials[i]));
            }

            AfxReportMessage("|   +-- sections[%u]", msh->secCnt);
            for (afxUnit i = 0; i < msh->secCnt; i++)
            {
                AfxReportMessage("|   |   +-- [%u, %u] #%u", msh->sections[i].baseTriIdx, msh->sections[i].triCnt, msh->sections[i].mtlIdx);
            }

            AfxReportMessage("|   +-- biases[%u]", msh->biasCnt);
            for (afxUnit i = 0; i < msh->biasCnt; i++)
            {
                AfxReportMessage("|   |   +-- <%.*s>", AfxPushString(&msh->biasId[i]));
            }

            AfxReportMessage("|   +-- morphes[%u]", msh->morphCnt);
            for (afxUnit i = 0; i < msh->morphCnt; i++)
            {
                AfxReportMessage("|   |   +-- <%.*s>", AfxPushString(&msh->morphTags[i]));
            }
        }
#endif//_ASX_DGB_ECHO_MESH
    }
    return err;
}
