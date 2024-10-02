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
    afxNat              portIdx;
    afxNat              queueCnt;
    afxReal const*      queuePriority;
};

AFX_DEFINE_STRUCT(amxSubmission)
{
    afxNat              portIdx;
    afxNat              baseQueIdx;
    afxNat              queCnt;

    afxFlags            flags;
    afxSemaphore        waitSems;
    afxNat64            waitValues;
    avxPipelineStage    waitStageMasks;
    afxNat32            waitReserveds;
    afxSemaphore        signalSems;
    afxNat64            signalValues;
    avxPipelineStage    signalStageMasks;
    afxNat32            signalReserveds;
    afxFence            fence;
};

AFX_DEFINE_STRUCT(amxQueueOpData);

AFX_DEFINE_STRUCT(amxQueueOp)
{
    afxNat              submType;
    afxError            (*f)(void*, void*, afxNat, void*);
    void*               udd;
    afxNat              dataSiz;
    amxQueueOpData*     data;
};

AMX afxComboDevice       AfxGetMathBridgeDevice(afxComboBridge mdge);
AMX afxSimulation       AfxGetMathBridgeContext(afxComboBridge mdge);

AMX afxNat              AfxGetMathBridgePort(afxComboBridge mdge);

AMX void                _AmxBeginMathQueueDebugScope(afxComboBridge mdge, afxNat queIdx, afxString const* name, afxColor const color);
AMX void                _AmxPushMathQueueDebugLabel(afxComboBridge mdge, afxNat queIdx, afxString const* name, afxColor const color);
AMX void                _AmxEndMathQueueDebugScope(afxComboBridge mdge, afxNat queIdx);

AMX afxNat              _AmxSubmitMathCommands(afxComboBridge mdge, amxSubmission const* ctrl, afxNat cnt, avxCmdb cmdbs[]);
AMX afxNat              _AmxSubmitMathWorkRequest(afxComboBridge mdge, afxNat cnt, amxQueueOp const subm[]);

#endif//AMX_MATH_BRIDGE_H
