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

// Morphable topogical mesh, a deformable mesh with fixed topology shared among shapes.

/**
    O objeto afxMesh � a estrutura prim�ria para dado geom�trico no Qwadro.
    Este referencia dados de v�rtice, dados de tri�ngulo, afxMaterial's, afxMeshMorph'es e afxMeshBias's.
    Assets padr�es do Qwadro cont�m um arranjo de afxMesh na estrutura de informa��o do arquivo, e afxMesh'es s�o tamb�m referenciadas pelos objetos afxModel que as usam.
    Cada afxMesh � separada em afxMeshSection's delitmitadas por afxMaterial's.
    A estrutura afxMeshSection � a estrutura prim�ria que voc� interage para obter estas por��es.
    A estrutura afxMeshSection referencia o arranjo de �ndices para a afxMesh, a qual em sua vez referencia o arranjo de v�rtice.
    Se a afxMesh � deform�vel, as v�rtices no arranjo de v�rtice referenciam as articula��es no arranjo de afxMeshBias's.
    Os dados de v�rtice para uma afxMesh, dados por um akxVertexBuffer referenciado pelo afxMesh, cont�m todos os v�rtices �nicos na afxMesh.

    As v�rtices no Qwadro s�o r�gidos ou deform�veis.
    afxMesh'es deform�veis s�o aquelas que est�o ligadas a m�ltiplas articula��es, onde afxMesh'es r�gidas s�o aquelas que est�o ligadas a uma singela articula��o (e portanto movem-se "rigidamente" com aquela articula��o).

    Para determinar quais articula��es uma afxMesh est� ligada a (uma para afxMesh r�gida, muitas para afxMesh deform�vel), voc� pode acessar o arranjo de afxMeshBias's. 
    Este arranjo cont�m nomes das articula��es as quais a afxMesh est� ligada, bem como par�metros de "oriented bounding box" para as partes da afxMesh que est�o ligadas �quela articula��o e outra informa��o pertinente a liga��o malha-a-articula��o.
    Note que na maioria dos casos voc� n�o necessitar� de usar os nomes das articula��es no afxMeshBias diretamente, porque voc� pode usar um objeto asxRiggedMesh para fazer este trabalho (e outro trabalho necess�rio de liga��o) para voc�.

    Os dados de �ndice para uma afxMesh, dado por um afxMesh referenciado pelo afxMesh, cont�m todos os �ndices para os tri�ngulos na afxMesh.
    Estes �ndices sempre descrevem uma lista de tri�ngulo - isso �, cada grupo de tr�s �ndices descrevem um singelo tri�ngulo - os dados n�o s�o organizados em "strips" ou "fans".

    Uma vez que voc� tem v�rtices e �ndices, voc� basicamente tem todos os dados da afxMesh que voc� necessita para quais por��es de �ndice v�o com quais afxMaterial's.
    A estrutura afxMeshSection prov�m esta informa��o.
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

/*
    A morph target typically stores delta vertex positions (relative to the base mesh), and optionally, delta normals, tangents, and other attributes (UVs, colors, etc.)
    Each morph target is essentially delta_position = target_vertex_position - base_vertex_position.
    You generally only store deltas for affected vertices, not the entire mesh.
    Actually, it should be always stored in delta to solve conflicts with original indexed vertices and additional buffer management.
*/

/// Vertex data consists of vertex elements combined to form vertex components.
/// Vertex elements, the smallest unit of a vertex, represent entities such as position, normal, or color.
/// Vertex components are one or more vertex elements stored contiguously (interleaved per vertex) in a single memory buffer.

#ifndef ASX_MESH_H
#define ASX_MESH_H

#include "qwadro/inc/sim/io/afxMeshBlueprint.h"

typedef enum afxMeshFlag
{
    afxMeshFlag_NAMED_MORPHES   = AFX_BITMASK(0)
} afxMeshFlags;

typedef enum afxMeshBiasFlag
{
    afxMeshBiasFlag_NONE
} afxMeshBiasFlags;

typedef enum afxVertexFlag
{
    // The data store contents will be modified repeatedly and used many times.
    afxVertexFlag_DYNAMIC = AFX_BITMASK(0),
    
    // The data store contents will be modified once and used at most a few times.
    afxVertexFlag_STREAM = AFX_BITMASK(1),

    afxVertexFlag_POSITIONAL = AFX_BITMASK(2),
    afxVertexFlag_SPATIAL = AFX_BITMASK(3),

    afxVertexFlag_NORMALIZED = AFX_BITMASK(4),
    afxVertexFlag_RASTERIZATION = AFX_BITMASK(5),

    // affected by affine transformations (ex.: position). Non-delta spatial attributes should receive affine transformations.
    afxVertexFlag_ATV = AFX_BITMASK(10),

    // affected by linear transformations (ex.: tangent, binormal). Delta spatial attributes should receive linear transformations (ex.: normal, tangent/binormal cross).
    afxVertexFlag_LTM = AFX_BITMASK(11),

    // affected by inverse linear transformations. Non-delta spatial attributes should receive inverse linear transformations (ex.: normal, tangent/binormal cross).
    afxVertexFlag_ILTM = AFX_BITMASK(12),
 
    // treat as delta
    afxVertexFlag_DELTA = AFX_BITMASK(13),

    // allocation for data is resident
    afxVertexFlag_RESIDENT = AFX_BITMASK(14)
} afxVertexFlags;

typedef enum asxMeshIndexation
{
    // indices mapping triangles to vertices
    asxMeshIndexation_TRI_VTX,

    // indices mapping triangles' edges to vertices
    asxMeshIndexation_EDGE_VTX,

    // (per-vertex) indices mapping provoking vertices for triangles
    asxMeshIndexation_PROVOKING_VTX,

    // (per-vertex) indices mapping actual vertices to original vertices
    asxMeshIndexation_ORIGINAL_VTX,

    // (per-edge, 3 per triangle) indices mapping edges to adjacent triangles
    asxMeshIndexation_EDGE_TO_ADJ,

    // indices mapping triangles to biases
    asxMeshIndexation_TRI_TO_BIAS,

    asxMeshIndexation_BIAS_FOR_TRI
} asxMeshIndexation;

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

AFX_DEFINE_STRUCT(avxIndexCache)
{
    afxLink          stream;
    avxBuffer           buf;
    afxUnit32           base;
    afxUnit32           range;
    afxUnit32           stride; // idxSiz
    afxMesh             msh;
};

AFX_DEFINE_STRUCT(afxVertexBias)
{
    afxUnit             pivotIdx;
    afxReal             weight;
};

AFX_DEFINE_STRUCT(afxVertexAttribute)
{
    avxFormat           fmt;
    afxVertexFlags      flags;
    afxString8          usage;
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

/**
    The AfxGetMeshMateriality() method retrieves the simulation that provides the mesh.

    @param msh must be a valid afxMesh handle.
    @return Must return an afxMorphology handle of the provider.
*/

ASX afxMorphology  AfxGetMeshMateriality(afxMesh msh);

ASX afxBool         AfxGetMeshUrn(afxMesh msh, afxString* id);

/**
    The AfxDescribeMeshMorphes() method retrieves information describing one or more mesh morphes.

    @param msh must be a valid afxMesh handle.
    @param baseMorphIdx is the number of the first/base morph to be indexed from.
    @param cnt is the number of morphes to be described. Must be greater than 0.
    @param info [out] must be a valid pointer to an array of afxMeshMorph structures.
*/

ASX afxUnit         AfxDescribeMeshMorphes(afxMesh msh, afxUnit baseMorphIdx, afxUnit cnt, afxMeshMorph morphes[]);

/**
    The AfxReformMesh() method changes geometric data to one or more mesh morphes.

    @param msh must be a valid afxMesh handle.
    @param baseMorphIdx is the number of the first/base morph to be indexed from.
    @param cnt is the number of morphes to be described. Must be greater than 0.
    @param info must be a valid pointer to an array of afxMeshMorph structures.
*/

ASX afxError        AfxReformMesh(afxMesh msh, afxUnit baseMorphIdx, afxUnit cnt, afxMeshMorph const morphes[]);

/**
    The AfxIsMeshDeformable() method tests whether a mesh is deformable (aka skinned).
    
    A Qwadro mesh is either rigid or deforming. 
    Deforming meshes are ones that are bound to multiple bones, 
    whereas rigid meshes are ones that are bound to a single bone (and thus move "rigidly" with that bone). 

    @param msh must be a valid afxMesh handle.
    @return Must return TRUE if the mesh has more than one bias, or FALSE if has only one.
*/

ASX afxBool         AfxIsMeshDeformable(afxMesh msh);

ASX afxUnit         AfxDescribeMeshBiases(afxMesh msh, afxUnit baseBiasIdx, afxUnit cnt, afxMeshBias biases[]);
ASX afxString*      AfxGetMeshBiasTags(afxMesh msh, afxUnit baseBiasIdx);

ASX afxUnit*        AfxGetMeshIndices(afxMesh msh, afxUnit baseTriIdx);

/**
    The AfxDescribeMesh() method retrieves information describing the mesh.

    @param msh must be a valid afxMesh handle.
    @param info [out] must be a valid pointer to a afxMeshInfo structure.
*/

ASX void            AfxDescribeMesh(afxMesh msh, afxMeshInfo* info);

/**
    The AfxDescribeMeshSections() method retrieves information describing one or more mesh sections.

    @param msh must be a valid afxMesh handle.
    @param baseSecIdx is the number of the first/base surface to be indexed from.
    @param cnt is the number of sections to be described. Must be greater than 0.
    @param info [out] must be a valid pointer to an array of afxMeshSection structures.
*/

ASX afxUnit         AfxDescribeMeshSections(afxMesh msh, afxUnit baseSecIdx, afxUnit cnt, afxMeshSection sections[]);

/**
    The AfxSectionizeMesh() method resets one or more mesh sections to new ranges.

    @param msh must be a valid afxMesh handle.
    @param baseSecIdx is the number of the first/base surface to be indexed from.
    @param cnt is the number of sections to be reset. Must be greater than 0.
    @param info [out] must be a valid pointer to an array of afxMeshSection structures containing the new values.
*/

ASX afxError        AfxSectionizeMesh(afxMesh msh, afxUnit baseSecIdx, afxUnit cnt, afxMeshSection const sections[]);

/**
    The AfxInvertMeshTopology() method inverts the winding of a mesh without changing the vertex data.

    Qwadro assumes that triangle indices are ordered in counter-clockwise winding.
    This method is util in case of source data be in clockwise order, 
    or the vertices were manipulated such that they have been mirrored in some way.

    @param msh must be a valid afxMesh handle.
*/

ASX void            AfxInvertMeshTopology(afxMesh msh);

/**
    The AfxRevestMeshSections() method replaces material indices across mesh sections.

    It can be useful to rearrange material indices across mesh sections after modifying a mesh.
    This method will iterate for each mesh surface, look for its material index in specified table,
    and replace material index with the value.

    @param msh must be a valid afxMesh handle.
    @param mtlIdxCnt is the number of material indices in the lookup table.
    @param mtlIdxLut must be a array of indices mapping actual material indices into new ones.
    @return Must return the number of material indices affectively replaced.
*/

ASX afxUnit         AfxRevestMeshSections(afxMesh msh, afxUnit baseSecIdx, afxUnit secCnt, afxUnit mtlIdxCnt, afxUnit const mtlIdxLut[]);

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

    @param morp must be a valid afxMorphology handle.
    @param first is the number of the first mesh to be enumerated from.
    @param cnt is the number of meshes to be enumerated. Must be greater than 0.
    @param meshes [out] must be a valid pointer to an array of afxMesh handles.
    @return Must return any error ocurried, if any, or zero.
*/

ASX afxUnit         AfxEnumerateMeshes(afxMorphology morp, afxUnit first, afxUnit cnt, afxMesh meshes[]);

/**
    The AfxBuildMeshes() function compiles new meshes using specified blueprints.

    @param morp must be a valid afxMorphology handle.
    @param first is the number of the first mesh to be enumerated from.
    @param cnt is the number of meshes to be compiled. Must be greater than 0.
    @param meshes [out] must be a valid pointer to an array of afxMesh handles.
    @return Must return any error ocurried, if any, or zero.
*/

ASX afxError        AfxBuildMeshes(afxMorphology morp, afxUnit cnt, afxMeshBlueprint const blueprints[], afxMesh meshes[]);

#endif//ASX_MESH_H
