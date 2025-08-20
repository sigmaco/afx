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

#define _AFX_SIM_C
#define _ARX_TERRAIN_C
#define _ASX_SIMULATION_C
#include "../ddi/arxImpl_Input.h"

/*
    Benefits of world-aligned sectors:
        Easy spatial queries - no transform logic needed.
        Raycasts, AI pathfinding, or physics don't care which sector they're in.
        World is fully seamless - no "jumps" or gaps between sectors.
*/

// Function to check if a point is in front of a plane
afxBool isPointInFrontOfPlane(afxPlane *plane, float point[3])
{
    return (plane->uvwd[0] * point[0] +
        plane->uvwd[1] * point[1] +
        plane->uvwd[2] * point[2] + plane->uvwd[AFX_PLANE_DIST]) >= 0.0f;
}

// Function to check if the AABB is in front of all frustum planes
afxBool isBoxCulled(const afxBox *box, const afxFrustum *frustum)
{
    // Check all planes of the frustum
    for (int i = 0; i < afxCubeFace_TOTAL; ++i)
    {
        afxPlane *plane = &frustum->planes[i];

        // Check the 8 corners of the AABB
        afxBool isBehindPlane = TRUE;
        for (int j = 0; j < 8; ++j)
        {
            // Generate the 8 corners of the AABB
            float corner[3] = {
                (j & 1) ? box->max[0] : box->min[0],
                (j & 2) ? box->max[1] : box->min[1],
                (j & 4) ? box->max[2] : box->min[2]
            };

            // If any corner is in front of the plane, the AABB is not culled
            if (isPointInFrontOfPlane(plane, corner))
            {
                isBehindPlane = FALSE;
                break;
            }
        }

        // If the AABB is completely behind the plane, cull it
        if (isBehindPlane) {
            return TRUE;  // AABB is outside of the frustum, so it's culled
        }
    }

    return FALSE;  // AABB is within the frustum
}

// New function to check an array of AABBs
void cullAABBs(const afxBox *boxes, size_t numBoxes, const afxFrustum *frustum, afxBool *culledResults)
{
    for (size_t i = 0; i < numBoxes; ++i)
    {
        // Cull each box and store the result in the array
        culledResults[i] = isBoxCulled(&boxes[i], frustum);
    }
}

// Optimized function to check if the AABB is in front of all frustum planes
void cullAABBs2(const afxBox *boxes, size_t numBoxes, const afxFrustum *frustum, afxBool *culledResults)
{
    for (size_t i = 0; i < numBoxes; ++i)
    {
        const afxBox *box = &boxes[i];
        afxBool isCulled = FALSE;

        // Check all planes of the frustum
        for (int planeIndex = 0; planeIndex < afxCubeFace_TOTAL; ++planeIndex)
        {
            const afxPlane *plane = &frustum->planes[planeIndex];

            // Check the 8 corners of the AABB
            afxBool isBehindPlane = TRUE;
            for (int j = 0; j < 8; ++j)
            {
                // Generate the 8 corners of the AABB
                float corner[3] = {
                    (j & 1) ? box->max[0] : box->min[0],
                    (j & 2) ? box->max[1] : box->min[1],
                    (j & 4) ? box->max[2] : box->min[2]
                };

                // If any corner is in front of the plane, the AABB is not culled
                if (isPointInFrontOfPlane(plane, corner))
                {
                    isBehindPlane = FALSE;
                    break;
                }
            }

            // If the AABB is completely behind the plane, cull it
            if (isBehindPlane)
            {
                isCulled = TRUE;  // AABB is outside of the frustum, so it's culled
                break;
            }
        }

        // Store the result in the array
        culledResults[i] = isCulled;
    }
}

afxBool CheckHeightOfTriangle(float x, float z, float* height, afxV3d v0, afxV3d v1, afxV3d v2)
{
    afxV3d startVector, directionVector, edge1, edge2, normal;
    afxV3d Q, e1, e2, e3, edgeNormal, temp;
    float magnitude, D, denominator, numerator, t, determinant;


    // Starting position of the ray that is being cast.
    startVector[0] = x;
    startVector[1] = 0.0f;
    startVector[2] = z;

    // The direction the ray is being cast.
    directionVector[0] = 0.0f;
    directionVector[1] = -1.0f;
    directionVector[2] = 0.0f;

    // Calculate the two edges from the three points given.
    edge1[0] = v1[0] - v0[0];
    edge1[1] = v1[1] - v0[1];
    edge1[2] = v1[2] - v0[2];

    edge2[0] = v2[0] - v0[0];
    edge2[1] = v2[1] - v0[1];
    edge2[2] = v2[2] - v0[2];

    // Calculate the normal of the triangle from the two edges.
    normal[0] = (edge1[1] * edge2[2]) - (edge1[2] * edge2[1]);
    normal[1] = (edge1[2] * edge2[0]) - (edge1[0] * edge2[2]);
    normal[2] = (edge1[0] * edge2[1]) - (edge1[1] * edge2[0]);

    magnitude = (float)sqrt((normal[0] * normal[0]) + (normal[1] * normal[1]) + (normal[2] * normal[2]));
    normal[0] = normal[0] / magnitude;
    normal[1] = normal[1] / magnitude;
    normal[2] = normal[2] / magnitude;

    // Find the distance from the origin to the plane.
    D = ((-normal[0] * v0[0]) + (-normal[1] * v0[1]) + (-normal[2] * v0[2]));

    // Get the denominator of the equation.
    denominator = ((normal[0] * directionVector[0]) + (normal[1] * directionVector[1]) + (normal[2] * directionVector[2]));

    // Make sure the result doesn't get too close to zero to prevent divide by zero.
    if (fabs(denominator) < 0.0001f)
    {
        return FALSE;
    }

    // Get the numerator of the equation.
    numerator = -1.0f * (((normal[0] * startVector[0]) + (normal[1] * startVector[1]) + (normal[2] * startVector[2])) + D);

    // Calculate where we intersect the triangle.
    t = numerator / denominator;

    // Find the intersection vector.
    Q[0] = startVector[0] + (directionVector[0] * t);
    Q[1] = startVector[1] + (directionVector[1] * t);
    Q[2] = startVector[2] + (directionVector[2] * t);

    // Find the three edges of the triangle.
    e1[0] = v1[0] - v0[0];
    e1[1] = v1[1] - v0[1];
    e1[2] = v1[2] - v0[2];

    e2[0] = v2[0] - v1[0];
    e2[1] = v2[1] - v1[1];
    e2[2] = v2[2] - v1[2];

    e3[0] = v0[0] - v2[0];
    e3[1] = v0[1] - v2[1];
    e3[2] = v0[2] - v2[2];

    // Calculate the normal for the first edge.
    edgeNormal[0] = (e1[1] * normal[2]) - (e1[2] * normal[1]);
    edgeNormal[1] = (e1[2] * normal[0]) - (e1[0] * normal[2]);
    edgeNormal[2] = (e1[0] * normal[1]) - (e1[1] * normal[0]);

    // Calculate the determinant to see if it is on the inside, outside, or directly on the edge.
    temp[0] = Q[0] - v0[0];
    temp[1] = Q[1] - v0[1];
    temp[2] = Q[2] - v0[2];

    determinant = ((edgeNormal[0] * temp[0]) + (edgeNormal[1] * temp[1]) + (edgeNormal[2] * temp[2]));

    // Check if it is outside.
    if (determinant > 0.001f)
    {
        return FALSE;
    }

    // Calculate the normal for the second edge.
    edgeNormal[0] = (e2[1] * normal[2]) - (e2[2] * normal[1]);
    edgeNormal[1] = (e2[2] * normal[0]) - (e2[0] * normal[2]);
    edgeNormal[2] = (e2[0] * normal[1]) - (e2[1] * normal[0]);

    // Calculate the determinant to see if it is on the inside, outside, or directly on the edge.
    temp[0] = Q[0] - v1[0];
    temp[1] = Q[1] - v1[1];
    temp[2] = Q[2] - v1[2];

    determinant = ((edgeNormal[0] * temp[0]) + (edgeNormal[1] * temp[1]) + (edgeNormal[2] * temp[2]));

    // Check if it is outside.
    if (determinant > 0.001f)
    {
        return FALSE;
    }

    // Calculate the normal for the third edge.
    edgeNormal[0] = (e3[1] * normal[2]) - (e3[2] * normal[1]);
    edgeNormal[1] = (e3[2] * normal[0]) - (e3[0] * normal[2]);
    edgeNormal[2] = (e3[0] * normal[1]) - (e3[1] * normal[0]);

    // Calculate the determinant to see if it is on the inside, outside, or directly on the edge.
    temp[0] = Q[0] - v2[0];
    temp[1] = Q[1] - v2[1];
    temp[2] = Q[2] - v2[2];

    determinant = ((edgeNormal[0] * temp[0]) + (edgeNormal[1] * temp[1]) + (edgeNormal[2] * temp[2]));

    // Check if it is outside.
    if (determinant > 0.001f)
    {
        return FALSE;
    }

    // Now we have our height.
    *height = Q[1];

    return TRUE;
}

afxBool GetHeightAtPosition(arxTerrain ter, float inputX, float inputZ, float* height)
{
    float vertex1[3], vertex2[3], vertex3[3];
    afxBool foundHeight;

    afxV3d v = { inputX, 0, inputZ };

    // Loop through all of the terrain cells to find out which one the inputX and inputZ would be inside.
    afxUnit cellId = -1;
    for (afxUnit i = 0; i < ter->secCnt; i++)
    {
        // Get the current cell dimensions.
        afxBox aabb = ter->sectors[i].aabb;

        // Check to see if the positions are in this cell.
        if (AfxDoesAabbIncludeAtv3d(&aabb, 1, &v))
        {
            cellId = i;
            break;
        }
    }

    // If we didn't find a cell then the input position is off the terrain grid.
    if (cellId == -1)
    {
        return FALSE;
    }

    _arxTerrSec* sec = &ter->sectors[cellId];
    afxUnit collVtxCnt = sec->collVtxCnt;
    afxV3d const* collVtx = sec->collVtx;

    // If this is the right cell then check all the triangles in this cell to see what the height of the triangle at this position is.
    for (afxUnit i = 0; i < (collVtxCnt / 3); i++)
    {
        // Check to see if this is the polygon we are looking for.
        foundHeight = CheckHeightOfTriangle(inputX, inputZ, height, sec->collVtx[i], sec->collVtx[i + 1], sec->collVtx[i + 2]);

        if (foundHeight)
        {
            return TRUE;
        }
    }
    return FALSE;
}

_ARX afxError ArxLoadHeighmap(arxTerrain ter, afxUnit secIdx, afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_TER, 1, &ter);
    AFX_ASSERT_RANGE(ter->secCnt, secIdx, 1);

    afxStream iob;
    if (AfxOpenFile(uri, afxFileFlag_RX, &iob))
    {
        AfxThrowError();
        return err;
    }
    avxRasterFile tga;
    AfxReadRasterFile(&tga, iob);

    afxByte* data;
    AfxAllocate(tga.decSiz, AFX_SIMD_ALIGNMENT, AfxHere(), (void**)&data);
    AfxDecodeRasterFile(&tga, iob, data);
    AfxDisposeObjects(1, &iob);

    avxFormat fmt = tga.fmt;
    avxFormatDescription pfd;
    AvxDescribeFormats(1, &fmt, &pfd);




    arxMesh msh;
    ArxGetTerrainMeshes(ter, secIdx, 1, &msh);
    arxMeshMorph mshm;
    ArxDescribeMeshMorphes(msh, 0, 1, &mshm);

    afxV3d* pos = ArxAccessVertexData(msh, 0, 0, 0);

    afxReal yScale = 64.0f / 256.0f, yShift = 16.0f;  // apply a scale+shift to the height data

    for (unsigned int i = 0; i < tga.width; i++)
    {
        for (unsigned int j = 0; j < tga.height; j++)
        {
            // retrieve texel for (i,j) tex coord
            unsigned char* texel = data + (j + tga.width * i) * pfd.compCnt;
            // raw height at coordinate
            unsigned char y = texel[0];

            // vertex
            AfxV3dSet(pos[i * j + j], -tga.height / 2.0f + i, (int)y * yScale - yShift, -tga.width / 2.0f + j);
        }
    }

    AfxDeallocate((void**)&data, AfxHere());

    return err;
}

_ARX afxUnit ArxGetTerrainMeshes(arxTerrain ter, afxUnit secIdx, afxUnit cnt, arxMesh meshes[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_TER, 1, &ter);
    AFX_ASSERT_RANGE(ter->secCnt, secIdx, cnt);
    afxUnit rslt = 0;

    for (afxUnit i = 0; i < cnt; i++)
    {
        arxMesh msh = ter->sectors[secIdx].msh;

        if (msh)
        {
            AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
            ++rslt;
        }
        meshes[i] = msh;
    }
    return rslt;
}

_ARX afxError ArxResetTerrainSector(arxTerrain ter, afxUnit secIdx, arxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_TER, 1, &ter);
    
    if (ter->sectors[secIdx].msh != msh)
    {
        if (msh)
        {
            AfxReacquireObjects(1, &msh);
        }

        if (ter->sectors[secIdx].msh)
        {
            AfxDisposeObjects(1, &ter->sectors[secIdx].msh);
        }

        ter->sectors[secIdx].msh = msh;
    }
    return err;
}

_ARX afxUnit findSectorIndex(arxTerrain ter, afxReal x, afxReal z)
{
    int sectorX = (int)(x / ter->secWidth);
    int sectorZ = (int)(z / ter->secDepth);
    return sectorZ * ter->rowSecCnt + sectorX;
}

_ARX afxBool getHeightInSector(arxTerrain ter, afxUnit secIdx, afxReal worldX, afxReal worldZ, afxReal* worldY)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_TER, 1, &ter);

    _arxTerrSec* s = &ter->sectors[secIdx];

    // Compute which quad (cell) we're in, directly in world space
    int localX = (int)(worldX)-(s->gridNodeX * ter->secWidth);
    int localZ = (int)(worldZ)-(s->gridNodeZ * ter->secDepth);

    if (localX < 0 || localX >= ter->secWidth || localZ < 0 || localZ >= ter->secDepth)
    {
        // Outside sector bounds
        *worldY = 0.0f; 
        return FALSE;
    }

    afxReal fx = worldX - (s->gridNodeX * ter->secWidth + localX);
    afxReal fz = worldZ - (s->gridNodeZ * ter->secDepth + localZ);

    int vertsPerRow = ter->secWidth + 1;
    int topLeft = localZ * vertsPerRow + localX;
    int topRight = topLeft + 1;
    int bottomLeft = (localZ + 1) * vertsPerRow + localX;
    int bottomRight = bottomLeft + 1;

    afxUnit ia, ib, ic;

    if (fx + fz <= 1.0f)
    {
        // Lower-left triangle
        ia = topLeft;
        ib = bottomLeft;
        ic = topRight;
    }
    else
    {
        // Upper-right triangle
        ia = topRight;
        ib = bottomLeft;
        ic = bottomRight;
    }

    afxV3d const v0 =
    {
        s->collVtx[ia][0],
        s->collVtx[ia][1],
        s->collVtx[ia][2],
    };
    afxV3d const v1 =
    {
        s->collVtx[ib][0],
        s->collVtx[ib][1],
        s->collVtx[ib][2],
    };
    afxV3d const v2 =
    {
        s->collVtx[ic][0],
        s->collVtx[ic][1],
        s->collVtx[ic][2],
    };

    // Barycentric interpolation
    // This is the denominator of a barycentric coordinate equation - it's based on the 2D positions of the triangle's vertices. 
    // It is using a mix of x and z (assuming the triangle lies in xz-plane for interpolation).
    afxReal denom = (v1[2] - v2[2]) * (v0[0] - v2[0]) + (v2[0] - v1[0]) * (v0[2] - v2[2]);
    
    // This checks if the triangle is degenerate - meaning, it has near-zero area (the points are colinear or very close to it).
    // A floating point tolerance check is used to avoid division by nearly-zero, which could cause instability or garbage results.
    if (fabsf(denom) < 1e-6f)
    {
        *worldY = v0[1];
        return FALSE;
    }

    // After checking that the triangle isn't degenerate (that denom isn't near-zero), we are computing barycentric coordinates 
    // of a point (worldX, worldZ) relative to a triangle defined by three vertices v0, v1, v2.

    afxReal lambda1 = ((v1[2] - v2[2]) * (worldX - v2[0]) + (v2[0] - v1[0]) * (worldZ - v2[2])) / denom;
    afxReal lambda2 = ((v2[2] - v0[2]) * (worldX - v2[0]) + (v0[0] - v2[0]) * (worldZ - v2[2])) / denom;
    afxReal lambda3 = 1.0f - lambda1 - lambda2;

    *worldY = lambda1 * v0[1] + lambda2 * v1[1] + lambda3 * v2[1];
    return TRUE;
}

_ARX afxBool ArxGetTerrainHeightAt(arxTerrain ter, afxReal x, afxReal z, afxReal* y)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_TER, 1, &ter);

    afxInt sectorX = (afxInt)(x / ter->secWidth);
    afxInt sectorZ = (afxInt)(z / ter->secDepth);

    if (sectorX < 0 || sectorX >= ter->rowSecCnt || sectorZ < 0 || sectorZ >= ter->sliceSecCnt)
    {
        *y = 0.0f; // Out of bounds
        return FALSE;
    }

    afxUnit secIdx = sectorZ * ter->rowSecCnt + sectorX;
    return getHeightInSector(ter, secIdx, x, z, y);
}

_ARX afxError _ArxBuildTerrainCollideShape(arxTerrain ter, afxUnit baseSecIdx, afxUnit secCnt)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_TER, 1, &ter);
    AFX_ASSERT_RANGE(ter->secCnt, baseSecIdx, secCnt);

    for (afxUnit secIter = 0; secIter < secCnt; secIter++)
    {
        afxUnit secIdx = baseSecIdx + secIter;
        _arxTerrSec* sec = &ter->sectors[secIdx];

        if (!sec->collVtx)
        {
            sec->collTriCnt = sec->visuVtxIdxCnt / 3;
            sec->collVtxCnt = sec->collTriCnt * 3;

            if (AfxAllocate(sec->collVtxCnt * sizeof(sec->collVtx[0]), AFX_SIMD_ALIGNMENT, AfxHere(), (void**)&sec->collVtx))
            {
                AfxThrowError();
                continue;
            }
            AfxZero(sec->collVtx, sec->collVtxCnt * sizeof(sec->collVtx[0]));

            afxUnit triCnt = sec->visuVtxIdxCnt / 3;

            for (int i = 0; i < triCnt; i++)
            {
                int i0 = sec->visuVtxMap[i * 3 + 0];
                int i1 = sec->visuVtxMap[i * 3 + 1];
                int i2 = sec->visuVtxMap[i * 3 + 2];

                AfxV3dCopy(sec->collVtx[i * 3 + 0], sec->visuVtxPos[i0]);
                AfxV3dCopy(sec->collVtx[i * 3 + 1], sec->visuVtxPos[i1]);
                AfxV3dCopy(sec->collVtx[i * 3 + 2], sec->visuVtxPos[i2]);
            }
        }
    }
    return err;
}

_ARX float getHeight(float x, float z, float heightScale)
// Simple height function
{
    return sinf(x * 0.1f) * cosf(z * 0.1f) * heightScale;
}

_ARX afxError _ArxBuildTerrainVisualShape(arxTerrain ter, afxUnit baseSecIdx, afxUnit secCnt)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_TER, 1, &ter);

    for (afxUnit secIter = 0; secIter < secCnt; secIter++)
    {
        afxUnit secIdx = baseSecIdx + secIter;
        _arxTerrSec* sec = &ter->sectors[secIdx];

        // bump up the reference counter.
        sec->visuReqCnt++;

        // Skip if it is already loaded.
        if (sec->visuReqCnt > 1)
            continue;

        afxV3d* vertices = NIL;
        afxUnit16* indices = NIL;

        AFX_ASSERT(sec->vtxCnt == ((ter->secWidth + 1) * (ter->secDepth + 1)));
        sec->visuVtxCnt = ((ter->secWidth + 1) * (ter->secDepth + 1));
        AfxAllocate(sec->visuVtxCnt * sizeof(vertices[0]), AFX_SIMD_ALIGNMENT, AfxHere(), (void**)&vertices);
        AFX_ASSERT(sec->vtxIdxCnt == ((ter->secWidth * ter->secDepth) * 6));
        sec->visuVtxIdxCnt = ((ter->secWidth * ter->secDepth) * 6);
        AfxAllocate(sec->visuVtxIdxCnt * sizeof(indices[0]), AFX_SIMD_ALIGNMENT, AfxHere(), (void**)&indices);

        // Generate vertices
        for (afxUnit z = 0; z <= ter->secDepth; z++)
        {
            for (afxUnit x = 0; x <= ter->secWidth; x++)
            {
                afxUnit idx = z * sec->vtxPerRow + x;
                afxReal worldX = (afxReal)(sec->vtxStartX + x);
                afxReal worldZ = (afxReal)(sec->vtxStartZ + z);
                afxReal y = 0;// getHeight(worldX, worldZ, ter->heightScale);

                AfxV3dSet(vertices[idx], worldX, y, worldZ);
            }
        }

        sec->visuVtxPos = vertices;

        // Generate indices in CCW order
        afxUnit idx = 0;
        for (afxUnit z = 0; z < ter->secDepth; z++)
        {
            for (afxUnit x = 0; x < ter->secWidth; x++)
            {
                afxUnit topLeft = z * sec->vtxPerRow + x;
                afxUnit topRight = topLeft + 1;
                afxUnit bottomLeft = (z + 1) * sec->vtxPerRow + x;
                afxUnit bottomRight = bottomLeft + 1;

                // Triangle 1 (CCW): topLeft -> bottomLeft -> topRight
                indices[idx++] = topLeft;
                indices[idx++] = bottomLeft;
                indices[idx++] = topRight;

                // Triangle 2 (CCW): topRight -> bottomLeft -> bottomRight
                indices[idx++] = topRight;
                indices[idx++] = bottomLeft;
                indices[idx++] = bottomRight;
            }
        }
        AFX_ASSERT(sec->vtxIdxCnt == idx);
        sec->visuVtxMap = indices;

        AfxMakeAabb(&ter->secAabb[secIdx], 0, NIL);
        AfxEmboxTriangles(&ter->secAabb[secIdx], sec->vtxIdxCnt / 3, vertices, indices, sizeof(indices[0]));
        sec->aabb = ter->secAabb[secIdx];
    }
    return err;
}

_ARX afxError _ArxTerDtorCb(arxTerrain ter)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_TER, 1, &ter);

    afxObjectStash const stashes[] =
    {
        {
            .cnt = ter->secCnt,
            .siz = sizeof(ter->sectors[0]),
            .var = (void**)&ter->sectors
        },
        {
            .cnt = ter->secCnt,
            .siz = sizeof(ter->secAabb[0]),
            .var = (void**)&ter->secAabb
        }
    };

    if (AfxDeallocateInstanceData(ter, ARRAY_SIZE(stashes), stashes))
        AfxThrowError();

    return err;
}

_ARX afxError _ArxTerCtorCb(arxTerrain ter, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_TER, 1, &ter);

    arxGeome morp = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MORP, 1, &morp);
    arxTerrainConfig const* cfg = AFX_CAST(arxTerrainConfig const*, args[1]);

    afxDrawSystem dsys = ArxGetDrawInputContext(AfxGetProvider(morp));

    ter->dsys = dsys;

    ter->width = AFX_MAX(128, cfg->width);
    ter->depth = AFX_MAX(128, cfg->depth);
    ter->heightScale = AFX_MAX(16, cfg->heightScale);
    ter->heightmap = cfg->heightmap;

    // Set the height and width of each terrain cell to a fixed 33x33 vertex array.
    ter->secWidth = AFX_MAX(cfg->secWidth, 32);
    ter->secDepth = AFX_MAX(cfg->secHeight, 32);

    // Calculate the number of cells needed to store the terrain data.
    ter->rowSecCnt = ter->width / ter->secWidth;
    ter->sliceSecCnt = ter->depth / ter->secDepth;
    ter->secCnt = ter->sliceSecCnt * ter->rowSecCnt;

    afxObjectStash const stashes[] =
    {
        {
            .cnt = ter->secCnt,
            .siz = sizeof(ter->sectors[0]),
            .var = (void**)&ter->sectors
        },
        {
            .cnt = ter->secCnt,
            .siz = sizeof(ter->secAabb[0]),
            .var = (void**)&ter->secAabb
        }
    };

    if (AfxAllocateInstanceData(ter, ARRAY_SIZE(stashes), stashes))
    {
        AfxThrowError();
        return err;
    }

    // Loop through and initialize all the terrain cells.
    for (afxUnit gridZ = 0; gridZ < ter->sliceSecCnt; gridZ++)
    {
        for (afxUnit gridX = 0; gridX < ter->rowSecCnt; gridX++)
        {
            afxUnit index = (ter->rowSecCnt * gridZ) + gridX;

            _arxTerrSec* sec = &ter->sectors[index];
            *sec = (_arxTerrSec) { 0 };

            sec->gridNode = index;
            sec->gridNodeX = gridX;
            sec->gridNodeZ = gridZ;

            afxUnit vtxStartX = gridX * ter->secWidth;
            afxUnit vtxStartZ = gridZ * ter->secDepth;
            // Calculate the number of vertices in this terrain cell.
            afxUnit vtxPerRow = ter->secWidth + 1;
            afxUnit vtxPerCol = ter->secDepth + 1;
            afxUnit vtxCnt = vtxPerRow * vtxPerCol;
            afxUnit quadCnt = ter->secWidth * ter->secDepth;
            afxUnit idxCnt = quadCnt * 6;

            sec->vtxCnt = vtxCnt;
            sec->vtxIdxCnt = idxCnt;
            sec->quadCnt = quadCnt;
            sec->vtxStartZ = vtxStartZ;
            sec->vtxStartX = vtxStartX;
            sec->vtxPerRow = vtxPerRow;
            sec->vtxPerCol = vtxPerCol;

            sec->msh = NIL;
        }
    }

    for (afxUnit i = 0; i < ter->secCnt; i++)
    {
        _ArxBuildTerrainVisualShape(ter, i, 1);
        _ArxBuildTerrainCollideShape(ter, i, 1);
    }

    avxVertexInput vin[2];
    avxVertexLayout vtxl[2] = { 0 };
    vtxl[0].srcCnt = 1;
    vtxl[0].srcs[0].attrCnt = 5;
    vtxl[0].attrs[0].fmt = avxFormat_RGB32f;
    vtxl[0].attrs[1].fmt = avxFormat_RG32f;
    vtxl[0].attrs[1].location = 1;
    vtxl[0].attrs[1].offset = 12;
    vtxl[0].attrs[2].fmt = avxFormat_RGB32f;
    vtxl[0].attrs[2].location = 2;
    vtxl[0].attrs[2].offset = 20;
    vtxl[0].attrs[3].fmt = avxFormat_RGB32f;
    vtxl[0].attrs[3].location = 3;
    vtxl[0].attrs[3].offset = 32;
    vtxl[0].attrs[4].fmt = avxFormat_RGB32f;
    vtxl[0].attrs[4].location = 4;
    vtxl[0].attrs[4].offset = 44;
    vtxl[1].srcCnt = 1;
    vtxl[1].srcs[0].attrCnt = 1;
    vtxl[1].attrs[0].fmt = avxFormat_RGB32f;
    AvxDeclareVertexInputs(dsys, 2, vtxl, vin);

    avxPipeline pip[2];
    avxPipelineBlueprint pipb[2] = { 0 };
    pipb[0].depthTestEnabled = TRUE;
    pipb[0].vin = vin[0];
    pipb[0].tag = AFX_STRING("terrainMesh");
    pipb[0].primTop = avxTopology_TRI_LIST;
    pipb[1].depthTestEnabled = TRUE;
    pipb[1].vin = vin[1];
    pipb[1].tag = AFX_STRING("terrainAabb");
    pipb[1].primTop = avxTopology_LINE_LIST;
    pipb[1].fillMode = avxFillMode_LINE;
    AvxAssemblePipelines(dsys, 2, pipb, pip);
    AvxUplinkPipelineFunction(pip[0], avxShaderType_VERTEX, AfxUri("../terrainMeshVsh.glsl"), NIL, NIL, NIL);
    AvxUplinkPipelineFunction(pip[0], avxShaderType_FRAGMENT, AfxUri("../terrainMeshFsh.glsl"), NIL, NIL, NIL);
    AvxUplinkPipelineFunction(pip[1], avxShaderType_VERTEX, AfxUri("../terrainAabbVsh.glsl"), NIL, NIL, NIL);
    AvxUplinkPipelineFunction(pip[1], avxShaderType_FRAGMENT, AfxUri("../terrainAabbFsh.glsl"), NIL, NIL, NIL);

    ter->dbgAabbPip = pip[1];
    ter->meshPip = pip[0];

    AfxDisposeObjects(2, vin);

    if (err && AfxDeallocateInstanceData(ter, ARRAY_SIZE(stashes), stashes))
        AfxThrowError();

    return err;
}

_ARX afxClassConfig const _ARX_TER_CLASS_CONFIG =
{
    .fcc = afxFcc_TER,
    .name = "Terrain",
    .desc = "Managed Terrain",
    .fixedSiz = sizeof(AFX_OBJECT(arxTerrain)),
    .ctor = (void*)_ArxTerCtorCb,
    .dtor = (void*)_ArxTerDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ARX afxError ArxAcquireTerrain(arxGeome morp, arxTerrainConfig const* cfg, arxTerrain* terrain)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MORP, 1, &morp);

    afxClass* cls = (afxClass *)_ArxMorpGetTerrainClass(morp);
    AFX_ASSERT_CLASS(cls, afxFcc_TER);

    if (AfxAcquireObjects(cls, 1, (afxObject*)terrain, (void const*[]) { morp, cfg }))
    {
        AfxThrowError();
        return err;
    }

    return err;
}

_ARX afxError ArxGenerateTerrain(arxGeome morp, afxWarp const whd, arxTerrain* terrain)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MORP, 1, &morp);

    afxUnit width = whd.w, height = whd.h, depth = whd.d;

    arxMesh msh;
    //msh = AfxBuildPlaneMesh(sim->din, 256, 256, 256, 256);
    //msh = AfxBuildParallelepipedMesh(sim->din, AfxWhd(10, 10, 10), 10, 10);
    //msh = AfxBuildCubeMesh(sim->din, 10, 1);
    ArxBuildGridMesh(morp, 100, 100, 1000, 1000, &msh);
    //msh = AfxBuildGridMesh(sim->din, 10, 10, 10, 10);
    //msh = AfxBuildDiscMesh(sim->din, 10, 10);
    //AfxInvertMeshTopology(msh);

    arxTerrain ter;
    arxTerrainConfig terc = { 0 };
    ArxAcquireTerrain(morp, &terc, &ter);
    ArxResetTerrainSector(ter, 0, msh);
    AfxDisposeObjects(1, &msh);
    *terrain = ter;

    return err;
}

_ARX afxError ArxGenerateHeightmappedTerrain(arxGeome morp, afxUri const* uri, arxTerrain* terrain)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MORP, 1, &morp);

#if 0
    avxRaster ras;
    AvxLoadRasters(sim->dsys, 1, NIL, uri, &ras);
    AvxPrintRaster(ras, AfxUri("../heightmap_dump.tga"), NIL, 1, 0);
#endif

    afxStream iob;
    afxStreamInfo iobi = { 0 };
    iobi.usage = afxIoUsage_FILE;
    AfxAcquireStream(1, &iobi, &iob);
    AfxReopenFile(iob, uri, afxFileFlag_R);

    avxRasterFile tga;
    AfxReadRasterFile(&tga, iob);

    afxByte* data;
    AfxAllocate(tga.decSiz, AFX_SIMD_ALIGNMENT, AfxHere(), (void**)&data);
    AfxDecodeRasterFile(&tga, iob, data);

    avxFormat fmt = tga.fmt;
    avxFormatDescription pfd;
    AvxDescribeFormats(1, &fmt, &pfd);

    //tga.width = tga.width, tga.height = tga.height / 8;

    arxMesh msh;
    ArxBuildGridMesh(morp, tga.width, tga.height, tga.width * 10, tga.height * 10, &msh);

    arxTerrain ter;
    arxTerrainConfig terc = {0};
    ArxAcquireTerrain(morp, &terc, &ter);
    ArxResetTerrainSector(ter, 0, msh);

    //arxMesh msh;
    ArxGetTerrainMeshes(ter, 0, 1, &msh);
    arxMeshMorph mshm;
    ArxDescribeMeshMorphes(msh, 0, 1, &mshm);

    afxV3d* pos = ArxAccessVertexData(msh, 0, 0, 0);

    afxReal yScale = 64.0f / 256.0f, yShift = 16.0f;  // apply a scale+shift to the height data

    afxUnit vtxIdx = 0;
    for (unsigned int i = 0; i < tga.height; i++)
    {
        for (unsigned int j = 0; j < tga.width; j++)
        {
            unsigned char* pixelOffset = data + (j + tga.width * i) * pfd.stride;
            unsigned char y = pixelOffset[0];
#if 0
#if 0
            // vertex
            AfxV3dSet(pos[vtxIdx++],
                (-tga.height / 2.0f + tga.height * i / (float)tga.height), // vx
                ((int)y * yScale - yShift), // vy
                (-tga.width / 2.0f + tga.width * j / (float)tga.width));   // vz
#else
            // vertex
            AfxV3dSet(pos[vtxIdx++],
                (-tga.width / 2.0f + tga.width * j / (float)tga.width), // vx
                ((int)y * yScale - yShift), // vy
                (-tga.height / 2.0f + tga.height * i / (float)tga.height));   // vz
#endif
#endif
            pos[(j + tga.width * i)][1] = ((afxReal)y * yScale - yShift);

        }
    }
    *terrain = ter;
    return err;
}
