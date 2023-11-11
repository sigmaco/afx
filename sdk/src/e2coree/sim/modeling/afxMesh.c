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
#define _AFX_MESH_C
#define _AFX_VERTEX_DATA_C
#define _AFX_MESH_TOPOLOGY_C
#include "afx/sim/modeling/afxMesh.h"
#include "afx/sim/afxSimulation.h"
#include "afx/math/afxMatrix.h"

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
            AfxRotationQuatFromAxial(helpQuaternion, AfxSpawnV3d(0, 1, 0), 360.0f * texCoords[texCoordsIndex + 0]);
            AfxRotateM4d_AxialFromQuat((afxV4d*)helpMatrix, helpQuaternion);

            AfxTransformPivotV3d(&tangents[tangentIndex], helpVector, (afxV4d*)helpMatrix);
        }
    }

    afxMeshSurface msec = AfxAcquireMeshSection(msh, afxPrimTopology_TRI_LIST, numberIndices, 0, AfxAcquireMaterial(mdl, "parallelepiped"));
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
    afxMeshSurface msec = AfxAcquireMeshSection(msh, afxPrimTopology_TRI_LIST, numberIndices, 0, AfxAcquireMaterial(mdl, "parallelepiped"));
    AfxFillMeshSection(msec, numberIndices, xy_indices);

    afxM4d m4d;
    AfxResetM4d(m4d);
    AfxSetM4dScale(m4d, AfxSpawnV4d(width, 10.0, height, 1.0));

    for (afxNat32 i = 0; i < numberVertices; i++)
    {
        AfxPreMultiplyV4d(((afxV4d*)msh->vbuf.attrs[AFX_VERTEX_ATTR_XYZW].pool)[i], ((afxV4d*)msh->vbuf.attrs[AFX_VERTEX_ATTR_XYZW].pool)[i], m4d);
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
    afxMeshSurface msec = AfxAcquireMeshSection(msh, afxPrimTopology_TRI_LIST, numberIndices, 0, AfxAcquireMaterial(mdl, "cube"));
    AfxFillMeshSection(msec, numberIndices, cubeIndices);
    return msh;
}
#endif//0

#if 0
_AFX afxVertex const* AfxMeshGetVertices(afxMesh msh)
{
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    return AfxGetArrayData(&msh->vertices);
}

_AFX afxNat AfxMeshGetVertexCount(afxMesh msh)
{
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    return AfxCountArrayElements(&msh->vertices);
}

_AFX afxVertexWeight const* AfxMeshGetWeights(afxMesh msh)
{
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    return AfxGetArrayData(&msh->weights);
}

_AFX afxNat AfxMeshGetWeightCount(afxMesh msh)
{
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    return AfxCountArrayElements(&msh->weights);
}

_AFX afxTriangle const* AfxMeshGetTriangles(afxMesh msh)
{
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    return (afxTriangle const*)AfxIndexBuffer.Expose(msh->ibo);
}

_AFX afxNat AfxMeshGetTriangleCount(afxMesh msh)
{
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    return msh->ibo->idxCnt / 3;
}

_AFX afxResult _AfxMarkModelAsVolumeRecomputeNeeded(afxMesh msh, afxModel mdl, void *data)
{
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxAssertObjects(1, &mdl, afxFcc_MDL);
    (void)data;
    AfxObjectSetFlags(&(mdl->obj), AFX_MDL_FLAG_RECOMPUTE_VOLUME);
    return 1; // continue
}

_AFX afxAabb const* AfxMeshRecomputeVolume(afxMesh msh)
{
    AfxEntry("msh=%p", msh);
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    afxAabb* aabb = &(msh->volume);

    if (AfxObjectTestFlags(&msh->obj, AFX_MSH_FLAG_RECOMPUTE_VOLUME))
    {
        AfxResetAabb(aabb);

        AfxObjectClearFlags(&msh->obj, AFX_MSH_FLAG_RECOMPUTE_VOLUME);

        AfxEcho("Mesh %p dimensions: %f, %f, %f.", msh, aabb->sup[0] - aabb->inf[0], aabb->sup[1] - aabb->inf[1], aabb->sup[2] - aabb->inf[2]);
    }
    return aabb;
}

_AFX afxBool AfxMeshIsDeformable(afxMesh msh)
{
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    return FALSE;
}

_AFX afxAabb* AfxMeshGetVolume(afxMesh msh, afxAabb *aabb)
{
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxAssert(aabb);

    if (AfxObjectTestFlags(&msh->obj, AFX_MSH_FLAG_RECOMPUTE_VOLUME))
        AfxCopyAabb(AfxMeshRecomputeVolume(msh), aabb);
    else
        AfxCopyAabb(&msh->volume, aabb);

    return aabb;
}

#endif

_AFX void _AfxMshsRecomputeAabbCallback(void const *vtx, void *data)
{
    afxAabb *aabb = data;
    // WARNING: Expects that position vectors are afxV4d-based.
    AfxEncapsulatePoints(aabb, 1, vtx);
}

_AFX afxMeshVertebra* AfxGetMeshVertebra(afxMesh msh, afxNat artIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxAssertRange(msh->vertebraCnt, artIdx, 1);
    afxMeshVertebra *mshv = &msh->vertebras[artIdx];
    AfxAssertType(mshv, afxFcc_MSHV);
    return mshv;
}

_AFX afxNat AfxCountMeshVertebras(afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    return msh->vertebraCnt;
}

_AFX afxBool AfxMeshIsRigid(afxMesh msh)
{
    // The vertices in a Qwadro mesh are either deforming or rigid. 
    // Deforming meshes are ones that are bound to multiple bones, whereas rigid meshes are ones that are bound to a single bone (and thus move "rigidly" with that bone). 

    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    return (1 >= msh->vertebraCnt);
}

_AFX void AfxSetMeshMaterial(afxMesh msh, afxNat mtlIdx, afxMaterial mtl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxAssertRange(msh->mtlSlotCnt, mtlIdx, 1);

    if (mtl)
    {
        AfxAssertObjects(1, &mtl, afxFcc_MTL);
        AfxReacquireObjects(1, (void*[]) { mtl });
    }

    afxMaterial mtl2 = msh->mtlSlots[mtlIdx].mtl;
    msh->mtlSlots[mtlIdx].mtl = mtl;
    
    if (mtl2)
    {
        AfxAssertObjects(1, &mtl2, afxFcc_MTL);
        AfxReleaseObjects(1, (void*[]) { mtl2 });
    }
}

_AFX afxBool AfxFindMeshMaterial(afxMesh msh, afxUri const* id, afxMaterial* mtl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxAssertType(id, afxFcc_URI);

    for (afxNat i = 0; i < msh->mtlSlotCnt; i++)
    {
        afxMaterial mtl2 = msh->mtlSlots[i].mtl;

        if (mtl2)
        {
            AfxAssertObjects(1, &mtl2, afxFcc_MTL);

            if (0 == AfxCompareUri(AfxGetMaterialUri(mtl2), id))
            {
                if (mtl)
                    *mtl = mtl2;

                return TRUE;
            }
        }
    }
    return FALSE;
}

_AFX afxBool AfxGetMeshMaterial(afxMesh msh, afxNat mtlIdx, afxMaterial* mtl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxAssertRange(msh->mtlSlotCnt, mtlIdx, 1);
    afxMaterial mtl2 = msh->mtlSlots[mtlIdx].mtl;
    
    if (mtl)
    {
        AfxTryAssertObjects(1, &mtl2, afxFcc_MTL);
        *mtl = mtl2;
    }

    return !!mtl2;
}

_AFX afxNat AfxCountMeshMaterials(afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    return msh->mtlSlotCnt;
}

_AFX afxBool AfxGetMeshTopology(afxMesh msh, afxMeshTopology* msht)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    afxMeshTopology msht2 = msh->topology;

    if (msht)
    {
        AfxAssertObjects(1, &msht2, afxFcc_MSHT);
        *msht = msht2;
    }

    return !!msht2;
}

_AFX void AfxRelinkMeshTopology(afxMesh msh, afxMeshTopology msht)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);

    if (msht)
    {
        AfxAssertObjects(1, &msht, afxFcc_MSHT);
        AfxReacquireObjects(1, (void*[]) { msht });
    }

    afxMeshTopology msht2 = msh->topology;
    msh->topology = msht;

    if (msht2)
    {
        AfxAssertObjects(1, &msht2, afxFcc_MSHT);
        AfxReleaseObjects(1, (void*[]) { msht2 });
    }    
}

_AFX afxMeshMorph* AfxGetMeshMorph(afxMesh msh, afxNat morphIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxAssertRange(msh->morphCnt, morphIdx, 1);
    afxMeshMorph *mshm = &msh->morphs[morphIdx];
    AfxAssertType(mshm, afxFcc_MSHM);
    return mshm;
}

_AFX afxNat AfxCountMeshMorphes(afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    return msh->morphCnt;
}

_AFX afxNat AfxCountMeshVertices(afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    return msh->vtxCnt;
}

_AFX afxVertexData AfxGetMeshVertices(afxMesh msh, afxNat* baseVtxIdx, afxNat* vtxCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    afxVertexData vtd = msh->vtd;
    AfxTryAssertObjects(1, &vtd, afxFcc_VTD);

    if (baseVtxIdx)
        *baseVtxIdx = msh->baseVtxIdx;

    if (vtxCnt)
        *vtxCnt = msh->vtxCnt;

    return vtd;
}

_AFX afxUri const* AfxGetMeshId(afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    return &msh->id;
}

_AFX afxError _AfxMshDtor(afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxEntry("msh=%p", msh);

    afxSimulation sim = AfxGetObjectProvider(msh);
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxContext mem = AfxSimulationGetMemory(sim);

    for (afxNat i = 0; i < msh->vertebraCnt; i++)
    {
        afxMeshVertebra *msha = &msh->vertebras[i];

        //if (msha->boneName)
            //AfxDeallocateString(&msha->id);
    }

    if (msh->vertebras)
        AfxDeallocate(mem, msh->vertebras);

    for (afxNat i = 0; i < msh->mtlSlotCnt; i++)
        AfxSetMeshMaterial(msh, i, NIL);

    if (msh->mtlSlots)
        AfxDeallocate(mem, msh->mtlSlots);

    if (msh->topology)
    {
        AfxAssertObjects(1, &msh->topology, afxFcc_MSHT);
        AfxRelinkMeshTopology(msh, NIL);
    }

    if (msh->morphs)
        AfxDeallocate(mem, msh->morphs);

    if (msh->vtd)
        AfxReleaseObjects(1, (void*[]) { msh->vtd });

    //if (msh->uri)
    AfxUriDeallocate(&msh->id);

    return err;
}

_AFX afxError _AfxMshCtor(afxMesh msh, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxEntry("msh=%p", msh);

    afxSimulation sim = cookie->udd[0];
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxMeshBuilder *mshb = cookie->udd[1];
    AfxAssert(mshb);
    void* data = cookie->udd[2] ? ((void **)(cookie->udd[2]))[cookie->no] : NIL;
    AfxAssert(data);

    afxContext mem = AfxSimulationGetMemory(sim);

    AfxBuildVertexDatas(sim, mshb, 1, &data, &msh->vtd);
    msh->baseVtxIdx = 0;
    msh->vtxCnt = msh->vtd->vtxCnt;

    AfxBuildMeshTopologies(sim, mshb, 1, &data, &msh->topology);

    msh->mtlSlots = NIL;
    msh->mtlSlotCnt = 0;
    afxNat mtlCnt, artCnt;
    AfxAssert(mshb->GetBindingInfo);
    mshb->GetBindingInfo(data, &mtlCnt, &artCnt);

    if (mtlCnt && !(msh->mtlSlots = AfxAllocate(mem, mtlCnt * sizeof(msh->mtlSlots[0]), 0, AfxSpawnHint()))) AfxThrowError();
    else
    {
        for (afxNat i = 0; i < mtlCnt; i++)
        {
            afxMaterialSlot *slot = &msh->mtlSlots[i];
            AfxAssert(slot);

            afxMaterial mtl = (slot->mtl = mshb->GetMaterial(data, i));
            
            if (mtl)
            {
                AfxAssertObjects(1, &mtl, afxFcc_MTL);
                AfxReacquireObjects(1, (void*[]) { mtl });
            }
            ++msh->mtlSlotCnt;
        }
        AfxAssert(mtlCnt == msh->mtlSlotCnt);
    }

    if (!err)
    {
        msh->vertebras = NIL;
        msh->vertebraCnt = 0;
        
        if (artCnt && !(msh->vertebras = AfxAllocate(mem, artCnt * sizeof(msh->vertebras[0]), 0, AfxSpawnHint()))) AfxThrowError();
        else
        {
            for (afxNat i = 0; i < artCnt; i++)
            {
                //afxMeshBlueprintArticulation const *artb = AfxGetArrayUnit(&mshb->joints, i);
                afxMeshVertebra *mshv = &msh->vertebras[i];
                AfxAssignTypeFcc(mshv, afxFcc_MSHV);

                afxString str;
                mshv->triCnt = mshb->GetVertebraInfo(data, i, &str);
                mshv->triIdx = AfxAllocate(mem, mshv->triCnt * sizeof(mshv->triIdx[0]), NIL, AfxSpawnHint());
                
                AfxString16(&mshv->id);
                AfxCopyString(&mshv->id.str, &str);
                //mshv->vtxCnt = artb->vtxIdxCnt;
                //mshv->vtxMap = (void*)artb->tris; // ???

                //AfxAssert(artb->tris == NIL);

                mshb->GetVertebraData(data, &mshv->id.str, 0, 3 * mshv->triCnt, mshv->triIdx);

                AfxResetAabb(&mshv->aabb);
                //AfxAssert(AfxGetVertexAttributeSize(vbuf, 0) == sizeof(afxV4d)); // actually, _AfxMshsRecomputeAabbCallback expects afxV4d-based vectors and position at vertex arrange 0. TODO: someone need to do refactoring here.
                //AfxIndexBufferForEachVertex(ibuf, msh->topology.idxRgn, 0, mshv->triCnt, vbuf, 0, msh->baseVtx, _AfxMshsRecomputeAabbCallback, &mshv->aabb);

                msh->vertebraCnt++;
            }
            AfxAssert(artCnt == msh->vertebraCnt);

        }

        msh->extData = NIL;

        if (err)
        {
            for (afxNat i = msh->vertebraCnt; i-- > 0;)
            {
                afxMeshVertebra *mshv = &msh->vertebras[i];
                AfxAssertType(mshv, afxFcc_MSHV);
                
                if (mshv->triIdx)
                    AfxDeallocate(mem, mshv->triIdx);
            }

            if (msh->vertebras) // what if tris is set?
                AfxDeallocate(mem, msh->vertebras);
        }
    }

    if (err)
    {
        for (afxNat i = msh->mtlSlotCnt; i-- > 0;)
        {
            afxMaterialSlot *slot = &msh->mtlSlots[i];
            
            if (slot->mtl)
            {
                AfxAssertObjects(1, &slot->mtl, afxFcc_MTL);
                AfxReleaseObjects(1, (void*[]) { slot->mtl });
            }
        }
        
        if (msh->mtlSlots)
            AfxDeallocate(mem, msh->mtlSlots);
    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxBuildMeshes(afxSimulation sim, afxMeshBuilder const* mshb, afxNat cnt, void *data[], afxMesh msh[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AfxAssert(cnt);
    AfxAssert(mshb);
    AfxAssert(msh);

    if (AfxAcquireObjects(AfxGetMeshClass(sim), cnt, (afxHandle*)msh, (void*[]) { sim, (void*)mshb, data }))
        AfxThrowError();

    return err;
}

_AFX void AfxTransformMeshes(afxReal const lt[3][3], afxReal const ilt[3][3], afxReal const at[3], afxReal affineTol, afxReal linearTol, afxFlags flags, afxNat cnt, afxMesh msh[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(at);
    AfxAssert(lt);
    AfxAssert(ilt);
    AfxAssert(cnt);
    AfxAssert(msh);
    AfxThrowError();
    (void)affineTol;
    (void)linearTol;

    for (afxNat i = 0; i < cnt; i++)
    {
        afxMesh msh2 = msh[i];

        if (msh2)
        {
            AfxAssertObjects(1, &msh2, afxFcc_MSH);

            afxNat baseVtxIdx = 0, vtxCnt = 0;
            afxVertexData vtd = AfxGetMeshVertices(msh2, &baseVtxIdx, &vtxCnt);
            AfxTryAssertObjects(1, &vtd, afxFcc_VTD);

            AfxTransformVertexDatas(lt, ilt, at, flags & 1, 1, &vtd); // sinalizado para renormalizar normals

            afxNat morphCnt = AfxCountMeshMorphes(msh2);

            for (afxNat j = 0; j < morphCnt; j++)
            {
                afxMeshMorph* mshm = AfxGetMeshMorph(msh2, j);
                vtd = mshm->vtd;
                AfxTryAssertObjects(1, &vtd, afxFcc_VTD);
                AfxAssert(!mshm->delta || AfxGetVertexAttributeFlags(vtd, 0) & afxVertexFlag_DELTA);

                if (mshm->delta)
                {
                    AfxTransformVertexDatas(lt, ilt, at, FALSE, 1, &vtd);
                }
                else
                {
                    AfxTransformVertexDatas(lt, ilt, at, flags & 1, 1, &vtd);
                }
            }

            afxMeshTopology msht;

            if ((flags & 2) && (AfxDetM3d(lt) < 0.0) && AfxGetMeshTopology(msh2, &msht)) // se sinalizado para reordenar índices de faces
                AfxInvertMeshWinding(msht);

            afxNat artCnt = AfxCountMeshVertebras(msh2);

            for (afxNat j = 0; j < artCnt; j++)
            {
                afxMeshVertebra* mshv = AfxGetMeshVertebra(msh2, j);
                AfxTransformAabbs(lt, at, 1, &mshv->aabb, &mshv->aabb);
            }
        }
    }
}

_AFX afxClassConfig _AfxMshClsConfig =
{
    .fcc = afxFcc_MSH,
    .name = "Mesh",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxMesh)),
    .ctx = NIL,
    .ctor = (void*)_AfxMshCtor,
    .dtor = (void*)_AfxMshDtor
};
