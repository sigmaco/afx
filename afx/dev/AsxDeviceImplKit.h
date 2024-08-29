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

#ifndef ASX_DEVICE_IMPL_KIT_H
#define ASX_DEVICE_IMPL_KIT_H

#include "../dev/afxDevCoreBase.h"
#include "qwadro/sound/dev/afxSoundSystem.h"

AFX_DEFINE_STRUCT(afxSoundDeviceInfo)
{
    afxDeviceInfo       dev;
    afxNat              portCnt;
    afxSoundPortCaps const*portCaps;
    //afxSoundDeviceCaps  caps;
    //afxSoundDeviceLimits limits;
    afxClassConfig const*sdgeClsCfg;
    afxClassConfig const*sctxClsCfg;
    afxClassConfig const*sinClsCfg;
    afxClassConfig const*soutClsCfg;

    afxError(*stopCb)(afxSoundDevice);
    afxError(*startCb)(afxSoundDevice);
    afxError(*openCb)(afxSoundDevice, afxSoundContext, void** udd, afxNat invokeNo); // unused if a custom dctx ctor is installed.
    afxError(*closeCb)(afxSoundDevice, afxSoundContext); // unused if a custom dctx ctor is installed.

    afxError(*closeSdgeCb)(afxSoundDevice, afxSoundBridge);
    afxError(*openSdgeCb)(afxSoundDevice, afxSoundBridge, afxSoundBridgeConfig const*);

    afxError(*closeSinCb)(afxSoundDevice, afxSoundInput);
    afxError(*openSinCb)(afxSoundDevice, afxSoundInput, afxSoundInputConfig const*);
    afxError(*relinkSinCb)(afxSoundDevice, afxSoundContext, afxNat, afxSoundInput[]);

    afxError(*closeSoutCb)(afxSoundDevice, afxSoundOutput);
    afxError(*openSoutCb)(afxSoundDevice, afxSoundOutput, afxSoundOutputConfig const*);
    afxError(*relinkSoutCb)(afxSoundDevice, afxSoundContext, afxNat, afxSoundOutput[]);

};

#ifdef _ASX_SOUND_DEVICE_C
AFX_DEFINE_STRUCT(afxSoundOutputEndpoint)
{
    afxLinkage          sdev;
    afxChain            instances;
    afxMutex            mtx;
    afxCondition        cnd;
    afxString           name;
};

AFX_OBJECT(afxSoundDevice)
{
    AFX_OBJECT(afxDevice) dev;

    afxClass            sdgeCls;
    afxClass            sctxCls;
    afxClass            soutCls;
    afxClass            sinCls;
    afxChain            ineps;

    //afxSoundDeviceCaps const* caps;
    //afxSoundDeviceLimits const* limits;
    afxNat              portCnt;
    afxSoundPortCaps*   portCaps;

    afxBool             relinking;
    afxCondition        relinkedCnd;
    afxMutex            relinkedCndMtx;

    afxError(*stopCb)(afxSoundDevice);
    afxError(*startCb)(afxSoundDevice);
    afxError(*openCb)(afxSoundDevice, afxSoundContext, void** udd, afxNat invokeNo); // unused if a custom dctx ctor is installed.
    afxError(*closeCb)(afxSoundDevice, afxSoundContext); // unused if a custom dctx ctor is installed.
    
    afxError(*closeSdgeCb)(afxSoundDevice, afxSoundBridge);
    afxError(*openSdgeCb)(afxSoundDevice, afxSoundBridge, afxSoundBridgeConfig const*);

    afxError(*closeSinCb)(afxSoundDevice, afxSoundInput);
    afxError(*openSinCb)(afxSoundDevice, afxSoundInput, afxSoundInputConfig const*);
    afxError(*relinkSinCb)(afxSoundDevice, afxSoundContext, afxNat, afxSoundInput[]);
    
    afxError(*closeSoutCb)(afxSoundDevice, afxSoundOutput);
    afxError(*openSoutCb)(afxSoundDevice, afxSoundOutput, afxSoundOutputConfig const*);
    afxError(*relinkSoutCb)(afxSoundDevice, afxSoundContext, afxNat, afxSoundOutput[]);

    struct _afxSdevIdd* idd;
};
#endif//_ASX_SOUND_DEVICE_C

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

    afxError(*waitCb)(afxSoundBridge, afxNat);
    afxNat(*submitCb)(afxSoundBridge, asxSubmission const*, afxNat, asxTracker[]);
    afxNat(*transferCb)(afxSoundBridge, asxTransference const*, afxNat, void const*);
    afxNat(*flushCb)(afxSoundBridge, asxFlush const*, afxNat, afxSoundOutput[], afxNat const[]);
};
#endif//_ASX_SOUND_BRIDGE_C

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

    afxNat              exuCnt;
    afxSoundBridge*     exus;

    afxChain            inputs;
    afxChain            outputs;

    //afxChain            classes;
    afxClass            wavCls;
    afxClass            traxCls;

    afxV3d              listener;
    afxV3d              orientation;

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

#ifdef _ASX_SOUND_OUTPUT_C

AFX_OBJECT(afxSoundOutput)
{
    afxLinkage          sctx; // bound context
    afxAtom32           reconnecting;

    void*               udd; // user-defined data
    afxBool             (*getIddCb)(afxSoundOutput,afxNat,void*);

    afxClock            startClock;
    afxClock            lastClock;
    afxClock            outCntResetClock;

    struct _afxDoutIdd* idd;
};
#endif//_ASX_SOUND_OUTPUT_C

#ifdef _ASX_SOUND_INPUT_C
AFX_OBJECT(afxSoundInput)
{
    afxLinkage          sctx; // bound context
    afxBool             reconnecting;

    afxChain            classes;
    afxSoundInputProc   procCb;

    struct _afxDinIdd*  idd;
    void*               udd; // user-defined data
};
#endif//_ASX_SOUND_INPUT_C

ASX afxClassConfig const _AsxSqueStdImplementation;
ASX afxClassConfig const _AsxSdgeStdImplementation;
ASX afxClassConfig const _AsxSctxStdImplementation;

ASX afxError _AsxRegisterSoundDevices(afxDriver icd, afxNat cnt, afxSoundDeviceInfo const infos[], afxSoundDevice devices[]);

#endif//ASX_DEVICE_IMPL_KIT_H
