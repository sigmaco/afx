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

// This section is part of SIGMA Simulation Infrastructure.

#ifndef AMX_IMPL_KIT_H
#define AMX_IMPL_KIT_H

#include "AmxPuppetImplKit.h"
#include "AmxCadImplKit.h"
#include "../dev/afxDevCoreBase.h"
#include "qwadro/inc/sim/dev/afxPragmaSystem.h"
#include "qwadro/inc/sim/dev/afxSimulation.h"

#ifdef _AMX_SIMULATION_C
AFX_OBJECT(afxSimulation)
{
    afxBool running;

    afxNat exuCnt;
    afxPragmaBridge* exus;

    afxChain        classes;
    afxClass      bodies;
    afxClass      motors;
    afxClass      lights;

    afxClass      motMgr;
    afxClass      aniMgr;
    afxClass      nodes;
    afxClass      renderers;
    afxClass      terCls;

    afxClass            sklCls;
    afxClass            poseCls;
    afxClass            posbCls;
    afxClass            curCls;

    afxClass            mtlCls;
    afxClass            mdlCls;

    afxStringBase   strbMdlSklMotUrns;

    afxV4d          right;
    afxV4d          up;
    afxV4d          back;
    afxM3d          basis;
    afxV4d          origin;
    afxBox         extent;
    afxReal         unitsPerMeter;
    afxReal         allowedLodErrFadingFactor;

    afxSize         stepNum, stepsPerSecond;
    afxSize         stepTime, swapTime;
    afxReal         stepDeltaTime;

    akxMotive       globalMotorInterlinks;

    afxError(*waitCb)(afxSimulation);

    afxDrawContext  dctx;
    afxDrawInput    din;

    struct _afxSimIdd* idd;
    void* udd; // user-defined data
};
#endif//_AMX_SIMULATION_C

AFX_DEFINE_STRUCT(afxPragmaDeviceInfo)
{
    afxDeviceInfo           dev;
    afxNat                  portCnt;
    afxPragmaPortCaps const*  portCaps;
    //afxSoundDeviceCaps  caps;
    //afxSoundDeviceLimits limits;
    afxClassConfig const*   mdgeClsCfg;
    afxClassConfig const*   simClsCfg;
    afxClassConfig const*   sinClsCfg;
    afxClassConfig const*   soutClsCfg;

    afxError(*stopCb)(afxPragmaDevice);
    afxError(*startCb)(afxPragmaDevice);
    afxError(*openSimCb)(afxPragmaDevice, afxSimulation, void** udd, afxNat invokeNo); // unused if a custom dctx ctor is installed.
    afxError(*closeSimCb)(afxPragmaDevice, afxSimulation); // unused if a custom dctx ctor is installed.

    afxError(*closeMdgeCb)(afxPragmaDevice, afxPragmaBridge);
    afxError(*openMdgeCb)(afxPragmaDevice, afxPragmaBridge, afxPragmaBridgeConfig const*);
};

#ifdef _AMX_MATH_DEVICE_C
AFX_OBJECT(afxPragmaDevice)
{
    AFX_OBJECT(afxDevice) dev;

    afxClass            mdgeCls;
    afxClass            simCls;

    afxPragmaDeviceCaps const* caps;
    afxPragmaDeviceLimits const* limits;
    afxNat              portCnt;
    afxPragmaPortCaps*    portCaps;

    afxBool             relinking;
    afxCondition        relinkedCnd;
    afxMutex            relinkedCndMtx;

    afxError(*stopCb)(afxPragmaDevice);
    afxError(*startCb)(afxPragmaDevice);
    afxError(*openSimCb)(afxPragmaDevice, afxSimulation, void** udd, afxNat invokeNo); // unused if a custom dctx ctor is installed.
    afxError(*closeSimCb)(afxPragmaDevice, afxSimulation); // unused if a custom dctx ctor is installed.

    afxError(*closeMdgeCb)(afxPragmaDevice, afxPragmaBridge);
    afxError(*openMdgeCb)(afxPragmaDevice, afxPragmaBridge, afxPragmaBridgeConfig const*);

    struct _afxSdevIdd* idd;
};
#endif//_AMX_MATH_DEVICE_C

#ifdef _AMX_MATH_QUEUE_C
#ifdef _AMX_MATH_QUEUE_IMPL
AFX_OBJECT(_amxMathQueue)
#else
AFX_OBJECT(afxMathQueue)
#endif
{
    afxPragmaBridge mdge; // owner bridge
    afxSimulation sim; // owner context
    afxBool immediate; // 0 = deferred, 1 = immediate
    afxBool closed; // can't enqueue

    afxSlock workArenaSlock;
    afxArena workArena; // used by submission of queue operations, not stream commands.        
    afxChain workChn;
    afxMutex workChnMtx;

    afxCondition idleCnd;
    afxMutex idleCndMtx;
};
#endif//_AMX_MATH_QUEUE_C

#ifdef _AMX_MATH_BRIDGE_C
#ifdef _AMX_MATH_BRIDGE_IMPL
AFX_OBJECT(_amxMathBridge)
#else
AFX_OBJECT(afxPragmaBridge)
#endif
{
    afxSimulation sim; // owner
    afxNat portIdx;
    afxNat queCnt;
    afxMathQueue* queues;

    afxChain classes;
    afxClass mqueCls;

    afxError(*waitCb)(afxPragmaBridge, afxNat);
    afxNat(*submitCb)(afxPragmaBridge, amxSubmission const*, afxNat, avxCmdb[]);
    afxNat(*submCb)(afxPragmaBridge mdge, afxFence fenc, afxNat cnt, amxQueueOp const req[]);
};
#endif//_AMX_MATH_BRIDGE_C

AMX afxReal _AkxGetAllowedLodErrorFadingFactor(afxSimulation sim);
AMX afxError _AmxRegisterPragmaDevices(afxDriver icd, afxNat cnt, afxPragmaDeviceInfo const infos[], afxPragmaDevice devices[]);

AMX afxClass const* _AmxGetMathQueueClass(afxPragmaBridge mdge);
AMX afxNat _AmxCountMathQueues(afxPragmaBridge mdge, afxNat baseQueIdx);
AMX afxBool _AmxGetMathQueue(afxPragmaBridge mdge, afxNat queIdx, afxMathQueue* queue);
AMX afxError _AmxWaitForIdleMathQueue(afxPragmaBridge mdge, afxNat queIdx);
AMX afxError _AmxWaitForIdleMathBridge(afxPragmaBridge mdge);

AMX afxClassConfig const _AmxSimStdImplementation;
AMX afxClassConfig const _AmxMdgeStdImplementation;
AMX afxClassConfig const _AmxMqueStdImplementation;

#endif//AMX_IMPL_KIT_H
