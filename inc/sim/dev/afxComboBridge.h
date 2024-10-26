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

#ifndef AMX_MATH_BRIDGE_H
#define AMX_MATH_BRIDGE_H

#include "afxSimulation.h"

typedef enum afxComboBridgeFlag
{
    afxComboBridgeFlag_
} afxComboBridgeFlags;

AFX_DEFINE_STRUCT(afxComboBridgeConfig)
{
    afxComboBridgeFlags  flags;
    afxUnit              portIdx;
    afxUnit              queueCnt;
    afxReal const*      queuePriority;
};

AFX_DEFINE_STRUCT(amxSubmission)
{
    afxUnit              portIdx;
    afxUnit              baseQueIdx;
    afxUnit              queCnt;

    afxFlags            flags;
    afxSemaphore        waitSems;
    afxUnit64            waitValues;
    avxPipelineStage    waitStageMasks;
    afxUnit32            waitReserveds;
    afxSemaphore        signalSems;
    afxUnit64            signalValues;
    avxPipelineStage    signalStageMasks;
    afxUnit32            signalReserveds;
    afxFence            fence;
};

AFX_DEFINE_STRUCT(amxQueueOpData);

AFX_DEFINE_STRUCT(amxQueueOp)
{
    afxUnit              submType;
    afxError            (*f)(void*, void*, afxUnit, void*);
    void*               udd;
    afxUnit              dataSiz;
    amxQueueOpData*     data;
};

AMX afxComboDevice       AfxGetMathBridgeDevice(afxComboBridge mdge);
AMX afxSimulation       AfxGetMathBridgeContext(afxComboBridge mdge);

AMX afxUnit              AfxGetMathBridgePort(afxComboBridge mdge);

AMX void                _AmxBeginMathQueueDebugScope(afxComboBridge mdge, afxUnit queIdx, afxString const* name, afxColor const color);
AMX void                _AmxPushMathQueueDebugLabel(afxComboBridge mdge, afxUnit queIdx, afxString const* name, afxColor const color);
AMX void                _AmxEndMathQueueDebugScope(afxComboBridge mdge, afxUnit queIdx);

AMX afxUnit              _AmxSubmitMathCommands(afxComboBridge mdge, amxSubmission const* ctrl, afxUnit cnt, avxCmdb cmdbs[]);
AMX afxUnit              _AmxSubmitMathWorkRequest(afxComboBridge mdge, afxUnit cnt, amxQueueOp const subm[]);

#endif//AMX_MATH_BRIDGE_H
