/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *       Q W A D R O   S O U N D   S Y N T H E S I S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA A4D <https://sigmaco.org/a4d>

#ifndef ASX_SOUND_BRIDGE_H
#define ASX_SOUND_BRIDGE_H

#include "qwadro/inc/sound/afxSoundDefs.h"

typedef enum afxSoundPortFlag
{
    afxSoundPortFlag_MIX = AFX_BIT(0), // supports mixing
    afxSoundPortFlag_SIM = AFX_BIT(1)
} afxSoundPortFlags;

AFX_DEFINE_STRUCT(afxSoundPortCaps)
{
    afxSoundPortFlags   capabilities;
    afxAcceleration     acceleration;
    afxUnit             minQueCnt; // usually 3
    afxUnit             maxQueCnt; // the count of queues in this port. Each port must support at least one queue.
};

AFX_DEFINE_STRUCT(afxSoundBridgeConfig)
{
    afxUnit             sdevId;
    afxSoundPortFlags   capabilities; // specifies capabilities of queues in a port.
    afxAcceleration     acceleration;
    afxUnit             minQueCnt;
    afxReal const*      queuePriority;
};

// the submission thing

AFX_DEFINE_STRUCT(asxSubmission)
{
    afxUnit             portIdx;
    afxUnit             baseQueIdx;
    afxUnit             queCnt;

    afxFlags            flags;
    afxSemaphore        waitSems;
    afxSemaphore        signalSems;
    afxFence            fence;
};

AFX_DEFINE_STRUCT(asxFlush)
{
    afxUnit             portIdx;
    afxUnit             baseQueIdx;
    afxUnit             queCnt;
    afxSemaphore        wait;
};

AFX_DEFINE_STRUCT(asxTransference)
{
    afxUnit             portIdx;
    afxUnit             baseQueIdx;
    afxUnit             queCnt;

    afxSemaphore        wait;
    afxSemaphore        signal;
    afxFence            fence;

    union
    {
        afxAudio    wav;
        afxBuffer   buf;
        void*       dst;
        void const* src;
        afxStream   iob;
    }               src;
    afxFcc          srcFcc;
    union
    {
        afxAudio    wav;
        afxBuffer   buf;
        void*       dst;
        void const* src;
        afxStream   iob;
    }               dst;
    afxFcc          dstFcc;

    afxCodec        codec;
    afxUnit         encSiz;
    afxUnit         decSiz;
};

ASX afxSoundDevice  AfxGetSoundBridgeDevice(afxSoundBridge sexu);
ASX afxSoundSystem  AfxGetSoundBridgeSystem(afxSoundBridge sexu);

ASX afxUnit         AfxQuerySoundBridgePort(afxSoundBridge sexu, afxSoundDevice* device);

ASX afxUnit         AfxGetSoundQueues(afxSoundBridge sexu, afxUnit first, afxUnit cnt, afxSoundQueue queues[]);

ASX afxError        AfxWaitForIdleSoundBridge(afxSoundBridge sexu, afxTime timeout);
ASX afxError        AfxWaitForEmptySoundQueue(afxSoundBridge sexu, afxUnit queIdx, afxTime timeout);

// SOUND QUEUE METHODS

ASX afxSoundDevice  AfxGetSoundQueueDevice(afxSoundQueue sque);
ASX afxSoundSystem  AfxGetSoundQueueContext(afxSoundQueue sque);
ASX afxUnit         AfxGetSoundQueuePort(afxSoundQueue sque);

#endif//ASX_SOUND_BRIDGE_H
