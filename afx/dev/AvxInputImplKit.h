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

#include "../dev/afxExecImplKit.h"
#include "qwadro/inc/draw/afxDrawSystem.h"

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

#ifdef _AVX_GEOMETRY_C

AFX_DEFINE_STRUCT(avxGeometryAttr)
{
    afxVertexFormat     fmt;
    afxVertexFlags      flags;
    afxString8          usage; // 8
};

AFX_OBJECT(afxGeometry)
{
    //afxNat              biasCnt;
    //afxVertexBias*      biases;
    afxNat              vtxCnt;
    //akxVertex*          vtx; // one for each vertex
    afxNat              attrCnt;
    avxGeometryAttr*    attrs;
    afxByte**           data;
    afxBox              aabb;

    akxVertexCache      cache;
};

#endif

#ifdef _AVX_MESH_C

AFX_OBJECT(afxMesh)
{
    afxNat              triCnt; // count of face indices.
    afxIndexedTriangle* tris; // indices of face indices.
    afxNat              surfCnt;
    afxMeshSurface*     surfaces;
    afxBox*             surfAabbMap;
    afxIndexedTriangle* sideToAdjacentMap; // edgeCnt

    afxNat              biasCnt;
    afxMeshBias*        biases;
    afxString*          biasTagMap;
    afxBox*             biasObbMap;
    afxNat              jointsForTriCnt;
    afxNat*             jointsForTriMap;
    afxNat              triToJointCnt;
    afxNat*             triToJointMap;

    afxNat              vtxCnt;
    afxNat*             vtxToVtxMap; // vtxCnt
    afxNat*             vtxToTriMap; // vtxCnt
    afxNat              morphCnt;
    afxMeshMorph*       morphs;
    afxString*          extraMorphTagMap; // morphCnt - 1; doesn't include base morph.

    avxTopology         topology;
    afxNat              maxIdxValue;
    afxNat              minIdxSiz;
    afxVertexIndexCache cache;

    afxString           urn; // 32
    void*               extData;
};

#endif

#endif//AVX_INPUT_IMPL_KIT_H
