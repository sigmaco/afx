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
/// Cada afxMesh é separada em afxMeshSection's delitmitadas por afxMaterial's.
/// A estrutura afxMeshSection é a estrutura primária que você interage para obter estas porções.
/// A estrutura afxMeshSection referencia o arranjo de índices para a afxMesh, a qual em sua vez referencia o arranjo de vértice.
/// Se a afxMesh é deformável, as vértices no arranjo de vértice referenciam as articulações no arranjo de afxMeshBias's.
/// Os dados de vértice para uma afxMesh, dados por um akxVertexBuffer referenciado pelo afxMesh, contêm todos os vértices únicos na afxMesh.

/// As vértices no Qwadro são rígidos ou deformáveis.
/// afxMesh'es deformáveis são aquelas que estão ligadas a múltiplas articulações, onde afxMesh'es rígidas são aquelas que estão ligadas a uma singela articulação (e portanto movem-se "rigidamente" com aquela articulação).

/// Para determinar quais articulações uma afxMesh está ligada a (uma para afxMesh rígida, muitas para afxMesh deformável), você pode acessar o arranjo de afxMeshBias's. 
/// Este arranjo contém nomes das articulações as quais a afxMesh está ligada, bem como parâmetros de "oriented bounding box" para as partes da afxMesh que estão ligadas àquela articulação e outra informação pertinente a ligação malha-a-articulação.
/// Note que na maioria dos casos você não necessitará de usar os nomes das articulações no afxMeshBias diretamente, porque você pode usar um objeto afxMeshRig para fazer este trabalho (e outro trabalho necessário de ligação) para você.

/// Os dados de índice para uma afxMesh, dado por um afxMesh referenciado pelo afxMesh, contém todos os índices para os triângulos na afxMesh.
/// Estes índices sempre descrevem uma lista de triângulo - isso é, cada grupo de três índices descrevem um singelo triângulo - os dados não são organizados em "strips" ou "fans".

/// Uma vez que você tem vértices e índices, você basicamente tem todos os dados da afxMesh que você necessita para quais porções de índice vão com quais afxMaterial's.
/// A estrutura afxMeshSection provém esta informação.

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

/// The afxMeshSection array specifies the actual triangulation of the mesh. 
/// Each afxMeshSection structure indexes into either the vertex index array, for the indices that make up the triangles.  
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
#include "qwadro/inc/cad/afxTriangulation.h"

#define AMX_INDICES_PER_TRI 3

typedef afxIndex8 afxTriangle8[3];
typedef afxIndex16 afxTriangle16[3];
typedef afxIndex32 afxTriangle32[3];

typedef enum afxMeshFlag
{
    afxMeshFlag_NONE
} afxMeshFlags;

typedef enum afxMeshMorphFlag
{
    afxMeshMorphFlag_NONE
} afxMeshMorphFlags;

typedef enum afxMeshBiasFlag
{
    afxMeshBiasFlag_NONE
} afxMeshBiasFlags;

typedef enum afxMeshSurfaceFlag
{
    afxMeshSurfaceFlag_HOLE = AFX_BIT(0)
} afxMeshSurfaceFlags;

AFX_DEFINE_STRUCT(avxIndexCache)
{
    afxLinkage          stream;
    afxBuffer           buf;
    afxUnit32           base;
    afxUnit32           range;
    afxUnit32           stride; // idxSiz
    afxMesh             msh;
};

AFX_DEFINE_STRUCT(afxMeshSection)
{
    afxUnit             mtlIdx;
    afxUnit             baseTriIdx;
    afxUnit             triCnt;
    afxMeshSurfaceFlags flags;
};

AFX_DEFINE_STRUCT(afxMeshMorph)
// aka morph target, blend shape
{
    afxGeometry         geo;
    afxUnit             baseVtx;
    afxUnit             vtxCnt;
    afxBool             delta;
    afxMeshMorphFlags   flags;
};

AFX_DEFINE_STRUCT(afxMeshBias)
{
    afxUnit             triCnt;
    afxUnit*            tris; // indices to vertices
    afxMeshBiasFlags    flags;
};

AFX_DEFINE_STRUCT(afxMeshInfo)
{
    avxTopology         topology;
    afxUnit             idxCnt;
    afxUnit             triCnt;
    afxUnit             edgeCnt;
    afxUnit             secCnt;
    afxUnit             biasCnt;
    afxUnit             morphCnt;
    afxUnit             minIdxSiz;
    afxString32         urn;
    void*               udd;
};

AFX_DEFINE_STRUCT(afxMeshBlueprint)
/// Data needed for mesh assembly
{
    afxUnit             triCnt;
    afxUnit const*      sideToAdjacentMap; // triCnt * 3
    afxUnit             mtlCnt;
    afxUnit             secCnt;
    afxUnit const*      surfToMtlMap; // secCnt;
    afxUnit const*      baseTrisForSurfMap; // secCnt; one per surface
    afxUnit const*      trisForSurfMap; // secCnt; one per surface
    
    afxUnit             biasCnt;
    afxUnit const*      biasesForTriMap;
    afxUnit const*      triToBiasMap;
    afxString const*    biases;

    afxGeometry         geo;
    afxUnit             baseVtx;
    afxUnit             vtxCnt;
    afxUnit const*      vtxToTriMap; // vtxCnt
    afxUnit             extraMorphCnt;
    afxMeshMorph const* extraMorphs;
    afxString const*    extraMorphTagMap;

    afxString32         urn;
    void*               udd;
};

/**
    The AfxGetMeshDrawInput() method retrieves the draw input mechanism that provides the mesh.

    @param msh must be a valid afxMesh handle.
    @return Must return an afxDrawInput handle of the provider.
*/

AVX afxDrawInput    AfxGetMeshDrawInput(afxMesh msh);

AVX afxBool         AfxGetMeshUrn(afxMesh msh, afxString* id);

/**
    The AfxGetMeshMorphes() method retrieves information describing one or more mesh morphes.

    @param msh must be a valid afxMesh handle.
    @param baseMorphIdx is the number of the first/base morph to be indexed from.
    @param cnt is the number of morphes to be described. Must be greater than 0.
    @param info [out] must be a valid pointer to an array of afxMeshMorph structures.
*/

AVX afxUnit         AfxGetMeshMorphes(afxMesh msh, afxUnit baseMorphIdx, afxUnit cnt, afxMeshMorph morphes[]);

/**
    The AfxChangeMeshMorphes() method changes geometric data to one or more mesh morphes.

    @param msh must be a valid afxMesh handle.
    @param baseMorphIdx is the number of the first/base morph to be indexed from.
    @param cnt is the number of morphes to be described. Must be greater than 0.
    @param info must be a valid pointer to an array of afxMeshMorph structures.
*/

AVX afxError        AfxChangeMeshMorphes(afxMesh msh, afxUnit baseMorphIdx, afxUnit cnt, afxMeshMorph const morphes[]);

/**
    The AfxGetMeshTopology() method retrieves the primitive topology of faces in the mesh.

    @param msh must be a valid afxMesh handle.
    @return Must return a valid avxTopology value.
*/

AVX avxTopology     AfxGetMeshTopology(afxMesh msh);

/**
    The AfxIsMeshDeformable() method tests whether a mesh is deformable (aka skinned).
    
    A Qwadro mesh is either rigid or deforming. 
    Deforming meshes are ones that are bound to multiple bones, 
    whereas rigid meshes are ones that are bound to a single bone (and thus move "rigidly" with that bone). 

    @param msh must be a valid afxMesh handle.
    @return Must return TRUE if the mesh has more than one bias, or FALSE if has only one.
*/

AVX afxBool         AfxIsMeshDeformable(afxMesh msh);

AVX afxUnit         AfxGetMeshBiases(afxMesh msh, afxUnit baseBiasIdx, afxUnit cnt, afxMeshBias biases[]);
AVX afxString*      AfxGetMeshBiasTags(afxMesh msh, afxUnit baseBiasIdx);

AVX afxUnit*        AfxGetMeshIndices(afxMesh msh, afxUnit baseTriIdx);

/**
    The AfxDescribeMesh() method retrieves information describing the mesh.

    @param msh must be a valid afxMesh handle.
    @param info [out] must be a valid pointer to a afxMeshInfo structure.
*/

AVX void            AfxDescribeMesh(afxMesh msh, afxMeshInfo* info);

/**
    The AfxGetMeshSections() method retrieves information describing one or more mesh sections.

    @param msh must be a valid afxMesh handle.
    @param baseSecIdx is the number of the first/base surface to be indexed from.
    @param cnt is the number of sections to be described. Must be greater than 0.
    @param info [out] must be a valid pointer to an array of afxMeshInfo structures.
*/

AVX afxUnit         AfxGetMeshSections(afxMesh msh, afxUnit baseSecIdx, afxUnit cnt, afxMeshSection sections[]);

/**
    The AfxInvertMeshTopology() method inverts the winding of a mesh without changing the vertex data.

    Qwadro assumes that triangle indices are ordered in counter-clockwise winding.
    This method is util in case of source data be in clockwise order, 
    or the vertices were manipulated such that they have been mirrored in some way.

    @param msh must be a valid afxMesh handle.
*/

AVX void            AfxInvertMeshTopology(afxMesh msh);

/**
    The AfxRemapMeshCoverage() method replaces material indices across mesh sections.

    It can be useful to rearrange material indices across mesh sections after modifying a mesh.
    This method will iterate for each mesh surface, look for its material index in specified table,
    and replace material index with the value.

    @param msh must be a valid afxMesh handle.
    @param mtlIdxCnt is the number of material indices in the lookup table.
    @param mtlIdxLut must be a array of indices mapping actual material indices into new ones.
    @return Must return the number of material indices affectively replaced.
*/

AVX afxUnit         AfxRemapMeshCoverage(afxMesh msh, afxUnit mtlIdxCnt, afxUnit const mtlIdxLut[]);

AVX afxError        AfxUpdateMeshIndices(afxMesh msh, afxUnit baseTriIdx, afxUnit triCnt, void const* src, afxUnit srcIdxSiz);
AVX afxError        AfxDumpMeshIndices(afxMesh msh, afxUnit baseTriIdx, afxUnit triCnt, void* dst, afxUnit dstIdxSiz);

AVX afxError        AfxUploadMeshIndices(afxMesh msh, afxUnit baseTriIdx, afxUnit triCnt, afxStream in, afxUnit srcIdxSiz);
AVX afxError        AfxDownloadMeshIndices(afxMesh msh, afxUnit baseTriIdx, afxUnit triCnt, afxStream out, afxUnit dstIdxSiz);

AVX afxError        AfxRecomputeMeshBounds(afxMesh msh, afxUnit morphIdx, afxUnit baseSecIdx, afxUnit cnt, afxUnit posAttrIdx);
AVX afxError        AfxUpdateMeshBounds(afxMesh msh, afxUnit morphIdx, afxUnit baseSecIdx, afxUnit cnt, afxBox const aabbs[]);

/**
    The AfxRecomputeMeshNormals() method computes normal vectors of vertices for a mesh morph.

    The computed normal vector at each vertex is always normalized to have unit length.

    @param msh must be a valid afxMesh handle.
    @param morphIdx must be a valid index into the mesh morphes.
    @param posAttrIdx must be a valid index into the geometry backing the specified mesh morph.
    @param nrmAttrIdx must be a valid index into the geometry backing the specified mesh morph.
    @return Must return any error ocurried, if any, or zero.
*/

AVX afxError        AfxRecomputeMeshNormals(afxMesh msh, afxUnit morphIdx, afxUnit posAttrIdx, afxUnit nrmAttrIdx);

/**
    The AfxRecomputeMeshTangents() method performs tangent frame computations on a mesh.

    Tangent and binormal vectors are generated.

    @param msh must be a valid afxMesh handle.
    @param morphIdx must be a valid index into the mesh morphes.
    @param posAttrIdx must be a valid index into the geometry backing the specified mesh morph.
    @param uvAttrIdx must be a valid index into the geometry backing the specified mesh morph.
    @param tanAttrIdx must be a valid index into the geometry backing the specified mesh morph or AFX_INVALID_INDEX to don't store tangent data.
    @param bitAttrIdx must be a valid index into the geometry backing the specified mesh morph or AFX_INVALID_INDEX to don't store bitangent data.
    @return Must return any error ocurried, if any, or zero.
*/

AVX afxError        AfxRecomputeMeshTangents(afxMesh msh, afxUnit morphIdx, afxUnit posAttrIdx, afxUnit uvAttrIdx, afxUnit tanAttrIdx, afxUnit bitAttrIdx);

AVX afxError        AfxBufferizeMesh(afxMesh msh, afxUnit morphIdx, avxVertexCache* vtxCache, avxIndexCache* idxCache);
AVX afxError        AfxDestripifyMesh(afxMesh msh, afxUnit baseTriIdx, afxUnit triCnt, void const* src, afxUnit srcIdxSiz);

////////////////////////////////////////////////////////////////////////////////

/**
    The AfxTransformMeshes() function similarity transforms an array of meshes.

    @param ltm is the 3x3 linear transformation to perform.
    @param iltm is the 3x3 inverse linear transformation to perform.
    @param ltTol is the tolerance factor for linear transformations.
    @param atv is the translation to perform.
    @param atTol is the tolerance factor for affine transformations.
    @param cnt is the number of meshes to be transformed.
    @param meshes must be a valid pointer to an array of afxMesh handles.
    @return Must return any error ocurried, if any, or zero.
*/

AVX afxError        AfxTransformMeshes(afxM3d const ltm, afxM3d const iltm, afxReal ltTol, afxV3d const atv, afxReal atTol, afxFlags flags, afxUnit cnt, afxMesh meshes[]);

/**
    The AfxEnumerateMeshes() function enumerates some or all meshes provided by a draw input mechanism.

    @param din must be a valid afxDrawInput handle.
    @param first is the number of the first mesh to be enumerated from.
    @param cnt is the number of meshes to be enumerated. Must be greater than 0.
    @param meshes [out] must be a valid pointer to an array of afxMesh handles.
    @return Must return any error ocurried, if any, or zero.
*/

AVX afxUnit         AfxEnumerateMeshes(afxDrawInput din, afxUnit first, afxUnit cnt, afxMesh meshes[]);

/**
    The AfxAssembleMeshes() function assembles new meshes using specified blueprints.

    @param din must be a valid afxDrawInput handle.
    @param first is the number of the first mesh to be enumerated from.
    @param cnt is the number of meshes to be assembled. Must be greater than 0.
    @param meshes [out] must be a valid pointer to an array of afxMesh handles.
    @return Must return any error ocurried, if any, or zero.
*/

AVX afxError        AfxAssembleMeshes(afxDrawInput din, afxUnit cnt, afxMeshBlueprint const blueprints[], afxMesh meshes[]);

AVX afxError        AfxBuildMeshes(afxDrawInput din, afxUnit cnt, afxTriangulation const mshb[], afxMesh meshes[]);

#endif//AVX_MESH_H
