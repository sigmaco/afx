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

/// O objeto afxMesh é a estrutura primária para dado geométrico no Qwadro.
/// Este referencia dados de vértice, dados de triângulo, afxMaterial's, afxMeshMorph'es e afxMeshBias's.
/// Assets padrões do Qwadro contêm um arranjo de afxMesh na estrutura de informação do arquivo, e afxMesh'es são também referenciadas pelos objetos afxModel que as usam.
/// Cada afxMesh é separada em afxMeshSurface's delitmitadas por afxMaterial's.
/// A estrutura afxMeshSurface é a estrutura primária que você interage para obter estas porções.
/// A estrutura afxMeshSurface referencia o arranjo de índices para a afxMesh, a qual em sua vez referencia o arranjo de vértice.
/// Se a afxMesh é deformável, as vértices no arranjo de vértice referenciam as articulações no arranjo de afxMeshBias's.
/// Os dados de vértice para uma afxMesh, dados por um akxVertexBuffer referenciado pelo afxMesh, contêm todos os vértices únicos na afxMesh.

/// As vértices no Qwadro são rígidos ou deformáveis.
/// afxMesh'es deformáveis são aquelas que estão ligadas a múltiplas articulações, onde afxMesh'es rígidas são aquelas que estão ligadas a uma singela articulação (e portanto movem-se "rigidamente" com aquela articulação).

/// Para determinar quais articulações uma afxMesh está ligada a (uma para afxMesh rígida, muitas para afxMesh deformável), você pode acessar o arranjo de afxMeshBias's. 
/// Este arranjo contém nomes das articulações as quais a afxMesh está ligada, bem como parâmetros de "oriented bounding box" para as partes da afxMesh que estão ligadas àquela articulação e outra informação pertinente a ligação malha-a-articulação.
/// Note que na maioria dos casos você não necessitará de usar os nomes das articulações no afxMeshBias diretamente, porque você pode usar um objeto akxMeshRig para fazer este trabalho (e outro trabalho necessário de ligação) para você.

/// Os dados de índice para uma afxMesh, dado por um afxMesh referenciado pelo afxMesh, contém todos os índices para os triângulos na afxMesh.
/// Estes índices sempre descrevem uma lista de triângulo - isso é, cada grupo de três índices descrevem um singelo triângulo - os dados não são organizados em "strips" ou "fans".

/// Uma vez que você tem vértices e índices, você basicamente tem todos os dados da afxMesh que você necessita para quais porções de índice vão com quais afxMaterial's.
/// A estrutura afxMeshSurface provém esta informação.

/*
    MESH TOPOLOGY

    Mesh topology refers to the structure or connectivity of a mesh.
    It describes how vertices, edges, and faces are connected to form a 3D shape.
    It is more concerned with the relationships and organization of the mesh components rather than their actual positions in space.

    Key aspects of topology:
        Vertex Connectivity: How vertices are connected by edges.
        Edge Connectivity: How edges form faces and how faces are connected.
        Face Configuration: The arrangement of faces(triangles, quads, etc.) and their connectivity.
        Surface Properties: Includes properties like boundary edges, vertices' degrees (how many edges connect to a vertex), and mesh continuity.

    Examples:
        Manifold Mesh: A mesh where every edge belongs to exactly two faces.No edge should be shared by more than two faces.
        Non-Manifold Mesh: A mesh with edges shared by more than two faces or other irregular configurations.
        Polygonal Mesh: A mesh composed of polygons, often triangles or quads, defining the shape of the object.

    Topology is concerned with the structure of the mesh, such as how vertices, edges, and faces are arranged and connected. It defines the mesh's form and its geometric relationships but not the precise shape.

    Topology affects geometry indirectly. For example, changing the connectivity of vertices and faces (topology) will change the shape and structure of the 3D model (geometry).

    When modifying a mesh, adjustments to the topology might be needed to preserve certain geometric properties, and changes to geometry can affect how the topology is interpreted.
*/

/// Mesh triangle topology is described by the afxMesh structure, which is pointed to by the afxMesh. 
/// The afxMesh structure provides a number of useful arrays of data, including the mesh triangulation, edge connectivity, and vertex relationships. 

/// The afxMeshSurface array specifies the actual triangulation of the mesh. 
/// Each afxMeshSurface structure indexes into either the vertex index array, for the indices that make up the triangles.  
/// The material index indexes into the material slot array, and specifies the material used for the group of triangles. 

/// The afxMesh structure also contains extra information about the mesh triangulation that can be useful in mesh processing. 
/// The vertex-to-tertex map has, for each vertex in the corresponding afxGeometry, an index of the next vertex which was originally the same vertex. 
/// This is used to track vertices that are split during exporting, for example because of material boundaries. 
/// It is a circular list, so that each vertex points to the next coincident vertex, and then the final vertex points back to the first. 
/// If a vertex has no coincident vertices, then it simply points to itself. 

/// The vertex-to-triangle map specifies, for each vertex, what triangle caused it to be created. 
/// So, for example, for the original vertices, it is simply the first triangle to reference it in the index list. 
/// But, for a vertex which was generated because of a material boundary or something similar, then it is the index of the first triangle who forced the vertex to be generated. 

/// The side-to-neighbor map specifies, for each edge of a triangle, what the corresponding triangle and edge is across that edge. 
/// This array lines up with the vertex index array such that, for each index, the "edge" corresponding to that index is the edge between that index and the next index in its triangle. 
/// The value stored in the side-to-neighbor map is actually bit-packed. 
/// The two lowest-order bits of the value are the corresponding edge index in the corresponding triangle (ie., 00 would be the 0th edge, 01 the 1st, 10 the 2nd, and 11 is undefined). 
/// The rest of the bits (the high-order 30 bits) specify the actual triangle index - you would multiply it by three to get its location in the vertex index arrays. 

/// The index data for a mesh, given by a afxMesh referenced by the mesh, contains all the indices for the triangles in the mesh. 
/// These indices always describe a triangle list - that is, each group of three indices describes a single triangle - the data is not organised into strips or fans in any way. 

#ifndef AVX_MESH_H
#define AVX_MESH_H

#include "qwadro/inc/draw/afxDrawDefs.h"
#include "qwadro/inc/mem/afxArray.h"
#include "qwadro/inc/math/afxBox.h"
#include "qwadro/inc/base/afxObject.h"
#include "qwadro/inc/io/afxUrd.h"
#include "qwadro/inc/math/afxVertex.h"
#include "qwadro/inc/cad/afxGeometry.h"
#include "qwadro/inc/base/afxFixedString.h"
#include "qwadro/inc/cad/afxMeshBuilder.h"

typedef afxNat8 afxVertexIndex8;
typedef afxNat16 afxVertexIndex16;
typedef afxNat32 afxVertexIndex32;
typedef afxVertexIndex32 afxVertexIndex;
typedef afxNat akxFaceIndex;
typedef afxVertexIndex8 afxIndexedTriangle8[3];
typedef afxVertexIndex16 afxIndexedTriangle16[3];
typedef afxVertexIndex32 afxIndexedTriangle32[3];
typedef afxIndexedTriangle32 afxIndexedTriangle;

AFX_DEFINE_STRUCT(afxMeshSurface)
{
    afxNat              mtlIdx;
    afxNat              baseTriIdx;
    afxNat              triCnt;
    //afxBox              aabb; // added this field to ease occlusion culling.
};

AFX_DEFINE_STRUCT(afxVertexIndexCache)
{
    afxLinkage          stream;
    afxBuffer           buf;
    afxNat32            base;
    afxNat32            range;
    afxNat32            stride; // idxSiz
    afxMesh             msh;
};
AFX_DEFINE_STRUCT(afxMeshMorph) // aka morph target, blend shape
{
    afxGeometry         geo;
    afxNat              baseVtx;
    afxNat              vtxCnt;
    afxBool             delta;
};

AFX_DEFINE_STRUCT(afxMeshBias)
{
    afxBox              obb;
    afxNat              triCnt;
    afxNat*             tris; // indices to vertices
};

AFX_DEFINE_STRUCT(afxMeshBlueprint)
/// Data needed for mesh assembly
{
    afxNat              triCnt;
    afxNat const*       sideToAdjacentMap; // triCnt * 3
    afxNat              mtlCnt;
    afxNat              surfCnt;
    afxNat const*       surfToMtlMap; // surfCnt;
    afxNat const*       baseTrisForSurfMap; // surfCnt; one per surface
    afxNat const*       trisForSurfMap; // surfCnt; one per surface
    
    afxNat              biasCnt;
    afxNat const*       biasesForTriMap;
    afxNat const*       triToBiasMap;
    afxString const*    biases;

    afxGeometry         geo;
    afxNat              baseVtx;
    afxNat              vtxCnt;
    afxNat const*       vtxToVtxMap; // vtxCnt
    afxNat const*       vtxToTriMap; // vtxCnt
    afxNat              extraMorphCnt;
    afxMeshMorph const* extraMorphs;
    afxString const*    extraMorphTagMap;

    afxString32         urn;
    void*               udd;
};

AVX afxBool             AfxGetMeshUrn(afxMesh msh, afxString* id);

AVX afxNat              AfxCountMeshVertices(afxMesh msh, afxNat morphIdx, afxNat baseVtxIdx);
AVX afxBool             AfxGetMeshVertices(afxMesh msh, afxNat morphIdx, afxGeometry* data, afxNat* baseVtx, afxNat* vtxCnt);

AVX afxNat              AfxCountMeshMorphs(afxMesh msh, afxNat baseMorphIdx);
AVX afxMeshMorph*       AfxGetMeshMorphs(afxMesh msh, afxNat baseMorphIdx);

AVX avxTopology         AfxGetMeshTopology(afxMesh msh);

AVX afxBool             AfxMeshIsDeformable(afxMesh msh);

AVX afxNat              AfxCountMeshBiases(afxMesh msh, afxNat baseBiasIdx);
AVX afxMeshBias*        AfxGetMeshBiases(afxMesh msh, afxNat baseBiasIdx);
AVX afxString*          AfxGetMeshBiasTags(afxMesh msh, afxNat baseBiasIdx);

AVX afxNat              AfxCountMeshIndices(afxMesh msh, afxNat baseIdx);

AVX afxNat*             AfxGetMeshIndices(afxMesh msh, afxNat baseIdx);
AVX afxIndexedTriangle* AfxGetMeshTriangles(afxMesh msh, afxNat baseTriIdx);

AVX afxNat              AfxCountMeshTriangles(afxMesh msh, afxNat baseTriIdx);

AVX afxNat              AfxCountMeshTriangleEdges(afxMesh msh, afxNat baseEdgeIdx);

AVX afxNat              AfxCountMeshSurfaces(afxMesh msh, afxNat baseSurfIdx);
AVX afxMeshSurface*     AfxGetMeshSurface(afxMesh msh, afxNat surIdx);

AVX afxBool             AfxDescribeMeshCoverage(afxMesh msh, afxNat surIdx, afxMeshSurface* desc);

/// By default, Qwadro always write triangle indices in counter-clockwise winding. 
/// If you need your indices in clockwise order, or if you've manipulated your mesh vertices such that they've been mirrored in some way, 
/// you can always invert the winding of a afxMesh's indices like this: 

AVX void                AfxInvertMeshWinding(afxMesh msh);

/// If you are merging multiple topologies, or processing material groups, sometimes it can be useful to remap the material indices in a afxMeshSurface. 
/// For each material index in the afxMesh, it will lookup that index in the remapTable array and replace material index with the value. 

AVX void                AfxRemapMeshCoverage(afxMesh msh, afxNat remapCnt, afxNat const remapTable[]);


AVX afxError            AfxUpdateMeshIndices(afxMesh msh, afxNat baseTriIdx, afxNat triCnt, void const* src, afxNat srcIdxSiz);

AVX afxNat              AfxDetermineMeshIndexSize(afxMesh msh);

////////////////////////////////////////////////////////////////////////////////

AVX afxError            AfxAssembleMeshes(afxDrawInput din, afxNat cnt, afxMeshBlueprint const blueprints[], afxMesh meshes[]);

AVX afxNat              AfxEnumerateMeshes(afxDrawInput din, afxNat base, afxNat cnt, afxMesh msh[]);

AVX afxError            AfxBuildMeshes(afxDrawInput din, afxNat cnt, afxMeshBuilder const mshb[], afxMesh meshes[]);

AVX void                AfxTransformMeshes(afxM3d const ltm, afxM3d const iltm, afxReal ltTol, afxV3d const atv, afxReal atTol, afxFlags flags, afxNat cnt, afxMesh meshes[]);

AVX void                AfxRenormalizeMeshes(afxNat cnt, afxMesh meshes[]);

#endif//AVX_MESH_H
