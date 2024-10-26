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
    afxBox              aabb; // mainly used to optimize memory usage with ATV data.
    afxBool             aabbUpdReq;
};

AFX_OBJECT(afxGeometry)
{
    //afxUnit              biasCnt;
    //afxVertexBias*      biases;
    afxUnit             vtxCnt;
    //akxVertex*          vtx; // one for each vertex
    afxUnit*            vtxToVtxMap; // vtxCnt
    afxUnit             attrCnt;
    avxGeometryAttr*    attrs;
    afxByte**           data;
    afxBox              aabb; // mainly used to optimize memory usage with ATV data.

    avxVertexCache      cache;
};

#endif

#ifdef _AVX_MESH_C

AFX_OBJECT(afxMesh)
{
    afxUnit             triCnt; // count of primitives.
    afxUnit             idxCnt; // count of primitive indices.
    afxUnit*            indices; // indices into primitive vertices.
    afxUnit             secCnt;
    afxMeshSection*     sections;
    afxUnit*            sideToAdjacentMap; // edgeCnt

    afxUnit             biasCnt;
    afxMeshBias*        biases;
    afxString*          biasTagMap;
    afxUnit             jointsForTriCnt;
    afxUnit*            jointsForTriMap;
    afxUnit             triToJointCnt;
    afxUnit*            triToJointMap;

    afxUnit             vtxCnt;
    afxUnit*            vtxToTriMap; // vtxCnt
    afxUnit             morphCnt;
    afxMeshMorph*       morphs;
    afxBox*             morphBasedSecAabb; // secCnt * morphCnt
    afxBox*             morphBasedBiasObb; // biasCnt * morphCnt
    afxString*          extraMorphTagMap; // morphCnt - 1; doesn't include base morph.

    avxTopology         topology;
    afxUnit             minIdxSiz;
    avxIndexCache cache;

    afxString           urn; // 32
    void*               extData;
};

#endif

#endif//AVX_INPUT_IMPL_KIT_H
