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

#ifndef AMX_SIM_BRIDGE_H
#define AMX_SIM_BRIDGE_H

#include "qwadro/inc/sim/afxSimDefs.h"

typedef enum afxSimPortFlags
{
    afxSimPortFlag_COMPUTE,
    afxSimPortFlag_KINEMA,
} afxSimPortFlags;

AFX_DEFINE_STRUCT(afxSimPortCaps)
{
    afxSimPortFlags     capabilities;
    afxAcceleration     acceleration;
    afxUnit             minQueCnt; // usually 3
    afxUnit             maxQueCnt; // the count of queues in this port. Each port must support at least one queue.
};

AFX_DEFINE_STRUCT(afxSimBridgeConfig)
{
    afxUnit             engId;
    afxSimPortFlags     capabilities;
    afxAcceleration     acceleration;
    afxUnit             minQueCnt;
    afxReal const*      queuePriority;
};

AFX_DEFINE_STRUCT(amxSubmission)
{
    afxUnit             portIdx;
    afxUnit             baseQueIdx;
    afxUnit             queCnt;

    afxFlags            flags;
    afxSemaphore        waitSems;
    afxUnit64           waitValues;
    avxPipelineStage    waitStageMasks;
    afxUnit32           waitReserveds;
    afxSemaphore        signalSems;
    afxUnit64           signalValues;
    avxPipelineStage    signalStageMasks;
    afxUnit32           signalReserveds;
    afxFence            fence;
};

AFX_DEFINE_STRUCT(amxQueueOpData);

AFX_DEFINE_STRUCT(amxQueueOp)
{
    afxUnit             submType;
    afxError            (*f)(void*, void*, afxUnit, void*);
    void*               udd;
    afxUnit             dataSiz;
    amxQueueOpData*     data;
};

AMX afxEngine           AfxGetMathBridgeDevice(afxSimBridge mexu);
AMX afxSimulation       AfxGetMathBridgeContext(afxSimBridge mexu);

AMX afxUnit             AfxQuerySimBridgePort(afxSimBridge mexu, afxEngine* engine);

AMX afxError            AfxWaitForIdleSimBridge(afxSimBridge mexu, afxTime timeout);

AMX afxUnit             AfxGetSimQueues(afxSimBridge mexu, afxUnit baseQueIdx, afxUnit cnt, afxSimQueue queues[]);

AMX void                _AmxBeginMathQueueDebugScope(afxSimBridge mexu, afxUnit queIdx, afxString const* name, afxColor const color);
AMX void                _AmxPushMathQueueDebugLabel(afxSimBridge mexu, afxUnit queIdx, afxString const* name, afxColor const color);
AMX void                _AmxEndMathQueueDebugScope(afxSimBridge mexu, afxUnit queIdx);

AMX afxUnit             _AmxSubmitMathCommands(afxSimBridge mexu, amxSubmission const* ctrl, afxUnit cnt, afxDrawContext cmdbs[]);
AMX afxUnit             _AmxSubmitMathWorkRequest(afxSimBridge mexu, afxUnit cnt, amxQueueOp const subm[]);

#endif//AMX_SIM_BRIDGE_H
