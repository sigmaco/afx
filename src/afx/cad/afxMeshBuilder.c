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

#define _AFX_SIM_C
#define _AKX_MESH_C
#define _AKX_VERTEX_DATA_C
#define _AFX_VERTEX_BUILDER_C
#define _AKX_MESH_TOPOLOGY_C
#include "../sim/dev/AkxSimDevKit.h"

#define MaxNumWeights 8

struct WeightVertex
{
    afxNat8 BoneWeights[MaxNumWeights];
    afxNat8 BoneIndices[MaxNumWeights];
    afxNat BonesUsed;
};

struct TriWeightData
{
    afxNat jntCnt;
    afxNat8 jntIdx[MaxNumWeights * 3];
    akxIndexedTriangle vtxIdx;
};

////////////////////////////////////////////////////////////////////////////////
// MESH                                                                       //
////////////////////////////////////////////////////////////////////////////////

#if 0
_AKX void _AfxMesh113131(afxMesh msh)
{
    afxNat* OriginalIndices = msh->topology->vtxIdx;
    
    afxNat const NumMeshTris = AfxCountMeshTriangles(msh->topology);
    struct TriWeightData* TriWeights = AfxAllocate(sizeof(TriWeights[0]), NumMeshTris, 0, AfxHere());
    
    for (afxNat triIdx = 0; triIdx < NumMeshTris; ++triIdx)
    {
        struct TriWeightData* TriData = &TriWeights[triIdx];
        TriData->jntCnt = 0;
        
        for (afxNat vtxIdx = 0; vtxIdx < 3; ++vtxIdx)
        {
            TriData->vtxIdx[vtxIdx] = OriginalIndices[triIdx * 3 + vtxIdx];
            
            akxVertexBias* bias = &msh->vtd->biases[OriginalIndices[triIdx * 3 + vtxIdx]];
            
            for (afxNat vtxWgtIdx = 0; vtxWgtIdx < bias->weightCnt; ++vtxWgtIdx)
            {
                akxVertexWeight* w = &msh->vtd->weights[bias->baseWeightIdx + vtxWgtIdx];
                
                if (AfxFind(TriData->jntIdx, TriData->jntIdx + TriData->jntCnt, w->pivotIdx) == TriData->jntIdx + TriData->jntCnt)
                {
                    TriData->jntIdx[TriData->jntCnt++] = w->pivotIdx;
                }
            }
        }
    }
}
#endif


#if 0
afxMesh AfxAddDomeToModel(afxModel mdl, afxReal radius, afxNat32 slices)
{
    afxNat32 numberParallels = slices / 4;
    afxNat32 numberVertices = (numberParallels + 1) * (slices + 1);
    afxNat32 numberIndices = numberParallels * slices * 6;
    afxReal angleStep = (2.0f * AFX_PI) / ((afxReal)slices);

    afxNat32 indexIndices;

    // used later to help us calculating tangents vectors
    afxReal helpVector[3] = { 1.0f, 0.0f, 0.0f };
    afxReal helpQuaternion[4];
    afxReal helpMatrix[16];

    slices = slices < 3 ? 3 : slices;

    akxEntity ent = AfxGetAspectOwner(&mdl->asp);

    afxMesh msh = AfxAcquireMesh(mdl, numberVertices, 1, "parallelepiped");
    //AfxPopulateVertexBuffer(&msh->vbuf, AFX_VERTEX_ATTR_XYZW, numberVertices, 0, xy_vertices, 0);
    //AfxPopulateVertexBuffer(&msh->vbuf, AFX_VERTEX_ATTR_TAN, numberVertices, 0, xy_tangents, 0);
    //AfxPopulateVertexBuffer(&msh->vbuf, AFX_VERTEX_ATTR_NRM, numberVertices, 0, xy_normals, 0);
    //AfxPopulateVertexBuffer(&msh->vbuf, AFX_VERTEX_ATTR_UV, numberVertices, 0, xy_texCoords, 0);
    
    afxReal* vertices = msh->vbuf.attrs[AFX_VERTEX_ATTR_XYZW].pool;
    afxReal* normals = msh->vbuf.attrs[AFX_VERTEX_ATTR_NRM].pool;
    afxReal* tangents = msh->vbuf.attrs[AFX_VERTEX_ATTR_TAN].pool;
    afxReal* texCoords = msh->vbuf.attrs[AFX_VERTEX_ATTR_UV].pool;

    msh->vbuf.attrs[AFX_VERTEX_ATTR_XYZW].pop = msh->vbuf.attrs[AFX_VERTEX_ATTR_XYZW].cap;
    msh->vbuf.attrs[AFX_VERTEX_ATTR_NRM].pop = msh->vbuf.attrs[AFX_VERTEX_ATTR_NRM].cap;
    msh->vbuf.attrs[AFX_VERTEX_ATTR_TAN].pop = msh->vbuf.attrs[AFX_VERTEX_ATTR_TAN].cap;
    msh->vbuf.attrs[AFX_VERTEX_ATTR_UV].pop = msh->vbuf.attrs[AFX_VERTEX_ATTR_UV].cap;

    for (afxNat32 i = 0; i < numberParallels + 1; i++)
    {
        for (afxNat32 j = 0; j < slices + 1; j++)
        {
            afxNat32 vertexIndex = (i * (slices + 1) + j) * 4;
            afxNat32 normalIndex = (i * (slices + 1) + j) * 3;
            afxNat32 tangentIndex = (i * (slices + 1) + j) * 3;
            afxNat32 texCoordsIndex = (i * (slices + 1) + j) * 2;

            vertices[vertexIndex + 0] = radius * sinf(angleStep * (afxReal)i) * sinf(angleStep * (afxReal)j);
            vertices[vertexIndex + 1] = radius * cosf(angleStep * (afxReal)i);
            vertices[vertexIndex + 2] = radius * sinf(angleStep * (afxReal)i) * cosf(angleStep * (afxReal)j);
            vertices[vertexIndex + 3] = 1.0f;

            normals[normalIndex + 0] = vertices[vertexIndex + 0] / radius;
            normals[normalIndex + 1] = vertices[vertexIndex + 1] / radius;
            normals[normalIndex + 2] = vertices[vertexIndex + 2] / radius;

            texCoords[texCoordsIndex + 0] = (afxReal)j / (afxReal)slices;
            texCoords[texCoordsIndex + 1] = 1.0f - (afxReal)i / (afxReal)numberParallels;

            // use quaternion to get the tangent vector
            AfxRotationQuatFromAxial(helpQuaternion, AfxSpawnV3d(0, 1, 0), 360.0f * texCoords[texCoordsIndex + 0]);
            AfxRotateM4d_AxialFromQuat((afxV4d*)helpMatrix, helpQuaternion);

            AfxTransformPivotV3d(&tangents[tangentIndex], helpVector, (afxV4d*)helpMatrix);
        }
    }

    akxMeshSurface msec = AfxAcquireMeshSection(msh, avxTopology_TRI_LIST, numberIndices, 0, AfxAcquireMaterial(mdl, "parallelepiped"));
    //AfxFillMeshSection(msh, msecIdx, numberIndices);
    afxNat32* indices = msec->indices.pool;
    msec->indices.pop = msec->indices.cap;

    indexIndices = 0;
    for (afxNat32 i = 0; i < numberParallels; i++)
    {
        for (afxNat32 j = 0; j < slices; j++)
        {
            indices[indexIndices++] = i * (slices + 1) + j;
            indices[indexIndices++] = (i + 1) * (slices + 1) + j;
            indices[indexIndices++] = (i + 1) * (slices + 1) + (j + 1);

            indices[indexIndices++] = i * (slices + 1) + j;
            indices[indexIndices++] = (i + 1) * (slices + 1) + (j + 1);
            indices[indexIndices++] = i * (slices + 1) + (j + 1);
        }
    }

    return msh;
}

afxMesh AfxAddParallelepipedToModel(afxModel mdl, afxReal width, afxReal height)
{
    static afxNat32 numberVertices = 4, numberIndices = 6;
    static afxReal const xy_vertices[] = { -1.0f, -1.0f, 0.0f, +1.0f, +1.0f, -1.0f, 0.0f, +1.0f, -1.0f, +1.0f, 0.0f, +1.0f, +1.0f, +1.0f, 0.0f, +1.0f };
    static afxReal const xy_normals[] = { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f };
    static afxReal const xy_tangents[] = { 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f };
    static afxReal const xy_texCoords[] = { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };
    static afxNat32 const xy_indices[] = { 0, 1, 2, 1, 3, 2 };

    akxEntity ent = AfxGetAspectOwner(&mdl->asp);

    afxMesh msh = AfxAcquireMesh(mdl, numberVertices, 1, "parallelepiped");
    AfxPopulateVertexBuffer(&msh->vbuf, AFX_VERTEX_ATTR_XYZW, numberVertices, 0, xy_vertices, 0);
    AfxPopulateVertexBuffer(&msh->vbuf, AFX_VERTEX_ATTR_TAN, numberVertices, 0, xy_tangents, 0);
    AfxPopulateVertexBuffer(&msh->vbuf, AFX_VERTEX_ATTR_NRM, numberVertices, 0, xy_normals, 0);
    AfxPopulateVertexBuffer(&msh->vbuf, AFX_VERTEX_ATTR_UV, numberVertices, 0, xy_texCoords, 0);
    akxMeshSurface msec = AfxAcquireMeshSection(msh, avxTopology_TRI_LIST, numberIndices, 0, AfxAcquireMaterial(mdl, "parallelepiped"));
    AfxFillMeshSection(msec, numberIndices, xy_indices);

    afxM4d m4d;
    AfxM4dReset(m4d);
    AfxM4dScaling(m4d, AfxSpawnV4d(width, 10.0, height, 1.0));

    for (afxNat32 i = 0; i < numberVertices; i++)
    {
        AfxV4dPreMultiplyM4d(((afxV4d*)msh->vbuf.attrs[AFX_VERTEX_ATTR_XYZW].pool)[i], ((afxV4d*)msh->vbuf.attrs[AFX_VERTEX_ATTR_XYZW].pool)[i], m4d);
    }
    return msh;
}

afxMesh AfxAddCubeToModel(afxModel mdl, afxReal scale)
{
    static afxNat32 numberVertices = 24, numberIndices = 36;

    static afxReal const cubeVertices[]=
    {
        -1.0f, -1.0f, -1.0f, +1.0f, -1.0f, -1.0f, +1.0f, +1.0f, +1.0f, -1.0f, +1.0f, +1.0f, +1.0f, -1.0f, -1.0f, +1.0f,
        -1.0f, +1.0f, -1.0f, +1.0f, -1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, -1.0f, +1.0f,
        -1.0f, -1.0f, -1.0f, +1.0f, -1.0f, +1.0f, -1.0f, +1.0f, +1.0f, +1.0f, -1.0f, +1.0f, +1.0f, -1.0f, -1.0f, +1.0f,
        -1.0f, -1.0f, +1.0f, +1.0f, -1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, -1.0f, +1.0f, +1.0f,
        -1.0f, -1.0f, -1.0f, +1.0f, -1.0f, -1.0f, +1.0f, +1.0f, -1.0f, +1.0f, +1.0f, +1.0f, -1.0f, +1.0f, -1.0f, +1.0f,
        +1.0f, -1.0f, -1.0f, +1.0f, +1.0f, -1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, -1.0f, +1.0f
    };

    static afxReal const cubeNormals[] =
    {
        0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f,
        0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f,
        -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f
    };

    static afxReal const cubeTangents[] =
    {
        +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f,
        +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f,
        0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f
    };

    static afxReal const cubeTexCoords[] =
    {
        0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f
    };

    static afxNat32 const cubeIndices[] = { 0, 2, 1, 0, 3, 2, 4, 5, 6, 4, 6, 7, 8, 9, 10, 8, 10, 11, 12, 15, 14, 12, 14, 13, 16, 17, 18, 16, 18, 19, 20, 23, 22, 20, 22, 21 };

    akxEntity ent = AfxGetAspectOwner(&mdl->asp);

    afxMesh msh = AfxAcquireMesh(mdl, numberVertices, 1, "cube");
    AfxPopulateVertexBuffer(&msh->vbuf, AFX_VERTEX_ATTR_XYZW, numberVertices, 0, cubeVertices, 0);
    AfxRescaleVertexBuffer(&msh->vbuf, AFX_VERTEX_ATTR_XYZW, scale);
    AfxPopulateVertexBuffer(&msh->vbuf, AFX_VERTEX_ATTR_TAN, numberVertices, 0, cubeTangents, 0);
    AfxPopulateVertexBuffer(&msh->vbuf, AFX_VERTEX_ATTR_NRM, numberVertices, 0, cubeNormals, 0);
    AfxPopulateVertexBuffer(&msh->vbuf, AFX_VERTEX_ATTR_UV, numberVertices, 0, cubeTexCoords, 0);
    akxMeshSurface msec = AfxAcquireMeshSection(msh, avxTopology_TRI_LIST, numberIndices, 0, AfxAcquireMaterial(mdl, "cube"));
    AfxFillMeshSection(msec, numberIndices, cubeIndices);
    return msh;
}
#endif//0

_AKXINL void AfxEndMeshBuilding(afxMeshBuilder* mshb)
{
    afxError err = NIL;
    //AfxAssertType(mshb, afxFcc_MSHB);

    AfxDeallocateArray(&mshb->biases);

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

_AKXINL afxError AfxBeginMeshBuilding(afxMeshBuilder* mshb, afxString const* id, afxNat vtxCnt, afxNat triCnt, afxNat surfCnt, afxNat artCnt)
{
    afxError err = NIL;
    AfxAssert(mshb);
    AfxAssert(vtxCnt);
    AfxAssert(triCnt);
    AfxAssert(surfCnt);
    AfxAssert(artCnt);
    mshb->vtxCnt = vtxCnt;
    mshb->artCnt = artCnt;
    mshb->triCnt = triCnt;
    mshb->surfCnt = surfCnt;
    mshb->faces = NIL;

    if (surfCnt && !(mshb->faces = AfxAllocate(triCnt, sizeof(mshb->faces[0]), 0, AfxHere()))) AfxThrowError();
    else
    {
        AfxZero2(triCnt, sizeof(mshb->faces[0]), mshb->faces);        
    }

    AfxMakeString32(&mshb->id, id);

    AfxAllocateArray(&mshb->biases, artCnt, sizeof(akxVertexBias), (akxVertexBias[]) { 0 });

    mshb->vtx = AfxAllocate(vtxCnt, sizeof(mshb->vtx[0]), NIL, AfxHere());
    AfxZero2(vtxCnt, sizeof(mshb->vtx[0]), mshb->vtx);

    mshb->pivots = AfxAllocate(artCnt, sizeof(mshb->pivots[0]), NIL, AfxHere());

    mshb->posn = AfxAllocate(vtxCnt, sizeof(mshb->posn[0]), AFX_SIMD_ALIGN, AfxHere());
    mshb->nrm = NIL;
    mshb->uv = NIL;

    //AfxAssignFcc(mshb, afxFcc_MSHB);

    return err;
}

_AKXINL void AfxUpdateVertices(afxMeshBuilder* mshb, afxNat baseVtxIdx, afxNat vtxCnt, afxNat const baseBiasIdx[], afxNat const biasCnt[])
{
    afxError err = NIL;
    //AfxAssertType(mshb, afxFcc_MSHB);
    AfxAssertRange(mshb->vtxCnt, baseVtxIdx, vtxCnt);

    for (afxNat i = 0; i < vtxCnt; i++)
    {
        akxVertex *v = &mshb->vtx[baseVtxIdx + i];
        //AfxV4dCopyAtv3d(v->posn, posn[i]);
        v->baseBiasIdx = baseBiasIdx ? baseBiasIdx[i] : 0;
        v->biasCnt = biasCnt ? biasCnt[i] : 1;
        //AfxV2dCopy(v->uv, uv ? uv[i] : AfxSpawnV2d(0, 0));

        AfxAssertRange(mshb->biases.cnt, v->baseBiasIdx, v->biasCnt);
    }
}

_AKXINL void AfxUpdateVertexPositions(afxMeshBuilder* mshb, afxNat baseVtxIdx, afxNat vtxCnt, afxReal const posn[][3], afxNat srcStride)
{
    afxError err = NIL;
    //AfxAssertType(mshb, afxFcc_MSHB);
    AfxAssertRange(mshb->vtxCnt, baseVtxIdx, vtxCnt);
    AfxAssert(posn);

    if (srcStride == sizeof(posn[0]))
    {
        for (afxNat i = 0; i < vtxCnt; i++)
            AfxV4dCopyAtv3d(mshb->posn[baseVtxIdx + i], posn[i]);
    }
    else
    {
        afxByte* posnBytemap = (void*)posn;
        
        for (afxNat i = 0; i < vtxCnt; i++)
            AfxV4dCopyAtv3d(mshb->posn[baseVtxIdx + i], (void*)(&posnBytemap[i * srcStride]));
    }
}

_AKXINL void AfxUpdateVertexPositions4(afxMeshBuilder* mshb, afxNat baseVtxIdx, afxNat vtxCnt, afxV4d const posn[], afxNat srcStride)
{
    afxError err = NIL;
    //AfxAssertType(mshb, afxFcc_MSHB);
    AfxAssertRange(mshb->vtxCnt, baseVtxIdx, vtxCnt);
    AfxAssert(posn);

    if (srcStride == sizeof(posn[0]))
    {
        AfxCopyArrayedV4d(vtxCnt, posn, &mshb->posn[baseVtxIdx]);
    }
    else
    {
        afxByte* posnBytemap = (void*)posn;

        for (afxNat i = 0; i < vtxCnt; i++)
            AfxV4dCopy(mshb->posn[baseVtxIdx + i], (void*)(&posnBytemap[i * srcStride]));
    }
}

_AKXINL afxError AfxUpdateVertexNormals(afxMeshBuilder* mshb, afxNat baseVtxIdx, afxNat vtxCnt, afxV3d const nrm[], afxNat srcStride)
{
    afxError err = NIL;
    //AfxAssertType(mshb, afxFcc_MSHB);
    AfxAssertRange(mshb->vtxCnt, baseVtxIdx, vtxCnt);
    AfxAssert(nrm);

    if (!mshb->nrm && !(mshb->nrm = AfxAllocate(mshb->vtxCnt, sizeof(mshb->nrm[0]), AFX_SIMD_ALIGN, AfxHere())))
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

            for (afxNat i = 0; i < vtxCnt; i++)
                AfxV3dCopy(mshb->nrm[baseVtxIdx + i], (void*)(&nrmBytemap[i * srcStride]));
        }
    }
    return err;
}

_AKXINL afxError AfxUpdateVertexWraps(afxMeshBuilder* mshb, afxNat baseVtxIdx, afxNat vtxCnt, afxV2d const uv[], afxNat srcStride)
{
    afxError err = NIL;
    //AfxAssertType(mshb, afxFcc_MSHB);
    AfxAssertRange(mshb->vtxCnt, baseVtxIdx, vtxCnt);
    AfxAssert(uv);

    if (!mshb->uv && !(mshb->uv = AfxAllocate(mshb->vtxCnt, sizeof(mshb->uv[0]), AFX_SIMD_ALIGN, AfxHere())))
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

            for (afxNat i = 0; i < vtxCnt; i++)
                AfxV2dCopy(mshb->uv[baseVtxIdx + i], (void*)(&uvBytemap[i * srcStride]));
        }
    }
    
    return err;
}

_AKXINL void AfxEmitTriangles(afxMeshBuilder* mshb, afxNat surfIdx, afxNat baseTriIdx, afxNat triCnt, afxNat const triVtxIdx[][3])
{
    afxError err = NIL;
    //AfxAssertType(mshb, afxFcc_MSHB);
    AfxAssertRange(mshb->surfCnt, surfIdx, 1);
    AfxAssertRange(mshb->triCnt, baseTriIdx, triCnt);

    for (afxNat i = 0; i < triCnt; i++)
    {
        mshb->faces[baseTriIdx + i].vtxIdx[0] = triVtxIdx[i][0];
        mshb->faces[baseTriIdx + i].vtxIdx[1] = triVtxIdx[i][1];
        mshb->faces[baseTriIdx + i].vtxIdx[2] = triVtxIdx[i][2];
        mshb->faces[baseTriIdx + i].surfIdx = surfIdx;
        mshb->faces[baseTriIdx + i].artCnt = 0;

        AfxAssert(surfIdx == mshb->faces[baseTriIdx + i].surfIdx);
        AfxAssertRange(mshb->surfCnt, mshb->faces[baseTriIdx + i].surfIdx, 1);
    }
}

_AKXINL afxNat AfxAddVertexBiases(afxMeshBuilder* mshb, afxNat cnt, afxNat const jntIdx[], afxReal const weight[])
{
    afxError err = NIL;
    //AfxAssertType(mshb, afxFcc_MSHB);
    AfxReserveArraySpace(&mshb->biases, mshb->biases.cnt + cnt);

    afxNat baseBiasIdx = AFX_INVALID_INDEX;
    AfxInsertArrayUnits(&mshb->biases, cnt, &baseBiasIdx, NIL);

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxAssert(weight && 1.f >= weight[i]);
        AfxUpdateArrayRange(&mshb->biases, baseBiasIdx + i, 1, (const akxVertexBias[]) { { .pivotIdx = jntIdx ? jntIdx[i] : 0, .weight = weight ? weight[i] : 1.f } });
        AfxAssertRange(mshb->artCnt, ((akxVertexBias const*)AfxGetArrayUnit(&mshb->biases, baseBiasIdx + i))->pivotIdx, 1);
    }
    return baseBiasIdx;
}

_AKXINL void AfxRenameVertexPivots(afxMeshBuilder* mshb, afxNat basePivotIdx, afxNat cnt, afxString const name[])
{
    afxError err = NIL;
    //AfxAssertType(mshb, afxFcc_MSHB);
    AfxAssertRange(mshb->artCnt, basePivotIdx, cnt);
    AfxAssert(name);

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxReflectString(&name[i], &mshb->pivots[basePivotIdx + i]);
    }
}

_AKX afxMesh AfxBuildCubeMesh(afxSimulation sim, afxReal scale, afxNat secCnt)
{
    afxError err = NIL;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AfxAssert(scale);

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

    static akxIndexedTriangle const tris[] =
    { 
        {  0,  2,  1 }, {  0,  3,  2 }, {  4,  5,  6 },
        {  4,  6,  7 }, {  8,  9, 10 }, {  8, 10, 11 },
        { 12, 15, 14 }, { 12, 14, 13 }, { 16, 17, 18 },
        { 16, 18, 19 }, { 20, 23, 22 }, { 20, 22, 21 }
    };

    if (secCnt == 0) secCnt = 6;
    else if (secCnt > 12)
        secCnt = 12;

    afxNat meshSecs[12];
    afxNat secDiv = 12 / secCnt;

    for (afxNat i = 0; i < secCnt; i++)
        meshSecs[i] = secDiv;

    akxVertexAttrSpec attrSpecs[] =
    {
        {
            .fmt = afxVertexFormat_V3D,
            .flags = akxVertexFlag_POSITIONAL | akxVertexFlag_SPATIAL | akxVertexFlag_AFFINE,
            .usage = akxVertexUsage_POS | akxVertexUsage_POSITIONAL | akxVertexUsage_SPATIAL,
            .src = cubeVertices,
            .srcStride = sizeof(cubeVertices[0])
        },
        {
            .fmt = afxVertexFormat_V3D,
            .flags = akxVertexFlag_NORMALIZED | akxVertexFlag_LINEAR,
            .usage = akxVertexUsage_NRM,
            .src = cubeNormals,
            .srcStride = sizeof(cubeNormals[0])
        },
        {
            .fmt = afxVertexFormat_V2D,
            .flags = akxVertexFlag_NORMALIZED | akxVertexFlag_LINEAR,
            .usage = akxVertexUsage_UV,
            .src = cubeTexCoords,
            .srcStride = sizeof(cubeTexCoords[0])
        }
    };

    akxVertexDataSpec vtdb = { 0 };
    vtdb.attrCnt = AFX_COUNTOF(attrSpecs);
    vtdb.vtxCnt = AFX_COUNTOF(cubeVertices);
    
    akxVertexData vtd;
    AkxAcquireVertexDatas(sim, attrSpecs, 1, &vtdb, &vtd);

    afxMeshTopology msht;
    akxMeshTopologyBlueprint mshtb = { 0 };
    mshtb.triCnt = AFX_COUNTOF(tris);
    mshtb.src = tris;
    mshtb.srcIdxSiz = sizeof(tris[0][0]);
    mshtb.surfCnt = secCnt;
    mshtb.trisForSurfMap = meshSecs;
    AfxAssembleMeshTopology(sim, 1, &mshtb, &msht);
    
    akxMeshBlueprint mshb = { 0 };
    mshb.vtxCnt = AFX_COUNTOF(cubeVertices);
    mshb.vtd = vtd;
    mshb.topology = msht;
    mshb.biasCnt = 1;
    
    afxMesh msh;
    AfxAssembleMeshes(sim, 1, &mshb, &msh);
    AfxReleaseObjects(1, &msht);
    AfxReleaseObjects(1, &vtd);

    afxV3d* pos = AkxExposeVertexData(vtd, 0, 0);

    for (afxNat i = 0; i < mshb.vtxCnt; i++)
        AfxV3dScale(pos[i], pos[i], scale);

    return msh;
}

_AKX afxMesh AfxBuildParallelepipedMesh(afxSimulation sim, afxV3d whd, afxReal slantX, afxReal slantY)
{
    afxError err = NIL;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AfxAssert(whd);

    if (!slantX)
        slantX = 0.5f;

    if (!slantY)
        slantY = 0.5f;

    afxNat numVertices = 8;
    afxNat numIndices = 36; // 12 triangles * 3 vertices per triangle

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
    afxNat const indicesData[36] =
    {
        0, 1, 2, 2, 3, 0, // Back face
        4, 5, 6, 6, 7, 4, // Front face
        0, 1, 5, 5, 4, 0, // Bottom face
        2, 3, 7, 7, 6, 2, // Top face
        0, 3, 7, 7, 4, 0, // Left face
        1, 2, 6, 6, 5, 1  // Right face
    };

    akxVertexAttrSpec attrSpecs[] =
    {
        {
            .fmt = afxVertexFormat_V3D,
            .flags = akxVertexFlag_POSITIONAL | akxVertexFlag_SPATIAL | akxVertexFlag_AFFINE,
            .usage = akxVertexUsage_POS | akxVertexUsage_POSITIONAL | akxVertexUsage_SPATIAL,
            .src = vertData,
            .srcStride = sizeof(vertData[0])
        },
        {
            .fmt = afxVertexFormat_V3D,
            .flags = akxVertexFlag_NORMALIZED | akxVertexFlag_LINEAR,
            .usage = akxVertexUsage_NRM
        },
        {
            .fmt = afxVertexFormat_V2D,
            .flags = akxVertexFlag_NORMALIZED | akxVertexFlag_LINEAR,
            .usage = akxVertexUsage_UV
        }
    };

    akxVertexDataSpec vtdb = { 0 };
    vtdb.attrCnt = AFX_COUNTOF(attrSpecs);
    vtdb.vtxCnt = numVertices;

    akxVertexData vtd;
    AkxAcquireVertexDatas(sim, attrSpecs, 1, &vtdb, &vtd);

    afxV3d* nrm = AkxExposeVertexData(vtd, 1, 0);
    afxV2d* uv = AkxExposeVertexData(vtd, 2, 0);

    afxNat index = 0;

    for (afxNat i = 0; i < 8; ++i)
    {
        nrm[index][0] = 0.0f; // Normals
        nrm[index][1] = 0.0f;
        nrm[index][2] = 1.0f;
        uv[index][0] = (i % 2) ? 1.0f : 0.0f; // Texture coordinates
        uv[index][1] = (i / 4) ? 1.0f : 0.0f;
        index++;
    }

    afxMeshTopology msht;
    akxMeshTopologyBlueprint mshtb = { 0 };
    mshtb.triCnt = numIndices / 3;
    mshtb.src = indicesData;
    mshtb.srcIdxSiz = sizeof(indicesData[0]);
    AfxAssembleMeshTopology(sim, 1, &mshtb, &msht);

    akxMeshBlueprint mshb = { 0 };
    mshb.vtxCnt = numVertices;
    mshb.vtd = vtd;
    mshb.topology = msht;
    mshb.biasCnt = 1;

    afxMesh msh;
    AfxAssembleMeshes(sim, 1, &mshb, &msh);
    AfxReleaseObjects(1, &msht);
    AfxReleaseObjects(1, &vtd);

    return msh;
}

_AKX afxMesh AfxBuildDomeMesh(afxSimulation sim, afxReal radius, afxNat slices)
{
    afxError err = NIL;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AfxAssert(radius);
    AfxAssert(slices);
    AfxAssert(slices >= 3);

    afxNat numberParallels = slices / 4;
    afxNat numberVertices = (numberParallels + 1) * (slices + 1);
    afxNat numberIndices = numberParallels * slices * 6;
    afxReal angleStep = (2.f * AFX_PI) / ((afxReal)slices);

    // used later to help us calculating tangents vectors
    afxQuat helpQuaternion;
    afxM4d helpMatrix;

    slices = slices < 3 ? 3 : slices;

    afxMeshBuilder mshb;
    AfxBeginMeshBuilding(&mshb, &AfxStaticString("dome"), numberVertices, numberIndices / 3, 1, 1);
    AfxRenameVertexPivots(&mshb, 0, 1, &AfxStaticString("dome"));
    AfxAddVertexBiases(&mshb, 1, (afxNat[]) { 0 }, (afxReal[]) { 1.0 });
    AfxUpdateVertices(&mshb, 0, numberVertices, NIL, NIL);

    AfxUpdateVertexNormals(&mshb, 0, 1, &AFX_V3D_ZERO, 0);

    AfxUpdateVertexWraps(&mshb, 0, 1, &AFX_V2D_ZERO, 0);

    afxV4d* vertices = mshb.posn;
    afxV3d* normals = mshb.nrm;
    //afxV3d* tangents = mshb.tan;
    afxV2d* texCoords = mshb.uv;

    for (afxNat i = 0; i < numberParallels + 1; i++)
    {
        for (afxNat j = 0; j < slices + 1; j++)
        {
            afxNat vertexIndex = (i * (slices + 1) + j) * 4;
            afxNat normalIndex = (i * (slices + 1) + j) * 3;
            afxNat tangentIndex = (i * (slices + 1) + j) * 3;
            afxNat texCoordsIndex = (i * (slices + 1) + j) * 2;

            vertices[vertexIndex][0] = radius * AfxSinf(angleStep * (afxReal)i) * AfxSinf(angleStep * (afxReal)j);
            vertices[vertexIndex][1] = radius * AfxCosf(angleStep * (afxReal)i);
            vertices[vertexIndex][2] = radius * AfxSinf(angleStep * (afxReal)i) * AfxCosf(angleStep * (afxReal)j);
            vertices[vertexIndex][3] = 1.0f;

            normals[normalIndex][0] = vertices[vertexIndex][0] / radius;
            normals[normalIndex][1] = vertices[vertexIndex][1] / radius;
            normals[normalIndex][2] = vertices[vertexIndex][2] / radius;

            texCoords[texCoordsIndex][0] = (afxReal)j / (afxReal)slices;
            texCoords[texCoordsIndex][1] = 1.0f - (afxReal)i / (afxReal)numberParallels;

            // use quaternion to get the tangent vector
            AfxQuatRotationFromAxis(helpQuaternion, AfxSpawnV3d(0, 1, 0), 360.0f * texCoords[texCoordsIndex][0]);
            AfxM4dRotationFromQuat(helpMatrix, helpQuaternion);
            //AfxV3dPostMultiplyM3d(&tangents[tangentIndex], helpMatrix, AFX_V3D_X);
        }
    }

    afxNat baseTriIdx = 0;
    akxIndexedTriangle tri;

    for (afxNat32 i = 0; i < numberParallels; i++)
    {
        for (afxNat32 j = 0; j < slices; j++)
        {
            tri[0] = i * (slices + 1) + j;
            tri[1] = (i + 1) * (slices + 1) + j;
            tri[2] = (i + 1) * (slices + 1) + (j + 1);

            AfxEmitTriangles(&mshb, 0, baseTriIdx, 1, &tri);
            ++baseTriIdx;

            tri[0] = i * (slices + 1) + j;
            tri[1] = (i + 1) * (slices + 1) + (j + 1);
            tri[2] = i * (slices + 1) + (j + 1);

            AfxEmitTriangles(&mshb, 0, baseTriIdx, 1, &tri);
            ++baseTriIdx;
        }
    }

    afxMesh msh;
    AfxBuildMeshes(sim, 1, &mshb, &msh);
    //AfxEndMeshBuilding(&mshb);

    return msh;
}

_AKX afxMesh AfxBuildDomeMesh2(afxSimulation sim, afxReal radius, afxNat stacks, afxNat slices, afxBool inv)
{
    afxError err = NIL;
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    if (!radius)
        radius = 1.f;

    if (!stacks)
        stacks = 10;

    if (!slices)
        slices = 20;

    afxNat numVertices = (stacks + 1) * (slices + 1);
    
    akxVertexAttrSpec attrSpecs[] =
    {
        {
            .fmt = afxVertexFormat_V3D,
            .flags = akxVertexFlag_POSITIONAL | akxVertexFlag_SPATIAL | akxVertexFlag_AFFINE,
            .usage = akxVertexUsage_POS | akxVertexUsage_POSITIONAL | akxVertexUsage_SPATIAL
        },
        {
            .fmt = afxVertexFormat_V3D,
            .flags = akxVertexFlag_NORMALIZED | akxVertexFlag_LINEAR,
            .usage = akxVertexUsage_NRM
        },
        {
            .fmt = afxVertexFormat_V2D,
            .flags = akxVertexFlag_NORMALIZED | akxVertexFlag_LINEAR,
            .usage = akxVertexUsage_UV
        }
    };

    akxVertexDataSpec vtdb = { 0 };
    vtdb.attrCnt = AFX_COUNTOF(attrSpecs);
    vtdb.vtxCnt = numVertices;

    akxVertexData vtd;
    AkxAcquireVertexDatas(sim, attrSpecs, 1, &vtdb, &vtd);

    afxV3d* pos = AkxExposeVertexData(vtd, 0, 0);
    afxV3d* nrm = AkxExposeVertexData(vtd, 1, 0);
    afxV2d* uv = AkxExposeVertexData(vtd, 2, 0);
    afxNat index = 0;

    for (afxNat i = 0; i <= stacks; ++i)
    {
        afxReal stackAngle = AFX_PI / 2 - i * AFX_PI / stacks; // angle from top
        afxReal xy = radius * AfxCosf(stackAngle); // radius of current stack
        afxReal z = radius * AfxSinf(stackAngle);  // height of current stack

        for (afxNat j = 0; j <= slices; ++j)
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

    afxMeshTopology msht;
#if 0 // we couldn't use trip strip at the time this code was written.
    afxBool strip = FALSE;

    if (strip)
    {
        afxNat numIndices = stacks * slices * 2 * 3; // 2 triangles per quad, 3 indices per triangle

        akxMeshTopologyBlueprint mshtb = { 0 };
        mshtb.triCnt = numIndices / 3;
        AfxAssembleMeshTopology(sim, 1, &mshtb, &msht);

        akxIndexedTriangle* tris = AfxGetMeshTriangles(msht, 0);

        // Generate indices for triangle strips
        afxNat idx = 0;

        for (afxNat i = 0; i < stacks; ++i)
        {
            for (afxNat j = 0; j < slices; ++j)
            {
                afxNat topLeft = i * (slices + 1) + j;
                afxNat topRight = topLeft + 1;
                afxNat bottomLeft = (i + 1) * (slices + 1) + j;
                afxNat bottomRight = bottomLeft + 1;

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
        afxNat numIndices = stacks * slices * 6; // 6 indices per quad (2 triangles, 3 indices per triangle)

        akxMeshTopologyBlueprint mshtb = { 0 };
        mshtb.triCnt = numIndices / 3;
        AfxAssembleMeshTopology(sim, 1, &mshtb, &msht);

        akxIndexedTriangle* tris = AfxGetMeshTriangles(msht, 0);

        // Generate indices for triangles
        afxNat idx = 0;

        for (afxNat i = 0; i < stacks; ++i)
        {
            for (afxNat j = 0; j < slices; ++j)
            {
                afxNat topLeft = i * (slices + 1) + j;
                afxNat topRight = topLeft + 1;
                afxNat bottomLeft = (i + 1) * (slices + 1) + j;
                afxNat bottomRight = bottomLeft + 1;

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

    if (inv)
    {
        AfxInvertMeshWinding(msht);
        AkxInvertNormalizedVertexData(vtd, 1, 0, numVertices);
    }

    akxMeshBlueprint mshb = { 0 };
    mshb.vtxCnt = numVertices;
    mshb.vtd = vtd;
    mshb.topology = msht;
    mshb.biasCnt = 1;

    afxMesh msh;
    AfxAssembleMeshes(sim, 1, &mshb, &msh);
    AfxReleaseObjects(1, &msht);
    AfxReleaseObjects(1, &vtd);

    return msh;
}

_AKX afxMesh AfxBuildSphereMesh(afxSimulation sim, afxReal radius, afxNat stacks, afxNat slices, afxBool inv)
{
    afxError err = NIL;
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    if (!radius)
        radius = 1.f;

    if (!stacks)
        stacks = 20;

    if (!slices)
        slices = 20;

    afxNat numVertices = (stacks + 1) * (slices + 1);

    akxVertexAttrSpec attrSpecs[] =
    {
        {
            .fmt = afxVertexFormat_V3D,
            .flags = akxVertexFlag_POSITIONAL | akxVertexFlag_SPATIAL | akxVertexFlag_AFFINE,
            .usage = akxVertexUsage_POS | akxVertexUsage_POSITIONAL | akxVertexUsage_SPATIAL
        },
        {
            .fmt = afxVertexFormat_V3D,
            .flags = akxVertexFlag_NORMALIZED | akxVertexFlag_LINEAR,
            .usage = akxVertexUsage_NRM
        },
        {
            .fmt = afxVertexFormat_V2D,
            .flags = akxVertexFlag_NORMALIZED | akxVertexFlag_LINEAR,
            .usage = akxVertexUsage_UV
        }
    };

    akxVertexDataSpec vtdb = { 0 };
    vtdb.attrCnt = AFX_COUNTOF(attrSpecs);
    vtdb.vtxCnt = numVertices;

    akxVertexData vtd;
    AkxAcquireVertexDatas(sim, attrSpecs, 1, &vtdb, &vtd);

    afxV3d* pos = AkxExposeVertexData(vtd, 0, 0);
    afxV3d* nrm = AkxExposeVertexData(vtd, 1, 0);
    afxV2d* uv = AkxExposeVertexData(vtd, 2, 0);
    afxNat index = 0;

    for (afxNat i = 0; i <= stacks; ++i)
    {
        afxReal stackAngle = AFX_PI * i / stacks; // angle from top
        afxReal xy = radius * AfxSinf(stackAngle); // radius of current stack
        afxReal z = radius * AfxCosf(stackAngle);  // height of current stack

        for (afxNat j = 0; j <= slices; ++j)
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

    afxMeshTopology msht;
#if 0 // we couldn't use trip strip at the time this code was written.
    afxBool strip = FALSE;

    if (strip)
    {
        afxNat numIndices = stacks * slices * 2 * 3; // 2 triangles per quad, 3 indices per triangle

        akxMeshTopologyBlueprint mshtb = { 0 };
        mshtb.triCnt = numIndices / 3;
        AfxAssembleMeshTopology(sim, 1, &mshtb, &msht);

        akxIndexedTriangle* tris = AfxGetMeshTriangles(msht, 0);

        // Generate indices for triangle strips
        afxNat idx = 0;

        for (afxNat i = 0; i < stacks; ++i)
        {
            for (afxNat j = 0; j < slices; ++j)
            {
                afxNat topLeft = i * (slices + 1) + j;
                afxNat topRight = topLeft + 1;
                afxNat bottomLeft = (i + 1) * (slices + 1) + j;
                afxNat bottomRight = bottomLeft + 1;

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
        afxNat numIndices = stacks * slices * 6; // 6 indices per quad (2 triangles, 3 indices per triangle)

        akxMeshTopologyBlueprint mshtb = { 0 };
        mshtb.triCnt = numIndices / 3;
        AfxAssembleMeshTopology(sim, 1, &mshtb, &msht);

        akxIndexedTriangle* tris = AfxGetMeshTriangles(msht, 0);

        // Generate indices for triangles
        afxNat idx = 0;

        for (afxNat i = 0; i < stacks; ++i)
        {
            for (afxNat j = 0; j < slices; ++j)
            {
                afxNat topLeft = i * (slices + 1) + j;
                afxNat topRight = topLeft + 1;
                afxNat bottomLeft = (i + 1) * (slices + 1) + j;
                afxNat bottomRight = bottomLeft + 1;

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

    if (inv)
    {
        AfxInvertMeshWinding(msht);
        AkxInvertNormalizedVertexData(vtd, 1, 0, numVertices);
    }

    akxMeshBlueprint mshb = { 0 };
    mshb.vtxCnt = numVertices;
    mshb.vtd = vtd;
    mshb.topology = msht;
    mshb.biasCnt = 1;

    afxMesh msh;
    AfxAssembleMeshes(sim, 1, &mshb, &msh);
    AfxReleaseObjects(1, &msht);
    AfxReleaseObjects(1, &vtd);

    return msh;
}

_AKX afxMesh AfxBuildCapsuleMesh(afxSimulation sim, afxReal radius, afxReal height, afxNat stacks, afxNat slices, afxNat cylinderSlices, afxBool inv)
{
    afxError err = NIL;
    AfxAssertObjects(1, &sim, afxFcc_SIM);

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

    afxNat numVerticesSphere = (stacks + 1) * (slices + 1);
    afxNat numVerticesCylinder = (cylinderSlices + 1) * 2; // Top and bottom circles
    afxNat numVertices = numVerticesSphere * 2 + numVerticesCylinder; // 2 hemispheres and 1 cylinder
    afxNat numIndicesSphere = stacks * slices * 6; // Each hemisphere has these many indices
    afxNat numIndicesCylinder = cylinderSlices * 6; // Cylinder indices
    afxNat numIndices = numIndicesSphere * 2 + numIndicesCylinder; // Total indices

    akxVertexAttrSpec attrSpecs[] =
    {
        {
            .fmt = afxVertexFormat_V3D,
            .flags = akxVertexFlag_POSITIONAL | akxVertexFlag_SPATIAL | akxVertexFlag_AFFINE,
            .usage = akxVertexUsage_POS | akxVertexUsage_POSITIONAL | akxVertexUsage_SPATIAL
        },
        {
            .fmt = afxVertexFormat_V3D,
            .flags = akxVertexFlag_NORMALIZED | akxVertexFlag_LINEAR,
            .usage = akxVertexUsage_NRM
        },
        {
            .fmt = afxVertexFormat_V2D,
            .flags = akxVertexFlag_NORMALIZED | akxVertexFlag_LINEAR,
            .usage = akxVertexUsage_UV
        }
    };

    akxVertexDataSpec vtdb = { 0 };
    vtdb.attrCnt = AFX_COUNTOF(attrSpecs);
    vtdb.vtxCnt = numVertices;

    akxVertexData vtd;
    AkxAcquireVertexDatas(sim, attrSpecs, 1, &vtdb, &vtd);

    afxV3d* pos = AkxExposeVertexData(vtd, 0, 0);
    afxV3d* nrm = AkxExposeVertexData(vtd, 1, 0);
    afxV2d* uv = AkxExposeVertexData(vtd, 2, 0);
    afxNat index = 0;

    // Generate spherical caps (top and bottom)
    for (afxNat hemisphere = 0; hemisphere < 2; ++hemisphere)
    {
        afxReal zOffset = (hemisphere == 0) ? (height / 2) : (-height / 2);

        for (afxNat i = 0; i <= stacks; ++i)
        {
            afxReal stackAngle = AFX_PI * i / stacks; // angle from top
            afxReal xy = radius * sinf(stackAngle); // radius of current stack
            afxReal z = zOffset + radius * cosf(stackAngle);  // height of current stack

            if (hemisphere == 1) {
                z = -z; // Flip z for the bottom hemisphere
            }

            for (afxNat j = 0; j <= slices; ++j)
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
    for (afxNat i = 0; i <= 1; ++i)
    {
        afxReal yOffset = (i == 0) ? (-height / 2) : (height / 2); // Top and bottom circles

        for (afxNat j = 0; j <= cylinderSlices; ++j)
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

    afxMeshTopology msht;
    {
        akxMeshTopologyBlueprint mshtb = { 0 };
        mshtb.triCnt = numIndices / 3;
        AfxAssembleMeshTopology(sim, 1, &mshtb, &msht);

        akxIndexedTriangle* tris = AfxGetMeshTriangles(msht, 0);

        // Generate indices for spherical caps
        afxNat idx = 0;

        for (afxNat hemisphere = 0; hemisphere < 2; ++hemisphere)
        {
            for (afxNat i = 0; i < stacks; ++i)
            {
                for (afxNat j = 0; j < slices; ++j)
                {
                    afxNat topLeft = (i * (slices + 1) + j) + (hemisphere * numVerticesSphere);
                    afxNat topRight = topLeft + 1;
                    afxNat bottomLeft = ((i + 1) * (slices + 1) + j) + (hemisphere * numVerticesSphere);
                    afxNat bottomRight = bottomLeft + 1;

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

        // Generate indices for cylindrical section
        for (afxNat i = 0; i < cylinderSlices; ++i)
        {
            afxNat topLeft = i;
            afxNat topRight = i + 1;
            afxNat bottomLeft = numVerticesCylinder + i;
            afxNat bottomRight = numVerticesCylinder + i + 1;

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

    if (inv)
    {
        AfxInvertMeshWinding(msht);
        AkxInvertNormalizedVertexData(vtd, 1, 0, numVertices);
    }

    akxMeshBlueprint mshb = { 0 };
    mshb.vtxCnt = numVertices;
    mshb.vtd = vtd;
    mshb.topology = msht;
    mshb.biasCnt = 1;

    afxMesh msh;
    AfxAssembleMeshes(sim, 1, &mshb, &msh);
    AfxReleaseObjects(1, &msht);
    AfxReleaseObjects(1, &vtd);

    return msh;
}

_AKX afxMesh AfxBuildPlaneMesh(afxSimulation sim, afxNat gridSizeX, afxNat gridSizeY, afxReal width, afxReal height)
{
    afxError err = NIL;
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    if (!gridSizeX)
        gridSizeX = 10;

    if (!gridSizeY)
        gridSizeY = 10;

    if (!width)
        width = 10.f;

    if (!height)
        height = 10.f;

    afxNat numVertices = (gridSizeX + 1) * (gridSizeY + 1);
    afxNat numIndices = gridSizeX * gridSizeY * 6; // Each grid cell is split into 2 triangles
    afxReal halfWidth = width / 2.0f;
    afxReal halfHeight = height / 2.0f;

    akxVertexAttrSpec attrSpecs[] =
    {
        {
            .fmt = afxVertexFormat_V3D,
            .flags = akxVertexFlag_POSITIONAL | akxVertexFlag_SPATIAL | akxVertexFlag_AFFINE,
            .usage = akxVertexUsage_POS | akxVertexUsage_POSITIONAL | akxVertexUsage_SPATIAL
        },
        {
            .fmt = afxVertexFormat_V3D,
            .flags = akxVertexFlag_NORMALIZED | akxVertexFlag_LINEAR,
            .usage = akxVertexUsage_NRM
        },
        {
            .fmt = afxVertexFormat_V2D,
            .flags = akxVertexFlag_NORMALIZED | akxVertexFlag_LINEAR,
            .usage = akxVertexUsage_UV
        }
    };

    akxVertexDataSpec vtdb = { 0 };
    vtdb.attrCnt = AFX_COUNTOF(attrSpecs);
    vtdb.vtxCnt = numVertices;

    akxVertexData vtd;
    AkxAcquireVertexDatas(sim, attrSpecs, 1, &vtdb, &vtd);

    afxV3d* pos = AkxExposeVertexData(vtd, 0, 0);
    afxV3d* nrm = AkxExposeVertexData(vtd, 1, 0);
    afxV2d* uv = AkxExposeVertexData(vtd, 2, 0);

    // Generate vertices
    afxNat index = 0;

    for (afxNat y = 0; y <= gridSizeY; ++y)
    {
        for (afxNat x = 0; x <= gridSizeX; ++x)
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

    afxMeshTopology msht;
    {
        akxMeshTopologyBlueprint mshtb = { 0 };
        mshtb.triCnt = numIndices / 3;
        AfxAssembleMeshTopology(sim, 1, &mshtb, &msht);

        akxIndexedTriangle* tris = AfxGetMeshTriangles(msht, 0);

        // Generate indices
        afxNat idx = 0;

        for (afxNat y = 0; y < gridSizeY; ++y)
        {
            for (afxNat x = 0; x < gridSizeX; ++x)
            {
                afxNat topLeft = y * (gridSizeX + 1) + x;
                afxNat topRight = topLeft + 1;
                afxNat bottomLeft = (y + 1) * (gridSizeX + 1) + x;
                afxNat bottomRight = bottomLeft + 1;

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

    akxMeshBlueprint mshb = { 0 };
    mshb.vtxCnt = numVertices;
    mshb.vtd = vtd;
    mshb.topology = msht;
    mshb.biasCnt = 1;

    afxMesh msh;
    AfxAssembleMeshes(sim, 1, &mshb, &msh);
    AfxReleaseObjects(1, &msht);
    AfxReleaseObjects(1, &vtd);

    return msh;
}
