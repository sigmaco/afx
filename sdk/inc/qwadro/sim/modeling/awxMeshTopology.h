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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_MESH_TOPOLOGY_H
#define AFX_MESH_TOPOLOGY_H

#include "qwadro/sim/modeling/awxMeshBuilder.h"

/// Mesh triangle topology is described by the awxMeshTopology structure, which is pointed to by the awxMesh. 
/// The awxMeshTopology structure provides a number of useful arrays of data, including the mesh triangulation, edge connectivity, and vertex relationships. 

typedef afxNat8 afxVertexIndex8;
typedef afxNat16 afxVertexIndex16;
typedef afxNat afxVertexIndex;
typedef afxNat afxFaceIndex;
typedef afxVertexIndex8 afxIndexedTriangle8[3];
typedef afxVertexIndex16 afxIndexedTriangle16[3];
typedef afxVertexIndex afxIndexedTriangle[3];

/// The awxMeshSurface array specifies the actual triangulation of the mesh. 
/// Each awxMeshSurface structure indexes into either the vertex index array, for the indices that make up the triangles.  
/// The material index indexes into the material slot array, and specifies the material used for the group of triangles. 

AFX_DEFINE_STRUCT(awxMeshSurface) // aka tri material group
{
    _AFX_DBG_FCC;
    afxNat                  mtlIdx;
    afxNat                  baseTriIdx;
    afxNat                  triCnt; // range from first vertex index
  //afxAabb                 aabb; // SIGMA added this field to ease occlusion culling.
};

AFX_DEFINE_STRUCT(awxMeshTopologyCache)
{
    afxLinkage              stream;

    afxBuffer               buf;
    afxNat32                base;
    afxNat32                range;
    afxNat32                stride;

    awxMeshTopology         msht;
    //afxNat                  idxSiz;
};

/// The awxMeshTopology structure also contains extra information about the mesh triangulation that can be useful in mesh processing. 
/// The vertex-to-tertex map has, for each vertex in the corresponding awxVertexData, an index of the next vertex which was originally the same vertex. 
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

#ifdef _AFX_MESH_TOPOLOGY_C
AFX_OBJECT(awxMeshTopology)
{
    afxNat                  surfCnt;
    awxMeshSurface*         surfaces;
  //afxAabb                 aabb; // SIGMA added this field to ease occlusion culling.
  //afxIndexBuffer          ibuf; // afxIndexBuffer --- aka triangles
  //afxNat                  idxRgn; // if ibuf is shared among other meshes, it should be a base for sections.    
    afxNat                  triCnt; // count of mapped vertices.
    afxIndexedTriangle*     tris; // indices of mapped vertices.

    // EXP ---
    struct
    {
        afxNat              triCnt; // count of mapped vertices.
        afxIndexedTriangle* tris; // indices of mapped vertices.
    }*                      lods;
    afxNat                  lodCnt;
    // --- EXP


    awxMeshTopologyCache    cache;

    afxNat                  vtxToVtxCnt;
    afxNat*                 vtxToVtxMap;
    afxNat                  vtxToTriCnt;
    afxNat*                 vtxToTriMap;
    afxNat                  sideToNeighborCnt;
    afxIndexedTriangle*     sideToNeighborMap;
    afxNat                  jointsForTriCnt;
    afxNat*                 jointsForTriMap;
    afxNat                  triToJointCnt;
    afxNat*                 triToJointMap;
};
#endif

/// The index data for a mesh, given by a awxMeshTopology referenced by the mesh, contains all the indices for the triangles in the mesh. 
/// These indices always describe a triangle list - that is, each group of three indices describes a single triangle - the data is not organised into strips or fans in any way. 
/// You can put these indices into an index buffer like this:

AFX afxNat              AwxCountMeshIndices(awxMeshTopology msht);

AFX afxNat*             AwxGetMeshIndices(awxMeshTopology msht, afxNat baseIdx);
AFX afxIndexedTriangle* AwxGetMeshTriangles(awxMeshTopology msht, afxNat baseTriIdx);

AFX afxNat              AwxCountMeshTriangles(awxMeshTopology msht);

AFX afxNat              AwxCountMeshTriangleEdges(awxMeshTopology msht);

AFX afxNat              AwxCountMeshSurfaces(awxMeshTopology msht);
AFX awxMeshSurface*     AwxGetMeshSurface(awxMeshTopology msht, afxNat surfIdx);

AFX void                AwxGetMeshPrimitives(awxMeshTopology msht, afxNat surfIdx, afxNat* baseTriIdx, afxNat* triCnt);

AFX void                AfxSetMeshCoverage(awxMeshTopology msht, afxNat surfIdx, afxNat mtlIdx);
AFX afxBool             AwxGetMeshCoverage(awxMeshTopology msht, afxNat surfIdx, afxNat* mtlIdx);

/// By default, Qwadro always write triangle indices in counter-clockwise winding. 
/// If you need your indices in clockwise order, or if you've manipulated your mesh vertices such that they've been mirrored in some way, 
/// you can always invert the winding of a awxMeshTopology's indices like this: 

AFX void                AfxInvertMeshWinding(awxMeshTopology msht);

/// If you are merging multiple topologies, or processing material groups, sometimes it can be useful to remap the material indices in a awxMeshSurface. 
/// Qwadro can do this for you: 

AFX void                AfxRemapMeshCoverage(awxMeshTopology msht, afxNat remapCnt, afxNat const remapTable[]);

/// For each material index in the awxMeshTopology, it will lookup that index in the remapTable array and replace material index with the value. 


AFX afxNat              AfxDetermineMeshIndexSize(awxMeshTopology msht);

AFX afxError            AfxBufferizeMeshTopology(awxMeshTopology msht);

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

AFX awxMeshTopology     AfxBuildMeshTopology(afxSimulation sim, awxMeshBuilder const* mshb, afxNat baseSurfIdx, afxNat surfCnt);

#endif//AFX_MESH_TOPOLOGY_H
