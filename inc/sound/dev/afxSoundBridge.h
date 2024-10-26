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

// This code is part of SIGMA A4D <https://sigmaco.org/a4d>

#ifndef ASX_SOUND_BRIDGE_H
#define ASX_SOUND_BRIDGE_H

#include "qwadro/inc/sound/afxSoundDefs.h"

AFX_DEFINE_STRUCT(afxSoundBridgeConfig)
{
    afxFlags            flags;
    afxUnit              portIdx;
    afxUnit              queueCnt;
    afxReal const*      queuePriority;
};


AFX_DEFINE_STRUCT(asxSubmission)
{
    afxUnit              portIdx;
    afxUnit              baseQueIdx;
    afxUnit              queCnt;

    afxFlags            flags;
    afxSemaphore        waitSems;
    afxSemaphore        signalSems;
    afxFence            fence;
};

AFX_DEFINE_STRUCT(asxFlush)
{
    afxUnit              portIdx;
    afxUnit              baseQueIdx;
    afxUnit              queCnt;
    afxSemaphore        wait;
};

AFX_DEFINE_STRUCT(asxTransference)
{
    afxUnit              portIdx;
    afxUnit              baseQueIdx;
    afxUnit              queCnt;

    afxSemaphore        wait;
    afxSemaphore        signal;
    afxFence            fence;

    afxFcc              srcFcc;
    afxFcc              dstFcc;
    afxCodec            codec;
    afxUnit              decSiz;

    union
    {
        struct
        {
            afxWaveform     wav;
            //afxRasterIo     op;
            union
            {
                void*       dst;
                void const* src;
                //afxBuffer   buf;
                afxStream   iob;
            };
        }                   snd;
        struct
        {
            //afxBuffer       buf;
            //afxBufferIo     op;
            union
            {
                void*       dst;
                void const* src;
                afxStream   iob;
            };
        }                   data;
    };
};

ASX afxSoundDevice      AfxGetSoundBridgeDevice(afxSoundBridge sdge);
ASX afxSoundContext     AfxGetSoundBridgeContext(afxSoundBridge sdge);

ASX afxUnit              AfxGetSoundBridgePort(afxSoundBridge sdge);

ASX afxUnit              _AsxSubmitSoundCommands(afxSoundBridge sdge, asxSubmission const* ctrl, afxUnit cnt, asxTracker trackers[]);
ASX afxUnit              _AsxSubmitTransferences(afxSoundBridge sdge, asxTransference const* ctrl, afxUnit opCnt, void const* ops);
ASX afxUnit              _AsxSubmitSoundOutputFlushes(afxSoundBridge sdge, asxFlush const* ctrl, afxUnit cnt, afxSoundOutput outputs[], afxUnit const bufIdx[]);



#endif//ASX_SOUND_BRIDGE_H
