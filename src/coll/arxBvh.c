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
#include "qwadro/coll/arxBvh.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include <stdbool.h>

#define MAX_TRIANGLES 1024
#define MAX_BVH_NODES (MAX_TRIANGLES * 2)

typedef struct
{
    float x, y, z;
} float3;

typedef struct
{
    float3 v0;
    float3 e1;
    float3 e2;
    float3 normal;
} ColliderTri;

typedef struct
{
    float3 min;
    float3 max;
} AABB;

typedef struct
{
    AABB bounds;
    int leftIndex;
    int rightIndex;
    int start;
    int count;
    bool isLeaf;
} BVHNode;

typedef struct
{
    float3 orig;
    float3 dir;
} Ray;

typedef struct
{
    float t;
    int triangleIndex;
} Hit;

typedef struct
{
    BVHNode* nodes;
    int nodeCount;

    int* triangleIndices; // triangles
    int* leafTriangleIndices; // triangles
    int leafIndexCursor;
} BVH;

float3 float3_add(float3 a, float3 b)
{
    return (float3) { a.x + b.x, a.y + b.y, a.z + b.z };
}

float3 float3_sub(float3 a, float3 b)
{
    return (float3) { a.x - b.x, a.y - b.y, a.z - b.z };
}

float3 float3_min(float3 a, float3 b)
{
    return (float3) { fminf(a.x, b.x), fminf(a.y, b.y), fminf(a.z, b.z) };
}

float3 float3_max(float3 a, float3 b)
{
    return (float3) { fmaxf(a.x, b.x), fmaxf(a.y, b.y), fmaxf(a.z, b.z) };
}

float dot(float3 a, float3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float3 cross(float3 a, float3 b)
{
    return (float3) {
        a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
    };
}

AABB compute_triangle_aabb(const ColliderTri* tri)
{
    float3 v1 = float3_add(tri->v0, tri->e1);
    float3 v2 = float3_add(tri->v0, tri->e2);
    float3 min = float3_min(tri->v0, float3_min(v1, v2));
    float3 max = float3_max(tri->v0, float3_max(v1, v2));
    return (AABB) { min, max };
}

AABB merge_aabb(AABB a, AABB b)
{
    return (AABB) {
        float3_min(a.min, b.min),
            float3_max(a.max, b.max)
    };
}

float3 triangle_centroid(const ColliderTri* tri)
{
    float3 v1 = float3_add(tri->v0, tri->e1);
    float3 v2 = float3_add(tri->v0, tri->e2);
    float3 sum = float3_add(tri->v0, float3_add(v1, v2));
    return (float3) { sum.x / 3.0f, sum.y / 3.0f, sum.z / 3.0f };
}

int store_triangle_indices(BVH* bvh, int* src, int count)
{
    int offset = bvh->leafIndexCursor;
    memcpy(&bvh->leafTriangleIndices[offset], src, sizeof(int) * count);
    bvh->leafIndexCursor += count;
    return offset;
}

typedef struct
{
    int index;
    float key;
} SortEntry;

int compare_entries(const void* a, const void* b)
{
    float ka = ((SortEntry*)a)->key;
    float kb = ((SortEntry*)b)->key;
    return (ka < kb) ? -1 : (ka > kb);
}

void sort_triangle_indices_by_centroid_axis(int* indices, int count, const ColliderTri* tris, int axis)
{
    SortEntry* entries = malloc(sizeof(SortEntry) * count);
    for (int i = 0; i < count; ++i) {
        float3 c = triangle_centroid(&tris[indices[i]]);
        entries[i].index = indices[i];
        entries[i].key = (axis == 0) ? c.x : (axis == 1 ? c.y : c.z);
    }

    qsort(entries, count, sizeof(SortEntry), compare_entries);
    for (int i = 0; i < count; ++i)
        indices[i] = entries[i].index;
    free(entries);
}

int build_bvh_node(BVH* bvh, int* nodeCounter, int* triIndices, int count, const ColliderTri* tris, int maxLeafSize)
{
    int index = (*nodeCounter)++;
    BVHNode* node = &bvh->nodes[index];

    AABB bounds = compute_triangle_aabb(&tris[triIndices[0]]);
    for (int i = 1; i < count; ++i)
        bounds = merge_aabb(bounds, compute_triangle_aabb(&tris[triIndices[i]]));

    node->bounds = bounds;

    if (count <= maxLeafSize) {
        node->isLeaf = true;
        node->start = store_triangle_indices(bvh, triIndices, count);
        node->count = count;
        node->leftIndex = node->rightIndex = -1;
        return index;
    }

    float3 extent = float3_sub(bounds.max, bounds.min);
    int axis = (extent.x > extent.y && extent.x > extent.z) ? 0 :
        (extent.y > extent.z) ? 1 : 2;

    sort_triangle_indices_by_centroid_axis(triIndices, count, tris, axis);

    int mid = count / 2;
    int left = build_bvh_node(bvh, nodeCounter, triIndices, mid, tris, maxLeafSize);
    int right = build_bvh_node(bvh, nodeCounter, triIndices + mid, count - mid, tris, maxLeafSize);

    node->isLeaf = false;
    node->leftIndex = left;
    node->rightIndex = right;
    node->start = -1;
    node->count = 0;

    return index;
}

#define MAX_TRIANGLES_PER_LEAF 8

BVH build_bvh(const ColliderTri* tris, int triCount, int maxLeafSize)
{
    BVH bvh = { 0 };

    bvh.triangleIndices = malloc(sizeof(int) * triCount);
    bvh.leafTriangleIndices = malloc(sizeof(int) * triCount * MAX_TRIANGLES_PER_LEAF);

    // Estimate max number of nodes: 2N - 1 (worst case)
    int maxNodes = 2 * triCount - 1;
    bvh.nodes = malloc(sizeof(BVHNode) * maxNodes);

    for (int i = 0; i < triCount; ++i)
        bvh.triangleIndices[i] = i;

    int counter = 0;
    build_bvh_node(&bvh, &counter, bvh.triangleIndices, triCount, tris, maxLeafSize);
    bvh.nodeCount = counter;
    return bvh;
}

bool ray_intersects_aabb(Ray ray, AABB box)
{
    float tmin = 0.0f, tmax = FLT_MAX;
    for (int i = 0; i < 3; ++i) {
        float invD = 1.0f / ((&ray.dir.x)[i]);
        float t0 = ((&box.min.x)[i] - (&ray.orig.x)[i]) * invD;
        float t1 = ((&box.max.x)[i] - (&ray.orig.x)[i]) * invD;
        if (invD < 0.0f) {
            float tmp = t0; t0 = t1; t1 = tmp;
        }
        tmin = fmaxf(tmin, t0);
        tmax = fminf(tmax, t1);
        if (tmax < tmin) return false;
    }
    return true;
}

bool ray_intersects_triangle(Ray ray, const ColliderTri* tri, float* tOut)
{
    const float EPSILON = 1e-6f;
    float3 pvec = cross(ray.dir, tri->e2);
    float det = dot(tri->e1, pvec);
    if (fabsf(det) < EPSILON) return false;

    float invDet = 1.0f / det;
    float3 tvec = float3_sub(ray.orig, tri->v0);
    float u = dot(tvec, pvec) * invDet;
    if (u < 0.0f || u > 1.0f) return false;

    float3 qvec = cross(tvec, tri->e1);
    float v = dot(ray.dir, qvec) * invDet;
    if (v < 0.0f || u + v > 1.0f) return false;

    float t = dot(tri->e2, qvec) * invDet;
    if (t > EPSILON) {
        *tOut = t;
        return true;
    }
    return false;
}

bool bvh_raycast(const BVH* bvh, const ColliderTri* tris, Ray ray, Hit* hit)
{
    int stack[64];
    int stackSize = 0;
    stack[stackSize++] = 0; // root node

    hit->t = FLT_MAX;
    hit->triangleIndex = -1;

    while (stackSize > 0) {
        int index = stack[--stackSize];
        const BVHNode* node = &bvh->nodes[index];

        if (!ray_intersects_aabb(ray, node->bounds))
            continue;

        if (node->isLeaf) {
            for (int i = 0; i < node->count; ++i) {
                int triIdx = bvh->leafTriangleIndices[node->start + i];
                float t;
                if (ray_intersects_triangle(ray, &tris[triIdx], &t)) {
                    if (t < hit->t) {
                        hit->t = t;
                        hit->triangleIndex = triIdx;
                    }
                }
            }
        }
        else {
            stack[stackSize++] = node->leftIndex;
            stack[stackSize++] = node->rightIndex;
        }
    }

    return hit->triangleIndex != -1;
}

void print_bvh_node(const BVH* bvh, int index, int depth)
{
    if (index < 0 || index >= bvh->nodeCount) return;

    const BVHNode* node = &bvh->nodes[index];
    for (int i = 0; i < depth; ++i) printf("  ");

    if (node->isLeaf) {
        printf("Leaf: Triangles [%d - %d]\n", node->start, node->start + node->count - 1);
    }
    else {
        printf("Node: Split L=%d R=%d\n", node->leftIndex, node->rightIndex);
        print_bvh_node(bvh, node->leftIndex, depth + 1);
        print_bvh_node(bvh, node->rightIndex, depth + 1);
    }
}

void debug_print_bvh(const BVH* bvh)
{
    print_bvh_node(bvh, 0, 0);
}

BVH load_bvh_from_file(const char* filename)
{
    BVH bvh = { 0 };
    FILE* f = fopen(filename, "rb");
    if (!f) return bvh;

    fread(&bvh.nodeCount, sizeof(int), 1, f);
    bvh.nodes = malloc(sizeof(BVHNode) * bvh.nodeCount);
    fread(bvh.nodes, sizeof(BVHNode), bvh.nodeCount, f);

    fread(&bvh.leafIndexCursor, sizeof(int), 1, f);
    bvh.leafTriangleIndices = malloc(sizeof(int) * bvh.leafIndexCursor);
    fread(bvh.leafTriangleIndices, sizeof(int), bvh.leafIndexCursor, f);

    fclose(f);
    return bvh;
}

void save_bvh_to_file(const BVH* bvh, const char* filename)
{
    FILE* f = fopen(filename, "wb");
    if (!f) return;

    fwrite(&bvh->nodeCount, sizeof(int), 1, f);
    fwrite(bvh->nodes, sizeof(BVHNode), bvh->nodeCount, f);
    fwrite(&bvh->leafIndexCursor, sizeof(int), 1, f);
    fwrite(bvh->leafTriangleIndices, sizeof(int), bvh->leafIndexCursor, f);

    fclose(f);
}

void free_bvh(BVH* bvh)
{
    free(bvh->nodes);
    free(bvh->triangleIndices);
    free(bvh->leafTriangleIndices);
}
