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

#ifndef _AVX_DRAW_C
AFX_DECLARE_STRUCT(_avxDsysImpl);
#else
AFX_DEFINE_STRUCT(_avxDsysImpl)
{
    afxError(*waitCb)(afxDrawSystem, afxTime);
    afxError(*waitFenc)(afxDrawSystem, afxBool, afxUnit64, afxUnit, avxFence const[]);
    afxError(*resetFenc)(afxDrawSystem, afxUnit, avxFence const[]);
    afxError(*flushMaps)(afxDrawSystem, afxUnit, avxBufferMap const[]);
    afxError(*syncMaps)(afxDrawSystem, afxUnit, avxBufferMap const[]);
    afxError(*mapBufs)(afxDrawSystem, afxUnit, avxBufferMap const[]);
    afxError(*unmapBufs)(afxDrawSystem, afxUnit, avxBufferMap const[]);
    afxUnit(*getProcs)(afxDrawSystem, afxUnit, afxString const[], void*[]);

    afxClass const*(*dexuCls)(afxDrawSystem);
    afxClass const*(*fencCls)(afxDrawSystem);
    afxClass const*(*doutCls)(afxDrawSystem);
    afxClass const*(*dinCls)(afxDrawSystem);
    afxClass const*(*qrypCls)(afxDrawSystem);
    afxClass const*(*vtxdCls)(afxDrawSystem);
    afxClass const*(*rasCls)(afxDrawSystem);
    afxClass const*(*bufCls)(afxDrawSystem);
    afxClass const*(*sampCls)(afxDrawSystem);
    afxClass const*(*pipCls)(afxDrawSystem);
    afxClass const*(*canvCls)(afxDrawSystem);
    afxClass const*(*shadCls)(afxDrawSystem);
    afxClass const*(*ligaCls)(afxDrawSystem);
};
#endif

#ifdef _AVX_DRAW_SYSTEM_C

AFX_DEFINE_STRUCT(_avxDrawSystemAcquisition)
{
    afxUnit             bridgeCnt;
    afxDrawFeatures     reqFeatures;
    afxUnit             reqExtCnt;
    afxString const*    reqExts;
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
    _avxDsysImpl const* pimpl;
    struct _afxDsysIdd* idd;
    void* udd[4]; // user-defined data

    afxBool         running;
    afxUnit         bridgeCnt; // one per bridge
    afxDrawBridge*  bridges;

    afxDrawFeatures requirements;
    afxDrawLimits const*limits;

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

    afxStringBase   shdStrb;

    afxBool     leftHandedSpace;
    avxClipSpaceDepth clipSpaceDepth;

};
#ifdef _AVX_DRAW_SYSTEM_IMPL
//AFX_STATIC_ASSERT(offsetof(AFX_OBJECT(afxDrawSystem), m) == 0x00, "");
#else
//AFX_STATIC_ASSERT(offsetof(AFX_OBJECT(afxDrawSystem), m) == 0x00, "");
#endif
#endif//_AVX_DRAW_SYSTEM_C

AVX afxClassConfig const _AVX_DSYS_CLASS_CONFIG;
AVX _avxDsysImpl const _AVX_DSYS_IMPL;

AVX _avxDsysImpl const* _AvxDsysGetImpl(afxDrawSystem dsys);

#ifdef AVX_DRIVER_SRC
AVX afxClass const* _AvxDsysGetDexuClass(afxDrawSystem dsys);
AVX afxClass const* _AvxDsysGetDoutClass(afxDrawSystem dsys);
AVX afxClass const* _AvxDsysGetDinClass(afxDrawSystem dsys);

AVX afxClass const* _AvxDsysGetFencClass(afxDrawSystem dsys);
AVX afxClass const* _AvxDsysGetBufClass(afxDrawSystem dsys);
AVX afxClass const* _AvxDsysGetCanvClass(afxDrawSystem dsys);
AVX afxClass const* _AvxDsysGetLigaClass(afxDrawSystem dsys);
AVX afxClass const* _AvxDsysGetPipClass(afxDrawSystem dsys);
AVX afxClass const* _AvxDsysGetQrypClass(afxDrawSystem dsys);
AVX afxClass const* _AvxDsysGetRasClass(afxDrawSystem dsys);
AVX afxClass const* _AvxDsysGetSampClass(afxDrawSystem dsys);
AVX afxClass const* _AvxDsysGetShadClass(afxDrawSystem dsys);
AVX afxClass const* _AvxDsysGetVtxdClass(afxDrawSystem dsys);
#endif

AVX afxError    _AvxLoadGlScript(afxStream file, afxArray* fCode);

AVX afxBool     AvxGetShaderStringBase(afxDrawSystem dsys, afxStringBase* base);

AVX afxError _AvxTransferVideoMemory(afxDrawSystem dsys, avxTransference* ctrl, afxUnit opCnt, void const* ops);

AVX afxDrawFeatures const* _AvxDsysAccessReqFeatures(afxDrawSystem dsys);
AVX afxDrawLimits const* _AvxDsysAccessLimits(afxDrawSystem dsys);

#endif//AVX_IMPL___SYSTEM_H
