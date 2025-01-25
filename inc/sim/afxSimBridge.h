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

#ifndef ASX_SIM_BRIDGE_H
#define ASX_SIM_BRIDGE_H

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

AFX_DEFINE_STRUCT(asxSubmission)
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

ASX afxEngine           AfxGetMathBridgeDevice(afxSimBridge sexu);
ASX afxSimulation       AfxGetMathBridgeContext(afxSimBridge sexu);

ASX afxUnit             AfxQuerySimBridgePort(afxSimBridge sexu, afxEngine* engine);

ASX afxError            AfxWaitForIdleSimBridge(afxSimBridge sexu, afxTime timeout);

ASX afxUnit             AfxGetSimQueues(afxSimBridge sexu, afxUnit baseQueIdx, afxUnit cnt, afxSimQueue queues[]);

ASX void                _AsxBeginMathQueueDebugScope(afxSimBridge sexu, afxUnit queIdx, afxString const* name, afxColor const color);
ASX void                _AsxPushMathQueueDebugLabel(afxSimBridge sexu, afxUnit queIdx, afxString const* name, afxColor const color);
ASX void                _AsxEndMathQueueDebugScope(afxSimBridge sexu, afxUnit queIdx);

ASX afxUnit             _AsxSubmitMathCommands(afxSimBridge sexu, asxSubmission const* ctrl, afxUnit cnt, afxDrawContext cmdbs[]);
ASX afxUnit             _AsxSubmitMathWorkRequest(afxSimBridge sexu, afxUnit cnt, asxQueueOp const subm[]);

#endif//ASX_SIM_BRIDGE_H
