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

#ifndef AVX_IMPL___SYSTEM_H
#define AVX_IMPL___SYSTEM_H

#include "qwadro/inc/draw/afxDrawSystem.h"

#define AFX_MAX_DRAW_BRIDGE_PER_SYSTEM 16
#define AFX_MAX_DRAW_QUEUE_PER_BRIDGE 32

#ifdef _AVX_DRAW_SYSTEM_C

AFX_DEFINE_STRUCT(_avxDrawSystemAcquisition)
{
    afxUnit             bridgeCnt;
    afxDrawFeatures     requirements;
    afxUnit             extensionCnt;
    afxString const*    extensions;
    void*               udd;

    afxClassConfig const* bufClsCfg;
    afxClassConfig const* rasClsCfg;
    afxClassConfig const* canvClsCfg; // req RAS
    afxClassConfig const* sampClsCfg;
    afxClassConfig const* ligaClsCfg; // req BUF, RAS, SAMP
    afxClassConfig const* shdClsCfg;
    afxClassConfig const* vtxdClsCfg;
    afxClassConfig const* pipClsCfg; // req SHD, LIGA, VTXD
    afxClassConfig const* qrypClsCfg;

    afxClassConfig const* doutClsCfg; // req RAS, CANV

    afxClassConfig const* dinClsCfg; // req BUF

    afxClassConfig const* dexuClsCfg;
    afxClassConfig const* dqueClsCfg;
    afxClassConfig const* dctxClsCfg;
};

#ifndef _AFX_CONTEXT_C
#   error "Require afxDevLink implementation"
#endif
#ifdef _AVX_DRAW_SYSTEM_IMPL
AFX_OBJECT(_avxDrawSystem)
#else
AFX_OBJECT(afxDrawSystem)
#endif
{
    AFX_OBJ(afxDevLink) ctx;
    afxBool         running;
    afxUnit         bridgeCnt; // one per bridge
    afxDrawBridge*  bridges;

    afxDrawFeatures requirements;

    //afxChain classes;
    afxClass        bufCls;
    afxClass        rasCls;
    afxClass        canvCls; // req RAS
    afxClass        sampCls;
    afxClass        ligaCls; // req BUF, RAS, SAMP

    afxClass        vinCls;
    afxClass        shadCls;
    afxClass        pipCls; // req SHD, SHD, VTXD

    afxClass        qrypCls;

    afxClass        doutCls; // req RAS, CANV
    afxClass        dinCls; // req BUF

    afxClass        dexuCls;
    afxClass        fencCls;
    afxError        (*waitFenc)(afxDrawSystem,afxBool,afxUnit64,afxUnit,avxFence const[]);
    afxError        (*resetFenc)(afxDrawSystem,afxUnit,avxFence const[]);

    afxStringBase   shdStrb;

    afxBool     leftHandedSpace;
    avxClipSpaceDepth clipSpaceDepth;

    afxError(*waitCb)(afxDrawSystem,afxTime);

    afxError(*flushMapsCb)(afxDrawSystem, afxUnit, afxBufferMap const[]);
    afxError(*syncMapsCb)(afxDrawSystem,afxUnit,afxBufferMap const[]);
    afxError(*mapBufsCb)(afxDrawSystem, afxUnit, afxBufferMap const[]);
    afxError(*unmapBufsCb)(afxDrawSystem, afxUnit, afxBufferMap const[]);

    struct _afxDsysIdd* idd;
    void* udd[4]; // user-defined data
};
#ifdef _AVX_DRAW_SYSTEM_IMPL
//AFX_STATIC_ASSERT(offsetof(AFX_OBJECT(afxDrawSystem), m) == 0x00, "");
#else
//AFX_STATIC_ASSERT(offsetof(AFX_OBJECT(afxDrawSystem), m) == 0x00, "");
#endif
#endif//_AVX_DRAW_SYSTEM_C

AVX afxClassConfig const _AVX_DSYS_CLASS_CONFIG;

AVX afxClass const* _AvxGetDrawBridgeClass(afxDrawSystem dsys);
AVX afxClass const* _AvxGetDrawOutputClass(afxDrawSystem dsys);
AVX afxClass const* _AvxGetDrawInputClass(afxDrawSystem dsys);

AVX afxClass const* _AvxGetFenceClass(afxDrawSystem dsys);
AVX afxClass const* _AvxGetBufferClass(afxDrawSystem dsys);
AVX afxClass const* _AvxGetCanvasClass(afxDrawSystem dsys);
AVX afxClass const* _AvxGetLigatureClass(afxDrawSystem dsys);
AVX afxClass const* _AvxGetPipelineClass(afxDrawSystem dsys);
AVX afxClass const* _AvxGetQueryPoolClass(afxDrawSystem dsys);
AVX afxClass const* _AvxGetRasterClass(afxDrawSystem dsys);
AVX afxClass const* _AvxGetSamplerClass(afxDrawSystem dsys);
AVX afxClass const* _AvxGetShaderClass(afxDrawSystem dsys);
AVX afxClass const* _AvxGetVertexInputClass(afxDrawSystem dsys);

AVX afxError    _AvxLoadGlScript(afxStream file, afxArray* fCode);

AVX afxBool     AvxGetShaderStringBase(afxDrawSystem dsys, afxStringBase* base);

AVX afxError _AvxTransferVideoMemory(afxDrawSystem dsys, avxTransference* ctrl, afxUnit opCnt, void const* ops);

AVX afxDrawFeatures const* _AvxAccessDrawRequirements(afxDrawSystem dsys);

#endif//AVX_IMPL___SYSTEM_H
