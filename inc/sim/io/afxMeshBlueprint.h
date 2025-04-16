/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *        Q W A D R O   V I D E O   G R A P H I C S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#ifndef ASX_MESH_BLUEPRINT_H
#define ASX_MESH_BLUEPRINT_H

#include "qwadro/inc/sim/afxSimDefs.h"
#include "qwadro/inc/draw/afxDrawDefs.h"
#include "qwadro/inc/mem/afxArray.h"
#include "qwadro/inc/math/bound/afxBox.h"
#include "qwadro/inc/base/afxObject.h"
#include "qwadro/inc/io/afxUrd.h"
#include "qwadro/inc/math/afxVertex.h"
#include "qwadro/inc/draw/io/afxVertexStream.h"
#include "qwadro/inc/base/afxFixedString.h"
#include "qwadro/inc/sim/io/afxTriangulation.h"

#define ASX_INDICES_PER_TRI 3
#define AFX_MAX_VERTEX_ATTRIBUTES 32

typedef afxIndex8 afxTriangle8[3];
typedef afxIndex16 afxTriangle16[3];
typedef afxIndex32 afxTriangle32[3];

typedef enum afxMeshSectionFlag
{
    afxMeshSectionFlag_HOLE = AFX_BIT(0)
} afxMeshSectionFlags;

typedef enum afxMeshMorphFlag
{
    // Morph geometry contains displacement data (deltas).
    afxMeshMorphFlag_DELTA = AFX_BIT(0),

    // Values are obtained by adding the original values to a weighted sum of the target's values.
    afxMeshMorphFlag_ADDITIVE = AFX_BIT(1),
} afxMeshMorphFlags;

/**
    The morph targets are a extension to the afxMesh structure.
    For each morph target attribute, an original attribute MUST be present in the mesh.
    A implementation SHOULD support at least POSITION, NORMAL and TANGENT attributes for morphing.
    All morphed data MUST have the same 'vtxCnt' as the base morph of the mesh.
*/

AFX_DEFINE_STRUCT(afxMeshMorph)
// aka morph target, blend shape
{
    afxMeshMorphFlags   flags;
    // For each morph target attribute, an original attribute MUST be present in the mesh.
    // Implementations SHOULD support at least 8 morphed attributes.
    afxUnit             morphAttrCnt;
    afxMask             morphedAttrs;
    afxUnit             baseVtx;
    // All morphed data MUST have the same 'vtxCnt' as the base morph of the mesh.
};

AFX_DEFINE_STRUCT(afxMeshSection)
{
    afxMeshSectionFlags flags;
    afxUnit             mtlIdx;
    afxUnit             baseTriIdx;
    afxUnit             triCnt;
};

AFX_DEFINE_STRUCT(afxMeshBlueprint)
/// Data needed for mesh assembly
{
    afxUnit             triCnt;
    afxUnit const*      sideToAdjacentMap; // [triCnt * 3]
    afxUnit             mtlCnt;
    afxString const*    materials;
    afxUnit             secCnt;
    afxMeshSection const*sections; // [secCnt]
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
    afxMeshMorph const* morphs;
    afxString const*    morphTags;

    afxString32         urn;
    void*               udd;
};


ASX afxMesh             AfxBuildParallelepipedMesh(afxSimulation sim, afxV3d whd, afxReal slantX, afxReal slantY);
ASX afxMesh             AfxBuildDomeMesh(afxSimulation sim, afxReal radius, afxUnit slices);

ASX afxMesh             AfxBuildDomeMesh2(afxSimulation sim, afxReal radius, afxUnit stacks, afxUnit slices, afxBool inv);
ASX afxMesh             AfxBuildSphereMesh(afxSimulation sim, afxReal radius, afxUnit stacks, afxUnit slices, afxBool inv);
ASX afxMesh             AfxBuildCapsuleMesh(afxSimulation sim, afxReal radius, afxReal height, afxUnit stacks, afxUnit slices, afxUnit cylinderSlices, afxBool inv);
ASX afxMesh             AfxBuildPlaneMesh(afxSimulation sim, afxUnit gridSizeX, afxUnit gridSizeY, afxReal width, afxReal height);

ASX afxError            AfxBuildBoxMesh(afxSimulation sim, afxV3d const whd, afxUnit secCnt, afxMesh* mesh);
ASX afxMesh             AfxBuildDiscMesh(afxSimulation sim, afxReal radius, afxUnit secCnt);
ASX afxError            AfxBuildGridMesh(afxSimulation sim, afxUnit rows, afxUnit layers, afxReal width, afxReal depth, afxMesh* mesh);

#endif//ASX_MESH_BLUEPRINT_H
