/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *            Q W A D R O   M U L T I M E D I A   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This software is part of Advanced Multimedia Extensions & Experiments.

#ifndef AMX_SYSTEM_DDK_H
#define AMX_SYSTEM_DDK_H

#include "qwadro/mix/afxMixSystem.h"
#include "mpu/amxMpuExecutor.h"
#include "amxSoundscapeDDK.h"

#ifndef _AMX_MIX_C
AFX_DECLARE_STRUCT(_amxDdiMsys);
#else
AFX_DEFINE_STRUCT(_amxDdiMsys)
{
    afxError(*waitCb)(afxMixSystem, afxUnit64);
    //afxError(*waitFencCb)(afxMixSystem, afxBool, afxUnit64, afxUnit, amxFence const[]);
    //afxError(*resetFencCb)(afxMixSystem, afxUnit, amxFence const[]);
    afxError(*cohereCb)(afxMixSystem, afxBool, afxUnit, amxBufferedMap const[]);
    afxError(*remapCb)(afxMixSystem, afxBool, afxUnit, _amxBufferRemapping const[]);
    afxUnit(*getProcCb)(afxMixSystem, afxUnit, afxString const[], void*[]);
    afxError(*transferCb)(afxMixSystem dsys, amxTransference* ctrl, afxUnit opCnt, void const* ops);
    afxError(*allocBufCb)(afxMixSystem, afxUnit, amxBufferInfo const[], amxBuffer[]);
    afxError(*deallocBufCb)(afxMixSystem, afxUnit, amxBuffer[]);

    afxClass const*(*mexuCls)(afxMixSystem);
    afxClass const*(*fencCls)(afxMixSystem);
    afxClass const*(*sinkCls)(afxMixSystem);
    afxClass const*(*bufCls)(afxMixSystem);
};
#endif


AFX_DEFINE_STRUCT(__amxMsysAcq)
{
    afxUnit             bridgeCnt;
    afxMixFeatures      reqFeatures;
    afxUnit             reqExtCnt;
    afxString const*    reqExts;
    void*               udd;
    afxDrawSystem       dsys;

    afxClassConfig const* mbufClsCfg;
    afxClassConfig const* wavClsCfg;
    afxClassConfig const* sndClsCfg;
    afxClassConfig const* sndsClsCfg;

    afxClassConfig const* vidClsCfg;
    //afxClassConfig const* viddClsCfg;
    afxClassConfig const* mdecClsCfg;
    afxClassConfig const* mencClsCfg;

    afxClassConfig const* traxClsCfg;
    afxClassConfig const* sinkClsCfg;

    afxClassConfig const* mexuClsCfg;
    afxClassConfig const* mixClsCfg;
};

#ifdef _AMX_MIX_SYSTEM_C
#ifdef _AMX_MIX_SYSTEM_IMPL
AFX_OBJECT(_amxMixSystem)
#else
AFX_OBJECT(afxMixSystem)
#endif
{
    AFX_OBJECT(afxDevLink) ctx;
    _amxDdiMsys const* ddi;
    afxBool             running;
    afxUnit             bridgeCnt;
    afxMixBridge*       bridges;
    afxMask             ioExuMask;
    afxMask             dedIoExuMask;
    afxMask             cfxExuMask;
    afxMask             dedCfxExuMask;
    afxMask             gfxExuMask;
    afxMask             videoExuMask;
    afxMixFeatures      requirements;
    afxMixLimits const* limits;

    //afxChain            classes;
    afxClass            mexuCls;
    afxClass            asioCls;
    afxClass            mbufCls;
    afxClass            wavCls;
    afxClass            sndsCls;
    afxClass            brokCls;
    afxClass            vidCls;
    //afxClass            viddCls;
    afxClass            mdecCls;
    afxClass            mencCls;
    afxClass            mixCls;
    afxClass            traxCls;
    afxClass            mcdcCls;
    afxClass            msrcCls;
    afxClass            msnkCls;

    amxSoundscape         heads[AFX_MAX_AUDITION_HEADS];

    afxDrawSystem       dsys;

    afxV3d              listener;
    afxV3d              orientation;

    afxChain        activeTrackers;
    afxError(*waitCb)(afxMixSystem);

    struct _afxSctxIdd* idd;
    void*               udd; // user-defined data
};
#ifdef _AMX_MIX_SYSTEM_IMPL
//AFX_STATIC_ASSERT(offsetof(struct _afxBaseMixSystem, ctx) == 0x00, "");
#else
//AFX_STATIC_ASSERT(offsetof(AFX_OBJECT(afxMixSystem), ctx) == 0x00, "");
#endif
#endif//_AMX_MIX_SYSTEM_C

AMX afxClassConfig const _AMX_MSYS_CLASS_CONFIG;

AMX afxClass const* _AmxMsysGetMexuClass(afxMixSystem msys);
AMX afxClass const* _AmxMsysGetSinkClass(afxMixSystem msys);
AMX afxClass const* _AmxMsysGetTraxClass(afxMixSystem msys);

AMX afxClass const* _AmxMsysGetMixClass(afxMixSystem msys);
AMX afxClass const* _AmxMsysGetAudClass(afxMixSystem msys);
AMX afxClass const* _AmxMsysGetBufClass(afxMixSystem msys);
AMX afxClass const* _AmxMsysGetSndsClass(afxMixSystem msys);

AMX afxClass const* _AmxMsysGetVidClass(afxMixSystem msys);
AMX afxClass const* _AmxMsysGetViddClass(afxMixSystem msys);
AMX afxClass const* _AmxMsysGetMencClass(afxMixSystem msys);
AMX afxClass const* _AmxMsysGetMdecClass(afxMixSystem msys);

AMX _amxDdiMsys const* _AmxMsysGetDdi(afxMixSystem msys);
AMX afxMask _AmxMsysGetIoExuMask(afxMixSystem msys, afxMask* dedIoExuMask);

AMX afxClassConfig const _AMX_MCDC_CLASS_CONFIG;
AMX afxClassConfig const _AMX_ASIO_CLASS_CONFIG;
AMX afxClassConfig const _AMX_TRAX_CLASS_CONFIG;

AMX afxClassConfig const _AMX_MBUF_CLASS_CONFIG;
AMX afxClassConfig const _AMX_SND_CLASS_CONFIG;
AMX afxClassConfig const _AMX_AUD_CLASS_CONFIG;
AMX afxClassConfig const _AMX_SNDS_CLASS_CONFIG;

AMX afxClassConfig const _AMX_VID_CLASS_CONFIG;
AMX afxClassConfig const _AMX_VIDD_CLASS_CONFIG;
AMX afxClassConfig const _AMX_MDEC_CLASS_CONFIG;
AMX afxClassConfig const _AMX_MENC_CLASS_CONFIG;

#endif//AMX_SYSTEM_DDK_H
