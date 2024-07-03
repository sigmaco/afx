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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef ASX_DEVICE_DEV_KIT_H
#define ASX_DEVICE_DEV_KIT_H

#include "../src/afx/dev/afxDevCoreBase.h"
#include "qwadro/sound/afxSoundSystem.h"

#ifdef _ASX_SOUND_CONTEXT_C
#ifdef _ASX_SOUND_CONTEXT_IMPL
AFX_OBJECT(_asxSoundContext)
#else
AFX_OBJECT(afxSoundContext)
#endif
{
    AFX_OBJECT(afxContext)  ctx;
    afxBool             running;
    afxArena            aren;

    afxNat              ownedBridgeCnt;
    afxSoundBridge*     ownedBridges;

    afxChain            inputs;
    afxChain            outputs;

    //afxChain            classes;
    afxClass          buffers;

    afxError(*waitCb)(afxSoundContext);

    struct _afxSctxIdd* idd;
    void*               udd; // user-defined data
};
#ifdef _ASX_SOUND_CONTEXT_IMPL
//AFX_STATIC_ASSERT(offsetof(struct _afxBaseSoundContext, ctx) == 0x00, "");
#else
//AFX_STATIC_ASSERT(offsetof(AFX_OBJECT(afxSoundContext), ctx) == 0x00, "");
#endif
#endif//_ASX_SOUND_CONTEXT_C

#ifdef _ASX_SOUND_QUEUE_C
#ifdef _ASX_SOUND_QUEUE_IMPL
AFX_OBJECT(_asxSoundQueue)
#else
AFX_OBJECT(afxSoundQueue)
#endif
{
    afxSoundBridge      sdge;
    afxSoundContext     sctx;
    afxBool             immediate; // 0 = deferred, 1 = immediate
    afxBool             closed; // can't enqueue

    afxSlock            workArenaSlock;
    afxArena            workArena; // used by submission of queue operations, not stream commands.        
    afxChain            workChn;
    afxMutex            workChnMtx;

    afxCondition        idleCnd;
    afxMutex            idleCndMtx;
};
#endif//_ASX_SOUND_QUEUE_C

#ifdef _ASX_SOUND_BRIDGE_C
#ifdef _ASX_SOUND_BRIDGE_IMPL
AFX_OBJECT(_asxSoundBridge)
#else
AFX_OBJECT(afxSoundBridge)
#endif
{
    afxSoundContext     sctx; // owner
    afxNat              portIdx;
    afxNat              queCnt;
    afxSoundQueue*      queues;
    afxChain            classes;
    afxClass            squeCls;

    afxError            (*waitCb)(afxSoundBridge,afxNat);
    afxNat              (*executeCb)(afxSoundBridge,afxFence,afxNat,afxExecutionRequest2 const[]);
    afxNat              (*transferCb)(afxSoundBridge,afxFence,afxNat,afxTransferRequest2 const[]);
};
#endif//_ASX_SOUND_BRIDGE_C



#endif//ASX_DEVICE_DEV_KIT_H
