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

/// No QWADRO, uma fila é dividida em duas partes, afxDrawInput na vanguarda e afxDrawBridge na retaguarda.
/// Ao criar um afxDrawContext, necessariamente ao menos uma afxDrawBridge foi criada consequentemente e associado ao mesmo.
/// Isto porque, no QWADRO, você não executa as operações de desenho. Ao invés, você submete elas a serem processadas através de um afxDrawInput.
/// Uma afxDrawBridge livre, por sua vez, e que pode realizar determinada operação submetida, então puxará a submissão.

/// A afxDrawBridge é uma unidade de gestão de transporte, onde se armazena a carga de trabalho até sua consumação.

#ifndef AFX_DRAW_BRIDGE_H
#define AFX_DRAW_BRIDGE_H

#include "qwadro/draw/pipe/afxPipeline.h"
#include "qwadro/draw/io/afxRaster.h"

typedef enum afxDrawBridgeFlags
{
    afxDrawBridgeFlag_DRAW       = AFX_BIT_OFFSET(0), // supports draw ops
  //afxDrawBridgeFlag_DRAW_AUX   = AFX_BIT_OFFSET(1), // supports auxiliary draw ops (but can't perform a entire draw pipeline execution)
    afxDrawBridgeFlag_COMPUTE    = AFX_BIT_OFFSET(2), // supports compute ops
    afxDrawBridgeFlag_TRANSFER  = AFX_BIT_OFFSET(3), // supports transfer ops
    afxDrawBridgeFlag_VHS       = AFX_BIT_OFFSET(4), // supports VHS enc/dec
} afxDrawBridgeFlags;

AFX_DEFINE_STRUCT(afxDrawBridgeConfig)
{
    afxDrawBridgeFlags  flags;
    afxNat              queueCnt;
    afxReal const*      queuePriority;
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
    afxDrawStream*      scripts;
    afxNat              outputCnt;
    afxDrawOutput*      outputs;
    afxNat*             outBufIdx;
    afxSemaphore*       wait;
    afxPipelineStage const*waitStage;
    afxSemaphore*       signal;
    void*               data;
};

AFX_DEFINE_STRUCT(afxExecutionRequest)
{
    afxSemaphore        wait;
    afxDrawStream       diob;
    afxSemaphore        signal;
};

AFX_DEFINE_STRUCT(afxPresentationRequest)
{
    afxSemaphore        wait;
    afxDrawOutput       dout;
    afxNat              bufIdx;
};

AFX_DEFINE_STRUCT(afxTransferRequest)
{
    afxSemaphore        wait;
    struct
    {
        union
        {
            void const* buf;
            struct
            {
                afxStream   iob;
                afxSize offset;
            };
        };
        afxNat          siz;
    }                   src;
    union
    {
        struct
        {
            afxBuffer   obj;
            afxSize     offset;
            afxNat      siz;
        }               buf;
        struct
        {
            afxRaster   obj;
            afxRasterRegion rgn;
        }               ras;
    }                   dst;
    afxCodec            codec;
    afxNat              uncompressedSiz;
    afxSemaphore        signal;
};

#ifdef _AFX_DRAW_C
#ifdef _AFX_DRAW_BRIDGE_C
#ifndef _AFX_DRAW_BRIDGE_IMPL
AFX_OBJECT(afxDrawBridge)
#else
struct afxBaseDrawBridge
#endif
{
    afxLinkage          dctx; // owner
    afxNat              portIdx;

    afxError            (*waitCb)(afxDrawBridge,afxNat);

    struct
    {
        afxSlock        reqLock;
        afxBool         locked;
        afxBool         immediate; // 0 = deferred, 1 = immediate
        afxSlock        arenaSlock;
        afxArena        cmdArena; // used by submission commands, not script commands.
        afxSlock        pendingChainSlock;
        afxChain        pendingChain;
    }*                  queues;
    afxNat              queueCnt;

    afxChain            managements;
    afxManager          scripts;
};
#endif
#endif

AVX afxDrawDevice       AfxGetDrawQueueDevice(afxDrawBridge dxge);
AVX afxDrawContext      AfxGetDrawQueueContext(afxDrawBridge dxge);

AVX afxError            AfxWaitForIdleDrawQueue
/// Wait for a queue to become idle. To wait on the host for the completion of outstanding queue operations for a given queue.
(
    afxDrawBridge       dxge, /// the bridge on which to wait.
    afxNat              queueIdx /// the queue on which to wait.
);

AVX afxError            AfxEnqueueExecutionRequest(afxDrawBridge dxge, afxFence fenc, afxNat cnt, afxExecutionRequest const req[]);
AVX afxError            AfxEnqueuePresentRequest(afxDrawBridge dxge);
AVX afxError            AfxEnqueueTransferRequest(afxDrawBridge dxge);

#endif//AFX_DRAW_BRIDGE_H
