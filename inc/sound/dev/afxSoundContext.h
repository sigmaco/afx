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

#include "qwadro/inc/mem/afxArena.h"
#include "qwadro/inc/base/afxClass.h"
#include "qwadro/inc/sound/afxWaveform.h"
#include "qwadro/inc/sound/asxSound.h"
#include "qwadro/inc/sound/dev/afxSoundOutput.h"
#include "qwadro/inc/sound/dev/afxSoundBridge.h"


ASX afxClass const* AfxGetTrackerClass(afxSoundContext sctx);
ASX afxClass const* AfxGetWaveformClass(afxSoundContext sctx);

ASX afxSoundDevice  AfxGetSoundContextDevice(afxSoundContext sctx);


ASX afxNat          AfxCountSoundBridges(afxSoundContext sctx, afxNat baseBridIdx);
ASX afxNat          AfxCountSoundQueues(afxSoundContext sctx, afxNat portIdx, afxNat baseQueIdx);

/// Get a bridge handle from a device context.
/// Get a queue handle from a device context.

ASX afxBool         AfxGetSoundBridge(afxSoundContext sctx, afxNat portIdx, afxSoundBridge* bridge);

ASX afxBool         AfxGetSoundQueue(afxSoundContext sctx, afxNat portIdx, afxNat queIdx, afxSoundQueue* queue);

/// Wait for a device context to become idle.
/// Wait for a bridge to become idle. To wait on the host for the completion of outstanding queue operations for a given bridge.
/// Wait for a queue to become idle. To wait on the host for the completion of outstanding queue operations for a given queue.

ASX afxError        AfxWaitForSoundContext(afxSoundContext sctx);
ASX afxError        AfxWaitForIdleSoundBridge(afxSoundContext sctx, afxNat portIdx);
ASX afxError        AfxWaitForIdleSoundQueue(afxSoundContext sctx, afxNat portIdx, afxNat queIdx);

ASX void            AfxCountSoundContextConnections(afxSoundContext sctx, afxNat* inputCnt, afxNat* outputCnt);
ASX afxError        AfxDisconnectSoundContext(afxSoundContext sctx, afxBool keepInputs, afxBool keepOutputs);

ASX afxNat          AfxCountSoundInputConnections(afxSoundContext sctx);
ASX afxBool         AfxGetConnectedSoundInput(afxSoundContext sctx, afxNat conNo, afxSoundInput* input);
ASX afxNat          AfxGetConnectedSoundInputs(afxSoundContext sctx, afxNat baseCon, afxNat cnt, afxSoundInput inputs[]);
ASX afxNat          AfxInvokeConnectedSoundInputs(afxSoundContext sctx, afxNat baseCon, afxNat cnt, afxBool(*f)(afxSoundInput, void*), void *udd);

ASX afxBool         AfxGetConnectedSoundOutput(afxSoundContext sctx, afxNat conNo, afxSoundOutput* output);
ASX afxNat          AfxGetConnectedSoundOutputs(afxSoundContext sctx, afxNat baseCon, afxNat cnt, afxSoundOutput outputs[]);
ASX afxNat          AfxInvokeConnectedSoundOutputs(afxSoundContext sctx, afxNat baseCon, afxNat cnt, afxBool(*f)(afxSoundOutput, void*), void *udd);

#endif//ASX_SOUND_CONTEXT_H
