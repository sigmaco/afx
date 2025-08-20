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

// Morphable topogical mesh, a deformable mesh with fixed topology shared among shapes.

#ifndef ARX_MESH_H
#define ARX_MESH_H

/**
    O objeto arxMesh é a estrutura primária para dado geométrico no Qwadro.
    Este referencia dados de vértice, dados de triângulo, arxMaterial's, arxMeshMorph'es e arxMeshBias's.
    Assets padrões do Qwadro contêm um arranjo de arxMesh na estrutura de informação do arquivo, e arxMesh'es são também referenciadas pelos objetos arxModel que as usam.
    Cada arxMesh é separada em arxMeshSection's delitmitadas por arxMaterial's.
    A estrutura arxMeshSection é a estrutura primária que você interage para obter estas porções.
    A estrutura arxMeshSection referencia o arranjo de índices para a arxMesh, a qual em sua vez referencia o arranjo de vértice.
    Se a arxMesh é deformável, as vértices no arranjo de vértice referenciam as articulações no arranjo de arxMeshBias's.
    Os dados de vértice para uma arxMesh, dados por um akxVertexBuffer referenciado pelo arxMesh, contêm todos os vértices únicos na arxMesh.

    As vértices no Qwadro são rígidos ou deformáveis.
    arxMesh'es deformáveis são aquelas que estão ligadas a múltiplas articulações, onde arxMesh'es rígidas são aquelas que estão ligadas a uma singela articulação (e portanto movem-se "rigidamente" com aquela articulação).

    Para determinar quais articulações uma arxMesh está ligada a (uma para arxMesh rígida, muitas para arxMesh deformável), você pode acessar o arranjo de arxMeshBias's. 
    Este arranjo contém nomes das articulações as quais a arxMesh está ligada, bem como parâmetros de "oriented bounding box" para as partes da arxMesh que estão ligadas àquela articulação e outra informação pertinente a ligação malha-a-articulação.
    Note que na maioria dos casos você não necessitará de usar os nomes das articulações no arxMeshBias diretamente, porque você pode usar um objeto _arxRiggedMesh para fazer este trabalho (e outro trabalho necessário de ligação) para você.

    Os dados de índice para uma arxMesh, dado por um arxMesh referenciado pelo arxMesh, contém todos os índices para os triângulos na arxMesh.
    Estes índices sempre descrevem uma lista de triângulo - isso é, cada grupo de três índices descrevem um singelo triângulo - os dados não são organizados em "strips" ou "fans".

    Uma vez que você tem vértices e índices, você basicamente tem todos os dados da arxMesh que você necessita para quais porções de índice vão com quais arxMaterial's.
    A estrutura arxMeshSection provém esta informação.
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
    Mesh triangle topology is described by the arxMesh structure, which is pointed to by the arxMesh. 
    The arxMesh structure provides a number of useful arrays of data, including the mesh triangulation, edge connectivity, and vertex relationships. 

    The arxMeshSection array specifies the actual triangulation of the mesh.
    Each arxMeshSection structure indexes into either the vertex index array, for the indices that make up the triangles.
    The material index indexes into the material slot array, and specifies the material used for the group of triangles. 

    The arxMesh structure also contains extra information about the mesh triangulation that can be useful in mesh processing.
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

    The index data for a mesh, given by a arxMesh referenced by the mesh, contains all the indices for the triangles in the mesh. 
    These indices always describe a triangle list - that is, each group of three indices describes a single triangle - the data is not organised into strips or fans in any way. 
*/

/*
    A morph target typically stores delta vertex positions (relative to the base mesh), and optionally, delta normals, tangents, and other attributes (UVs, colors, etc.)
    Each morph target is essentially delta_position = target_vertex_position - base_vertex_position.
    You generally only store deltas for affected vertices, not the entire mesh.
    Actually, it should be always stored in delta to solve conflicts with original indexed vertices and additional buffer management.
*/

/// Vertex data consists of vertex elements combined to form vertex components.
/// Vertex elements, the smallest unit of a vertex, represent entities such as position, normal, or color.
/// Vertex components are one or more vertex elements stored contiguously (interleaved per vertex) in a single memory buffer.

#include "qwadro/inc/render/arxMeshBlueprint.h"

typedef enum arxMeshFlag
{
    arxMeshFlag_NAMED_MORPHES   = AFX_BITMASK(0)
} arxMeshFlags;

typedef enum arxMeshBiasFlag
{
    arxMeshBiasFlag_NONE
} arxMeshBiasFlags;

typedef enum arxVertexFlag
{
    // The data store contents will be modified repeatedly and used many times.
    arxVertexFlag_DYNAMIC = AFX_BITMASK(0),
    
    // The data store contents will be modified once and used at most a few times.
    arxVertexFlag_STREAM = AFX_BITMASK(1),

    arxVertexFlag_POSITIONAL = AFX_BITMASK(2),
    arxVertexFlag_SPATIAL = AFX_BITMASK(3),

    arxVertexFlag_NORMALIZED = AFX_BITMASK(4),
    arxVertexFlag_RASTERIZATION = AFX_BITMASK(5),

    // affected by affine transformations (ex.: position). Non-delta spatial attributes should receive affine transformations.
    arxVertexFlag_ATV = AFX_BITMASK(10),

    // affected by linear transformations (ex.: tangent, binormal). Delta spatial attributes should receive linear transformations (ex.: normal, tangent/binormal cross).
    arxVertexFlag_LTM = AFX_BITMASK(11),

    // affected by inverse linear transformations. Non-delta spatial attributes should receive inverse linear transformations (ex.: normal, tangent/binormal cross).
    arxVertexFlag_ILTM = AFX_BITMASK(12),
 
    // treat as delta
    arxVertexFlag_DELTA = AFX_BITMASK(13),

    // allocation for data is resident
    arxVertexFlag_RESIDENT = AFX_BITMASK(14)
} arxVertexFlags;

typedef enum arxMeshIndexation
{
    // indices mapping triangles to vertices
    arxMeshIndexation_TRI_VTX,

    // indices mapping triangles' edges to vertices
    arxMeshIndexation_EDGE_VTX,

    // (per-vertex) indices mapping provoking vertices for triangles
    arxMeshIndexation_PROVOKING_VTX,

    // (per-vertex) indices mapping actual vertices to original vertices
    arxMeshIndexation_ORIGINAL_VTX,

    // (per-edge, 3 per triangle) indices mapping edges to adjacent triangles
    arxMeshIndexation_EDGE_TO_ADJ,

    // indices mapping triangles to biases
    arxMeshIndexation_TRI_TO_BIAS,

    arxMeshIndexation_BIAS_FOR_TRI
} arxMeshIndexation;

AFX_DEFINE_STRUCT(arxVertexCache)
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

AFX_DEFINE_STRUCT(arxIndexCache)
{
    afxLink          stream;
    avxBuffer           buf;
    afxUnit32           base;
    afxUnit32           range;
    afxUnit32           stride; // idxSiz
    arxMesh             msh;
};

AFX_DEFINE_STRUCT(arxVertexBias)
{
    afxUnit             pivotIdx;
    afxReal             weight;
};

AFX_DEFINE_STRUCT(arxVertexAttribute)
{
    avxFormat           fmt;
    arxVertexFlags      flags;
    afxString8          usage;
};

AFX_DEFINE_STRUCT(arxMeshBias)
{
    arxMeshBiasFlags    flags;
    afxUnit             triCnt;
    afxUnit*            tris; // indices to vertices
};

AFX_DEFINE_STRUCT(arxMeshInfo)
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

/**
    The ArxGetMeshMateriality() method retrieves the simulation that provides the mesh.

    @param msh must be a valid arxMesh handle.
    @return Must return an arxGeome handle of the provider.
*/

ARX arxGeome        ArxGetMeshMateriality(arxMesh msh);

ARX afxBool         ArxGetMeshUrn(arxMesh msh, afxString* id);

/**
    The ArxDescribeMeshMorphes() method retrieves information describing one or more mesh morphes.

    @param msh must be a valid arxMesh handle.
    @param baseMorphIdx is the number of the first/base morph to be indexed from.
    @param cnt is the number of morphes to be described. Must be greater than 0.
    @param info [out] must be a valid pointer to an array of arxMeshMorph structures.
*/

ARX afxUnit         ArxDescribeMeshMorphes(arxMesh msh, afxUnit baseMorphIdx, afxUnit cnt, arxMeshMorph morphes[]);

/**
    The ArxReformMesh() method changes geometric data to one or more mesh morphes.

    @param msh must be a valid arxMesh handle.
    @param baseMorphIdx is the number of the first/base morph to be indexed from.
    @param cnt is the number of morphes to be described. Must be greater than 0.
    @param info must be a valid pointer to an array of arxMeshMorph structures.
*/

ARX afxError        ArxReformMesh(arxMesh msh, afxUnit baseMorphIdx, afxUnit cnt, arxMeshMorph const morphes[]);

/**
    The ArxIsMeshDeformable() method tests whether a mesh is deformable (aka skinned).
    
    A Qwadro mesh is either rigid or deforming. 
    Deforming meshes are ones that are bound to multiple bones, 
    whereas rigid meshes are ones that are bound to a single bone (and thus move "rigidly" with that bone). 

    @param msh must be a valid arxMesh handle.
    @return Must return TRUE if the mesh has more than one bias, or FALSE if has only one.
*/

ARX afxBool         ArxIsMeshDeformable(arxMesh msh);

ARX afxUnit         ArxDescribeMeshBiases(arxMesh msh, afxUnit baseBiasIdx, afxUnit cnt, arxMeshBias biases[]);
ARX afxString*      ArxGetMeshBiasTags(arxMesh msh, afxUnit baseBiasIdx);

ARX afxUnit*        ArxGetMeshIndices(arxMesh msh, afxUnit baseTriIdx);

/**
    The ArxDescribeMesh() method retrieves information describing the mesh.

    @param msh must be a valid arxMesh handle.
    @param info [out] must be a valid pointer to a arxMeshInfo structure.
*/

ARX void            ArxDescribeMesh(arxMesh msh, arxMeshInfo* info);

/**
    The ArxDescribeMeshSections() method retrieves information describing one or more mesh sections.

    @param msh must be a valid arxMesh handle.
    @param baseSecIdx is the number of the first/base surface to be indexed from.
    @param cnt is the number of sections to be described. Must be greater than 0.
    @param info [out] must be a valid pointer to an array of arxMeshSection structures.
*/

ARX afxUnit         ArxDescribeMeshSections(arxMesh msh, afxUnit baseSecIdx, afxUnit cnt, arxMeshSection sections[]);

/**
    The ArxSectionizeMesh() method resets one or more mesh sections to new ranges.

    @param msh must be a valid arxMesh handle.
    @param baseSecIdx is the number of the first/base surface to be indexed from.
    @param cnt is the number of sections to be reset. Must be greater than 0.
    @param info [out] must be a valid pointer to an array of arxMeshSection structures containing the new values.
*/

ARX afxError        ArxSectionizeMesh(arxMesh msh, afxUnit baseSecIdx, afxUnit cnt, arxMeshSection const sections[]);

/**
    The ArxInvertMeshTopology() method inverts the winding of a mesh without changing the vertex data.

    Qwadro assumes that triangle indices are ordered in counter-clockwise winding.
    This method is util in case of source data be in clockwise order, 
    or the vertices were manipulated such that they have been mirrored in some way.

    @param msh must be a valid arxMesh handle.
*/

ARX void            ArxInvertMeshTopology(arxMesh msh);

/**
    The ArxRevestMeshSections() method replaces material indices across mesh sections.

    It can be useful to rearrange material indices across mesh sections after modifying a mesh.
    This method will iterate for each mesh surface, look for its material index in specified table,
    and replace material index with the value.

    @param msh must be a valid arxMesh handle.
    @param mtlIdxCnt is the number of material indices in the lookup table.
    @param mtlIdxLut must be a array of indices mapping actual material indices into new ones.
    @return Must return the number of material indices affectively replaced.
*/

ARX afxUnit         ArxRevestMeshSections(arxMesh msh, afxUnit baseSecIdx, afxUnit secCnt, afxUnit mtlIdxCnt, afxUnit const mtlIdxLut[]);

ARX afxString*      ArxGetMeshMaterials(arxMesh msh, afxUnit baseMtlIdx);

ARX afxError        ArxUpdateMeshIndices(arxMesh msh, afxUnit baseTriIdx, afxUnit triCnt, void const* src, afxUnit srcIdxSiz);
ARX afxError        ArxDumpMeshIndices(arxMesh msh, afxUnit baseTriIdx, afxUnit triCnt, void* dst, afxUnit dstIdxSiz);

ARX afxError        ArxUploadMeshIndices(arxMesh msh, afxUnit baseTriIdx, afxUnit triCnt, afxStream in, afxUnit srcIdxSiz);
ARX afxError        ArxDownloadMeshIndices(arxMesh msh, afxUnit baseTriIdx, afxUnit triCnt, afxStream out, afxUnit dstIdxSiz);

ARX afxError        ArxRecomputeMeshBounds(arxMesh msh, afxUnit morphIdx, afxUnit baseSecIdx, afxUnit cnt, afxUnit posAttrIdx);
ARX afxError        ArxUpdateMeshBounds(arxMesh msh, afxUnit morphIdx, afxUnit baseSecIdx, afxUnit cnt, afxBox const aabbs[]);

/**
    The ArxRecomputeMeshNormals() method computes normal vectors of vertices for a mesh morph.

    The computed normal vector at each vertex is always normalized to have unit length.

    @param msh must be a valid arxMesh handle.
    @param morphIdx must be a valid index into the mesh morphes.
    @param posAttrIdx must be a valid index into the geometry backing the specified mesh morph.
    @param nrmAttrIdx must be a valid index into the geometry backing the specified mesh morph.
    @return Must return any error ocurried, if any, or zero.
*/

ARX afxError        ArxRecomputeMeshNormals(arxMesh msh, afxUnit morphIdx, afxUnit posAttrIdx, afxUnit nrmAttrIdx);

/**
    The ArxRecomputeMeshTangents() method performs tangent frame cosputations on a mesh.

    Tangent and binormal vectors are generated.

    @param msh must be a valid arxMesh handle.
    @param morphIdx must be a valid index into the mesh morphes.
    @param posAttrIdx must be a valid index into the geometry backing the specified mesh morph.
    @param uvAttrIdx must be a valid index into the geometry backing the specified mesh morph.
    @param tanAttrIdx must be a valid index into the geometry backing the specified mesh morph or AFX_INVALID_INDEX to don't store tangent data.
    @param bitAttrIdx must be a valid index into the geometry backing the specified mesh morph or AFX_INVALID_INDEX to don't store bitangent data.
    @return Must return any error ocurried, if any, or zero.
*/

ARX afxError        ArxRecomputeMeshTangents(arxMesh msh, afxUnit morphIdx, afxUnit posAttrIdx, afxUnit uvAttrIdx, afxUnit tanAttrIdx, afxUnit bitAttrIdx);

ARX afxError        ArxBufferizeMesh(arxMesh msh, afxUnit morphIdx, arxVertexCache* vtxCache, arxIndexCache* idxCache);
ARX afxError        ArxDestripifyMesh(arxMesh msh, afxUnit baseTriIdx, afxUnit triCnt, void const* src, afxUnit srcIdxSiz);

ARX afxUnit ArxFindVertexAttributes(arxMesh msh, afxUnit cnt, afxString const usages[], afxUnit indices[]);
ARX afxError ArxDescribeVertexAttribute(arxMesh msh, afxUnit attrIdx, arxVertexAttribute* info);
ARX afxError ArxFormatVertexAttribute(arxMesh msh, afxUnit attrIdx, avxFormat fmt, arxVertexFlags flags, afxString const* usage);
ARX void* ArxAccessVertexData(arxMesh msh, afxUnit attrIdx, afxUnit morphIdx, afxUnit baseVtx);
ARX afxError ArxUpdateVertexData(arxMesh msh, afxUnit attrIdx, afxUnit morphIdx, afxUnit baseVtx, afxUnit vtxCnt, void const* src, afxUnit32 srcStride);
ARX afxError ArxUploadVertexData(arxMesh msh, afxUnit attrIdx, afxUnit morphIdx, afxUnit baseVtx, afxUnit vtxCnt, afxStream in, afxUnit stride);
ARX afxError ArxDownloadVertexData(arxMesh msh, afxUnit attrIdx, afxUnit morphIdx, afxUnit baseVtx, afxUnit vtxCnt, afxStream out, afxUnit stride);
ARX afxError ArxInvertVertexData(arxMesh msh, afxUnit attrIdx, afxUnit morphIdx, afxUnit baseVtx, afxUnit vtxCnt);
ARX afxError ArxNormalizeVertexData(arxMesh msh, afxUnit attrIdx, afxUnit morphIdx, afxUnit baseVtx, afxUnit vtxCnt);

////////////////////////////////////////////////////////////////////////////////

/**
    The ArxTransformMeshes() function similarity transforms an array of meshes.

    @param ltm is the 3x3 linear transformation to perform.
    @param iltm is the 3x3 inverse linear transformation to perform.
    @param ltTol is the tolerance factor for linear transformations.
    @param atv is the translation to perform.
    @param atTol is the tolerance factor for affine transformations.
    @param cnt is the number of meshes to be transformed.
    @param meshes must be a valid pointer to an array of arxMesh handles.
    @return Must return any error ocurried, if any, or zero.
*/

ARX afxError        ArxTransformMeshes(afxM3d const ltm, afxM3d const iltm, afxReal ltTol, afxV3d const atv, afxReal atTol, afxFlags flags, afxUnit cnt, arxMesh meshes[]);

/**
    The ArxEnumerateMeshes() function enumerates some or all meshes provided by a simulation.

    @param morp must be a valid arxGeome handle.
    @param first is the number of the first mesh to be enumerated from.
    @param cnt is the number of meshes to be enumerated. Must be greater than 0.
    @param meshes [out] must be a valid pointer to an array of arxMesh handles.
    @return Must return any error ocurried, if any, or zero.
*/

ARX afxUnit         ArxEnumerateMeshes(arxGeome morp, afxUnit first, afxUnit cnt, arxMesh meshes[]);

/**
    The ArxBuildMeshes() function compiles new meshes using specified blueprints.

    @param morp must be a valid arxGeome handle.
    @param first is the number of the first mesh to be enumerated from.
    @param cnt is the number of meshes to be compiled. Must be greater than 0.
    @param meshes [out] must be a valid pointer to an array of arxMesh handles.
    @return Must return any error ocurried, if any, or zero.
*/

ARX afxError        ArxBuildMeshes(arxGeome morp, afxUnit cnt, arxMeshBlueprint const blueprints[], arxMesh meshes[]);

#endif//ARX_MESH_H
