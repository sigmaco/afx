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

AFX_DEFINE_STRUCT(afxMeshBlueprintArticulation)
{
    afxString*              boneName; // 32
    afxNat                  triCnt;
    void const              *tris;
};

AFX_DEFINE_STRUCT(afxMeshBlueprintSection) // aka tri material group
{
    afxString*              name; // 32
    afxNat                  mtlIdx;
    afxNat                  firstTriIdx;
    afxNat                  triCnt;

    void const              *idxSrc;
    afxNat                  srcIdxSiz;
};

AFX_DEFINE_STRUCT(afxMeshBlueprintVertexArrange)
{
    afxString*              semantic; // 8
    afxVertexFormat         fmt;
    afxVertexUsage          usage;

    void const              *src;
    afxVertexFormat         srcFmt;
};

// Um blueprint foi idealizado para ser um meta-objeto quase-completo para geração de uma malha. Dependendo apenas das referências das fontes de vértices e índices, para evitar alocações desnecessárias.

AFX_DEFINE_STRUCT(afxMeshBlueprint)
{
    afxFcc                  fcc;
    void*                   sim;
    afxUri128               uri;

    // vertex data
    afxNat                  vtxCnt; // must be greater than zero.
    afxArray                arranges; // afxMeshBlueprintVertexArrange

    // topology data
    afxNat                  idxCnt;
    void const              *idxSrc;
    afxNat                  srcIdxSiz;
    afxPrimTopology         prim;
    afxArray                sections; // afxMeshBlueprintSection

    // rigging data
    afxArray                articulations; // afxMeshBlueprintArticulation

    // material data
    afxArray                materials; // afxUri
};

#if 0
struct mesh_builder
{
    float NormalTolerance;
    float TangentTolerance;
    float BinormalTolerance;
    float TangentBinormalCrossTolerance;
    float TangentMergingMinCosine;
    float ChannelTolerance[256];
    const data_type_definition *VertexType;
    int VertexSize;
    int ChannelCount;
    int MaximumVertexCount;
    int VertexCount;
    stack_allocator VertexStack;
    stack_allocator NextCoincidentVertStack;
    stack_allocator FromTriangleStack;
    float Point[3];
    int BoneCount;
    float *BoneWeights;
    int WeightCount;
    vertex_weight_arrays WeightArrays;
    stack_allocator VertexIndexStack;
    stack_allocator NextInMaterialStack;
    int MaterialCount;
    int UsedMaterialCount;
    int *FirstMaterialTriangle;
    stack_allocator EdgeStack;
    int *EdgeHashHeads;
    int NextUnusedTriangle;
    int MaterialIndex;
    int VertexIndex[3];
    data_type_definition *BufferedVertexType;
    data_type_definition *ChannelTypes;
    int BufferedPrefixSize;
    int BufferedChannelSize;
    int BufferedVertexSize;
    buffered_vertex_prefix *BufferedVertex[3];
    buffered_vertex_prefix *ComparisonVertex;
    void *TruncatedVertex;
    stack_allocator TriangleTangentStack;
    stack_allocator VertexTangentStack;
    int VertexComponentNameCount;
    char **VertexComponentNames;
}
#endif

AFX_DEFINE_STRUCT(afxMeshMorph) // aka morph target, blend shape
{
    afxFcc                  fcc;
    afxString*              scalarName; // 16
    afxVertexBuffer         vbuf; // afxVertexBuffer --- aka vertex data
    afxNat                  baseVtx;
    afxNat                  vtxCnt;
    afxBool                 dataIsDeltas;
};

AFX_DEFINE_STRUCT(afxMeshSection) // aka tri material group
{
    afxFcc                  fcc;
    afxNat                  mtlIdx;
    afxNat                  firstTriIdx;
    afxNat                  triCnt;
    afxAabb                 aabb; // SIGMA added this field to ease occlusion culling.
    afxString*              name; // 32 // SIGMA added this field to handle Wavefront OBJect group names.
};

AFX_DEFINE_STRUCT(afxMeshTopology)
{
    afxFcc                  fcc;
    afxNat                  secCnt;
    afxMeshSection          *sections; // aka tri material group
    afxAabb                 aabb; // SIGMA added this field to ease occlusion culling.
    afxIndexBuffer          ibuf; // afxIndexBuffer --- aka triangles
    afxNat                  idxRgn; // if ibuf is shared among other meshes, it should be a base for sections.    
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
    afxFcc                  fcc;
    afxString*              boneName; // 16
    afxAabb                 aabb; // originally oobb;
    afxNat                  triCnt;
    afxNat                  *triIndices;
};

AFX_DEFINE_STRUCT(afxMaterialSlot)
{
    afxMaterial             mtl;
};

AFX_OBJECT(afxMesh)
{
    afxObject               obj;
    afxUri*                 uri; // 128

    afxVertexBuffer         vbuf; // afxVertexBuffer --- aka vertex data
    afxNat                  baseVtx; // when vbuf is shared with other meshes it's not 0. Maybe that index buffer already does this?
    afxNat                  vtxCnt;
    afxNat                  morphCnt;
    afxMeshMorph            *morphs;
    afxMeshTopology         *topology;
    afxNat                  mtlSlotCnt;
    afxMaterialSlot         *mtlSlots;
    afxNat                  articulationCnt;
    afxMeshArticulation     *articulations;
    void                    *extData;
};

AFX afxUri const*           AfxMeshGetUri(afxMesh msh);
AFX afxVertexBuffer         AfxMeshGetVertexBuffer(afxMesh msh);
    
AFX afxNat                  AfxMeshGetMorphCount(afxMesh msh);
AFX afxMeshMorph*           AfxMeshGetMorph(afxMesh msh, afxNat morphIdx);

AFX afxNat                  AfxMeshGetSectionCount(afxMesh msh);
AFX afxMeshSection*         AfxMeshGetSection(afxMesh msh, afxNat secIdx);

AFX afxIndexBuffer          AfxMeshGetIndexBuffer(afxMesh msh);

AFX afxNat                  AfxMeshGetMaterialCount(afxMesh msh);
AFX afxMaterial             AfxMeshGetBoundMaterial(afxMesh msh, afxNat mtlIdx);
AFX afxMaterial             AfxMeshFindMaterial(afxMesh msh, afxUri const *name);
AFX void                    AfxMeshBindMaterial(afxMesh msh, afxNat mtlIdx, afxMaterial mtl);

AFX afxBool                 AfxMeshIsRigid(afxMesh msh);
AFX afxNat                  AfxMeshGetArticulationCount(afxMesh msh);
AFX afxMeshArticulation*    AfxMeshGetArticulation(afxMesh msh, afxNat artIdx);


AFX void                    AfxMeshTransform(afxMesh msh, afxV3d const affine, afxM3d const linear, afxM3d const invLinear, afxReal affineTol, afxReal linearTol, afxFlags flags);

////////////////////////////////////////////////////////////////////////////////
// BLUEPRINT                                                                  //
////////////////////////////////////////////////////////////////////////////////

// Inicializa uma blueprint, e reserva recursos.
AFX void                    AfxMeshBlueprintDeploy(afxMeshBlueprint *mshb, void *sim, afxUri const *name, afxNat vtxCnt, afxNat estVtxArrCnt, afxNat estSecCnt, afxNat estArtCnt, afxNat estMtlCnt);

// Retira de uso uma blueprint, e libera recursos.
AFX void                    AfxMeshBlueprintDiscard(afxMeshBlueprint *mshb);

AFX void                    AfxMeshBlueprintErase(afxMeshBlueprint *mshb);

// Buscar por um nomeado material se já inserido.
AFX afxBool                 AfxMeshBlueprintFindMaterial(afxMeshBlueprint *mshb, afxUri const *uri, afxNat *idx);

// Adiciona numa nova articulação a malha. Ao menos uma deve existir.
AFX afxError                AfxMeshBlueprintAddArticulation(afxMeshBlueprint *mshb, afxString const *name, afxNat triCnt, void const *tris);

// Adiciona um novo material a malha.
AFX afxError                AfxMeshBlueprintAddMaterial(afxMeshBlueprint *mshb, afxMaterial mtl);

// Adiciona numa nova seção a malha. Ao menos uma deve existir.
AFX afxError                AfxMeshBlueprintAddSection(afxMeshBlueprint *mshb, afxString const *name, afxNat mtlIdx, afxNat firstTriIdx, afxNat triCnt, void const *idxSrc, afxNat idxSrcSiz);

// Define dados de topologia da malha. Se um 'ibuf' não for oferecido, um novo será gerado com a capacidade de 'idxCnt'. Noutro caso, 'baseIdx' deve ser usado para delimitar o início do espaço utilizável num 'ibuf' compartilhado.
AFX void                    AfxMeshBlueprintSetTopology(afxMeshBlueprint *mshb, afxPrimTopology prim, afxNat idxCnt, void const *idxSrc, afxNat idxSrcSiz);

// Adiciona um novo arranjo de vértice a malha. Ao menos uma, especificando posição, é necessária.
AFX afxError                AfxMeshBlueprintAddVertexArrange(afxMeshBlueprint *mshb, afxVertexRowSpecification const *spec);

#endif//AFX_MESH_H