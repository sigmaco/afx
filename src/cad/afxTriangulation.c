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
#define _AVX_GEOMETRY_C
#define _AVX_VERTEX_BUILDER_C
#define _AVX_MESH_TOPOLOGY_C
#include "../dev/AvxImplKit.h"

#define MaxNumWeights 8

struct WeightVertex
{
    afxUnit8 BoneWeights[MaxNumWeights];
    afxUnit8 BoneIndices[MaxNumWeights];
    afxUnit BonesUsed;
};

struct TriWeightData
{
    afxUnit jntCnt;
    afxUnit8 jntIdx[MaxNumWeights * 3];
    afxTriangle32 vtxIdx;
};

////////////////////////////////////////////////////////////////////////////////
// MESH                                                                       //
////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxRecomputeTriangularTangents(afxUnit triCnt, afxUnit const indices[], afxV3d const pos[], afxV2d const uv[], afxUnit distro, afxV3d tan[], afxV3d bit[])
{
    afxError err = NIL;
    AFX_ASSERT(indices);
    AFX_ASSERT(pos);
    AFX_ASSERT(uv);
    AFX_ASSERT(tan || bit);

    distro = AfxClamp(distro, 1, 3);

    for (afxUnit i = 0; i < triCnt; i++)
    {
        afxUnit const ia = indices[i * AMX_INDICES_PER_TRI + 0];
        afxUnit const ib = indices[i * AMX_INDICES_PER_TRI + 1];
        afxUnit const ic = indices[i * AMX_INDICES_PER_TRI + 2];

        afxV3d ea, eb; // edges
        AfxV3dSub(ea, pos[ib], pos[ia]);
        AfxV3dSub(eb, pos[ic], pos[ia]);

        afxV2d deltaUv1, deltaUv2;
        AfxV2dSub(deltaUv1, uv[ib], uv[ia]);
        AfxV2dSub(deltaUv2, uv[ic], uv[ia]);
        afxReal f = 1.f / (deltaUv1[0] * deltaUv2[1] - deltaUv2[0] * deltaUv1[1]);

        if (tan)
        {
            tan[ia][0] = f * (deltaUv2[1] * ea[0] - deltaUv1[1] * eb[0]);
            tan[ia][1] = f * (deltaUv2[1] * ea[1] - deltaUv1[1] * eb[1]);
            tan[ia][2] = f * (deltaUv2[1] * ea[2] - deltaUv1[1] * eb[2]);

            // Because a triangle is always a flat shape, we only need to calculate a single tangent/bitangent pair per triangle as they will be the same for each of the triangle's vertices.

            if (distro > 1)
                AfxV3dCopy(tan[ib], tan[ia]);

            if (distro > 2)
                AfxV3dCopy(tan[ic], tan[ia]);
        }

        if (bit)
        {
            bit[ia][0] = f * (-deltaUv2[0] * ea[0] + deltaUv1[0] * eb[0]);
            bit[ia][1] = f * (-deltaUv2[0] * ea[1] + deltaUv1[0] * eb[1]);
            bit[ia][2] = f * (-deltaUv2[0] * ea[2] + deltaUv1[0] * eb[2]);

            // Because a triangle is always a flat shape, we only need to calculate a single tangent/bitangent pair per triangle as they will be the same for each of the triangle's vertices.
            
            if (distro > 1)
                AfxV3dCopy(bit[ib], bit[ia]);

            if (distro > 2)
                AfxV3dCopy(bit[ic], bit[ia]);
        }
    }
    return err;
}

_AVX afxError AfxRecomputeTriangularNormals(afxUnit triCnt, afxUnit const indices[], afxV3d const pos[], afxV3d const nrm[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(indices);
    AFX_ASSERT(pos);
    AFX_ASSERT(nrm);

    // nrm must be already zeroed.

    /*
        For each face on the mesh, calc the face normal (without normalization, as just explained), and directly accumulate this normal in each vertex belonging to the face.
        After you are done with the faces, each vertex will have recieved all the face normals it was supposed to recieve. That simple.
    */

    for (afxUnit j = 0; j < triCnt; j++)
    {
        afxUnit const ia = indices[j * AMX_INDICES_PER_TRI + 0];
        afxUnit const ib = indices[j * AMX_INDICES_PER_TRI + 1];
        afxUnit const ic = indices[j * AMX_INDICES_PER_TRI + 2];

        afxV4d e1, e2, no;
        AfxV4dSub(e1, pos[ia], pos[ib]);
        AfxV4dSub(e2, pos[ic], pos[ib]);
        AfxV3dCross(no, e1, e2);

        AfxV3dAdd(nrm[ia], nrm[ia], no);
        AfxV3dAdd(nrm[ib], nrm[ib], no);
        AfxV3dAdd(nrm[ic], nrm[ic], no);
    }

    AfxNormalizeArrayedV3d(triCnt * AMX_INDICES_PER_TRI, (void*)nrm, (void*)nrm);

    return err;
}

_AVX afxError AfxDestripifyTriangles(afxUnit triCnt, void const* src, afxUnit srcIdxSiz, void* dst, afxUnit dstIdxSiz)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(src);
    AFX_ASSERT(dst);
    afxByte const* src2 = src;
    afxByte* dst2 = dst;
    afxUnit stripLen = triCnt + 2;

    afxUnit stride = AfxMax(dstIdxSiz, AfxMin(dstIdxSiz, srcIdxSiz));

    for (afxUnit i = 0; i < stripLen - 2; i++)
    {
        if (i % 2 == 0)
        {
            // Even index: v0, v1, v2
            for (afxUnit j = 0; j < AMX_INDICES_PER_TRI; j++)
                AfxCopy(&dst2[dstIdxSiz * i + j], &src2[srcIdxSiz * i + j], stride);
        }
        else
        {
            // Odd index: v1, v2, v0
            afxUnit idx0;
            // to allowed in-place conversion, we must cache at least of indices here before swapping.
            AfxCopy(&idx0, &src2[srcIdxSiz * i + 0], stride);
            AfxCopy(&dst2[dstIdxSiz * i + 0], &src2[srcIdxSiz * i + 1], stride);
            AfxCopy(&dst2[dstIdxSiz * i + 1], &src2[srcIdxSiz * i + 2], stride);
            AfxCopy(&dst2[dstIdxSiz * i + 2], &idx0, stride);
        }
    }
    return err;
}


_AVXINL void AfxEndMeshBuilding(afxTriangulation* mshb)
{
    afxError err = NIL;
    //AfxAssertType(mshb, afxFcc_MSHB);

    AfxCleanUpArray(&mshb->biases);

    if (mshb->faces)
        AfxDeallocate(mshb->faces);

    if (mshb->vtx)
        AfxDeallocate(mshb->vtx);

    if (mshb->posn)
        AfxDeallocate(mshb->posn);

    if (mshb->nrm)
        AfxDeallocate(mshb->nrm);

    if (mshb->uv)
        AfxDeallocate(mshb->uv);

    if (mshb->pivots)
        AfxDeallocate(mshb->pivots);

    //AfxAssignFcc(mshb, NIL);
}

_AVXINL afxError AfxBeginMeshBuilding(afxTriangulation* mshb, afxString const* id, afxUnit vtxCnt, afxUnit triCnt, afxUnit secCnt, afxUnit artCnt)
{
    afxError err = NIL;
    AFX_ASSERT(mshb);
    AFX_ASSERT(vtxCnt);
    AFX_ASSERT(triCnt);
    AFX_ASSERT(secCnt);
    AFX_ASSERT(artCnt);
    mshb->vtxCnt = vtxCnt;
    mshb->artCnt = artCnt;
    mshb->triCnt = triCnt;
    mshb->secCnt = secCnt;
    mshb->faces = NIL;

    if (secCnt && !(mshb->faces = AfxAllocate(triCnt, sizeof(mshb->faces[0]), 0, AfxHere()))) AfxThrowError();
    else
    {
        AfxZero2(mshb->faces, sizeof(mshb->faces[0]), triCnt);
    }

    AfxMakeString32(&mshb->id, id);

    AfxMakeArray(&mshb->biases, artCnt, sizeof(afxVertexBias), (afxVertexBias[]) { 0 });

    mshb->vtx = AfxAllocate(vtxCnt, sizeof(mshb->vtx[0]), NIL, AfxHere());
    AfxZero2(mshb->vtx, sizeof(mshb->vtx[0]), vtxCnt);

    mshb->pivots = AfxAllocate(artCnt, sizeof(mshb->pivots[0]), NIL, AfxHere());

    mshb->posn = AfxAllocate(vtxCnt, sizeof(mshb->posn[0]), AFX_SIMD_ALIGNMENT, AfxHere());
    mshb->nrm = NIL;
    mshb->uv = NIL;

    //AfxAssignFcc(mshb, afxFcc_MSHB);

    return err;
}

_AVXINL void AfxUpdateVertices(afxTriangulation* mshb, afxUnit baseVtxIdx, afxUnit vtxCnt, afxUnit const baseBiasIdx[], afxUnit const biasCnt[])
{
    afxError err = NIL;
    //AfxAssertType(mshb, afxFcc_MSHB);
    AFX_ASSERT_RANGE(mshb->vtxCnt, baseVtxIdx, vtxCnt);

    for (afxUnit i = 0; i < vtxCnt; i++)
    {
        akxVertex *v = &mshb->vtx[baseVtxIdx + i];
        //AfxV4dCopyAtv3d(v->posn, posn[i]);
        v->baseBiasIdx = baseBiasIdx ? baseBiasIdx[i] : 0;
        v->biasCnt = biasCnt ? biasCnt[i] : 1;
        //AfxV2dCopy(v->uv, uv ? uv[i] : AfxV2d(0, 0));

        AFX_ASSERT_RANGE(mshb->biases.pop, v->baseBiasIdx, v->biasCnt);
    }
}

_AVXINL void AfxUpdateVertexPositions(afxTriangulation* mshb, afxUnit baseVtxIdx, afxUnit vtxCnt, afxReal const posn[][3], afxUnit srcStride)
{
    afxError err = NIL;
    //AfxAssertType(mshb, afxFcc_MSHB);
    AFX_ASSERT_RANGE(mshb->vtxCnt, baseVtxIdx, vtxCnt);
    AFX_ASSERT(posn);

    if (srcStride == sizeof(posn[0]))
    {
        for (afxUnit i = 0; i < vtxCnt; i++)
            AfxV4dCopyAtv3d(mshb->posn[baseVtxIdx + i], posn[i]);
    }
    else
    {
        afxByte* posnBytemap = (void*)posn;
        
        for (afxUnit i = 0; i < vtxCnt; i++)
            AfxV4dCopyAtv3d(mshb->posn[baseVtxIdx + i], (void*)(&posnBytemap[i * srcStride]));
    }
}

_AVXINL void AfxUpdateVertexPositions4(afxTriangulation* mshb, afxUnit baseVtxIdx, afxUnit vtxCnt, afxV4d const posn[], afxUnit srcStride)
{
    afxError err = NIL;
    //AfxAssertType(mshb, afxFcc_MSHB);
    AFX_ASSERT_RANGE(mshb->vtxCnt, baseVtxIdx, vtxCnt);
    AFX_ASSERT(posn);

    if (srcStride == sizeof(posn[0]))
    {
        AfxCopyArrayedV4d(vtxCnt, posn, &mshb->posn[baseVtxIdx]);
    }
    else
    {
        afxByte* posnBytemap = (void*)posn;

        for (afxUnit i = 0; i < vtxCnt; i++)
            AfxV4dCopy(mshb->posn[baseVtxIdx + i], (void*)(&posnBytemap[i * srcStride]));
    }
}

_AVXINL afxError AfxUpdateVertexNormals(afxTriangulation* mshb, afxUnit baseVtxIdx, afxUnit vtxCnt, afxV3d const nrm[], afxUnit srcStride)
{
    afxError err = NIL;
    //AfxAssertType(mshb, afxFcc_MSHB);
    AFX_ASSERT_RANGE(mshb->vtxCnt, baseVtxIdx, vtxCnt);
    AFX_ASSERT(nrm);

    if (!mshb->nrm && !(mshb->nrm = AfxAllocate(mshb->vtxCnt, sizeof(mshb->nrm[0]), AFX_SIMD_ALIGNMENT, AfxHere())))
        AfxThrowError();

    if (!err)
    {
        if (srcStride == sizeof(nrm[0]))
        {
            AfxCopyArrayedV3d(vtxCnt, nrm, &mshb->nrm[baseVtxIdx]);
        }
        else
        {
            afxByte* nrmBytemap = (void*)nrm;

            for (afxUnit i = 0; i < vtxCnt; i++)
                AfxV3dCopy(mshb->nrm[baseVtxIdx + i], (void*)(&nrmBytemap[i * srcStride]));
        }
    }
    return err;
}

_AVXINL afxError AfxUpdateVertexWraps(afxTriangulation* mshb, afxUnit baseVtxIdx, afxUnit vtxCnt, afxV2d const uv[], afxUnit srcStride)
{
    afxError err = NIL;
    //AfxAssertType(mshb, afxFcc_MSHB);
    AFX_ASSERT_RANGE(mshb->vtxCnt, baseVtxIdx, vtxCnt);
    AFX_ASSERT(uv);

    if (!mshb->uv && !(mshb->uv = AfxAllocate(mshb->vtxCnt, sizeof(mshb->uv[0]), AFX_SIMD_ALIGNMENT, AfxHere())))
        AfxThrowError();

    if (!err)
    {
        if (srcStride == sizeof(uv[0]))
        {
            AfxCopyArrayedV2d(vtxCnt, uv, &mshb->uv[baseVtxIdx]);
        }
        else
        {
            afxByte* uvBytemap = (void*)uv;

            for (afxUnit i = 0; i < vtxCnt; i++)
                AfxV2dCopy(mshb->uv[baseVtxIdx + i], (void*)(&uvBytemap[i * srcStride]));
        }
    }
    
    return err;
}

_AVXINL void AfxEmitTriangles(afxTriangulation* mshb, afxUnit secIdx, afxUnit baseTriIdx, afxUnit triCnt, afxUnit const triVtxIdx[][3])
{
    afxError err = NIL;
    //AfxAssertType(mshb, afxFcc_MSHB);
    AFX_ASSERT_RANGE(mshb->secCnt, secIdx, 1);
    AFX_ASSERT_RANGE(mshb->triCnt, baseTriIdx, triCnt);

    for (afxUnit i = 0; i < triCnt; i++)
    {
        mshb->faces[baseTriIdx + i].vtxIdx[0] = triVtxIdx[i][0];
        mshb->faces[baseTriIdx + i].vtxIdx[1] = triVtxIdx[i][1];
        mshb->faces[baseTriIdx + i].vtxIdx[2] = triVtxIdx[i][2];
        mshb->faces[baseTriIdx + i].secIdx = secIdx;
        mshb->faces[baseTriIdx + i].artCnt = 0;

        AFX_ASSERT(secIdx == mshb->faces[baseTriIdx + i].secIdx);
        AFX_ASSERT_RANGE(mshb->secCnt, mshb->faces[baseTriIdx + i].secIdx, 1);
    }
}

_AVXINL afxUnit AfxAddVertexBiases(afxTriangulation* mshb, afxUnit cnt, afxUnit const jntIdx[], afxReal const weight[])
{
    afxError err = NIL;
    //AfxAssertType(mshb, afxFcc_MSHB);
    AfxReserveArraySpace(&mshb->biases, mshb->biases.pop + cnt);

    afxUnit baseBiasIdx = AFX_INVALID_INDEX;
    AfxInsertArrayUnits(&mshb->biases, cnt, &baseBiasIdx, NIL);

    for (afxUnit i = 0; i < cnt; i++)
    {
        AFX_ASSERT(weight && 1.f >= weight[i]);
        AfxUpdateArray(&mshb->biases, baseBiasIdx + i, 1, (const afxVertexBias[]) { { .pivotIdx = jntIdx ? jntIdx[i] : 0, .weight = weight ? weight[i] : 1.f } });
        AFX_ASSERT_RANGE(mshb->artCnt, ((afxVertexBias const*)AfxGetArrayUnit(&mshb->biases, baseBiasIdx + i))->pivotIdx, 1);
    }
    return baseBiasIdx;
}

_AVXINL void AfxRenameVertexPivots(afxTriangulation* mshb, afxUnit basePivotIdx, afxUnit cnt, afxString const name[])
{
    afxError err = NIL;
    //AfxAssertType(mshb, afxFcc_MSHB);
    AFX_ASSERT_RANGE(mshb->artCnt, basePivotIdx, cnt);
    AFX_ASSERT(name);

    for (afxUnit i = 0; i < cnt; i++)
    {
        AfxReflectString(&name[i], &mshb->pivots[basePivotIdx + i]);
    }
}

_AVX afxMesh AfxBuildParallelepipedMesh(afxDrawInput din, afxV3d whd, afxReal slantX, afxReal slantY)
{
    afxError err = NIL;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AFX_ASSERT(whd);

    if (!slantX)
        slantX = 0.5f;

    if (!slantY)
        slantY = 0.5f;

    afxUnit numVertices = 8;
    afxUnit numIndices = 36; // 12 triangles * 3 vertices per triangle

    afxReal halfWidth = whd[0] / 2.0f;
    afxReal halfHeight = whd[1] / 2.0f;
    afxReal halfDepth = whd[2] / 2.0f;

    // Define the 8 vertices of the parallelepiped
    afxV3d const vertData[8] =
    {
        {-halfWidth - slantX, -halfHeight - slantY, -halfDepth},
        {halfWidth - slantX, -halfHeight - slantY, -halfDepth},
        {halfWidth - slantX, halfHeight - slantY, -halfDepth},
        {-halfWidth - slantX, halfHeight - slantY, -halfDepth},
        {-halfWidth + slantX, -halfHeight + slantY, halfDepth},
        {halfWidth + slantX, -halfHeight + slantY, halfDepth},
        {halfWidth + slantX, halfHeight + slantY, halfDepth},
        {-halfWidth + slantX, halfHeight + slantY, halfDepth}
    };

    // Define indices for the 12 triangles of the parallelepiped
    afxUnit const indicesData[36] =
    {
        0, 1, 2, 2, 3, 0, // Back face
        4, 5, 6, 6, 7, 4, // Front face
        0, 1, 5, 5, 4, 0, // Bottom face
        2, 3, 7, 7, 6, 2, // Top face
        0, 3, 7, 7, 4, 0, // Left face
        1, 2, 6, 6, 5, 1  // Right face
    };

    afxGeometryInfo vtdb = { 0 };
    vtdb.attrCnt = 3;
    vtdb.vtxCnt = numVertices;

    afxGeometry geo;
    AfxAcquireGeometries(din, 1, &vtdb, &geo);

    AfxResetGeometry(geo, 0, afxVertexFormat_V3D, afxVertexFlag_POSITIONAL | afxVertexFlag_SPATIAL | afxVertexFlag_ATV, &AfxString("pos"));
    AfxResetGeometry(geo, 1, afxVertexFormat_V3D, afxVertexFlag_NORMALIZED | afxVertexFlag_LTM, &AfxString("nrm"));
    AfxResetGeometry(geo, 2, afxVertexFormat_V2D, afxVertexFlag_NORMALIZED | afxVertexFlag_LTM, &AfxString("uv"));

    AfxUpdateGeometry(geo, 0, 0, vtdb.vtxCnt, vertData, sizeof(vertData[0]));

    afxV3d* nrm = AfxAccessGeometry(geo, 1, 0);
    afxV2d* uv = AfxAccessGeometry(geo, 2, 0);

    afxUnit index = 0;

    for (afxUnit i = 0; i < 8; ++i)
    {
        nrm[index][0] = 0.0f; // Normals
        nrm[index][1] = 0.0f;
        nrm[index][2] = 1.0f;
        uv[index][0] = (i % 2) ? 1.0f : 0.0f; // Texture coordinates
        uv[index][1] = (i / 4) ? 1.0f : 0.0f;
        index++;
    }

    afxMeshBlueprint mshb = { 0 };
    mshb.vtxCnt = numVertices;
    mshb.geo = geo;
    mshb.biasCnt = 1;
    mshb.triCnt = numIndices / 3;
    
    afxMesh msh;
    AfxAssembleMeshes(din, 1, &mshb, &msh);
    AfxReleaseObjects(1, &geo);

    AfxUpdateMeshIndices(msh, 0, mshb.triCnt, indicesData, sizeof(indicesData[0]));

    return msh;
}

_AVX afxMesh AfxBuildDomeMesh2(afxDrawInput din, afxReal radius, afxUnit stacks, afxUnit slices, afxBool inv)
{
    afxError err = NIL;
    AfxAssertObjects(1, &din, afxFcc_DIN);

    if (!radius)
        radius = 1.f;

    if (!stacks)
        stacks = 10;

    if (!slices)
        slices = 20;

    afxUnit numVertices = (stacks + 1) * (slices + 1);
    
    afxGeometryInfo vtdb = { 0 };
    vtdb.attrCnt = 3;
    vtdb.vtxCnt = numVertices;

    afxGeometry geo;
    AfxAcquireGeometries(din, 1, &vtdb, &geo);

    AfxResetGeometry(geo, 0, afxVertexFormat_V3D, afxVertexFlag_POSITIONAL | afxVertexFlag_SPATIAL | afxVertexFlag_ATV, &AfxString("pos"));
    AfxResetGeometry(geo, 1, afxVertexFormat_V3D, afxVertexFlag_NORMALIZED | afxVertexFlag_LTM, &AfxString("nrm"));
    AfxResetGeometry(geo, 2, afxVertexFormat_V2D, afxVertexFlag_NORMALIZED | afxVertexFlag_LTM, &AfxString("uv"));

    afxV3d* pos = AfxAccessGeometry(geo, 0, 0);
    afxV3d* nrm = AfxAccessGeometry(geo, 1, 0);
    afxV2d* uv = AfxAccessGeometry(geo, 2, 0);
    afxUnit index = 0;

    for (afxUnit i = 0; i <= stacks; ++i)
    {
        afxReal stackAngle = AFX_PI / 2 - i * AFX_PI / stacks; // angle from top
        afxReal xy = radius * AfxCosf(stackAngle); // radius of current stack
        afxReal z = radius * AfxSinf(stackAngle);  // height of current stack

        for (afxUnit j = 0; j <= slices; ++j)
        {
            afxReal sectorAngle = 2 * AFX_PI * j / slices; // angle around the stack
            afxReal x = xy * AfxCosf(sectorAngle);
            afxReal y = xy * AfxSinf(sectorAngle);

            AfxV3dSet(pos[index], x, y, z);
            
            // Normal vector (normalized vertex position)
            afxReal length = AfxSqrtf(x * x + y * y + z * z);
            AfxV3dSet(nrm[index], x / length, y / length, z / length);
            AfxV2dSet(uv[index], (afxReal)j / slices, (afxReal)i / stacks);
            index++;
        }
    }
    
    afxUnit numIndices = stacks * slices * 2 * 3; // 2 triangles per quad, 3 indices per triangle
    //afxUnit numIndices = stacks * slices * 6; // 6 indices per quad (2 triangles, 3 indices per triangle)

    afxMeshBlueprint mshb = { 0 };
    mshb.vtxCnt = numVertices;
    mshb.geo = geo;
    mshb.biasCnt = 1;
    mshb.triCnt = numIndices / 3;

    afxMesh msh;
    AfxAssembleMeshes(din, 1, &mshb, &msh);
    AfxReleaseObjects(1, &geo);

#if 0 // we couldn't use trip strip at the time this code was written.
    afxBool strip = FALSE;

    if (strip)
    {
        afxMeshTopologyBlueprint mshtb = { 0 };
        mshtb.triCnt = numIndices / 3;
        AfxAssembleMeshTopology(din, 1, &mshtb, &msht);

        afxIndexedTriangle* tris = AfxGetMeshTriangles(msht, 0);

        // Generate indices for triangle strips
        afxUnit idx = 0;

        for (afxUnit i = 0; i < stacks; ++i)
        {
            for (afxUnit j = 0; j < slices; ++j)
            {
                afxUnit topLeft = i * (slices + 1) + j;
                afxUnit topRight = topLeft + 1;
                afxUnit bottomLeft = (i + 1) * (slices + 1) + j;
                afxUnit bottomRight = bottomLeft + 1;

                // First triangle
                tris[idx][0] = topLeft;
                tris[idx][1] = bottomLeft;
                tris[idx][2] = topRight;
                idx++;

                // Second triangle
                tris[idx][0] = topRight;
                tris[idx][1] = bottomLeft;
                tris[idx][2] = bottomRight;
                idx++;
            }
        }
    }
    else
#endif
    {
        afxUnit* indices = AfxGetMeshIndices(msh, 0);

        // Generate indices for triangles
        afxUnit idx = 0;

        for (afxUnit i = 0; i < stacks; ++i)
        {
            for (afxUnit j = 0; j < slices; ++j)
            {
                afxUnit topLeft = i * (slices + 1) + j;
                afxUnit topRight = topLeft + 1;
                afxUnit bottomLeft = (i + 1) * (slices + 1) + j;
                afxUnit bottomRight = bottomLeft + 1;

                // First triangle
                indices[idx + 0] = topLeft;
                indices[idx + 1] = bottomLeft;
                indices[idx + 2] = topRight;
                idx++;

                // Second triangle
                indices[idx + 0] = topRight;
                indices[idx + 1] = bottomLeft;
                indices[idx + 2] = bottomRight;
                idx++;
            }
        }
    }

    if (inv)
    {
        AfxInvertMeshTopology(msh);
        AfxInvertGeometry(geo, 1, 0, numVertices);
    }

    return msh;
}

_AVX afxMesh AfxBuildCapsuleMesh(afxDrawInput din, afxReal radius, afxReal height, afxUnit stacks, afxUnit slices, afxUnit cylinderSlices, afxBool inv)
{
    afxError err = NIL;
    AfxAssertObjects(1, &din, afxFcc_DIN);

    if (!radius)
        radius = 1.f; // Radius of the sphere caps

    if (!height)
        height = 2.f; // Height of the cylindrical part

    if (!stacks)
        stacks = 20; // Number of stacks for the sphere

    if (!slices)
        slices = 20; // Number of slices for the sphere

    if (!cylinderSlices)
        cylinderSlices = 36; // Number of slices for the cylinder

    afxUnit numVerticesSphere = (stacks + 1) * (slices + 1);
    afxUnit numVerticesCylinder = (cylinderSlices + 1) * 2; // Top and bottom circles
    afxUnit numVertices = numVerticesSphere * 2 + numVerticesCylinder; // 2 hemispheres and 1 cylinder
    afxUnit numIndicesSphere = stacks * slices * 6; // Each hemisphere has these many indices
    afxUnit numIndicesCylinder = cylinderSlices * 6; // Cylinder indices
    afxUnit numIndices = numIndicesSphere * 2 + numIndicesCylinder; // Total indices

    afxGeometryInfo vtdb = { 0 };
    vtdb.attrCnt = 3;
    vtdb.vtxCnt = numVertices;

    afxGeometry geo;
    AfxAcquireGeometries(din, 1, &vtdb, &geo);

    AfxResetGeometry(geo, 0, afxVertexFormat_V3D, afxVertexFlag_POSITIONAL | afxVertexFlag_SPATIAL | afxVertexFlag_ATV, &AfxString("pos"));
    AfxResetGeometry(geo, 1, afxVertexFormat_V3D, afxVertexFlag_NORMALIZED | afxVertexFlag_LTM, &AfxString("nrm"));
    AfxResetGeometry(geo, 2, afxVertexFormat_V2D, afxVertexFlag_NORMALIZED | afxVertexFlag_LTM, &AfxString("uv"));

    afxV3d* pos = AfxAccessGeometry(geo, 0, 0);
    afxV3d* nrm = AfxAccessGeometry(geo, 1, 0);
    afxV2d* uv = AfxAccessGeometry(geo, 2, 0);
    afxUnit index = 0;

    // Generate spherical caps (top and bottom)
    for (afxUnit hemisphere = 0; hemisphere < 2; ++hemisphere)
    {
        afxReal zOffset = (hemisphere == 0) ? (height / 2) : (-height / 2);

        for (afxUnit i = 0; i <= stacks; ++i)
        {
            afxReal stackAngle = AFX_PI * i / stacks; // angle from top
            afxReal xy = radius * sinf(stackAngle); // radius of current stack
            afxReal z = zOffset + radius * cosf(stackAngle);  // height of current stack

            if (hemisphere == 1) {
                z = -z; // Flip z for the bottom hemisphere
            }

            for (afxUnit j = 0; j <= slices; ++j)
            {
                afxReal sectorAngle = 2 * AFX_PI * j / slices; // angle around the stack
                afxReal x = xy * cosf(sectorAngle);
                afxReal y = xy * sinf(sectorAngle);

                AfxV3dSet(pos[index], x, y, z);

                // Normal vector (normalized vertex position)
                afxReal length = sqrtf(x * x + y * y + z * z);
                AfxV3dSet(nrm[index], x / length, y / length, z / length);
                AfxV2dSet(uv[index], (afxReal)j / slices, (afxReal)i / stacks);
                index++;
            }
        }
    }

    // Generate cylindrical section
    for (afxUnit i = 0; i <= 1; ++i)
    {
        afxReal yOffset = (i == 0) ? (-height / 2) : (height / 2); // Top and bottom circles

        for (afxUnit j = 0; j <= cylinderSlices; ++j)
        {
            afxReal angle = 2 * AFX_PI * j / cylinderSlices;
            afxReal x = radius * cosf(angle);
            afxReal y = yOffset;
            afxReal z = radius * sinf(angle);

            AfxV3dSet(pos[index], x, y, z);

            // Normal vector (perpendicular to the cylinder surface)
            AfxV3dSet(nrm[index], 0.f, (i == 0) ? -1.0f : 1.0f, 0.f);

            AfxV2dSet(uv[index], (afxReal)j / cylinderSlices, (i == 0) ? 0.0f : 1.0f);
            index++;
        }
    }

    afxMeshBlueprint mshb = { 0 };
    mshb.vtxCnt = numVertices;
    mshb.geo = geo;
    mshb.biasCnt = 1;
    mshb.triCnt = numIndices / 3;

    afxMesh msh;
    AfxAssembleMeshes(din, 1, &mshb, &msh);
    AfxReleaseObjects(1, &geo);

    {
        afxUnit* indices = AfxGetMeshIndices(msh, 0);

        // Generate indices for spherical caps
        afxUnit idx = 0;

        for (afxUnit hemisphere = 0; hemisphere < 2; ++hemisphere)
        {
            for (afxUnit i = 0; i < stacks; ++i)
            {
                for (afxUnit j = 0; j < slices; ++j)
                {
                    afxUnit topLeft = (i * (slices + 1) + j) + (hemisphere * numVerticesSphere);
                    afxUnit topRight = topLeft + 1;
                    afxUnit bottomLeft = ((i + 1) * (slices + 1) + j) + (hemisphere * numVerticesSphere);
                    afxUnit bottomRight = bottomLeft + 1;

                    // First triangle
                    indices[idx + 0] = topLeft;
                    indices[idx + 1] = bottomLeft;
                    indices[idx + 2] = topRight;
                    idx++;

                    // Second triangle
                    indices[idx + 0] = topRight;
                    indices[idx + 1] = bottomLeft;
                    indices[idx + 2] = bottomRight;
                    idx++;
                }
            }
        }

        // Generate indices for cylindrical section
        for (afxUnit i = 0; i < cylinderSlices; ++i)
        {
            afxUnit topLeft = i;
            afxUnit topRight = i + 1;
            afxUnit bottomLeft = numVerticesCylinder + i;
            afxUnit bottomRight = numVerticesCylinder + i + 1;

            // First triangle
            indices[idx + 0] = topLeft;
            indices[idx + 1] = bottomLeft;
            indices[idx + 2] = topRight;
            idx++;

            // Second triangle
            indices[idx + 0] = topRight;
            indices[idx + 1] = bottomLeft;
            indices[idx + 2] = bottomRight;
            idx++;
        }
    }

    if (inv)
    {
        AfxInvertMeshTopology(msh);
        AfxInvertGeometry(geo, 1, 0, numVertices);
    }

    return msh;
}

_AVX afxMesh AfxBuildPlaneMesh(afxDrawInput din, afxUnit gridSizeX, afxUnit gridSizeY, afxReal width, afxReal height)
{
    afxError err = NIL;
    AfxAssertObjects(1, &din, afxFcc_DIN);

    if (!gridSizeX)
        gridSizeX = 10;

    if (!gridSizeY)
        gridSizeY = 10;

    if (!width)
        width = 10.f;

    if (!height)
        height = 10.f;

    afxUnit numVertices = (gridSizeX + 1) * (gridSizeY + 1);
    afxUnit numIndices = gridSizeX * gridSizeY * 6; // Each grid cell is split into 2 triangles
    afxReal halfWidth = width / 2.0f;
    afxReal halfHeight = height / 2.0f;

    afxGeometryInfo vtdb = { 0 };
    vtdb.attrCnt = 3;
    vtdb.vtxCnt = numVertices;

    afxGeometry geo;
    AfxAcquireGeometries(din, 1, &vtdb, &geo);

    AfxResetGeometry(geo, 0, afxVertexFormat_V3D, afxVertexFlag_POSITIONAL | afxVertexFlag_SPATIAL | afxVertexFlag_ATV, &AfxString("pos"));
    AfxResetGeometry(geo, 1, afxVertexFormat_V3D, afxVertexFlag_NORMALIZED | afxVertexFlag_LTM, &AfxString("nrm"));
    AfxResetGeometry(geo, 2, afxVertexFormat_V2D, afxVertexFlag_NORMALIZED | afxVertexFlag_LTM, &AfxString("uv"));

    afxV3d* pos = AfxAccessGeometry(geo, 0, 0);
    afxV3d* nrm = AfxAccessGeometry(geo, 1, 0);
    afxV2d* uv = AfxAccessGeometry(geo, 2, 0);

    // Generate vertices
    afxUnit index = 0;

    for (afxUnit y = 0; y <= gridSizeY; ++y)
    {
        for (afxUnit x = 0; x <= gridSizeX; ++x)
        {
            afxReal xpos = (x / (afxReal)gridSizeX) * width - halfWidth;
            afxReal ypos = (y / (afxReal)gridSizeY) * height - halfHeight;

            // Vertex position
            AfxV3dSet(pos[index], xpos, 0.f, ypos); // Plane is at y = 0
            AfxV3dSet(nrm[index], 0.f, 1.f, 0.f); // Normal vector (pointing up)
            AfxV2dSet(uv[index], x / (afxReal)gridSizeX, y / (afxReal)gridSizeY);
            index++;
        }
    }

    afxMeshBlueprint mshb = { 0 };
    mshb.vtxCnt = numVertices;
    mshb.geo = geo;
    mshb.biasCnt = 1;
    mshb.triCnt = numIndices / 3;

    afxMesh msh;
    AfxAssembleMeshes(din, 1, &mshb, &msh);
    AfxReleaseObjects(1, &geo);

    {
        afxUnit* indices = AfxGetMeshIndices(msh, 0);

        // Generate indices
        afxUnit idx = 0;

        for (afxUnit y = 0; y < gridSizeY; ++y)
        {
            for (afxUnit x = 0; x < gridSizeX; ++x)
            {
                afxUnit topLeft = y * (gridSizeX + 1) + x;
                afxUnit topRight = topLeft + 1;
                afxUnit bottomLeft = (y + 1) * (gridSizeX + 1) + x;
                afxUnit bottomRight = bottomLeft + 1;

                // First triangle
                indices[idx + 0] = topLeft;
                indices[idx + 1] = bottomLeft;
                indices[idx + 2] = topRight;
                idx++;

                // Second triangle
                indices[idx + 0] = topRight;
                indices[idx + 1] = bottomLeft;
                indices[idx + 2] = bottomRight;
                idx++;
            }
        }
    }

    return msh;
}

// TESTED

/**
    The AfxBuildSphereMesh() function uses a right-handed coordinate system to create a mesh containing a sphere.

    @param radius Radius of the sphere. This value should be greater than or equal to 0.0f.
    @param slices Number of slices about the main axis.
    @param stacks Number of stacks along the main axis.
    @param mesh Address of a pointer to the output shape, an afxMesh instance.
*/

_AVX afxMesh AfxBuildSphereMesh(afxDrawInput din, afxReal radius, afxUnit stacks, afxUnit slices, afxBool inv)
{
    afxError err = NIL;
    AfxAssertObjects(1, &din, afxFcc_DIN);

    if (!radius)
        radius = 1.f;

    if (!stacks)
        stacks = 20;

    if (!slices)
        slices = 20;

    afxUnit numVertices = (stacks + 1) * (slices + 1);

    afxGeometryInfo vtdb = { 0 };
    vtdb.attrCnt = 3;
    vtdb.vtxCnt = numVertices;

    afxGeometry geo;
    AfxAcquireGeometries(din, 1, &vtdb, &geo);

    AfxResetGeometry(geo, 0, afxVertexFormat_V3D, afxVertexFlag_POSITIONAL | afxVertexFlag_SPATIAL | afxVertexFlag_ATV, &AfxString("pos"));
    AfxResetGeometry(geo, 1, afxVertexFormat_V3D, afxVertexFlag_NORMALIZED | afxVertexFlag_LTM, &AfxString("nrm"));
    AfxResetGeometry(geo, 2, afxVertexFormat_V2D, afxVertexFlag_NORMALIZED | afxVertexFlag_LTM, &AfxString("uv"));

    afxV3d* pos = AfxAccessGeometry(geo, 0, 0);
    afxV3d* nrm = AfxAccessGeometry(geo, 1, 0);
    afxV2d* uv = AfxAccessGeometry(geo, 2, 0);
    afxUnit index = 0;

    for (afxUnit i = 0; i <= stacks; ++i)
    {
        afxReal stackAngle = AFX_PI * i / stacks; // angle from top
        afxReal xy = radius * AfxSinf(stackAngle); // radius of current stack
        afxReal z = radius * AfxCosf(stackAngle);  // height of current stack

        for (afxUnit j = 0; j <= slices; ++j)
        {
            afxReal sectorAngle = 2 * AFX_PI * j / slices; // angle around the stack
            afxReal x = xy * AfxCosf(sectorAngle);
            afxReal y = xy * AfxSinf(sectorAngle);

            AfxV3dSet(pos[index], x, y, z);

            // Normal vector (normalized vertex position)
            afxReal length = AfxSqrtf(x * x + y * y + z * z);
            AfxV3dSet(nrm[index], x / length, y / length, z / length);
            AfxV2dSet(uv[index], (afxReal)j / slices, (afxReal)i / stacks);
            index++;
        }
    }

    afxUnit numIndices = stacks * slices * 2 * 3; // 2 triangles per quad, 3 indices per triangle
    //afxUnit numIndices = stacks * slices * 6; // 6 indices per quad (2 triangles, 3 indices per triangle)

    afxMeshBlueprint mshb = { 0 };
    mshb.vtxCnt = numVertices;
    mshb.geo = geo;
    mshb.biasCnt = 1;
    mshb.triCnt = numIndices / 3;

    afxMesh msh;
    AfxAssembleMeshes(din, 1, &mshb, &msh);

    // we couldn't use trip strip at the time this code was written.
    afxBool strip = FALSE;

    if (strip)
    {
        afxUnit* indices = AfxGetMeshIndices(msh, 0);

        // Generate indices for triangle strips
        afxUnit idx = 0;

        for (afxUnit i = 0; i < stacks; ++i)
        {
            for (afxUnit j = 0; j < slices; ++j)
            {
                afxUnit topLeft = i * (slices + 1) + j;
                afxUnit topRight = topLeft + 1;
                afxUnit bottomLeft = (i + 1) * (slices + 1) + j;
                afxUnit bottomRight = bottomLeft + 1;

                // First triangle
                indices[idx + 0] = topLeft;
                indices[idx + 1] = bottomLeft;
                indices[idx + 2] = topRight;
                idx++;

                // Second triangle
                indices[idx + 0] = topRight;
                indices[idx + 1] = bottomLeft;
                indices[idx + 2] = bottomRight;
                idx++;
            }
        }
    }
    else
    {
        afxUnit* indices = AfxGetMeshIndices(msh, 0);

        // Generate indices for triangles
        afxUnit idx = 0;

        for (afxUnit i = 0; i < stacks; ++i)
        {
            for (afxUnit j = 0; j < slices; ++j)
            {
                afxUnit topLeft = i * (slices + 1) + j;
                afxUnit topRight = topLeft + 1;
                afxUnit bottomLeft = (i + 1) * (slices + 1) + j;
                afxUnit bottomRight = bottomLeft + 1;

                // First triangle
                indices[idx + 0] = topLeft;
                indices[idx + 1] = bottomLeft;
                indices[idx + 2] = topRight;
                idx++;

                // Second triangle
                indices[idx + 0] = topRight;
                indices[idx + 1] = bottomLeft;
                indices[idx + 2] = bottomRight;
                idx++;
            }
        }
    }

    if (inv)
    {
        AfxInvertMeshTopology(msh);
        AfxInvertGeometry(geo, 1, 0, numVertices);
    }

    AfxReleaseObjects(1, &geo);

    return msh;
}

/**
    The AfxBuildTorusMesh() function uses a right-handed coordinate system to create a mesh containing a torus.

    @param innerRadius Inner-radius of the torus. Value should be greater than or equal to 0.0f.
    @param outerRadius Outer-radius of the torus. Value should be greater than or equal to 0.0f.
    @param sides Number of sides in a cross-section. Value must be greater than or equal to 3.
    @param rings Number of rings making up the torus. Value must be greater than or equal to 3.
    @param mesh Address of a pointer to the output shape, an afxMesh instance.
*/

_AVX afxError AfxBuildTorusMesh(afxDrawInput din, afxReal innerRadius, afxReal outerRadius, afxUnit sides, afxUnit rings, afxMesh* mesh)
{
    afxError err = NIL;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AFX_ASSERT(mesh);


    return err;
}

/**
    The AfxBuildCylinderMesh() function uses a right-handed coordinate system to create a mesh containing a cylinder.
    
    @param radius1 Radius at the negative Z end. Value should be greater than or equal to 0.0f.
    @param radius2 Radius at the positive Z end. Value should be greater than or equal to 0.0f.
    @param len Length of the cylinder along the z-axis.
    @param slices Number of slices about the main axis.
    @param stacks Number of stacks along the main axis.
    @param mesh Address of a pointer to the output shape, an afxMesh instance.
*/

_AVX afxError AfxBuildCylinderMesh(afxDrawInput din, afxReal radius1, afxReal radius2, afxReal len, afxUnit slices, afxUnit stacks, afxMesh* mesh)
{
    afxError err = NIL;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AFX_ASSERT(mesh);
    AFX_ASSERT(radius1 >= 0.f);
    AFX_ASSERT(radius1 >= 0.f);
    return err;
}

/**
    The AfxBuildBoxMesh() function uses a right-handed coordinate system to create a mesh containing an axis-aligned box.

    @param whd Width, height and depth of the box, along the X, Y and Z-axis respectively.
    @param secCnt Maximum number of sections to segment the mesh.
    @param mesh Address of a pointer to the output shape, an afxMesh instance.
*/

_AVX afxError AfxBuildBoxMesh(afxDrawInput din, afxV3d const whd, afxUnit secCnt, afxMesh* mesh)
{
    afxError err = NIL;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AFX_ASSERT(mesh);
    AFX_ASSERT(whd);

    static afxV3d const cubeVertices[] =
    {
        {-0.5f,-0.5f,-0.5f }, {-0.5f,-0.5f, 0.5f }, { 0.5f,-0.5f, 0.5f }, { 0.5f,-0.5f,-0.5f },
        {-0.5f, 0.5f,-0.5f }, {-0.5f, 0.5f, 0.5f }, { 0.5f, 0.5f, 0.5f }, { 0.5f, 0.5f,-0.5f },
        {-0.5f,-0.5f,-0.5f }, {-0.5f, 0.5f,-0.5f }, { 0.5f, 0.5f,-0.5f }, { 0.5f,-0.5f,-0.5f },
        {-0.5f,-0.5f, 0.5f }, {-0.5f, 0.5f, 0.5f }, { 0.5f, 0.5f, 0.5f }, { 0.5f,-0.5f, 0.5f },
        {-0.5f,-0.5f,-0.5f }, {-0.5f,-0.5f, 0.5f }, {-0.5f, 0.5f, 0.5f }, {-0.5f, 0.5f,-0.5f },
        { 0.5f,-0.5f,-0.5f }, { 0.5f,-0.5f, 0.5f }, { 0.5f, 0.5f, 0.5f }, { 0.5f, 0.5f,-0.5f }
    };

    static afxV3d const cubeNormals[] =
    {
        { 0.f,-1.f, 0.f }, { 0.f,-1.f, 0.f }, { 0.f,-1.f, 0.f }, { 0.f,-1.f, 0.f },
        { 0.f, 1.f, 0.f }, { 0.f, 1.f, 0.f }, { 0.f, 1.f, 0.f }, { 0.f, 1.f, 0.f },
        { 0.f, 0.f,-1.f }, { 0.f, 0.f,-1.f }, { 0.f, 0.f,-1.f }, { 0.f, 0.f,-1.f },
        { 0.f, 0.f, 1.f }, { 0.f, 0.f, 1.f }, { 0.f, 0.f, 1.f }, { 0.f, 0.f, 1.f },
        {-1.f, 0.f, 0.f }, {-1.f, 0.f, 0.f }, {-1.f, 0.f, 0.f }, {-1.f, 0.f, 0.f },
        { 1.f, 0.f, 0.f }, { 1.f, 0.f, 0.f }, { 1.f, 0.f, 0.f }, { 1.f, 0.f, 0.f }
    };

    static afxV3d const cubeTangents[] =
    {
        { 1.f, 0.f, 0.f }, { 1.f, 0.f, 0.f }, { 1.f, 0.f, 0.f }, { 1.f, 0.f, 0.f },
        { 1.f, 0.f, 0.f }, { 1.f, 0.f, 0.f }, { 1.f, 0.f, 0.f }, { 1.f, 0.f, 0.f },
        {-1.f, 0.f, 0.f }, {-1.f, 0.f, 0.f }, {-1.f, 0.f, 0.f }, {-1.f, 0.f, 0.f },
        { 1.f, 0.f, 0.f }, { 1.f, 0.f, 0.f }, { 1.f, 0.f, 0.f }, { 1.f, 0.f, 0.f },
        { 0.f, 0.f, 1.f }, { 0.f, 0.f, 1.f }, { 0.f, 0.f, 1.f }, { 0.f, 0.f, 1.f },
        { 0.f, 0.f,-1.f }, { 0.f, 0.f,-1.f }, { 0.f, 0.f,-1.f }, { 0.f, 0.f,-1.f }
    };

    static afxV2d const cubeTexCoords[] =
    {
        { 0.f, 0.f }, { 0.f, 1.f }, { 1.f, 1.f }, { 1.f, 0.f },
        { 0.f, 1.f }, { 0.f, 0.f }, { 1.f, 0.f }, { 1.f, 1.f },
        { 1.f, 0.f }, { 1.f, 1.f }, { 0.f, 1.f }, { 0.f, 0.f },
        { 0.f, 0.f }, { 0.f, 1.f }, { 1.f, 1.f }, { 1.f, 0.f },
        { 0.f, 0.f }, { 1.f, 0.f }, { 1.f, 1.f }, { 0.f, 1.f },
        { 1.f, 0.f }, { 0.f, 0.f }, { 0.f, 1.f }, { 1.f, 1.f }
    };

    static afxTriangle32 const tris[] =
    {
        {  0,  2,  1 }, {  0,  3,  2 }, {  4,  5,  6 },
        {  4,  6,  7 }, {  8,  9, 10 }, {  8, 10, 11 },
        { 12, 15, 14 }, { 12, 14, 13 }, { 16, 17, 18 },
        { 16, 18, 19 }, { 20, 23, 22 }, { 20, 22, 21 }
    };

    if (secCnt == 0) secCnt = 6;
    else if (secCnt > 12)
        secCnt = 12;

    afxUnit meshSecs[12];
    afxUnit secDiv = 12 / secCnt;

    for (afxUnit i = 0; i < secCnt; i++)
        meshSecs[i] = secDiv;

    afxGeometryInfo geob = { 0 };
    geob.attrCnt = 4;
    geob.vtxCnt = ARRAY_SIZE(cubeVertices);

    afxGeometry geo;
    AfxAcquireGeometries(din, 1, &geob, &geo);

    AfxResetGeometry(geo, 0, afxVertexFormat_V3D, afxVertexFlag_POSITIONAL | afxVertexFlag_SPATIAL | afxVertexFlag_ATV, &AfxString("pos"));
    AfxResetGeometry(geo, 1, afxVertexFormat_V3D, afxVertexFlag_NORMALIZED | afxVertexFlag_LTM, &AfxString("nrm"));
    AfxResetGeometry(geo, 2, afxVertexFormat_V3D, afxVertexFlag_NORMALIZED | afxVertexFlag_LTM, &AfxString("tan"));
    AfxResetGeometry(geo, 3, afxVertexFormat_V2D, afxVertexFlag_NORMALIZED, &AfxString("uv"));

    AfxUpdateGeometry(geo, 0, 0, geob.vtxCnt, cubeVertices, sizeof(cubeVertices[0]));
    AfxUpdateGeometry(geo, 1, 0, geob.vtxCnt, cubeNormals, sizeof(cubeNormals[0]));
    AfxUpdateGeometry(geo, 2, 0, geob.vtxCnt, cubeTangents, sizeof(cubeTangents[0]));
    AfxUpdateGeometry(geo, 3, 0, geob.vtxCnt, cubeTexCoords, sizeof(cubeTexCoords[0]));

    afxMeshBlueprint mshb = { 0 };
    mshb.vtxCnt = ARRAY_SIZE(cubeVertices);
    mshb.geo = geo;
    mshb.biasCnt = 1;
    mshb.triCnt = ARRAY_SIZE(tris);
    mshb.secCnt = secCnt;
    mshb.trisForSurfMap = meshSecs;

    afxMesh msh;
    AfxAssembleMeshes(din, 1, &mshb, &msh);
    AfxReleaseObjects(1, &geo);

    AfxUpdateMeshIndices(msh, 0, mshb.triCnt, tris, sizeof(tris[0][0]));

    afxV3d* pos = AfxAccessGeometry(geo, 0, 0);

    afxV3d whdSanitized;
    AfxMaxV3d(whdSanitized, AFX_V3D_ONE, whd);

    for (afxUnit i = 0; i < mshb.vtxCnt; i++)
        AfxV3dMultiply(pos[i], pos[i], whdSanitized);


    *mesh = msh;
    return err;
}

_AVX afxError AfxBuildGridMesh(afxDrawInput din, afxUnit rows, afxUnit layers, afxReal width, afxReal depth, afxMesh* mesh)
{
    // _ldrShapeCreateRectangularGridPlanef
    afxError err = NIL;
    AFX_ASSERT(mesh);

    afxBool triangleStrip = FALSE;
    
    rows = AfxMax(1, rows);
    layers = AfxMax(1, layers);
    afxUnit numberVertices = (rows + 1) * (layers + 1);

    afxGeometry geo;
    afxGeometryInfo geoi = { 0 };
    geoi.vtxCnt = numberVertices;
    geoi.attrCnt = 4;
    AfxAcquireGeometries(din, 1, &geoi, &geo);

    AfxResetGeometry(geo, 0, afxVertexFormat_V3D, afxVertexFlag_ATV, &AfxString("pos"));
    AfxResetGeometry(geo, 1, afxVertexFormat_V3D, afxVertexFlag_LTM, &AfxString("nrm"));
    AfxResetGeometry(geo, 2, afxVertexFormat_V3D, afxVertexFlag_LTM, &AfxString("tan"));
    AfxResetGeometry(geo, 3, afxVertexFormat_V2D, NIL, &AfxString("uv"));
    afxV3d* pos = AfxAccessGeometry(geo, 0, 0);
    afxV3d* nrm = AfxAccessGeometry(geo, 1, 0);
    afxV3d* tan = AfxAccessGeometry(geo, 2, 0);
    afxV2d* uv0 = AfxAccessGeometry(geo, 3, 0);

    for (afxUnit i = 0; i < numberVertices; i++)
    {
        afxReal x = (afxReal)(i % (layers + 1)) / (afxReal)layers;
        afxReal z = 1.f - (afxReal)(i / (layers + 1)) / (afxReal)rows;
        afxReal s = x, t = z;
        AfxV3dSet(pos[i], width * (x - 0.5f), 0.f, depth * (z - 0.5f));
        AfxV3dSet(nrm[i], 0, 1, 0);
        AfxV3dSet(tan[i], 1, 0, 0); //
        AfxV2dSet(uv0[i], s, t);
    }

    afxUnit numberIndices = (triangleStrip) ? rows * 2 * (layers + 1) : rows * 6 * layers;

    afxMesh msh;
    afxMeshBlueprint mshb = { 0 };
    mshb.geo = geo;
    mshb.triCnt = numberIndices / 3;
    AfxAssembleMeshes(din, 1, &mshb, &msh);
    AfxReleaseObjects(1, &geo);

    afxUnit* indices = AfxGetMeshIndices(msh, 0);

    if (triangleStrip) for (afxUnit i = 0, total = rows * (layers + 1); i < total; i++)
    {
        afxUnit x = i / (layers + 1), z = i % (layers + 1);
        // NOTE: This function was based on a grid generator where we swaped height by depth. It was need to do a index inversion between Y and Z for indexed triangle list, but it has not been done here. And it was not tested too.

        if (x == 0)
        {
            // left to right, top to bottom
            indices[i * 2] = z + x * (layers + 1);
            indices[i * 2 + 1] = z + (x + 1) * (layers + 1);
        }
        else
        {
            // right to left, bottom to up
            indices[i * 2] = (layers - z) + (x + 1) * (layers + 1);
            indices[i * 2 + 1] = (layers - z) + x * (layers + 1);
        }
    }
    else for (afxUnit i = 0, total = rows * layers; i < total; i++)
    {
        afxUnit x = i / layers, z = i % layers;
        // from
        indices[i * 6 + 0] =  z + x * (layers + 1); // left to right
        indices[i * 6 + 1] = (z + 1) + (x + 1) * (layers + 1);
        indices[i * 6 + 2] =  z + (x + 1) * (layers + 1); // top to bottom
        // to
        indices[i * 6 + 3] = (z + 1) + (x + 1) * (layers + 1);
        indices[i * 6 + 4] =  z + x * (layers + 1);
        indices[i * 6 + 5] = (z + 1) + x * (layers + 1);
    }

    //AfxInvertMeshTopology(msh);

    *mesh = msh;
    return err;
}
