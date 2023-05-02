/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

#ifndef AFX_MESH_H
#define AFX_MESH_H

#include "afxCadDefs.h"
#include "afx/core/mem/afxArray.h"
#include "../../math/volume/afxAabb.h"
#include "afx/core/afxObject.h"
#include "afx/draw/res/afxIndexBuffer.h"
#include "afx/draw/res/afxVertexBuffer.h"
#include "afxMaterial.h"

// The granny_mesh is the primary structure for geometric data in Granny.
// It references vertex data, triangle data, materials, morph targets, and bone bindings.
// Standard Granny files contain an array of meshes in the granny_file_info structure, and meshes are also referenced by the granny_model objects that use them.
// Each mesh is broken into chunks along material boundaries.
// The granny_tri_material_group structure is the primary structure you iterate over to get these chunks.
// The granny_tri_material_group structure references the index array for the mesh, which in turn references the vertex array.
// If the mesh is deforming, the vertices in the vertex array reference the bones in the bone binding array.
// The vertex data for a mesh, given by a granny_vertex_data referenced by the PrimaryVertexData member, contains all the unique vertices in the mesh.

// The GrannyPNT332VertexType passed to GrannyCopyMeshVertices is an arbitrary choice - you canv pass any granny_data_type_definition that describes a vertex format, and you will get vertices in that format.

// The vertices in a Granny mesh are either deforming or rigid.
// Deforming meshes are ones that are bound to multiple bones, whereas rigid meshes are ones that are bound to a single bone (and thus move "rigidly" with that bone). 

// To determine which bones a mesh is bound to (one for rigid meshes, many for deforming meshes), you canv access the granny_bone_binding array. 
// This array contains the names of the bones to which the mesh is bound, as well as oriented bounding box parameters for the parts of the mesh that are bound to that bone and other information pertinent to the mesh-to-bone binding.
// Note that in most cases you will not need to use the bone names in the granny_bone_binding directly, because you canv use a granny_mesh_binding object to do this work (and other necessary binding work) for you.

// The index data for a mesh, given by a granny_tri_topology referenced by the PrimaryTopology member, contains all the indices for the triangles in the mesh.
// These indices always describe a triangle list - that is, each group of three indices describes a single triangle - the data is not organised into strips or fans in any way.

// Once you have vertices and indices, you basically have all the mesh data you need except for which chunks of the indices go with which materials.
// The granny_tri_material_group structure provides this information.

AFX_DEFINE_HANDLE(afxMesh);

// A serialization wrapper for the relationship between a Vertex buffer and a set of primitives.

AFX_DEFINE_STRUCT(afxMeshMorph) // aka morph target, blend shape
{
    afxFcc                  fcc; // AFX_FCC_MSHM
    afxString16     scalarName;
    afxConnection           vbuf; // afxVertexBuffer --- aka vertex data
    afxBool                 dataIsDeltas;
};

AFX_DEFINE_STRUCT(afxMeshSection) // aka tri material group
{
    afxFcc                  fcc; // AFX_FCC_MSHS
    afxString32     name; // granny hasn't this field. We implemented due to Wavefront OBJect group names.
    afxNat                  mtlIdx;
    afxAabb                 aabb;
    afxNat                  baseTriIdx;
    afxNat                  triCnt;
};

AFX_DEFINE_STRUCT(afxMeshTopology)
{
    afxFcc                  fcc; // AFX_FCC_MSHT
    afxNat                  secCnt;
    afxMeshSection          *sections; // aka tri material group
    afxConnection           ibuf; // afxIndexBuffer --- aka triangles
    afxNat                  baseIdx; // if ibuf is shared among other meshes, it should be a base for sections.
    afxNat                  idxCnt;
    afxPrimTopology         primType;

    // democratizado mas não sei o que fazer com isso k
    afxInt                  _vtx2vtxCnt;
    afxInt                  *_vtx2vtxMap;
    afxInt                  _vtx2triCnt;
    afxInt                  *_vtx2triMap;
    afxInt                  _side2neighborCnt;
    afxInt                  *_side2neighborMap;
    afxInt                  _bonesForTriCnt;
    afxInt                  *_bonesForTri;
    afxInt                  _tri2boneCnt;
    afxInt                  *_tri2boneIndices;
};

AFX_DEFINE_STRUCT(afxMeshArticulation)
{
    afxFcc                  fcc; // 
    afxString16     boneName;
    afxAabb                 aabb; // originally oobb;
    afxNat                  triCnt;
    afxNat                  *tris;
};


AFX afxString const*        AfxMeshGetName(afxMesh msh);
AFX afxVertexBuffer         AfxMeshGetVertexBuffer(afxMesh msh);
    
AFX afxNat                  AfxMeshGetMorphCount(afxMesh msh);
AFX afxMeshMorph*           AfxMeshGetMorph(afxMesh msh, afxNat morphIdx);

AFX afxNat                  AfxMeshGetSectionCount(afxMesh msh);
AFX afxMeshSection*         AfxMeshGetSection(afxMesh msh, afxNat secIdx);
AFX afxResult               AfxMeshResetSection(afxMesh msh, afxNat secIdx, afxString const *name, afxNat mtlIdx, afxNat baseTriIdx, afxNat triCnt);

AFX afxIndexBuffer          AfxMeshGetIndexBuffer(afxMesh msh);

AFX afxNat                  AfxMeshGetMaterialCount(afxMesh msh);
AFX afxNat                  AfxMeshGetMaterialIndex(afxMesh msh, afxMaterial mtl);
AFX afxMaterial             AfxMeshGetMaterial(afxMesh msh, afxNat mtlIdx);
AFX afxMaterial             AfxMeshFindMaterial(afxMesh msh, afxString const *name);
AFX afxNat                  AfxMeshFindMaterialIndex(afxMesh msh, afxString const *name);
AFX afxResult               AfxMeshResetMaterial(afxMesh msh, afxNat mtlIdx, afxMaterial mtl);

AFX afxBool                 AfxMeshIsRigid(afxMesh msh);
AFX afxNat                  AfxMeshGetArticulationCount(afxMesh msh);
AFX afxMeshArticulation*    AfxMeshGetArticulation(afxMesh msh, afxNat artIdx);
AFX afxResult               AfxMeshResetArticulation(afxMesh msh, afxNat artIdx, afxString const *name, afxNat triCnt, afxNat *tris);


#define _AFX_MSH_IMPL
#ifdef _AFX_MSH_IMPL

AFX_OBJECT(afxMesh)
{
    afxObject               obj;
    afxString32             name;

    afxConnection           vbuf; // afxVertexBuffer --- aka vertex data
    afxNat                  baseVtx; // when vbuf is shared with other meshes it's not 0. Maybe that index buffer already does this?
    afxNat                  vtxCnt;

    afxNat                  morphCnt;
    afxMeshMorph            *morphs;

    afxMeshTopology         *topology;

    afxNat                  mtlCnt;
    afxConnection           *materials; // binding

    afxNat                  artCnt;
    afxMeshArticulation     *articulations;
    afxAabb                 aabb;

    void                    *extData;
};

#endif


#endif//AFX_MESH_H