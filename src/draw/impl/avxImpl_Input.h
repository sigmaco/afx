/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      Q W A D R O   D R A W   I / O   S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

  //////////////////////////////////////////////////////////////////////////////
 // The Qwadro Video Graphics Infrastructure                                 //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AVX_IMPL___INPUT_H
#define AVX_IMPL___INPUT_H

#include "qwadro/inc/draw/afxDrawSystem.h"

#ifdef _AVX_DRAW_INPUT_C
AFX_DEFINE_STRUCT(afxDrawInputEndpoint)
{
    afxLink          ddev;
    afxChain            instances;
    afxMutex            mtx;
    afxCondition        cnd;
    afxString           name;
};

#ifdef _AVX_DRAW_INPUT_IMPL
AFX_OBJECT(_avxDrawInput)
#else
AFX_OBJECT(afxDrawInput)
#endif
{
    afxChain            classes;
    afxClass            camCls;
    afxClass            dtecCls;
    afxClass            txdCls;
    afxClass            texCls;
    afxClass            ibuffers;
    afxClass            vbuffers;

    afxClass            mshtCls;
    afxClass            geomCls;

    void*               vbMgr;

    afxBool             leftHandedSpace;
    avxClipSpaceDepth   clipSpaceDepth;

    afxUri128           txdUris[8];
    afxFile             txdHandles[8];

    afxUnit             minVtxBufSiz;
    afxUnit             maxVtxBufSiz;
    afxUnit             minIdxBufSiz;
    afxUnit             maxIdxBufSiz; // 13500000

    afxDrawInputProc    procCb;

    struct _afxDinIdd*  idd;
    void*               udd; // user-defined data
};
#endif//_AVX_DRAW_INPUT_C


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
    avxClipSpaceDepth     depthRange;
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
    avxFormat           fmt;
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

AVX afxClassConfig const _AVX_DIN_CLASS_CONFIG;

AVX afxClass const* AvxGetCameraClass(afxDrawInput din);
AVX afxClass const* AvxGetDrawTechniqueClass(afxDrawInput din);
AVX afxClass*       AvxGetIndexBufferClass(afxDrawInput din);
AVX afxClass*       AvxGetVertexBufferClass(afxDrawInput din);
AVX afxClass const* AvxGetTextureClass(afxDrawInput din);
AVX afxClass const* AvxGetTxdClass(afxDrawInput din);

AVX afxClass const* AvxGetGeometryClass(afxDrawInput din);
AVX afxClass const* AvxGetMeshTopologyClass(afxDrawInput din);

#endif//AVX_IMPL___INPUT_H
