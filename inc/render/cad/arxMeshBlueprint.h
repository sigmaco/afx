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

#ifndef ARX_MESH_BLUEPRINT_H
#define ARX_MESH_BLUEPRINT_H

#include "qwadro/inc/sim/afxSimDefs.h"
#include "qwadro/inc/draw/afxDrawDefs.h"
#include "qwadro/inc/mem/afxArray.h"
#include "qwadro/inc/math/coll/afxBox.h"
#include "qwadro/inc/base/afxObject.h"
#include "qwadro/inc/io/afxUrd.h"
#include "qwadro/inc/math/afxVertex.h"
//#include "qwadro/inc/draw/io/afxVertexStream.h"
#include "qwadro/inc/base/afxFixedString.h"
#include "qwadro/inc/render/cad/arxTriangulation.h"

#define ARX_INDICES_PER_TRI 3
#define ARX_MAX_VERTEX_ATTRIBUTES 32

typedef afxIndex8 afxTriangle8[3];
typedef afxIndex16 afxTriangle16[3];
typedef afxIndex32 afxTriangle32[3];

typedef enum arxMeshSectionFlag
{
    arxMeshSectionFlag_HOLE = AFX_BITMASK(0)
} arxMeshSectionFlags;

typedef enum arxMeshMorphFlag
{
    // Morph geometry contains displacement data (deltas).
    arxMeshMorphFlag_DELTA = AFX_BITMASK(0),

    // Values are obtained by adding the original values to a weighted sum of the target's values.
    arxMeshMorphFlag_ADDITIVE = AFX_BITMASK(1),
} arxMeshMorphFlags;

/**
    The morph targets are a extension to the arxMesh structure.
    For each morph target attribute, an original attribute MUST be present in the mesh.
    A implementation SHOULD support at least POSITION, NORMAL and TANGENT attributes for morphing.
    All morphed data MUST have the same 'vtxCnt' as the base morph of the mesh.
*/

AFX_DEFINE_STRUCT(arxMeshMorph)
// aka morph target, blend shape
{
    arxMeshMorphFlags   flags;
    // For each morph target attribute, an original attribute MUST be present in the mesh.
    // Implementations SHOULD support at least 8 morphed attributes.
    afxUnit             affectedAttrCnt;
    afxMask             affectedAttrs;
    afxUnit             baseVtx;
    // All morphed data MUST have the same 'vtxCnt' as the base morph of the mesh.
};

AFX_DEFINE_STRUCT(arxMeshMorphing)
{
    afxFlags            flags;
    afxMask             mask;
    afxUnit             startMorphIdx, endMorphIdx;
    afxReal             t, tRecip;
    afxReal             posn;
};

AFX_DEFINE_STRUCT(arxMeshSection)
{
    arxMeshSectionFlags flags;
    afxUnit             mtlIdx;
    afxUnit             baseTriIdx;
    afxUnit             triCnt;
};

AFX_DEFINE_STRUCT(arxMeshBlueprint)
/// Data needed for mesh assembly
{
    afxUnit             triCnt;
    afxUnit const*      sideToAdjacentMap; // [triCnt * 3]
    afxUnit             mtlCnt;
    afxString const*    materials;
    afxUnit             secCnt;
    arxMeshSection const*sections; // [secCnt]
    afxUnit             biasCnt;
    afxString const*    biases;
    afxUnit const*      biasesForTriMap;
    afxUnit const*      triToBiasMap;

    afxUnit             vtxCnt;
    afxUnit const*      vtxToVtxMap; // [vtxCnt]
    afxUnit const*      vtxToTriMap; // [vtxCnt]
    afxUnit             attrCnt;
    afxString const*    attrs;
    afxUnit             morphCnt;
    arxMeshMorph const* morphs;
    afxString const*    morphTags;

    afxString32         urn;
    void*               udd;
};


ARX arxMesh             ArxBuildParallelepipedMesh(arxGeome morp, afxV3d whd, afxReal slantX, afxReal slantY);
ARX arxMesh             ArxBuildDomeMesh(arxGeome morp, afxReal radius, afxUnit slices);

ARX arxMesh             ArxBuildDomeMesh2(arxGeome morp, afxReal radius, afxUnit stacks, afxUnit slices, afxBool inv);
ARX arxMesh             ArxBuildSphereMesh(arxGeome morp, afxReal radius, afxUnit stacks, afxUnit slices, afxBool inv);
ARX arxMesh             ArxBuildCapsuleMesh(arxGeome morp, afxReal radius, afxReal height, afxUnit stacks, afxUnit slices, afxUnit cylinderSlices, afxBool inv);
ARX arxMesh             ArxBuildPlaneMesh(arxGeome morp, afxUnit gridSizeX, afxUnit gridSizeY, afxReal width, afxReal height);

ARX afxError            ArxBuildBoxMesh(arxGeome morp, afxV3d const whd, afxUnit secCnt, arxMesh* mesh);
ARX arxMesh             ArxBuildDiscMesh(arxGeome morp, afxReal radius, afxUnit secCnt);
ARX afxError            ArxBuildGridMesh(arxGeome morp, afxUnit rows, afxUnit layers, afxReal width, afxReal depth, arxMesh* mesh);

#endif//ARX_MESH_BLUEPRINT_H
