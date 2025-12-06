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
// This software is part of Advanced Video Graphics Extensions & Experiments.

#ifndef AVX_SYSTEM_DDK_H
#define AVX_SYSTEM_DDK_H

#include "avxBridgeDDK.h"
#include "avxStorageDDK.h"
#include "avxFenceDDK.h"
#include "avxPipelineDDK.h"
#include "avxQueryPoolDDK.h"
#include "avxCodebaseDDK.h"
#include "avxLigatureDDK.h"
#include "avxVertexInputDDK.h"
#include "avxSamplerDDK.h"
#include "avxCanvasDDK.h"
#include "avxSurfaceDDK.h"
#include "avxRasterDDK.h"
#include "avxBufferDDK.h"

AFX_DECLARE_STRUCT(_avxIddDsys);

#ifndef _AVX_DRAW_C
AFX_DECLARE_STRUCT(_avxDdiDsys);
#else
AFX_DEFINE_STRUCT(_avxDdiDsys)
{
    afxError(*waitCb)(afxDrawSystem, afxUnit64);
    afxError(*waitFencCb)(afxDrawSystem, afxUnit64, afxBool, afxUnit, avxFence const[], afxUnit64 const values[]);
    afxError(*resetFencCb)(afxDrawSystem, afxUnit, avxFence const[]);
    afxError(*cohereCb)(afxDrawSystem, afxBool invalidate, afxUnit, avxBufferedMap const[]);
    afxError(*remapCb)(afxDrawSystem, afxBool, afxUnit, _avxBufferRemapping const[]);
    afxUnit(*getProcCb)(afxDrawSystem, afxUnit, afxString const[], void*[]);
    afxError(*transferCb)(afxDrawSystem dsys, avxTransference* ctrl, afxUnit opCnt, void const* ops);
    afxError(*allocRasCb)(afxDrawSystem,afxUnit,avxRasterInfo const[],avxRaster[]);
    afxError(*deallocRasCb)(afxDrawSystem, afxUnit, avxRaster[]);
    afxError(*allocBufCb)(afxDrawSystem, afxUnit, avxBufferInfo const[], avxBuffer[]);
    afxError(*deallocBufCb)(afxDrawSystem, afxUnit, avxBuffer[]);

    afxError(*resetQryp)(afxDrawSystem, avxQueryPool, afxUnit base, afxUnit cnt);
    afxResult(*getQrypRslt)(afxDrawSystem, avxQueryPool, avxQueryResultFlags, afxUnit base, afxUnit cnt, afxSize cap, void* dst, afxSize stride);

    afxClass const*(*dexuCls)(afxDrawSystem);
    afxClass const*(*fencCls)(afxDrawSystem);
    afxClass const*(*doutCls)(afxDrawSystem);
    afxClass const*(*qrypCls)(afxDrawSystem);
    afxClass const*(*vtxdCls)(afxDrawSystem);
    afxClass const*(*rasCls)(afxDrawSystem);
    afxClass const*(*bufCls)(afxDrawSystem);
    afxClass const*(*sampCls)(afxDrawSystem);
    afxClass const*(*pipCls)(afxDrawSystem);
    afxClass const*(*canvCls)(afxDrawSystem);
    afxClass const*(*shadCls)(afxDrawSystem);
    afxClass const*(*ligaCls)(afxDrawSystem);

    afxClass const*(*txdCls)(afxDrawSystem);
};
#endif

#ifdef _AVX_DRAW_SYSTEM_C

AFX_DEFINE_STRUCT(_avxDsysAcq)
{
    afxUnit             bridgeCnt;
    avxFeatures     reqFeatures;
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

    _avxDdiDsys const*  ddi;
    _avxIddDsys*        idd;
    // User-defined data.
    void*               udd;
    // Debugging tag.
    afxString           tag;

    afxBool             running;
    afxUnit             bridgeCnt; // one per bridge
    afxDrawBridge*      bridges;
    afxMask             ioExuMask;
    afxMask             dedIoExuMask;
    afxMask             cfxExuMask;
    afxMask             dedCfxExuMask;
    afxMask             gfxExuMask;
    afxMask             videoExuMask;

    avxFeatures     requirements;
    avxLimits const*limits;

    //afxChain classes;

    afxClass            bufCls;
    afxClass            rasCls;
    afxClass            canvCls; // req RAS
    afxClass            sampCls;
    afxClass            ligaCls; // req BUF, RAS, SAMP

    afxClass            vinCls;
    afxClass            shadCls;
    afxClass            pipCls; // req SHD, SHD, VTXD

    afxClass            qrypCls;

    afxClass            doutCls; // req RAS, CANV

    afxClass            txdCls; // req RAS

    afxClass            dexuCls;
    afxClass            fencCls;
    afxClass            dctxCls;

    afxStringBase       shdStrb;

    afxBool             leftHandedSpace;
    avxClipSpaceDepth   clipSpaceDepth;
    afxReal             clipSpaceDepthRangeEpsilon;

    struct
    {
        afxBool         devLocal;
        afxChain        allocs;
    } heaps[2];
};
#ifdef _AVX_DRAW_SYSTEM_IMPL
//AFX_STATIC_ASSERT(offsetof(AFX_OBJECT(afxDrawSystem), m) == 0x00, "");
#else
//AFX_STATIC_ASSERT(offsetof(AFX_OBJECT(afxDrawSystem), m) == 0x00, "");
#endif
#endif//_AVX_DRAW_SYSTEM_C

AVX afxClassConfig const _AVX_CLASS_CONFIG_DSYS;
AVX _avxDdiDsys const _AVX_DDI_DSYS;

AVX _avxDdiDsys const* _AvxDsysGetDdi(afxDrawSystem dsys);
AVX afxMask _AvxDsysGetIoExuMask(afxDrawSystem dsys, afxMask* dedIoExuMask);

AVX afxClass const* _AvxDsysGetDexuClassCb_SW(afxDrawSystem dsys);
AVX afxClass const* _AvxDsysGetDctxClassCb_SW(afxDrawSystem dsys);

AVX afxClass const* _AvxDsysGetDoutClassCb_SW(afxDrawSystem dsys);

AVX afxClass const* _AvxDsysGetFencClassCb_SW(afxDrawSystem dsys);
AVX afxClass const* _AvxDsysGetBufClassCb_SW(afxDrawSystem dsys);
AVX afxClass const* _AvxDsysGetCanvClassCb_SW(afxDrawSystem dsys);
AVX afxClass const* _AvxDsysGetLigaClassCb_SW(afxDrawSystem dsys);
AVX afxClass const* _AvxDsysGetPipClassCb_SW(afxDrawSystem dsys);
AVX afxClass const* _AvxDsysGetQrypClassCb_SW(afxDrawSystem dsys);
AVX afxClass const* _AvxDsysGetRasClassCb_SW(afxDrawSystem dsys);
AVX afxClass const* _AvxDsysGetSampClassCb_SW(afxDrawSystem dsys);
AVX afxClass const* _AvxDsysGetShadClassCb_SW(afxDrawSystem dsys);
AVX afxClass const* _AvxDsysGetVinClassCb_SW(afxDrawSystem dsys);

AVX afxError    _AvxLoadGlScript(afxStream file, afxArray* fCode);

AVX afxBool     AvxGetShaderStringBase(afxDrawSystem dsys, afxStringBase* base);

AVX afxClass const* _AvxDsysGetFencClassCb_SW(afxDrawSystem dsys);
AVX afxClass const* _AvxDsysGetDoutClassCb_SW(afxDrawSystem dsys);
AVX afxClass const* _AvxDsysGetQrypClassCb_SW(afxDrawSystem dsys);
AVX afxClass const* _AvxDsysGetVinClassCb_SW(afxDrawSystem dsys);
AVX afxClass const* _AvxDsysGetRasClassCb_SW(afxDrawSystem dsys);
AVX afxClass const* _AvxDsysGetBufClassCb_SW(afxDrawSystem dsys);
AVX afxClass const* _AvxDsysGetSampClassCb_SW(afxDrawSystem dsys);
AVX afxClass const* _AvxDsysGetPipClassCb_SW(afxDrawSystem dsys);
AVX afxClass const* _AvxDsysGetCanvClassCb_SW(afxDrawSystem dsys);
AVX afxClass const* _AvxDsysGetShadClassCb_SW(afxDrawSystem dsys);
AVX afxClass const* _AvxDsysGetLigaClassCb_SW(afxDrawSystem dsys);
AVX afxClass const* _AvxDsysGetTxdClassCb_SW(afxDrawSystem dsys);

AVX avxFeatures const* _AvxDsysGetReqFeatures(afxDrawSystem dsys);
AVX avxLimits const* _AvxDsysGetLimits(afxDrawSystem dsys);

AVX afxClassConfig const _AVX_CLASS_CONFIG_TXD;

#endif//AVX_SYSTEM_DDK_H
