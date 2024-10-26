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


ASX afxUnit          AfxCountSoundBridges(afxSoundContext sctx, afxUnit baseBridIdx);
ASX afxUnit          AfxCountSoundQueues(afxSoundContext sctx, afxUnit portIdx, afxUnit baseQueIdx);

/// Get a bridge handle from a device context.
/// Get a queue handle from a device context.

ASX afxBool         AfxGetSoundBridge(afxSoundContext sctx, afxUnit portIdx, afxSoundBridge* bridge);

ASX afxBool         AfxGetSoundQueue(afxSoundContext sctx, afxUnit portIdx, afxUnit queIdx, afxSoundQueue* queue);

/// Wait for a device context to become idle.
/// Wait for a bridge to become idle. To wait on the host for the completion of outstanding queue operations for a given bridge.
/// Wait for a queue to become idle. To wait on the host for the completion of outstanding queue operations for a given queue.

ASX afxError        AfxWaitForSoundContext(afxSoundContext sctx);
ASX afxError        AfxWaitForIdleSoundBridge(afxSoundContext sctx, afxUnit portIdx);
ASX afxError        AfxWaitForIdleSoundQueue(afxSoundContext sctx, afxUnit portIdx, afxUnit queIdx);

ASX void            AfxCountSoundContextConnections(afxSoundContext sctx, afxUnit* inputCnt, afxUnit* outputCnt);
ASX afxError        AfxDisconnectSoundContext(afxSoundContext sctx, afxBool keepInputs, afxBool keepOutputs);

ASX afxUnit          AfxCountSoundInputConnections(afxSoundContext sctx);
ASX afxBool         AfxGetConnectedSoundInput(afxSoundContext sctx, afxUnit conNo, afxSoundInput* input);
ASX afxUnit          AfxGetConnectedSoundInputs(afxSoundContext sctx, afxUnit baseCon, afxUnit cnt, afxSoundInput inputs[]);
ASX afxUnit          AfxInvokeConnectedSoundInputs(afxSoundContext sctx, afxUnit baseCon, afxUnit cnt, afxBool(*f)(afxSoundInput, void*), void *udd);

ASX afxBool         AfxGetConnectedSoundOutput(afxSoundContext sctx, afxUnit conNo, afxSoundOutput* output);
ASX afxUnit          AfxGetConnectedSoundOutputs(afxSoundContext sctx, afxUnit baseCon, afxUnit cnt, afxSoundOutput outputs[]);
ASX afxUnit          AfxInvokeConnectedSoundOutputs(afxSoundContext sctx, afxUnit baseCon, afxUnit cnt, afxBool(*f)(afxSoundOutput, void*), void *udd);

#endif//ASX_SOUND_CONTEXT_H
