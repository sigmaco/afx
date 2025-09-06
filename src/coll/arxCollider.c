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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#include "../sim/impl/asxImplementation.h"
#include "qwadro/sim/afxCollision.h"
#include "qwadro/coll/arxCollider.h"
#include "qwadro/coll/afxRay.h"

afxBox triangle_compute_aabb(arxCollTriangle const* tri)
{
    // To compute the AABB, we need the 3 actual vertices.

    afxV3d v1;
    AfxV3dAdd(v1, tri->v0, tri->e1);
    afxV3d v2;
    AfxV3dAdd(v2, tri->v0, tri->e2);

    afxBox box;
    box.min[0] = fminf(fminf(tri->v0[0], v1[0]), v2[0]);
    box.min[1] = fminf(fminf(tri->v0[1], v1[1]), v2[1]);
    box.min[2] = fminf(fminf(tri->v0[2], v1[2]), v2[2]);

    box.max[0] = fmaxf(fmaxf(tri->v0[0], v1[0]), v2[0]);
    box.max[1] = fmaxf(fmaxf(tri->v0[1], v1[1]), v2[1]);
    box.max[2] = fmaxf(fmaxf(tri->v0[2], v1[2]), v2[2]);

    return box;
}

void triangle_center(afxV3d c, arxCollTriangle const* tri)
{
    // To compute the centroid of a triangle during BVH partitioning.

    afxV3d v1;
    AfxV3dAdd(v1, tri->v0, tri->e1);
    afxV3d v2;
    AfxV3dAdd(v2, tri->v0, tri->e2);
    afxV3d sum, v01;
    AfxV3dAdd(v01, tri->v0, v1);
    AfxV3dAdd(sum, v01, v2);
    AfxV3dScale(c, sum, 1.0f / 3.0f);
}

// Triangle construction
inline void compute_triangle(arxCollTriangle* tri, afxV3d v0, afxV3d v1, afxV3d v2)
{
    AfxV3dCopy(tri->v0, v0);
    AfxV3dSub(tri->e1, v1, v0);
    AfxV3dSub(tri->e2, v2, v0);
    afxV3d c;
    AfxV3dCross(c, tri->e1, tri->e2);
    AfxV3dNormalize(tri->nrm, c);
}

// Triangle construction
void ArxComputeCollidableTriangles(afxV3d const vertices[], afxUnit vtxStride, afxInt const indices[], afxUnit idxSiz, afxUnit triCnt, arxCollTriangle* tri)
{
    afxError err = NIL;

    afxByte const* vertices2 = (afxByte const*)vertices;

    if (!indices)
    {
        for (afxUnit i = 0; i < triCnt; i++)
        {
            afxUnit32 ia = i + 0;
            afxUnit32 ib = i + 1;
            afxUnit32 ic = i + 2;
            compute_triangle(&tri[i], (afxReal const*)&vertices2[ia * vtxStride], (afxReal const*)&vertices2[ib * vtxStride], (afxReal const*)&vertices2[ic * vtxStride]);
        }
    }
    else
    {
        switch (idxSiz)
        {
        case sizeof(afxUnit32):
        {
            afxUnit32 const* indices32 = indices;

            for (afxUnit i = 0; i < triCnt; i++)
            {
                afxUnit32 ia = indices32[i + 0];
                afxUnit32 ib = indices32[i + 1];
                afxUnit32 ic = indices32[i + 2];
                compute_triangle(&tri[i], (afxReal const*)&vertices2[ia * vtxStride], (afxReal const*)&vertices2[ib * vtxStride], (afxReal const*)&vertices2[ic * vtxStride]);
            }
            break;
        }
        case sizeof(afxUnit16):
        {
            afxUnit16 const* indices32 = indices;

            for (afxUnit i = 0; i < triCnt; i++)
            {
                afxUnit16 ia = indices32[i + 0];
                afxUnit16 ib = indices32[i + 1];
                afxUnit16 ic = indices32[i + 2];
                compute_triangle(&tri[i], (afxReal const*)&vertices2[ia * vtxStride], (afxReal const*)&vertices2[ib * vtxStride], (afxReal const*)&vertices2[ic * vtxStride]);
            }
            break;
        }
        case sizeof(afxUnit8):
        {
            afxUnit8 const* indices32 = indices;

            for (afxUnit i = 0; i < triCnt; i++)
            {
                afxUnit8 ia = indices32[i + 0];
                afxUnit8 ib = indices32[i + 1];
                afxUnit8 ic = indices32[i + 2];
                compute_triangle(&tri[i], (afxReal const*)&vertices2[ia * vtxStride], (afxReal const*)&vertices2[ib * vtxStride], (afxReal const*)&vertices2[ic * vtxStride]);
            }
            break;
        }
        default: AfxThrowError(); break;
        }
    }
}

// Ray-triangle intersection: returns 1 if hit, 0 otherwise; t_out gets distance
// A ray-triangle intersection function using the Moller–Trumbore algorithm, optimized for collidable triangle.
int ray_intersects_triangle2(afxRay const* ray, arxCollTriangle const* tri, float* t_out)
{
    // This works perfectly with our collidable triangle format; no need to reconstruct v1, v2 unless we want to visualize or compute centroids.

    //const afxReal EPSILON = 1e-6f;

    afxV3d pvec;
    AfxV3dCross(pvec, ray->normal, tri->e2);
    afxReal det = AfxV3dDot(tri->e1, pvec);
    if (fabsf(det) < AFX_EPSILON) return 0;

    afxReal invDet = 1.0f / det;
    afxV3d tvec;
    AfxV3dSub(tvec, ray->origin, tri->v0);
    afxReal u = AfxV3dDot(tvec, pvec) * invDet;
    if (u < 0.0f || u > 1.0f) return 0;

    afxV3d qvec;
    AfxV3dCross(qvec, tvec, tri->e1);
    afxReal v = AfxV3dDot(ray->normal, qvec) * invDet;
    if (v < 0.0f || u + v > 1.0f) return 0;

    afxReal t = AfxV3dDot(tri->e2, qvec) * invDet;
    if (t > AFX_EPSILON)
    {
        *t_out = t;
        return 1;
    }
    return 0;
}

afxError ArxBuildCollider(arxRenderware rwe, void* idd)
{

}

#if 0
typedef struct VkAccelerationStructureGeometryAabbsDataKHR {
    VkDeviceOrHostAddressConstKHR    data;
    VkDeviceSize                     stride;
} VkAccelerationStructureGeometryAabbsDataKHR;

typedef struct VkAccelerationStructureGeometryInstancesDataKHR {
    VkBool32                         arrayOfPointers;
    VkDeviceOrHostAddressConstKHR    data;
} VkAccelerationStructureGeometryInstancesDataKHR;

typedef struct VkAccelerationStructureGeometryTrianglesDataKHR {
    VkFormat                         vertexFormat;
    VkDeviceOrHostAddressConstKHR    vertexData;
    VkDeviceSize                     vertexStride;
    uint32_t                         maxVertex;
    VkIndexType                      indexType;
    VkDeviceOrHostAddressConstKHR    indexData;
    VkDeviceOrHostAddressConstKHR    transformData;
} VkAccelerationStructureGeometryTrianglesDataKHR;

typedef union VkAccelerationStructureGeometryDataKHR {
    VkAccelerationStructureGeometryTrianglesDataKHR    triangles;
    VkAccelerationStructureGeometryAabbsDataKHR        aabbs;
    VkAccelerationStructureGeometryInstancesDataKHR    instances;
} VkAccelerationStructureGeometryDataKHR;

typedef struct VkAccelerationStructureGeometryKHR {
    VkGeometryTypeKHR                         geometryType;
    VkAccelerationStructureGeometryDataKHR    geometry;
    VkGeometryFlagsKHR                        flags;
} VkAccelerationStructureGeometryKHR;

typedef struct VkAccelerationStructureBuildRangeInfoKHR {
    uint32_t    primitiveCount;
    uint32_t    primitiveOffset;
    uint32_t    firstVertex;
    uint32_t    transformOffset;
} VkAccelerationStructureBuildRangeInfoKHR;

typedef struct VkAccelerationStructureBuildGeometryInfoKHR {
    VkAccelerationStructureTypeKHR                      type;
    VkBuildAccelerationStructureFlagsKHR                flags;
    VkBuildAccelerationStructureModeKHR                 mode;
    VkAccelerationStructureKHR                          srcAccelerationStructure;
    VkAccelerationStructureKHR                          dstAccelerationStructure;
    uint32_t                                            geometryCount;
    const VkAccelerationStructureGeometryKHR*           pGeometries;
    const VkAccelerationStructureGeometryKHR* const*    ppGeometries;
    VkDeviceOrHostAddressKHR                            scratchData;
} VkAccelerationStructureBuildGeometryInfoKHR;

typedef struct VkAccelerationStructureCreateInfoKHR {
    VkAccelerationStructureCreateFlagsKHR    createFlags;
    VkBuffer                                 buffer;
    VkDeviceSize                             offset;
    VkDeviceSize                             size;
    VkAccelerationStructureTypeKHR           type;
    VkDeviceAddress                          deviceAddress;
} VkAccelerationStructureCreateInfoKHR;
#endif
