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

#include "qwadro/sound/afxSoundContext.h"

AFX_DEFINE_STRUCT(afxExecutionRequest2)
{
    afxSemaphore        wait;
    //afxSoundStream      cmdb;
    afxSemaphore        signal;
};

AFX_DEFINE_STRUCT(afxTransferRequest2)
{
    afxFcc              srcFcc;
    afxFcc              dstFcc;

    afxCodec            codec;
    afxNat              decompressedSiz;

    afxSemaphore        wait;
    afxSemaphore        signal;
};

AFX_DEFINE_STRUCT(afxSoundBridgeConfig)
{
    afxFlags            flags;
    afxNat              portIdx;
    afxNat              queueCnt;
    afxReal const*      queuePriority;
};

#ifdef _ASX_SOUND_C
#ifdef _ASX_SOUND_BRIDGE_C
AFX_DEFINE_STRUCT(afxSoundQueue)
{
    afxSoundBridge      sdge;
    afxBool             immediate; // 0 = deferred, 1 = immediate
    afxBool             closed; // can't enqueue

    afxSlock            workArenaSlock;
    afxArena            workArena; // used by submission of queue operations, not stream commands.        
    afxChain            workChn;
    afxMutex            workChnMtx;

    afxCondition        idleCnd;
    afxMutex            idleCndMtx;

    // one command pool per queue to avoid thread interation.
    // one stream manager per queue to avoid thread interaction.

    afxManager          streams;
    afxQueue            recycQue;
    afxSlock            reqLock;
    afxBool             lockedForReq;
};
#ifndef _ASX_SOUND_BRIDGE_IMPL
AFX_OBJECT(afxSoundBridge)
#else
struct _afxBaseSoundBridge
#endif
{
    afxSoundContext     sctx; // owner
    afxNat              portIdx;
    afxNat              queCnt;
    afxSoundQueue*      queues;
    afxChain            managers;

    afxError            (*waitCb)(afxSoundBridge,afxNat);
    afxNat              (*executeCb)(afxSoundBridge,afxFence,afxNat,afxExecutionRequest2 const[]);
    afxNat              (*transferCb)(afxSoundBridge,afxFence,afxNat,afxTransferRequest2 const[]);
};
#endif//_ASX_SOUND_BRIDGE_C
#endif//_ASX_SOUND_C

ASX afxSoundDevice      AfxGetSoundBridgeDevice(afxSoundBridge sdge);
ASX afxSoundContext     AfxGetSoundBridgeContext(afxSoundBridge sdge);

ASX afxNat              AfxCountSoundQueues(afxSoundBridge sdge);

ASX afxError            AfxWaitForIdleSoundQueue
/// Wait for a queue to become idle. To wait on the host for the completion of outstanding queue operations for a given queue.
(
    afxSoundBridge      sdge, /// the bridge on which to wait.
    afxNat              queIdx /// the queue on which to wait.
);

#endif//ASX_SOUND_BRIDGE_H
