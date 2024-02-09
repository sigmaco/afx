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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

// This section is part of SIGMA GL/2.

#ifndef AFX_DRAW_QUEUE_H
#define AFX_DRAW_QUEUE_H

#include "qwadro/draw/pipe/afxPipeline.h"

// No QWADRO, uma fila é dividida em duas partes, afxDrawInput na vanguarda e afxDrawQueue na retaguarda.
// Ao criar um afxDrawContext, necessariamente ao menos uma afxDrawQueue foi criada consequentemente e associado ao mesmo.
// Isto porque, no QWADRO, você executa as operações de desenho. Ao invés, você submete elas a serem processadas através de um afxDrawInput.
// Uma afxDrawQueue livre, por sua vez, e que pode realizar determinada operação submetida, então puxará a submissão.

// A afxDrawQueue é uma unidade de gestão de transporte, onde se armazena a carga de trabalho até sua consumação.

AFX_DEFINE_STRUCT(_afxDqueSubm)
{
    afxLinkage              chain;
    afxNat                  submNo; // this submission number ordinal (B2F)
    afxNat                  reqSubmNo; // required submission num ordinal (need be executed before this). Usually submissions of resource benefiting of fetch priority.
    afxTime                 pushTime; // submission (into input) time
    afxTime                 pullTime; // fecth (by queue) time
    afxTime                 complTime; // completation time    
    struct
    {
        afxNat              scriptCnt;
        afxDrawScript       scripts[4];
        void*               data[4];
    };
    struct
    {
        afxNat              outputCnt;
        afxDrawOutput       outputs[4];
        afxNat              outBufIdx[4];
    };
    afxError                (*exec)(afxDrawQueue,_afxDqueSubm*);
};

AFX_DEFINE_STRUCT(afxDrawCommand)
{
    afxNat              cmdNo;
    afxLinkage          imm;
    afxLinkage          drawCall;
    afxLinkage          stateCh;
    afxLinkage          pipBind;
    afxLinkage          vbufBind;
    afxLinkage          ibufBind;
    afxLinkage          bufBind;
};

AFX_DEFINE_STRUCT(afxDrawSubmissionSpecification)
{
    afxNat              scriptCnt;
    afxDrawScript*      scripts;
    afxNat              outputCnt;
    afxDrawOutput*      outputs;
    afxNat*             outBufIdx;
    afxSemaphore*       wait;
    afxPipelineStage const*waitStage;
    afxSemaphore*       signal;
    void*               data;
};

typedef enum afxDrawQueueFlags
{
    AFX_DQUE_DRAW           = AfxGetBitOffset(0), // supports draw ops
    //AFX_DQUE_DRAW_AUX       = AfxGetBitOffset(1), // supports auxiliary draw ops (but can't perform a entire draw pipeline execution)
    AFX_DQUE_COMPUTE        = AfxGetBitOffset(2), // supports compute ops
    AFX_DQUE_TRANSFER       = AfxGetBitOffset(3), // supports transfer ops
    AFX_DQUE_VHS            = AfxGetBitOffset(4), // supports VHS enc/dec
} afxDrawQueueFlags;

#ifdef _AFX_DRAW_C
#ifdef _AFX_DRAW_QUEUE_C
#ifndef _AFX_DRAW_QUEUE_IMPL
AFX_OBJECT(afxDrawQueue)
#else
struct afxBaseDrawQueue
#endif
{
    afxDrawContext          dctx;
    afxNat                  portIdx;

    afxError                (*waitCb)(afxDrawQueue);

    afxSlock                pendingChainSlock;
    afxChain                pendingChain;

    afxSlock                reqLock;
    afxBool                 locked;
    afxBool                 immediate; // 0 = deferred, 1 = immediate
    afxSlock                arenaSlock;
    afxArena                cmdArena; // used by submission commands, not script commands.
};
#endif
#endif

AFX afxDrawDevice       AfxGetDrawQueueDevice(afxDrawQueue dque);
AFX afxDrawContext      AfxGetDrawQueueContext(afxDrawQueue dque);

AFX afxError            AfxWaitForIdleDrawQueue
/// Wait for a queue to become idle. To wait on the host for the completion of outstanding queue operations for a given queue.
(
    afxDrawQueue        dque /// the queue on which to wait.
); 

#endif//AFX_DRAW_QUEUE_H
