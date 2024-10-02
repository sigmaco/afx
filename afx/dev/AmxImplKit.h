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
#include "../dev/afxExecImplKit.h"
#include "qwadro/inc/sim/dev/afxComboSystem.h"
#include "qwadro/inc/sim/dev/afxSimulation.h"

#ifdef _AMX_SIMULATION_C
AFX_OBJECT(afxSimulation)
{
    afxBool running;

    afxNat exuCnt;
    afxComboBridge* exus;

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

AFX_DEFINE_STRUCT(afxComboDeviceInfo)
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

    afxError(*stopCb)(afxComboDevice);
    afxError(*startCb)(afxComboDevice);
    afxError(*openSimCb)(afxComboDevice, afxSimulation, void** udd, afxNat invokeNo); // unused if a custom dctx ctor is installed.
    afxError(*closeSimCb)(afxComboDevice, afxSimulation); // unused if a custom dctx ctor is installed.

    afxError(*closeMdgeCb)(afxComboDevice, afxComboBridge);
    afxError(*openMdgeCb)(afxComboDevice, afxComboBridge, afxComboBridgeConfig const*);
};

#ifdef _AMX_MATH_DEVICE_C
AFX_OBJECT(afxComboDevice)
{
    AFX_OBJECT(afxDevice) dev;

    afxClass            mdgeCls;
    afxClass            simCls;

    afxComboDeviceCaps const* caps;
    afxComboDeviceLimits const* limits;
    afxNat              portCnt;
    afxPragmaPortCaps*    portCaps;

    afxBool             relinking;
    afxCondition        relinkedCnd;
    afxMutex            relinkedCndMtx;

    afxError(*stopCb)(afxComboDevice);
    afxError(*startCb)(afxComboDevice);
    afxError(*openSimCb)(afxComboDevice, afxSimulation, void** udd, afxNat invokeNo); // unused if a custom dctx ctor is installed.
    afxError(*closeSimCb)(afxComboDevice, afxSimulation); // unused if a custom dctx ctor is installed.

    afxError(*closeMdgeCb)(afxComboDevice, afxComboBridge);
    afxError(*openMdgeCb)(afxComboDevice, afxComboBridge, afxComboBridgeConfig const*);

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
    afxComboBridge mdge; // owner bridge
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
AFX_OBJECT(afxComboBridge)
#endif
{
    afxSimulation sim; // owner
    afxNat portIdx;
    afxNat queCnt;
    afxMathQueue* queues;

    afxChain classes;
    afxClass mqueCls;

    afxError(*waitCb)(afxComboBridge, afxNat);
    afxNat(*submitCb)(afxComboBridge, amxSubmission const*, afxNat, avxCmdb[]);
    afxNat(*submCb)(afxComboBridge mdge, afxFence fenc, afxNat cnt, amxQueueOp const req[]);
};
#endif//_AMX_MATH_BRIDGE_C

AMX afxReal _AkxGetAllowedLodErrorFadingFactor(afxSimulation sim);
AMX afxError _AmxRegisterComboDevices(afxModule icd, afxNat cnt, afxComboDeviceInfo const infos[], afxComboDevice devices[]);

AMX afxClass const* _AmxGetMathQueueClass(afxComboBridge mdge);
AMX afxNat _AmxCountMathQueues(afxComboBridge mdge, afxNat baseQueIdx);
AMX afxBool _AmxGetMathQueue(afxComboBridge mdge, afxNat queIdx, afxMathQueue* queue);
AMX afxError _AmxWaitForIdleMathQueue(afxComboBridge mdge, afxNat queIdx);
AMX afxError _AmxWaitForIdleMathBridge(afxComboBridge mdge);

AMX afxClassConfig const _AmxSimStdImplementation;
AMX afxClassConfig const _AmxMdgeStdImplementation;
AMX afxClassConfig const _AmxMqueStdImplementation;

#endif//AMX_IMPL_KIT_H
