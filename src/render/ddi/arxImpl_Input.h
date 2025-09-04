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

  //////////////////////////////////////////////////////////////////////////////
 // The Qwadro Video Graphics Infrastructure                                 //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef ARX_IMPL___INPUT_H
#define ARX_IMPL___INPUT_H

#include "qwadro/inc/draw/afxDrawSystem.h"
#include "qwadro/inc/render/arxRenderware.h"
#include "qwadro/inc/render/arxRenderContext.h"
#include "arxImpl_Model.h"
#include "arxImpl_Animation.h"
#include "arxImpl_Body.h"
#include "arxImpl_Curve.h"

AFX_DEFINE_STRUCT(_arxDinAcquisition)
{
    arxRenderwareConfig  cfg;
    afxDrawSystem       dsys;

    afxClassConfig const* mdlClsCfg;
    afxClassConfig const* mshClsCfg;
    afxClassConfig const* mtlyClsCfg;
    afxClassConfig const* poseClsCfg;
    afxClassConfig const* plceClsCfg;

    afxClassConfig const* terClsCfg;
    afxClassConfig const* litClsCfg;
    afxClassConfig const* rndClsCfg;
};

#ifdef _ARX_DRAW_INPUT_C
AFX_DEFINE_STRUCT(afxDrawInputEndpoint)
{
    afxLink          ddev;
    afxChain            instances;
    afxMutex            mtx;
    afxCondition        cnd;
    afxString           name;
};

#ifdef _ARX_DRAW_INPUT_IMPL
AFX_OBJECT(_arxDrawInput)
#else
AFX_OBJECT(arxRenderware)
#endif
{
    afxChain            classes;
    afxClass            dtecCls;
    afxClass            texCls;
    afxClass            ibuffers;
    afxClass            vbuffers;
    afxClass            camCls;

    afxClass            mshtCls;
    afxClass            geomCls;


    afxClass      lights;
    afxClass      renderers;
    afxClass        scnCls;
    afxClass        sklCls;
    afxClass        poseCls;
    afxClass        plceCls;
    afxClass        mtlyCls;

    struct smt* Smt;
    struct smt2* Smt2;


    afxDrawSystem       dsys;
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
#endif//_ARX_DRAW_INPUT_C


#ifdef _ARX_CAMERA_C

AFX_OBJECT(arxCamera)
{
    afxCameraFlags      flags;
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
    afxM4d              v, iv; // inverse view
    afxM4d              p, ip; // inverse projection
    afxM4d              pv, ipv; // IP * IV
    afxBool             shouldSyncV;
    afxBool             shouldSyncP;



    afxBool     perspective; // is projection perspective or orthographic.
    afxFrustum  frustum;
};

#endif//_ARX_CAMERA_C

//AFX_STATIC_ASSERT(offsetof(arxCamera, focus % AFX_CAM_ALIGN == 0, "");

#ifdef _ARX_GEOMETRY_C

AFX_DEFINE_STRUCT(avxGeometryAttr)
{
    avxFormat           fmt;
    arxVertexFlags      flags;
    afxString8          usage; // 8
    afxBox              aabb; // mainly used to optimize memory usage with ATV data.
    afxBool             aabbUpdReq;
};

AFX_OBJECT(afxGeometry)
{
    //afxUnit              biasCnt;
    //arxVertexBias*      biases;
    afxUnit             vtxCnt;
    //akxVertex*          vtx; // one for each vertex
    afxUnit*            vtxToVtxMap; // vtxCnt
    afxUnit             attrCnt;
    avxGeometryAttr*    attrs;
    afxByte**           data;
    afxBox              aabb; // mainly used to optimize memory usage with ATV data.

    arxVertexCache      cache;
};

#endif

ARX afxClassConfig const _ARX_DIN_CLASS_CONFIG;

ARX afxClass const* ArxGetDrawTechniqueClass(arxRenderware din);
ARX afxClass*       ArxGetIndexBufferClass(arxRenderware din);
ARX afxClass*       ArxGetVertexBufferClass(arxRenderware din);
ARX afxClass const* ArxGetTextureClass(arxRenderware din);

ARX afxClass const* ArxGetGeometryClass(arxRenderware din);
ARX afxClass const* ArxGetMeshTopologyClass(arxRenderware din);

ARX afxClass const* _ArxDinGetCamClassCb_SW(arxRenderware din);

ARX afxClass const* _ArxGetPoseClass(arxRenderware din);
ARX afxClass const* _ArxGetPlacementClass(arxRenderware din);
ARX afxClass const* _ArxDinGetGeomClass(arxRenderware din);
ARX afxClass const* _ArxGetLightClass(arxRenderware din);
ARX afxClass const* _ArxGetRendererClass(arxRenderware din);
ARX afxClass const* _ArxGetSceneClass(arxRenderware din);

ARX afxClassConfig const _ARX_CAM_CLASS_CONFIG;
ARX afxClassConfig const _ARX_LIT_CLASS_CONFIG;
ARX afxClassConfig const _ARX_RND_CLASS_CONFIG;
ARX afxClassConfig const _ARX_TER_CLASS_CONFIG;
ARX afxClassConfig const _ARX_SCN_CLASS_CONFIG;
ARX afxClassConfig const _ARX_TEX_CLASS_CONFIG;
ARX afxClassConfig const _ARX_DTEC_CLASS_CONFIG;
ARX afxClassConfig const _ARX_VBUF_CLASS_CONFIG;


ARX afxClass const* _ArxMorpGetMaterialClass(arxGeome morp);
ARX afxClass const* _ArxMorpGetModelClass(arxGeome morp);
ARX afxClass const* _ArxMorpGetMeshClass(arxGeome morp);
ARX afxClass const* _ArxMorpGetTerrainClass(arxGeome morp);
ARX afxStringBase   _ArxMorpGetModelUrnStringBase(arxGeome morp);
ARX afxStringBase   _ArxMorpGetPivotTagStringBase(arxGeome morp);
ARX afxStringBase   _ArxMorpGetMorphTagStringBase(arxGeome morp);

#endif//ARX_IMPL___INPUT_H
