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

#ifndef ASX_SOUND_CONTEXT_H
#define ASX_SOUND_CONTEXT_H

#include "qwadro/mem/afxArena.h"
#include "qwadro/base/afxClass.h"
#include "qwadro/sound/asxBuffer.h"
#include "qwadro/sound/asxSound.h"
#include "qwadro/sound/dev/afxSoundOutput.h"
#include "qwadro/sound/dev/afxSoundDevice.h"
#include "qwadro/sound/dev/afxSoundBridge.h"

AFX_DEFINE_STRUCT(afxSoundContextConfig)
{
    afxSoundBridgeConfig    mainBridge;
    afxNat                  auxBridgeCnt;
    afxSoundBridgeConfig*   auxBridges;
    
    void*                   udd;
};

ASX afxSoundDevice      AfxGetSoundContextDevice(afxSoundContext sctx);

ASX afxNat              AfxCountSoundBridges(afxSoundContext sctx);
ASX afxNat              AfxCountSoundQueues(afxSoundContext sctx, afxNat portIdx);

ASX afxBool             AfxGetSoundBridge
/// Get a bridge handle from a device context.
(
    afxSoundContext     sctx,
    afxNat              portIdx, /// the index of the bridge.
    afxSoundBridge*     bridge /// a pointer to a afxSoundBridge object that will be filled with the handle for the requested bridge.
);

ASX afxBool             AfxGetSoundQueue
/// Get a queue handle from a device context.
(
    afxSoundContext     sctx,
    afxNat              portIdx, /// the index of the bridge to which the queue belongs.
    afxNat              queIdx, /// the index within this bridge of the queue to retrieve.
    afxSoundQueue*      queue /// a pointer to a afxSoundQueue object that will be filled with the handle for the requested queue.
);

ASX afxError            AfxWaitForSoundContext
/// Wait for a device context to become idle.
(
    afxSoundContext     sctx
);

ASX afxError            AfxWaitForIdleSoundBridge
/// Wait for a bridge to become idle. To wait on the host for the completion of outstanding queue operations for a given bridge.
(
    afxSoundContext     sctx,
    afxNat              portIdx /// the bridge on which to wait.
);

ASX afxError            AfxWaitForIdleSoundQueue
/// Wait for a queue to become idle. To wait on the host for the completion of outstanding queue operations for a given queue.
(
    afxSoundContext     sctx,
    afxNat              portIdx, /// the bridge on which to wait.
    afxNat              queIdx /// the queue on which to wait.
);

ASX afxNat              AfxCountSoundOutputConnections(afxSoundContext sctx);
ASX afxNat              AfxCountSoundInputConnections(afxSoundContext sctx);
ASX afxBool             AfxGetConnectedSoundInput(afxSoundContext sctx, afxNat conNo, afxSoundInput* input);
ASX afxBool             AfxGetConnectedSoundOutput(afxSoundContext sctx, afxNat conNo, afxSoundOutput* output);
ASX afxNat              AfxInvokeConnectedSoundInputs(afxSoundContext sctx, afxNat baseCon, afxNat conCnt, afxBool(*f)(afxSoundInput, void*), void *udd);
ASX afxNat              AfxInvokeConnectedSoundOutputs(afxSoundContext sctx, afxNat baseCon, afxNat conCnt, afxBool(*f)(afxSoundOutput, void*), void *udd);
ASX afxNat              AfxEnumerateConnectedSoundInputs(afxSoundContext sctx, afxNat baseCon, afxNat conCnt, afxSoundInput inputs[]);
ASX afxNat              AfxEnumerateConnectedSoundOutputs(afxSoundContext sctx, afxNat baseCon, afxNat conCnt, afxSoundOutput outputs[]);
ASX afxError            AfxDisconnectSoundContext(afxSoundContext sctx, afxBool keepInputs, afxBool keepOutputs);

////////////////////////////////////////////////////////////////////////////////

ASX afxError            AfxConfigureSoundDevice(afxNat sdevId, afxSoundContextConfig* cfg);

ASX afxError            AfxOpenSoundDevice(afxNat sdevId, afxSoundContextConfig const* cfg, afxSoundContext* context);

#endif//ASX_SOUND_CONTEXT_H
