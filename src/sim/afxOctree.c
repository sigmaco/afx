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

#include "impl/asxImplementation.h"
#include "qwadro/sim/afxOctree.h"


ASX void AfxDeployOctree(afxOctree* tree, afxBox* bounds)
{
    tree->root.bounds = *bounds;
    tree->root.parentIdx = AFX_INVALID_INDEX;

    for (afxUnit i = 0; i < /* it is always a octree*/ 8; i++)
        tree->root.childIdx[i] = AFX_INVALID_INDEX;

    AfxDeployChain(&tree->root.contents, tree);
    AfxDeployPool(&tree->octants, sizeof(afxOctant), /* it is always a octree*/ 8, AFX_SIMD_ALIGNMENT);
}

ASX afxError AfxSubdivideOctree(afxOctree* tree, afxUnit octantIdx)
// Subdivide the node into 8 octants
{
    afxError err = NIL;

    afxOctant* node = NIL;
    AfxGetPoolUnit(&tree->octants, octantIdx, (void**)&node);
    AFX_ASSERT(node);

    if (node->childIdx[0] != AFX_INVALID_INDEX)
    {
        AfxThrowError();
        return err;
    }

    afxReal midX = (node->bounds.min[0] + node->bounds.max[0]) / 2;
    afxReal midY = (node->bounds.min[1] + node->bounds.max[1]) / 2;
    afxReal midZ = (node->bounds.min[2] + node->bounds.max[2]) / 2;

    afxBox bounds[8];
    for (afxUnit i = 0; i < 8; i++)
    {
        if (i & 1) bounds[i].min[0] = midX;
        else bounds[i].max[0] = midX;
        if (i & 2) bounds[i].min[1] = midY;
        else bounds[i].max[1] = midY;
        if (i & 4) bounds[i].min[2] = midZ;
        else bounds[i].max[2] = midZ;
    }

    // Create the 8 new children with updated bounding boxes
    for (afxUnit i = 0; i < 8; i++)
    {
        afxSize childIdx;
        afxOctant* child = NIL;
        if (!(AfxRequestPoolUnits(&tree->octants, 1, &childIdx, &child)))
        {
            AfxThrowError();

            for (afxUnit j = i; j-- > 0;)
            {
                childIdx = j;
                // TODO: Fix this shit
                AfxReclaimPoolUnits(&tree->octants, 1, &child);
                node->childIdx[j] = AFX_INVALID_INDEX;
            }
            break;
        }

        child->bounds = bounds[i];

        child->parentIdx = octantIdx;
        for (afxUnit j = 0; j < 8; j++)
            child->childIdx[j] = AFX_INVALID_INDEX;

        AfxDeployChain(&child->contents, child);

        node->childIdx[i] = childIdx;
    }
    return err;
}

// Function to insert an AABB into the Octree
void AfxOctreeInsertAabbs(afxOctree* tree, afxUnit octantIdx, afxBox* box)
{
    afxError err = NIL;

    // expand the tree if it does not fit.
    AfxEmboxAabbs(&tree->root.bounds, 1, box);

    afxOctant* node = NIL;
    if (octantIdx == AFX_INVALID_INDEX) node = &tree->root;
    else
    {
        AfxGetPoolUnit(&tree->octants, octantIdx, (void**)&node);
        AFX_ASSERT(node);
    }

    // Check if the AABB intersects with the node's bounding box. No intersection, don't insert.
    if (!AfxDoesAabbInclude(&node->bounds, 1, box))
        return;

    // If the node doesn't have children, check if it's time to subdivide
    if (node->childIdx[0] == AFX_INVALID_INDEX)
        if (AfxSubdivideOctree(tree, octantIdx))
            AfxThrowError();

    // Insert the AABB into the children or store it in the node itself
    afxBool inserted = FALSE;
    for (afxUnit i = 0; i < 8; i++)
    {
        afxOctant* child = NIL;
        afxSize childIdx = node->childIdx[i];
        if (!AfxGetPoolUnit(&tree->octants, childIdx, (void**)&child))
        {
            AfxThrowError();
            break;
        }

        if (AfxDoesAabbInclude(&child->bounds, 1, box))
        {
            AfxOctreeInsertAabbs(tree, childIdx, box);  // Insert into the appropriate child
            inserted = TRUE;
        }
    }

    // If the AABB is completely inside the node (and doesn't span children),
    // you could store it at the node's level (depending on your use case).
    // For now, we don't store data at the node level in this example.

    AFX_ASSERT(inserted);
}

void AfxOctreeInsertAtv3d(afxOctree* tree, afxUnit octantIdx, afxV3d p)
// Insert a point into the Octree
{
    afxError err = NIL;

    afxOctant* node = NIL;
    if (octantIdx == AFX_INVALID_INDEX) node = &tree->root;
    else
    {
        AfxGetPoolUnit(&tree->octants, octantIdx, (void**)&node);
        AFX_ASSERT(node);
    }

    // If the point is outside the bounding box, do not insert
    if (!AfxDoesAabbIncludeAtv3d(&node->bounds, 1, p)) return;

    // If the node doesn't have children, check if it's time to subdivide
    if (node->childIdx[0] == AFX_INVALID_INDEX)
        if (AfxSubdivideOctree(tree, octantIdx))
            AfxThrowError();

    // Insert the AABB into the children or store it in the node itself
    afxBool inserted = FALSE;
    for (afxUnit i = 0; i < 8; i++)
    {
        afxOctant* child = NIL;
        afxSize childIdx = node->childIdx[i];
        if (!AfxGetPoolUnit(&tree->octants, childIdx, (void**)&child))
        {
            AfxThrowError();
            break;
        }

        if (AfxDoesAabbIncludeAtv3d(&child->bounds, 1, p))
        {
            AfxOctreeInsertAabbs(tree, childIdx, p);  // Insert into the appropriate child
            inserted = TRUE;
            break;
        }
    }

    AFX_ASSERT(inserted);
}
