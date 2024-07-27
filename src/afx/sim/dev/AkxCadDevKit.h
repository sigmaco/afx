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

// This section is part of SIGMA Simulation Infrastructure.

#ifndef AKX_CAD_DEV_KIT_H
#define AKX_CAD_DEV_KIT_H

#include "qwadro/sim/dev/afxSimulation.h"

#ifdef _AKX_MATERIAL_C

AFX_DEFINE_STRUCT(akxMaterialMap)
{
    afxString     usage; // usage 16
    afxMaterial     sub; // afxMaterial
};

AFX_OBJECT(afxMaterial)
{
    afxString       urn; // 128
    afxRaster       tex;
    avxSampler      smp;
    afxNat          mapCnt;
    akxMaterialMap* maps;
    void*           extData;

    // non-Granny stuff
    afxInt          s[3];
    afxV4d          color; // albedo
    afxReal         shininess;

    afxV3d          Ka; // ambient color. The default is (0.2,0.2,0.2);
    afxV3d          Kd; // diffuse color. The default is (0.8,0.8,0.8);
    afxReal         Tr; // transparency. The default is 0.0 (not transparent at all)
    afxV3d          Ks; // specular color. This color shows up in highlights. The default is (1.0,1.0,1.0);
    afxReal         Ns; // specular exponent (aka shininess). The default is 0.0. A high exponent results in a tight, concentrated highlight.
    afxV3d          Ke; // 
    afxV3d          Tf; // Any light passing through the object is filtered by the transmission filter, which only allows the specifiec colors to pass through. For example, Tf 0 1 0 allows all the green to pass through and filters out all the red and blue.
    afxReal         Ni; // optical density for the surface.  This is also known as index of refraction. The values can range from 0.001 to 10. A value of 1.0 means that light does not bend as it passes through an object.
};

//AFX_STATIC_ASSERT(offsetof(afxMaterial, color) % AFX_MTL_ALIGN == 0, "");

#endif//_AKX_MATERIAL_C

#ifdef _AKX_MESH_C

AFX_OBJECT(afxMesh)
{
    afxNat              morphCnt;
    akxMeshMorph*       morphs;
    afxString*          extraMorphTagMap; // morphCnt - 1; doesn't include base morph.

    afxNat              biasCnt;
    akxMeshBias*        biases;
    afxString*          biasTagMap;
    afxBox*             biasObbMap;
    afxMeshTopology     topology;

    afxString           urn; // 32
    void*               extData;
};

#endif

#ifdef _AKX_MESH_TOPOLOGY_C

AFX_OBJECT(afxMeshTopology)
{
    avxTopology         topology;
    afxNat              triCnt; // count of face indices.
    akxIndexedTriangle* tris; // indices of face indices.

    afxNat              surfCnt;
    akxMeshSurface*     surfaces;
    
    afxNat              vtxCnt;
    afxNat*             vtxToVtxMap; // vtxCnt
    afxNat*             vtxToTriMap; // vtxCnt
    akxIndexedTriangle* sideToAdjacentMap; // edgeCnt
    afxNat              jointsForTriCnt;
    afxNat*             jointsForTriMap;
    afxNat              triToJointCnt;
    afxNat*             triToJointMap;

    afxNat              maxIdxValue;
    afxNat              minIdxSiz;
    akxVertexIndexCache cache;
};

#endif

#ifdef _AKX_MODEL_C

AFX_DEFINE_STRUCT(akxMeshRig)
{
    afxMesh             msh;
    afxMaterial         txd;
    afxNat*             biasToJointMap; // to assembled skeleton.
    afxNat*             biasFromJointMap; // from original skeleton.
    afxSkeleton         sklOrig;
};

AFX_OBJECT(afxModel)
{
    afxSkeleton         skl;
    afxNat              rigCnt;
    akxMeshRig*         rigs;
    afxBox              aabb;
    afxTransform        displacement;
    afxString           urn; // 32
};

#endif

#ifdef _AKX_VERTEX_DATA_C

AFX_DEFINE_STRUCT(akxVertexDataAttr)
{
    akxVertexUsage      usage;
    akxVertexFlags      flags;
    afxVertexFormat     fmt;
    union
    {
        void*           data;
        afxByte*        dataBytemap;
    };
    afxString           id; // 8
};

AFX_OBJECT(akxVertexData)
{
    //afxNat              biasCnt;
    //akxVertexBias*      biases;
    afxNat              vtxCnt;
    //akxVertex*          vtx; // one for each vertex
    afxNat              attrCnt;
    akxVertexDataAttr*  attrs;
    afxBox             aabb;

    akxVertexCache  cache;

    afxUri              urd;
    afxNat              urdEntryIdx;
};

#endif

#endif//AKX_CAD_DEV_KIT_H
