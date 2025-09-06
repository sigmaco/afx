/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *          Q W A D R O   4 D   R E N D E R I N G   I N F R A S T R U C T U R E
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

#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/render/arxRenderware.h"
#include "qwadro/render/arxRenderContext.h"
#include "arxImpl_Model.h"
#include "arxImpl_Animation.h"
#include "arxImpl_Body.h"
#include "arxImpl_Curve.h"
#include "arxImpl_Memory.h"

AFX_DEFINE_STRUCT(_arxRweAcquisition)
{
    arxRenderwareConfig     cfg;
    afxDrawSystem           dsys;

    afxClassConfig const*   sbufClsCfg;

    afxClassConfig const*   mdlClsCfg;
    afxClassConfig const*   mshClsCfg;
    afxClassConfig const*   poseClsCfg;
    afxClassConfig const*   plceClsCfg;

    afxClassConfig const*   terClsCfg;
    afxClassConfig const*   litClsCfg;
    afxClassConfig const*   rndClsCfg;

    afxClassConfig const*   rctxClsCfg;
};

#ifdef _ARX_RENDER_CONTEXT_C
AFX_DEFINE_STRUCT(_arxRweEndpoint)
{
    afxLink          ddev;
    afxChain            instances;
    afxMutex            mtx;
    afxCondition        cnd;
    afxString           name;
};

#ifdef _ARX_RENDER_CONTEXT_IMPL
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

    afxClass        sbufCls;
    afxClass        mtlCls;
    afxClass        mshCls;
    afxClass        mdlCls;
    afxClass        terCls;

    afxClass        rctxCls;

    afxStringBase   strbMdlSklMotUrns;
    afxStringBase   strbJointBiasesTags;
    afxStringBase   strbMorphTags;

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
#endif//_ARX_RENDER_CONTEXT_C


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

ARX afxClassConfig const _ARX_RWE_CLASS_CONFIG;

ARX afxClass const* _ArxRweGetTechClass(arxRenderware rwe);
ARX afxClass*       _ArxRweGetIbufClass(arxRenderware rwe);
ARX afxClass*       _ArxRweGetVbufClass(arxRenderware rwe);
ARX afxClass const* _ArxRweGetTexClass(arxRenderware rwe);

ARX afxClass const* _ArxRweGetGeomClass(arxRenderware rwe);
ARX afxClass const* _ArxRweGetMshtClass(arxRenderware rwe);

ARX afxClass const* _ArxRweGetCamClassCb_SW(arxRenderware rwe);

ARX afxClass const* _ArxRweGetPoseClass(arxRenderware rwe);
ARX afxClass const* _ArxRweGetPlceClass(arxRenderware rwe);
ARX afxClass const* _ArxDinGetGeomClass(arxRenderware rwe);
ARX afxClass const* _ArxRweGetLitClass(arxRenderware rwe);
ARX afxClass const* _ArxRweGetRndrClass(arxRenderware rwe);
ARX afxClass const* _ArxRweGetScnClass(arxRenderware rwe);
ARX afxClass const* _ArxRweGetRctxClass(arxRenderware rwe);

ARX afxClassConfig const _ARX_CAM_CLASS_CONFIG;
ARX afxClassConfig const _ARX_LIT_CLASS_CONFIG;
ARX afxClassConfig const _ARX_RND_CLASS_CONFIG;
ARX afxClassConfig const _ARX_TER_CLASS_CONFIG;
ARX afxClassConfig const _ARX_SCN_CLASS_CONFIG;
ARX afxClassConfig const _ARX_TEX_CLASS_CONFIG;
ARX afxClassConfig const _ARX_DTEC_CLASS_CONFIG;
ARX afxClassConfig const _ARX_VBUF_CLASS_CONFIG;

ARX afxClassConfig const _ARX_RCTX_CLASS_CONFIG;

ARX afxClass const* _ArxRweGetMtlClass(arxRenderware rwe);
ARX afxClass const* _ArxRweGetMdlClass(arxRenderware rwe);
ARX afxClass const* _ArxRweGetMshClass(arxRenderware rwe);
ARX afxClass const* _ArxRweGetTerClass(arxRenderware rwe);
ARX afxStringBase   _ArxRweGetModelUrnStringBase(arxRenderware rwe);
ARX afxStringBase   _ArxRweGetPivotTagStringBase(arxRenderware rwe);
ARX afxStringBase   _ArxRweGetMorphTagStringBase(arxRenderware rwe);

ARX afxClass const* _ArxRweGetRbufClass(arxRenderware rwe);

#endif//ARX_IMPL___INPUT_H
