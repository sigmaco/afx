#define _AFX_CLASSIFIED_VBUF 1
#include "afx/sim/modeling/afxModel.h"
#include "afx/sim/modeling/afxMesh.h"
#include "afx/sim/afxSimulation.h"
#include "afx/draw/afxDrawSystem.h"
#include "afx/sim/modeling/afxMaterial.h"
#include "afx/sim/modeling/afxMesh.h"

typedef struct
{
    afxSimulation       sim;
    afxMemory           mem;
    afxNat              vtxDataCnt;
    struct
    {
        afxNat          vtxCnt;
        afxNat          arrCnt;
        afxString const *arrSemantic[8];
        afxVertexFormat arrFmt[8];
        afxVertexUsage  arrUsage[8];
        afxNat          nextBaseVtx;
    } vtxData[16];
    afxVertexBuffer     vbuf[16]; // separado para usar multiacquire

    afxIndexBuffer      ibuf; // used by model builder to share ibuf. If nil, should create one.
    
} _afxMshCtorCachedData;

////////////////////////////////////////////////////////////////////////////////
// MESH BLUEPRINT                                                             //
////////////////////////////////////////////////////////////////////////////////

_AFX afxError _AfxMeshBlueprintValidate(afxMeshBlueprint *mshb)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(mshb, AFX_FCC_MSHB);

    AfxAssert(mshb->prim);
    AfxAssert(mshb->sections._pop);
    AfxAssert(mshb->idxCnt); // must have indexes
    AfxAssert(!mshb->idxSrc || mshb->idxSrc && mshb->srcIdxSiz); // if not using shared ibuf, must have size of index to generate a new ibuf.

    return err;
}

_AFX void AfxMeshBlueprintDiscard(afxMeshBlueprint *mshb)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(mshb, AFX_FCC_MSHB);

    AfxArrayDrop(&mshb->materials);

    for (afxNat i = 0; i < AfxArrayGetPop(&mshb->articulations); i++)
    {
        afxMeshBlueprintArticulation *art = AfxArrayGetElement(&mshb->articulations, i);

        if (art->boneName)
        {
            AfxStringDeallocate(art->boneName);
            art->boneName = NIL;
        }
    }
    AfxArrayDrop(&mshb->articulations);

    for (afxNat i = 0; i < AfxArrayGetPop(&mshb->sections); i++)
    {
        afxMeshBlueprintSection *sec = AfxArrayGetElement(&mshb->sections, i);

        if (sec->name)
        {
            AfxStringDeallocate(sec->name);
            sec->name = NIL;
        }
    }
    AfxArrayDrop(&mshb->sections);

    for (afxNat i = 0; i < AfxArrayGetPop(&mshb->arranges); i++)
    {
        afxMeshBlueprintVertexArrange *arr = AfxArrayGetElement(&mshb->arranges, i);

        if (arr->semantic)
        {
            AfxStringDeallocate(arr->semantic);
            arr->semantic = NIL;
        }
    }
    AfxArrayDrop(&mshb->arranges);

    mshb->fcc = NIL;
}

_AFX void AfxMeshBlueprintDeploy(afxMeshBlueprint *mshb, void *sim, afxUri const *name, afxNat vtxCnt, afxNat estVtxArrCnt, afxNat estSecCnt, afxNat estArtCnt, afxNat estMtlCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(mshb);
    mshb->fcc = AFX_FCC_MSHB;

    mshb->sim = sim;
    afxMemory mem = AfxSimulationGetMemory(sim);

    AfxUri128(&mshb->uri);

    if (name)
        AfxUriCopy(&mshb->uri.uri, name);

    AfxAssert(vtxCnt);
    mshb->vtxCnt = vtxCnt;

    AfxArrayDeploy(&mshb->arranges, mem, sizeof(afxMeshBlueprintVertexArrange), estVtxArrCnt);
    AfxArrayDeploy(&mshb->sections, mem, sizeof(afxMeshBlueprintSection), estSecCnt);
    AfxArrayDeploy(&mshb->materials, mem, sizeof(afxMaterial), estMtlCnt);
    AfxArrayDeploy(&mshb->articulations, mem, sizeof(afxMeshBlueprintArticulation), estArtCnt);

    mshb->prim = NIL;
    mshb->idxCnt = 0;
    mshb->idxSrc = NIL;
    mshb->srcIdxSiz = 0;
}

_AFX afxError AfxMeshBlueprintAddVertexArrange(afxMeshBlueprint *mshb, afxVertexRowSpecification const *spec)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(mshb, AFX_FCC_MSHB);
    afxNat idx;

    if (AFX_INVALID_INDEX == (idx = AfxArrayPushElement(&mshb->arranges, NIL))) AfxThrowError();
    else
    {
        afxMeshBlueprintVertexArrange *arr = AfxArrayGetElement(&mshb->arranges, idx);
        AfxAssert(arr);

        arr->semantic = spec->semantic && !AfxStringIsEmpty(spec->semantic) ? AfxStringClone(spec->semantic) : NIL;

        AfxAssert(AFX_VTX_FMT_TOTAL > spec->fmt);
        arr->fmt = spec->fmt;

        arr->usage = spec->usage;

        afxNat vtxSiz = (AFX_VTX_FMT_TOTAL > spec->srcFmt) ? AfxVertexFormatGetSize(spec->srcFmt) : 0;
        AfxAssert(!spec->src || (spec->src && vtxSiz));
        arr->src = spec->src;

        AfxAssert(AFX_VTX_FMT_TOTAL > spec->srcFmt);
        arr->srcFmt = spec->srcFmt;

    }
    return err;
}

_AFX void AfxMeshBlueprintSetTopology(afxMeshBlueprint *mshb, afxPrimTopology prim, afxNat idxCnt, void const *idxSrc, afxNat idxSrcSiz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(mshb, AFX_FCC_MSHB);

    AfxAssert(AFX_PRIM_TOPOLOGY_TOTAL > prim);
    mshb->prim = prim;

    AfxAssert(idxCnt);
    mshb->idxCnt = idxCnt;

    AfxAssert(!idxSrc || (idxSrc && idxSrcSiz));
    mshb->idxSrc = idxSrc;
    mshb->srcIdxSiz = idxSrcSiz;
}

_AFX afxError AfxMeshBlueprintAddSection(afxMeshBlueprint *mshb, afxString const *name, afxNat mtlIdx, afxNat firstTriIdx, afxNat triCnt, void const *idxSrc, afxNat srcIdxSiz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(mshb, AFX_FCC_MSHB);
    afxNat idx;

    if (AFX_INVALID_INDEX == (idx = AfxArrayPushElement(&mshb->sections, NIL))) AfxThrowError();
    else
    {
        afxMeshBlueprintSection *sec = AfxArrayGetElement(&mshb->sections, idx);
        AfxAssert(sec);

        sec->name = name && !AfxStringIsEmpty(name) ? AfxStringClone(name) : NIL;

        sec->mtlIdx = mtlIdx;

        AfxAssert(firstTriIdx < mshb->idxCnt);
        sec->firstTriIdx = firstTriIdx;

        AfxAssert(firstTriIdx + triCnt <= mshb->idxCnt);
        sec->triCnt = triCnt;

        AfxAssert(!idxSrc || (idxSrc && srcIdxSiz));
        sec->idxSrc = idxSrc;
        sec->srcIdxSiz = srcIdxSiz;
    }
    return err;
}

_AFX afxError AfxMeshBlueprintAddArticulation(afxMeshBlueprint *mshb, afxString const *name, afxNat triCnt, void const *tris)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(mshb, AFX_FCC_MSHB);
    afxNat idx;

    if (AFX_INVALID_INDEX == (idx = AfxArrayPushElement(&mshb->articulations, NIL))) AfxThrowError();
    else
    {
        afxMeshBlueprintArticulation *art = AfxArrayGetElement(&mshb->articulations, idx);
        AfxAssert(art);
        
        art->boneName = name && !AfxStringIsEmpty(name) ? AfxStringClone(name) : NIL;

        art->triCnt = triCnt;
        art->tris = tris;
    }
    return err;
}

AFX afxError AfxMeshBlueprintAddMaterial(afxMeshBlueprint *mshb, afxMaterial mtl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(mshb, AFX_FCC_MSHB);
    afxNat idx;

    if (AFX_INVALID_INDEX == (idx = AfxArrayPushElement(&mshb->materials, &mtl))) AfxThrowError();
    else
    {
        afxMaterial *slot = AfxArrayGetElement(&mshb->materials, idx);
        AfxAssert(slot);
    }
    return err;
}

AFX afxBool AfxMeshBlueprintFindMaterial(afxMeshBlueprint *mshb, afxUri const *uri, afxNat *idx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(mshb, AFX_FCC_MSHB);
    AfxAssertType(uri, AFX_FCC_URI);

    for (afxNat i = 0; i < AfxArrayGetPop(&mshb->materials); i++)
    {
        afxMaterial mtl = *(afxMaterial*)AfxArrayGetElement(&mshb->materials, i);
        
        if (AfxUriIsEquivalent(uri, AfxMaterialGetUri(mtl)))
        {
            *idx = i;
            return TRUE;
        }
    }
    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
// MESH                                                                       //
////////////////////////////////////////////////////////////////////////////////

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

    afxEntity ent = AfxGetAspectOwner(&mdl->asp);

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
            AfxQuatMakeAxialRotation(helpQuaternion, AfxSpawnV3d(0, 1, 0), 360.0f * texCoords[texCoordsIndex + 0]);
            AfxM4dAccumulateAxialRotationFromQuat((afxV4d*)helpMatrix, helpQuaternion);

            AfxTransformPivotV3d(&tangents[tangentIndex], helpVector, (afxV4d*)helpMatrix);
        }
    }

    afxMeshSection msec = AfxAcquireMeshSection(msh, AFX_PRIM_TOPOLOGY_TRI_LIST, numberIndices, 0, AfxAcquireMaterial(mdl, "parallelepiped"));
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

    afxEntity ent = AfxGetAspectOwner(&mdl->asp);

    afxMesh msh = AfxAcquireMesh(mdl, numberVertices, 1, "parallelepiped");
    AfxPopulateVertexBuffer(&msh->vbuf, AFX_VERTEX_ATTR_XYZW, numberVertices, 0, xy_vertices, 0);
    AfxPopulateVertexBuffer(&msh->vbuf, AFX_VERTEX_ATTR_TAN, numberVertices, 0, xy_tangents, 0);
    AfxPopulateVertexBuffer(&msh->vbuf, AFX_VERTEX_ATTR_NRM, numberVertices, 0, xy_normals, 0);
    AfxPopulateVertexBuffer(&msh->vbuf, AFX_VERTEX_ATTR_UV, numberVertices, 0, xy_texCoords, 0);
    afxMeshSection msec = AfxAcquireMeshSection(msh, AFX_PRIM_TOPOLOGY_TRI_LIST, numberIndices, 0, AfxAcquireMaterial(mdl, "parallelepiped"));
    AfxFillMeshSection(msec, numberIndices, xy_indices);

    afxM4d m4d;
    AfxM4dMakeIdentity(m4d);
    AfxM4dMakeScaling(m4d, AfxSpawnV4d(width, 10.0, height, 1.0));

    for (afxNat32 i = 0; i < numberVertices; i++)
    {
        AfxTransformV4d(((afxV4d*)msh->vbuf.attrs[AFX_VERTEX_ATTR_XYZW].pool)[i], ((afxV4d*)msh->vbuf.attrs[AFX_VERTEX_ATTR_XYZW].pool)[i], m4d);
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

    afxEntity ent = AfxGetAspectOwner(&mdl->asp);

    afxMesh msh = AfxAcquireMesh(mdl, numberVertices, 1, "cube");
    AfxPopulateVertexBuffer(&msh->vbuf, AFX_VERTEX_ATTR_XYZW, numberVertices, 0, cubeVertices, 0);
    AfxRescaleVertexBuffer(&msh->vbuf, AFX_VERTEX_ATTR_XYZW, scale);
    AfxPopulateVertexBuffer(&msh->vbuf, AFX_VERTEX_ATTR_TAN, numberVertices, 0, cubeTangents, 0);
    AfxPopulateVertexBuffer(&msh->vbuf, AFX_VERTEX_ATTR_NRM, numberVertices, 0, cubeNormals, 0);
    AfxPopulateVertexBuffer(&msh->vbuf, AFX_VERTEX_ATTR_UV, numberVertices, 0, cubeTexCoords, 0);
    afxMeshSection msec = AfxAcquireMeshSection(msh, AFX_PRIM_TOPOLOGY_TRI_LIST, numberIndices, 0, AfxAcquireMaterial(mdl, "cube"));
    AfxFillMeshSection(msec, numberIndices, cubeIndices);
    return msh;
}
#endif//0

#if 0
_AFX afxVertex const* AfxMeshGetVertices(afxMesh msh)
{
    AfxAssertObject(msh, AFX_FCC_MSH);
    return AfxArrayGetPool(&msh->vertices);
}

_AFX afxNat AfxMeshGetVertexCount(afxMesh msh)
{
    AfxAssertObject(msh, AFX_FCC_MSH);
    return AfxArrayGetPop(&msh->vertices);
}

_AFX afxVertexWeight const* AfxMeshGetWeights(afxMesh msh)
{
    AfxAssertObject(msh, AFX_FCC_MSH);
    return AfxArrayGetPool(&msh->weights);
}

_AFX afxNat AfxMeshGetWeightCount(afxMesh msh)
{
    AfxAssertObject(msh, AFX_FCC_MSH);
    return AfxArrayGetPop(&msh->weights);
}

_AFX afxTriangle const* AfxMeshGetTriangles(afxMesh msh)
{
    AfxAssertObject(msh, AFX_FCC_MSH);
    return (afxTriangle const*)AfxIndexBuffer.Expose(msh->ibo);
}

_AFX afxNat AfxMeshGetTriangleCount(afxMesh msh)
{
    AfxAssertObject(msh, AFX_FCC_MSH);
    return msh->ibo->idxCnt / 3;
}

_AFX afxResult _AfxMarkModelAsVolumeRecomputeNeeded(afxMesh msh, afxModel mdl, void *data)
{
    AfxAssertObject(msh, AFX_FCC_MSH);
    AfxAssertObject(mdl, AFX_FCC_MDL);
    (void)data;
    AfxObjectSetFlags(&(mdl->obj), AFX_MDL_FLAG_RECOMPUTE_VOLUME);
    return 1; // continue
}

_AFX afxAabb const* AfxMeshRecomputeVolume(afxMesh msh)
{
    AfxEntry("msh=%p", msh);
    AfxAssertObject(msh, AFX_FCC_MSH);
    afxAabb* aabb = &(msh->volume);

    if (AfxObjectTestFlags(&msh->obj, AFX_MSH_FLAG_RECOMPUTE_VOLUME))
    {
        AfxAabbMakeIdentity(aabb);

        AfxObjectClearFlags(&msh->obj, AFX_MSH_FLAG_RECOMPUTE_VOLUME);

        AfxEcho("Mesh %p dimensions: %f, %f, %f.", msh, aabb->sup[0] - aabb->inf[0], aabb->sup[1] - aabb->inf[1], aabb->sup[2] - aabb->inf[2]);
    }
    return aabb;
}

_AFX afxBool AfxMeshIsDeformable(afxMesh msh)
{
    AfxAssertObject(msh, AFX_FCC_MSH);
    return FALSE;
}

_AFX afxAabb* AfxMeshGetVolume(afxMesh msh, afxAabb *aabb)
{
    AfxAssertObject(msh, AFX_FCC_MSH);
    AfxAssert(aabb);

    if (AfxObjectTestFlags(&msh->obj, AFX_MSH_FLAG_RECOMPUTE_VOLUME))
        AfxAabbCopy(AfxMeshRecomputeVolume(msh), aabb);
    else
        AfxAabbCopy(&msh->volume, aabb);

    return aabb;
}

#endif

_AFX afxMeshRig* AfxMeshGenerateRig(afxMesh msh, afxSkeleton src, afxSkeleton dst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(msh, AFX_FCC_MSH);
    afxMeshRig *rig = AfxAllocate(NIL, sizeof(*rig), AfxSpawnHint());

    afxBool transfered = src != dst;

    rig->msh = msh;
    rig->boneCnt = msh->articulationCnt;
    rig->srcSkl = src;
    rig->srcBoneIndices = AfxAllocate(NIL, rig->boneCnt * sizeof(afxNat), AfxSpawnHint());
    rig->dstSkl = dst;
    rig->dstBoneIndices = transfered ? AfxAllocate(NIL, rig->boneCnt * sizeof(afxNat), AfxSpawnHint()) : rig->srcBoneIndices;

    for (afxNat i = 0; i < msh->articulationCnt; i++)
    {
        if (!msh->articulations[i].boneName || !AfxSkeletonFindBone(rig->dstSkl, msh->articulations[i].boneName, &rig->dstBoneIndices[i]))
        {
            AfxError("Unable to find bone: '%s' in the destination skeleton", msh->articulations[i].boneName ? AfxPushString(msh->articulations[i].boneName) : NIL);
            AfxAssert(rig->dstBoneIndices[i] == AFX_INVALID_INDEX);
        }

        if (transfered && !AfxSkeletonFindBone(rig->srcSkl, msh->articulations[i].boneName, &rig->srcBoneIndices[i]))
        {
            AfxError("Unable to find bone: '%s' in the source skeleton", msh->articulations[i].boneName ? AfxPushString(msh->articulations[i].boneName) : NIL);
            AfxAssert(rig->srcBoneIndices[i] == AFX_INVALID_INDEX);
        }
    }

    return rig;
}

_AFX void _AfxMshsRecomputeAabbCallback(void const *vtx, void *data)
{
    afxAabb *aabb = data;
    // WARNING: Expects that position vectors are afxV4d-based.
    AfxAabbEncapsulatePoint(aabb, vtx);
}

_AFX afxMeshArticulation* AfxMeshGetArticulation(afxMesh msh, afxNat artIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(msh, AFX_FCC_MSH);
    AfxAssert(artIdx < msh->articulationCnt);

    afxMeshArticulation *msha = &msh->articulations[artIdx];
    AfxAssertType(msha, AFX_FCC_MSHA);

    return msha;
}

_AFX afxNat AfxMeshGetArticulationCount(afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(msh, AFX_FCC_MSH);
    return msh->articulationCnt;
}

_AFX afxBool AfxMeshIsRigid(afxMesh msh)
{
    // The vertices in a Granny mesh are either deforming or rigid. 
    // Deforming meshes are ones that are bound to multiple bones, whereas rigid meshes are ones that are bound to a single bone (and thus move "rigidly" with that bone). 

    afxError err = AFX_ERR_NONE;
    AfxAssertObject(msh, AFX_FCC_MSH);
    return (1 >= msh->articulationCnt);
}

_AFX void AfxMeshBindMaterial(afxMesh msh, afxNat mtlIdx, afxMaterial mtl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(msh, AFX_FCC_MSH);
    AfxAssertObject(mtl, AFX_FCC_MTL);
    AfxAssert(mtlIdx < msh->mtlSlotCnt);

    if (mtl)
    {
        AfxAssertObject(mtl, AFX_FCC_MTL);
        
        if (!(AfxObjectReacquire(&mtl->obj, &msh->obj, NIL, NIL, NIL)))
            AfxThrowError();
    }

    afxMaterial mtl2 = msh->mtlSlots[mtlIdx].mtl;
    msh->mtlSlots[mtlIdx].mtl = mtl;

    if (mtl2)
    {
        AfxAssertObject(mtl2, AFX_FCC_MTL);
        AfxObjectRelease(&mtl2->obj);
    }
}

_AFX afxMaterial AfxMeshFindMaterial(afxMesh msh, afxUri const *name)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(msh, AFX_FCC_MSH);
    AfxAssertType(name, AFX_FCC_URI);

    for (afxNat i = 0; i < msh->mtlSlotCnt; i++)
    {
        afxMaterial mtl = msh->mtlSlots[i].mtl;

        if (mtl)
        {
            AfxAssertObject(mtl, AFX_FCC_MTL);

            if (AfxUriIsEquivalent(AfxMaterialGetUri(mtl), name))
                return mtl;
        }
    }

    return NIL;
}

_AFX afxMaterial AfxMeshGetBoundMaterial(afxMesh msh, afxNat mtlIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(msh, AFX_FCC_MSH);
    AfxAssert(mtlIdx < msh->mtlSlotCnt);
    afxMaterial mtl = msh->mtlSlots[mtlIdx].mtl;
    AfxTryAssertObject(mtl, AFX_FCC_MTL);
    return mtl;
}

_AFX afxNat AfxMeshGetMaterialCount(afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(msh, AFX_FCC_MSH);
    return msh->mtlSlotCnt;
}

_AFX afxIndexBuffer AfxMeshGetIndexBuffer(afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(msh, AFX_FCC_MSH);
    AfxAssertType(msh->topology, AFX_FCC_MSHT);

    afxIndexBuffer ibuf = msh->topology->ibuf;
    AfxAssertObject(ibuf, AFX_FCC_IBUF);

    return ibuf;
}

_AFX afxMeshSection* AfxMeshGetSection(afxMesh msh, afxNat secIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(msh, AFX_FCC_MSH);
    AfxAssertType(msh->topology, AFX_FCC_MSHT);
    AfxAssert(secIdx < msh->topology->secCnt);

    afxMeshSection *mshs = &msh->topology->sections[secIdx];
    AfxAssertType(mshs, AFX_FCC_MSHS);

    return mshs;
}

_AFX afxNat AfxMeshGetSectionCount(afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(msh, AFX_FCC_MSH);
    AfxAssertType(msh->topology, AFX_FCC_MSHT);
    return msh->topology->secCnt;
}

_AFX void AfxMeshTransform(afxMesh msh, afxV3d const affine, afxM3d const linear, afxM3d const invLinear, afxReal affineTol, afxReal linearTol, afxFlags flags)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(msh, AFX_FCC_MSH);
    AfxAssert(affine);
    AfxAssert(linear);
    AfxAssert(invLinear);
    //void __cdecl granny::TransformMesh(granny::mesh *Mesh, const float *Affine3, const float *Linear3x3, const float *InverseLinear3x3, float AffineTolerance, float LinearTolerance, unsigned int Flags)
    AfxThrowError();
#if 0
    const float *v7; // ebx
    granny::mesh *v8; // ebp
    granny::vertex_data *v9; // eax
    const float *v10; // esi
    granny::vertex_data *v11; // eax
    int v12; // esi
    granny::curve2 *v13; // ebp
    int i; // ebx
    int v15; // edx
    bool v16; // sf
    unsigned __int8 v17; // of
    int v18; // ecx
    granny::morph_target *v19; // edx
    int v20; // eax
    int v21; // ecx
    granny::curve2 *v22; // ebp
    unsigned int k; // eax
    int v24; // ecx
    int v25; // edx
    float v26; // ecx
    int v27; // edx
    int v28; // ecx
    float v29; // edx
    int v30; // ecx
    int v31; // edx
    granny::curve2 *v32; // ebp
    unsigned int j; // eax
    float v34; // ecx
    float v35; // edx
    float v36; // ecx
    float v37; // edx
    float v38; // ecx
    float v39; // edx
    float v40; // ecx
    float v41; // edx
    long double v42; // st7
    long double v43; // st7
    int v44; // edx
    int v45; // esi
    int v46; // ebp
    int v47; // [esp+14h] [ebp-54h]
    int VertNum; // [esp+18h] [ebp-50h]
    int VertNuma; // [esp+18h] [ebp-50h]
    int VertNumb; // [esp+18h] [ebp-50h]
    int MorphTargetIndex; // [esp+1Ch] [ebp-4Ch]
    int MorphTargetIndexa; // [esp+1Ch] [ebp-4Ch]
    float LocalTransform[9]; // [esp+20h] [ebp-48h]
    float Dest; // [esp+44h] [ebp-24h]
    int v55; // [esp+48h] [ebp-20h]
    int v56; // [esp+4Ch] [ebp-1Ch]
    float v57; // [esp+50h] [ebp-18h]
    int v58; // [esp+54h] [ebp-14h]
    int v59; // [esp+58h] [ebp-10h]
    float v60; // [esp+5Ch] [ebp-Ch]
    int v61; // [esp+60h] [ebp-8h]
    int v62; // [esp+64h] [ebp-4h]

    v7 = InverseLinear3x3;
    v8 = Mesh;
    v9 = Mesh->PrimaryVertexData;
    v10 = Affine3;
    if (v9)
    {
        granny::TransformVertices( v9->VertexCount, v9->VertexType, v9->Vertices, Affine3, Linear3x3, InverseLinear3x3, Flags & 1, 0);

        v11 = Mesh->PrimaryVertexData;
        MorphTargetIndex = 0;
        if (v11->VertexAnnotationSetCount > 0)
        {
            v47 = 0;
            while (1)
            {
                v12 = (int)&v11->VertexAnnotationSets[v47];

                if ((unsigned __int8)granny::StringsAreEqual(*(const char **)v12, "VertexMorphCurvePosition"))
                {
                    v13 = *(granny::curve2 **)(v12 + 12);

                    for (i = 0; i < *(_DWORD *)(v12 + 8); ++i)
                    {
                        if (!granny::CurveIsIdentity(v13))
                            granny::TransformCurveVec3(Affine3, Linear3x3, AffineTolerance, LinearTolerance, v13);

                        ++v13;
                    }
                    v7 = InverseLinear3x3;
                }
                else if ((unsigned __int8)granny::StringsAreEqual(*(const char **)v12, "VertexMorphCurveNormal") || (unsigned __int8)granny::StringsAreEqual(*(const char **)v12, "VertexMorphCurveTangentBinormalCross"))
                {
                    v32 = *(granny::curve2 **)(v12 + 12);
                    VertNumb = 0;

                    if (*(_DWORD *)(v12 + 8) > 0)
                    {
                        for (j = Flags & 1; ; j = Flags & 1)
                        {
                            v34 = v7[3];
                            LocalTransform[0] = *v7;
                            v35 = v7[6];
                            LocalTransform[1] = v34;
                            v36 = v7[1];
                            LocalTransform[2] = v35;
                            v37 = v7[4];
                            LocalTransform[3] = v36;
                            v38 = v7[7];
                            LocalTransform[4] = v37;
                            v39 = v7[2];
                            LocalTransform[5] = v38;
                            v40 = v7[5];
                            LocalTransform[6] = v39;
                            v41 = v7[8];
                            LocalTransform[7] = v40;
                            LocalTransform[8] = v41;

                            if (j)
                            {
                                v42 = sqrt(LocalTransform[2] * LocalTransform[2] + LocalTransform[1] * LocalTransform[1] + LocalTransform[0] * LocalTransform[0]);

                                if (v42 <= 0.0000099999997)
                                {
                                    LocalTransform[2] = 0.0;
                                    LocalTransform[1] = 0.0;
                                    LocalTransform[0] = 0.0;
                                }
                                else
                                {
                                    v43 = 1.0 / v42;
                                    LocalTransform[0] = LocalTransform[0] * v43;
                                    LocalTransform[1] = LocalTransform[1] * v43;
                                    LocalTransform[2] = LocalTransform[2] * v43;
                                }
                                granny::NormalizeOrZero3(&LocalTransform[3]);
                                granny::NormalizeOrZero3(&LocalTransform[6]);
                            }

                            if (!granny::CurveIsIdentity(v32))
                                granny::TransformCurveVec3(GlobalZeroVector_1, LocalTransform, 1.0, LinearTolerance, v32);

                            ++v32;

                            if (++VertNumb >= *(_DWORD *)(v12 + 8))
                                break;
                        }
                    }
                }
                else
                {
                    if (!(unsigned __int8)granny::StringsAreEqual(*(const char **)v12, "VertexMorphCurveTangent") && !(unsigned __int8)granny::StringsAreEqual(*(const char **)v12, "VertexMorphCurveBinormal"))
                    {
                        goto LABEL_11;
                    }

                    v22 = *(granny::curve2 **)(v12 + 12);
                    VertNuma = 0;

                    if (*(_DWORD *)(v12 + 8) > 0)
                    {
                        for (k = Flags & 1; ; k = Flags & 1)
                        {
                            v24 = *((_DWORD *)Linear3x3 + 1);
                            Dest = *Linear3x3;
                            v25 = *((_DWORD *)Linear3x3 + 2);
                            v55 = v24;
                            v26 = Linear3x3[3];
                            v56 = v25;
                            v27 = *((_DWORD *)Linear3x3 + 4);
                            v57 = v26;
                            v28 = *((_DWORD *)Linear3x3 + 5);
                            v58 = v27;
                            v29 = Linear3x3[6];
                            v59 = v28;
                            v30 = *((_DWORD *)Linear3x3 + 7);
                            v60 = v29;
                            v31 = *((_DWORD *)Linear3x3 + 8);
                            v61 = v30;
                            v62 = v31;

                            if (k)
                            {
                                granny::NormalizeOrZero3(&Dest);
                                granny::NormalizeOrZero3(&v57);
                                granny::NormalizeOrZero3(&v60);
                            }

                            if (!granny::CurveIsIdentity(v22))
                                granny::TransformCurveVec3(GlobalZeroVector_1, &Dest, 1.0, LinearTolerance, v22);

                            ++v22;

                            if (++VertNuma >= *(_DWORD *)(v12 + 8))
                                break;
                        }
                    }
                }
                v8 = Mesh;
            LABEL_11:
                v11 = v8->PrimaryVertexData;
                v15 = v11->VertexAnnotationSetCount;
                v17 = __OFSUB__(MorphTargetIndex + 1, v15);
                v16 = MorphTargetIndex++ + 1 - v15 < 0;
                ++v47;

                if (!(v16 ^ v17))
                {
                    v10 = Affine3;
                    break;
                }
            }
        }
    }

    v18 = 0;
    MorphTargetIndexa = 0;

    if (v8->MorphTargetCount > 0)
    {
        VertNum = 0;

        do
        {
            v19 = v8->MorphTargets;
            v20 = *(int *)((char *)&v19->VertexData + v18);
            v21 = (int)v19 + v18;

            if (v20)
            {
                if (*(_DWORD *)(v21 + 8))
                    granny::TransformVertices( *(_DWORD *)(v20 + 4), *(const granny::data_type_definition **)v20, *(void **)(v20 + 8), v10, Linear3x3, v7, 0, 1);
                else
                    granny::TransformVertices( *(_DWORD *)(v20 + 4), *(const granny::data_type_definition **)v20, *(void **)(v20 + 8), v10, Linear3x3, v7, Flags & 1, 0);
            }

            v44 = v8->MorphTargetCount;
            v18 = VertNum + 12;
            v17 = __OFSUB__(MorphTargetIndexa + 1, v44);
            v16 = MorphTargetIndexa++ + 1 - v44 < 0;
            VertNum += 12;
        } while (v16 ^ v17);
    }

    if (Flags & 2 && v8->PrimaryTopology && (Linear3x3[8] * Linear3x3[4] - Linear3x3[7] * Linear3x3[5]) * *Linear3x3 - (Linear3x3[3] * Linear3x3[8] - Linear3x3[5] * Linear3x3[6]) * Linear3x3[1] + (Linear3x3[7] * Linear3x3[3] - Linear3x3[4] * Linear3x3[6]) * Linear3x3[2] < 0.0)
    {
        granny::InvertTriTopologyWinding(v8->PrimaryTopology);
    }

    v45 = 0;

    if (v8->BoneBindingCount > 0)
    {
        v46 = 0;

        do
        {
            granny::TransformBoundingBox(Affine3, Linear3x3, Mesh->BoneBindings[v46].OBBMin, Mesh->BoneBindings[v46].OBBMax);
            ++v45;
            ++v46;
        } while (v45 < Mesh->BoneBindingCount);
    }
#endif
}

_AFX afxMeshMorph* AfxMeshGetMorph(afxMesh msh, afxNat morphIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(msh, AFX_FCC_MSH);
    AfxAssert(morphIdx < msh->morphCnt);

    afxMeshMorph *mshm = &msh->morphs[morphIdx];
    //AfxAssertType(mshm, AFX_FCC_MSHM);

    return mshm;
}

_AFX afxNat AfxMeshGetMorphCount(afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(msh, AFX_FCC_MSH);
    return msh->morphCnt;
}

_AFX afxVertexBuffer AfxMeshGetVertexBuffer(afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(msh, AFX_FCC_MSH);

    afxVertexBuffer vbuf = msh->vbuf;
    AfxAssertObject(vbuf, AFX_FCC_VBUF);

    return vbuf;
}

_AFX afxUri const* AfxMeshGetUri(afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(msh, AFX_FCC_MSH);
    return msh->uri;
}

_AFX afxBool _AfxMshEventHandler(afxObject *obj, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxMesh msh = (void*)obj;
    AfxAssertObject(msh, AFX_FCC_MSH);
    (void)ev;
    return FALSE;
}

_AFX afxBool _AfxMshEventFilter(afxObject *obj, afxObject *watched, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxMesh msh = (void*)obj;
    AfxAssertObject(msh, AFX_FCC_MSH);
    (void)watched;
    (void)ev;
    return FALSE;
}

_AFX afxError _AfxMshDtor(afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(msh, AFX_FCC_MSH);
    AfxEntry("msh=%p", msh);

    afxSimulation sim = AfxObjectGetProvider(&msh->obj);
    afxMemory mem = AfxSimulationGetMemory(sim);

    afxNat i;

    for (afxNat i = 0; i < msh->articulationCnt; i++)
    {
        afxMeshArticulation *msha = &msh->articulations[i];

        if (msha->boneName)
            AfxStringDeallocate(msha->boneName);
    }

    if (msh->articulations)
        AfxDeallocate(mem, msh->articulations);

    for (i = 0; i < msh->mtlSlotCnt; i++)
    {
        if (msh->mtlSlots[i].mtl)
            AfxObjectRelease(&(msh->mtlSlots[i].mtl->obj));
    }

    if (msh->mtlSlots)
        AfxDeallocate(mem, msh->mtlSlots);

    if (msh->topology)
    {
        AfxAssertType(msh->topology, AFX_FCC_MSHT);

        if (msh->topology->ibuf)
            AfxObjectRelease(&msh->topology->ibuf->buf.obj);

        for (afxNat i = 0; i < msh->topology->secCnt; i++)
        {
            afxMeshSection *mshs = &(msh->topology->sections[i]);

            if (mshs->name)
                AfxStringDeallocate(mshs->name);
        }

        AfxDeallocate(mem, msh->topology->sections);
        AfxDeallocate(mem, msh->topology);
    }

    if (msh->morphs)
        AfxDeallocate(mem, msh->morphs);

    if (msh->vbuf)
        AfxObjectRelease(&msh->vbuf->buf.obj);

    if (msh->uri)
        AfxUriDeallocate(msh->uri);

    return err;
}

_AFX afxError _AfxMshCtor(_afxMshCtorCachedData *cache, afxNat idx, afxMesh msh, afxMeshBlueprint const blueprints[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(msh, AFX_FCC_MSH);
    AfxEntry("msh=%p", msh);

    AfxAssertObject(cache->sim, AFX_FCC_SIM);
    afxMeshBlueprint const *blueprint = &blueprints[idx];
    AfxAssert(blueprint);

    afxMemory mem = cache->mem;

    msh->uri = AfxUriIsBlank(&blueprint->uri.uri) ? NIL : AfxUriClone(&blueprint->uri.uri);

    afxNat vbufIdx = AFX_INVALID_INDEX;
    afxBool vbufFound = FALSE;

    for (afxNat j = 0; j < cache->vtxDataCnt; j++)
    {
        if (cache->vtxData[j].arrCnt == AfxArrayGetPop(&blueprint->arranges))
        {
            for (afxNat k = 0; k < cache->vtxData[j].arrCnt; k++)
            {
                afxMeshBlueprintVertexArrange const *mbva = AfxArrayGetElement(&blueprint->arranges, k);

                if ((cache->vtxData[j].arrFmt[k] == mbva->fmt) || 0 == AfxStringDifferCi(cache->vtxData[j].arrSemantic[k], mbva->semantic))
                {
                    vbufIdx = j;
                    vbufFound = TRUE;
                    break;
                }
            }
        }

        if (vbufFound)
            break;
    }

    AfxAssert(vbufFound); // BuildMeshes must ensure vertex data validation.
    AfxAssert(vbufIdx != AFX_INVALID_INDEX);

    afxVertexBuffer vbuf = cache->vbuf[vbufIdx];
    AfxAssertObject(vbuf, AFX_FCC_VBUF);

    if (!(AfxObjectReacquire(&vbuf->buf.obj, &msh->obj, NIL, NIL, NIL))) AfxThrowError();
    else
    {
        msh->vbuf = vbuf;
        msh->baseVtx = cache->vtxData[vbufIdx].nextBaseVtx;
        msh->vtxCnt = blueprint->vtxCnt;
        msh->morphs = NIL;
        msh->morphCnt = 0;

        for (afxNat i = 0; i < AfxArrayGetPop(&blueprint->arranges); i++)
        {
            afxMeshBlueprintVertexArrange const *mbva = AfxArrayGetElement(&blueprint->arranges, i);

            if (mbva->src)
                if (AfxVertexBufferUpdate(vbuf, i, msh->baseVtx, msh->vtxCnt, mbva->src, mbva->srcFmt))
                    AfxThrowError();
        }

        cache->vtxData[vbufIdx].nextBaseVtx += msh->vtxCnt;

        if (!(msh->topology = AfxAllocate(mem, sizeof(*msh->topology), AfxSpawnHint()))) AfxThrowError();
        else
        {
            afxIndexBuffer ibuf = cache->ibuf; // BuildMeshes must ensure afxIndexBuffer validation
            AfxAssertObject(ibuf, AFX_FCC_IBUF);

            if (!(AfxObjectReacquire(&ibuf->buf.obj, &msh->obj, NIL, NIL, NIL))) AfxThrowError();
            else
            {
                msh->topology->fcc = AFX_FCC_MSHT;
                msh->topology->ibuf = ibuf;
                msh->topology->idxRgn = idx;                
                msh->topology->primType = blueprint->prim;
                AfxAssert(msh->topology->primType);

                if (blueprint->idxSrc)
                    if (AfxIndexBufferUpdate(ibuf, msh->topology->idxRgn, 0, blueprint->idxCnt, blueprint->idxSrc, blueprint->srcIdxSiz))
                        AfxThrowError();

                if (!err)
                {
                    afxNat secCnt = AfxArrayGetPop(&blueprint->sections);
                    AfxAssert(secCnt);

                    if (!(msh->topology->sections = AfxAllocate(mem, secCnt * sizeof(msh->topology->sections[0]), AfxSpawnHint()))) AfxThrowError();
                    else
                    {
                        msh->topology->secCnt = 0;

                        for (afxNat i = 0; i < secCnt; i++)
                        {
                            afxMeshBlueprintSection const *secb = AfxArrayGetElement(&blueprint->sections, i);
                            afxMeshSection *mshs = &(msh->topology->sections[i]);
                            mshs->fcc = AFX_FCC_MSHS;
                            mshs->mtlIdx = secb->mtlIdx;
                            mshs->firstTriIdx = secb->firstTriIdx;
                            mshs->triCnt = secb->triCnt;

                            mshs->name = secb->name && !AfxStringIsEmpty(secb->name) ? AfxStringClone(secb->name) : NIL;

                            if (secb->idxSrc)
                                if (AfxIndexBufferUpdate(ibuf, msh->topology->idxRgn, mshs->firstTriIdx, mshs->triCnt, secb->idxSrc, secb->srcIdxSiz))
                                    AfxThrowError();

                            AfxAabbMakeIdentity(&mshs->aabb);
                            AfxAssert(AfxVertexBufferGetStride(vbuf, 0) == sizeof(afxV4d)); // actually, _AfxMshsRecomputeAabbCallback expects afxV4d-based vectors and position at vertex arrange 0. TODO: someone need to do refactoring here.
                            AfxIndexBufferForEachVertex(ibuf, msh->topology->idxRgn, mshs->firstTriIdx, mshs->triCnt, vbuf, 0, msh->baseVtx, _AfxMshsRecomputeAabbCallback, &mshs->aabb);

                            ++msh->topology->secCnt;
                        }

                        AfxAssert(msh->topology->secCnt == secCnt);

                        msh->mtlSlots = NIL;
                        afxNat mtlCnt = AfxArrayGetPop(&blueprint->materials);

                        if (mtlCnt && !(msh->mtlSlots = AfxAllocate(mem, mtlCnt * sizeof(msh->mtlSlots[0]), AfxSpawnHint()))) AfxThrowError();
                        else
                        {
                            msh->mtlSlotCnt = 0;

                            for (afxNat i = 0; i < mtlCnt; i++)
                            {
                                afxMaterialSlot *slot = &msh->mtlSlots[i];
                                AfxAssert(slot);
                                afxMaterial mtl = *(afxMaterial*)AfxArrayGetElement(&blueprint->materials, i);
                                slot->mtl = mtl;

                                if (mtl)
                                {
                                    AfxAssertObject(slot->mtl, AFX_FCC_MTL);
                                    AfxObjectReacquire(&mtl->obj, &msh->obj, NIL, NIL, NIL);
                                }
                                ++msh->mtlSlotCnt;
                            }
                        }

                        if (!err)
                        {
                            afxNat artCnt = AfxArrayGetPop(&blueprint->articulations);
                            AfxAssert(artCnt);
                            msh->articulations = NIL;
                            
                            if (artCnt && !(msh->articulations = AfxAllocate(mem, artCnt * sizeof(msh->articulations[0]), AfxSpawnHint()))) AfxThrowError();
                            else
                            {
                                msh->articulationCnt = 0;

                                for (afxNat i = 0; i < artCnt; i++)
                                {
                                    afxMeshBlueprintArticulation const *artb = AfxArrayGetElement(&blueprint->articulations, i);
                                    afxMeshArticulation *msha = &msh->articulations[i];
                                    msha->fcc = AFX_FCC_MSHA;
                                    msha->boneName = artb->boneName && !AfxStringIsEmpty(artb->boneName) ? AfxStringClone(artb->boneName) : NIL;
                                    msha->triCnt = artb->triCnt;
                                    msha->triIndices = (void*)artb->tris; // ???

                                    AfxAssert(artb->tris == NIL); // Não sabemos o que fazer com isto ainda. K

                                    AfxAabbMakeIdentity(&msha->aabb);
                                    AfxAssert(AfxVertexBufferGetStride(vbuf, 0) == sizeof(afxV4d)); // actually, _AfxMshsRecomputeAabbCallback expects afxV4d-based vectors and position at vertex arrange 0. TODO: someone need to do refactoring here.
                                    AfxIndexBufferForEachVertex(ibuf, msh->topology->idxRgn, 0, msha->triCnt, vbuf, 0, msh->baseVtx, _AfxMshsRecomputeAabbCallback, &msha->aabb);

                                    msh->articulationCnt++;
                                }
                            }

                            msh->extData = NIL;

                            if (err)
                            {
                                if (msh->articulations) // what if tris is set?
                                    AfxDeallocate(mem, msh->articulations);
                            }
                        }

                        if (err)
                        {
                            if (msh->mtlSlots)
                            {
                                for (afxNat i = 0; i < msh->mtlSlotCnt; i++)
                                {
                                    afxMaterialSlot *slot = &msh->mtlSlots[i];
                                    
                                    if (slot->mtl)
                                    {
                                        AfxAssertObject(slot->mtl, AFX_FCC_MTL);
                                        AfxObjectRelease(&slot->mtl->obj);
                                    }
                                }
                                AfxDeallocate(mem, msh->mtlSlots);
                            }
                        }

                        if (err)
                        {
                            AfxAssert(msh->topology->sections);
                            AfxDeallocate(mem, msh->topology->sections);
                        }
                    }
                }

                if (err)
                    AfxObjectRelease(&msh->topology->ibuf->buf.obj);
            }

            if (err)
            {
                AfxAssert(msh->topology);
                AfxDeallocate(mem, msh->topology);
            }
        }


        if (err)
            AfxObjectRelease(&msh->vbuf->buf.obj);
    }

    return err;
}

_AFX afxError AfxSimulationBuildMeshes(afxSimulation sim, afxNat cnt, afxMeshBlueprint blueprint[], afxMesh msh[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(sim, AFX_FCC_SIM);
    AfxAssert(cnt);
    AfxAssert(blueprint);
    AfxAssert(msh);

    _afxMshCtorCachedData cache = { 0 };
    cache.sim = sim;
    cache.mem = AfxSimulationGetMemory(sim);

    afxNat mshCnt = cnt;

    afxIndexBufferBlueprint ibub;
    AfxIndexBufferBlueprintBegin(&ibub, sim->dctx, mshCnt);

    for (afxNat i = 0; i < mshCnt; i++)
    {
        afxMeshBlueprint *mshb = &blueprint[i];
        
        if (AfxIndexBufferBlueprintPushRegion(&ibub, mshb->idxCnt, NIL, 0))
            AfxThrowError();

        AfxAssert(AfxArrayGetPop(&blueprint->arranges));

        if (0 == cache.vtxDataCnt)
        {
            cache.vbuf[cache.vtxDataCnt] = NIL;
            cache.vtxData[cache.vtxDataCnt].nextBaseVtx = 0;
            cache.vtxData[cache.vtxDataCnt].vtxCnt += mshb->vtxCnt;
            cache.vtxData[cache.vtxDataCnt].arrCnt = AfxArrayGetPop(&blueprint->arranges);

            for (afxNat j = 0; j < cache.vtxData[cache.vtxDataCnt].arrCnt; j++)
            {
                afxMeshBlueprintVertexArrange *mbva = AfxArrayGetElement(&blueprint->arranges, j);
                cache.vtxData[cache.vtxDataCnt].arrSemantic[j] = mbva->semantic;
                cache.vtxData[cache.vtxDataCnt].arrFmt[j] = mbva->fmt;
                cache.vtxData[cache.vtxDataCnt].arrUsage[j] = mbva->usage;
            }
            cache.vtxDataCnt++;
        }
        else
        {
            afxBool found = FALSE;

            for (afxNat j = 0; j < cache.vtxDataCnt; j++)
            {
                if (cache.vtxData[j].arrCnt == AfxArrayGetPop(&blueprint->arranges))
                {
                    for (afxNat k = 0; k < cache.vtxData[j].arrCnt; k++)
                    {
                        afxMeshBlueprintVertexArrange *mbva = AfxArrayGetElement(&blueprint->arranges, k);

                        if ((cache.vtxData[j].arrFmt[k] == mbva->fmt) || 0 == AfxStringDifferCi(cache.vtxData[j].arrSemantic[k], mbva->semantic))
                        {
                            cache.vtxData[j].vtxCnt += mshb->vtxCnt;
                            found = TRUE;
                            break;
                        }
                    }
                }

                if (found)
                    break;
            }

            if (!found)
            {
                cache.vbuf[cache.vtxDataCnt] = NIL;
                cache.vtxData[cache.vtxDataCnt].nextBaseVtx = 0;
                cache.vtxData[cache.vtxDataCnt].vtxCnt += mshb->vtxCnt;
                cache.vtxData[cache.vtxDataCnt].arrCnt = AfxArrayGetPop(&blueprint->arranges);;

                for (afxNat j = 0; j < cache.vtxData[cache.vtxDataCnt].arrCnt; j++)
                {
                    afxMeshBlueprintVertexArrange *mbva = AfxArrayGetElement(&blueprint->arranges, j);
                    cache.vtxData[cache.vtxDataCnt].arrSemantic[j] = mbva->semantic;
                    cache.vtxData[cache.vtxDataCnt].arrFmt[j] = mbva->fmt;
                    cache.vtxData[cache.vtxDataCnt].arrUsage[j] = mbva->usage;
                }
                cache.vtxDataCnt++;
            }
        }
    }

    afxVertexBufferBlueprint vbub[16];

    for (afxNat i = 0; i < cache.vtxDataCnt; i++)
    {
        AfxVertexBufferBlueprintReset(&vbub[i], cache.vtxData[i].vtxCnt);

        for (afxNat j = 0; j < cache.vtxData[i].arrCnt; j++)
            if (AfxVertexBufferBlueprintAddRow(&vbub[i], cache.vtxData[i].arrSemantic[j], cache.vtxData[i].arrFmt[j], cache.vtxData[i].arrUsage[j], NIL, NIL))
                AfxThrowError();

        if (err)
        {
            for (afxNat j = 0; j < i; j++)
                AfxVertexBufferBlueprintDiscard(&vbub[j]);
        }
    }

    if (!err)
    {
        if (AfxDrawContextBuildVertexBuffers(sim->dctx, cache.vtxDataCnt, vbub, cache.vbuf))
            AfxThrowError();

        for (afxNat j = 0; j < cache.vtxDataCnt; j++)
            AfxVertexBufferBlueprintDiscard(&vbub[j]);

        if (!err)
        {
            if (AfxDrawContextBuildIndexBuffers(sim->dctx, 1, &ibub, &cache.ibuf))
                AfxThrowError();

            if (!err)
            {
                if (AfxClassAcquireObjects(AfxSimulationGetMeshClass(sim), &cache, cnt, blueprint, (afxObject**)msh, AfxSpawnHint()))
                    AfxThrowError();

                AfxObjectRelease(&(cache.ibuf->buf.obj));
            }

            for (afxNat i = 0; i < cache.vtxDataCnt; i++)
                AfxObjectRelease(&(cache.vbuf[i]->buf.obj));
        }
    }

    AfxIndexBufferBlueprintEnd(&ibub, 0, NIL);

    return err;
}

_AFX afxResult AfxSimulationFindMeshes(afxSimulation sim, afxNat cnt, afxUri const uri[], afxMesh msh[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(sim, AFX_FCC_SIM);
    AfxAssert(cnt);
    AfxAssertType(uri, AFX_FCC_URI);
    AfxAssert(msh);
    afxResult rslt = 0;
    afxClass *cls = AfxSimulationGetMeshClass(sim);

    for (afxNat i = 0; i < cnt; i++)
        msh[i] = NIL;

    for (afxNat j = 0; j < AfxClassGetObjectCount(cls); j++)
    {
        afxMesh tmp;

        if (!AfxSimulationEnumerateMeshes(sim, 0, 1, &tmp)) break;
        else
        {
            AfxAssertObject(tmp, AFX_FCC_SHD);

            for (afxNat i = 0; i < cnt; i++)
            {
                if (AfxUriIsEquivalent(AfxMeshGetUri(tmp), &uri[i]))
                {
                    msh[i] = tmp;
                    rslt++;
                    break;
                }
            }
        }

        if (rslt == (afxResult)cnt)
            break;
    }
    return rslt;
}

_AFX afxError AfxSimulationUploadMeshes(afxSimulation sim, afxNat cnt, afxUri const uri[], afxMesh msh[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(sim, AFX_FCC_SIM);
    AfxAssert(cnt);
    AfxAssert(uri);
    AfxAssert(msh);
    afxResult rslt = 0;
#if 0
    afxMemory mem = AfxSimulationGetMemory(sim);
    AfxAssertObject(mem, AFX_FCC_MEM);
    afxDrawSystem dsys = AfxDrawContextGetDrawSystem(dctx);
    AfxAssertObject(dsys, AFX_FCC_DSYS);
    afxFileSystem fsys = AfxSimulationGetFileSystem(dsys);
    AfxAssertObject(fsys, AFX_FCC_FSYS);

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxAssertType(&uri[i], AFX_FCC_URI);
        AfxAssert(!AfxUriIsBlank(&uri[i]));

        AfxEcho("Uploading shader '%.*s'", AfxPushString(&uri[i].str));

        afxUri fext;
        AfxUriExcerptExtension(&fext, &uri[i], FALSE);

        if (AfxUriIsBlank(&fext)) AfxThrowError();
        else
        {
            afxUri fpath, query;
            AfxUriExcerptPath(&fpath, &uri[i]);
            AfxUriExcerptQuery(&query, &uri[i], TRUE);

            if (0 == AfxStringDifferLiteralCi(AfxUriGetStringConst(&fext), 0, 4, ".xml"))
            {
                afxXml xml;

                if (!(xml = AfxFileSystemLoadXml(fsys, &fpath))) AfxThrowError();
                else
                {
                    AfxAssertObject(xml, AFX_FCC_XML);

                    afxXmlNode const *node = AfxXmlGetRootNode(xml);
                    afxString const *name = AfxXmlNodeGetName(node);
                    AfxAssert(0 == AfxStringDiffer(name, &g_str_Qwadro));
                    afxString const *queryStr = AfxUriGetStringConst(&query);
                    afxBool hasQuery = !AfxStringIsEmpty(queryStr);
                    node = AfxXmlNodeFindChild(node, &g_str_Shader, hasQuery ? &g_str_name : NIL, hasQuery ? queryStr : NIL);

                    if (node)
                    {
                        afxShaderBlueprint blueprint;
                        AfxShaderBlueprintFormulate(&blueprint, dctx);

                        if (AfxParseXmlBackedShaderBlueprint(node, &blueprint)) AfxThrowError();
                        else
                        {
                            afxString512 tmp;
                            AfxString512(&tmp, AfxUriGetStringConst(&fpath));

                            if (!AfxUriIsBlank(&blueprint.uri.uri))
                            {
                                AfxStringAppendLiteral(&tmp.str, "?", 1);
                                AfxStringAppend(&tmp.str, 1, AfxUriGetStringConst(&blueprint.uri.uri));
                            }

                            afxUri tmpUri;
                            AfxUriReflectString(&tmpUri, &tmp.str);
                            AfxShaderBlueprintRename(&blueprint, &tmpUri);

                            if (AfxDrawContextBuildShaders(dctx, 1, &blueprint, &shd[i])) AfxThrowError();
                            else
                            {
                                AfxAssertObject(shd[i], AFX_FCC_SHD);
                                ++rslt;
                            }
                        }

                        AfxShaderBlueprintDiscard(&blueprint);
                    }
                    AfxObjectRelease(&xml->obj);
                }
            }
            else
            {
                AfxError("Extension (%.*s) not supported.", AfxPushString(AfxUriGetStringConst(&fext)));
                AfxThrowError();
            }
        }

        if (err)
        {
            for (afxNat j = 0; j < i; j++)
            {
                AfxObjectRelease(&shd[j]->obj);
                shd[j] = NIL;
            }
            break;
        }
    }
#endif
    AfxThrowError();
    return err;
}

_AFX afxError AfxSimulationAcquireMeshes(afxSimulation sim, afxNat cnt, afxUri const uri[], afxMesh msh[])
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("sim=%p,cnt=%u,uri=%p,msh=%p", sim, cnt, uri, msh);
    AfxAssertObject(sim, AFX_FCC_SIM);
    AfxAssert(cnt);
    AfxAssert(uri);
    AfxAssert(msh);
    afxResult rslt = 0;

    for (afxNat i = 0; i < cnt; i++)
    {
        afxUri name;
        AfxAssertType(&uri[i], AFX_FCC_URI);
        AfxAssert(!(AfxUriIsBlank(&uri[i])));
        AfxUriExcerptName(&uri[i], &name);
        AfxAssert(!(AfxUriIsBlank(&name)));

        if (1 == AfxSimulationFindMeshes(sim, 1, &name, &msh[i]))
        {
            AfxAssertObject(msh[i], AFX_FCC_MSH);
            AfxObjectReacquire(&(msh[i]->obj), NIL, NIL, NIL, NIL);
            rslt++;
        }
        else if (AfxSimulationUploadMeshes(sim, 1, &uri[i], &msh[i])) AfxThrowError();
        else
        {
            AfxAssertObject(msh[i], AFX_FCC_MSH);
            ++rslt;
        }

        if (err)
        {
            for (afxNat j = 0; j < i; j++)
            {
                AfxObjectRelease(&msh[j]->obj);
                msh[j] = NIL;
            }
            break;
        }
    }
    return err;
}

AFX afxClassSpecification const _AfxMshClassSpec;

afxClassSpecification const _AfxMshClassSpec =
{
    AFX_FCC_MSH,
    NIL,
    0,
    sizeof(AFX_OBJECT(afxMesh)),
    NIL,
    (void*)_AfxMshCtor,
    (void*)_AfxMshDtor,
    .event = _AfxMshEventHandler,
    .eventFilter = _AfxMshEventFilter,
    "afxMesh",
    NIL
};
