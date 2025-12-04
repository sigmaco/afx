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

// This file is part of Acceleration for RenderWare on Qwadro.

#ifndef ASX_SIM_BRIDGE_H
#define ASX_SIM_BRIDGE_H

#include "qwadro/sim/arxSimDefs.h"

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

AFX_DEFINE_STRUCT(asxSubmission)
{
    afxUnit             portIdx;
    afxUnit             baseQueIdx;
    afxUnit             queCnt;

    afxFlags            flags;
    afxSemaphore        waitSems;
    afxUnit64           waitValues;
    avxBusStage         waitStageMasks;
    afxUnit32           waitReserveds;
    afxSemaphore        signalSems;
    afxUnit64           signalValues;
    avxBusStage         signalStageMasks;
    afxUnit32           signalReserveds;
    avxFence            fence;
};

AFX_DEFINE_STRUCT(asxQueueOpData);

AFX_DEFINE_STRUCT(asxQueueOp)
{
    afxUnit             submType;
    afxError            (*f)(void*, void*, afxUnit, void*);
    void*               udd;
    afxUnit             dataSiz;
    asxQueueOpData*     data;
};

ASX arxEngine           ArxGetMathBridgeDevice(arxSimBridge sexu);
ASX arxSimulation       ArxGetMathBridgeContext(arxSimBridge sexu);

ASX afxUnit             ArxQuerySimBridgePort(arxSimBridge sexu, arxEngine* engine);

ASX afxError            ArxWaitForIdleSimBridge(arxSimBridge sexu, afxTime timeout);

ASX afxUnit             ArxGetSimQueues(arxSimBridge sexu, afxUnit baseQueIdx, afxUnit cnt, arxSimQueue queues[]);

ASX void                _AsxBeginMathQueueDebugScope(arxSimBridge sexu, afxUnit queIdx, afxString const* name, avxColor const color);
ASX void                _AsxPushMathQueueDebugLabel(arxSimBridge sexu, afxUnit queIdx, afxString const* name, avxColor const color);
ASX void                _AsxEndMathQueueDebugScope(arxSimBridge sexu, afxUnit queIdx);

ASX afxUnit             _AsxSubmitMathCommands(arxSimBridge sexu, asxSubmission const* ctrl, afxUnit cnt, afxDrawContext cmdbs[]);
ASX afxUnit             _AsxSubmitMathWorkRequest(arxSimBridge sexu, afxUnit cnt, asxQueueOp const subm[]);

#endif//ASX_SIM_BRIDGE_H
