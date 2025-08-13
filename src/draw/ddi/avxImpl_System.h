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

AFX_DEFINE_STRUCT(_avxBufferRemapping)
{
    avxBuffer       buf;
    afxSize         offset;
    afxUnit         range;
    avxMappingFlags flags;
    afxBool         unmap;
    void**          placeholder;
};

#ifndef _AVX_DRAW_C
AFX_DECLARE_STRUCT(_avxDsysImpl);
#else
AFX_DEFINE_STRUCT(_avxDsysImpl)
{
    afxError(*waitCb)(afxDrawSystem, afxUnit64);
    afxError(*waitFencCb)(afxDrawSystem, afxBool, afxUnit64, afxUnit, avxFence const[]);
    afxError(*resetFencCb)(afxDrawSystem, afxUnit, avxFence const[]);
    afxError(*cohereCb)(afxDrawSystem, afxBool, afxUnit, avxBufferedMap const[]);
    afxError(*remapCb)(afxDrawSystem, afxBool, afxUnit, _avxBufferRemapping const[]);
    afxUnit(*getProcCb)(afxDrawSystem, afxUnit, afxString const[], void*[]);
    afxError(*transferCb)(afxDrawSystem dsys, avxTransference* ctrl, afxUnit opCnt, void const* ops);

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

    afxClass const*(*camCls)(afxDrawSystem);
    afxClass const*(*txdCls)(afxDrawSystem);
};
#endif

#ifdef _AVX_DRAW_SYSTEM_C

AFX_DEFINE_STRUCT(_avxDsysAcquisition)
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

    afxClassConfig const* camClsCfg;
    afxClassConfig const* txdClsCfg; // req RAS

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
    afxMask         ioExuMask;
    afxMask         dedIoExuMask;
    afxMask         cfxExuMask;
    afxMask         dedCfxExuMask;
    afxMask         gfxExuMask;
    afxMask         videoExuMask;

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

    afxClass        camCls;
    afxClass        txdCls; // req RAS

    afxClass        dexuCls;
    afxClass        fencCls;

    afxStringBase   shdStrb;

    afxBool     leftHandedSpace;
    avxClipSpaceDepth clipSpaceDepth;


    struct
    {
        afxBool devLocal;
        afxChain allocs;
    } heaps[2];
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
AVX afxMask _AvxDsysGetIoExuMask(afxDrawSystem dsys, afxMask* dedIoExuMask);

AVX afxClass const* _AvxDsysGetDexuClass(afxDrawSystem dsys);
#ifdef AVX_DRIVER_SRC
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

AVX afxError _AvxDsysCohereMappedBuffers(afxDrawSystem dsys, afxBool discard, afxUnit cnt, avxBufferedMap const maps[]);
AVX afxError _AvxDsysTransferVideoMemory(afxDrawSystem dsys, avxTransference* ctrl, afxUnit opCnt, void const* ops);
AVX afxError _AvxDsysRemapBuffers(afxDrawSystem dsys, afxBool unmap, afxUnit cnt, _avxBufferRemapping const maps[]);

AVX afxDrawFeatures const* _AvxDsysAccessReqFeatures(afxDrawSystem dsys);
AVX afxDrawLimits const* _AvxDsysAccessLimits(afxDrawSystem dsys);

AVX afxClassConfig const _AVX_TXD_CLASS_CONFIG;
AVX afxClassConfig const _AVX_CAM_CLASS_CONFIG;

#endif//AVX_IMPL___SYSTEM_H
