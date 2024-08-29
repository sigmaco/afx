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

  //////////////////////////////////////////////////////////////////////////////
 // The Qwadro Video Graphics Infrastructure                                 //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AVX_INPUT_IMPL_KIT_H
#define AVX_INPUT_IMPL_KIT_H

#include "../dev/afxDevCoreBase.h"
#include "qwadro/draw/dev/afxDrawSystem.h"

#ifdef _AVX_CAMERA_C

AFX_OBJECT(afxCamera)
{
    afxReal             wpOverHp; // physical w/h
    afxReal             wrOverHr; // resolution w/h
    afxReal             wwOverHw; // window w/h
    afxReal             fovY;
    afxReal             nearClipPlane;
    afxReal             farClipPlane;
    afxReal             depthRangeEpsilon;
    avxClipBoundary     depthRange;
    afxBool             useQuatOrient;
    afxQuat             orient;
    afxM3d              orientM3d;
    afxV3d              elevAzimRoll;
    afxV3d              pos;
    afxV3d              displacement;
    afxM4d              v;
    afxM4d              iv; // inverse view
    afxM4d              p;
    afxM4d              ip; // inverse projection
    afxBool             vChanged;
    afxBool             pChanged;



    afxBool     perspective; // is projection perspective or orthographic.
    afxFrustum  frustum;
};

#endif//_AVX_CAMERA_C

//AFX_STATIC_ASSERT(offsetof(afxCamera, focus % AFX_CAM_ALIGN == 0, "");

#ifdef _AVX_TXD_C

AFX_OBJECT(avxTxd)
{
    afxUri128           uri;
    afxNat              texCnt;
    struct
    {
        afxString       urn;
        afxRaster       ras;
        avxSampler      samp;
        afxAtom32       reqCnt;
    }                   *texs;
};

#endif//_AVX_TXD_C

#ifdef _AVX_VERTEX_DATA_C

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

#ifdef _AVX_MESH_C

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
    afxBox*             surfAabbMap;

    afxString           urn; // 32
    void*               extData;
};

#endif

#ifdef _AVX_MESH_TOPOLOGY_C

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

#endif//AVX_INPUT_IMPL_KIT_H
