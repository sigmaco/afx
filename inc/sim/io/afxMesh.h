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

/**
    O objeto afxMesh é a estrutura primária para dado geométrico no Qwadro.
    Este referencia dados de vértice, dados de triângulo, afxMaterial's, afxMeshMorph'es e afxMeshBias's.
    Assets padrões do Qwadro contêm um arranjo de afxMesh na estrutura de informação do arquivo, e afxMesh'es são também referenciadas pelos objetos afxModel que as usam.
    Cada afxMesh é separada em afxMeshSection's delitmitadas por afxMaterial's.
    A estrutura afxMeshSection é a estrutura primária que você interage para obter estas porções.
    A estrutura afxMeshSection referencia o arranjo de índices para a afxMesh, a qual em sua vez referencia o arranjo de vértice.
    Se a afxMesh é deformável, as vértices no arranjo de vértice referenciam as articulações no arranjo de afxMeshBias's.
    Os dados de vértice para uma afxMesh, dados por um akxVertexBuffer referenciado pelo afxMesh, contêm todos os vértices únicos na afxMesh.

    As vértices no Qwadro são rígidos ou deformáveis.
    afxMesh'es deformáveis são aquelas que estão ligadas a múltiplas articulações, onde afxMesh'es rígidas são aquelas que estão ligadas a uma singela articulação (e portanto movem-se "rigidamente" com aquela articulação).

    Para determinar quais articulações uma afxMesh está ligada a (uma para afxMesh rígida, muitas para afxMesh deformável), você pode acessar o arranjo de afxMeshBias's. 
    Este arranjo contém nomes das articulações as quais a afxMesh está ligada, bem como parâmetros de "oriented bounding box" para as partes da afxMesh que estão ligadas àquela articulação e outra informação pertinente a ligação malha-a-articulação.
    Note que na maioria dos casos você não necessitará de usar os nomes das articulações no afxMeshBias diretamente, porque você pode usar um objeto asxMeshRig para fazer este trabalho (e outro trabalho necessário de ligação) para você.

    Os dados de índice para uma afxMesh, dado por um afxMesh referenciado pelo afxMesh, contém todos os índices para os triângulos na afxMesh.
    Estes índices sempre descrevem uma lista de triângulo - isso é, cada grupo de três índices descrevem um singelo triângulo - os dados não são organizados em "strips" ou "fans".

    Uma vez que você tem vértices e índices, você basicamente tem todos os dados da afxMesh que você necessita para quais porções de índice vão com quais afxMaterial's.
    A estrutura afxMeshSection provém esta informação.
*/

/*
    MESH GEOMETRY

    Mesh geometry refers to the spatial arrangement and positions of the vertices in a mesh. It deals with the actual coordinates of the vertices in 3D space, which define the shape of the object.

    Key aspects of geometry:
        Vertex Positions: The 3D coordinates (x, y, z) of each vertex in the mesh.
        Face Normals: The direction perpendicular to the surface of a face, used for lighting and rendering.
        Vertex Normals: The average direction perpendicular to the surface at a vertex, used for smooth shading.
        Texture Coordinates: Mapping of vertices to 2D texture space, which affects how textures are applied.

    Examples:
        Vertex Array: A list of vertices with their 3D coordinates.
        Normal Vectors: Vectors perpendicular to each face or vertex, used for rendering lighting effects.
        UV Mapping: Coordinates that map 2D textures to the 3D surface of the mesh.

    Geometry is concerned with the shape of the mesh. It provides the exact positions of vertices in space, the orientation of faces, and how the surface is textured.
*/

/**
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

/**
    Mesh triangle topology is described by the afxMesh structure, which is pointed to by the afxMesh. 
    The afxMesh structure provides a number of useful arrays of data, including the mesh triangulation, edge connectivity, and vertex relationships. 

    The afxMeshSection array specifies the actual triangulation of the mesh.
    Each afxMeshSection structure indexes into either the vertex index array, for the indices that make up the triangles.
    The material index indexes into the material slot array, and specifies the material used for the group of triangles. 

    The afxMesh structure also contains extra information about the mesh triangulation that can be useful in mesh processing.
    The vertex-to-tertex map has, for each vertex in the corresponding afxGeometry, an index of the next vertex which was originally the same vertex. 
    This is used to track vertices that are split during exporting, for example because of material boundaries. 
    It is a circular list, so that each vertex points to the next coincident vertex, and then the final vertex points back to the first. 
    If a vertex has no coincident vertices, then it simply points to itself. 

    The vertex-to-triangle map specifies, for each vertex, what triangle caused it to be created. 
    So, for example, for the original vertices, it is simply the first triangle to reference it in the index list. 
    But, for a vertex which was generated because of a material boundary or something similar, then it is the index of the first triangle who forced the vertex to be generated. 

    The side-to-neighbor map specifies, for each edge of a triangle, what the corresponding triangle and edge is across that edge. 
    This array lines up with the vertex index array such that, for each index, the "edge" corresponding to that index is the edge between that index and the next index in its triangle. 
    The value stored in the side-to-neighbor map is actually bit-packed. 
    The two lowest-order bits of the value are the corresponding edge index in the corresponding triangle (ie., 00 would be the 0th edge, 01 the 1st, 10 the 2nd, and 11 is undefined). 
    The rest of the bits (the high-order 30 bits) specify the actual triangle index - you would multiply it by three to get its location in the vertex index arrays. 

    The index data for a mesh, given by a afxMesh referenced by the mesh, contains all the indices for the triangles in the mesh. 
    These indices always describe a triangle list - that is, each group of three indices describes a single triangle - the data is not organised into strips or fans in any way. 
*/

/// Vertex data consists of vertex elements combined to form vertex components.
/// Vertex elements, the smallest unit of a vertex, represent entities such as position, normal, or color.
/// Vertex components are one or more vertex elements stored contiguously (interleaved per vertex) in a single memory buffer.

#ifndef ASX_MESH_H
#define ASX_MESH_H

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

typedef enum afxMeshFlag
{
    afxMeshFlag_NAMED_MORPHES   = AFX_BIT(0)
} afxMeshFlags;

typedef enum afxMeshMorphFlag
{
    // Morph geometry contains displacement data (deltas).
    afxMeshMorphFlag_DELTA = AFX_BIT(0),
    
    // Values are obtained by adding the original values to a weighted sum of the target's values.
    afxMeshMorphFlag_ADDITIVE = AFX_BIT(1),
} afxMeshMorphFlags;

typedef enum afxMeshBiasFlag
{
    afxMeshBiasFlag_NONE
} afxMeshBiasFlags;

typedef enum afxMeshSectionFlag
{
    afxMeshSectionFlag_HOLE = AFX_BIT(0)
} afxMeshSectionFlags;

typedef enum afxVertexFlag
{
    afxVertexFlag_DYNAMIC = AFX_BIT(0), // The data store contents will be modified repeatedly and used many times.
    afxVertexFlag_STREAM = AFX_BIT(1), // The data store contents will be modified once and used at most a few times.

    afxVertexFlag_POSITIONAL = AFX_BIT(2),
    afxVertexFlag_SPATIAL = AFX_BIT(3),

    afxVertexFlag_NORMALIZED = AFX_BIT(4),
    afxVertexFlag_RASTERIZATION = AFX_BIT(5),

    afxVertexFlag_ATV = AFX_BIT(10),
    // affected by affine transformations (ex.: position). Non-delta spatial attributes should receive affine transformations.

    afxVertexFlag_LTM = AFX_BIT(11),
    // affected by linear transformations (ex.: tangent, binormal). Delta spatial attributes should receive linear transformations (ex.: normal, tangent/binormal cross).

    afxVertexFlag_ILTM = AFX_BIT(12),
    // affected by inverse linear transformations. Non-delta spatial attributes should receive inverse linear transformations (ex.: normal, tangent/binormal cross).

    afxVertexFlag_DELTA = AFX_BIT(13), // treat as delta

    afxVertexFlag_RESIDENT = AFX_BIT(14) // allocation for data is resident
} afxVertexFlags;

AFX_DEFINE_STRUCT(avxVertexCache)
{
    afxLink          vbuf;
    avxVertexInput      vin;
    avxBuffer           buf;
    struct
    {
        afxUnit32       base;
        afxUnit32       range;
        afxUnit32       stride;
    }                   streams[2];
};

AFX_DEFINE_STRUCT(afxVertexBias)
{
    afxUnit             pivotIdx;
    afxReal             weight;
};

AFX_DEFINE_STRUCT(avxIndexCache)
{
    afxLink          stream;
    avxBuffer           buf;
    afxUnit32           base;
    afxUnit32           range;
    afxUnit32           stride; // idxSiz
    afxMesh             msh;
};

AFX_DEFINE_STRUCT(afxMeshSection)
{
    afxMeshSectionFlags flags;
    afxUnit             mtlIdx;
    afxUnit             baseTriIdx;
    afxUnit             triCnt;
};

AFX_DEFINE_STRUCT(afxVertexAttribute)
{
    avxFormat           fmt;
    afxVertexFlags      flags;
    afxString8          usage;
};

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

AFX_DEFINE_STRUCT(afxMeshBias)
{
    afxMeshBiasFlags    flags;
    afxUnit             triCnt;
    afxUnit*            tris; // indices to vertices
};

AFX_DEFINE_STRUCT(afxMeshInfo)
{
    avxTopology         topology;
    afxUnit             vtxCnt;
    afxUnit             attrCnt;
    afxUnit             idxCnt;
    afxUnit             triCnt;
    afxUnit             edgeCnt;
    afxUnit             secCnt;
    afxUnit             mtlCnt;
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
    afxString const*    materials;
    afxUnit             secCnt;
    afxMeshSection const*sections; // secCnt;
    afxUnit             biasCnt;
    afxString const*    biases;
    afxUnit const*      biasesForTriMap;
    afxUnit const*      triToBiasMap;

    afxUnit             vtxCnt;
    afxUnit const*      vtxToVtxMap; // vtxCnt
    afxUnit const*      vtxToTriMap; // vtxCnt
    afxUnit             attrCnt;
    afxString const*    attrs;
    afxUnit             morphCnt;
    afxMeshMorph const* morphs;
    afxString const*    morphTags;

    afxString32         urn;
    void*               udd;
};

/**
    The AfxGetMeshDrawInput() method retrieves the simulation that provides the mesh.

    @param msh must be a valid afxMesh handle.
    @return Must return an afxSimulation handle of the provider.
*/

ASX afxSimulation   AfxGetMeshContext(afxMesh msh);

ASX afxBool         AfxGetMeshUrn(afxMesh msh, afxString* id);

/**
    The AfxGetMeshMorphes() method retrieves information describing one or more mesh morphes.

    @param msh must be a valid afxMesh handle.
    @param baseMorphIdx is the number of the first/base morph to be indexed from.
    @param cnt is the number of morphes to be described. Must be greater than 0.
    @param info [out] must be a valid pointer to an array of afxMeshMorph structures.
*/

ASX afxUnit         AfxGetMeshMorphes(afxMesh msh, afxUnit baseMorphIdx, afxUnit cnt, afxMeshMorph morphes[]);

/**
    The AfxChangeMeshMorphes() method changes geometric data to one or more mesh morphes.

    @param msh must be a valid afxMesh handle.
    @param baseMorphIdx is the number of the first/base morph to be indexed from.
    @param cnt is the number of morphes to be described. Must be greater than 0.
    @param info must be a valid pointer to an array of afxMeshMorph structures.
*/

ASX afxError        AfxChangeMeshMorphes(afxMesh msh, afxUnit baseMorphIdx, afxUnit cnt, afxMeshMorph const morphes[]);

/**
    The AfxGetMeshTopology() method retrieves the shared topology of the mesh.

    @param msh must be a valid afxMesh handle.
    @return Must return a valid afxMeshTopology handle.
*/

ASX afxMeshTopology AfxGetMeshTopology(afxMesh msh);

/**
    The AfxIsMeshDeformable() method tests whether a mesh is deformable (aka skinned).
    
    A Qwadro mesh is either rigid or deforming. 
    Deforming meshes are ones that are bound to multiple bones, 
    whereas rigid meshes are ones that are bound to a single bone (and thus move "rigidly" with that bone). 

    @param msh must be a valid afxMesh handle.
    @return Must return TRUE if the mesh has more than one bias, or FALSE if has only one.
*/

ASX afxBool         AfxIsMeshDeformable(afxMesh msh);

ASX afxUnit         AfxGetMeshBiases(afxMesh msh, afxUnit baseBiasIdx, afxUnit cnt, afxMeshBias biases[]);
ASX afxString*      AfxGetMeshBiasTags(afxMesh msh, afxUnit baseBiasIdx);

ASX afxUnit*        AfxGetMeshIndices(afxMesh msh, afxUnit baseTriIdx);

/**
    The AfxDescribeMesh() method retrieves information describing the mesh.

    @param msh must be a valid afxMesh handle.
    @param info [out] must be a valid pointer to a afxMeshInfo structure.
*/

ASX void            AfxDescribeMesh(afxMesh msh, afxMeshInfo* info);

/**
    The AfxGetMeshSections() method retrieves information describing one or more mesh sections.

    @param msh must be a valid afxMesh handle.
    @param baseSecIdx is the number of the first/base surface to be indexed from.
    @param cnt is the number of sections to be described. Must be greater than 0.
    @param info [out] must be a valid pointer to an array of afxMeshSection structures.
*/

ASX afxUnit         AfxGetMeshSections(afxMesh msh, afxUnit baseSecIdx, afxUnit cnt, afxMeshSection sections[]);

/**
    The AfxResetMeshSections() method resets one or more mesh sections to new ranges.

    @param msh must be a valid afxMesh handle.
    @param baseSecIdx is the number of the first/base surface to be indexed from.
    @param cnt is the number of sections to be reset. Must be greater than 0.
    @param info [out] must be a valid pointer to an array of afxMeshSection structures containing the new values.
*/

ASX afxError        AfxResetMeshSections(afxMesh msh, afxUnit baseSecIdx, afxUnit cnt, afxMeshSection const sections[]);

/**
    The AfxInvertMeshTopology() method inverts the winding of a mesh without changing the vertex data.

    Qwadro assumes that triangle indices are ordered in counter-clockwise winding.
    This method is util in case of source data be in clockwise order, 
    or the vertices were manipulated such that they have been mirrored in some way.

    @param msh must be a valid afxMesh handle.
*/

ASX void            AfxInvertMeshTopology(afxMesh msh);

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

ASX afxUnit         AfxRemapMeshCoverage(afxMesh msh, afxUnit mtlIdxCnt, afxUnit const mtlIdxLut[]);

ASX afxString*      AfxGetMeshMaterials(afxMesh msh, afxUnit baseMtlIdx);

ASX afxError        AfxUpdateMeshIndices(afxMesh msh, afxUnit baseTriIdx, afxUnit triCnt, void const* src, afxUnit srcIdxSiz);
ASX afxError        AfxDumpMeshIndices(afxMesh msh, afxUnit baseTriIdx, afxUnit triCnt, void* dst, afxUnit dstIdxSiz);

ASX afxError        AfxUploadMeshIndices(afxMesh msh, afxUnit baseTriIdx, afxUnit triCnt, afxStream in, afxUnit srcIdxSiz);
ASX afxError        AfxDownloadMeshIndices(afxMesh msh, afxUnit baseTriIdx, afxUnit triCnt, afxStream out, afxUnit dstIdxSiz);

ASX afxError        AfxRecomputeMeshBounds(afxMesh msh, afxUnit morphIdx, afxUnit baseSecIdx, afxUnit cnt, afxUnit posAttrIdx);
ASX afxError        AfxUpdateMeshBounds(afxMesh msh, afxUnit morphIdx, afxUnit baseSecIdx, afxUnit cnt, afxBox const aabbs[]);

/**
    The AfxRecomputeMeshNormals() method computes normal vectors of vertices for a mesh morph.

    The computed normal vector at each vertex is always normalized to have unit length.

    @param msh must be a valid afxMesh handle.
    @param morphIdx must be a valid index into the mesh morphes.
    @param posAttrIdx must be a valid index into the geometry backing the specified mesh morph.
    @param nrmAttrIdx must be a valid index into the geometry backing the specified mesh morph.
    @return Must return any error ocurried, if any, or zero.
*/

ASX afxError        AfxRecomputeMeshNormals(afxMesh msh, afxUnit morphIdx, afxUnit posAttrIdx, afxUnit nrmAttrIdx);

/**
    The AfxRecomputeMeshTangents() method performs tangent frame cosputations on a mesh.

    Tangent and binormal vectors are generated.

    @param msh must be a valid afxMesh handle.
    @param morphIdx must be a valid index into the mesh morphes.
    @param posAttrIdx must be a valid index into the geometry backing the specified mesh morph.
    @param uvAttrIdx must be a valid index into the geometry backing the specified mesh morph.
    @param tanAttrIdx must be a valid index into the geometry backing the specified mesh morph or AFX_INVALID_INDEX to don't store tangent data.
    @param bitAttrIdx must be a valid index into the geometry backing the specified mesh morph or AFX_INVALID_INDEX to don't store bitangent data.
    @return Must return any error ocurried, if any, or zero.
*/

ASX afxError        AfxRecomputeMeshTangents(afxMesh msh, afxUnit morphIdx, afxUnit posAttrIdx, afxUnit uvAttrIdx, afxUnit tanAttrIdx, afxUnit bitAttrIdx);

ASX afxError        AfxBufferizeMesh(afxMesh msh, afxUnit morphIdx, avxVertexCache* vtxCache, avxIndexCache* idxCache);
ASX afxError        AfxDestripifyMesh(afxMesh msh, afxUnit baseTriIdx, afxUnit triCnt, void const* src, afxUnit srcIdxSiz);

ASX afxUnit AfxFindVertexAttributes(afxMesh msh, afxUnit cnt, afxString const usages[], afxUnit indices[]);
ASX afxError AfxDescribeVertexAttribute(afxMesh msh, afxUnit attrIdx, afxVertexAttribute* info);
ASX afxError AfxFormatVertexAttribute(afxMesh msh, afxUnit attrIdx, avxFormat fmt, afxVertexFlags flags, afxString const* usage);
ASX void* AfxAccessVertexData(afxMesh msh, afxUnit attrIdx, afxUnit morphIdx, afxUnit baseVtx);
ASX afxError AfxUpdateVertexData(afxMesh msh, afxUnit attrIdx, afxUnit morphIdx, afxUnit baseVtx, afxUnit vtxCnt, void const* src, afxUnit32 srcStride);
ASX afxError AfxUploadVertexData(afxMesh msh, afxUnit attrIdx, afxUnit morphIdx, afxUnit baseVtx, afxUnit vtxCnt, afxStream in, afxUnit stride);
ASX afxError AfxDownloadVertexData(afxMesh msh, afxUnit attrIdx, afxUnit morphIdx, afxUnit baseVtx, afxUnit vtxCnt, afxStream out, afxUnit stride);
ASX afxError AfxInvertVertexData(afxMesh msh, afxUnit attrIdx, afxUnit morphIdx, afxUnit baseVtx, afxUnit vtxCnt);
ASX afxError AfxNormalizeVertexData(afxMesh msh, afxUnit attrIdx, afxUnit morphIdx, afxUnit baseVtx, afxUnit vtxCnt);

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

ASX afxError        AfxTransformMeshes(afxM3d const ltm, afxM3d const iltm, afxReal ltTol, afxV3d const atv, afxReal atTol, afxFlags flags, afxUnit cnt, afxMesh meshes[]);

/**
    The AfxEnumerateMeshes() function enumerates some or all meshes provided by a simulation.

    @param sim must be a valid afxSimulation handle.
    @param first is the number of the first mesh to be enumerated from.
    @param cnt is the number of meshes to be enumerated. Must be greater than 0.
    @param meshes [out] must be a valid pointer to an array of afxMesh handles.
    @return Must return any error ocurried, if any, or zero.
*/

ASX afxUnit         AfxEnumerateMeshes(afxSimulation sim, afxUnit first, afxUnit cnt, afxMesh meshes[]);

/**
    The AfxCompileMeshes() function compiles new meshes using specified blueprints.

    @param sim must be a valid afxSimulation handle.
    @param first is the number of the first mesh to be enumerated from.
    @param cnt is the number of meshes to be compiled. Must be greater than 0.
    @param meshes [out] must be a valid pointer to an array of afxMesh handles.
    @return Must return any error ocurried, if any, or zero.
*/

ASX afxError        AfxCompileMeshes(afxSimulation sim, afxUnit cnt, afxMeshBlueprint const blueprints[], afxMesh meshes[]);

#endif//ASX_MESH_H
