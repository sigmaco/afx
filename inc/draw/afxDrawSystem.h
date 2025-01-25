/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *        Q W A D R O   V I D E O   G R A P H I C S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// QWADRO is an open-source coalition of libraries that supports rapid development of software that deals with 4D data.
// The QWADRO frontend exposes a set of carefully selected data structures and algorithms in C.
// The backend is highly optimized and is set up for parallelization.
// We welcome contributions from the open-source community.

#ifndef AVX_DRAW_SYSTEM_H
#define AVX_DRAW_SYSTEM_H

#include "qwadro/inc/draw/afxDrawDevice.h"
#include "qwadro/inc/draw/op/afxDrawContext.h"
#include "qwadro/inc/draw/io/afxBuffer.h"
#include "qwadro/inc/draw/io/afxRaster.h"
#include "qwadro/inc/draw/pipe/avxCanvas.h"
#include "qwadro/inc/draw/pipe/avxLigature.h"
#include "qwadro/inc/draw/pipe/avxPipeline.h"
#include "qwadro/inc/draw/pipe/avxQueryPool.h"
#include "qwadro/inc/draw/pipe/avxPipeline.h"
#include "qwadro/inc/draw/pipe/avxSampler.h"
#include "qwadro/inc/draw/pipe/avxShader.h"
#include "qwadro/inc/draw/pipe/avxVertexDecl.h"
#include "qwadro/inc/draw/op/avxDrawOps.h"
#include "qwadro/inc/draw/op/avxFence.h"

AFX_DEFINE_STRUCT(afxDrawSystemConfig)
// The system-wide settings and parameters.
{
    afxDrawBridgeConfig prime;
    afxUnit             auxCnt;
    afxDrawBridgeConfig*auxs;

    afxDrawFeatures     requirements;
    afxUnit             extensionCnt;
    afxString const*    extensions;
    void*               udd;
};

AVX afxModule       AfxGetDrawSystemIcd(afxDrawSystem dsys);

// DRAW BRIDGES AND QUEUES.

AVX afxUnit         AfxGetDrawBridges(afxDrawSystem dsys, afxUnit baseExuIdx, afxUnit cnt, afxDrawBridge bridges[]);
AVX afxUnit         AfxQueryDrawBridges(afxDrawSystem dsys, afxUnit ddevId, afxUnit portId, afxUnit first, afxUnit cnt, afxDrawBridge bridges[]);

/// Wait for a device context to become idle.
/// Wait for a bridge to become idle. To wait on the host for the completion of outstanding queue operations for a given bridge.
/// Wait for a queue to become idle. To wait on the host for the completion of outstanding queue operations for a given queue.

AVX afxError        AfxWaitForDrawSystem(afxDrawSystem dsys, afxTime timeout);
AVX afxError        AfxWaitForDrawDevice(afxDrawSystem dsys, afxUnit devId, afxTime timeout);
AVX afxError        AfxWaitForDrawBridge(afxDrawSystem dsys, afxUnit exuIdx, afxTime timeout);
AVX afxError        AfxWaitForDrawQueue(afxDrawSystem dsys, afxUnit exuIdx, afxUnit queId, afxTime timeout);
AVX afxError        AfxWaitForDrawPort(afxDrawSystem dsys, afxUnit portId, afxTime timeout);

AVX afxError        AfxRollDrawCommands(afxDrawSystem dsys, avxSubmission* ctrl, afxUnit cnt, afxDrawContext contexts[], avxFence fence);

// DRAW INPUT/OUTPUT MECHANISM CONNECTIONS

AVX afxUnit         AfxEnumerateDrawOutputs(afxDrawSystem dsys, afxUnit first, afxUnit cnt, afxDrawOutput outputs[]);
AVX afxUnit         AfxEnumerateDrawInputs(afxDrawSystem dsys, afxUnit first, afxUnit cnt, afxDrawInput inputs[]);

AVX afxUnit         AfxInvokeDrawOutputs(afxDrawSystem dsys, afxUnit first, afxUnit cnt, afxBool(*f)(afxDrawOutput, void*), void *udd);
AVX afxUnit         AfxInvokeDrawInputs(afxDrawSystem dsys, afxUnit first, afxUnit cnt, afxBool(*f)(afxDrawInput, void*), void *udd);

AVX afxUnit         AfxEvokeDrawOutputs(afxDrawSystem dsys, afxBool(*f)(afxDrawOutput, void*), void* udd, afxUnit first, afxUnit cnt, afxDrawOutput outputs[]);
AVX afxUnit         AfxEvokeDrawInputs(afxDrawSystem dsys, afxBool(*f)(afxDrawInput, void*), void* udd, afxUnit first, afxUnit cnt, afxDrawInput inputs[]);

AVX afxError        AfxPresentDrawOutputs(afxDrawSystem dsys, avxPresentation* ctrl, avxFence wait, afxUnit cnt, afxDrawOutput outputs[], afxUnit const bufIdx[], avxFence signal[]);


////////////////////////////////////////////////////////////////////////////////

AVX afxUnit         AfxEnumerateDrawSystems(afxUnit icd, afxUnit first, afxUnit cnt, afxDrawSystem systems[]);
AVX afxUnit         AfxInvokeDrawSystems(afxUnit icd, afxUnit first, void *udd, afxBool(*f)(void*, afxDrawSystem), afxUnit cnt);
AVX afxUnit         AfxEvokeDrawSystems(afxUnit icd, afxUnit first, void* udd, afxBool(*f)(void*, afxDrawSystem), afxUnit cnt, afxDrawSystem systems[]);

AVX afxError        AfxConfigureDrawSystem(afxUnit icd, afxDrawSystemConfig* cfg);

AVX afxError        AfxEstablishDrawSystem(afxUnit icd, afxDrawSystemConfig const* cfg, afxDrawSystem* system);

#endif//AVX_DRAW_SYSTEM_H
