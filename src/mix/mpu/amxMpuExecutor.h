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

#ifndef AMX_IMPL___EXECUTOR_H
#define AMX_IMPL___EXECUTOR_H

#include "../amxBridgeDDK.h"
#include "../amxContextDDK.h"
#include "../amxQueueDDK.h"

AFX_DEFINE_STRUCT(amxMpu)
{
    afxUnit         portId; // exuIdx
    afxUnit         exuIdx;
    afxMixBridge  mexu;
    afxThread       thr;
    afxMixSystem  msys;

    afxBool         running;
    afxBool         instanced;
    afxSize         numOfFedSamples;

    afxMixContext        mix;
    afxReal         bufA[48000];
    afxReal         bufB[48000];
    
    afxUnit         samplesPerFrame;
    afxUnit         tmpBufCap;
    amxAudio        a, b;
    afxUnit         queuedFrameCnt;

    afxUnit         mixFreq; // the mix frequency used in a mix scope.
    amxAudio        sinkBuf; // the audio's sink buffer object.
    afxUnit         sinkBufIdx;
    afxUnit         sinkBaseSamp;
    afxUnit         sinkBaseSeg;
    afxUnit         sinkSampCnt;
    afxUnit         sinkSegCnt;
    afxUnit         sinkBaseChan;
    afxUnit         sinkChanCnt;
    amxMixTarget      sinkChans[32]; // our SPU supports at least 32 sink channels.
    //arxSampleContext  sampCtx;
};

// standard sound port methods

AMX _amxIoReqLut const _AMX_MPU_IORP_VMT;
AMX afxInt   _AMX_MPU_THREAD_PROC(afxMixBridge mexu);
AMX afxBool  _AmxMpu_ProcCb(amxMpu*);

AMX afxError _AmxMpuRollMixContexts(amxMpu* mpu, afxMixContext mix);

#endif//AMX_IMPL___EXECUTOR_H
