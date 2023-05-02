/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

// afxDrawInput devices operates like device submission queues grouping sets of draw streams and present their result to the connected draw output devices.

// No QWADRO, h� um conceito de uma fila de submiss�o de trabalho para a GPU. Por�m, diferentemente do Vulkan, no QWADRO esta "fila" � separada em dois.
// A primeira parte cont�m a demanda e a segunda parte o estado da fila. Isto porque assim podemos usar a mesma demanda para dois contextos diferentes, cada qual contendo a outra parte, e logo, seu estado particular.

#ifndef AFX_DRAW_INPUT_H
#define AFX_DRAW_INPUT_H

#include "afx/draw/afxDrawScript.h"
#include "afx/draw/afxDrawOutput.h"

#include "afx/draw/pipelining/afxLego.h"
#include "afx/draw/pipelining/afxPipelineBlueprint.h"
#include "afx/draw/pipelining/afxSampler.h"
#include "afx/draw/res/afxBuffer.h"
#include "afx/draw/res/afxCanvas.h"
#include "afx/draw/res/afxIndexBuffer.h"
#include "afx/draw/res/afxVertexBuffer.h"
#include "afx/draw/afxDrawScript.h"
#include "afx/draw/res/afxTextureBlueprint.h"
#include "afx/core/mem/afxQueue.h"

AFX_DEFINE_HANDLE(afxDrawInput);

AFX_DEFINE_STRUCT(afxDrawInputSpecification)
{
    afxNat              cmdPoolMemStock;
    afxNat              estimatedSubmissionCnt;
    afxNat const*       enabledPresentationThreads;
    afxNat const*       enabledSubmissionThreads;
    afxNat const*       enabledStreamingThreads;
    afxError            (*prefetch)(afxDrawInput din, afxNat queueIdx, void *udd);
    void*               udd;
};

#ifndef AFX_DRAW_DRIVER_SRC

AFX_OBJECT(afxDrawInput)
{
    afxObject           obj;
};

#endif

AFX afxDrawScript       AfxDrawInputAcquireScript(afxDrawInput din, afxBool recycle);

AFX void*               AfxDrawInputGetContext(afxDrawInput din);
AFX void*               AfxDrawInputGetDriver(afxDrawInput din);
AFX void*               AfxDrawInputGetDrawSystem(afxDrawInput din);

AFX afxError            AfxDrawInputEnqueueStreaming(afxDrawInput din, afxNat objectCnt, afxObject *objects[]);
AFX afxError            AfxDrawInputEnqueueSubmission(afxDrawInput din, afxNat scriptCnt, afxDrawScript scripts[]);
AFX afxError            AfxDrawInputEnqueuePresentation(afxDrawInput din, afxNat outputCnt, afxDrawOutput outputs[], afxNat outputBufIdx[]);

AFX afxError            AfxDrawInputAffineStreamingThreads(afxDrawInput din, afxNat base, afxNat cnt, afxNat const enabled[]);
AFX afxError            AfxDrawInputAffineSubmissionThreads(afxDrawInput din, afxNat base, afxNat cnt, afxNat const enabled[]);
AFX afxError            AfxDrawInputAffinePresentationThreads(afxDrawInput din, afxNat base, afxNat cnt, afxNat const enabled[]);
AFX afxError            AfxDrawInputAffinePrefetchThreads(afxDrawInput din, afxNat base, afxNat cnt, afxNat const enabled[]);

#endif//AFX_DRAW_INPUT_H