/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *             Q W A D R O   R E N D E R I N G   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#define _ARX_MESH_C
//#define _ARX_GEOMETRY_C
#define _ARX_VERTEX_BUILDER_C
#define _ARX_MESH_TOPOLOGY_C
#include "../ddi/arxImpl_Input.h"
#include "qwadro/inc/render/arxTriangulation.h"

AFX_DEFINE_STRUCT(arxMeshFactory)
{
    afxReal tanMergMinCos;
    afxReal tanTol;
    afxReal binTol;
    afxReal nrmTol;
    afxReal*chanTol; // one per channel
    afxUnit triCnt;
    afxUnit triVtxCnt; // [triCnt * 3]
    afxV3d* triVtxPos; // [triVtxCnt * 3]
    afxUnit mtlCnt;
    afxUnit* mtlFromTriIdx; // [triCnt]
    afxV3d* triVtxNrm; // [triVtxCnt * 3]
    afxV3d* triVtxTan; // [triVtxCnt * 3]
    afxV3d* triVtxBit; // [triVtxCnt * 3]
    afxV2d* triVtxUv; // [triVtxCnt * 3]
    afxV3d* triVtxCol; // [triVtxCnt * 3]
    afxUnit maxBiasPerTriCnt;
    afxUnit* triVtxBias; // [triCnt * maxBiasPerTriCnt]
    afxUnit biasCnt;
};

_ARX afxError ArxCleanUpMeshFactory(arxMeshFactory* mfac)
{
    afxError err = NIL;
    AFX_ASSERT(mfac);
    return err;
}

_ARX afxError ArxSetUpMeshFactory(arxMeshFactory* mfac, afxUnit triCnt, afxUnit mtlCnt, afxUnit biasCnt)
{
    afxError err = NIL;
    AFX_ASSERT(mfac);
    *mfac = (arxMeshFactory) { 0 };
    return err;
}

_ARX afxError ArxAddTriangles(arxMeshFactory* mfac, afxUnit mtlIdx, afxUnit triCnt, afxV3d vertices[][3])
{
    afxError err = NIL;
    AFX_ASSERT(vertices);
    AFX_ASSERT(mfac);

    for (afxUnit i = 0; i < triCnt; i++)
        AfxV3dCopy(mfac->triVtxPos[i], vertices[i]);

    for (afxUnit i = 0; i < triCnt; i++)
    {
        mfac->mtlFromTriIdx[i] = mtlIdx;
    }
    return err;
}

_ARX afxError ArxAddMaterial(arxMeshFactory* mfac, afxString const* name, afxUnit* mtlIdx)
{
    afxError err = NIL;
    AFX_ASSERT(mfac);
    return err;
}

_ARX afxError ArxRebuildTriangleNormals(arxMeshFactory* mfac, afxUnit baseTriIdx, afxUnit cnt)
{
    afxError err = NIL;
    AFX_ASSERT(mfac);
    AFX_ASSERT_RANGE(mfac->triCnt, baseTriIdx, cnt);

    baseTriIdx = AFX_MIN(baseTriIdx, mfac->triCnt - 1);
    cnt = AFX_MIN(cnt, mfac->triCnt - baseTriIdx);
    
    afxUnit triCnt = cnt ? cnt : mfac->triCnt - baseTriIdx;
    afxV3d* pos = mfac->triVtxPos;
    afxV3d* nrm = mfac->triVtxNrm;

    for (afxUnit j = 0; j < triCnt; j++)
    {
        afxUnit const ia = baseTriIdx + j * ARX_INDICES_PER_TRI + 0;
        afxUnit const ib = baseTriIdx + j * ARX_INDICES_PER_TRI + 1;
        afxUnit const ic = baseTriIdx + j * ARX_INDICES_PER_TRI + 2;

        afxV4d e1, e2, no;
        AfxV4dSub(e1, pos[ia], pos[ib]);
        AfxV4dSub(e2, pos[ic], pos[ib]);
        AfxV3dCross(no, e1, e2);

        AfxV3dAdd(nrm[ia], nrm[ia], no);
        AfxV3dAdd(nrm[ib], nrm[ib], no);
        AfxV3dAdd(nrm[ic], nrm[ic], no);
    }
    return err;
}

_ARX afxError ArxRebuildTriangleTangents(arxMeshFactory* mfac, afxUnit baseTriIdx, afxUnit cnt)
{
    afxError err = NIL;
    AFX_ASSERT(mfac);
    AFX_ASSERT_RANGE(mfac->triCnt, baseTriIdx, cnt);

    baseTriIdx = AFX_MIN(baseTriIdx, mfac->triCnt - 1);
    cnt = AFX_MIN(cnt, mfac->triCnt - baseTriIdx);

    afxUnit triCnt = cnt ? cnt : mfac->triCnt - baseTriIdx;
    afxV3d* pos = mfac->triVtxPos;
    afxV3d* tan = mfac->triVtxTan;
    afxV3d* bit = mfac->triVtxBit;
    afxV2d* uv = mfac->triVtxUv;

    for (afxUnit i = 0; i < triCnt; i++)
    {
        afxUnit const ia = baseTriIdx + i * ARX_INDICES_PER_TRI + 0;
        afxUnit const ib = baseTriIdx + i * ARX_INDICES_PER_TRI + 1;
        afxUnit const ic = baseTriIdx + i * ARX_INDICES_PER_TRI + 2;

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
#if 0
            if (!perTriOut)
            {
                // Because a triangle is always a flat shape, we only need to calculate a single tangent/bitangent pair per triangle as they will be the same for each of the triangle's vertices.
                AfxV3dCopy(tan[ib], tan[ia]);
                AfxV3dCopy(tan[ic], tan[ia]);
            }
#endif
        }

        if (bit)
        {
            bit[ia][0] = f * (-deltaUv2[0] * ea[0] + deltaUv1[0] * eb[0]);
            bit[ia][1] = f * (-deltaUv2[0] * ea[1] + deltaUv1[0] * eb[1]);
            bit[ia][2] = f * (-deltaUv2[0] * ea[2] + deltaUv1[0] * eb[2]);
#if 0
            if (!perTriOut)
            {
                // Because a triangle is always a flat shape, we only need to calculate a single tangent/bitangent pair per triangle as they will be the same for each of the triangle's vertices.
                AfxV3dCopy(bit[ib], bit[ia]);
                AfxV3dCopy(bit[ic], bit[ia]);
            }
#endif
        }
    }
    return err;
}

_ARX afxError ArxRebuildTriangleSkins(arxMeshFactory* mfac, afxUnit baseTriIdx, afxUnit cnt)
{
    afxError err = NIL;
    AFX_ASSERT(mfac);
    AFX_ASSERT_RANGE(mfac->triCnt, baseTriIdx, cnt);

    baseTriIdx = AFX_MIN(baseTriIdx, mfac->triCnt - 1);
    cnt = AFX_MIN(cnt, mfac->triCnt - baseTriIdx);

    afxUnit triCnt = cnt ? cnt : mfac->triCnt - baseTriIdx;
    afxV3d* pos = mfac->triVtxPos;
    afxV2d* uv = mfac->triVtxUv;

    // Loop through each triangle and calculate the UVs
    for (afxUnit j = 0; j < triCnt; j++)
    {
        afxUnit const ia = baseTriIdx + j * ARX_INDICES_PER_TRI + 0;
        afxUnit const ib = baseTriIdx + j * ARX_INDICES_PER_TRI + 1;
        afxUnit const ic = baseTriIdx + j * ARX_INDICES_PER_TRI + 2;

        // Project the 3D vertices to 2D (XY plane)
        afxReal u1 = pos[ia][0], v1y = pos[ia][1];
        afxReal u2 = pos[ib][0], v2y = pos[ib][1];
        afxReal u3 = pos[ic][0], v3y = pos[ic][1];

        // Find the min and max values for the u and v coordinates
        afxReal min_u = fminf(fminf(u1, u2), u3);
        afxReal max_u = fmaxf(fmaxf(u1, u2), u3);
        afxReal min_v = fminf(fminf(v1y, v2y), v3y);
        afxReal max_v = fmaxf(fmaxf(v1y, v2y), v3y);

        // Normalize UV coordinates to the range [0, 1]
        afxReal range_u = max_u - min_u;
        afxReal range_v = max_v - min_v;

        // Store the UV coordinates for this triangle
        uv[ia][0] = (u1 - min_u) / range_u;
        uv[ia][1] = (v1y - min_v) / range_v;

        uv[ib][0] = (u2 - min_u) / range_u;
        uv[ib][1] = (v2y - min_v) / range_v;

        uv[ic][0] = (u3 - min_u) / range_u;
        uv[ic][1] = (v3y - min_v) / range_v;
    }
    return err;
}


////////////////////////////////////////////////////////////////////////////////

_ARX arxMesh ArxBuildParallelepipedMesh(arxGeome morp, afxV3d whd, afxReal slantX, afxReal slantY)
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_MORP, 1, &morp);
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

    arxMeshBlueprint mshb = { 0 };
    mshb.vtxCnt = numVertices;
    mshb.attrCnt = 3;
    mshb.attrs = (afxString[]) { AFX_STRING("pos"), AFX_STRING("nrm"), AFX_STRING("uv") };
    mshb.biasCnt = 1;
    mshb.triCnt = numIndices / 3;

    arxMesh msh;
    ArxBuildMeshes(morp, 1, &mshb, &msh);

    ArxFormatVertexAttribute(msh, 0, avxFormat_RGB32f, arxVertexFlag_POSITIONAL | arxVertexFlag_SPATIAL | arxVertexFlag_ATV, &AFX_STRING("pos"));
    ArxFormatVertexAttribute(msh, 1, avxFormat_RGB32f, arxVertexFlag_NORMALIZED | arxVertexFlag_LTM, &AFX_STRING("nrm"));
    ArxFormatVertexAttribute(msh, 2, avxFormat_RG32f, arxVertexFlag_NORMALIZED | arxVertexFlag_LTM, &AFX_STRING("uv"));

    ArxUpdateVertexData(msh, 0, 0, 0, mshb.vtxCnt, vertData, sizeof(vertData[0]));

    afxV3d* nrm = ArxAccessVertexData(msh, 1, 0, 0);
    afxV2d* uv = ArxAccessVertexData(msh, 2, 0, 0);

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

    ArxUpdateMeshIndices(msh, 0, mshb.triCnt, indicesData, sizeof(indicesData[0]));

    return msh;
}

_ARX arxMesh ArxBuildDomeMesh2(arxGeome morp, afxReal radius, afxUnit stacks, afxUnit slices, afxBool inv)
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_MORP, 1, &morp);

    if (!radius)
        radius = 1.f;

    if (!stacks)
        stacks = 10;

    if (!slices)
        slices = 20;

    afxUnit numVertices = (stacks + 1) * (slices + 1);

    afxUnit numIndices = stacks * slices * 2 * 3; // 2 triangles per quad, 3 indices per triangle
    //afxUnit numIndices = stacks * slices * 6; // 6 indices per quad (2 triangles, 3 indices per triangle)

    arxMeshBlueprint mshb = { 0 };
    mshb.vtxCnt = numVertices;
    mshb.attrCnt = 3;
    mshb.attrs = (afxString[]) { AFX_STRING("pos"), AFX_STRING("nrm"), AFX_STRING("uv") };
    mshb.biasCnt = 1;
    mshb.triCnt = numIndices / 3;

    arxMesh msh;
    ArxBuildMeshes(morp, 1, &mshb, &msh);

    ArxFormatVertexAttribute(msh, 0, avxFormat_RGB32f, arxVertexFlag_POSITIONAL | arxVertexFlag_SPATIAL | arxVertexFlag_ATV, &AFX_STRING("pos"));
    ArxFormatVertexAttribute(msh, 1, avxFormat_RGB32f, arxVertexFlag_NORMALIZED | arxVertexFlag_LTM, &AFX_STRING("nrm"));
    ArxFormatVertexAttribute(msh, 2, avxFormat_RG32f, arxVertexFlag_NORMALIZED | arxVertexFlag_LTM, &AFX_STRING("uv"));

    afxV3d* pos = ArxAccessVertexData(msh, 0, 0, 0);
    afxV3d* nrm = ArxAccessVertexData(msh, 1, 0, 0);
    afxV2d* uv = ArxAccessVertexData(msh, 2, 0, 0);
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

#if 0 // we couldn't use trip strip at the time this code was written.
    afxBool strip = FALSE;

    if (strip)
    {
        arxMeshTopologyBlueprint mshtb = { 0 };
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
        afxUnit* indices = ArxGetMeshIndices(msh, 0);

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
        ArxInvertMeshTopology(msh);
        ArxInvertVertexData(msh, 1, 0, 0, numVertices);
    }

    return msh;
}

_ARX arxMesh ArxBuildCapsuleMesh(arxGeome morp, afxReal radius, afxReal height, afxUnit stacks, afxUnit slices, afxUnit cylinderSlices, afxBool inv)
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_MORP, 1, &morp);

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

    arxMeshBlueprint mshb = { 0 };
    mshb.vtxCnt = numVertices;
    mshb.attrCnt = 3;
    mshb.attrs = (afxString[]) { AFX_STRING("pos"), AFX_STRING("nrm"), AFX_STRING("uv") };
    mshb.biasCnt = 1;
    mshb.triCnt = numIndices / 3;

    arxMesh msh;
    ArxBuildMeshes(morp, 1, &mshb, &msh);

    ArxFormatVertexAttribute(msh, 0, avxFormat_RGB32f, arxVertexFlag_POSITIONAL | arxVertexFlag_SPATIAL | arxVertexFlag_ATV, &AFX_STRING("pos"));
    ArxFormatVertexAttribute(msh, 1, avxFormat_RGB32f, arxVertexFlag_NORMALIZED | arxVertexFlag_LTM, &AFX_STRING("nrm"));
    ArxFormatVertexAttribute(msh, 2, avxFormat_RG32f, arxVertexFlag_NORMALIZED | arxVertexFlag_LTM, &AFX_STRING("uv"));

    afxV3d* pos = ArxAccessVertexData(msh, 0, 0, 0);
    afxV3d* nrm = ArxAccessVertexData(msh, 1, 0, 0);
    afxV2d* uv = ArxAccessVertexData(msh, 2, 0, 0);
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

    {
        afxUnit* indices = ArxGetMeshIndices(msh, 0);

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
        ArxInvertMeshTopology(msh);
        ArxInvertVertexData(msh, 1, 0, 0, numVertices);
    }

    return msh;
}

_ARX arxMesh ArxBuildPlaneMesh(arxGeome morp, afxUnit gridSizeX, afxUnit gridSizeY, afxReal width, afxReal height)
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_MORP, 1, &morp);

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

    arxMeshBlueprint mshb = { 0 };
    mshb.vtxCnt = numVertices;
    mshb.attrCnt = 3;
    mshb.attrs = (afxString[]) { AFX_STRING("pos"), AFX_STRING("nrm"), AFX_STRING("uv") };
    mshb.biasCnt = 1;
    mshb.triCnt = numIndices / 3;

    arxMesh msh;
    ArxBuildMeshes(morp, 1, &mshb, &msh);

    ArxFormatVertexAttribute(msh, 0, avxFormat_RGB32f, arxVertexFlag_POSITIONAL | arxVertexFlag_SPATIAL | arxVertexFlag_ATV, &AFX_STRING("pos"));
    ArxFormatVertexAttribute(msh, 1, avxFormat_RGB32f, arxVertexFlag_NORMALIZED | arxVertexFlag_LTM, &AFX_STRING("nrm"));
    ArxFormatVertexAttribute(msh, 2, avxFormat_RG32f, arxVertexFlag_NORMALIZED | arxVertexFlag_LTM, &AFX_STRING("uv"));

    afxV3d* pos = ArxAccessVertexData(msh, 0, 0, 0);
    afxV3d* nrm = ArxAccessVertexData(msh, 1, 0, 0);
    afxV2d* uv = ArxAccessVertexData(msh, 2, 0, 0);

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

    {
        afxUnit* indices = ArxGetMeshIndices(msh, 0);

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
    The ArxBuildSphereMesh() function uses a right-handed coordinate system to create a mesh containing a sphere.

    @param radius Radius of the sphere. This value should be greater than or equal to 0.0f.
    @param slices Number of slices about the main axis.
    @param stacks Number of stacks along the main axis.
    @param mesh Address of a pointer to the output shape, an arxMesh instance.
*/

_ARX arxMesh ArxBuildSphereMesh(arxGeome morp, afxReal radius, afxUnit stacks, afxUnit slices, afxBool inv)
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_MORP, 1, &morp);

    if (!radius)
        radius = 1.f;

    if (!stacks)
        stacks = 20;

    if (!slices)
        slices = 20;

    afxUnit numVertices = (stacks + 1) * (slices + 1);
    afxUnit numIndices = stacks * slices * 2 * 3; // 2 triangles per quad, 3 indices per triangle
    //afxUnit numIndices = stacks * slices * 6; // 6 indices per quad (2 triangles, 3 indices per triangle)

    arxMeshBlueprint mshb = { 0 };
    mshb.vtxCnt = numVertices;
    mshb.attrCnt = 3;
    mshb.attrs = (afxString[]) { AFX_STRING("pos"), AFX_STRING("nrm"), AFX_STRING("uv") };
    mshb.biasCnt = 1;
    mshb.triCnt = numIndices / 3;

    arxMesh msh;
    ArxBuildMeshes(morp, 1, &mshb, &msh);

    ArxFormatVertexAttribute(msh, 0, avxFormat_RGB32f, arxVertexFlag_POSITIONAL | arxVertexFlag_SPATIAL | arxVertexFlag_ATV, &AFX_STRING("pos"));
    ArxFormatVertexAttribute(msh, 1, avxFormat_RGB32f, arxVertexFlag_NORMALIZED | arxVertexFlag_LTM, &AFX_STRING("nrm"));
    ArxFormatVertexAttribute(msh, 2, avxFormat_RG32f, arxVertexFlag_NORMALIZED | arxVertexFlag_LTM, &AFX_STRING("uv"));

    afxV3d* pos = ArxAccessVertexData(msh, 0, 0, 0);
    afxV3d* nrm = ArxAccessVertexData(msh, 1, 0, 0);
    afxV2d* uv = ArxAccessVertexData(msh, 2, 0, 0);
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

    // we couldn't use trip strip at the time this code was written.
    afxBool strip = FALSE;

    if (strip)
    {
        afxUnit* indices = ArxGetMeshIndices(msh, 0);

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
        afxUnit* indices = ArxGetMeshIndices(msh, 0);

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
        ArxInvertMeshTopology(msh);
        ArxInvertVertexData(msh, 1, 0, 0, numVertices);
    }

    return msh;
}

/**
    The ArxBuildTorusMesh() function uses a right-handed coordinate system to create a mesh containing a torus.

    @param innerRadius Inner-radius of the torus. Value should be greater than or equal to 0.0f.
    @param outerRadius Outer-radius of the torus. Value should be greater than or equal to 0.0f.
    @param sides Number of sides in a cross-section. Value must be greater than or equal to 3.
    @param rings Number of rings making up the torus. Value must be greater than or equal to 3.
    @param mesh Address of a pointer to the output shape, an arxMesh instance.
*/

_ARX afxError ArxBuildTorusMesh(arxGeome morp, afxReal innerRadius, afxReal outerRadius, afxUnit sides, afxUnit rings, arxMesh* mesh)
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_MORP, 1, &morp);
    AFX_ASSERT(mesh);


    return err;
}

/**
    The ArxBuildCylinderMesh() function uses a right-handed coordinate system to create a mesh containing a cylinder.

    @param radius1 Radius at the negative Z end. Value should be greater than or equal to 0.0f.
    @param radius2 Radius at the positive Z end. Value should be greater than or equal to 0.0f.
    @param len Length of the cylinder along the z-axis.
    @param slices Number of slices about the main axis.
    @param stacks Number of stacks along the main axis.
    @param mesh Address of a pointer to the output shape, an arxMesh instance.
*/

_ARX afxError ArxBuildCylinderMesh(arxGeome morp, afxReal radius1, afxReal radius2, afxReal len, afxUnit slices, afxUnit stacks, arxMesh* mesh)
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_MORP, 1, &morp);
    AFX_ASSERT(mesh);
    AFX_ASSERT(radius1 >= 0.f);
    AFX_ASSERT(radius1 >= 0.f);
    return err;
}

/**
    The ArxBuildBoxMesh() function uses a right-handed coordinate system to create a mesh containing an axis-aligned box.

    @param whd Width, height and depth of the box, along the X, Y and Z-axis respectively.
    @param secCnt Maximum number of sections to segment the mesh.
    @param mesh Address of a pointer to the output shape, an arxMesh instance.
*/

_ARX afxError ArxBuildBoxMesh(arxGeome morp, afxV3d const whd, afxUnit secCnt, arxMesh* mesh)
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_MORP, 1, &morp);
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

    afxUnit baseTriIdx = 0;
    arxMeshSection sections[12];
    afxUnit secDiv = 12 / secCnt;

    for (afxUnit i = 0; i < secCnt; i++)
        sections[i].baseTriIdx = baseTriIdx, baseTriIdx += (sections[i].triCnt = secDiv);

    arxMeshBlueprint mshb = { 0 };
    mshb.vtxCnt = ARRAY_SIZE(cubeVertices);
    mshb.attrCnt = 4;
    mshb.attrs = (afxString[]) { AFX_STRING("pos"), AFX_STRING("nrm"), AFX_STRING("tan"), AFX_STRING("uv") };
    mshb.biasCnt = 1;
    mshb.triCnt = ARRAY_SIZE(tris);
    mshb.secCnt = secCnt;
    mshb.sections = sections;

    arxMesh msh;
    ArxBuildMeshes(morp, 1, &mshb, &msh);

    ArxFormatVertexAttribute(msh, 0, avxFormat_RGB32f, arxVertexFlag_POSITIONAL | arxVertexFlag_SPATIAL | arxVertexFlag_ATV, &AFX_STRING("pos"));
    ArxFormatVertexAttribute(msh, 1, avxFormat_RGB32f, arxVertexFlag_NORMALIZED | arxVertexFlag_LTM, &AFX_STRING("nrm"));
    ArxFormatVertexAttribute(msh, 2, avxFormat_RGB32f, arxVertexFlag_NORMALIZED | arxVertexFlag_LTM, &AFX_STRING("tan"));
    ArxFormatVertexAttribute(msh, 3, avxFormat_RG32f, arxVertexFlag_NORMALIZED, &AFX_STRING("uv"));

    ArxUpdateVertexData(msh, 0, 0, 0, mshb.vtxCnt, cubeVertices, sizeof(cubeVertices[0]));
    ArxUpdateVertexData(msh, 1, 0, 0, mshb.vtxCnt, cubeNormals, sizeof(cubeNormals[0]));
    ArxUpdateVertexData(msh, 2, 0, 0, mshb.vtxCnt, cubeTangents, sizeof(cubeTangents[0]));
    ArxUpdateVertexData(msh, 3, 0, 0, mshb.vtxCnt, cubeTexCoords, sizeof(cubeTexCoords[0]));

    ArxUpdateMeshIndices(msh, 0, mshb.triCnt, tris, sizeof(tris[0][0]));

    afxV3d* pos = ArxAccessVertexData(msh, 0, 0, 0);

    afxV3d whdSanitized;
    AfxV3dMax(whdSanitized, AFX_V3D_ONE, whd);

    for (afxUnit i = 0; i < mshb.vtxCnt; i++)
        AfxV3dMultiply(pos[i], pos[i], whdSanitized);


    *mesh = msh;
    return err;
}

_ARX afxError ArxBuildGridMesh(arxGeome morp, afxUnit rows, afxUnit layers, afxReal width, afxReal depth, arxMesh* mesh)
{
    // _ldrShapeCreateRectangularGridPlanef
    afxError err = NIL;
    AFX_ASSERT(mesh);

    afxBool triangleStrip = FALSE;

    rows = AFX_MAX(1, rows);
    layers = AFX_MAX(1, layers);
    afxUnit numberVertices = (rows + 1) * (layers + 1);

    afxUnit numberIndices = (triangleStrip) ? rows * 2 * (layers + 1) : rows * 6 * layers;

    arxMesh msh;
    arxMeshBlueprint mshb = { 0 };
    mshb.vtxCnt = numberVertices;
    mshb.attrCnt = 4;
    mshb.attrs = (afxString[]) { AFX_STRING("pos"), AFX_STRING("nrm"), AFX_STRING("tan"), AFX_STRING("uv") };
    mshb.triCnt = numberIndices / 3;
    ArxBuildMeshes(morp, 1, &mshb, &msh);

    afxUnit* indices = ArxGetMeshIndices(msh, 0);

    ArxFormatVertexAttribute(msh, 0, avxFormat_RGB32f, arxVertexFlag_ATV, &AFX_STRING("pos"));
    ArxFormatVertexAttribute(msh, 1, avxFormat_RGB32f, arxVertexFlag_LTM, &AFX_STRING("nrm"));
    ArxFormatVertexAttribute(msh, 2, avxFormat_RGB32f, arxVertexFlag_LTM, &AFX_STRING("tan"));
    ArxFormatVertexAttribute(msh, 3, avxFormat_RG32f, NIL, &AFX_STRING("uv"));
    afxV3d* pos = ArxAccessVertexData(msh, 0, 0, 0);
    afxV3d* nrm = ArxAccessVertexData(msh, 1, 0, 0);
    afxV3d* tan = ArxAccessVertexData(msh, 2, 0, 0);
    afxV2d* uv0 = ArxAccessVertexData(msh, 3, 0, 0);

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
        indices[i * 6 + 0] = z + x * (layers + 1); // left to right
        indices[i * 6 + 1] = (z + 1) + (x + 1) * (layers + 1);
        indices[i * 6 + 2] = z + (x + 1) * (layers + 1); // top to bottom
        // to
        indices[i * 6 + 3] = (z + 1) + (x + 1) * (layers + 1);
        indices[i * 6 + 4] = z + x * (layers + 1);
        indices[i * 6 + 5] = (z + 1) + x * (layers + 1);
    }

    //ArxInvertMeshTopology(msh);

    *mesh = msh;
    return err;
}
