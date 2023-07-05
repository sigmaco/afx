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
 *                               (c) 2017 Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_MESH_H
#define AFX_MESH_H

#include "afx/sim/afxSimDefs.h"
#include "afx/core/mem/afxArray.h"
#include "../../math/volume/afxAabb.h"
#include "afx/core/afxObject.h"
#include "afx/draw/res/afxIndexBuffer.h"
#include "afx/draw/res/afxVertexBuffer.h"
#include "afxMaterial.h"
#include "afx/sim/modeling/afxSkeleton.h"

/// O objeto afxMesh é a estrutura primária para dado geométrico no Qwadro.
/// Este referencia dados de vértice, dados de triângulo, afxMaterial's, afxMeshMorph'es e afxMeshArticulation's.
/// Assets padrões do Qwadro contêm um arranjo de afxMesh na estrutura de informação do arquivo, e afxMesh'es são também referenciadas pelos objetos afxModel que as usam.
/// Cada afxMesh é separada em afxMeshSection's delitmitadas por afxMaterial's.
/// A estrutura afxMeshSection é a estrutura primária que você interage para obter estas porções.
/// A estrutura afxMeshSection referencia o arranjo de índices para a afxMesh, a qual em sua vez referencia o arranjo de vértice.
/// Se a afxMesh é deformável, as vértices no arranjo de vértice referenciam as articulações no arranjo de afxMeshArticulation's.
/// Os dados de vértice para uma afxMesh, dados por um afxVertexBuffer referenciado pelo afxMesh, contêm todos os vértices únicos na afxMesh.

/// As vértices no Qwadro são rígidos ou deformáveis.
/// afxMesh'es deformáveis são aquelas que estão ligadas a múltiplas articulações, onde afxMesh'es rígidas são aquelas que estão ligadas a uma singela articulação (e portanto movem-se "rigidamente" com aquela articulação).

/// Para determinar quais articulações uma afxMesh está ligada a (uma para afxMesh rígida, muitas para afxMesh deformável), você pode acessar o arranjo de afxMeshArticulation's. 
/// Este arranjo contém nomes das articulações as quais a afxMesh está ligada, bem como parâmetros de "oriented bounding box" para as partes da afxMesh que estão ligadas àquela articulação e outra informação pertinente a ligação malha-a-articulação.
/// Note que na maioria dos casos você não necessitará de usar os nomes das articulações no afxMeshArticulation diretamente, porque você pode usar um objeto afxMeshRigging para fazer este trabalho (e outro trabalho necessário de ligação) para você.

/// Os dados de índice para uma afxMesh, dado por um afxMeshTopology referenciado pelo afxMesh, contém todos os índices para os triângulos na afxMesh.
/// Estes índices sempre descrevem uma lista de triângulo - isso é, cada grupo de três índices descrevem um singelo triângulo - os dados não são organizados em "strips" ou "fans".

/// Uma vez que você tem vértices e índices, você basicamente tem todos os dados da afxMesh que você necessita para quais porções de índice vão com quais afxMaterial's.
/// A estrutura afxMeshSection provém esta informação.

AFX_DEFINE_HANDLE(afxMesh);

/// A serialization wrapper for the relationship between a Vertex buffer and a set of primitives.

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

/// Um blueprint foi idealizado para ser um meta-objeto quase-completo para geração de uma malha. Dependendo apenas das referências das fontes de vértices e índices, para evitar alocações desnecessárias.

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

AFX_DEFINE_STRUCT(afxMeshRigging)
{
    afxMesh                 msh;
    afxNat                  boneCnt;
    afxSkeleton             srcSkl;
    afxNat*                 srcBoneIndices;
    afxSkeleton             dstSkl;
    afxNat*                 dstBoneIndices;
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

AFX afxMeshRigging*             AfxMeshGenerateRig(afxMesh msh, afxSkeleton src, afxSkeleton dst);

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