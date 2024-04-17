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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

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
    afxDrawBridgeFlag_DRAW      = AFX_BIT_OFFSET(0), // supports draw ops
  //afxDrawBridgeFlag_DRAW_AUX  = AFX_BIT_OFFSET(1), // supports auxiliary draw ops (but can't perform a entire draw pipeline execution)
    afxDrawBridgeFlag_COMPUTE   = AFX_BIT_OFFSET(2), // supports compute ops
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
    union
    {
        struct
        {
            afxRaster       ras;
            afxRasterRegion rgn;
            union
            {
                void*       dst;
                void const* src;
                afxBuffer   buf;
                afxStream   iob;
            };
            afxSize         offset; // offset into buffer data;
            afxNat          rowSiz; // bytes per row
            afxNat          rowCnt; // rows per layer
        }                   img;
        struct
        {
            afxBuffer       buf;
            afxSize         offset;
            afxNat          range;
            union
            {
                void*       dst;
                void const* src;
                afxStream   iob;
            };
            afxSize         dataOff;
        }                   data;
    };

    afxFcc              srcFcc;
    afxFcc              dstFcc;

    afxCodec            codec;
    afxNat              decompressedSiz;

    afxSemaphore        wait;
    afxSemaphore        signal;
};

#ifdef _AFX_DRAW_C
#ifdef _AFX_DRAW_BRIDGE_C
AFX_DEFINE_STRUCT(afxDrawQueue)
{
    afxBool             immediate; // 0 = deferred, 1 = immediate

    afxSlock            workArenaSlock;
    afxArena            workArena; // used by submission of queue operations, not stream commands.        
    afxChain            workChn;
    afxMutex            workChnMtx;
    afxMutex            idleCndMtx;
    afxCondition        idleCnd;

    // one command pool per queue to avoid thread interation.
    // one stream manager per queue to avoid thread interaction.

    afxManager          streams;
    afxQueue            recycQue;
    afxSlock            reqLock;
    afxBool             lockedForReq;
};
#ifndef _AFX_DRAW_BRIDGE_IMPL
AFX_OBJECT(afxDrawBridge)
#else
struct afxBaseDrawBridge
#endif
{
    afxLinkage          dctx; // owner
    afxNat              portIdx;
    afxChain            mgrChn;

    afxError            (*waitCb)(afxDrawBridge,afxNat);
    afxNat              (*executeCb)(afxDrawBridge,afxFence,afxNat,afxExecutionRequest const[]);
    afxNat              (*transferCb)(afxDrawBridge, afxFence, afxNat, afxTransferRequest const[]);
    afxNat              (*presentCb)(afxDrawBridge,afxNat,afxPresentationRequest const[]);
    afxNat              (*stampCb)(afxDrawBridge,afxNat,afxPresentationRequest const[],afxV2d const,afxString const*);

    afxNat              queCnt;
    afxDrawQueue*       queues;
};
#endif
#endif

AVX afxDrawDevice       AfxGetDrawBridgeDevice(afxDrawBridge ddge);
AVX afxDrawContext      AfxGetDrawBridgeContext(afxDrawBridge ddge);

AVX afxError            AfxWaitForIdleDrawQueue
/// Wait for a queue to become idle. To wait on the host for the completion of outstanding queue operations for a given queue.
(
    afxDrawBridge       ddge, /// the bridge on which to wait.
    afxNat              queIdx /// the queue on which to wait.
);

AVX afxNat              AfxEnqueueExecutionRequest(afxDrawBridge ddge, afxFence fenc, afxNat cnt, afxExecutionRequest const req[]);
AVX afxNat              AfxEnqueueTransferRequest(afxDrawBridge ddge, afxFence fenc, afxNat cnt, afxTransferRequest const req[]);
AVX afxNat              AfxEnqueuePresentRequest(afxDrawBridge ddge, afxNat cnt, afxPresentationRequest const req[]);
AVX afxNat              AfxEnqueueStampRequest(afxDrawBridge ddge, afxNat cnt, afxPresentationRequest const req[], afxV2d const origin, afxString const* caption);

#endif//AFX_DRAW_BRIDGE_H
